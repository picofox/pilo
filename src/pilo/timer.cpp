#include    "./timer.hpp"
#include	"./core/process/context.hpp"

namespace pilo
{
	::pilo::err_t timer::set(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, task_func_type f_func, void* obj, ::pilo::tlv* param, task_destructor_func_type dtor)
	{
		_m_id = 0;
		_m_duration = duration;
		_m_expire = 0;
		_m_repeat_count = rep_cnt;
		_m_repeat_duration = rep_dura;
		_m_task = PILO_CONTEXT->allocate_task();
		if (_m_task == nullptr) {
			return ::pilo::mk_err(PERR_CREATE_OBJ_FAIL);
		}
		_m_task->set(f_func, obj, param, dtor);
	}

	::pilo::err_t timer::reset()
	{
		_m_id = invalid_timer_id;
		if (_m_task != nullptr)
		{
			PILO_CONTEXT->deallocate_task(_m_task);
			_m_task = nullptr;
		}
	}
}


