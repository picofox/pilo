#ifndef _pilo_core_service_builtin_timer_service_group_hpp_
#define _pilo_core_service_builtin_timer_service_group_hpp_

#include "../../service/mono_service_group.hpp"
#include "./timer_service.hpp"

namespace pilo
{
	namespace core
	{
		namespace service
		{
			namespace builtin
			{
				class timer_service_group 
					: public ::pilo::core::service::mono_service_group
				{
				public:
					timer_service_group(::pilo::core::service::service_manager* mgr, const ::pilo::core::config::service_config* cfg)
						: ::pilo::core::service::mono_service_group(mgr, cfg)						
					{
						
					}

					::pilo::i16_t initial_service_count() const { return 1; }

				private:
					
				};

			}
		}
	}
}









#endif