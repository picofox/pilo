#include "./formatted_io.hpp"

namespace pilo
{
    namespace core
    {
        namespace io
        {

            ::pilo::i64_t file_formatted_output(FILE* fp, const char* fmt, ...)
            {
                int n = 0;
                va_list args;
                va_start(args, fmt);
#               if defined(WINDOWS)
                n = vfprintf_s(fp, fmt, args);
#               else
                n = vfprintf(fp, fmt, args);
#               endif
                va_end(args);


                if (n < 0)
                {
                    return -1;
                }

                return (size_t)n;
            }

            ::pilo::i64_t file_formatted_output(FILE* fp, const wchar_t* fmt, ...)
            {
                int n = 0;
                va_list args;
                va_start(args, fmt);
#               if defined(WINDOWS)
                n = vfwprintf_s(fp, fmt, args);
#               else
                n = vfwprintf(fp, fmt, args);
#               endif
                va_end(args);

                if (n < 0)
                {
                    return -1;
                }

                return (size_t)n;
            }

            ::pilo::i64_t string_formated_output(char* buf, ::pilo::i64_t len, const char* fmt, ...)
            {
                va_list args;
                int ret;

                va_start(args, fmt);
#               if defined(WINDOWS)
                ret = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
#               else
                ret = vsnprintf(buf, len, fmt, args);
#               endif
                va_end(args);

                if (ret < 0)
                {
                    return -1;
                }

                return (size_t)ret;
            }

            ::pilo::i64_t string_formated_output(wchar_t* buf, ::pilo::i64_t len, const wchar_t* fmt, ...)
            {
                va_list args;
                int ret;

                va_start(args, fmt);
#               if defined(WINDOWS)
                ret = _vsnwprintf_s(buf, len, _TRUNCATE, fmt, args);
#               else
                ret = vswprintf(buf, len, fmt, args);
#               endif
                va_end(args);

                if (ret < 0)
                {
                    return -1;
                }

                return (size_t)ret;
            }

        }
    }
}