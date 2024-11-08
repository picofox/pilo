#include "./timer_service.hpp"
#include "../../logging/logger_interface.hpp"
#include "../../process/context.hpp"

namespace pilo
{
	namespace core
	{
		namespace service
		{
			namespace builtin
			{
				::pilo::err_t timer_service::start(::pilo::i64_t now_ts)
				{
					::pilo::err_t err = PILO_OK;
					err = this->_time_wheel_millisec.initialize();
					if (err != PILO_OK) {
						return err;
					}
					err = this->_time_wheel_sec.initialize();
					if (err != PILO_OK) {
						return err;
					}
					this->_m_group_ref->services()->_timer_service = this;
					return abstract_fixed_interval_service::start(now_ts);
				}

				::pilo::i64_t timer_service::add_rel_milli_timer(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor)
				{
					::pilo::core::sched::timer* ptimer = PILO_CONTEXT->allocate_timer();
					if (ptimer != nullptr) {
						ptimer->set(duration / 10, rep_cnt, rep_dura / 10, 10, f_func, obj, param, dtor);
					}
					else
						return ::pilo::core::sched::timer::invalid_timer_id;
					this->_timer_queue.enqueue(ptimer);
					return ptimer->id();
				}

				::pilo::i64_t timer_service::add_rel_sec_timer(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor)
				{
					::pilo::core::sched::timer* ptimer = PILO_CONTEXT->allocate_timer();
					if (ptimer != nullptr) {
						ptimer->set(duration , rep_cnt, rep_dura , 1000, f_func, obj, param, dtor);
					}
					else
						return ::pilo::core::sched::timer::invalid_timer_id;
					this->_timer_queue.enqueue(ptimer);
					return ptimer->id();
				}

				::pilo::i64_t timer_service::add_abs_milli_timer(::pilo::i64_t epoch, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor)
				{
					::pilo::i64_t diff = epoch - ::pilo::core::datetime::timestamp_milli_system();
					if (diff < 0)
						diff = 0;
					return add_rel_milli_timer((::pilo::u32_t)diff, rep_cnt, rep_dura, f_func, obj, param, dtor);
				}

				::pilo::i64_t timer_service::add_abs_sec_timer(::pilo::i64_t epoch, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor)
				{
					::pilo::i64_t diff = (epoch*1000 - ::pilo::core::datetime::timestamp_milli_system()) / 1000;
					if (diff < 0)
						diff = 0;
					return add_rel_sec_timer((::pilo::u32_t)diff, rep_cnt, rep_dura, f_func, obj, param, dtor);
				}

				void timer_service::delete_timer(::pilo::i64_t timer_id)
				{
					if (timer_id % 2 == 0) {
						this->_time_wheel_millisec.delete_timer(timer_id);
					}
					else {
						this->_time_wheel_sec.delete_timer(timer_id);
						
					}
				}



				void timer_service::pulse(::pilo::i64_t now_ts)
				{
					//PLOG(::pilo::core::logging::level::info, "%lld", now_ts);
					::pilo::core::sched::timer* ptimer = nullptr;
					bool has_timer = false;
					do {
						has_timer = _timer_queue.try_dequeue(ptimer);
						if (has_timer) {
							if (ptimer->unit_in_millsecs() == 10) {
								_time_wheel_millisec.add_timer(ptimer);
							}								
							else if (ptimer->unit_in_millsecs() == 1000) {
								_time_wheel_sec.add_timer(ptimer);
							}
							else {
								PMC_ASSERT(false);
								PLOG(::pilo::core::logging::level::debug, SP_PMS_LOGSVC_TIMER" Invalid timer unit_in_millsecs %lld.", ptimer->unit_in_millsecs());
								delete ptimer;
							}
						}

					} while (has_timer);

					this->_time_wheel_millisec.update();
					if (now_ts - this->_last_sec_timer_ts > 1000) {
						this->_time_wheel_sec.update();
						this->_last_sec_timer_ts = now_ts;
					}
				}
			}
		}
	}
}



