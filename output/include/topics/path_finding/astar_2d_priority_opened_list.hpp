#pragma once

#include "core/coredefs.hpp"
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

            template <typename _Node, size_t ROWS, size_t COLS>
            class astar_2d_priority_opened_list
            {
            public:
                typedef _Node node_type;
                astar_2d_priority_opened_list() { reset(); }
                void reset()
                {
                    memset(m_data, 0x00, sizeof(m_data));
                    m_size = 0;
                    m_flag.reset();
                }

                void value_changed(node_type* node)
                {
                    M_UNUSED(node);
                    int mintag = m_size - 1;

                    for (int i = 0; i < m_size - 1; i++)
                    {
                        if (m_data[i]->m_f < m_data[mintag]->m_f)
                        {
                            mintag = i;
                        }
                    }

                    if (m_size > 1 && mintag != (m_size - 1))
                    {
                        node_type* t = m_data[m_size - 1];
                        m_data[m_size - 1] = m_data[mintag];
                        m_data[mintag] = t;
                    }
                }

                void add_node(node_type* node)
                {
                    m_flag.set(node->m_pos.y*COLS + node->m_pos.x);
                    m_data[m_size++] = node;
                    int mintag = m_size - 1;

                    for (int i = 0; i < m_size - 1; i++)
                    {
                        if (m_data[i]->m_f < m_data[mintag]->m_f)
                        {
                            mintag = i;
                        }
                    }

                    if (m_size > 1 && mintag != (m_size - 1))
                    {
                        node_type* t = m_data[m_size - 1];
                        m_data[m_size - 1] = m_data[mintag];
                        m_data[mintag] = t;
                    }

                }

                bool find_node_by_pos(const typename node_type::coord_type & pos) const
                {
                    assert(pos.x < COLS && pos.y < ROWS);
                    return m_flag.test(pos.y*COLS + pos.x);
                }

                node_type* detach_lowest_cost_node()
                {
                    if (m_size == 0)
                    {
                        return nullptr;
                    }
                    node_type*  node = m_data[--m_size];

                    m_flag.reset(node->m_pos.y*COLS + node->m_pos.x);
                    return node;
                }

                node_type*      m_data[ROWS*COLS];
                pilo::core::container::fixed_bitset<ROWS*COLS>  m_flag;
                size_t          m_size;
            };


        }
    }
}
