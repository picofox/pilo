#ifndef _pilo_core_service_rac_service_group_hpp_
#define _pilo_core_service_rac_service_group_hpp_

#include "service_group_interface.hpp"
#include <string>
#include <deque>

namespace pilo
{
	namespace core
	{
		namespace service
		{
			class service_manager;

			class rac_service_group : public service_group_interface
			{		
			public:
				rac_service_group(::pilo::core::service::service_manager* mgr, const ::pilo::core::config::service_config* cfg)
					: service_group_interface(mgr, cfg)
				{
					
				}

				~rac_service_group();




				// Inherited via service_group_interface
				service_interface* service(::pilo::service_id key_or_index) override;

				::pilo::service_id add_service(service_interface* iservice) override;

				::pilo::err_t remove_service(::pilo::service_id sid) override;

				::pilo::err_t remove_all_service() override;

				::pilo::i32_t service_count() const override;

				::pilo::err_t iterate(service_iterate_func_type cb) override;

				::pilo::err_t start(::pilo::i64_t now_ts) override;

				::pilo::err_t stop(::pilo::i64_t now_ts) override;

				bool all_stopped() const override;

			protected:
				std::deque<service_interface*>		_services;

			};

		}
	}
}



#endif