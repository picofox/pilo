#ifndef _pilo_core_sched_time_wheel_hpp_
#define _pilo_core_sched_time_wheel_hpp_

#include    "./timer.hpp"
#include	"../threading/native_mutex.hpp"
#include	"../container/singly_linked_selflist.hpp"
#include	"../datetime/timestamp.hpp"
#include	"../threading/dummy_mutex.hpp"
#include	"../process/context.hpp"
#include	"../logging/logger_interface.hpp"

namespace pilo
{
	namespace core
	{
		namespace sched
		{			
			template<typename LOCK =  ::pilo::core::threading::dummy_mutex>
			class time_wheel
			{
			public:
				const static ::pilo::i32_t TIME_NEAR_SHIFT = 8;
				const static ::pilo::i32_t TIME_NEAR = 1 << 8;
				const static ::pilo::i32_t TIME_LEVEL_SHIFT = 6;
				const static ::pilo::i32_t TIME_LEVEL = 1 << 6;
				const static ::pilo::i32_t TIME_NEAR_MASK = TIME_NEAR - 1;
				const static ::pilo::i32_t TIME_LEVEL_MASK = TIME_LEVEL - 1;
				typedef LOCK  lock_type;

			public:
				time_wheel(::pilo::i64_t unit_in_millisecs) : _time(0), _start_time(0), _current(0), _current_point(0), _unit_in_millsecs(unit_in_millisecs) 
				{					
				}

				~time_wheel() 
				{
				}

				timer* add_timer(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, ::pilo::i64_t unit_in_millsecs,  task_func_type f_func, void* obj, void* param, task_destructor_func_type dtor)
				{
					::pilo::core::sched::timer* ptimer = PILO_CONTEXT->allocate_timer();
					if (ptimer != nullptr) {
						ptimer->set(duration, rep_cnt, rep_dura, unit_in_millsecs,f_func, obj, param, dtor);
					}
					else {
						return nullptr;
					}

					std::lock_guard<lock_type>	guard(this->_mutex);
					ptimer->set_expire(duration + this->_time);
					this->_add_node(ptimer);
					return ptimer;
				}

				void add_timer(::pilo::core::sched::timer* ptimer)
				{
					std::lock_guard<lock_type>	guard(this->_mutex);
					ptimer->add_expire(this->_time);
					this->_add_node(ptimer);
				}

				::pilo::err_t initialize()
				{
					::pilo::u32_t cur = 0;
					this->_get_sys_time(&this->_start_time, &cur);
					this->_current = cur;
					this->_current_point = this->_mono_time();
					return PILO_OK;
				}

				void update()
				{
					::pilo::i64_t ts = this->_mono_time();
					if (ts < this->_current_point) {
						PLOG(::pilo::core::logging::level::warn, "[time-wheel] time diff error: change from %lld to %lld", ts, this->_current_point);
						this->_current_point = ts;
					}
					else if (ts != this->_current_point) {
						::pilo::u32_t diff = (::pilo::u32_t)(ts - this->_current_point);
						this->_current_point = ts;
						this->_current += diff;
						for (::pilo::u32_t i = 0; i < diff; i++) {
							this->_update();
						}
					}
				}

			private:
				inline ::pilo::i64_t _mono_time()
				{
					::pilo::i64_t ts = ::pilo::core::datetime::timestamp_milli_steady();
					return ts / _unit_in_millsecs;
				}

				inline void _get_sys_time(::pilo::u32_t* sec, ::pilo::u32_t *cs)
				{
					::pilo::i64_t ts = ::pilo::core::datetime::timestamp_milli_system();
					*sec = (::pilo::u32_t)(ts / 1000000000);
					*cs = (::pilo::u32_t)(ts - (ts % 1000000000)) ;

				}

				void _add_node(::pilo::core::sched::timer* ptimer)
				{
					::pilo::u32_t time_exp = ptimer->expire();
					::pilo::u32_t cur_time = this->_time;

					if ((time_exp | TIME_NEAR_MASK) == (cur_time | TIME_NEAR_MASK)) {
						this->_near[time_exp & TIME_NEAR_MASK].link(ptimer);
					}
					else {
						::pilo::i32_t i = 0;
						::pilo::u32_t mask = TIME_NEAR << TIME_LEVEL_SHIFT;
						for (i = 0; i < 3; i++) {
							if ((time_exp | (mask - 1)) == (cur_time | (mask - 1))) {
								break;
							}
							mask <<= TIME_LEVEL_SHIFT;
						}
						::pilo::i32_t idx = ((time_exp >> (TIME_NEAR_SHIFT + i * TIME_LEVEL_SHIFT))) & TIME_LEVEL_MASK;
						this->_t[i][idx].link(ptimer);
					}
				}

				void _move_list(::pilo::u32_t level, ::pilo::u32_t idx)
				{
					::pilo::core::sched::timer* current_timer = this->_t[level][idx].clear();
					while (current_timer != nullptr) {
						::pilo::core::sched::timer* tmp_timer = current_timer->next();
						this->_add_node(current_timer);
						current_timer = tmp_timer;
					}
				}
					
				void _shift()
				{
					int mask = TIME_NEAR;
					::pilo::u32_t ct = ++ (this->_time);
					if (ct == 0) {
						this->_move_list(3, 0);
					}
					else {
						uint32_t time = ct >> TIME_NEAR_SHIFT;
						::pilo::u32_t i = 0;
						while ((ct & (mask - 1)) == 0) {
							::pilo::u32_t idx = time & TIME_LEVEL_MASK;
							if (idx != 0) {
								this->_move_list(i, idx);
								break;
							}
							mask <<= TIME_LEVEL_SHIFT;
							time >>= TIME_LEVEL_SHIFT;
							++i;
						}
					}
				}

				void _dispatch_list(::pilo::core::sched::timer* current_timer)
				{
					//while (true) {
					//	current_timer->exec();
					//	bool resched = current_timer->resched_check(this->_time);
					//	if (resched) {

					//	}
					//}

					::pilo::core::sched::timer* tmp_timer = nullptr;
					do {
						current_timer->exec();
						bool resched = current_timer->resched_check(this->_time);
						if (resched) {
							tmp_timer = current_timer;
							current_timer = current_timer->next();
							tmp_timer->set_next(nullptr);
							{
								std::lock_guard<lock_type>	guard(this->_mutex);								
								this->_add_node(tmp_timer);
							}		
						}
						else {
							tmp_timer = current_timer;
							current_timer = current_timer->next();
							PILO_CONTEXT->deallocate_timer(tmp_timer);
						}

					} while (current_timer != nullptr);
				}

				void _execute()
				{
					int idx = this->_time & TIME_NEAR_MASK;
					while (this->_near[idx].head()->next() != nullptr) {
						::pilo::core::sched::timer* current = this->_near[idx].clear();
						_mutex.unlock();
						// dispatch_list don't need lock T
						this->_dispatch_list(current);
						_mutex.lock();
					}
				}

				void _update()
				{
					std::lock_guard<lock_type>	guard(this->_mutex);
					_execute();
					_shift();
					_execute();
				}
				 

			private:
				::pilo::u32_t									_time;
				::pilo::u32_t									_start_time;
				::pilo::i64_t									_current;
				::pilo::i64_t									_current_point;
				const ::pilo::i64_t								_unit_in_millsecs;			
				::pilo::core::sched::timer_linked_list			_near[TIME_NEAR];
				::pilo::core::sched::timer_linked_list			_t[4][TIME_LEVEL];
				lock_type										_mutex;

			private:
				M_DISABLE_COPY(time_wheel)

			};
			
		}
	}
}

#endif