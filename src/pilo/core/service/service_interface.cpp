#include "./service_interface.hpp"
#include "service_interface.hpp"

#include "./service_group_interface.hpp"

namespace pilo
{
	namespace core
	{
		namespace service
		{
			::pilo::service_group_id core::service::service_interface::group_id() const
			{
				return _m_group_ref->id();
			}
		}
	}
}

