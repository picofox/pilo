#include "./timer_service.hpp"
#include "../../logging/logger_interface.hpp"
#include "../../process/context.hpp"

namespace pilo
{
	namespace core
	{
		namespace service
		{
			namespace builtin
			{
				::pilo::err_t timer_service::start(::pilo::i64_t now_ts)
				{
					this->_time_wheel_millisec.initialize();
					return abstract_fixed_interval_service::start(now_ts);
				}
				void timer_service::pulse(::pilo::i64_t now_ts)
				{
					PLOG(::pilo::core::logging::level::info, "%lld", now_ts);
					this->_time_wheel_millisec.update();
				}
			}
		}
	}
}



