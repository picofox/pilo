#include "core/fs/file.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_file.hpp"


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

            ::pilo::error_number_t err = ::pilo::EC_UNDEFINED;

            ::pilo::core::fs::file<> f0;

//             err = ::pilo::core::fs::fs_util::delete_directory(__st_c_test_file_paths[0], false);
//             if (err != ::pilo::EC_OK)
//             {                
//                 return -1;
//             }

            err = f0.initialize(__st_c_test_file_paths[1], MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE, nullptr);
            if (err != ::pilo::EC_OK)
            {
                return -5;
            }
            f0.finalize();

            err = f0.initialize(__st_c_test_file_paths[1], MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE, nullptr);
            if (err != ::pilo::EC_FILE_ALREAY_EXIST)
            {
                return -10;
            }
            f0.finalize();

            err = f0.initialize(__st_c_test_file_paths[1], MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE | 
                                MC_IO_DEV_FLAG_FORCE_DELETE_FILE_ON_INITIALIZ, nullptr);
            if (err != ::pilo::EC_OK)
            {
                return -20;
            }
            f0.finalize();

            err = f0.initialize(__st_c_test_file_paths[1], MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE | 
                                                    MC_IO_DEV_FLAG_FORCE_DELETE_DIR_ON_INITIALIZE |
                                                    MC_IO_DEV_FLAG_FORCE_DELETE_FILE_ON_INITIALIZ |
                                                    MC_IO_DEV_FLAG_AUTO_DELETE_ON_FINALIZE, nullptr);

            if (err != ::pilo::EC_OK)
            {
                return -40;
            }

            err = f0.open(::pilo::core::fs::eDAM_OpenExisting, ::pilo::core::fs::eDRWM_Write, 0);
            if (err != ::pilo::EC_OK)
            {
                return -50;
            }

            TestFileRecord rec;
            size_t rlen;
            for (int i = 0; i < 1024; i++)
            {
                memset(&rec, 0, sizeof(rec));
                rec.id = i;
                err = f0.write(&rec, sizeof(rec),  &rlen);
                if (err != ::pilo::EC_OK)
                {
                    return -60;
                }
                if (rlen != sizeof(rec))
                {
                    return -70;
                }
            }

            err = f0.open(::pilo::core::fs::eDAM_OpenExisting, ::pilo::core::fs::eDRWM_Write, MC_IO_DEV_OP_FLAG_APPEND | MC_IO_DEV_OP_REOPEN);
            if (err != ::pilo::EC_OK)
            {
                return -50;
            }

            for (int i = 1024; i < 2048; i++)
            {
                memset(&rec, 0, sizeof(rec));
                rec.id = i;
                err = f0.write(&rec, sizeof(rec), &rlen);
                if (err != ::pilo::EC_OK)
                {
                    return -70;
                }
                if (rlen != sizeof(rec))
                {
                    return -80;
                }
            }

            err = f0.close();

            err = f0.open(::pilo::core::fs::eDAM_OpenExisting, ::pilo::core::fs::eDRWM_Read, MC_IO_DEV_OP_REOPEN);
            if (err != ::pilo::EC_OK)
            {
                return -90;
            }

            for (int i = 0; i < 2048; i++)
            {
                err = f0.read(&rec, sizeof(rec), &rlen);
                if (err != ::pilo::EC_OK)
                {
                    return -100;
                }

                if (rlen != sizeof(rec))
                {
                    return -110;
                }

                if (rec.id != i)
                {
                    return -120;
                }
            }  

            err = f0.close();
            if (err != ::pilo::EC_OK)
            {
                return -130;
            }

            err = f0.open(::pilo::core::fs::eDAM_OpenExisting, ::pilo::core::fs::eDRWM_Read, MC_IO_DEV_OP_REOPEN);
            if (err != ::pilo::EC_OK)
            {
                return -140;
            }

            ::pilo::i64_t r_off = 0;
            err = f0.seek(765 * sizeof(rec), ::pilo::core::fs::eDSW_Begin, &r_off);
            if (err != ::pilo::EC_OK)
            {
                return -150;
            }
            err = f0.read(&rec, sizeof(rec), &rlen);
            if (err != ::pilo::EC_OK)
            {
                return -160;
            }
            if (rec.id != 765)
            {
                return -170;
            }
           
            return 0;
        }


    }
}