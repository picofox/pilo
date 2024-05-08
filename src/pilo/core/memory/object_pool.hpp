#pragma once

#include <mutex>
#include "../../pilo.hpp"
#include "../threading/dummy_mutex.hpp"
#include "../container/singly_linked_selflist.hpp"
#include "../memory/memory_pool.hpp"
#include <set>


#include <string>
#include <sstream>




namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  typename T,
                        ::pilo::i64_t _Step,
                        typename _Lock = pilo::core::threading::dummy_mutex>
            class object_pool
            {
            public:
                typedef T                                                                            object_type;
                typedef _Lock                                                                        lock_type;
                typedef pilo::core::container::singly_linked_list_node<object_type>                  object_node;                
                typedef memory_pool<sizeof(object_node), _Step, pilo::core::threading::dummy_mutex>  pool_type;
                typedef pilo::core::container::singly_linked_selflist<object_node>                   object_list;

            public:
                mutable lock_type   m_mutex;
                object_list m_free_obj_list;
                pool_type   m_memory_pool;

            public:
                object_pool() {};
                ~object_pool() 
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
                        node->m_next = (object_node*)PMI_INVALID_PTR;
                        return (object_type*)node;
                    }
                    node = (object_node*)m_memory_pool.allocate();
                    PMC_ASSERT(node != nullptr);
                    if (node == nullptr)
                    {
                        return (object_type*) nullptr;
                    }
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
                        //ASSERT_I(!" !!!!!!!!!!! free the pointer more than once !!!!!!!!!!!");
                        return;
                    }

                    m_free_obj_list.push_back(node);
                }

                std::string to_string() const
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    ::pilo::i64_t avail = 0;
                    ::pilo::i64_t total = 0;
                    this->m_memory_pool._calc_available_units_nolock(avail, total);
                    std::stringstream ss;
                    ss << "object_pool<" << sizeof(object_node) <<"," << _Step << ">: " << "us=" << avail + m_free_obj_list.size() << "/" << total << ", free_nodes=" << m_free_obj_list.size();
                    ss << ",size=" << (double)total * sizeof(object_node) / 1024.0f / 1024.0f << "MB";
                    return ss.str();
                }

            };

            template <typename T, ::pilo::i64_t _Step, typename _Lock = pilo::core::threading::dummy_mutex>
            class portable_object_pool
            {
            public:
                typedef  object_pool<T, _Step, _Lock>       object_pool_type;
                static object_pool_type* pool()
                {
                    static object_pool_type _pool;
                    return &_pool;
                }
                
                static T* allocate()
                {
                    object_pool_type* p = portable_object_pool::pool();
                    PMC_ASSERT(p != nullptr);
                    if (p == nullptr) return (T*) nullptr;
                    return p->allocate();
                }

                static void deallocate(T* object)
                {
                    object_pool_type* p = portable_object_pool::pool();
                    PMC_ASSERT(p != nullptr);
                    if (p == nullptr) return;
                    p->deallocate(object);
                }

                static std::string to_string()
                {
                    object_pool_type* p = portable_object_pool::pool();
                    PMC_ASSERT(p != nullptr);
                    if (p == nullptr) return "";
                    return p->to_string();
                }


            };
        }
    }
}