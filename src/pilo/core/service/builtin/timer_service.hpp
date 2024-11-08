#ifndef _pilo_core_service_builtin_timer_service_hpp_
#define _pilo_core_service_builtin_timer_service_hpp_

#include "../../sched/time_wheel.hpp"
#include "../../service/abstract_fixed_interval_service.hpp"
#include "../../service/services_def.hpp"
#include "../../container/concurrent_queue.hpp"


namespace pilo
{
	namespace core
	{
		namespace service
		{
			namespace builtin
			{
				class timer_service_group;

				class timer_service : public ::pilo::core::service::abstract_fixed_interval_service
				{
				public:
					timer_service(::pilo::core::service::service_group_interface* grp)
						: abstract_fixed_interval_service(grp, 0)
						, _time_wheel_sec(1000)
						, _time_wheel_millisec(10) 
						, _last_sec_timer_ts(0)
					{
						
					}

				public:
					void pulse(::pilo::i64_t now_ts) override;
					::pilo::err_t start(::pilo::i64_t now_ts);

					::pilo::i64_t add_rel_milli_timer(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura
						, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor);

					::pilo::i64_t add_rel_sec_timer(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura
						, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor);

					::pilo::i64_t add_abs_milli_timer(::pilo::i64_t epoch, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura
						, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor);

					::pilo::i64_t add_abs_sec_timer(::pilo::i64_t epoch, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura
						, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor);

					void delete_timer(::pilo::i64_t timer);

				private:
					::pilo::i64_t _last_sec_timer_ts;
					::pilo::core::sched::time_wheel<::pilo::core::threading::dummy_mutex>	_time_wheel_sec;
					::pilo::core::sched::time_wheel<::pilo::core::threading::dummy_mutex>	_time_wheel_millisec;
					::pilo::core::container::concurrent_queue<::pilo::core::sched::timer*>	_timer_queue;
				};

			}
		}
	}
}









#endif