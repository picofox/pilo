#include "./string_util.hpp"
#include "../../core/io/format_output.hpp"
#include "core/net/net_sys_header.hpp"

#ifdef WINDOWS
#include <WS2tcpip.h>
#endif


namespace pilo
{
    namespace core
    {
        namespace string
        {


            bool string_util::is_invalid_filename_char(const char x)
            {
                if ((x == '\"') || (x == '\'') || (x == '/') || (x == '\\') || (x == ':') || (x == '|') || (x == '<') || (x == '>')) {
                    return true;
                }

                return false;
            }


            bool string_util::is_invalid_path_char(const char x, size_t pos)
            {
                if ((x == '\"') || (x == '\'') || (x == '|') || (x == '<') || (x == '>')) 
                {
                    return true;
                }

#ifdef WINDOWS
                if (x == ':') 
                {
                    if (pos == 1) 
                    {
                        return false;
                    }
                    else 
                    {
                        return true;
                    }
                }

#else
                if (x == '\\') 
                {
                    return true;
                }

#endif
                return false;
            }

            size_t string_util::length(const char* str)
            {
                if (str == nullptr)
                {
                    return MC_INVALID_SIZE;
                }
                
                return ::strlen(str);
            }

            size_t string_util::length(const wchar_t* str)
            {
                if (str == nullptr)
                {
                    return MC_INVALID_SIZE;
                }

                return ::wcslen(str);
            }

            size_t string_util::element_count(const char* str, pilo::core::string::EnumCharsetEncodeType eType)
			{
                if (pilo::core::string::eEncodeMBCS == eType)
				{
					return string_util::_element_count_mbcs((const unsigned char*) str);
				}
                else if (pilo::core::string::eEncodeUTF8 == eType)
				{
                    return string_util::_element_count_utf8(str);
				}

                return MC_INVALID_SIZE;
			}

            size_t element_count(const wchar_t* str, pilo::core::string::EnumCharsetEncodeType eType)
            {
                M_UNUSED(eType);
                return pilo::core::string::string_util::length(str);
            }

            size_t string_util::_element_count_mbcs(const unsigned char* str)
            {
                if (str == nullptr)
                {
                    return MC_INVALID_SIZE;
                }

#               if defined(WIN32) || defined(WINDOWS)
                return _mbslen(str);

#               else
                if (str == nullptr) {
                    return -1;
                }

                int count = 0;
                while (*str != 0) 
                {
                    if ((*str) & (1 << 7)) 
                    {
                        if ((*(str + 1) != 0) && ((*(str + 1)) & (1 << 7))) {
                            str++;
                        }
                    }
                    count++;
                    str++;
                }
                return count;
#               endif
            }

			size_t string_util::_element_count_utf8(const char* str)
			{
				if (str == nullptr)
				{
					return MC_INVALID_SIZE;
				}


				size_t i = 0, j = 0;
				while (0 != str[i])
                {
                    if ((str[i] & 0xc0) != 0x80)
                    {
                        j++;
                    }
                    i++;
                }
                return j;
			}

            void string_util::set(char* str, char ch, size_t count)
            {
                ::memset(str, (int)ch, count);
                *(str + count) = 0;
            }

            void string_util::set(wchar_t* str, wchar_t ch, size_t count)
            {
                ::wmemset(str, ch, count);
                *(str + count) = 0;

            }

            void string_util::m_set(char* str, char ch, size_t count)
            {
                ::memset(str, (int) ch, count);
            }

            void string_util::m_set(wchar_t* str, wchar_t ch, size_t count)
            {
                ::wmemset(str, ch, count);
            }

