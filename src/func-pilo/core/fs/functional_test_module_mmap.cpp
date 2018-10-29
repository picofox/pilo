#include "core/fs/mmap.hpp"
#include "core/fs/file.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_mmap.hpp"
#include "core/threading/basic_thread.hpp"


namespace pilo
{
    namespace test
    {
        pilo::i32_t functional_test_mmap_basic(void* param);

        pilo::test::testing_case g_functional_cases_mmap[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "initilized()                                 ", nullptr, functional_test_mmap_basic, 0, -1, (pilo::u32_t) - 1 },
            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_test_mmap_basic(void* param)
        {
            M_UNUSED(param);

            ::pilo::core::fs::fs_util::create_directory_recursively("../../test_data_dir/func_test/fs/mmap/", false);
            ::pilo::core::fs::fs_util::delete_regular_file("../../test_data_dir/func_test/fs/mmap/basic.mmap");


            ::pilo::core::fs::mmap<1> map0;


            return ::pilo::EC_OK;
        }
    }
}