#ifndef __pilo_core_memory_linked_buffer_node_hpp_
#define __pilo_core_memory_linked_buffer_node_hpp_

#include "../../pilo.hpp"
#include "../process/context.hpp"

namespace pilo
{
    namespace core
    {        
        namespace memory
        {
            template<::pilo::i64_t TA_UNIT_SIZE>
            class linked_buffer_node
            {
            public:
                linked_buffer_node() : m_next(nullptr) 
                {
                    data[0] = 0;
                }
                linked_buffer_node* m_next;
                char         data[TA_UNIT_SIZE];

            public:
                static linked_buffer_node<TA_UNIT_SIZE>* allocate()
                {
                    return new linked_buffer_node<TA_UNIT_SIZE>();
                }
                static void deallocate(linked_buffer_node<TA_UNIT_SIZE>* pnode)
                {
                    delete pnode;
                }

                static ::pilo::err_t update_pool_object_stat(::pilo::core::stat::pool_object_stat_manager::stat_item* si)
                {
                    si->set(0, 0);
                    return PILO_OK;
                }

            };

            template<>
            class linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE>
            {
            public:
                linked_buffer_node() : m_next(nullptr)
                {
                    data[0] = 0;
                }
                linked_buffer_node* m_next;
                char         data[SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE];

            public:
                static linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE>* allocate()
                {
                    return PILO_CONTEXT->allocate_linked_buffer_node_4k();
                }
                static void deallocate(linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE>* pnode)
                {
                    PILO_CONTEXT->deallocate_linked_buffer_node_4k(pnode);
                }

                static ::pilo::err_t update_pool_object_stat(::pilo::core::stat::pool_object_stat_manager::stat_item* si)
                {
                    ::pilo::i64_t avail = 0;
                    ::pilo::i64_t total = 0;
                    PILO_CONTEXT->linked_buffer_node_4k_pool()->m_memory_pool.calc_available_units_nolock(avail, total, 0);
                    si->set(total, avail);
                    return PILO_OK;
                }
            };
        }
    }
}

#endif
