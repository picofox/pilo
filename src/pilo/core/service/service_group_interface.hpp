#ifndef _pilo_core_service_service_group_interface_hpp_
#define _pilo_core_service_service_group_interface_hpp_

#include "../config/service_config.hpp"
#include "service_interface.hpp"
#include <string>

namespace pilo
{
	namespace core
	{
		namespace service
		{
			class service_manager;
			class service_group_interface;

			typedef ::pilo::err_t (*service_iterate_func_type)(::pilo::service_id key_or_index, service_interface* iservice, service_group_interface* ctx);

			class service_group_interface
			{		
			public:
				service_group_interface(service_manager* mgr, const ::pilo::core::config::service_config* cfg) :_m_mgr_ref(mgr), _config(cfg)
				{
					_m_id = (::pilo::service_group_id) cfg->type_id();
				}

				virtual ~service_group_interface() {};
				virtual service_interface* service(::pilo::service_id key_or_index) = 0;
				virtual ::pilo::service_id add_service(service_interface* iservice) = 0;
				virtual ::pilo::err_t remove_service(::pilo::service_id sid) = 0;
				virtual ::pilo::err_t remove_all_service() = 0;
				virtual ::pilo::i32_t service_count() const = 0;
				virtual ::pilo::err_t iterate(service_iterate_func_type cb) = 0;
				virtual ::pilo::err_t start(::pilo::i64_t now_ts) = 0;
				virtual ::pilo::err_t stop(::pilo::i64_t now_ts) = 0;
				virtual bool all_stopped() const = 0;


			public:
				inline service_manager*  services() { return _m_mgr_ref;  }
				inline ::pilo::service_group_id id() const { return _m_id;  }

			protected:
				::pilo::core::service::service_manager* const			_m_mgr_ref;
				const ::pilo::core::config::service_config * const		_config;	
				::pilo::service_group_id								_m_id;
			};

		}
	}
}



#endif