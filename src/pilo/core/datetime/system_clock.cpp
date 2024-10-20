#include "system_clock.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>

namespace pilo {
	namespace core {
		namespace datetime {

			std::string system_clock::format_to_string(const systime& t, const char* fmt, ::pilo::i8_t tz)
			{
				std::time_t secs = std::chrono::system_clock::to_time_t(t);
				return format_to_string(secs, fmt, tz);
			}


			std::string system_clock::format_to_string(time_t t, const char* fmt, ::pilo::i8_t tz)
			{
				if (fmt == nullptr)
					fmt = "%Y-%m-%d %H:%M:%S";
				std::stringstream ss;
				struct tm tms;
				localtime_s(&tms, &t);
				ss << std::put_time(&tms, fmt);
				return ss.str();
			}
		}
	}
}