#include "core/string/string_util.hpp"
#include "core/string/astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_astring.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/string/dynamic_astring.hpp"
#include "core/string/auto_string.hpp"
#include <string>


namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_auto_astring(void* param);
        static pilo::i32_t functional_test_pure_dyn_astring(void* param);
        static pilo::i32_t functional_test_common(void* param);
        static pilo::i32_t functional_test_auto_string(void* param);

        pilo::test::testing_case g_functional_cases_auto_astring[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "astring<SZ>()                                ", nullptr, functional_test_auto_astring, 0, -1, (pilo::u32_t) - 1 },
            { 2, "astring<0>()                                 ", nullptr, functional_test_pure_dyn_astring, 0, -1, (pilo::u32_t) - 1 },
            { 3, "common                                       ", nullptr, functional_test_common, 0, -1, (pilo::u32_t) - 1 },
            { 4, "auto_string                                  ", nullptr, functional_test_auto_string, 0, -1, (pilo::u32_t) - 1 },
            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        template <typename T>
        ::pilo::i32_t __common_test_stdstring_and_astring()
        {
            T str;

            if (str.length() != 0)
            {
                return -10;
            }

            if (str.size() != 0)
            {
                return -20;
            }

            if (str.c_str()[0] != 0)
            {
                return -30;
            }

            str = "0123456789";
            if (str.compare("0123456789"))
            {
                return -40;
            }

            std::string strtmp ( "0123456789" );
            if (str.compare(strtmp))
            {
                return -50;
            }

            str.push_back('A');
            if (str.compare("0123456789A"))
            {
                return -51;
            }

            str.pop_back();
            if (str.compare("0123456789"))
            {
                return -52;
            }

            str = "01234567890123456789012345678901234567890123456789";
            if (str.compare("01234567890123456789012345678901234567890123456789"))
            {
                return -60;
            }

            strtmp = ("01234567890123456789012345678901234567890123456789");
            if (str.compare(strtmp))
            {
                return -70;
            }

            if (str.length() != 50)
            {
                return -80;
            }

            str.append("");
            if (str.compare(strtmp))
            {
                return -90;
            }

            
            str.append("abcdefghij");
            strtmp += "abcdefghij";
            if (str.compare(strtmp))
            {
                return -90;
            }

            str.assign("fox is ");
            str.insert(3,"'s friendXXXXX", 9);

            if (str.compare("fox's friend is "))
            {
                return -100;
            }

            str.insert(str.size(), "god", 3 );
            if (str.compare("fox's friend is god"))
            {
                return -110;
            }
            

            return 0;
        }

        template <typename T>
        ::pilo::i32_t __check_test_auto_string(T& astr, const typename T::value_type* content, size_t len, size_t capa, bool bDyn, ::pilo::i32_t ret)
        {
            if (0 != ::pilo::core::string::string_util::binary_compare(astr.c_str(), content))
            {
                return ret - 1;
            }

            if (astr.length() != len)
            {
                return ret - 2;
            }

            if (astr.capacity() != capa)
            {
                return ret - 3;
            }

            if (astr.is_dynamic() != bDyn)
            {
                return ret - 4;
            }

            return 0;
        }

        template <typename T>
        ::pilo::i32_t __check_test_std_string(T& stdstr, const typename T::value_type * content, size_t len, size_t capa, ::pilo::i32_t ret)
        {
            
            if (::pilo::core::string::string_util::binary_compare(stdstr.c_str(), content) != 0)
            {
                return ret - 7;
            }

            if (stdstr.length() != len)
            {
                return ret - 8;
            }

            if (stdstr.capacity() < capa)
            {
                return ret - 9;
            }

            return 0;
        }


        template <typename T>
        ::pilo::i32_t __test_auto_string()
        {
            ::pilo::core::string::auto_string<char, 0> str0;
            ::pilo::core::string::auto_string<char, 32> str1;
            std::string stdstr0;

            ::pilo::i32_t ret = -1;

            ret = __check_test_auto_string(str1, "", 0, 32, false, -10000);
            if (ret != 0)
            {
                return ret;
            }


            ret = __check_test_auto_string(str0, "", 0, 0, true, -10010);
            if (ret != 0)
            {
                return ret;
            }

            ret = __check_test_std_string(stdstr0, "", 0, 0, -10020);
            if (ret != 0)
            {
                return ret;
            }

            const char* sample_str = "i am a fox";
            const char* sample_str2 = "i am a";
            const char* sample_str3 = "I aM A";
            const char* sample_str4 = "i am a fox man";
            size_t sample_str_len = ::pilo::core::string::string_util::length(sample_str);
            size_t sample_str2_len = ::pilo::core::string::string_util::length(sample_str2);
            size_t sample_str4_len = ::pilo::core::string::string_util::length(sample_str4);

            pilo::core::string::auto_string<char, 0> str3(sample_str);
            ret = __check_test_auto_string(str3, sample_str, sample_str_len, 10, true, -10030);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<char, 32> str4(sample_str, 6);
            ret = __check_test_auto_string(str4, sample_str2, sample_str2_len, 32, false, -10040);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<char, 0> str5 = sample_str;
            ret = __check_test_auto_string(str5, sample_str, sample_str_len, 10, true, -10050);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<char, 32> str4_2 = str4;
            ret = __check_test_auto_string(str4_2, sample_str2, sample_str2_len, 32, false, -10060);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<char, 30> str4_3 = str4;
            ret = __check_test_auto_string(str4_3, sample_str2, sample_str2_len, 30, false, -10070);
            if (ret != 0)
            {
                return ret;
            }
            ::pilo::core::string::auto_string<char, 0> str4_4 = str4;
            ret = __check_test_auto_string(str4_4, sample_str2, sample_str2_len, sample_str2_len, true, -10080);
            if (ret != 0)
            {
                return ret;
            }

            if (str4_4[3] == 'm')   str4_4[3] = 'M';
            if (str4_4.front() == 'i') str4_4.front() = 'I';
            if (str4_4.back() == 'a') str4_4.back() = 'A';
            ret = __check_test_auto_string(str4_4, sample_str3, sample_str2_len, sample_str2_len, true, -10090);
            if (ret != 0)
            {
                return ret;
            }

            str5.push_back(' '); str5.push_back('m'); str5.push_back('a'); str5.push_back('n');
            ret = __check_test_auto_string(str5, sample_str4, sample_str4_len, sample_str4_len, true, -10100);
            if (ret != 0)
            {
                return ret;
            }

            str4.push_back(' '); str4.push_back('f'); str4.push_back('o'); str4.push_back('x');
            str4.push_back(' '); str4.push_back('m'); str4.push_back('a'); str4.push_back('n');
            ret = __check_test_auto_string(str4, sample_str4, sample_str4_len, 32, false, -10100);
            if (ret != 0)
            {
                return ret;
            }

            for (int i = 0; i < 4; i++)
            {
                str4.pop_back();
                str5.pop_back();
            }
            ret = __check_test_auto_string(str4, sample_str, sample_str_len, 32, false, -10100);
            if (ret != 0)
            {
                return ret;
            }
            ret = __check_test_auto_string(str5, sample_str, sample_str_len, 14, true, -10100);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<char, 0> str6;
            str6.append("you know, i am a fox",10,10);
            ret = __check_test_auto_string(str6, sample_str, sample_str_len, sample_str_len, true, -10110);
            if (ret != 0)
            {
                return ret;
            }

            str6.append(" man");
            ret = __check_test_auto_string(str6, sample_str4, sample_str4_len, sample_str4_len, true, -10120);
            if (ret != 0)
            {
                return ret;
            }

            str6.clear();
            str6 = "i am a";
            str6.append(std::string(" fox")); 
            ret = __check_test_auto_string(str6, sample_str, sample_str_len, 14, true, -10130);
            if (ret != 0)
            {
                return ret;
            }
            
            ::pilo::core::string::auto_string<char, 32> str7;
            str7.append("you know, i am a fox", 10, 10);
            ret = __check_test_auto_string(str7, sample_str, sample_str_len, 32, false, -10140);
            if (ret != 0)
            {
                return ret;
            }

            const char* sample_str7 = "i am a fox0123456789012345678901";
            size_t  sample_str7_len = ::pilo::core::string::string_util::length(sample_str7);
            str7.append("0123456789012345678901");
            ret = __check_test_auto_string(str7, sample_str7, sample_str7_len, 32, false, -10150);
            if (ret != 0)
            {
                return ret;
            }
            const char* sample_str7_1 = "i am a fox0123456789012345678901ABCDEFGHIJ";
            size_t  sample_str71_len = ::pilo::core::string::string_util::length(sample_str7_1);
            str7.append("ABCDEFGHIJ");
            ret = __check_test_auto_string(str7, sample_str7_1, sample_str71_len, sample_str71_len, true, -10160);
            if (ret != 0)
            {
                return ret;
            }

            str6.clear();
            for (int i = 0; i < 10; i++)
            {
                char tmpbuffer[32] = { 0 };
                ::pilo::core::io::string_format_output(tmpbuffer, 32, "%d", i);
                str6 += tmpbuffer;
            }
            ret = __check_test_auto_string(str6, "0123456789", 10, 14, true, -10170);
            if (ret != 0)
            {
                return ret;
            }



            const char* sample_str7_2 = "hi, i am a fox0123456789012345678901ABCDEFGHIJ";
            str7.insert(0, "hi, 123456789", 4);
            ret = __check_test_auto_string(str7, sample_str7_2, sample_str71_len + 4, sample_str71_len + 4, true, -10200);
            if (ret != 0)
            {
                return ret;
            }
            const char* sample_str7_3 = "hi, i am a little fox0123456789012345678901ABCDEFGHIJ";
            str7.insert(10, " little", 7);
            ret = __check_test_auto_string(str7, sample_str7_3, sample_str71_len + 11, sample_str71_len + 11, true, -10210);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<char, 32> str8 = "";
            const char* sample_str8_1 = "i am a little fox";
            str8 = sample_str;
            str8.insert(6, " little", 7);
            ret = __check_test_auto_string(str8, sample_str8_1, 17, 32, false, -10410);
            if (ret != 0)
            {
                return ret;
            }
            
            str8.insert(4, " 01234567890123456789012345678", 30);
            const char* sample_str8_2 = "i am 01234567890123456789012345678 a little fox";
            size_t  sample_str82_len = ::pilo::core::string::string_util::length(sample_str8_2);
            ret = __check_test_auto_string(str8, sample_str8_2, sample_str82_len, sample_str82_len, true, -10420);
            if (ret != 0)
            {
                return ret;
            }     

            str6.clear();
            str7.clear();
            str6 = "0123456789";
            str8 = "0123456789";
            std::string stdtstr = str6.c_str();
            std::string stdfstr = "sdfa";
            if (str6 == str8)
            {

            }
            else
            {
                return -90000010;
            }

            if (str6 == "0123456789")
            {

            }
            else
            {
                return -90000020;
            }

            if ("0123456789" == str6)
            {

            }
            else
            {
                return -90000020;
            }

            if (stdtstr == str6)
            {

            }
            else
            {
                return -90000030;
            }

            if (str6 == stdtstr)
            {

            }
            else
            {
                return -90000030;
            }

            if (stdfstr != str6)
            {

            }
            else
            {
                return -90000030;
            }

            if (str6 != stdfstr)
            {

            }
            else
            {
                return -90000030;
            }

            if (str6 != str7)
            {

            }
            else
            {
                return -90000050;
            }

            if (str6 != "ghj")
            {

            }
            else
            {
                return -90000020;
            }

            if ("gjh" != str6)
            {

            }
            else
            {
                return -90000020;
            }

            return 0;
        }

        template  <>
        ::pilo::i32_t __test_auto_string<wchar_t>()
        {
            ::pilo::core::string::auto_string<wchar_t, 0> str0;
            ::pilo::core::string::auto_string<wchar_t, 32> str1;

            std::wstring stdwstr0;

            ::pilo::i32_t ret = -1;

            ret = __check_test_auto_string(str1, L"", 0, 32, false, -5000000);
            if (ret != 0)
            {
                return ret;
            }

            ret = __check_test_auto_string(str0, L"", 0, 0, true, -5000010);
            if (ret != 0)
            {
                return ret;
            }

            ret = __check_test_std_string(stdwstr0, L"", 0, 0, -5000020);
            if (ret != 0)
            {
                return ret;
            }

            const wchar_t* sample_str = L"i am a fox";
            const wchar_t* sample_str2 = L"i am a";
            const wchar_t* sample_str3 = L"I aM A";
            const wchar_t* sample_str4 = L"i am a fox man";
            size_t sample_str_len = ::pilo::core::string::string_util::length(sample_str);
            size_t sample_str2_len = ::pilo::core::string::string_util::length(sample_str2);
            size_t sample_str4_len = ::pilo::core::string::string_util::length(sample_str4);
            pilo::core::string::auto_string<wchar_t, 0> str3(sample_str);
            ret = __check_test_auto_string(str3, sample_str, sample_str_len, 10, true, -5000030);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<wchar_t, 32> str4(sample_str, 6);
            ret = __check_test_auto_string(str4, sample_str2, sample_str2_len, 32, false, -5000040);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<wchar_t, 0> str5 = sample_str;
            ret = __check_test_auto_string(str5, sample_str, sample_str_len, 10, true, -5000050);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<wchar_t, 32> str4_2 = str4;
            ret = __check_test_auto_string(str4_2, sample_str2, sample_str2_len, 32, false, -5000060);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<wchar_t, 30> str4_3 = str4;
            ret = __check_test_auto_string(str4_3, sample_str2, sample_str2_len, 30, false, -5000070);
            if (ret != 0)
            {
                return ret;
            }
            ::pilo::core::string::auto_string<wchar_t, 0> str4_4 = str4;
            ret = __check_test_auto_string(str4_4, sample_str2, sample_str2_len, sample_str2_len, true, -5000080);
            if (ret != 0)
            {
                return ret;
            }

            if (str4_4[3] == 'm')   str4_4[3] = 'M';
            if (str4_4.front() == 'i') str4_4.front() = 'I';
            if (str4_4.back() == 'a') str4_4.back() = 'A';
            ret = __check_test_auto_string(str4_4, sample_str3, sample_str2_len, sample_str2_len, true, -5000090);
            if (ret != 0)
            {
                return ret;
            }

            if (str4_4[3] == 'm')   str4_4[3] = 'M';
            if (str4_4.front() == 'i') str4_4.front() = 'I';
            if (str4_4.back() == 'a') str4_4.back() = 'A';
            ret = __check_test_auto_string(str4_4, sample_str3, sample_str2_len, sample_str2_len, true, -10090);
            if (ret != 0)
            {
                return ret;
            }

            str5.push_back(' '); str5.push_back('m'); str5.push_back('a'); str5.push_back('n');
            ret = __check_test_auto_string(str5, sample_str4, sample_str4_len, sample_str4_len, true, -10100);
            if (ret != 0)
            {
                return ret;
            }

            str4.push_back(' '); str4.push_back('f'); str4.push_back('o'); str4.push_back('x');
            str4.push_back(' '); str4.push_back('m'); str4.push_back('a'); str4.push_back('n');
            ret = __check_test_auto_string(str4, sample_str4, sample_str4_len, 32, false, -10100);
            if (ret != 0)
            {
                return ret;
            }

            for (int i = 0; i < 4; i++)
            {
                str4.pop_back();
                str5.pop_back();
            }
            ret = __check_test_auto_string(str4, sample_str, sample_str_len, 32, false, -10100);
            if (ret != 0)
            {
                return ret;
            }
            ret = __check_test_auto_string(str5, sample_str, sample_str_len, sample_str4_len, true, -10100);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<wchar_t, 0> str6;
            str6.append(L"you know, i am a fox", 10, 10);
            ret = __check_test_auto_string(str6, sample_str, sample_str_len, sample_str_len, true, -10110);
            if (ret != 0)
            {
                return ret;
            }

            str6.append(L" man");
            ret = __check_test_auto_string(str6, sample_str4, sample_str4_len, sample_str4_len, true, -10120);
            if (ret != 0)
            {
                return ret;
            }

            str6.clear();
            str6 = L"i am a";
            str6.append(std::wstring(L" fox"));
            ret = __check_test_auto_string(str6, sample_str, sample_str_len, 14, true, -10130);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<wchar_t, 32> str7;
            str7.append(L"you know, i am a fox", 10, 10);
            ret = __check_test_auto_string(str7, sample_str, sample_str_len, 32, false, -10140);
            if (ret != 0)
            {
                return ret;
            }

            const wchar_t* sample_str7 = L"i am a fox0123456789012345678901";
            size_t  sample_str7_len = ::pilo::core::string::string_util::length(sample_str7);
            str7.append(L"0123456789012345678901");
            ret = __check_test_auto_string(str7, sample_str7, sample_str7_len, 32, false, -10150);
            if (ret != 0)
            {
                return ret;
            }
            const wchar_t* sample_str7_1 = L"i am a fox0123456789012345678901ABCDEFGHIJ";
            size_t  sample_str71_len = ::pilo::core::string::string_util::length(sample_str7_1);
            str7.append(L"ABCDEFGHIJ");
            ret = __check_test_auto_string(str7, sample_str7_1, sample_str71_len, sample_str71_len, true, -10160);
            if (ret != 0)
            {
                return ret;
            }

            str6.clear();
            for (int i = 0; i <10; i++)
            {
                wchar_t tmpbuffer[32] = {0};
                ::pilo::core::io::string_format_output(tmpbuffer, 32, L"%d", i);
                str6 += tmpbuffer;
            }
            ret = __check_test_auto_string(str6, L"0123456789", 10, 14, true, -10170);
            if (ret != 0)
            {
                return ret;
            }

            const wchar_t* sample_str7_2 = L"hi, i am a fox0123456789012345678901ABCDEFGHIJ";
            str7.insert(0, L"hi, 123456789", 4);
            ret = __check_test_auto_string(str7, sample_str7_2, sample_str71_len + 4, sample_str71_len + 4, true, -10200);
            if (ret != 0)
            {
                return ret;
            }
            const wchar_t* sample_str7_3 = L"hi, i am a little fox0123456789012345678901ABCDEFGHIJ";
            str7.insert(10, L" little", 7);
            ret = __check_test_auto_string(str7, sample_str7_3, sample_str71_len + 11, sample_str71_len + 11, true, -10210);
            if (ret != 0)
            {
                return ret;
            }

            ::pilo::core::string::auto_string<wchar_t, 32> str8 = L"";
            const wchar_t* sample_str8_1 = L"i am a little fox";
            str8 = sample_str;
            str8.insert(6, L" little", 7);
            ret = __check_test_auto_string(str8, sample_str8_1, 17, 32, false, -10410);
            if (ret != 0)
            {
                return ret;
            }

            str8.insert(4, L" 01234567890123456789012345678", 30);
            const wchar_t* sample_str8_2 = L"i am 01234567890123456789012345678 a little fox";
            size_t  sample_str82_len = ::pilo::core::string::string_util::length(sample_str8_2);
            ret = __check_test_auto_string(str8, sample_str8_2, sample_str82_len, sample_str82_len, true, -10420);
            if (ret != 0)
            {
                return ret;
            }

            str6.clear();
            str7.clear();
            str6 = L"0123456789";
            str8 = L"0123456789";
            std::wstring stdtstr = str6.c_str();
            std::wstring stdfstr = L"sdfa";
            if (str6 == str8)
            {

            }
            else
            {
                return -90000010;
            }

            if (str6 == L"0123456789")
            {

            }
            else
            {
                return -90000020;
            }

            if (L"0123456789" == str6)
            {

            }
            else
            {
                return -90000020;
            }

            if (stdtstr == str6)
            {

            }
            else
            {
                return -90000030;
            }

            if (str6 == stdtstr)
            {

            }
            else
            {
                return -90000030;
            }

            if (stdfstr != str6)
            {

            }
            else
            {
                return -90000030;
            }

            if (str6 != stdfstr)
            {

            }
            else
            {
                return -90000030;
            }

            if (str6 != str7)
            {

            }
            else
            {
                return -90000050;
            }

            if (str6 != L"ghj")
            {

            }
            else
            {
                return -90000020;
            }

            if (L"gjh" != str6)
            {

            }
            else
            {
                return -90000020;
            }

            return 0;
        }

        pilo::i32_t functional_test_auto_string(void* param)
        {
            M_UNUSED(param);
    
            ::pilo::i32_t ret = __test_auto_string<char>();
            if (ret != 0)
            {
                return -1;
            }


            ret = __test_auto_string<wchar_t>();
            if (ret != 0)
            {
                return -100;
            }



            return 0;
        }

        pilo::i32_t functional_test_common(void* param)
        {
            M_UNUSED(param);


            ::pilo::i32_t ret = 0;
            //auto string
            ret = __common_test_stdstring_and_astring<::pilo::core::string::astring<32> >();
            if (ret != 0)
            {
                return ret;
            }
            
            


            //auto string <0>
            ret = __common_test_stdstring_and_astring<::pilo::core::string::astring<0>  >();
            if (ret != 0)
            {
                return ret - 1000000;
            }


            //std string
            ret = __common_test_stdstring_and_astring<std::string >();
            if (ret != 0)
            {
                return ret - 2000000;
            }

            return 0;
        }




        pilo::i32_t functional_test_auto_astring(void* param)
        {
            M_UNUSED(param);
            ::pilo::core::string::astring<32> astr0("fox is good guy");
            if (astr0.is_dynamic())
            {
                return -1;
            }
            if (astr0.length() != 15)
            {
                return -20;
            }
            if (astr0.available_capacity() != 17)
            {
                return -30;
            }
            astr0.clear();
            if (astr0.length() != 0)
            {
                return -40;
            }
            if (astr0.c_str()[0] != 0)
            {
                return -50;
            }

            astr0 = "01234567890123456789012345678901";
            if (astr0.is_dynamic())
            {
                return -60;
            }
            if (astr0.length() != 32)
            {
                return -70;
            }
            if (astr0.available_capacity() != 0)
            {
                return -80;
            }
            if (astr0.c_str()[31] != '1' || astr0.c_str()[32] != '\0')
            {
                return -90;
            }

            astr0 = "012345678901234567890123456789012";
            if (!astr0.is_dynamic())
            {
                return -100;
            }
            if (astr0.length() != 33)
            {
                return -110;
            }
            if (astr0.available_capacity() != 0)
            {
                return -120;
            }
            if (astr0.c_str()[32] != '2' || astr0.c_str()[33] != '\0')
            {
                return -130;
            }

            ::pilo::core::string::astring<32> astr1("0123456789012345678901234567890123456789", 32);
            if (astr1.is_dynamic())
            {
                return -140;
            }

            if (astr1.compare("01234567890123456789012345678901") != 0)
            {
                return -150;
            }

            if (astr1.compare("012345678901234567890123456789010", 33) == 0)
            {
                return -160;
            }

            if (astr1.compare("012345678901234567890123456789010", 33) == 0)
            {
                return -160;
            }

            std::string stdstr = "01234567890123456789012345678901";
            if (astr1.compare(stdstr) != 0 || stdstr.length() != 32)
            {
                return -170;
            }
            
            
            return 0;
        }

        pilo::i32_t functional_test_pure_dyn_astring(void* param)
        {
            M_UNUSED(param);
            ::pilo::core::string::astring<0> astr0("fox is good guy");
            if (!astr0.is_dynamic())
            {
                return -1;
            }
            if (astr0.length() != 15)
            {
                return -20;
            }
            if (astr0.available_capacity() != 0)
            {
                return -30;
            }
            astr0.clear();
            if (astr0.length() != 0)
            {
                return -40;
            }
            if (astr0.c_str()[0] != 0)
            {
                return -50;
            }

            astr0 = "01234567890123456789012345678901";
            if (!astr0.is_dynamic())
            {
                return -60;
            }
            if (astr0.length() != 32)
            {
                return -70;
            }
            if (astr0.available_capacity() != 0)
            {
                return -80;
            }
            if (astr0.c_str()[31] != '1' || astr0.c_str()[32] != '\0')
            {
                return -90;
            }

            astr0 = "012345678901234567890123456789012";
            if (!astr0.is_dynamic())
            {
                return -100;
            }
            if (astr0.length() != 33)
            {
                return -110;
            }
            if (astr0.available_capacity() != 0)
            {
                return -120;
            }
            if (astr0.c_str()[32] != '2' || astr0.c_str()[33] != '\0')
            {
                return -130;
            }

            ::pilo::core::string::astring<0> astr1("0123456789012345678901234567890123456789", 32);
            if (!astr1.is_dynamic())
            {
                return -140;
            }

            if (astr1.compare("01234567890123456789012345678901") != 0)
            {
                return -150;
            }

            if (astr1.compare("012345678901234567890123456789010", 33) == 0)
            {
                return -160;
            }

            std::string stdstr = "01234567890123456789012345678901";
            if (astr1.compare(stdstr) != 0 || stdstr.length() != 32)
            {
                return -170;
            }

            return 0;
        }

    }
}