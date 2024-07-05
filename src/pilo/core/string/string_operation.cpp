#include "string_operation.hpp"
#include "../io/formatted_io.hpp"
#include <cwchar>
#include "../../error.hpp"

namespace pilo
{
    namespace core
    {
        namespace string
        {

            ::pilo::i64_t character_count(const char* str)
            {
                if (str == nullptr)
                {
                    return -1;
                }

                return strlen(str);
            }

            ::pilo::i64_t character_count(const wchar_t* cwstr)
            {
                if (cwstr == nullptr)
                {
                    return -1;
                }

                return wcslen(cwstr);
            }

            ::pilo::i64_t copyz(char* dst, ::pilo::i64_t capa_char, const char* src)
            {
                if (dst == nullptr || src == nullptr || capa_char == 0)
                {
                    return -1;
                }

                ::pilo::i64_t len = PMF_STRING_LENGTH_A(src);
                if (len >= capa_char)
                {
                    len = capa_char - 1;
                }

                if (len > 0)
                {
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    strncpy(dst, src, len);
#   pragma warning( pop ) 
#else
                    strncpy(dst, src, len);
#endif
                }

                dst[len] = 0;

                return len + 1;
            }

            ::pilo::i64_t copyz(wchar_t* dst, ::pilo::i64_t capa_char, const wchar_t* src)
            {
                if (dst == nullptr || src == nullptr || capa_char == 0)
                {
                    return -1;
                }

                ::pilo::i64_t len = PMF_STRING_LENGTH_W(src);
                if (len >= capa_char)
                {
                    len = capa_char - 1;
                }

                if (len > 0)
                {
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    wcsncpy(dst, src, len);
#   pragma warning( pop ) 
#else
                    wcsncpy(dst, src, len);
#endif
                }

                dst[len] = 0;
                return len + 1;
            }

            ::pilo::i64_t copy(char* dst, ::pilo::i64_t capa_char, const char* src)
            {
                if (dst == nullptr || src == nullptr)
                {
                    return -1;
                }                

                if (capa_char == 0)
                {
                    return 0;
                }
  
                ::pilo::i64_t len = PMF_STRING_LENGTH_A(src);
                if (len > capa_char)
                {
                    len = capa_char;
                }

                if (len > 0)
                {
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    strncpy(dst, src, len);
#   pragma warning( pop ) 
#else
                    strncpy(dst, src, len);
#endif
                }

                return len;
            }

            ::pilo::i64_t copy(wchar_t* dst, ::pilo::i64_t capa_char, const wchar_t* src)
            {
                if (dst == nullptr || src == nullptr)
                {
                    return -1;
                }                

                if (capa_char == 0)
                {
                    return 0;
                }

                ::pilo::i64_t len = PMF_STRING_LENGTH_W(src);
                if (len > capa_char)
                {
                    len = capa_char;
                }

                if (len > 0)
                {
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    wcsncpy(dst, src, len);
#   pragma warning( pop ) 
#else
                    wcsncpy(dst, src, len);
#endif
                }

                return len;
            }

            ::pilo::i64_t n_copy(char* dst, ::pilo::i64_t capa_char, const char* src, ::pilo::i64_t src_len)
            {
                if (dst == nullptr || src == nullptr)
                {
                    return -1;
                }

                if (capa_char == 0)
                {
                    return 0;
                }
  
                if (src_len == -1)
                {
                    src_len = PMF_STRING_LENGTH_A(src);
                }

                if (src_len > capa_char)
                {
                    src_len = capa_char;
                }

                if (src_len > 0)
                {
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    strncpy(dst, src, src_len);
#   pragma warning( pop ) 
#else
                    strncpy(dst, src, src_len);
#endif
                }
   
                return src_len;
            }

            ::pilo::i64_t n_copy(wchar_t* dst, ::pilo::i64_t capa_char, const wchar_t* src, ::pilo::i64_t src_len)
            {
                if (dst == nullptr || src == nullptr)
                {
                    return -1;
                }

                if (capa_char == 0)
                {
                    return 0;
                }

                if (src_len == -1)
                {
                    src_len = PMF_STRING_LENGTH_W(src);
                }

                if (src_len > capa_char)
                {
                    src_len = capa_char;
                }

                if (src_len > 0)
                {
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    wcsncpy(dst, src, src_len);
#   pragma warning( pop ) 
#else
                    wcsncpy(dst, src, src_len);
#endif
                }

                return src_len;
            }

