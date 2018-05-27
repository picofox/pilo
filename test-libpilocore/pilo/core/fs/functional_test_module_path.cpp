#include "core/fs/path.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_file.hpp"
#include "core/fs/fs_util.hpp"


namespace pilo
{
    namespace test
    {

        struct TestFileRecord
        {
            int id;
            char name[1000];
            char seq[20];
        };

#ifdef WINDOWS
        static const char* __st_c_test_inv_paths[] = { "\\...\\d0\\d1\\d2\\d3","\\d0\\d1\\d2\\d3\\...","\\d0\\d1\\d2\\d3\\|","\\d0\\d1\\d2\\>" };
        static const char* __st_c_test_abs_paths[] = { "d:123", "d:/123", "d:\\123" };
        static const char* __st_c_test_to_abs_paths[] = { 
            "d0\\\\\\\\\\d1/d2\\\\d3","d0\\\\\\\\\\d1/d2\\\\d3/../", 
            "d0\\\\\\\\\\d1/d2\\\\d3/../tf.txt", "d:\\1\\..",
            "d0\\\\\\\\\\d1/d2\\\\d3/../..\\..\\a.txt" };
#else
        static const char* __st_c_test_inv_paths[] = { "/.../d0/d1/d2/d3","/d0/d1/d2/d3/...", "/d0/d1/d2/d3/|", "/d0/d1/d2/</"};        
        static const char* __st_c_test_abs_paths[] = { "/123","/123","//123"};
        static const char* __st_c_test_to_abs_paths[] = { 
            "d0///////d1/d2///d3", "d0///////d1/d2///d3/../","d0///////d1/d2///d3/../tf.txt" ,"/1/..",  "d0///////d1/d2///d3/../../../tf.txt"};
#endif

        static pilo::i32_t functional_test_fix(void* param);
        static pilo::i32_t functional_test_dyn(void* param);


        pilo::test::testing_case g_functional_cases_path[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "test path<FIX_SIZE>                          ", nullptr, functional_test_fix, 0, -1, (pilo::u32_t) - 1 },
            { 2, "test path<0>                                 ", nullptr, functional_test_dyn, 0, -1, (pilo::u32_t) - 1 },

            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_test_dyn(void* param)
        {
            M_UNUSED(param);

            //check fatal invalid paths
            ::pilo::core::fs::path<0> path_fatal_test;
            for (int i = 0; i < (int) (sizeof(__st_c_test_inv_paths) / sizeof(char*)); i++)
            {
                path_fatal_test.reset();
                path_fatal_test.assign(__st_c_test_inv_paths[i]);
                if (path_fatal_test.valid())
                {
                    return -20;
                }
            }


            ::pilo::core::fs::path<0> path_abs_test;
            for (int i = 0; i < (int) (sizeof(__st_c_test_abs_paths) / sizeof(char*)); i++)
            {
                path_abs_test.reset();
                path_abs_test.assign(__st_c_test_abs_paths[i]);
                bool b = path_abs_test.is_absolute();

                if (!b)
                {
                    return -30;
                }
            }

            for (int i = 0; i < (int) (sizeof(__st_c_test_to_abs_paths) / sizeof(char*)); i++)
            {
                path_abs_test.assign(__st_c_test_to_abs_paths[i]);
                ::pilo::error_number_t ret = path_abs_test.to_absolute(false, true);
                if (ret != ::pilo::EC_OK)
                {
                    return -40;
                }

                printf("-> (%s) \n",path_abs_test.c_str());

                ret = path_abs_test.to_absolute(true, true);

                printf("-> (%s) \n", path_abs_test.c_str());
                if (ret != ::pilo::EC_OK)
                {
                    return -40;
                }
            }


            return 0;
        }

        pilo::i32_t functional_test_fix(void* param)
        {
            M_UNUSED(param);

            const char* str_toolong = "0123456789012345678901234567890123456789012345678901234567891234";
            //test too long string
            ::pilo::core::fs::path<64> path_toolong(str_toolong);
            if (path_toolong.valid())
            {
                return -1;
            }
            if (path_toolong.length() != MC_INVALID_SIZE)
            {
                return -10;
            }            

            //check fatal invalid paths
            ::pilo::core::fs::path<128> path_fatal_test;
            for (int i = 0; i < (int) (sizeof(__st_c_test_inv_paths) / sizeof(char*)); i++)
            {
                path_fatal_test.reset();
                path_fatal_test.assign(__st_c_test_inv_paths[i]);
                if (path_fatal_test.valid())
                {
                    return -20;
                }
            }


            ::pilo::core::fs::path<128> path_abs_test;
            for (int i = 0; i < (int) (sizeof(__st_c_test_abs_paths) / sizeof(char*)); i++)
            {
                path_abs_test.reset();
                path_abs_test.assign(__st_c_test_abs_paths[i]);
                bool b = path_abs_test.is_absolute();

                if (!b)
                {
                    return -30;
                }
            }

            for (int i = 0; i < (int) (sizeof(__st_c_test_to_abs_paths) / sizeof(char*)); i++)
            {
                printf("(%s)\n", __st_c_test_to_abs_paths[i]);
                path_abs_test.assign(__st_c_test_to_abs_paths[i]);
                ::pilo::error_number_t ret = path_abs_test.to_absolute(false, true);
                if (ret != ::pilo::EC_OK)
                {
                    printf("(%s) Failed\n", __st_c_test_to_abs_paths[i]);
                    return -40;
                }

                ret = path_abs_test.to_absolute(true, true);
                if (ret != ::pilo::EC_OK)
                {
                    return -40;
                }
            }

            
            

            return 0;
        }


    }
}