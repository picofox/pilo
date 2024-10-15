#include    "./timer.hpp"
#include	"../process/context.hpp"
#include	"../algorithm/uint_sequence_generator.hpp"

namespace pilo
{
	namespace core
	{
		namespace sched
		{
		
			static ::pilo::core::algorithm::uint_sequence_generator<::pilo::i64_t>	s_timer_id_generator(::pilo::core::sched::timer::invalid_timer_id);

			::pilo::err_t timer::set(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, task_func_type f_func, void* obj, void* param, task_destructor_func_type dtor)
			{				
				if (_m_task != nullptr)
				{
					PILO_CONTEXT->deallocate_task(_m_task);
					_m_task = nullptr;
				}
				_m_next = nullptr;
				_m_id_n_cancel_flag = s_timer_id_generator.next();
				_m_duration = duration;
				_m_expire = 0;
				_m_repeat_count = rep_cnt;
				_m_repeat_duration = rep_dura;
				_m_task = PILO_CONTEXT->allocate_task();
				if (_m_task == nullptr) {
					return ::pilo::mk_err(PERR_CREATE_OBJ_FAIL);
				}
				_m_task->set(f_func, obj, param, this, dtor);
				return PILO_OK;
			}

			void timer::reset()
			{
				_m_next = nullptr;
				_m_id_n_cancel_flag = invalid_timer_id;
				if (_m_task != nullptr)
				{
					PILO_CONTEXT->deallocate_task(_m_task);
					_m_task = nullptr;
				}
			}
		}
	}	
}


