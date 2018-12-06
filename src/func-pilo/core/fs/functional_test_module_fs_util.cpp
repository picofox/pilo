#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "core/io/fixed_memory_istream.hpp"
#include "core/io/fixed_memory_ostream.hpp"
#include "core/io/memory_ostream_adapter.hpp"
#include "core/io/memory_istream_adapter.hpp"
#include "core/datetime/datetime.hpp"
#include "core/fs/fs_util.hpp"
#include "./functional_test_module_fs_util.hpp"

namespace pilo
{
    namespace test
    {
        class test_fs_node_visitor : public ::pilo::core::fs::fs_node_visitor_interface
        {
        public:
            virtual ::pilo::i32_t visit(const ::pilo::core::fs::fs_find_data* data)
            {
                ::pilo::core::io::console_format_output("Visit filename=(%s) fp=(%s) t=%d\n", data->filename(), data->full_pathname(), data->type());
                return 0;
            }
            virtual ::pilo::i32_t post_dir_visit(const char* path)
            {
                ::pilo::core::io::console_format_output("Post Visit path (%s)\n",  path);
                return 0;
            }

            virtual ::pilo::i32_t pre_dir_visit(const char* path)
            {
                ::pilo::core::io::console_format_output("Pre Visit path (%s)\n", path);
                return 0;
            }
        };

#ifdef WINDOWS
        static const char* __st_c_test_trv_paths[] = { "..\\data/test_dir" };
#else
        static const char* __st_c_test_trv_paths[] = { "..\\data/test_dir" };
#endif

        static pilo::i32_t functional_is_absolute_path(void* param);   
        static pilo::i32_t functional_getcwd(void* param);
        static pilo::i32_t functional_traval_path_preorder(void* param);
        static pilo::i32_t functional_truncate(void* param);

