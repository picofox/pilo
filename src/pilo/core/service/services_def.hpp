#ifndef _pilo_core_service_service_def_hpp
#define _pilo_core_service_service_def_hpp

#include "../../pilo.hpp"

#define	PMI_BUILTIN_SERVICE_TIMER		((::pilo::service_group_id)0)
#define PMS_BUILTIN_SERVICE_TIMER		("timer")

namespace pilo
{
	namespace core
	{
		namespace service
		{
			const char* service_id_to_name(::pilo::service_group_id id);
			::pilo::service_group_id service_name_to_id(const char* name);
		}
	}
}




#endif // !_pilo_core_service_service_def_hpp
