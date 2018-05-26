#include "core/string/string_util.hpp"
#include "core/string/dynamic_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_dynamic_astring.hpp"
#include <string>


namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_auto_astring(void* param);


        pilo::test::testing_case g_functional_cases_dynamic_astring[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "dynamic_astring<char>()                         ", nullptr, functional_test_auto_astring, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_test_auto_astring(void* param)
        {
            M_UNUSED(param);
            pilo::core::string::dynamic_astring  str0;
            if (str0.length() != 0)
            {
                return -1;
            }
            if (str0.capacity() != 0)
            {
                return -2;
            }
            if (str0.c_str()[0] != 0)
            {
                return -3;
            }

            const char* teststr = "test12345  hehe_size @#%$#$^ 0304579353____ ";
            const char* teststrsub10 = "test12345  he";
            size_t len = strlen(teststr);
            pilo::core::string::dynamic_astring  str1(teststr);
            if (str1.length() != len)
            {
                return -11;
            }

            if (str1.capacity() < len )
            {
                return -12;
            }
            if (0 != ::strcmp(str1.c_str(), teststr))
            {
                return -13;
            }

            pilo::core::string::dynamic_astring  str2(nullptr);
            if (str2.length() != 0)
            {
                return -21;
            }
            if (str2.capacity() != 0)
            {
                return -22;
            }
            if (0 != str2.c_str()[0])
            {
                return -23;
            }

            const char * empty = "";
            pilo::core::string::dynamic_astring  str3(empty);
            if (str3.length() != 0)
            {
                return -31;
            }

            if (str3.capacity() < 0 )
            {
                return -32;
            }
            if (0 != str3.c_str()[0])
            {
                return -34;
            }           

            pilo::core::string::dynamic_astring  str4(teststr, 13);
            if (str4.length() != 13)
            {
                return -41;
            }

            if (str4.capacity() < 13 )
            {
                return -42;
            }
            if (0 != ::strcmp(str4.c_str(), teststrsub10))
            {
                return -53;
            }

            pilo::core::string::dynamic_astring  str5(str4);
            if (str5.length() != 13)
            {
                return -51;
            }

            if (str5.capacity() < 13)
            {
                return -52;
            }
            if (0 != ::strcmp(str5.c_str(), teststrsub10))
            {
                return -53;
            }

            pilo::core::string::dynamic_astring  str6;
            str6 = str5;
            if (str6.length() != 13)
            {
                return -61;
            }

            if (str6.capacity() < 13 )
            {
                return -62;
            }
            if (0 != ::strcmp(str6.c_str(), teststrsub10))
            {
                return -63;
            }
            
            const char * bigstr = "a23456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789z";
            str6 = bigstr;
            if (str6.length() != 100)
            {
                return -71;
            }

            if (str6.capacity() < 100 )
            {
                return -72;
            }
            if (0 != ::strcmp(str6.c_str(), bigstr))
            {
                return -73;
            }

            std::string stdstr = bigstr;
            str1 = stdstr;
            if (str1.length() != 100)
            {
                return -81;
            }

            if (str1.capacity() < 100)
            {
                return -82;
            }
            if (0 != ::strcmp(str1.c_str(), bigstr))
            {
                return -83;
            }

            str6.reserve(2047);
            if (str6.length() != 100)
            {
                return -91;
            }
            if (str6.capacity() != 2048)
            {
                return -92;
            }
            if (0 != ::strcmp(str6.c_str(), bigstr))
            {
                return -93;
            }
            
            return 0;
        }

   
    }
}