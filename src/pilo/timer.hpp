#ifndef _pilo_timer_hpp_
#define _pilo_timer_hpp_

#include    "./task.hpp"


namespace pilo
{
	class timer
	{
	public:
		const static ::pilo::i64_t invalid_timer_id = 0L;

	public:
		timer()
			:_m_id(invalid_timer_id), _m_duration(0), _m_expire(0), _m_repeat_count(0), _m_repeat_duration(0), _m_task(nullptr)
		{

		}

		~timer()
		{
			reset();
		}

		::pilo::err_t set(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, task_func_type f_func, void* obj, ::pilo::tlv* param, task_destructor_func_type dtor);
		::pilo::err_t reset();
		inline bool invalid() const
		{
			return (_m_id == invalid_timer_id || _m_task == nullptr);
		}

		
		

		




	protected:
		::pilo::i64_t		_m_id;
		::pilo::u32_t		_m_duration;
		::pilo::u32_t		_m_expire;
		::pilo::u32_t		_m_repeat_count;
		::pilo::u32_t		_m_repeat_duration;
		::pilo::task*		_m_task;
	};
}


#endif