            size_t string_util::number_to_string(char* dst_buffer, size_t dst_len, pilo::i32_t i, pilo::i32_t width,  bool zero_fill)
            {                
                if (width == 0)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, "%d", i);
                }
                char fmt_buffer[32];
                if (zero_fill == 0)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%%dd",  width);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%0%dd", width);
                }
                
                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, i);
            }

            size_t string_util::number_to_string(wchar_t* dst_buffer, size_t dst_len, pilo::i32_t i, pilo::i32_t width, bool zero_fill)
            {
                if (width == 0)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, L"%d", i);
                }
                wchar_t fmt_buffer[32];
                if (zero_fill == 0)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%%dd", width);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%0%dd", width);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, i);
            }

            size_t string_util::number_to_string(char* dst_buffer, size_t dst_len, pilo::i64_t i, pilo::i32_t width, bool zero_fill)
            {
                if (width == 0)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, "%lld", i);
                }
                char fmt_buffer[32];
                if (zero_fill == 0)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%%dlld", width);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%0%dlld", width);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, i);
            }

            size_t string_util::number_to_string(wchar_t* dst_buffer, size_t dst_len, pilo::i64_t i, pilo::i32_t width, bool zero_fill)
            {
                if (width == 0)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, L"%lld", i);
                }
                wchar_t fmt_buffer[32];
                if (zero_fill == 0)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%%dlld", width);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%0%dlld", width);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, i);
            }

            size_t string_util::number_to_string(char* dst_buffer, size_t dst_len, pilo::u32_t i, pilo::i32_t width, bool zero_fill)
            {
                if (width == 0)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, "%u", i);
                }
                char fmt_buffer[32];
                if (zero_fill == 0)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%%du", width);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%0%du", width);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, i);
            }

            size_t string_util::number_to_string(wchar_t* dst_buffer, size_t dst_len, pilo::u32_t i, pilo::i32_t width, bool zero_fill)
            {
                if (width == 0)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, L"%u", i);
                }
                wchar_t fmt_buffer[32];
                if (zero_fill == 0)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%%du", width);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%0%du", width);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, i);
            }

            size_t string_util::number_to_string(char* dst_buffer, size_t dst_len, pilo::u64_t i, pilo::i32_t width, bool zero_fill)
            {
                if (width == 0)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, "%llu", i);
                }
                char fmt_buffer[32];
                if (zero_fill == 0)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%%dllu", width);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%0%dllu", width);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, i);
            }

            size_t string_util::number_to_string(wchar_t* dst_buffer, size_t dst_len, pilo::u64_t i, pilo::i32_t width, bool zero_fill)
            {
                if (width == 0)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, L"%llu", i);
                }
                wchar_t fmt_buffer[32];
                if (zero_fill == 0)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%%dllu", width);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%0%dllu", width);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, i);
            }





            size_t string_util::number_to_string(char* dst_buffer, size_t dst_len, float f, const char* fmt, bool zero_fill)
            {
                if (fmt == nullptr)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, "%f", f);
                }
                char fmt_buffer[32];
                if (zero_fill == false)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%%sf", fmt);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%0%sf", fmt);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, f);
                
            }

            size_t string_util::number_to_string(wchar_t* dst_buffer, size_t dst_len, float f, const wchar_t* fmt, bool zero_fill)
            {
                if (fmt == nullptr)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, L"%f", f);
                }
                wchar_t fmt_buffer[32];
                if (zero_fill == false)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%%lsf", fmt);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%0%lsf", fmt);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, f);
            }

            size_t string_util::number_to_string(char* dst_buffer, size_t dst_len, double f, const char* fmt, bool zero_fill)
            {
                if (fmt == nullptr)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, "%f", f);
                }
                char fmt_buffer[32];
                if (zero_fill == false)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%%sf", fmt);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), "%%0%sf", fmt);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, f);
            }

            size_t string_util::number_to_string(wchar_t* dst_buffer, size_t dst_len, double f, const wchar_t* fmt, bool zero_fill)
            {
                if (fmt == nullptr)
                {
                    return pilo::core::io::string_format_output(dst_buffer, dst_len, L"%f", f);
                }
                wchar_t fmt_buffer[32];
                if (zero_fill == false)
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%%sf", fmt);
                }
                else
                {
                    pilo::core::io::string_format_output(fmt_buffer, MF_COUNT_OF(fmt_buffer), L"%%0%sf", fmt);
                }

                return pilo::core::io::string_format_output(dst_buffer, dst_len, fmt_buffer, f);
            }

            pilo::i32_t string_util::binary_compare(const char* str0, size_t begin_pos_0, const char* str1, size_t begin_pos_1, size_t length)
            {
                if (length == MC_INVALID_SIZE) 
                {
                    return ::strcmp(str0 + begin_pos_0, str1 + begin_pos_1);
                }
                else 
                {
                    return ::strncmp(str0 + +begin_pos_0, str1 + +begin_pos_1, length);
                }
            }

            pilo::i32_t string_util::binary_compare(const wchar_t* str0, size_t begin_pos_0, const wchar_t* str1, size_t begin_pos_1, size_t length)
            {
                if (length == MC_INVALID_SIZE) 
                {
                    return ::wcscmp(str0 + begin_pos_0, str1 + begin_pos_1);
                }
                else 
                {
                    return ::wcsncmp(str0 + begin_pos_0, str1 + begin_pos_1, length);
                }
            }


            pilo::i32_t string_util::cstr_to_int32(const char* pc)
            {
                if (pc == nullptr || pc[0] == '\0') return 0;

                //bool negative = false;
                const char* pb = pc;
                if (pc[0] == '+' || pc[0] == '-')
                {
                    pb = pc + 1;
                    //if (pc[0] == '-') negative = true;
                }

                size_t len = strlen(pb);
                if (len > 2 && pb[0] == '0' && toupper(pb[1]) == 'X')
                    return strtoul(pc, nullptr, 16);

                return strtoul(pc, nullptr, 10);
            }


            pilo::i64_t string_util::cstr_to_int64(const char* pc)
            {
                if (pc == nullptr || pc[0] == '\0') return 0;

                //bool negative = false;
                const char* pb = pc;
                if (pc[0] == '+' || pc[0] == '-')
                {
                    pb = pc + 1;
                    //if (pc[0] == '-') negative = true;
                }

                size_t len = strlen(pb);
                if (len > 2 && pb[0] == '0' && toupper(pb[1]) == 'X')
#ifdef WINDOWS
                {
                    return _strtoi64(pc, nullptr, 10);
                }
                return _strtoi64(pc, nullptr, 10);
#else
                {
                    return strtoll(pc, nullptr, 10);
                }
                return strtoll(pc, nullptr, 10);
#endif
            }


            const char* string_util::binary_to_cstr(char* dst_buffer, size_t dst_len, const void* src, size_t src_len, size_t num_perline)
            {
                MP_CHECK_NULL_PTR_RET(dst_buffer, nullptr);
                MP_CHECK_NULL_PTR_RET(src, nullptr);
                MP_CHECK_EQUAL_VALUE_RET(src_len, MC_INVALID_SIZE, nullptr);

                const char* p = (const char *)src;
                if (dst_len <= src_len * 3)
                {
                    return nullptr;
                }

                for (size_t i = 0; i < src_len; i++)
                {
                    ::pilo::core::io::string_format_output(dst_buffer + i*3, dst_len-i*3, "%02X ", (unsigned char) p[i]);

                    if ((i + 1) % num_perline == 0)
                    {
                        dst_buffer[(i + 1) * 3 - 1] = '\n';
                    }
                }

                return dst_buffer;
            }

            size_t string_util::split(char* s, const char delimiter, char* ss[], size_t maxs)
            {
                size_t i = 0;
                ss[i++] = s;
                while (s[0] && i < maxs)
                {
                    size_t n = pilo_max(1, mblen(s, 6));
                    if (n == 1 && delimiter == s[0])
                    {
                        s[0] = '\0'; s += n;
                        ss[i++] = s;
                    }
                    else
                    {
                        s += n;
                    }
                }
                return i;
            }


            size_t string_util::split(char* s, const char* delimiters, char* ss[], size_t maxs)
            {
                size_t i = 0;
                ss[i++] = s;
                while (s[0] && i < maxs)
                {
                    size_t n = pilo_max(1, ::mblen(s, 6));
                    if (n == 1 && ::strchr(delimiters, s[0]))
                    {
                        s[0] = '\0'; s += n;
                        while ((n = mblen(s, 6)) == 1 && strchr(delimiters, s[0])) s++; //TODO mblen 
                        ss[i++] = s;
                    }
                    else
                    {
                        s += n;
                    }
                }
                return i;
            }

            bool string_util::ipv4_address_cstr_to_int(pilo::u32_t& ip, pilo::u16_t& port, const char* cstr, size_t length)
            {
                char buf[256];
                pilo::core::string::string_util::copy(buf, sizeof(buf), cstr, length);

                char* ss[2];
                size_t ns = pilo::core::string::string_util::split(buf, ':', ss, 2);
                if (ns != 2)
                {
                    return false;
                }

                struct in_addr s;
                inet_pton(AF_INET, ss[0], (void *)&s);
                ip = ntohl(s.s_addr);

                port = (pilo::u16_t) pilo::core::string::string_util::cstr_to_int32(ss[1]);

                return true;
            }

            void string_util::erase_front_until(char* str, char ch, size_t sz,bool inc)
            {
                if (sz == MC_INVALID_SIZE)
                {
                    sz = ::pilo::core::string::string_util::length(str);
                }

                int n_ex = inc ? 1 : 0;
                char* p = find(str ,ch, 0 , sz);
                if (p != nullptr)
                {
                    size_t sz_copy = sz - (p-str+n_ex);
                    if (sz_copy <= 0)
                    {
                        return;
                    }
                    memmove(str, p+n_ex, sz_copy);
                    str[sz_copy] = 0;
                }
            } 

            void string_util::erase_front_until(wchar_t* str, wchar_t ch, size_t sz,bool inc)
            {
                if (sz == MC_INVALID_SIZE)
                {
                    sz = ::pilo::core::string::string_util::length(str);
                }

                int n_ex = inc ? 1 : 0;
                wchar_t* p = find(str ,ch, 0 , sz);
                if (p != nullptr)
                {
                    size_t sz_copy = sz - (p-str+n_ex);
                    if (sz_copy <= 0)
                    {
                        return;
                    }
                    memmove(str, p+n_ex, sz_copy*sizeof(wchar_t));
                    str[sz_copy] = 0;
                }
            }

        }
    }
}