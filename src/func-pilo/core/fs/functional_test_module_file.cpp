#include "core/fs/file.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_file.hpp"
#include "core/threading/basic_thread.hpp"


namespace pilo
{
    namespace test
    {
    
        class TestFlockThr : public ::pilo::core::threading::basic_thread
        {
            public:
            TestFlockThr()
            {
                pfile = nullptr;
                m_index = 0;
            }

            void set(::pilo::core::fs::file<>* p, int n)
            {
                pfile = p;
                m_index = n;
            }

            virtual i32_t on_run()
            {
                int times = 100;
                ::pilo::error_number_t err = 0;
                while (times-- > 0)
                {
                    err = pfile->flock_exclusive(0, MC_INVALID_SIZE);
                    if (err != ::pilo::EC_OK)
                    {
                        M_ASSERT(false);
                    }
                    ::pilo::core::threading::basic_thread::sleep(5);
                    pfile->funlock(0, MC_INVALID_SIZE);
                }
                

                return 0;
            }

            int     m_index;
            ::pilo::core::fs::file<>* pfile;
        };

        struct TestFileRecord
        {
            int id;
            char name[1000];
            char seq[20];
        };

#ifdef WINDOWS
        static const char* __st_c_test_file_paths[] = {"../../test_data_dir/func_test/fs/del_test/", "..\\..\\test_data_dir\\func_test\\fs\\file\\d0\\d1\\testinitfile"};
#else
        static const char* __st_c_test_file_paths[] = {"../../test_data_dir/func_test/fs/del_test/", "../../test_data_dir/func_test/fs/file/d0/d1/testinitfile"};
#endif

        static pilo::i32_t functional_test_init(void* param);
        static pilo::i32_t functional_test_flock(void* param);



        pilo::test::testing_case g_functional_cases_file[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "initilized()                                 ", nullptr, functional_test_init, 0, -1, (pilo::u32_t) - 1 },
            { 2, "test flock()                                 ", nullptr, functional_test_flock, 0, -1, (pilo::u32_t) - 1 },

            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_test_flock(void* param)
        {
            M_UNUSED(param);

            ::pilo::error_number_t err = ::pilo::EC_UNDEFINED;
            ::pilo::core::fs::file<> f0;
            err = f0.initialize(__st_c_test_file_paths[1], MB_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE, nullptr);

            err = f0.open(::pilo::core::fs::eDAM_CreateAlways, ::pilo::core::fs::eDRWM_Write, 0);
            if (err != ::pilo::EC_OK)
            {
                return -5;
            }
           
            for (int i = 0; i < 10000; i++)
            {
                if (i % 2 == 0)
                {
                    err = f0.flock_exclusive(0, MC_INVALID_SIZE);
                    if (err != ::pilo::EC_OK)
                    {
                        return -10;
                    }
                }
                else
                {
                    err = f0.flock_shared(0, MC_INVALID_SIZE);
                    if (err != ::pilo::EC_OK)
                    {
                        return -20;
                    }
                }


                err = f0.funlock(0, MC_INVALID_SIZE);
                if (err != ::pilo::EC_OK)
                {
                    return -30;
                }
            }
            f0.close();    
            f0.finalize();

            return 0;
        }


        pilo::i32_t functional_test_init(void* param)
        {
            M_UNUSED(param);

            ::pilo::error_number_t err = ::pilo::EC_UNDEFINED;

            ::pilo::core::fs::file<> f0;

            char szbuffer[1024] = { 0 };
            ::pilo::core::fs::fs_util::get_current_working_directory(szbuffer, sizeof(szbuffer), false, nullptr, false);
            
            err = ::pilo::core::fs::fs_util::create_directory_recursively("../../test_data_dir/func_test/fs/del_test", false);
            if (err != ::pilo::EC_OK)
            {
                return -1000;
            }


            err = ::pilo::core::fs::fs_util::delete_directory(__st_c_test_file_paths[0], false);
            if (err != ::pilo::EC_OK)
            {                
                return -1;
            }

            ::pilo::core::fs::fs_util::delete_regular_file(__st_c_test_file_paths[1]);

            err = f0.initialize(__st_c_test_file_paths[1], MB_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE, nullptr);
            if (err != ::pilo::EC_OK)
            {                
                return -5;
            }
            f0.finalize();
            err = f0.initialize(__st_c_test_file_paths[1], MB_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE, nullptr);
            if (err != ::pilo::EC_FILE_ALREADY_EXIST)
            {
                return -10;
            }
            f0.finalize();



            err = f0.initialize(__st_c_test_file_paths[1], MB_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE | 
                                MB_IO_DEV_FLAG_FORCE_DELETE_FILE_ON_INITIALIZ, nullptr);
            if (err != ::pilo::EC_OK)
            {
                return -20;
            }
            f0.finalize();      


            err = f0.initialize(__st_c_test_file_paths[1], MB_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE | 
                                                    MB_IO_DEV_FLAG_FORCE_DELETE_DIR_ON_INITIALIZE |
                                                    MB_IO_DEV_FLAG_FORCE_DELETE_FILE_ON_INITIALIZ |
                                                    MB_IO_DEV_FLAG_AUTO_DELETE_ON_FINALIZE, nullptr);

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

            err = f0.open(::pilo::core::fs::eDAM_OpenExisting, ::pilo::core::fs::eDRWM_Write, MB_IO_DEV_OP_FLAG_APPEND | MB_IO_DEV_OP_REOPEN);
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

            err = f0.open(::pilo::core::fs::eDAM_OpenExisting, ::pilo::core::fs::eDRWM_Read, MB_IO_DEV_OP_REOPEN);
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

            err = f0.open(::pilo::core::fs::eDAM_OpenExisting, ::pilo::core::fs::eDRWM_Read, MB_IO_DEV_OP_REOPEN);
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