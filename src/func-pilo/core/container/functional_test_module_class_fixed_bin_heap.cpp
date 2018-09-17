#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_fixed_bin_heap.hpp"
#include "core/container/fixed_bin_heap.hpp"
#include <set>
#include <algorithm>

using namespace pilo::core::container;

namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_ascend(void* param);
        static pilo::i32_t functional_test_descend(void* param);
        static pilo::i32_t functional_test_pod_ascend(void* param);
        static pilo::i32_t functional_test_pod_ptr_ascend(void* param);   
        

        pilo::i32_t functional_test_pod_ptr_ascend(void* param)
        {
            M_UNUSED(param);

            const size_t ELEM_SZ = 1024;

            fixed_bin_heap<pilo::test::testing_pod_info*, ELEM_SZ, 
                ptr_gel_compare_greater<pilo::test::testing_pod_info>,
                ptr_gel_compare_greater<pilo::test::testing_pod_info>
            > bh0;

            if (bh0.size() != 0)
            {
                return -1;
            }

            std::vector<pilo::test::testing_pod_info> sample;
            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                pilo::test::testing_pod_info info(i, "");
                sample.push_back(info);
            }
            std::random_shuffle(sample.begin(), sample.end());

            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                pilo::test::testing_pod_info* info = new  pilo::test::testing_pod_info(i, "");
                bh0.push(info);
            }

            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                pilo::i32_t nbr = (*bh0.top())->m_id;
                if (nbr != i)
                {
                    return -100000 - i;
                }

                bh0.pop();
            }

            return 0;
        }

        pilo::test::testing_case g_functional_cases_fixed_bin_heap[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "ascend fixed_bin_heap                         ", nullptr, functional_test_ascend, 0, -1, (pilo::u32_t) - 1 },
            { 2, "descend fixed_bin_heap                        ", nullptr, functional_test_descend, 0, -1, (pilo::u32_t) - 1 },
            { 3, "ascend pod fixed_bin_heap                     ", nullptr, functional_test_pod_ascend, 0, -1, (pilo::u32_t) - 1 },
            { 4, "ascend pod ptr fixed_bin_heap                 ", nullptr, functional_test_pod_ptr_ascend, 0, -1, (pilo::u32_t) - 1 },

            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_test_ascend(void* param)
        {
            M_UNUSED(param);

            const size_t ELEM_SZ = 1024;

            fixed_bin_heap<pilo::i32_t, ELEM_SZ> bh0;

            if (bh0.size() != 0)
            {
                return -1;
            }

            std::vector<pilo::i32_t> sample;
            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                sample.push_back(i);
            }
            std::random_shuffle(sample.begin(), sample.end());

            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                bh0.push(sample[i]);
            }

            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                pilo::i32_t *nbr = bh0.top();
                if (*nbr != i)
                {
                    return -100000 - i;
                } 

                bh0.pop();
            }

            return 0;
        }

        pilo::i32_t functional_test_descend(void* param)
        {
            M_UNUSED(param);

            const size_t ELEM_SZ = 1024;

            fixed_bin_heap<pilo::i32_t, ELEM_SZ,
                ref_gel_compare_less<pilo::i32_t>,
                ref_gel_compare_less<pilo::i32_t>
            > bh0;

            if (bh0.size() != 0)
            {
                return -1;
            }

            std::vector<pilo::i32_t> sample;
            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                sample.push_back(i);
            }
            std::random_shuffle(sample.begin(), sample.end());

            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                bh0.push(sample[i]);
            }

            for (int i = ELEM_SZ-1; i >= 0; i--)
            {
                pilo::i32_t *nbr = bh0.top();
                if (*nbr != i)
                {
                    return -100000 - i;
                }

                bh0.pop();
            }

            return 0;
        }

        pilo::i32_t functional_test_pod_ascend(void* param)
        {
            M_UNUSED(param);

            const size_t ELEM_SZ = 1024;

            fixed_bin_heap<pilo::test::testing_pod_info, ELEM_SZ> bh0;

            if (bh0.size() != 0)
            {
                return -1;
            }

            std::vector<pilo::test::testing_pod_info> sample;
            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                pilo::test::testing_pod_info info(i, "");
                sample.push_back(info);
            }
            std::random_shuffle(sample.begin(), sample.end());

            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                pilo::test::testing_pod_info info(i, "");
                bh0.push(info);
            }

            for (int i = 0; i < (int) ELEM_SZ; i++)
            {
                pilo::i32_t nbr = (int) bh0.top()->m_id;
                if (nbr != i)
                {
                    return -100000 - i;
                }

                bh0.pop();
            }

            return 0;
        }
    }
}