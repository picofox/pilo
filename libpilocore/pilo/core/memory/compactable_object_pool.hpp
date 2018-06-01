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
                compactable_object_pool(bool manual_compact = false) : m_memory_pool(manual_compact)
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

                void set_manual_compact(bool enable)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_memory_pool.set_manual_compact(enable);
                }

                bool is_manual_compact() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return m_memory_pool.is_manual_compact();
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

                    if ((! m_memory_pool.is_manual_compact()) && _need_compact_nolock())
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
                    while (node = m_free_obj_list.pop_front(), node != nullptr)
                    {
                        ((object_type*)node)->~object_type();
                        m_memory_pool.deallocate(node);
                    }
                }

#ifdef _DEBUG_MEM_OBJ_POOL
            public:
                template <size_t _BUFFSZ>
                void make_summary_report(char (&buffer)[_BUFFSZ], const char* prefix = nullptr)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    if (prefix == nullptr)
                    {
                        prefix = "";
                    }

                    unsigned long long free_node_count = m_free_obj_list.size();
                    unsigned long long in_used_full_piece_count = m_memory_pool.m_full_piece_list.size();
                    unsigned long long in_used_aval_piece_count = m_memory_pool.m_available_piece_list.size();

                    ::pilo::core::io::string_format_output(buffer,_BUFFSZ, "%sFree=%llu InUsed:(Full=%llu Av=%llu) ",
                        prefix,free_node_count, in_used_full_piece_count,in_used_aval_piece_count);

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
                    return p->allocate();
                }

                static void deallocate(T* object)
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
                    M_ASSERT(p != nullptr);
                    p->deallocate(object);
                }

#ifdef _DEBUG_MEM_OBJ_POOL
                template <size_t _BUFFSZ>
                static void make_summary_report(char (&buffer)[_BUFFSZ], const char* prefix = nullptr)
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
                    M_ASSERT(p != nullptr);
                    p->make_summary_report(buffer, prefix);
                }
#endif
            };
        }
    }
}