            ::pilo::i64_t n_copyz(char* dst, ::pilo::i64_t capa_char, const char* src, ::pilo::i64_t src_len)
            {
                if (dst == nullptr || src == nullptr || capa_char == 0)
                {
                    return -1;
                }

                if (src_len == -1)
                {
                    src_len = PMF_STRING_LENGTH_A(src);
                }

                if (src_len >= capa_char)
                {
                    src_len = capa_char - 1;
                }

                if (src_len > 0)
                {
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    strncpy(dst, src, src_len);
#   pragma warning( pop ) 
#else
                    strncpy(dst, src, src_len);
#endif
                }

                dst[src_len] = 0;
                return src_len + 1;
            }

            ::pilo::i64_t n_copyz(wchar_t* dst, ::pilo::i64_t capa_char, const wchar_t* src, ::pilo::i64_t src_len)
            {
                if (dst == nullptr || src == nullptr || capa_char == 0)
                {
                    return -1;
                }

                if (src_len == -1)
                {
                    src_len = PMF_STRING_LENGTH_W(src);
                }

                if (src_len >= capa_char)
                {
                    src_len = capa_char - 1;
                }

                if (src_len > 0)
                {
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    wcsncpy(dst, src, src_len);
#   pragma warning( pop ) 
#else
                    wcsncpy(dst, src, src_len);
#endif
                }

                dst[src_len] = 0;
                return src_len + 1;
            }

            ::pilo::i64_t move(char* dst, ::pilo::i64_t cc_dst_capa, const char* src)
            {
                return ::pilo::core::string::n_move(dst, cc_dst_capa, src, -1);
            }

            ::pilo::i64_t move(wchar_t* dst, ::pilo::i64_t cc_dst_capa, const wchar_t* src)
            {
                return ::pilo::core::string::n_move(dst, cc_dst_capa, src, -1);
            }

            ::pilo::i64_t n_move(char* dst, ::pilo::i64_t cc_dst_capa, const char* src, ::pilo::i64_t src_length)
            {
                if (dst == nullptr || src == nullptr)
                {
                    return -1;
                }

                if (cc_dst_capa == 0)
                {
                    return 0;
                }

                if (src_length == -1)
                {
                    src_length = PMF_STRING_LENGTH_A(src);
                }

                if (src_length >= cc_dst_capa - 1)
                {
                    src_length = cc_dst_capa - 1;
                }

                if (src_length > 0)
                {
#if defined(WINDOWS)
                    memmove_s(dst, cc_dst_capa, src, src_length);
#else
                    memmove(dst, src, src_length);
#endif
                }

                return src_length;
            }

            ::pilo::i64_t n_move(wchar_t* dst, ::pilo::i64_t cc_dst_capa, const wchar_t* src, ::pilo::i64_t src_length)
            {
                if (dst == nullptr || src == nullptr)
                {
                    return -1;
                }

                if (cc_dst_capa == 0)
                {
                    return 0;
                }

                if (src_length == -1)
                {
                    src_length = PMF_STRING_LENGTH_W(src);
                }

                if (src_length >= cc_dst_capa - 1)
                {
                    src_length = cc_dst_capa - 1;
                }

                if (src_length > 0)
                {
#if defined(WINDOWS)
                    memmove_s(dst, cc_dst_capa * sizeof(wchar_t), src, src_length * sizeof(wchar_t));
#else
                    memmove(dst, src, src_length * sizeof(wchar_t));
#endif
                }

                return src_length;
            }

            ::pilo::i64_t movez(char* dst, ::pilo::i64_t cc_dst_capa, const char* src)
            {
                return ::pilo::core::string::n_movez(dst, cc_dst_capa, src, -1);
            }

            ::pilo::i64_t movez(wchar_t* dst, ::pilo::i64_t cc_dst_capa, const wchar_t* src)
            {
                return ::pilo::core::string::n_movez(dst, cc_dst_capa, src, -1);
            }

            ::pilo::i64_t n_movez(char* dst, ::pilo::i64_t cc_dst_capa, const char* src, ::pilo::i64_t src_length)
            {
                if (dst == nullptr || src == nullptr || cc_dst_capa == 0)
                {
                    return -1;
                }

                if (src_length == -1)
                {
                    src_length = PMF_STRING_LENGTH_A(src);
                }

                if (src_length >= cc_dst_capa - 1)
                {
                    src_length = cc_dst_capa - 1;
                }

                if (src_length > 0)
                {
#if defined(WINDOWS)
                    memmove_s(dst, cc_dst_capa, src, src_length);
#else
                    memmove(dst, src, src_length);
#endif
                }

                dst[src_length] = 0;
                return src_length + 1;
            }

