#include "system_clock.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>

namespace pilo {
	namespace core {
		namespace datetime {

			std::string system_clock::format_to_string(const systime& t, const char* fmt)
			{
				if (fmt == nullptr)
					fmt = "%Y-%m-%d %H:%M:%S";
				std::stringstream ss;
				std::time_t secs = std::chrono::system_clock::to_time_t(t);
				struct tm tms;
				localtime_s(&tms, &secs);
				ss << std::put_time(&tms, fmt);
				return ss.str();
			}
		}
	}
}