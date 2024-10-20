#ifndef _pilo_core_datetime_system_clock_hpp_
#define _pilo_core_datetime_system_clock_hpp_


#include	<chrono>
#include	"../../pilo.hpp"

namespace pilo {
	namespace core {
		namespace datetime {

			typedef std::chrono::system_clock				sysclock;
			typedef std::chrono::system_clock::time_point	systime;
			

			class system_clock
			{
			public:
				inline static systime s_now()
				{
					return std::chrono::system_clock::now();
				}

				static std::string format_to_string(const systime& t, const char* fmt);



			public:
				void update() { _m_current = std::chrono::system_clock::now();  }
				

			protected:
				systime		_m_current;

			};


		}
	}
}


#endif