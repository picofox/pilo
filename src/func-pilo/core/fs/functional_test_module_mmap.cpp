#include "core/fs/mmap.hpp"
#include "core/fs/file.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_mmap.hpp"
#include "core/threading/basic_thread.hpp"
#include "core/io/format_output.hpp"


struct test_mmap_rec
{
    test_mmap_rec()
    {
        m_id = -1;
        name[0] = 0;
    }
    int m_id;
    char name[12];
};

namespace pilo
{
    namespace test
    {
        pilo::i32_t functional_test_mmap_basic(void* param);

        pilo::test::testing_case g_functional_cases_mmap[] =
        {
            /*---"---------------------------------------------"*/
            { 1, "basicfunc()                                  ", nullptr, functional_test_mmap_basic, 0, -1, (pilo::u32_t) - 1 },
            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_test_mmap_basic(void* param)
        {
            M_UNUSED(param);
			const char* mmp_file_path0 = "../../test_data_dir/func_test/fs/mmap/basic.mmap";

            ::pilo::core::fs::fs_util::create_directory_recursively("../../test_data_dir/func_test/fs/mmap/", false);
			::pilo::core::fs::fs_util::delete_regular_file(mmp_file_path0);

			::pilo::core::fs::mmap<1> mmp0;
			::pilo::error_number_t ret = ::pilo::EC_UNDEFINED;
			ret = mmp0.initialize(mmp_file_path0, 16 * 1024 * 1024, ::pilo::core::fs::eDAM_CreateAlways, eGPE_ReadWrite);
			if (ret != ::pilo::EC_OK)
			{
				return -1;
			}

            size_t idx_ret;
            ret = mmp0.map(idx_ret, MC_INVALID_SIZE, eGPE_ReadWrite, 0, 0, nullptr);
            if (ret != ::pilo::EC_OK)
            {
                return -10;
            }

            char* ptr = (char*) mmp0.mapped_ptr(idx_ret);
            ::pilo::core::fs::mmap_parameter info;
            if (mmp0.get_mapped_info(&info, idx_ret) != ::pilo::EC_OK)
            {
                return -20;
            }

            if (ptr != info.m_address)
            {
                return -30;
            }

            if (info.m_length != 16 * 1024 * 1024)
            {
                return -40;
            }

            for (size_t bo = 0; bo < 1024 * 1024; bo++)
            {
                test_mmap_rec rec;
                rec.m_id = (int) bo;
                ::pilo::core::io::string_format_output(rec.name, 12, "n_%09d",bo);
                memcpy(ptr+ bo* 16, &rec, 16);
            }

            mmp0.finalize();

            ::pilo::core::fs::fs_util::delete_regular_file(mmp_file_path0);

            ::pilo::core::fs::mmap<2> mmp1;
            ret = mmp1.initialize(mmp_file_path0, 16 * 1024 * 1024, ::pilo::core::fs::eDAM_CreateAlways, eGPE_ReadWrite);
            if (ret != ::pilo::EC_OK)
            {
                return -110;
            }

            size_t idxes[2];
            ret = mmp1.map(idxes[0], MC_INVALID_SIZE, eGPE_ReadWrite, 0, 16 * 512 * 1024, nullptr);
            if (ret != ::pilo::EC_OK)
            {
                return -120;
            }
            ret = mmp1.map(idxes[1], MC_INVALID_SIZE, eGPE_ReadWrite, 16 * 512 * 1024, 16 * 512 * 1024, nullptr);
            if (ret != ::pilo::EC_OK)
            {
                return -130;
            }

            ptr = (char*) mmp1.mapped_ptr(idxes[0]);
            if (ptr == nullptr)
            {
                return -140;
            }
            for (size_t bo = 0; bo < 512 * 1024; bo++)
            {
                test_mmap_rec rec;
                rec.m_id = (int)bo;
                ::pilo::core::io::string_format_output(rec.name, 12, "a_%09d", bo);
                memcpy(ptr + bo * 16, &rec, 16);
            }

            ptr = (char*)mmp1.mapped_ptr(idxes[1]);
            if (ptr == nullptr)
            {
                return -150;
            }
            for (size_t bo = 0; bo < 512 * 1024; bo++)
            {
                test_mmap_rec rec;
                rec.m_id = (int)bo;
                ::pilo::core::io::string_format_output(rec.name, 12, "b_%09d", bo);
                memcpy(ptr + bo * 16, &rec, 16);
            }

            mmp1.finalize();

            ::pilo::core::fs::fs_util::delete_regular_file(mmp_file_path0);

            return ::pilo::EC_OK;
        }
    }
}