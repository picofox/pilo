#include "format_output.hpp"

using namespace pilo;
namespace pilo
{
    namespace core
    {
        namespace io
        {
            size_t console_format_output(const char * fmt, ...)
            {
                char _buffer[MC_FORMAT_OUTPUT_BUFFER_DEFAULT_SIZE];

                va_list args;
                va_start(args, fmt);
#               if defined(WINDOWS)
                _vsnprintf_s(_buffer, sizeof(_buffer), _TRUNCATE, fmt, args);
#               else
                vsnprintf(_buffer, sizeof(_buffer), fmt, args);
#               endif
                va_end(args);

#               if defined(WINDOWS)
                int ret = printf("%s", _buffer);
#               else
                int ret = printf("%s", _buffer);
#               endif

                if (ret < 0)
                {
                    return MC_INVALID_SIZE;
                }

                return static_cast<size_t>(ret);
            }

            size_t console_format_output(const wchar_t * fmt, ...)
            {
                wchar_t _buffer[MC_FORMAT_OUTPUT_BUFFER_DEFAULT_SIZE];

                va_list args;
                va_start(args, fmt);
#               if  defined(WINDOWS)
                _vsnwprintf_s(_buffer, sizeof(_buffer) / sizeof(wchar_t), _TRUNCATE, fmt, args);

#               else
                vswprintf(_buffer, sizeof(_buffer) / sizeof(wchar_t), fmt, args);
#               endif
                va_end(args);


#               if defined(WINDOWS)
                int ret = wprintf_s(_buffer);
#               else
                int ret = wprintf(L"%ls", _buffer);
#               endif

                if (ret < 0)
                {
                    return MC_INVALID_SIZE;
                }

                return static_cast<size_t>(ret);
            }

            size_t string_format_output(char * buf, size_t len, const char * fmt, ...)
            {
                va_list args;
                pilo::i32_t ret;

                va_start(args, fmt);
#               if defined(WINDOWS)
                ret = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
#               else
                ret = vsnprintf(buf, len, fmt, args);
#               endif
                va_end(args);

                if (ret < 0) 
                {
                    return MC_INVALID_SIZE;
                }

                return static_cast<size_t>(ret);
            }

            size_t string_format_output(wchar_t * buf, size_t len, const wchar_t * fmt, ...)
            {
                va_list args;
                pilo::i32_t ret;

                va_start(args, fmt);
#               if defined(WINDOWS)
                ret = _vsnwprintf_s(buf, len, _TRUNCATE, fmt, args);
#               else
                ret = vswprintf(buf, len, fmt, args);
#               endif
                va_end(args);

                if (ret < 0) 
                {
                    return MC_INVALID_SIZE;
                }

                return static_cast<size_t>(ret);
            }

            size_t _va_arg_format(char * buf, size_t len, const char * fmt, va_list args)
            {
                i32_t ret;
#               if defined(WINDOWS) || defined(WIN32)
                ret = _vsnprintf_s(buf, len, _TRUNCATE, fmt, args);
#               else
                ret = vsnprintf(buf, len, fmt, args);

#	            endif

                if (ret < 0) {
                    return MC_INVALID_SIZE;
                }

                return static_cast<size_t>(ret);
            }

            size_t _va_arg_format(wchar_t * buf, size_t len, const wchar_t * fmt, va_list args)
            {
                i32_t  ret;
#               if defined(WINDOWS) || defined(WIN32)
                ret = _vsnwprintf_s(buf, len, _TRUNCATE, fmt, args);
#               else
                ret = vswprintf(buf, len, fmt, args);
#	            endif

                if (ret < 0) {
                    return MC_INVALID_SIZE;
                }

                return static_cast<size_t>(ret);
            }

        }
    }
}