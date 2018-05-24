#include "core/string/string_util.hpp"
#include "core/string/astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_astring.hpp"
#include <string>


namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_auto_astring(void* param);
        static pilo::i32_t functional_test_pure_dyn_astring(void* param);

        pilo::test::testing_case g_functional_cases_auto_astring[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "astring<SZ>()                                ", nullptr, functional_test_auto_astring, 0, -1, (pilo::u32_t) - 1 },
            { 2, "astring<0>()                                 ", nullptr, functional_test_pure_dyn_astring, 0, -1, (pilo::u32_t) - 1 },

            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_test_auto_astring(void* param)
        {
            M_UNUSED(param);
            ::pilo::core::string::astring<32> astr0("fox is good guy");
            if (astr0.is_dynamic())
            {
                return -1;
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

            return 0;
        }

    }
}