#ifndef _pilo_core_service_abstract_compensable_fixed_frequency_service_hpp_
#define _pilo_core_service_abstract_compensable_fixed_frequency_service_hpp_

#include "./service_interface.hpp"
#include "../config/service_config.hpp"


namespace pilo
{
	namespace core
	{
		namespace service
		{
			class abstract_compensable_fixed_frequency_service : public service_interface
			{
			public:
				abstract_compensable_fixed_frequency_service(service_group_interface* grp, ::pilo::service_id id);

			public:
				// Inherited via service_interface
				void check_pulse(::pilo::i64_t now_ts) override;
				void set_pulse(::pilo::i64_t i) override;
				~abstract_compensable_fixed_frequency_service();


			private:
				::pilo::i64_t	_pulse_usec;

			};
		}
	}
}

#endif