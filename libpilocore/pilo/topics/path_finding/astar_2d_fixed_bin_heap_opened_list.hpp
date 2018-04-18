#pragma once

#include "core/coredefs.hpp"
#include "core/container/fixed_bin_heap.hpp"
#include "topics/path_finding/path_2d_map.hpp"
#include "topics/path_finding/astar_2d_behaviour.hpp"
#include <queue>
#include <list>
#include <cassert>



namespace pilo
{
    namespace topics
    {
        namespace path_finding
        {
            template <typename T> 
            class node_ptr_pos_equality_compare
            {
            private:
                typedef T value_type;

            public:
                bool operator () (const value_type* a, const value_type* b) const
                {
                    if (a->m_pos.y == b->m_pos.y && a->m_pos.x == b->m_pos.x)
                    {
                        return true;
                    }
                    return false;
                }
            };

            template <typename _Node, size_t ROWS, size_t COLS>
            class astar_2d_fixed_bin_heap_opened_list
            {
            public:
                typedef _Node node_type;
                astar_2d_fixed_bin_heap_opened_list() { reset(); }
                void reset()
                {
                    m_list.clear();
                    m_flag.reset();
                }


                void add_node(node_type* node)
                {
                    m_flag.set(node->m_pos.y*COLS + node->m_pos.x);
                    m_list.push(node);

                }

                bool find_node_by_pos(const typename node_type::coord_type & pos) const
                {
                    assert(pos.x < COLS && pos.y < ROWS);
                    return m_flag.test(pos.y*COLS + pos.x);
                }

                node_type* detach_lowest_cost_node()
                {
                    if (m_list.empty())
                    {
                        return nullptr;
                    }

                    node_type* ret_node = nullptr;
                    node_type** node = m_list.top();
                    M_ASSERT(node != nullptr);
                    if (node == nullptr)
                    {
                        return nullptr;
                    }

                    ret_node = *node;

                    m_flag.reset(ret_node->m_pos.y*COLS + ret_node->m_pos.x);


                    m_list.pop();
                    return ret_node;
                }

                void on_value_changed(node_type* node)
                {
                    pilo::topics::path_finding::node_ptr_pos_equality_compare<node_type> cmp;
                    m_list.value_changed(node, cmp);
                }

                pilo::core::container::fixed_bitset<ROWS*COLS>              m_flag;
                pilo::core::container::fixed_bin_heap<node_type*,  ROWS*COLS, ptr_gel_compare_greater<node_type>,  ptr_gel_compare_greater<node_type> >      m_list;

            };


        }
    }
}
