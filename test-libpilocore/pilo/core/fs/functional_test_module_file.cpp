#include "core/fs/file.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_file.hpp"


namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_init(void* param);


        pilo::test::testing_case g_functional_cases_file[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "initilized()                                 ", nullptr, functional_test_init, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_test_init(void* param)
        {
            M_UNUSED(param);

            ::pilo::core::fs::file<0, 8192, 8192,false> f0;

            f0.initialize(nullptr, 0, nullptr);

           
            return 0;
        }


    }
}