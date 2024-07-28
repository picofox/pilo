#include "./formatted_io.hpp"
#include "../io/io.hpp"
#               if defined(WINDOWS)
#include "../memory/util.hpp"
#include "../string/string_operation.hpp"
#endif



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

                return (::pilo::i64_t)n;
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

                return (::pilo::i64_t)n;
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
                if (ret < 0 || ret + 1 > len) {
                    return -1
                }
#               endif
                va_end(args);



                return (::pilo::i64_t)ret;
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
                if (ret < 0 || ret + 1 > len) {
                    return -1
                }
#               endif
                va_end(args);



                return (::pilo::i64_t)ret;
            }



            ::pilo::i64_t fd_formatted_output(::pilo::os_file_handle_t fd, const char* fmt, ...)
            {
                ::pilo::i64_t n = 0;
                va_list args;
                va_start(args, fmt);
#               if defined(WINDOWS)

                ::pilo::core::memory::object_array<char, 4096> buffer;
                ::pilo::i32_t i = 0;
                ::pilo::i64_t ret = 0;
                bool done = false;
                while (i < 16) {
                    ret = _vsnprintf_s(buffer.begin(), buffer.capacity(), _TRUNCATE, fmt, args);
                    if (ret >= 0) {
                        buffer.set_size((::pilo::i32_t)ret);  
                        done = true;
                        break;
                    } 
                    buffer.check_space(4096 * (i+1));
                    i++;
                }

                if (!done) {
                    ::pilo::i64_t tr_size = ::pilo::core::string::character_count(buffer.begin());
                    buffer.set_size((::pilo::i32_t)tr_size);
                }
                
                ::pilo::err_t err = ::pilo::core::io::xpf_write_file(fd, buffer.begin(), buffer.size(), &n);
                if (err != buffer.size())
                    n = -1;


#               else
                n = dprintf(fd, fmt, args);
#               endif

                va_end(args);

                if (n < 0)
                {
                    return -1;
                }

                return (::pilo::i64_t)n;
            }


        }
    }
}