#include "./abstract_fixed_interval_service.hpp"
#include "../datetime/timestamp.hpp"
#include "./service_group_interface.hpp"
#include "../logging/logger_interface.hpp"
#include "../process/context.hpp"


namespace pilo
{
	namespace core
	{
		namespace service
		{
			abstract_fixed_interval_service::abstract_fixed_interval_service(service_group_interface* grp, ::pilo::service_id id)
				: service_interface(grp, id)
			{
				_pulse_usec = grp->config()->pulse_usec();
			}


			void abstract_fixed_interval_service::check_pulse(::pilo::i64_t now_ts)
			{				
				::pilo::i64_t dist = now_ts - _m_last_pulse;
				if (dist >= this->_pulse_usec) {
					this->pulse(now_ts);
					_m_last_pulse = PILO_TIMESTAMP;
				}

			}

			void abstract_fixed_interval_service::set_pulse(::pilo::i64_t i)
			{
				_pulse_usec = i;
			}

			abstract_fixed_interval_service::~abstract_fixed_interval_service()
			{
				stop(-1);
			}

		}
	}
}