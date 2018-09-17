
#include "core/string/string_util.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_string_util.hpp"

#include <iostream>

namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_length_char(void* param); 
        static pilo::i32_t functional_test_length_wchar(void* param);
        static pilo::i32_t functional_test_element_count_utf8(void* param);
        static pilo::i32_t functional_test_element_count_mbcs(void* param);
        static pilo::i32_t functional_test_copy_0_char(void* param);
        static pilo::i32_t functional_test_copy_0_wchar(void* param); 
        static pilo::i32_t functional_test_copy_1_char(void* param);
        static pilo::i32_t functional_test_copy_1_wchar(void* param);
        static pilo::i32_t functional_test_m_copy_0_char(void* param);
        static pilo::i32_t functional_test_m_copy_0_wchar(void* param);    
        static pilo::i32_t functional_test_m_copy_1_char(void* param);
        static pilo::i32_t functional_test_m_copy_1_wchar(void* param);
        static pilo::i32_t functional_test_m_move_0_char(void* param);
        static pilo::i32_t functional_test_m_move_0_wchar(void* param);
        static pilo::i32_t functional_test_m_move_1_char(void* param);
        static pilo::i32_t functional_test_m_move_1_wchar(void* param);
        static pilo::i32_t functional_test_set_char(void* param);
        static pilo::i32_t functional_test_set_wchar(void* param);
        static pilo::i32_t functional_test_m_set_char(void* param);
        static pilo::i32_t functional_test_m_set_wchar(void* param);
        static pilo::i32_t functional_test_number_to_string_integer(void* param);
        static pilo::i32_t functional_test_number_to_string_float(void* param);

        static pilo::i32_t functional_test_rescanable_replace(void* param);
        

        pilo::i32_t functional_test_rescanable_replace(void* param)
        {
            M_UNUSED(param);

            char buffer[1024];
            size_t count;

            ::pilo::core::string::string_util::copy(buffer,  "1234567890abcdefg");

            char * ptr = nullptr;

            ::pilo::i32_t ret = ::pilo::core::string::string_util::rescanable_replace(ptr, 100, "1", "2", &count);
            if (ret != ::pilo::EC_NULL_PARAM)
            {
                return -1;
            }

            ret = ::pilo::core::string::string_util::rescanable_replace(buffer, 100, "0a", "xyz", &count);
            if (ret != ::pilo::EC_OK)
            {
                return -2;
            }
            if (::pilo::core::string::string_util::binary_compare(buffer, "123456789xyzbcdefg") != 0)
            {
                return -3;
            }

            ret = ::pilo::core::string::string_util::rescanable_replace(buffer, 100, "11", "11", &count);
            if (ret != ::pilo::EC_INFINITE_LOOP)
            {
                return -1;
            }

            ret = ::pilo::core::string::string_util::rescanable_replace(buffer, 100, "11", "111", &count);
            if (ret != ::pilo::EC_INFINITE_LOOP)
            {
                return -1;
            }
            ::pilo::core::string::string_util::copy(buffer, "1111111111111");
            ret = ::pilo::core::string::string_util::rescanable_replace(buffer, 100, "1111", "111", &count);
            if (ret != ::pilo::EC_OK)
            {
                return -1;
            }

            ::pilo::core::string::string_util::copy(buffer, "11111111112222222222222");
            ret = ::pilo::core::string::string_util::rescanable_replace(buffer, 100, "12", "", &count);
            if (ret != ::pilo::EC_OK)
            {
                return -1;
            }
            if (::pilo::core::string::string_util::binary_compare(buffer, "222") != 0)
            {
                return -3;
            }
            if (count != 10)
            {
                return -10;
            }

            ::pilo::core::string::string_util::copy(buffer, "11111111112222222222222");
            ret = ::pilo::core::string::string_util::rescanable_replace(buffer, 100, "12", (char*)nullptr, &count);
            if (ret != ::pilo::EC_OK)
            {
                return -1;
            }
            if (::pilo::core::string::string_util::binary_compare(buffer, "222") != 0)
            {
                return -3;
            } 
            if (count != 10)
            {
                return -10;
            }
 

            return 0;
        }

        pilo::test::testing_case g_functional_cases_string_util[] =
        {
            {    1, "length(<char type>)                          ", nullptr, functional_test_length_char, 0, -1, (pilo::u32_t)-1 },
            {    2, "length(<wchar type>)                         ", nullptr, functional_test_length_wchar, 0, -1, (pilo::u32_t) - 1 },
            {    3, "element_count(<utf8>)                        ", nullptr, functional_test_element_count_utf8, 0, -1, (pilo::u32_t) - 1 },
            {    4, "element_count(<mbcs>)                        ", nullptr, functional_test_element_count_mbcs, 0, -1, (pilo::u32_t) - 1 },
            {    5, "copy(<6 args, char type>)                    ", nullptr, functional_test_copy_0_char, 0, -1, (pilo::u32_t) - 1 },
            {    6, "copy(<6 args, wchar type>)                   ", nullptr, functional_test_copy_0_wchar, 0, -1, (pilo::u32_t) - 1 },
            {    7, "copy(<4 args, char type>)                    ", nullptr, functional_test_copy_1_char, 0, -1, (pilo::u32_t) - 1 },
            {    8, "copy(<4 args, char type>)                    ", nullptr, functional_test_copy_1_wchar, 0, -1, (pilo::u32_t) - 1 },
            {    9, "m_copy(<6 args, char type>)                  ", nullptr, functional_test_m_copy_0_char, 0, -1, (pilo::u32_t) - 1 },
            {   10, "m_copy(<6 args, wchar type>)                 ", nullptr, functional_test_m_copy_0_wchar, 0, -1, (pilo::u32_t) - 1 },
            {   11, "m_copy(<6 args, wchar type>)                 ", nullptr, functional_test_m_copy_1_char, 0, -1, (pilo::u32_t) - 1 },
            {   12, "m_copy(<6 args, wchar type>)                 ", nullptr, functional_test_m_copy_1_wchar, 0, -1, (pilo::u32_t) - 1 },
            {   13, "m_move(<6 args, char type>)                  ", nullptr, functional_test_m_move_0_char, 0, -1, (pilo::u32_t) - 1 },
            {   14, "m_move(<6 args, wchar type>)                 ", nullptr, functional_test_m_move_0_wchar, 0, -1, (pilo::u32_t) - 1 },
            {   15, "m_move(<4 args, char type>)                  ", nullptr, functional_test_m_move_1_char, 0, -1, (pilo::u32_t) - 1 },
            {   16, "m_move(<4 args, wchar type>)                 ", nullptr, functional_test_m_move_1_wchar, 0, -1, (pilo::u32_t) - 1 },
            {   17, "set(<char type>)                             ", nullptr, functional_test_set_char, 0, -1, (pilo::u32_t) - 1 },
            {   18, "set(<wchar type>)                            ", nullptr, functional_test_set_wchar, 0, -1, (pilo::u32_t) - 1 },
            {   19, "set(<char type>)                             ", nullptr, functional_test_m_set_char, 0, -1, (pilo::u32_t) - 1 },
            {   20, "set(<wchar type>)                            ", nullptr, functional_test_m_set_wchar, 0, -1, (pilo::u32_t) - 1 },
            {   21, "number_to_string(integer)                    ", nullptr, functional_test_number_to_string_integer, 0, -1, (pilo::u32_t) - 1 },
            {   22, "number_to_string(float)                      ", nullptr, functional_test_number_to_string_float, 0, -1, (pilo::u32_t) - 1 },
            {   23, "rescanable_replace                           ", nullptr, functional_test_rescanable_replace, 0, -1, (pilo::u32_t) - 1 },
            
            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        //implementation testing cases
        pilo::i32_t functional_test_length_char(void* param)
        {
            M_UNUSED(param);

            size_t ret = pilo::core::string::string_util::length("");
            if (ret != 0)
            {
                return -1;
            }

            ret = pilo::core::string::string_util::length((char*)nullptr);
            if (ret != MC_INVALID_SIZE)
            {
                return -2;
            }

            ret = pilo::core::string::string_util::length("liuxin,nihao ");
            if (ret != 13)
            {
                return -3;
            }

            const char* str = "0123456789";
            ret = pilo::core::string::string_util::length(str);
            if (ret != 10)
            {
                return -4;
            }


            return 0;
        }

        pilo::i32_t functional_test_length_wchar(void* param)
        {
            M_UNUSED(param);

            size_t ret = pilo::core::string::string_util::length(L"");
            if (ret != 0)
            {
                return -1;
            }

            ret = pilo::core::string::string_util::length((wchar_t*)nullptr);
            if (ret != MC_INVALID_SIZE)
            {
                return -2;
            }

            ret = pilo::core::string::string_util::length(L"liuxin,nihao ");
            if (ret != 13)
            {
                return -3;
            }

            const wchar_t* str = L"0123456789";
            ret = pilo::core::string::string_util::length(str);
            if (ret != 10)
            {
                return -4;
            }

            return 0;
        }

        pilo::i32_t functional_test_element_count_utf8(void* param)
        {
            M_UNUSED(param);

            size_t ret = pilo::core::string::string_util::element_count("", pilo::core::string::EnumCharsetEncodeType::eEncodeUTF8);
            if (ret != 0)
            {
                return -1;
            }

            ret = pilo::core::string::string_util::element_count((char*) nullptr, pilo::core::string::EnumCharsetEncodeType::eEncodeUTF8);
            if (ret != MC_INVALID_SIZE)
            {
                return -2;
            }

            ret = pilo::core::string::string_util::element_count((char*) "liuxin,nihao ", pilo::core::string::EnumCharsetEncodeType::eEncodeUTF8);
            if (ret != 13)
            {
                return -3;
            }

            return 0;
        }

        pilo::i32_t functional_test_element_count_mbcs(void* param)
        {
            M_UNUSED(param);

            size_t ret = pilo::core::string::string_util::element_count("", pilo::core::string::EnumCharsetEncodeType::eEncodeMBCS);
            if (ret != 0)
            {
                return -1;
            }

            ret = pilo::core::string::string_util::element_count((char*) nullptr, pilo::core::string::EnumCharsetEncodeType::eEncodeMBCS);
            if (ret != MC_INVALID_SIZE)
            {
                return -2;
            }

            ret = pilo::core::string::string_util::element_count((char*) "liuxin,nihao ", pilo::core::string::EnumCharsetEncodeType::eEncodeMBCS);
            if (ret != 13)
            {
                return -3;
            }

            ret = pilo::core::string::string_util::element_count("liuxin,nihao 天天上学 ", pilo::core::string::EnumCharsetEncodeType::eEncodeMBCS);
            if (ret != 18)
            {
                return -4;
            }

            ret = pilo::core::string::string_util::element_count("就liuxin,nihao 天天上学 .=啊、", pilo::core::string::EnumCharsetEncodeType::eEncodeMBCS);
            if (ret != 23)
            {
                return -4;
            }

            return 0;
        }

        pilo::i32_t functional_test_copy_0_char(void* param)
        {
            //template <typename T>
            //size_t pilo::core::string::string_util::copy(T * dst_str,
            //    size_t dst_pos,
            //    size_t dst_capacity,
            //    const T * src_str,
            //    size_t src_pos,
            //    size_t src_count)

            M_UNUSED(param);
            char dst_buffer[1024];
            size_t n_copied = 0;
            const char* src_str = "$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);
            
            n_copied = pilo::core::string::string_util::copy((char*)nullptr, 0, MF_COUNT_OF(dst_buffer), src_str, 0, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 0, MF_COUNT_OF(dst_buffer), (const char*) nullptr, 0, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::copy((char*) nullptr, 0, MF_COUNT_OF(dst_buffer), (const char*) nullptr, 0, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 0, MF_COUNT_OF(dst_buffer), src_str, 0, src_len);
            if (src_len != n_copied)
            {
                return -4;
            }
            std::string str_left = dst_buffer;
            std::string str_right = src_str;
            if (0 != str_left.compare(str_right))
            {
                return -41;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, MF_COUNT_OF(dst_buffer) - 1, 1, src_str, 0, src_len);
            if (0 != n_copied)
            {
                return -5;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MF_COUNT_OF(dst_buffer), src_str, 0, src_len);
            if (src_len != n_copied)
            {
                return -6;
            }
            str_left = dst_buffer+731;
            str_right = src_str;
            if (0 != str_left.compare(str_right))
            {
                return -61;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MC_INVALID_SIZE, src_str, 0, src_len);
            if (src_len != n_copied)
            {
                return -7;
            }
            str_left = dst_buffer + 731;
            str_right = src_str;
            if (0 != str_left.compare(str_right))
            {
                return -71;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MF_COUNT_OF(dst_buffer), src_str, 7, src_len);
            if (src_len-7 != n_copied)
            {
                return -8;
            }
            str_left = dst_buffer + 731;
            str_right = src_str+7;
            if (0 != str_left.compare(str_right))
            {
                return -81;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MC_INVALID_SIZE, src_str, 7, src_len);
            if (src_len - 7 != n_copied)
            {
                return -9;
            }
            str_left = dst_buffer + 731;
            str_right = src_str + 7;
            if (0 != str_left.compare(str_right))
            {
                return -91;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MC_INVALID_SIZE, src_str, 7, MC_INVALID_SIZE);
            if (src_len - 7 != n_copied)
            {
                return -10;
            }
            str_left = dst_buffer + 731;
            str_right = src_str + 7;
            if (0 != str_left.compare(str_right))
            {
                return -101;
            }

            return 0;
        }

        pilo::i32_t functional_test_copy_0_wchar(void* param)
        {
            M_UNUSED(param);
            wchar_t dst_buffer[1024];
            size_t n_copied = 0;
            const wchar_t* src_str = L"$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::copy((wchar_t*)nullptr, 0, MF_COUNT_OF(dst_buffer), src_str, 0, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 0, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, 0, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::copy((wchar_t*) nullptr, 0, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, 0, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 0, MF_COUNT_OF(dst_buffer), src_str, 0, src_len);
            if (src_len != n_copied)
            {
                return -4;
            }

            if (0 != memcmp(dst_buffer, src_str, (n_copied+1)*sizeof(wchar_t)))
            {
                return -41;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, MF_COUNT_OF(dst_buffer) - 1, 1, src_str, 0, src_len);
            if (0 != n_copied)
            {
                return -5;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MF_COUNT_OF(dst_buffer), src_str, 0, src_len);
            if (src_len != n_copied)
            {
                return -6;
            }
            if (0 != memcmp(dst_buffer+731, src_str, (n_copied + 1)*sizeof(wchar_t)))
            {
                return -61;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MC_INVALID_SIZE, src_str, 0, src_len);
            if (src_len != n_copied)
            {
                return -7;
            }
            if (0 != memcmp(dst_buffer + 731, src_str, (n_copied + 1)*sizeof(wchar_t)))
            {
                return -71;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MF_COUNT_OF(dst_buffer), src_str, 7, src_len);
            if (src_len - 7 != n_copied)
            {
                return -8;
            }
            if (0 != memcmp(dst_buffer + 731, src_str+7, (n_copied + 1)*sizeof(wchar_t)))
            {
                return -81;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MC_INVALID_SIZE, src_str, 7, src_len);
            if (src_len - 7 != n_copied)
            {
                return -9;
            }
            if (0 != memcmp(dst_buffer + 731, src_str + 7, (n_copied + 1)*sizeof(wchar_t)))
            {
                return -91;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 731, MC_INVALID_SIZE, src_str, 7, MC_INVALID_SIZE);
            if (src_len - 7 != n_copied)
            {
                return -10;
            }
            if (0 != memcmp(dst_buffer + 731, src_str + 7, (n_copied + 1)*sizeof(wchar_t)))
            {
                return -101;
            }

            return 0;
        }

        pilo::i32_t functional_test_copy_1_char(void* param)
        {
            M_UNUSED(param);
            char dst_buffer[1024];
            size_t n_copied = 0;
            const char* src_str = "$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::copy((char*)nullptr, MF_COUNT_OF(dst_buffer), src_str, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer,  MF_COUNT_OF(dst_buffer), (const char*) nullptr, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::copy((char*) nullptr, MF_COUNT_OF(dst_buffer), (const char*) nullptr, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, MF_COUNT_OF(dst_buffer), src_str, src_len);
            if (src_len != n_copied)
            {
                return -4;
            }
            std::string str_left = dst_buffer;
            std::string str_right = src_str;
            if (0 != str_left.compare(str_right))
            {
                return -41;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, sizeof(dst_buffer)-1, 1, src_str, 0, src_len);
            if (0 != n_copied)
            {
                return -5;
            }

            return 0;
        }

        pilo::i32_t functional_test_copy_1_wchar(void* param)
        {
            M_UNUSED(param);
            wchar_t dst_buffer[1024];
            size_t n_copied = 0;
            const wchar_t* src_str = L"$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::copy((wchar_t*)nullptr, MF_COUNT_OF(dst_buffer), src_str, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr,  10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::copy((wchar_t*) nullptr, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, MF_COUNT_OF(dst_buffer), src_str, src_len);
            if (src_len != n_copied)
            {
                return -4;
            }

            if (0 != memcmp(dst_buffer, src_str, (n_copied + 1)*sizeof(wchar_t)))
            {
                return -41;
            }

            n_copied = pilo::core::string::string_util::copy(dst_buffer, 1, src_str, src_len);
            if (0 != n_copied)
            {
                return -5;
            }

            return 0;
        }

        pilo::i32_t functional_test_m_copy_0_char(void* param)
        {
            M_UNUSED(param);
            char dst_buffer[1024];
            size_t n_copied = 0;
            const char* src_str = "$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::m_copy((char*)nullptr, 0, MF_COUNT_OF(dst_buffer), src_str, 0, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 0, MF_COUNT_OF(dst_buffer), (const char*) nullptr, 0, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::m_copy((char*) nullptr, 0, MF_COUNT_OF(dst_buffer), (const char*) nullptr, 0, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::m_copy((char*)dst_buffer, MF_COUNT_OF(dst_buffer), MF_COUNT_OF(dst_buffer), (const char*)src_str, 0, src_len);
            if (0 != n_copied)
            {
                return -4;
            }

            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, MF_COUNT_OF(dst_buffer)-1, MF_COUNT_OF(dst_buffer), src_str, 0, src_len);
            if (1 != n_copied)
            {
                return -5;
            }
            if (dst_buffer[MF_COUNT_OF(dst_buffer) - 1] != src_str[0])
            {
                return -51;
            }

            dst_buffer[15] = '^';
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 0, 15, src_str, 0, MC_INVALID_SIZE);
            if (15 != n_copied)
            {
                return -6;
            }
            if (0 != memcmp(dst_buffer, src_str, n_copied))
            {
                return -61;
            }
            if (dst_buffer[n_copied] != '^')
            {
                return -62;
            }

            dst_buffer[10] = '^';
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 0, 15, src_str, 0, 10);
            if (10 != n_copied)
            {
                return -7;
            }
            if (0 != memcmp(dst_buffer, src_str, n_copied))
            {
                return -71;
            }
            if (dst_buffer[n_copied] != '^')
            {
                return -72;
            }

            memset(dst_buffer, '^', sizeof(dst_buffer));
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 3, 15, src_str, 5, 10);
            if (10 != n_copied)
            {
                return -8;
            }
            if (0 != memcmp(dst_buffer+3, src_str+5, n_copied))
            {
                return -81;
            }
            if (dst_buffer[n_copied+3] != '^')
            {
                return -82;
            }
            char cmp_buffer[1024];
            memset(cmp_buffer, '^', sizeof(cmp_buffer));
            if (0 != memcmp(dst_buffer, cmp_buffer, 3))
            {
                return -83;
            }

            memset(dst_buffer, '^', sizeof(dst_buffer));
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 3, MC_INVALID_SIZE, src_str, 5, 10);
            if (10 != n_copied)
            {
                return -9;
            }
            if (0 != memcmp(dst_buffer + 3, src_str + 5, n_copied))
            {
                return -91;
            }
            if (dst_buffer[n_copied + 3] != '^')
            {
                return -92;
            }
            if (0 != memcmp(dst_buffer, cmp_buffer, 3))
            {
                return -93;
            }

            return 0;
        }

        pilo::i32_t functional_test_m_copy_0_wchar(void* param)
        {
            M_UNUSED(param);
            wchar_t dst_buffer[1024];
            size_t n_copied = 0;
            const wchar_t* src_str = L"$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::m_copy((wchar_t*)nullptr, 0, MF_COUNT_OF(dst_buffer), src_str, 0, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 0, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, 0, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::m_copy((wchar_t*) nullptr, 0, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, 0, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::m_copy((wchar_t*)dst_buffer, MF_COUNT_OF(dst_buffer), MF_COUNT_OF(dst_buffer), (const wchar_t*)src_str, 0, src_len);
            if (0 != n_copied)
            {
                return -4;
            }

            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, MF_COUNT_OF(dst_buffer) - 1, MF_COUNT_OF(dst_buffer), src_str, 0, src_len);
            if (1 != n_copied)
            {
                return -5;
            }
            if (dst_buffer[MF_COUNT_OF(dst_buffer) - 1] != src_str[0])
            {
                return -51;
            }

            dst_buffer[15] = '^';
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 0, 15, src_str, 0, MC_INVALID_SIZE);
            if (15 != n_copied)
            {
                return -6;
            }
            if (0 != wmemcmp(dst_buffer, src_str, n_copied))
            {
                return -61;
            }
            if (dst_buffer[n_copied] != '^')
            {
                return -62;
            }

            dst_buffer[10] = '^';
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 0, 15, src_str, 0, 10);
            if (10 != n_copied)
            {
                return -7;
            }
            if (0 != wmemcmp(dst_buffer, src_str, n_copied))
            {
                return -71;
            }
            if (dst_buffer[n_copied] != '^')
            {
                return -72;
            }

            for (int j = 0; j < (int) MF_COUNT_OF(dst_buffer); j++)
            {
                dst_buffer[j] = '^';
            }
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 3, 15, src_str, 5, 10);
            if (10 != n_copied)
            {
                return -8;
            }
            if (0 != wmemcmp(dst_buffer + 3, src_str + 5, n_copied))
            {
                return -81;
            }
            if (dst_buffer[n_copied + 3] != '^')
            {
                return -82;
            }

            wchar_t cmp_buffer[1024];
            for (int j = 0; j < (int) MF_COUNT_OF(cmp_buffer); j++)
            {
                cmp_buffer[j] = '^';
            }
            if (0 != wmemcmp(dst_buffer, cmp_buffer, 3))
            {
                return -83;
            }

            for (int j = 0; j < (int) MF_COUNT_OF(dst_buffer); j++)
            {
                dst_buffer[j] = '^';
            }
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, 3, MC_INVALID_SIZE, src_str, 5, 10);
            if (10 != n_copied)
            {
                return -9;
            }
            if (0 != wmemcmp(dst_buffer + 3, src_str + 5, n_copied))
            {
                return -91;
            }
            if (dst_buffer[n_copied + 3] != '^')
            {
                return -92;
            }
            if (0 != wmemcmp(dst_buffer, cmp_buffer, 3))
            {
                return -93;
            }

            return 0;
        }

        pilo::i32_t functional_test_m_copy_1_char(void* param)
        {
            M_UNUSED(param);

            M_UNUSED(param);
            char dst_buffer[1024];
            size_t n_copied = 0;
            const char* src_str = "$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::m_copy((char*)nullptr, MF_COUNT_OF(dst_buffer), src_str, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, MF_COUNT_OF(dst_buffer), (const char*) nullptr, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::m_copy((char*) nullptr,MF_COUNT_OF(dst_buffer), (const char*) nullptr, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::m_copy((char*)dst_buffer, MF_COUNT_OF(dst_buffer), (const char*)src_str,  src_len);
            if (27 != n_copied)
            {
                return -4;
            }
            if (::memcmp(dst_buffer, src_str, n_copied) != 0)
            {
                return -41;
            }

            memset(dst_buffer, '^', sizeof(dst_buffer));
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer+3, MC_INVALID_SIZE, src_str + 5, 10);
            if (10 != n_copied)
            {
                return -9;
            }
            if (0 != memcmp(dst_buffer + 3, src_str + 5, n_copied))
            {
                return -91;
            }
            if (dst_buffer[n_copied + 3] != '^')
            {
                return -92;
            }
            char cmp_buffer[1024];
            memset(cmp_buffer, '^', sizeof(cmp_buffer));
            if (0 != memcmp(dst_buffer, cmp_buffer, 3))
            {
                return -93;
            }

            return 0;
        }

        pilo::i32_t functional_test_m_copy_1_wchar(void* param)
        {
            M_UNUSED(param);
            wchar_t dst_buffer[1024];
            size_t n_copied = 0;
            const wchar_t* src_str = L"$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::m_copy((wchar_t*)nullptr, MF_COUNT_OF(dst_buffer), src_str, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::m_copy(dst_buffer,MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::m_copy((wchar_t*) nullptr,  MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::m_copy((wchar_t*)dst_buffer, MF_COUNT_OF(dst_buffer), MF_COUNT_OF(dst_buffer), (const wchar_t*)src_str, 0, src_len);
            if (0 != n_copied)
            {
                return -4;
            }

            n_copied = pilo::core::string::string_util::m_copy(dst_buffer, MF_COUNT_OF(dst_buffer), src_str,  src_len);
            if (27 != n_copied)
            {
                return -5;
            }
            if (::wmemcmp(dst_buffer, src_str,n_copied))
            {
                return -51;
            }
          
            for (int j = 0; j < (int) MF_COUNT_OF(dst_buffer); j++)
            {
                dst_buffer[j] = '^';
            }
            n_copied = pilo::core::string::string_util::m_copy(dst_buffer+3, MC_INVALID_SIZE, src_str+5, 10);
            if (10 != n_copied)
            {
                return -9;
            }
            if (0 != wmemcmp(dst_buffer + 3, src_str + 5, n_copied))
            {
                return -91;
            }
            if (dst_buffer[n_copied + 3] != '^')
            {
                return -92;
            }

            wchar_t cmp_buffer[1024];
            for (int j = 0; j < (int) MF_COUNT_OF(cmp_buffer); j++)
            {
                cmp_buffer[j] = '^';
            }
            if (0 != wmemcmp(dst_buffer, cmp_buffer, 3))
            {
                return -93;
            }

            return 0;
        }

        pilo::i32_t functional_test_m_move_0_char(void* param)
        {
            M_UNUSED(param);
            char dst_buffer[1024];
            size_t n_copied = 0;
            const char* src_str = "$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::m_move((char*)nullptr, 0, MF_COUNT_OF(dst_buffer), src_str, 0, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 0, MF_COUNT_OF(dst_buffer), (const char*) nullptr, 0, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::m_move((char*) nullptr, 0, MF_COUNT_OF(dst_buffer), (const char*) nullptr, 0, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 0, MC_INVALID_SIZE, src_str, 0, MC_INVALID_SIZE);
            if (0 != n_copied)
            {
                return -4;
            }

            ::memcpy(dst_buffer, src_str, src_len * sizeof(dst_buffer[0]));
            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 0, MC_INVALID_SIZE, dst_buffer, 5, 10);
            if (10 != n_copied)
            {
                return -5;
            }
            if (0 != memcmp(dst_buffer, src_str+5, n_copied*sizeof(dst_buffer[0])))
            {
                return -51;
            }
            if (0 != memcmp(dst_buffer + 10, src_str + 10, (src_len-n_copied)*sizeof(dst_buffer[0])))
            {
                return -52;
            }

            memset(dst_buffer, 0x00, sizeof(dst_buffer));
            ::memcpy(dst_buffer, src_str, src_len * sizeof(dst_buffer[0]));
            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 7, MC_INVALID_SIZE, dst_buffer, 5, 10);
            if (10 != n_copied)
            {
                return -6;
            }
            if (0 != memcmp(dst_buffer+7, src_str + 5, n_copied*sizeof(dst_buffer[0])))
            {
                return -61;
            }
            if (0 != memcmp(dst_buffer + 17, src_str + 17, (src_len - n_copied -7)*sizeof(dst_buffer[0])))
            {
                return -62;
            }
            
            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 0, MF_COUNT_OF(dst_buffer), src_str, 0, MC_INVALID_SIZE);
            if (0 != n_copied)
            {
                return -7;
            }

            return 0;
        }

        pilo::i32_t functional_test_m_move_0_wchar(void* param)
        {
            M_UNUSED(param);
            wchar_t dst_buffer[1024];
            size_t n_copied = 0;
            const wchar_t* src_str = L"$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::m_move((wchar_t*)nullptr, 0, MF_COUNT_OF(dst_buffer), src_str, 0, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 0, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, 0, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::m_move((wchar_t*) nullptr, 0, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, 0, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 0, MC_INVALID_SIZE, src_str, 0, MC_INVALID_SIZE);
            if (0 != n_copied)
            {
                return -4;
            }

            ::memcpy(dst_buffer, src_str, src_len * sizeof(dst_buffer[0]));
            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 0, MC_INVALID_SIZE, dst_buffer, 5, 10);
            if (10 != n_copied)
            {
                return -5;
            }
            if (0 != memcmp(dst_buffer, src_str + 5, n_copied*sizeof(dst_buffer[0])))
            {
                return -51;
            }
            if (0 != memcmp(dst_buffer + 10, src_str + 10, (src_len - n_copied)*sizeof(dst_buffer[0])))
            {
                return -52;
            }

            memset(dst_buffer, 0x00, sizeof(dst_buffer));
            ::memcpy(dst_buffer, src_str, src_len * sizeof(dst_buffer[0]));
            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 7, MC_INVALID_SIZE, dst_buffer, 5, 10);
            if (10 != n_copied)
            {
                return -6;
            }
            if (0 != memcmp(dst_buffer + 7, src_str + 5, n_copied*sizeof(dst_buffer[0])))
            {
                return -61;
            }
            if (0 != memcmp(dst_buffer + 17, src_str + 17, (src_len - n_copied - 7)*sizeof(dst_buffer[0])))
            {
                return -62;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, 0, MF_COUNT_OF(dst_buffer), src_str, 0, MC_INVALID_SIZE);
            if (0 != n_copied)
            {
                return -7;
            }

            return 0;
        }

        pilo::i32_t functional_test_m_move_1_char(void* param)
        {
            M_UNUSED(param);
            char dst_buffer[1024];
            size_t n_copied = 0;
            const char* src_str = "$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::m_move((char*)nullptr, MF_COUNT_OF(dst_buffer), src_str,10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, MF_COUNT_OF(dst_buffer), (const char*) nullptr, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::m_move((char*) nullptr, MF_COUNT_OF(dst_buffer), (const char*) nullptr, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, MC_INVALID_SIZE, src_str, MC_INVALID_SIZE);
            if (0 != n_copied)
            {
                return -4;
            }

            ::memcpy(dst_buffer, src_str, src_len * sizeof(dst_buffer[0]));
            n_copied = pilo::core::string::string_util::m_move(dst_buffer, MC_INVALID_SIZE, dst_buffer+5, 10);
            if (10 != n_copied)
            {
                return -5;
            }
            if (0 != memcmp(dst_buffer, src_str + 5, n_copied*sizeof(dst_buffer[0])))
            {
                return -51;
            }
            if (0 != memcmp(dst_buffer + 10, src_str + 10, (src_len - n_copied)*sizeof(dst_buffer[0])))
            {
                return -52;
            }

            memset(dst_buffer, 0x00, sizeof(dst_buffer));
            ::memcpy(dst_buffer, src_str, src_len * sizeof(dst_buffer[0]));
            n_copied = pilo::core::string::string_util::m_move(dst_buffer+7, MC_INVALID_SIZE, dst_buffer+5, 10);
            if (10 != n_copied)
            {
                return -6;
            }
            if (0 != memcmp(dst_buffer + 7, src_str + 5, n_copied*sizeof(dst_buffer[0])))
            {
                return -61;
            }
            if (0 != memcmp(dst_buffer + 17, src_str + 17, (src_len - n_copied - 7)*sizeof(dst_buffer[0])))
            {
                return -62;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, MF_COUNT_OF(dst_buffer), src_str, MC_INVALID_SIZE);
            if (0 != n_copied)
            {
                return -7;
            }

            return 0;
        }

        pilo::i32_t functional_test_m_move_1_wchar(void* param)
        {
            M_UNUSED(param);
            wchar_t dst_buffer[1024];
            size_t n_copied = 0;
            const wchar_t* src_str = L"$howdy, fox, how is life? #";
            size_t src_len = pilo::core::string::string_util::length(src_str);
            M_ASSERT(src_len > 0);

            n_copied = pilo::core::string::string_util::m_move((wchar_t*)nullptr, MF_COUNT_OF(dst_buffer), src_str, 10);
            if (0 != n_copied)
            {
                return -1;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, 10);
            if (0 != n_copied)
            {
                return -2;
            }

            n_copied = pilo::core::string::string_util::m_move((wchar_t*) nullptr, MF_COUNT_OF(dst_buffer), (const wchar_t*) nullptr, src_len);
            if (0 != n_copied)
            {
                return -3;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer,  MC_INVALID_SIZE, src_str, MC_INVALID_SIZE);
            if (0 != n_copied)
            {
                return -4;
            }

            ::memcpy(dst_buffer, src_str, src_len * sizeof(dst_buffer[0]));
            n_copied = pilo::core::string::string_util::m_move(dst_buffer, MC_INVALID_SIZE, dst_buffer+5, 10);
            if (10 != n_copied)
            {
                return -5;
            }
            if (0 != memcmp(dst_buffer, src_str + 5, n_copied*sizeof(dst_buffer[0])))
            {
                return -51;
            }
            if (0 != memcmp(dst_buffer + 10, src_str + 10, (src_len - n_copied)*sizeof(dst_buffer[0])))
            {
                return -52;
            }

            memset(dst_buffer, 0x00, sizeof(dst_buffer));
            ::memcpy(dst_buffer, src_str, src_len * sizeof(dst_buffer[0]));
            n_copied = pilo::core::string::string_util::m_move(dst_buffer+7, MC_INVALID_SIZE, dst_buffer+5, 10);
            if (10 != n_copied)
            {
                return -6;
            }
            if (0 != memcmp(dst_buffer + 7, src_str + 5, n_copied*sizeof(dst_buffer[0])))
            {
                return -61;
            }
            if (0 != memcmp(dst_buffer + 17, src_str + 17, (src_len - n_copied - 7)*sizeof(dst_buffer[0])))
            {
                return -62;
            }

            n_copied = pilo::core::string::string_util::m_move(dst_buffer, MF_COUNT_OF(dst_buffer), src_str,MC_INVALID_SIZE);
            if (0 != n_copied)
            {
                return -7;
            }

            return 0;
        }

        pilo::i32_t functional_test_set_char(void* param)
        {
            M_UNUSED(param);
            char buffer[1025];
            char cmp_buffer[1025];
            size_t len = sizeof(cmp_buffer);
            memset(cmp_buffer, 0x00, len);
            memset(cmp_buffer, '1', len - 1);
            pilo::core::string::string_util::set(buffer, '1', MF_COUNT_OF(buffer)-1);
            if (::memcmp(cmp_buffer, buffer, len) != 0)
            {
                return -1;
            }

            return 0;
        }

        pilo::i32_t functional_test_set_wchar(void* param)
        {
            M_UNUSED(param);
            wchar_t buffer[1025];
            wchar_t cmp_buffer[1025];
            size_t len = sizeof(cmp_buffer)/sizeof(wchar_t);
            memset(cmp_buffer, 0x00, sizeof(cmp_buffer));
            ::wmemset(cmp_buffer, '1', len - 1);
            pilo::core::string::string_util::set(buffer, '1', MF_COUNT_OF(buffer)-1);
            if (::memcmp(cmp_buffer, buffer, len) != 0)
            {
                return -1;
            }
            return 0;
        }

        pilo::i32_t functional_test_m_set_char(void* param)
        {
            M_UNUSED(param);

            char buffer[1025];
            char cmp_buffer[1025];

            memset(cmp_buffer, '1', MF_COUNT_OF(buffer));
            pilo::core::string::string_util::set(buffer, '1', MF_COUNT_OF(buffer));

            if (::memcmp(cmp_buffer, buffer, MF_COUNT_OF(buffer)) != 0)
            {
                return -1;
            }

            return 0;
        }

        pilo::i32_t functional_test_m_set_wchar(void* param)
        {
            M_UNUSED(param);

            wchar_t buffer[1025];
            wchar_t cmp_buffer[1025];

            for (size_t i = 0; i < MF_COUNT_OF(buffer); i++)
            {
                cmp_buffer[i] = '1';
            }
            pilo::core::string::string_util::set(buffer, '1', MF_COUNT_OF(buffer));

            if (::memcmp(cmp_buffer, buffer, MF_COUNT_OF(buffer) *sizeof(wchar_t)) != 0)
            {
                return -1;
            }

            return 0;
        }

        pilo::i32_t functional_test_number_to_string_integer(void* param)
        {
            M_UNUSED(param);
            pilo::i32_t test_i = (pilo::i32_t) 618446112;
            char result_buffer[128];
            size_t ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), test_i);
            if (ret != 9)
            {
                return -1;
            }
            if (::strncmp(result_buffer, "618446112", pilo::core::string::string_util::length("618446112")) != 0)
            {
                return -2;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), 7, 9);
            if (ret != 9)
            {
                return -3;
            }
            if (::strncmp(result_buffer, "        7", pilo::core::string::string_util::length("        7")) != 0)
            {
                return -4;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), 7, 9, true);
            if (ret != 9)
            {
                return -5;
            }
            if (::strncmp(result_buffer, "000000007", pilo::core::string::string_util::length("000000007")) != 0)
            {
                return -6;
            }

            wchar_t result_buffer_w[128];
            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), test_i);
            if (ret != 9)
            {
                return -7;
            }
            if (::memcmp(result_buffer_w, L"618446112", pilo::core::string::string_util::length(L"618446112")*sizeof(wchar_t)) != 0)
            {
                return -8;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), 7, 9);
            if (ret != 9)
            {
                return -9;
            }
            if (::memcmp(result_buffer_w, L"        7", pilo::core::string::string_util::length(L"        7") * 2) != 0)
            {
                return -10;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), 7, 9, true);
            if (ret != 9)
            {
                return -11;
            }
            size_t tmplen = pilo::core::string::string_util::length(L"000000007")*sizeof(wchar_t);
            if (::memcmp(result_buffer_w, L"000000007", tmplen) != 0)
            {
                return -12;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), -87654321);
            if (ret != 9)
            {
                return -13;
            }
            if (::memcmp(result_buffer, "-87654321", 9) != 0)
            {
                return -14;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), -87654321);
            if (ret != 9)
            {
                return -15;
            }
            if (::memcmp(result_buffer_w, L"-87654321", 9 * 2) != 0)
            {
                return -16;
            }


            pilo::i64_t test_lli = 8618618446112;
            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), test_lli);
            if (ret != 13)
            {
                return -21;
            }
            if (::strncmp(result_buffer, "8618618446112", pilo::core::string::string_util::length("8618618446112")) != 0)
            {
                return -22;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), 7, 15);
            if (ret != 15)
            {
                return -23;
            }
            if (::strncmp(result_buffer, "              7", pilo::core::string::string_util::length("              7")) != 0)
            {
                return -24;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), 7, 15, true);
            if (ret != 15)
            {
                return -25;
            }
            if (::strncmp(result_buffer, "000000000000007", pilo::core::string::string_util::length("000000000000007")) != 0)
            {
                return -26;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), test_lli);
            if (ret != 13)
            {
                return -27;
            }
            if (::memcmp(result_buffer_w, L"8618618446112", pilo::core::string::string_util::length(L"8618618446112")*sizeof(wchar_t)) != 0)
            {
                return -28;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), 7, 15);
            if (ret != 15)
            {
                return -29;
            }
            if (::memcmp(result_buffer_w, L"              7", pilo::core::string::string_util::length(L"              7") * 2) != 0)
            {
                return -30;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), 7, 15, true);
            if (ret != 15)
            {
                return -31;
            }
            tmplen = pilo::core::string::string_util::length(L"000000000000007")*sizeof(wchar_t);
            if (::memcmp(result_buffer_w, L"000000000000007", tmplen) != 0)
            {
                return -32;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), -1234567887654321LL);
            if (ret != 17)
            {
                return -33;
            }
            if (::memcmp(result_buffer, "-1234567887654321", 17) != 0)
            {
                return -34;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), -1234567887654321LL);
            if (ret != 17)
            {
                return -35;
            }
            if (::memcmp(result_buffer_w, L"-1234567887654321", 17 * 2) != 0)
            {
                return -36;
            }

            //unsigned 
            pilo::u32_t test_u = (pilo::u32_t) 3618446112U;
            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), test_u);
            if (ret != 10)
            {
                return -101;
            }
            if (::strncmp(result_buffer, "3618446112", pilo::core::string::string_util::length("3618446112")) != 0)
            {
                return -102;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), 7, 10);
            if (ret != 10)
            {
                return -103;
            }
            if (::strncmp(result_buffer, "         7", pilo::core::string::string_util::length("         7")) != 0)
            {
                return -104;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer, MF_COUNT_OF(result_buffer), 7, 10, true);
            if (ret != 10)
            {
                return -105;
            }
            if (::strncmp(result_buffer, "0000000007", pilo::core::string::string_util::length("0000000007")) != 0)
            {
                return -106;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), test_u);
            if (ret != 10)
            {
                return -107;
            }
            if (::memcmp(result_buffer_w, L"3618446112", pilo::core::string::string_util::length(L"3618446112")*sizeof(wchar_t)) != 0)
            {
                return -108;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), 7, 10);
            if (ret != 10)
            {
                return -109;
            }
            if (::memcmp(result_buffer_w, L"         7", pilo::core::string::string_util::length(L"         7") * 2) != 0)
            {
                return -110;
            }

            ret = pilo::core::string::string_util::number_to_string(result_buffer_w, MF_COUNT_OF(result_buffer_w), 7, 10, true);
            if (ret != 10)
            {
                return -111;
            }
            tmplen = pilo::core::string::string_util::length(L"0000000007")*sizeof(wchar_t);
            if (::memcmp(result_buffer_w, L"0000000007", tmplen) != 0)
            {
                return -112;
            }

            return 0;
        }

        pilo::i32_t functional_test_number_to_string_float(void* param)
        {
            M_UNUSED(param);

            float f = -12345.567f;
            size_t ret = 0;
            char dst_buffer[128];
            wchar_t dst_buffer_w[128];

            //char
            ret = pilo::core::string::string_util::number_to_string(dst_buffer, MF_COUNT_OF(dst_buffer), f, "5.3");
            if (ret != 10)
            {
                return -1;
            }
            if (::memcmp(dst_buffer, "-12345.567", ret))
            {
                return -2;
            }

            f = -2345.567f;
            ret = pilo::core::string::string_util::number_to_string(dst_buffer, MF_COUNT_OF(dst_buffer), f, "10.3", true);
            if (ret != 10)
            {
                return -3;
            }
            if (::memcmp(dst_buffer, "-02345.567", ret))
            {
                return -4;
            }

            //wchar
            f = -12345.567f;
            ret = pilo::core::string::string_util::number_to_string(dst_buffer_w, MF_COUNT_OF(dst_buffer_w), f, L"5.3");

            if (::memcmp(dst_buffer_w, L"-12345.567", 10))
            {
                return -12;
            }

            f = -2345.567f;

            ret = pilo::core::string::string_util::number_to_string(dst_buffer_w, MF_COUNT_OF(dst_buffer_w), f, L"10.3", true);
            if (ret != 10)
            {
                wprintf(dst_buffer_w);
                return -13;
            }
            if (::memcmp(dst_buffer_w, L"-02345.567", ret * 2))
            {
                return -14;
            }

            return 0;
        }

    }
}