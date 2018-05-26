#include "core/string/string_util.hpp"
#include "core/string/astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_astring.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/string/dynamic_astring.hpp"
#include <string>


namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_auto_astring(void* param);
        static pilo::i32_t functional_test_pure_dyn_astring(void* param);
        static pilo::i32_t functional_test_common(void* param);

        pilo::test::testing_case g_functional_cases_auto_astring[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "astring<SZ>()                                ", nullptr, functional_test_auto_astring, 0, -1, (pilo::u32_t) - 1 },
            { 2, "astring<0>()                                 ", nullptr, functional_test_pure_dyn_astring, 0, -1, (pilo::u32_t) - 1 },
            { 3, "common                                       ", nullptr, functional_test_common, 0, -1, (pilo::u32_t) - 1 },

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