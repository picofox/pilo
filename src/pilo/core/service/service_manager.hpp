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

			public:
				service_manager() : _state(::pilo::core::service::service_state::stopped) {}
				~service_manager();

			public:
				::pilo::err_t add_service(service_interface* iservice);
				::pilo::err_t remove_service(::pilo::service_group_id sgid,  ::pilo::service_id sid);
				::pilo::err_t add_group(service_group_interface* grp);

				
				::pilo::err_t start();
				::pilo::err_t stop();
				bool all_stopped() const;
				void pulse();

				service_group_interface* group(::pilo::service_group_id gid);
				service_interface* service(::pilo::service_group_id gid, ::pilo::service_id key_or_index);

			private:
				::pilo::core::container::concurrent_queue<service_interface*>	_sched_queue;
				std::map<::pilo::service_group_id, service_group_interface*>	_group;
				::pilo::core::service::service_state							_state;
			};

		}
	}
}



#endif