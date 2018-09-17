#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_overlapped_memory_pool.hpp"
#include <algorithm>
#include "core/memory/overlapped_memory_pool.hpp"
#include "core/datetime/datetime.hpp"
#include "core/threading/nonrecursive_mutex.hpp"

using namespace pilo;

namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_overlapped_memory_pool_basic(void* param);
        static pilo::i32_t functional_overlapped_memory_pool_mt(void* param);


        pilo::test::testing_case g_functional_cases_overlapped_memory_pool[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "Basic Functions of overlapped pool            ", nullptr, functional_overlapped_memory_pool_basic, 0, -1, (pilo::u32_t) - 1 },
            { 2, "Basic Functions of MT overlapped pool         ", nullptr, functional_overlapped_memory_pool_mt, 0, -1, (pilo::u32_t) - 1 },
     
            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


#define TEST_UNIT_IN_POOL_NUM (1024*1024)

        pilo::i32_t functional_overlapped_memory_pool_basic(void* param)
        {
            M_UNUSED(param);

            typedef testing_pod_info* testing_pod_info_ptr;

            testing_pod_info_ptr* saved_info = new testing_pod_info_ptr[TEST_UNIT_IN_POOL_NUM];

            pilo::core::memory::overlapped_memory_pool<sizeof(test::testing_pod_info), 1024> pool;
            
         
            for (int i = 0; i < TEST_UNIT_IN_POOL_NUM; i++)
            {
                test::testing_pod_info* info = (test::testing_pod_info*) pool.allocate();

                info->m_id = i;
                saved_info[i] = info;                
            }


            if (pool.piece_count() != 1024)
            {
                return -1;
            }


            for (int i = 0; i < TEST_UNIT_IN_POOL_NUM; i++)
            {
                pool.deallocate(saved_info[i]);
            }

            if (pool.piece_count() != 1024)
            {
                return -2;
            }

            pool.clear();
            
            if (pool.piece_count() != 1024)
            {
                return -3;
            }

            pool.reset();

            if (pool.piece_count() != 0)
            {
                return -4;
            }

            delete [] saved_info;
            return 0;
        }

        pilo::i32_t functional_overlapped_memory_pool_mt(void* param)
        {
            M_UNUSED(param);

            typedef testing_pod_info* testing_pod_info_ptr;

            testing_pod_info_ptr* saved_info = new testing_pod_info_ptr[TEST_UNIT_IN_POOL_NUM];

            pilo::core::memory::overlapped_memory_pool<sizeof(test::testing_pod_info), 1024, pilo::core::threading::nonrecursive_mutex> pool;


            for (int i = 0; i < TEST_UNIT_IN_POOL_NUM; i++)
            {
                test::testing_pod_info* info = (test::testing_pod_info*) pool.allocate();

                info->m_id = i;
                saved_info[i] = info;
            }


            if (pool.piece_count() != 1024)
            {
                return -1;
            }


            for (int i = 0; i < TEST_UNIT_IN_POOL_NUM; i++)
            {
                pool.deallocate(saved_info[i]);
            }

            if (pool.piece_count() != 1024)
            {
                return -2;
            }

            pool.clear();

            if (pool.piece_count() != 1024)
            {
                return -3;
            }

            pool.reset();

            if (pool.piece_count() != 0)
            {
                return -4;
            }

            delete[] saved_info;
            return 0;
        }



    }
}