#include "./service_interface.hpp"
#include "service_interface.hpp"
#include "./service_manager.hpp"
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
			::pilo::err_t service_interface::start(::pilo::i64_t now_ts)
			{
				_m_last_pulse = now_ts;
				_m_group_ref->services()->schedule_service(this);
				return this->set_running();
			}
		}
	}
}

