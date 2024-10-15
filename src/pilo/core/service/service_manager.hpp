#ifndef _pilo_core_service_service_manager_hpp_
#define _pilo_core_service_service_manager_hpp_

#include "service_group_interface.hpp"
#include "../container/concurrent_queue.hpp"
#include <map>

namespace pilo
{
	namespace core
	{
		namespace service
		{
			class service_manager
			{
			public:
				friend class service_group_interface;				
				typedef service_group_interface* (*service_group_creation_func_type) (service_manager* mgr, const ::pilo::core::config::service_config* config);
				typedef service_interface* (*service_creation_func_type) (service_group_interface* igroup, ::pilo::i32_t index);

			public:
				service_manager() : _state(::pilo::core::service::service_state::stopped) {}
				~service_manager();

			public:
				::pilo::err_t initialize(const std::map<::pilo::service_group_id, ::pilo::core::config::service_config>* configs);
				::pilo::err_t add_service(service_interface* iservice);
				::pilo::err_t remove_service(::pilo::service_group_id sgid,  ::pilo::service_id sid);
				
				inline void schedule_service(::pilo::core::service::service_interface* iservice) {_sched_queue.enqueue(iservice);}

				
				::pilo::err_t start();
				::pilo::err_t stop();
				bool all_stopped() const;
				void pulse();

				service_group_interface* group(::pilo::service_group_id gid);
				service_interface* service(::pilo::service_group_id gid, ::pilo::service_id key_or_index);

				::pilo::err_t	register_service_creator(::pilo::service_group_id gid
					, service_group_creation_func_type svc_grp_crt_func
					,service_creation_func_type svc_crt_func);

			private:
				::pilo::err_t _add_group(service_group_interface* grp);


			private:
				::pilo::core::service::service_state							_state;				
				::pilo::core::container::concurrent_queue<service_interface*>	_sched_queue;
				std::map<::pilo::service_group_id, service_group_interface*>	_group;
				std::map<::pilo::service_group_id, service_creation_func_type>	_service_creator;
				std::map<::pilo::service_group_id, service_group_creation_func_type>	_service_group_creator;
			};

		}
	}
}



#endif