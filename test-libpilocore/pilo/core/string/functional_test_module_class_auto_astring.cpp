#include "core/string/string_util.hpp"
#include "core/string/auto_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_auto_astring.hpp"
#include <string>


namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_constructor_0_s(void* param);


        pilo::test::testing_case g_functional_cases_auto_astring[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "auto_astring<char>()                         ", nullptr, functional_test_constructor_0_s, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_test_constructor_0_s(void* param)
        {
            M_UNUSED(param);
            const char* teststr0 = "a1234567890z";
            const char* teststr1 = "a123456789012345678901234567890123456789012345678z";
            ::pilo::core::string::auto_astring<24> str0;
            ::pilo::core::string::EnumAAStringTypeInUsing t  = str0.which_in_using();
            if (t != ::pilo::core::string::eASTIU_Fixed)
            {
                return -1;
            }
            if (str0.length() != 0)
            {
                return -2;
            }

            str0 = teststr0;
            if (::pilo::core::string::string_util::binary_compare(str0.c_str(), teststr0) != 0)
            {
                return -3;
            }

            str0 = teststr1;
            if (str0.which_in_using() != ::pilo::core::string::eASTIU_Dynamic)
            {
                return -11;
            }
            if (str0.length() != 50)
            {
                return -12;
            }

            ::pilo::core::string::auto_astring<0> str1 = str0;
            if (str0.which_in_using() != ::pilo::core::string::eASTIU_Dynamic)
            {
                return -21;
            }
            if (str0.length() != 50)
            {
                return -22;
            }

            ::pilo::core::string::auto_astring<0> str2 (str0);
            if (str0.which_in_using() != ::pilo::core::string::eASTIU_Dynamic)
            {
                return -31;
            }
            if (str0.length() != 50)
            {
                return -32;
            }

            if (str2[str2.length() - 1] != 'z')
            {
                return 42;
            }
            
            return 0;
        }


    }
}