            ::pilo::i64_t n_movez(wchar_t* dst, ::pilo::i64_t cc_dst_capa, const wchar_t* src, ::pilo::i64_t src_length)
            {
                if (dst == nullptr || src == nullptr || cc_dst_capa == 0)
                {
                    return -1;
                }

                if (src_length == -1)
                {
                    src_length = PMF_STRING_LENGTH_W(src);
                }

                if (src_length >= cc_dst_capa - 1)
                {
                    src_length = cc_dst_capa - 1;
                }

                if (src_length > 0)
                {
#if defined(WINDOWS)
                    memmove_s(dst, cc_dst_capa * sizeof(wchar_t), src, src_length * sizeof(wchar_t));
#else
                    memmove(dst, src, src_length * sizeof(wchar_t));
#endif
                }

                dst[src_length] = 0;
                return src_length + 1;
            }


			char* n_concatenate_inplace(char* dst_buffer, ::pilo::i64_t dst_capa, const char* src, ::pilo::i64_t len)
			{
				if (dst_buffer == nullptr)
				{
					return nullptr;
				}

				if (src == nullptr || dst_capa == 0)
				{
					return dst_buffer;
				}

				if (len == -1)
				{
					len = PMF_STRING_LENGTH_A(src);
				}

				if (dst_capa <= len)
				{
					len = dst_capa - 1;
				}

				if (dst_capa == -1)
				{
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
					strcat(dst_buffer, src);
#   pragma warning( pop ) 
#else
					strcat(dst_buffer, src);
#endif
				}
				else
				{
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
					strncat(dst_buffer, src, len);
#   pragma warning( pop ) 
#else
					strncat(dst_buffer, src, len);
#endif
				}

				return dst_buffer;
			}

			wchar_t* n_concatenate_inplace(wchar_t* dst_buffer, ::pilo::i64_t dst_capa, const wchar_t* src, ::pilo::i64_t len)
			{
				if (dst_buffer == nullptr)
				{
					return nullptr;
				}

				if (src == nullptr || dst_capa == 0)
				{
					return dst_buffer;
				}

				if (len == -1)
				{
					len = PMF_STRING_LENGTH_W(src);
				}

				if (dst_capa <= len)
				{
					len = dst_capa - 1;
				}

				if (dst_capa == -1)
				{
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    wcscat(dst_buffer, src);
#   pragma warning( pop ) 
#else
                    wcscat(dst_buffer, src);
#endif
				}
				else
				{
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
                    wcsncat(dst_buffer, src,len);
#   pragma warning( pop ) 
#else
                    wcsncat(dst_buffer, src, len);
#endif
				}
				return dst_buffer;
			}			

			void set_character(char* cstr, char ch, ::pilo::i64_t len)
            {
                if (cstr != nullptr)
                {
                    memset(cstr, ch, len);
                }                
            }

            void set_character(wchar_t* cwstr, wchar_t ch, ::pilo::i64_t len)
            {
                if (cwstr != nullptr)
                {
                    ::pilo::i64_t i = 0;
                    do
                    {
                        cwstr[i++] = ch;
                    } while (i < len);
                }                
            }

            void set_character_z(char* cstr, char ch, ::pilo::i64_t len)
            {
                if (cstr != nullptr)
                {
                    memset(cstr, ch, len);
                    cstr[len] = 0;
                }
            }

