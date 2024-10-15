﻿#ifndef _pilo_core_sched_timer_hpp_
#define _pilo_core_sched_timer_hpp_

#include    "./task.hpp"


namespace pilo
{
	namespace core
	{
		namespace sched
		{
			class timer
			{
			public:
				const static ::pilo::i64_t invalid_timer_id = 0L;
				const static ::pilo::u32_t infinit_repeat = 0xFFFFFFFFU;

			public:
				timer()
					: _m_next(nullptr)
					,_m_id_n_cancel_flag(invalid_timer_id), _m_duration(0), _m_expire(0), _m_repeat_count(0), _m_repeat_duration(0), _m_task(nullptr)
				{

				}

				~timer()
				{
					reset();
				}

				::pilo::err_t set(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, task_func_type f_func, void* obj, void* param, task_destructor_func_type dtor);
				void reset();

				inline bool invalid() const
				{
					return (_m_id_n_cancel_flag == invalid_timer_id || _m_task == nullptr);
				}

				inline ::pilo::i64_t id()
				{
					return (_m_id_n_cancel_flag & 0x7FFFFFFFFFFFFFFFLL);
				}

				inline bool cancelled() const
				{
					return (_m_id_n_cancel_flag & 0x8000000000000000LL);
				}

				inline void cancel()
				{
					_m_id_n_cancel_flag |= 0x8000000000000000LL;
				}

				inline ::pilo::core::sched::task* task()
				{
					return _m_task;
				}

				inline ::pilo::u32_t duration() const
				{
					return _m_duration;
				}

				inline ::pilo::u32_t expire() const
				{
					return _m_expire;
				}

				inline void set_expire(::pilo::u32_t exp)
				{
					this->_m_expire = exp;
				}

				inline ::pilo::u32_t repeat_count() const
				{
					return _m_repeat_count;
				}

				inline ::pilo::u32_t repeat_duration() const
				{
					return _m_repeat_duration;
				}

				inline void exec()
				{
					if (!cancelled() && _m_task != nullptr) {
						_m_task->exec();
					}
				}

				inline bool resched_check(::pilo::u32_t time_slot)
				{
					if (this->_m_repeat_count == infinit_repeat) {
						this->_m_expire = this->_m_repeat_duration + time_slot;
						return true;
					}
					else if (this->_m_repeat_count == 0) {
						return false;
					}
					this->_m_repeat_count--;
					if (this->_m_repeat_count > 0) {
						this->_m_expire = this->_m_repeat_duration + time_slot;
						return true;
					}
					return false;
				}

				inline timer* next() { return _m_next;  }
				inline void set_next(timer* nptr) { _m_next = nptr; }

			protected:				
				::pilo::core::sched::timer*		_m_next;
				::pilo::i64_t		_m_id_n_cancel_flag;
				::pilo::u32_t		_m_duration;
				::pilo::u32_t		_m_expire;
				::pilo::u32_t		_m_repeat_count;
				::pilo::u32_t		_m_repeat_duration;
				::pilo::core::sched::task* _m_task;
			};

			class timer_linked_list
			{
			public:
				timer_linked_list() : _tail_ptr(&_head) 
				{
					_head.set_next(nullptr);
				}

				inline ::pilo::core::sched::timer* tail() { return _tail_ptr; }
				inline ::pilo::core::sched::timer* head() { return &_head; }
				inline void link(::pilo::core::sched::timer* t)
				{
					this->_tail_ptr->set_next(t);
					this->_tail_ptr = t;
					t->set_next(nullptr);
				}
				inline ::pilo::core::sched::timer* clear()
				{
					::pilo::core::sched::timer* t = this->_head.next();
					this->_head.set_next(nullptr);
					this->_tail_ptr = &_head;
					return t;
				}

			private:
				class ::pilo::core::sched::timer* _tail_ptr;
				class timer _head;
				
			};
		}
	}	
}


#endif