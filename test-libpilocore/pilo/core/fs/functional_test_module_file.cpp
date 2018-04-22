#include "core/fs/file.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_file.hpp"


namespace pilo
{
    namespace test
    {
#ifdef WINDOWS
        static const char* __st_c_test_file_paths[] = {"..\\output\\tmp\\", "..\\output\\tmp\\d0\\d1\\testfile"};
#else
        static const char* __st_c_test_file_paths[] = {"./testfile"};
#endif

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

            ::pilo::core::fs::file<1024, 1024,false> f0;

            ::pilo::core::fs::fs_util::delete_directory(__st_c_test_file_paths[0], false);

            ::pilo::error_number_t err = ::pilo::EC_UNDEFINED;
            err = f0.initialize(__st_c_test_file_paths[1], MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE, nullptr);
            if (err != ::pilo::EC_OK)
            {
                return -1;
            }

            err = f0.initialize(__st_c_test_file_paths[1], MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE, nullptr);
            if (err != ::pilo::EC_FILE_ALREAY_EXIST)
            {
                return -10;
            }

            err = f0.initialize(__st_c_test_file_paths[1], MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE | 
                                MC_IO_DEV_FLAG_FORCE_DELETE_FILE_ON_INITIALIZ, nullptr);
            if (err != ::pilo::EC_OK)
            {
                return -20;
            }

            f0.initialize(__st_c_test_file_paths[1], MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE | 
                                                    MC_IO_DEV_FLAG_FORCE_DELETE_DIR_ON_INITIALIZE |
                                                    MC_IO_DEV_FLAG_FORCE_DELETE_FILE_ON_INITIALIZ |
                                                    MC_IO_DEV_FLAG_AUTO_DELETE_ON_FINALIZE, nullptr);

           
            return 0;
        }


    }
}