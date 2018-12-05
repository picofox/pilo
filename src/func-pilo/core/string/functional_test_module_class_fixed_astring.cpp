
#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_fixed_astring.hpp"
#include <string>

#define MC_STR0_SIZE    8192
namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_constructor_0_char(void* param);
        static pilo::i32_t functional_test_constructor_1_char(void* param);
        static pilo::i32_t functional_test_constructor_2_char(void* param);
        static pilo::i32_t functional_test_constructor_3_char(void* param);
        static pilo::i32_t functional_test_constructor_4_char(void* param);
        static pilo::i32_t functional_test_constructor_5_char(void* param);
        static pilo::i32_t functional_test_constructor_6_char(void* param);
        static pilo::i32_t functional_test_copy_operator_0(void* param); 
        static pilo::i32_t functional_test_copy_operator_1(void* param);
        static pilo::i32_t functional_test_copy_operator_2(void* param);
        static pilo::i32_t functional_test_copy_operator_3(void* param);
        static pilo::i32_t functional_test_copy_operator_4(void* param);
        static pilo::i32_t functional_test_assign_0(void* param);
        static pilo::i32_t functional_test_assign_1(void* param);
        static pilo::i32_t functional_test_assign_2(void* param);
        static pilo::i32_t functional_test_assign_3(void* param);

        

        pilo::test::testing_case g_functional_cases_fixed_astring[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "fixed_astring<char>()                         ", nullptr, functional_test_constructor_0_char, 0, -1, (pilo::u32_t) - 1 },
            { 2, "fixed_astring<char>(const char*)              ", nullptr, functional_test_constructor_1_char, 0, -1, (pilo::u32_t) - 1 },
            { 3, "fixed_astring<char>(const char*, size_t)      ", nullptr, functional_test_constructor_2_char, 0, -1, (pilo::u32_t) - 1 },
            { 4, "fixed_astring<char>(const fixed_astring&)     ", nullptr, functional_test_constructor_3_char, 0, -1, (pilo::u32_t) - 1 },
            { 5, "fixed_astring<char>(const std::string &)      ", nullptr, functional_test_constructor_4_char, 0, -1, (pilo::u32_t) - 1 },
            { 6, "fixed_astring<char>(int)                      ", nullptr, functional_test_constructor_5_char, 0, -1, (pilo::u32_t) - 1 },
            { 7, "fixed_astring<char>(float)                    ", nullptr, functional_test_constructor_6_char, 0, -1, (pilo::u32_t) - 1 },
            { 8, "operator=<fixed_astring>(fixed_astring)       ", nullptr, functional_test_copy_operator_0,    0, -1, (pilo::u32_t) - 1 },
            { 9, "operator=<std::astring>(std::string)          ", nullptr, functional_test_copy_operator_1,    0, -1, (pilo::u32_t) - 1 },
            {10, "operator=<std::astring>(const char*)          ", nullptr, functional_test_copy_operator_2,    0, -1, (pilo::u32_t) - 1 },
            {11, "operator=<std::astring>(char)                 ", nullptr, functional_test_copy_operator_3,    0, -1, (pilo::u32_t) - 1 },
            {12, "operator=<std::astring>(num)                  ", nullptr, functional_test_copy_operator_4,    0, -1, (pilo::u32_t) - 1 },
            {13, "assign(size_t, char   )                       ", nullptr, functional_test_assign_0,           0, -1, (pilo::u32_t) - 1 },
            {14, "assign(const std::string&)                    ", nullptr, functional_test_assign_1,           0, -1, (pilo::u32_t) - 1 },
            {15, "assign(const char*)                           ", nullptr, functional_test_assign_2,           0, -1, (pilo::u32_t) - 1 },
            {16, "assign(const fixed_string&)                   ", nullptr, functional_test_assign_3,           0, -1, (pilo::u32_t) - 1 },

            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_test_constructor_0_char(void* param)
        {
            M_UNUSED(param);
            pilo::core::string::fixed_astring<MC_STR0_SIZE>  str0;
            if (str0.length() != 0)
            {
                return -1;
            }
            if (str0[0] != 0)
            {
                return -2;
            }
            if (str0.capacity() != MC_STR0_SIZE)
            {
                return -3;
            }

            if (!str0.empty())
            {
                return -4;
            }

            return 0;
        }

        pilo::i32_t functional_test_constructor_1_char(void* param)
        {
            M_UNUSED(param);
            char str_buffer[MC_STR0_SIZE];
            pilo::core::string::string_util::m_set(str_buffer, 0, MF_COUNT_OF(str_buffer));
            pilo::core::string::string_util::m_set(str_buffer, 1, MF_COUNT_OF(str_buffer) - 1);
            str_buffer[0] = '$';
            str_buffer[MC_STR0_SIZE - 2] = '#';
            pilo::core::string::fixed_astring<MC_STR0_SIZE>  str0(str_buffer);

            if (str0.length() != 8191)
            {
                return -1;
            }

            if (str0.size() != 8191)
            {
                return -2;
            }

            if (str0.empty())
            {
                return -4;
            }

            if (str0.front() != '$')
            {
                return -5;
            }

            if (str0.back() != '#')
            {
                return -6;
            }

            if (::memcmp(str0.c_str(), str_buffer, MC_STR0_SIZE) != 0)
            {
                return -7;
            }

            pilo::core::string::fixed_astring<MC_STR0_SIZE>  str1("");
            if (str1.length() != 0)
            {
                return -8;
            }

            pilo::core::string::fixed_astring<10>  str2 = "1234567890";
            if (::memcmp(str2.c_str(), "1234567890", 11) != 0)
            {
                return -9;
            }

            return 0;
        }


        pilo::i32_t functional_test_constructor_2_char(void* param)
        {
            M_UNUSED(param);
            char str_buffer[MC_STR0_SIZE + 1];
            pilo::core::string::string_util::set(str_buffer, '1', MC_STR0_SIZE);
            pilo::core::string::fixed_astring<MC_STR0_SIZE + 1>  str0(str_buffer, MC_STR0_SIZE);
            if (str0.size() != MC_STR0_SIZE)
            {
                return -1;
            }

            if (::memcmp(str_buffer, str0.c_str(), sizeof(str_buffer) != 0))
            {
                return -2;
            }

            pilo::core::string::fixed_astring<10>  str2("1234567890", 10);
            if (::memcmp(str2.c_str(), "1234567890", 11) != 0)
            {
                return -9;
            }
            if (str2.length() != 10)
            {
                return -10;
            }            

            return 0;
        }

        pilo::i32_t functional_test_constructor_3_char(void* param)
        {
            M_UNUSED(param);
            const char* str = "1234567890";
            size_t len = pilo::core::string::string_util::length(str);
            pilo::core::string::fixed_astring<32> str0(str, len);
            pilo::core::string::fixed_astring<64> str1(str0);
            if (str0.size() != str1.length())
            {
                return -1;
            }
            if (::memcmp(str0.c_str(), str1.c_str(), (len + 1)*sizeof(char)) != 0)
            {
                return -2;
            }

            const char* cstr2 = "012345678901234567890123456789";
            len = pilo::core::string::string_util::length(cstr2);
            pilo::core::string::fixed_astring<112> str2(cstr2);
            pilo::core::string::fixed_astring<32> str3(str2);
            if (str3.size() != len)
            {
                return -3;
            }
            if (::memcmp(str3.c_str(), str2.c_str(), (len + 1) * sizeof(char)) != 0)
            {
                return -4;
            }

            return 0;
        }

        pilo::i32_t functional_test_constructor_4_char(void* param)
        {
            M_UNUSED(param);

            std::string stdstr0 = "012345678901234567890123456789";
            pilo::core::string::fixed_astring<31>  str0(stdstr0);
            if (stdstr0.length() != stdstr0.length())
            {
                return -1;
            }
            if (::memcmp(stdstr0.c_str(), str0.c_str(), str0.size()) != 0)
            {
                return -2;
            }

            std::string stdstr1 = "0123456789";
            pilo::core::string::fixed_astring<11>  str3 = stdstr1;
            if (::memcmp(str3.c_str(), "0123456789", 11) != 0)
            {
                return -9;
            }
            if (str3.length() != 10)
            {
                return -10;
            }

            return 0;
        }

        pilo::i32_t functional_test_constructor_5_char(void* param)
        {
            M_UNUSED(param);
            pilo::core::string::fixed_astring<10> str0(1234567);
            if (str0.size() != 7)
            {
                return -1;
            }

            pilo::core::string::fixed_astring<10> str1 = 987654321;
            if (str1.size() != 9)
            {
                return -3;
            }

            str1 = str0;
            if (str1.size() != 7)
            {
                return -1;
            }

            pilo::core::string::fixed_astring<11> stri64(9876543210LL);
            if (stri64.size() != 10)
            {
                return -1;
            }

            pilo::u64_t u64vv= 19876543210U;
            pilo::core::string::fixed_astring<12> stru64(u64vv);
            if (stru64.size() != 11)
            {
                return -1;
            }

            return 0;
        }

        pilo::i32_t functional_test_constructor_6_char(void* param)
        {
            M_UNUSED(param);

            float fv1 = -789.123f;
            pilo::core::string::fixed_astring<32>  str0(fv1);
            float fv2 = (float) ::atof(str0.c_str());
            if (fv1 != fv2)
            {
                return -1;
            }

            double dv1 = -789.123456f;
            pilo::core::string::fixed_astring<32>  str1(dv1);
            double dv2 = (float) ::atof(str1.c_str());
            if (dv1 != dv2)
            {
                return -1;
            }

            return 0;
        }

        pilo::i32_t functional_test_copy_operator_0(void* param)
        {
            M_UNUSED(param);

            const char* cstr0 = "012345678901234567890123456789";
            pilo::core::string::fixed_astring<64> tstr0 = cstr0;
            pilo::core::string::fixed_astring<32> tstr1;
            tstr1 = tstr0;

            if (tstr1.size() != 30)
            {
                return -1;
            }

            if (::memcmp(tstr1.c_str(), cstr0, ::strlen(cstr0) + 1) != 0)
            {
                return -2;
            }

            return 0;
        }

        pilo::i32_t functional_test_copy_operator_1(void* param)
        {
            M_UNUSED(param);
            std::string stdstr0 = "0123456789";
            pilo::core::string::fixed_astring<32> fastr;
            fastr = stdstr0;

            if (fastr.length() != stdstr0.length())
            {
                return -1;
            }


            return 0;
        }

        pilo::i32_t functional_test_copy_operator_2(void* param)
        {
            M_UNUSED(param);
            pilo::core::string::fixed_astring<32> fastr;
            fastr = "0123456789";

            if (fastr.length() != 10)
            {
                return -1;
            }
            
            return 0;
        }

        pilo::i32_t functional_test_copy_operator_3(void* param)
        {
            M_UNUSED(param);
            pilo::core::string::fixed_astring<32> fastr;
            fastr = 'x';

            if (fastr.length() != 1)
            {
                return -1;
            }

            if (fastr[0] != 'x' || fastr[1] != 0)
            {
                return -2;
            }

            return 0;
        }

        pilo::i32_t functional_test_copy_operator_4(void* param)
        {
            M_UNUSED(param);
            pilo::core::string::fixed_astring<MC_STR0_SIZE>  str0;

            pilo::i32_t vi32 = -12345678;
            str0 = vi32;
           
            pilo::i64_t vi64 = -1234567890;
            str0 = vi64;
          

            pilo::u32_t vu32 = 82345678;
            str0 = vu32;

            pilo::u64_t vu64 = 11234567890;
            str0 = vu64;

            float fv = -235.238f;
            str0 = fv;
            float fv2 = (float) ::atof(str0.c_str());
            if (fv != fv2)
            {
                return -5;
            }

            double dv = 123.683909;
            str0 = dv;
            double dv2 = ::atof(str0.c_str());
            if (dv != dv2)
            {
                return -6;
            }
            return 0;
        }

        pilo::i32_t functional_test_assign_0(void* param)
        {
            M_UNUSED(param);
            pilo::core::string::fixed_astring<12>  str0;
            pilo::core::string::fixed_astring<12> str1 = str0.assign(12, 'x');
            if (str1.size() != 12)
            {
                return -1;
            }
            if (::memcmp(str1.c_str(), "xxxxxxxxxxxx", 13) != 0)
            {
                return -2;
            }
            return 0;
        }
        pilo::i32_t functional_test_assign_1(void* param)
        {
            M_UNUSED(param);
            std::string stdstr = "xxxxxxxxxxxx";
            pilo::core::string::fixed_astring<12>  str0;
            pilo::core::string::fixed_astring<12> str1 = str0.assign(stdstr);
            if (str1.size() != 12)
            {
                return -1;
            }
            if (::memcmp(str1.c_str(), stdstr.c_str(), 13) != 0)
            {
                return -2;
            }
            return 0;
        }
        pilo::i32_t functional_test_assign_2(void* param)
        {
            M_UNUSED(param);
            const char* cstr = "xxxxxxxxxxxx";
            pilo::core::string::fixed_astring<12>  str0;
            pilo::core::string::fixed_astring<12> str1 = str0.assign(cstr);
            if (str1.size() != 12)
            {
                return -1;
            }
            if (::memcmp(str1.c_str(), "xxxxxxxxxxxx", 13) != 0)
            {
                return -2;
            }
            return 0;
        }
        pilo::i32_t functional_test_assign_3(void* param)
        {
            M_UNUSED(param);
            pilo::core::string::fixed_astring<1024>  fstr = "xxxxxxxxxxxx";
            pilo::core::string::fixed_astring<12>  str0;
            pilo::core::string::fixed_astring<12> str1 = str0.assign(fstr);
            if (str1.size() != 12)
            {
                return -1;
            }
            if (::memcmp(str1.c_str(), fstr.c_str(), 13) != 0)
            {
                return -2;
            }
            return 0;
        }

    }
}