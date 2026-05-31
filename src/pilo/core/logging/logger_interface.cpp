#include "./logger_interface.hpp"
#include <string>
#include "../io/formatted_io.hpp"


namespace pilo {
    namespace core {
        namespace logging {
            void buff_log(char* buf, int bfsz, const char* file, int line, const char* fmt, ...)
            {
                int ret;
                va_list ap;

                va_start(ap, fmt);
#if defined(WINDOWS)
                ret = _vsnprintf_s(buf, bfsz, _TRUNCATE, fmt, ap);
#else
                ret = vsnprintf(buf, bfsz, fmt, args);
#endif
                va_end(ap);

                int remain = bfsz - ret;
                if (ret < 0 || remain < 8) {
                    return;
                }

                ::pilo::core::io::string_formated_output(buf + ret, remain, " <- (%s:%d)", file, line);
            }

        }
    }
}


