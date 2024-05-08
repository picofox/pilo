#pragma once

#include <mutex>
#include "../../pilo.hpp"
#include "../threading/dummy_mutex.hpp"
#include "../container/singly_linked_selflist.hpp"
#include "../memory/compactable_memory_pool.hpp"
#include "../stat/pool_object_stat_info.hpp"

#include <sstream>
#include "../io/formatted_io.hpp"


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
                typedef compactable_memory_pool<sizeof(object_node), 
                        _Step, 
                        pilo::core::threading::dummy_mutex>                             pool_type;
                typedef pilo::core::container::singly_linked_selflist<object_node>      object_list;            

            public:
                mutable lock_type   m_mutex;
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
                    std::lock_guard<lock_type> locker(m_mutex);
                    object_node* node = nullptr;
                    while (node = m_free_obj_list.pop_front(), node != nullptr)
                        ((object_type*)node)->~object_type();
                    m_memory_pool.reset();
                }

                void clear()
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    object_node* node = nullptr;
                    while (node = m_free_obj_list.pop_front(), node != nullptr)
                        ((object_type*)node)->~object_type();
                    m_memory_pool.clear();
                }

                object_type* allocate()
                {
                    std::lock_guard<lock_type> locker(m_mutex);

                    object_node* node = m_free_obj_list.pop_front();
                    if (node != nullptr)
                    {
                        node->m_next = (object_node*) PMI_INVALID_PTR;
                        return (object_type*)node;
                    }
                    node = (object_node*)m_memory_pool.allocate();
                    PMC_ASSERT(node != nullptr);
                    new (node) object_type;

                    node->m_next = (object_node*)PMI_INVALID_PTR;
                    return (object_type*)node;
                }

                void deallocate(object_type* obj)
                {
                    std::lock_guard<lock_type> locker(m_mutex);

                    object_node* node = (object_node*)obj;
                    if (node->m_next != (object_node*)PMI_INVALID_PTR)
                    {
                        PMC_ASSERT(false);
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
                    std::lock_guard<lock_type> locker(m_mutex);
                    object_node* node = nullptr;
                    while (node = m_free_obj_list.pop_front(), node != nullptr)
                    {
                        ((object_type*)node)->~object_type();
                        m_memory_pool.deallocate(node);
                    }
                }

            //public:
            //    void calc_available_units_nolock(::pilo::i64_t& avial, ::pilo::i64_t& total) const
            //    {
            //        std::lock_guard<lock_type> locker(m_mutex);
            //        _calc_available_units_nolock(avial, total);
            //    }
            //   


            public:

                std::string to_string() const
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    ::pilo::i64_t avail = 0;
                    ::pilo::i64_t total = 0;
                    this->m_memory_pool.calc_available_units_nolock(avail, total, m_free_obj_list.size());
                    std::stringstream ss;
                    ss << "compactable_object_pool<" << sizeof(object_node) << "," << _Step << ">: " << "us=" << avail  << "/" << total << ", free_nodes=" << m_free_obj_list.size();
                    ss << ",size=" << (double)total * sizeof(object_node) / 1024.0f / 1024.0f << "MB";
                    return ss.str();
                }

            protected:
                bool _need_compact_nolock() const
                {
                    ::pilo::int_r free_obj_count = m_free_obj_list.size();
                    ::pilo::int_r piece_count = m_memory_pool.piece_count_nolock();
                    return piece_count > 3 && (free_obj_count / (::pilo::int_r)_Step) >= (piece_count / 2);
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

                static ::pilo::err_t update_pool_object_stat(::pilo::core::stat::pool_object_stat_manager::stat_item* si)
                {
                    ::pilo::i64_t avail = 0;
                    ::pilo::i64_t total = 0;
                    pool()->m_memory_pool.calc_available_units_nolock(avail, total, pool()->m_free_obj_list.size());
                    si->set(total, avail);
                    return PILO_OK;
                }

                static T* allocate()
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
					PMC_ASSERT(p != nullptr);
                    if (p == nullptr)
                    {
                        return nullptr;
                    }
                    return p->allocate();
                }

                static void deallocate(T* object)
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
					PMC_ASSERT(p != nullptr);
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

                static std::string to_string()
                {
                    object_pool_type* p = portable_compactable_object_pool::pool();
                    PMC_ASSERT(p != nullptr);
                    if (p == nullptr) return "";
                    return p->to_string();
                }

            };
        }
    }
}