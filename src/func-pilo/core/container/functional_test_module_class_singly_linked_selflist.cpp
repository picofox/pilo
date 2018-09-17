#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/datetime/datetime.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_singly_linked_selflist.hpp"
#include "core/container/singly_linked_selflist.hpp"
#include <set>
#include <algorithm>


using namespace pilo::core::container;

namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_test_basic_op(void* param);
        static pilo::i32_t functional_test_erase(void* param);
        static pilo::i32_t functional_test_insert(void* param);

        

        
        pilo::i32_t functional_test_insert(void* param)
        {
            M_UNUSED(param);
            singly_linked_list_node<testing_pod_info>* prev = nullptr;
            singly_linked_selflist<singly_linked_list_node<testing_pod_info> > list;

            int * pi = new int;
            if (pi == nullptr) return 0;

            for (int i = 0; i < 10000; i++)
            {
                singly_linked_list_node<testing_pod_info>* p = new singly_linked_list_node<testing_pod_info>;
                p->m_value.m_id = i;                

                list.push_back(p);

                if (i == 3333)
                {
                    prev = p;
                }
            }

            singly_linked_list_node<testing_pod_info>* p2 = new singly_linked_list_node<testing_pod_info>;
            p2->m_value.m_id = 197675;
            list.insert_after(prev, p2);

            if (list.size() != 10001)
            {
                return -1;
            }

            for (int i = 0; i < (int) list.size(); i++)
            {
                singly_linked_list_node<testing_pod_info>* p = list.pop_front();
                if (i < 3334)
                {
                    if (p->m_value.m_id != i)
                    {
                        return -10;
                    }
                }
                else if (i == 3334)
                {
                    if (p->m_value.m_id != 197675)
                    {
                        return -20;
                    }
                }
                else
                {
                    if (p->m_value.m_id != i-1)
                    {
                        return -30;
                    }
                }
            }

            return 0;
        }

        pilo::test::testing_case g_functional_cases_singly_linked_selflist[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "basic op                                      ", nullptr, functional_test_basic_op, 0, -1, (pilo::u32_t) - 1 },
            { 2, "erase                                         ", nullptr, functional_test_erase, 0, -1, (pilo::u32_t) - 1 },
            { 3, "insert                                        ", nullptr, functional_test_insert, 0, -1, (pilo::u32_t) - 1 },
            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_test_basic_op(void* param)
        {
            M_UNUSED(param);

            singly_linked_selflist<singly_linked_list_node<testing_pod_info> > list;

            for (int i = 9999; i >= 0; i--)
            {
                singly_linked_list_node<testing_pod_info>* p = new singly_linked_list_node<testing_pod_info>;
                p->m_value.m_id = i;

                list.push_front(p);
            }

            if (list.size() != 10000)
            {
                return -1;
            }

            for (int i = 10000; i < 20000; i++)
            {
                singly_linked_list_node<testing_pod_info>* p = new singly_linked_list_node<testing_pod_info>;
                p->m_value.m_id = i;

                list.push_back(p);
            }

            if (list.size() != 20000)
            {
                return -1;
            }

            int i = 0;
            while (!list.empty())
            {
                singly_linked_list_node<testing_pod_info>* p = list.pop_front();
                if (p == nullptr)
                {
                    return -1000000 - i;
                }

                if (p->m_value.m_id != i)
                {
                    return -2000000 - i;
                }

                i++;

            }

            return 0;
        }

        pilo::i32_t functional_test_erase(void* param)
        {
            M_UNUSED(param);
            singly_linked_selflist<singly_linked_list_node<testing_pod_info> > list;
            if (list.erase(0) != false)
            {
                return -1;
            }
            for (int i = 0; i < 10000; i++)
            {
                singly_linked_list_node<testing_pod_info>* p = new singly_linked_list_node<testing_pod_info>;
                p->m_value.m_id = i;

                list.push_back(p);
            }

            if (list.erase(0) != false)
            {
                return -2;
            }

            int i = 0;
            while (!list.empty())
            {
                singly_linked_list_node<testing_pod_info>* p = list.front();
                list.erase(p);
                i++;
                delete p;
            }

            if (i != 10000)
            {
                return -3;
            }

            //test erase_after
            for (int i = 0; i < 10000; i++)
            {
                singly_linked_list_node<testing_pod_info>* p = new singly_linked_list_node<testing_pod_info>;
                p->m_value.m_id = i;

                list.push_back(p);
            }

            int cnt = 0;
            while (!list.empty())
            {
                singly_linked_list_node<testing_pod_info>* p;
                if (!list.erase_after(list.front(), &p))
                {
                    break;
                }
                cnt++;
                delete p;
            }

            if (cnt != 9999)
            {
                return -5;
            }



            return 0;
        }


    }

    
}