            void set_character_z(wchar_t* cwstr, wchar_t ch, ::pilo::i64_t len)
            {
                if (cwstr != nullptr)
                {
                    ::pilo::i64_t i = 0;
                    do
                    {
                        cwstr[i++] = ch;
                    } while (i < len);
                    cwstr[len] = 0;
                }
            } 

            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::i8_t i, const char* fmt /*= nullptr*/)
            {
                if (fmt == nullptr)
                    ::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%d", (char) i);
                else
                    ::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, (unsigned char)i);
                return dst_buffer;
            }

            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::u8_t i, const char* fmt /*= nullptr*/)
            {
                if (fmt == nullptr)
                    ::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%u", (unsigned char)i);
                else
                    ::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, (unsigned char)i);
                return dst_buffer;
            }

            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::i16_t i, const char* fmt /*= nullptr*/)
            {
                if (fmt == nullptr)
                    ::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%hd", i);
                else
                    ::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
                return dst_buffer;
            }

            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::u16_t i, const char* fmt /*= nullptr*/)
            {
                if (fmt == nullptr)
                    ::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%hu", i);
                else
                    ::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
                return dst_buffer;
            }

			char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::i32_t i, const char* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%d", i);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
				return dst_buffer;				
			}
			wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::i32_t i, const wchar_t* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, L"%d", i);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
				return dst_buffer;

			}
			char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::i64_t i, const char* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%lld", i);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
				return dst_buffer;

			}
			wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::i64_t i, const wchar_t* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, L"%lld", i);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
				return dst_buffer;
			}

			char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::u32_t i, const char* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%u", i);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
				return dst_buffer;

			}
			wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::u32_t i, const wchar_t* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, L"%u", i);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
				return dst_buffer;

			}
			char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::u64_t i, const char* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%llu", i);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
				return dst_buffer;

			}
			wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::u64_t i, const wchar_t* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, L"%llu", i);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, i);
				return dst_buffer;
			}


			char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::f32_t f, const char* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%f", f);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, f);
				return dst_buffer;

			}
			wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::f32_t f, const wchar_t* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, L"%f", f);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, f);
				return dst_buffer;
			}

			char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::f64_t d, const char* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, "%lf", d);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, d);
				return dst_buffer;

			}
			wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::f64_t d, const wchar_t* fmt /*= nullptr*/)
			{
				if (fmt == nullptr)
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, L"%lf", d);
				else
					::pilo::core::io::string_formated_output(dst_buffer, dst_len, fmt, d);
				return dst_buffer;

			}

            ::pilo::i32_t strict_compare(const char* str0, ::pilo::i64_t begin_pos_0, const char* str1, ::pilo::i64_t begin_pos_1, ::pilo::i64_t length)
            {
                if (length < 0)
                {
                    return ::strcmp(str0 + begin_pos_0, str1 + begin_pos_1);
                }
                else
                {
                    return ::strncmp(str0 + begin_pos_0, str1 + begin_pos_1, length);
                }
            }

            ::pilo::i32_t strict_compare(const wchar_t* str0, ::pilo::i64_t begin_pos_0, const wchar_t* str1, ::pilo::i64_t begin_pos_1, ::pilo::i64_t length)
            {
                if (length < 0)
                {
                    return ::wcscmp(str0 + begin_pos_0, str1 + begin_pos_1);
                }
                else
                {
                    return ::wcsncmp(str0 + begin_pos_0, str1 + begin_pos_1, length);
                }
            }

			::pilo::i32_t i_compare(const char* str0, ::pilo::i64_t begin_pos_0, const char* str1, ::pilo::i64_t begin_pos_1, ::pilo::i64_t length)
			{
				if (length < 0)
				{
					return PMF_STRING_I_COMPARE_A(str0 + begin_pos_0, str1 + begin_pos_1);
				}
				else
				{
                    return PMF_STRING_I_N_COMPARE_A(str0 + begin_pos_0, str1 + begin_pos_1, length);
				}
			}


			::pilo::i32_t i_compare(const wchar_t* str0, ::pilo::i64_t begin_pos_0, const wchar_t* str1, ::pilo::i64_t begin_pos_1, ::pilo::i64_t length)
			{
				if (length < 0)
				{
					return PMF_STRING_I_COMPARE_W(str0 + begin_pos_0, str1 + begin_pos_1);
				}
				else
				{
					return PMF_STRING_I_N_COMPARE_W(str0 + begin_pos_0, str1 + begin_pos_1, length);
				}
			}
            

            ::pilo::err_t string_to_number(::pilo::i32_t& v, const char* pc, ::pilo::i64_t len)
            {
                if (pc == nullptr)
                {
                    v = 0;
                    return ::pilo::mk_perr(PERR_NULL_PARAM);
                }

                if (len < 0)
                    len = PMF_STRING_LENGTH_A(pc);

                if (len > 2 && pc[0] == '0' && toupper(pc[1]) == 'X')
                {
                    v = (::pilo::i32_t) strtoul(pc, NULL, 16);
                    return PILO_OK;
                }
                else if (len > 1 && pc[0] == '0')
                {
                    v = (::pilo::i32_t) strtoul(pc, NULL, 8);
                    return PILO_OK;
                }
                else
                {
                    v = (::pilo::i32_t) strtoul(pc, NULL, 10);
                    return PILO_OK;
                }                
            }

            ::pilo::err_t string_to_number(::pilo::i64_t& v, const char* pc, ::pilo::i64_t len)
            {
                if (pc == nullptr)
                {
                    v = 0;
                    return ::pilo::mk_perr(PERR_NULL_PARAM);
                }

                if (len < 0)
                    len = PMF_STRING_LENGTH_A(pc);

#ifdef WINDOWS
                if (len > 2 && pc[0] == '0' && toupper(pc[1]) == 'X')
                {
                    v = _strtoi64(pc, NULL, 16);
                    return PILO_OK;
                }                    
                else if (len > 1 && pc[0] == '0')
                {
                    v = _strtoi64(pc, NULL, 8);
                    return PILO_OK;
                }                    
                else
                {
                    v = _strtoi64(pc, NULL, 10);
                    return PILO_OK;
                }
                    
#else
                if (len > 2 && pc[0] == '0' && toupper(pc[1]) == 'X')
                {
                    v = strtoll(pc, NULL, 16);
                    return PILO_OK;
                }
                    
                else if (len > 1 && pc[0] == '0')
                {
                    v = strtoll(pc, NULL, 8);
                    return PILO_OK;
                }
                    
                else
                {
                    v = strtoll(pc, NULL, 10);
                    return PILO_OK;
                }                    
#endif
            }

            ::pilo::err_t string_to_number(::pilo::f32_t& v, const char* pc, ::pilo::i64_t len)
            {
                PMC_UNUSED(len);
                if (pc == nullptr) return ::pilo::mk_perr(PERR_NULL_PARAM);
                v = (::pilo::f32_t) ::strtod(pc, NULL);
                return PILO_OK;
            }

            ::pilo::err_t string_to_number(::pilo::f64_t& v, const char* pc, ::pilo::i64_t len)
            {
                PMC_UNUSED(len);
                if (pc == nullptr) return ::pilo::mk_perr(PERR_NULL_PARAM);
                v = ::strtod(pc, NULL);
                return PILO_OK;
            }



            ::pilo::err_t string_to_number(::pilo::i32_t& v, const wchar_t* pc, ::pilo::i64_t len)
            {
                if (pc == nullptr)
                {
                    v = 0;
                    return ::pilo::mk_perr(PERR_NULL_PARAM);
                }

                if (len < 0)
                    len = PMF_STRING_LENGTH_W(pc);

                if (len > 2 && pc[0] == '0' && toupper(pc[1]) == 'X')
                {
                    v = (::pilo::i32_t) wcstoul(pc, NULL, 16);
                    return PILO_OK;
                }
                else if (len > 1 && pc[0] == '0')
                {
                    v = (::pilo::i32_t) wcstoul(pc, NULL, 8);
                    return PILO_OK;
                }
                else
                {
                    v = (::pilo::i32_t) wcstoul(pc, NULL, 10);
                    return PILO_OK;
                }
            }

            ::pilo::err_t string_to_number(::pilo::i64_t& v, const wchar_t* pc, ::pilo::i64_t len)
            {
                if (pc == nullptr)
                {
                    v = 0;
                    return ::pilo::mk_perr(PERR_NULL_PARAM);
                }

                if (len < 0)
                    len = PMF_STRING_LENGTH_W(pc);

#ifdef WINDOWS
                if (len > 2 && pc[0] == '0' && toupper(pc[1]) == 'X')
                {
                    v = _wcstoi64(pc, NULL, 16);
                    return PILO_OK;
                }
                else if (len > 1 && pc[0] == '0')
                {
                    v = _wcstoi64(pc, NULL, 8);
                    return PILO_OK;
                }
                else
                {
                    v = _wcstoi64(pc, NULL, 10);
                    return PILO_OK;
                }

#else
                if (len > 2 && pc[0] == '0' && toupper(pc[1]) == 'X')
                {
                    v = wcstoll(pc, NULL, 16);
                    return PILO_OK;
                }

                else if (len > 1 && pc[0] == '0')
                {
                    v = wcstoll(pc, NULL, 8);
                    return PILO_OK;
                }

                else
                {
                    v = wcstoll(pc, NULL, 10);
                    return PILO_OK;
                }
#endif
            }            

            ::pilo::err_t string_to_number(::pilo::f32_t& v, const wchar_t* pc, ::pilo::i64_t len)
            {
                PMC_UNUSED(len);
                if (pc == nullptr) return ::pilo::mk_perr(PERR_NULL_PARAM);
                v = (::pilo::f32_t) ::wcstod(pc, NULL);
                return PILO_OK;
            }

            ::pilo::err_t string_to_number(::pilo::f64_t& v, const wchar_t* pc, ::pilo::i64_t len)
            {
                PMC_UNUSED(len);
                if (pc == nullptr) return ::pilo::mk_perr(PERR_NULL_PARAM);
                v = ::wcstod(pc, NULL);
                return PILO_OK;
            }

		}
    }
}
