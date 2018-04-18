#pragma once

#include "../../core/coredefs.hpp"
#include <cstdarg>

namespace pilo
{
    namespace core
    {
        namespace io
        {
            template <size_t MaxBufferSize>
            size_t console_format_output(const char * fmt, ...);
            size_t console_format_output(const char * fmt, ...);
            template <size_t MaxBufferSize>
            size_t console_format_output(const wchar_t * fmt, ...);
            size_t console_format_output(const wchar_t * fmt, ...);      

            size_t string_format_output(char * buf, size_t len, const char * fmt, ...);
            size_t string_format_output(wchar_t * buf, size_t len, const wchar_t * fmt, ...);            

            size_t _va_arg_format(char * buf, size_t len, const char * fmt, va_list args);
            size_t _va_arg_format(wchar_t * buf, size_t len, const wchar_t * fmt, va_list args);

            //implementation
            template <size_t MaxBufferSize>
            size_t console_format_output(const char * fmt , ...)
            {
                char _buffer[MaxBufferSize];

                va_list args;
                va_start(args, fmt);
#               if defined(WINDOWS)
                _vsnprintf_s(_buffer, sizeof(_buffer), _TRUNCATE, fmt, args);
#               else
                vsnprintf(_buffer, sizeof(_buffer), fmt, args);
#               endif
                va_end(args);

#               if defined(WINDOWS)
                int ret = printf_s("%s", sizeof(_buffer), _buffer);
#               else
                int ret = printf("%s", _buffer);
#               endif

                if (ret < 0) 
                {
                    return MC_INVALID_SIZE;
                }

                return static_cast<size_t>(ret);
            }           

            template <size_t MaxBufferSize>
            size_t console_format_output(const wchar_t * fmt , ...)
            {
                wchar_t _buffer[MaxBufferSize];

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
        }
    }
}