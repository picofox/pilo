#ifndef _pilo_core_service_builtin_timer_service_hpp_
#define _pilo_core_service_builtin_timer_service_hpp_

#include "../../sched/time_wheel.hpp"
#include "../../service/abstract_fixed_interval_service.hpp"
#include "../../service/services_def.hpp"

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

					::pilo::err_t start(::pilo::i64_t now_ts);


				private:
					::pilo::core::sched::time_wheel<::pilo::core::threading::dummy_mutex>	_time_wheel_sec;
					::pilo::core::sched::time_wheel<::pilo::core::threading::dummy_mutex>	_time_wheel_millisec;



					// Inherited via abstract_fixed_interval_service
					void pulse(::pilo::i64_t now_ts) override;

				private:
					::pilo::i64_t _last_sec_timer_ts;

				};

			}
		}
	}
}









#endif