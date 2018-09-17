#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_compactable_overlapped_memory_pool.hpp"
#include <algorithm>
#include "core/memory/compactable_overlapped_memory_pool.hpp"
#include "core/datetime/datetime.hpp"
#include "core/threading/nonrecursive_mutex.hpp"

using namespace pilo;


#define TEST_UNIT_IN_POOL_NUM (1024*1024)
#define TEST_UNIT_STEP          (1024)

namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_compactable_overlapped_memory_pool_basic(void* param);
        //static pilo::i32_t functional_compactable_memory_pool_mt(void* param);


        pilo::test::testing_case g_functional_cases_compactable_overlapped_memory_pool[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "Basic Func of compactable overlapp memory pool", nullptr, functional_compactable_overlapped_memory_pool_basic, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_compactable_overlapped_memory_pool_basic(void* param)
        {
            M_UNUSED(param);

            typedef testing_pod_info* testing_pod_info_ptr;

            testing_pod_info_ptr* saved_info = new testing_pod_info_ptr[TEST_UNIT_IN_POOL_NUM];

            pilo::core::memory::compactable_overlapped_memory_pool<sizeof(test::testing_pod_info), TEST_UNIT_STEP> pool;

            for (int i = 0; i < TEST_UNIT_IN_POOL_NUM; i++)
            {
                test::testing_pod_info* info = (test::testing_pod_info*) pool.allocate();

                info->m_id = i;
                info->m_name = pilo::core::string::fixed_astring<128>(i);
                saved_info[i] = info;
            }

            if (pool.piece_count() != (TEST_UNIT_IN_POOL_NUM / TEST_UNIT_STEP))
            {
                return -1;
            }


            for (int i = 0; i < TEST_UNIT_IN_POOL_NUM; i++)
            {
                if (saved_info[i]->m_id != i)
                {
                    return -10;
                }

                pilo::core::string::fixed_astring<128> cmpstr(i);

                if (saved_info[i]->m_name != cmpstr)
                {
                    return -20;
                }

            }


            for (int i = 0; i < TEST_UNIT_IN_POOL_NUM; i++)
            {
                pool.deallocate(saved_info[i]);
            }

            if (pool.piece_count() != 3)
            {
                return -2;
            }

            pool.clear();

            if (pool.piece_count() != 3)
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