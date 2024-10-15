#include "./abstract_compensable_fixed_frequency_service.hpp"
#include "../datetime/timestamp.hpp"
#include "./service_group_interface.hpp"

namespace pilo
{
	namespace core
	{
		namespace service
		{
			void abstract_compensable_fixed_frequency_service::check_pulse(::pilo::i64_t now_ts)
			{
				::pilo::i64_t dist = now_ts - _m_last_pulse;
				::pilo::i64_t cnt = dist / _pulse_msec;

				for (::pilo::i64_t i = 0; i < cnt; i++) {
					_m_last_pulse = PILO_TIMESTAMP;
					this->pulse(now_ts + (i * this->_pulse_msec)); 
				}
			}

			abstract_compensable_fixed_frequency_service::~abstract_compensable_fixed_frequency_service()
			{
				stop(-1);
			}
			void abstract_compensable_fixed_frequency_service::set_pulse(::pilo::i64_t i)
			{
				_pulse_msec = i;
			}

			abstract_compensable_fixed_frequency_service::abstract_compensable_fixed_frequency_service(service_group_interface* grp, ::pilo::service_id id)
				: service_interface(grp, id)
			{
				_pulse_msec = grp->config()->pulse_msec();
			}

		}
	}
} 