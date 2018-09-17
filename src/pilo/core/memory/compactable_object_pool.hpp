#pragma once

#include "core/threading/dummy_mutex.hpp"
#include "core/container/singly_linked_selflist.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/memory/compactable_overlapped_memory_pool.hpp"

#ifdef _DEBUG_MEM_OBJ_POOL
#include "core/io/format_output.hpp"
#endif

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  typename T,
                        size_t _Step,
                        typename _Lock = pilo::core::threading::dummy_mutex>
            class compactable_object_pool
            {
            public:
                typedef T                                                               object_type;
                typedef pilo::core::container::singly_linked_list_node<object_type>     object_node;
                typedef _Lock                                                           lock_type;
                typedef compactable_overlapped_memory_pool<sizeof(object_node), 
                        _Step, 
                        pilo::core::threading::dummy_mutex>                             pool_type;
                typedef pilo::core::container::singly_linked_selflist<object_node>      object_list;            

            public:
                lock_type   m_lock;
                object_list m_free_obj_list;
                pool_type   m_memory_pool;

            public:
                compactable_object_pool()
                {
                                        
                };
                ~compactable_object_pool()
                { 
                    this->reset(); 
                };

                void reset()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    object_node* node = nullptr;
                    while (node = m_free_obj_list.pop_front(), node != nullptr)
                        ((object_type*)node)->~object_type();
                    m_memory_pool.reset();
                }

                void clear()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    object_node* node = nullptr;
                    while (node = m_free_obj_list.pop_front(), node != nullptr)
                        ((object_type*)node)->~object_type();
                    m_memory_pool.clear();
                }

                object_type* allocate()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    object_node* node = m_free_obj_list.pop_front();
                    if (node != nullptr)
                    {
                        node->m_next = (object_node*) MC_INVALID_PTR;
                        return (object_type*)node;
                    }
                    node = (object_node*)m_memory_pool.allocate();
                    M_ASSERT(node != nullptr);
                    new (node) object_type;

                    node->m_next = (object_node*) MC_INVALID_PTR;
                    return (object_type*)node;
                }

                void deallocate(object_type* obj)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    object_node* node = (object_node*)obj;
                    if (node->m_next != (object_node*)MC_INVALID_PTR)
                    {
                        M_ASSERT(false);
                        return;
                    }

                    m_free_obj_list.push_back(node);

                    if (_need_compact_nolock())
                    {
                        while (node = m_free_obj_list.pop_front(), node != nullptr)
                        {
                            ((object_type*)node)->~object_type();
                            m_memory_pool.deallocate(node);
                        }
                    }
                }

                void compact()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    object_node* node = nullptr;
                    while (node = m_free_obj_list.pop_front(), node != nullptr)
                    {
                        ((object_type*)node)->~object_type();
                        m_memory_pool.deallocate(node);
                    }
                }
                

#ifdef _DEBUG_MEM_OBJ_POOL
            protected:
                ::pilo::i64_t _calculate_usage_nolock(size_t * total)
                {
                    ::pilo::i64_t free_obj_count = m_free_obj_list.size();
                    ::pilo::i64_t full_piece_count = m_memory_pool.m_full_piece_list.size();
                    ::pilo::i64_t avliad_piece_count = m_memory_pool.m_available_piece_list.size();

                    ::pilo::i64_t full_obj_count = (full_piece_count*_Step);
                    ::pilo::i64_t diff = full_obj_count - free_obj_count;

                    diff = diff + ((avliad_piece_count*_Step) - m_memory_pool.calculate_spare_available_units_nolock());

                    if (total != nullptr)
                    {
                        *total = m_memory_pool.piece_count_nolock() * _Step;
                    }

                    return diff;
                }

            public:
                ::pilo::i64_t calculate_usage(size_t * total)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return _calculate_usage_nolock(total);
                }

                std::string& make_summary_report(std::string& str)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    char buffer[128];

                    unsigned int free_node_count = m_free_obj_list.size();
                    ::pilo::core::io::string_format_output(buffer,sizeof(buffer), "(COP@%p) FreeNode=%u\n",this,free_node_count);
                    str += buffer;
                    m_memory_pool.make_summary_report(str);
                    return str;
                } 
#endif

            protected:
                bool _need_compact_nolock() const
                {
                    size_t free_obj_count = m_free_obj_list.size();
                    size_t piece_count = m_memory_pool.piece_count_nolock();
                    return piece_count > 3 && (free_obj_count / (size_t)_Step) >= (piece_count / 2);
                }
            };

            template <typename T, size_t _Step, typename _Lock = pilo::core::threading::dummy_mutex>
            class portable_compactable_object_pool
            {
            public:
                typedef  compactable_object_pool<T, _Step, _Lock>       object_pool_type;
                static object_pool_type* pool()
                {
                    static object_pool_type _pool;
                    return &_pool;
                }

                static T* allocate()
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
                    M_ASSERT(p != nullptr);
                    if (p == nullptr)
                    {
                        return nullptr;
                    }
                    return p->allocate();
                }

                static void deallocate(T* object)
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
                    M_ASSERT(p != nullptr);
                    if (p == nullptr)
                    {
                        return;
                    }
                    p->deallocate(object);
                }

                static void compact()
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
                    if (p != nullptr)
                    {
                        p->compact();
                    }
                }

#ifdef _DEBUG_MEM_OBJ_POOL

                static ::pilo::i64_t calculate_usage(size_t * total)
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
                    M_ASSERT(p != nullptr);
                    if (p == nullptr)
                    {
                        return -1;
                    }
                    return p->calculate_usage(total);
                }

                static std::string& make_summary_report(std::string& str)
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
                    M_ASSERT(p != nullptr);
                    if (p == nullptr)
                    {
                        return str;
                    }
                    return p->make_summary_report(str);
                }
#endif
            };
        }
    }
}