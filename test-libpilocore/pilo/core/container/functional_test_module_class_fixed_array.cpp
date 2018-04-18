#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_fixed_array.hpp"
#include "core/container/fixed_array.hpp"

using namespace pilo::core::container;

namespace pilo
{
    namespace test
    {


        static pilo::i32_t functional_test_construct(void* param);
        static pilo::i32_t functional_test_copy_construct(void* param);
        static pilo::i32_t functional_test_push_pop(void* param);
        static pilo::i32_t functional_test_insert_erase(void* param);
        static pilo::i32_t functional_test_front_back(void* param);

        pilo::test::testing_case g_functional_cases_fixed_array[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "constructing fixed_array                      ", nullptr, functional_test_construct, 0, -1, (pilo::u32_t) - 1 },
            { 2, "copy constructing fixed_array                 ", nullptr, functional_test_copy_construct, 0, -1, (pilo::u32_t) - 1 },
            { 3, "push and pop fixed_array                      ", nullptr, functional_test_push_pop, 0, -1, (pilo::u32_t) - 1 },
            { 4, "insert and erase fixed_array                  ", nullptr, functional_test_insert_erase, 0, -1, (pilo::u32_t) - 1 },
            { 5, "front and back of fixed_array                 ", nullptr, functional_test_front_back, 0, -1, (pilo::u32_t) - 1 },
            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_test_construct(void* param)
        {
            M_UNUSED(param);

            fixed_array<testing_pod_info,13>      array;
            if (array.size() != 0)
            {
                return -1;
            }

            testing_pod_info  info[13];
            for (int i = 0; i < 13; i++)
            {
                info[i].m_id = i;
            }
            fixed_array<testing_pod_info, 13>      array1(info, 13, false);
            if (array1.size() != 13)
            {
                return -20;
            }
            for (int i = 0; i < (int) array1.size(); i++)
            {
                if (array1[i].m_id != i)
                {
                    return -1000 + i;
                }
            }
           
            fixed_array<testing_pod_info, 12>      array2(info, 13, true);
            if (array2.size() != 12)
            {
                return -30;
            }
            for (int i = 0; i < (int) array2.size(); i++)
            {
                if (array2[i].m_id != i)
                {
                    return -2000 + i;
                }
            }
          
            return 0;
        }

        pilo::i32_t functional_test_copy_construct(void* param)
        {
            M_UNUSED(param);

            testing_pod_info  info[103];
            for (int i = 0; i < 103; i++)
            {
                info[i].m_id = i;
            }

            fixed_array<testing_pod_info, 133>      array(info,101);
            if (array.size() != 101)
            {
                return -1;
            }

            fixed_array<testing_pod_info, 133>      array2(array);
            if (array2.size() != 101)
            {
                return -20;
            }
            for (int i = 0; i < (int) array2.size(); i++)
            {
                if (array2[i].m_id != i)
                {
                    return -2000 + i;
                }
            }

            fixed_array<testing_pod_info, 133>      array3;
            array3 = array2;
            if (array3.size() != 101)
            {
                return -20;
            }
            for (int i = 0; i < (int) array3.size(); i++)
            {
                if (array3.at(i).m_id != i)
                {
                    return -4000 + i;
                }
            }        

            testing_pod_info tofindinfo(100,"");
            
            if (array3.find(tofindinfo) != 100)
            {
                return -1000;
            }

            return 0;
        }

        pilo::i32_t functional_test_push_pop(void* param)
        {
            M_UNUSED(param);

            fixed_array<testing_pod_info, 700>      array;
            if (array.capacity() != 700)
            {
                return -1;
            }

            for (int i = 0; i < (int) (array.capacity()) + ((int) 1); i++)
            {
                testing_pod_info info(i, "");
                array.push_back(info);

                if (array.full())
                {
                    break;
                }
            }

            for (pilo::i32_t i = (pilo::i32_t) array.size() - 1; i >= 0; i--)
            {
                if (array.pop_back().m_id != i)
                {
                    return -1000;
                }
            }

            if (array.size() != 0)
            {
                return -2000;
            }

            return 0;
        }

        pilo::i32_t functional_test_insert_erase(void* param)
        {
            M_UNUSED(param);

            fixed_array<testing_pod_info, 800>      array;
            if (array.capacity() != 800)
            {
                return -1;
            }

            for (int i = 0; i < 700; i++)
            {
                testing_pod_info info(i, "");
                array.push_back(info);

                if (array.full())
                {
                    break;
                }
            }

            testing_pod_info info1(700, "");
            array.insert(700, info1);
            if (array.size() != 701)
            {
                return -200;
            }

            for (int i = 0; i < (int) array.size(); i++)
            {
                if (array.get(i).m_id != i)
                {
                    return -10000 - i;
                }
            }

            testing_pod_info info2(-456, "");
            array.insert(125, info2);

            for (int i = 0; i < (int) array.size(); i++)
            {
                if (i == 125)
                {
                    if (array.at(i).m_id != -456)
                    {
                        return -456;
                    }
                }
                else if (i < 125)
                {
                    if (array.at(i).m_id != i)
                    {
                        return -2000 - i;
                    }
                }
                else if (i > 125)
                {
                    if (array.at(i).m_id != i - 1)
                    {
                        return -2000 - i;
                    }
                }
            }

            return 0;
        }

        pilo::i32_t functional_test_front_back(void* param)
        {
            M_UNUSED(param);

            fixed_array<testing_pod_info, 800>      array;

            for (int i = 0; i < 700; i++)
            {
                testing_pod_info info(i, "");
                array.push_back(info);

                if (array.full())
                {
                    break;
                }
            }

            pilo::i32_t ic = 0;
            while (!array.empty())
            {
                if (array.front().m_id != ic)
                {
                    return -3000 - ic;
                }

                array.erase(0);

                ic++;
            }

            if (array.size() != 0)
            {
                return -5000;
            }

            for (int i = 0; i < 700; i++)
            {
                testing_pod_info info(i, "");
                array.push_back(info);

                if (array.full())
                {
                    break;
                }
            }

            ic = 699;
            while (!array.empty())
            {
                if (array.back().m_id != ic)
                {
                    return -5000 - ic;
                }

                array.erase(array.size() - 1);

                ic--;
            }

            return 0;
        }

   
    }
}