        pilo::test::testing_case g_functional_cases_fs_util[] =
        {
            /*---"--------------------------------------------"*/
            { 1, "is_absolute_path                             ", nullptr, functional_is_absolute_path, 0, -1, (pilo::u32_t) - 1 },
            { 2, "get_current_working_directory                ", nullptr, functional_getcwd          , 0, -1, (pilo::u32_t) - 1 },
            { 3, "traval_path_preorder                         ", nullptr, functional_traval_path_preorder, 0, -1, (pilo::u32_t) - 1 },
            { 4, "truncate                                     ", nullptr, functional_truncate,        0, -1, (pilo::u32_t) - 1 },

            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_truncate(void* param)
        {
            M_UNUSED(param);
            const char* test_file_path = "..\\..\\test_data_dir\\func_test\\fs\\fs_util\\test_truncate.dat";
            ::pilo::core::fs::fs_util::delete_regular_file(test_file_path);
            size_t sz = 0;

            ::pilo::error_number_t eret = ::pilo::core::fs::fs_util::create_regular_file(test_file_path, false);
            if (eret != ::pilo::EC_OK)
            {
                return -1;
            }

            eret = ::pilo::core::fs::fs_util::truncate_file(test_file_path, 1234567);
            if (eret != ::pilo::EC_OK)
            {
                return -10;
            }

            eret = ::pilo::core::fs::fs_util::calculate_file_size(sz, test_file_path);
            if (eret != ::pilo::EC_OK)
            {
                return -15;
            }
            if (sz != 1234567)
            {
                return -16;
            }

            eret = ::pilo::core::fs::fs_util::truncate_file(test_file_path, 999);
            if (eret != ::pilo::EC_OK)
            {
                return -30;
            }

            eret = ::pilo::core::fs::fs_util::calculate_file_size(sz, test_file_path);
            if (eret != ::pilo::EC_OK)
            {
                return -40;
            }
            if (sz != 999)
            {
                return -50;
            }

            eret = ::pilo::core::fs::fs_util::truncate_file(test_file_path, 0);
            if (eret != ::pilo::EC_OK)
            {
                return -30;
            }
            eret = ::pilo::core::fs::fs_util::calculate_file_size(sz, test_file_path);
            if (eret != ::pilo::EC_OK)
            {
                return -40;
            }
            if (sz != 0)
            {
                return -50;
            }




            return ::pilo::EC_OK;
        }


        pilo::i32_t functional_traval_path_preorder(void* param)
        {
            M_UNUSED(param);

            ::pilo::core::fs::path_string<256> dst_path("../../test_data_dir/func_test/fs/dircopy_dst");
            ::pilo::core::fs::path_string<256> src_path("../../test_data_dir/readonly\\test_fs_tree");


            ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::copy_directory(dst_path, src_path, true, true);
            if (ret != ::pilo::EC_OK)
            {
                return -1;
            }

            test_fs_node_visitor vistor;
            ::pilo::core::fs::fs_util::travel_path_preorder(__st_c_test_trv_paths[0], &vistor, MB_FS_TRAVELSAL_DIR_COPY, true);

            return 0;
        }



        pilo::i32_t functional_is_absolute_path(void* param)
        {
            M_UNUSED(param);

#ifdef WINDOWS
            const char* path1 = "c:\\aaaa\\";
            const char* path2 = "c:\\aaaa\\b";
            const char* path3 = "c:\\";
            const char* path4 = "c:/aaaa/";
            const char* path5 = "c:/aaaa/b";
            const char* path6 = "c:/";
            const char* path7 = "adfadf";
            const char* path8 = "";
            const char* path9 = "adfa\\adfaf";
#else
            const char* path1 = "/tmp/";
            const char* path2 = "/tmp/aaaa/b";
            const char* path3 = "/";
            const char* path4 = "/tmp";
            const char* path5 = "/tmp/b";
            const char* path6 = "/12";
            const char* path7 = "adfadf";
            const char* path8 = "";
            const char* path9 = "adfa/adfaf";

#endif // WINDOWS

            if (!::pilo::core::fs::fs_util::is_absolute_path(path1))
            {
                return -10;
            }

            if (!::pilo::core::fs::fs_util::is_absolute_path(path2))
            {
                return -20;
            }

            if (!::pilo::core::fs::fs_util::is_absolute_path(path3))
            {
                return -30;
            }

            if (!::pilo::core::fs::fs_util::is_absolute_path(path4))
            {
                return -40;
            }

            if (!::pilo::core::fs::fs_util::is_absolute_path(path5))
            {
                return -50;
            }

            if (!::pilo::core::fs::fs_util::is_absolute_path(path6))
            {
                return -60;
            }

            if (::pilo::core::fs::fs_util::is_absolute_path(path7))
            {
                return -70;
            }

            if (::pilo::core::fs::fs_util::is_absolute_path(path8))
            {
                return -80;
            }

            if (::pilo::core::fs::fs_util::is_absolute_path(path9))
            {
                return -90;
            }

            return 0;
        }

        pilo::i32_t functional_getcwd(void* param)
        {
            M_UNUSED(param);

            char buffer[1024];
            const char* cwd = ::pilo::core::fs::fs_util::get_current_working_directory(buffer, sizeof(buffer), 0, 0);
            if (cwd == nullptr)
            {
                return -10;
            }

            cwd = ::pilo::core::fs::fs_util::get_current_working_directory(0, 0, 0, 0);
            if (cwd == nullptr)
            {
                return -20;
            }
            ::free((void*)cwd);

            char buffer_ne[2];
            cwd = ::pilo::core::fs::fs_util::get_current_working_directory(buffer_ne, sizeof(buffer_ne), 0, 0);
            if (cwd != nullptr)
            {
                return -30;
            }

            cwd = ::pilo::core::fs::fs_util::get_current_working_directory(buffer_ne, MC_INVALID_SIZE, 0, 0);
            if (cwd != nullptr)
            {
                return -40;
            }

            bool bUseHeap;
            cwd = ::pilo::core::fs::fs_util::get_current_working_directory(buffer_ne, sizeof(buffer_ne), &bUseHeap, 0);
            if (cwd == nullptr)
            {
                return -50;
            }
            if (bUseHeap != true)
            {
                return -60;
            }

            size_t nSize;
            cwd = ::pilo::core::fs::fs_util::get_current_working_directory(buffer_ne, 0, &bUseHeap, &nSize);
            if (cwd == nullptr)
            {
                return -70;
            }
            if (bUseHeap != true)
            {
                return -80;
            }
            if (nSize != ::pilo::core::string::string_util::length(cwd))
            {
                return -90;
            }

            return 0;
        }

    }
}