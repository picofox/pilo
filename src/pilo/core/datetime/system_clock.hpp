#ifndef _pilo_core_datetime_system_clock_hpp_
#define _pilo_core_datetime_system_clock_hpp_


#include	<chrono>
#include	"../../pilo.hpp"

namespace pilo {
	namespace core {
		namespace datetime {

			typedef std::chrono::system_clock				sysclock;
			typedef std::chrono::system_clock::time_point	systime;

			const ::pilo::i8_t system_timezone = 127;

			class system_clock
			{
			public:
				system_clock(::pilo::i8_t tz) : _m_tz(tz)
				{

				}


				inline static systime s_now()
				{
					return std::chrono::system_clock::now();
				}

				static std::string format_to_string(const systime& t, const char* fmt, ::pilo::i8_t tz = system_timezone);
				static std::string format_to_string(time_t t,const char* fmt, ::pilo::i8_t tz = system_timezone);
				



			public:
				::pilo::i8_t tz() const { return _m_tz;  }
				void update() { _m_current = std::chrono::system_clock::now();  }
				

			protected:
				systime		_m_current;
				::pilo::i8_t	_m_tz;

			};


		}
	}
}


#endif