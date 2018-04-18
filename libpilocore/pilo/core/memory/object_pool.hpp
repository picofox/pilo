#pragma once

#include "core/threading/dummy_mutex.hpp"
#include "core/container/singly_linked_selflist.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/memory/overlapped_memory_pool.hpp"

#ifdef _DEBUG_MEM_OBJ_POOL
#include <string>
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
            class object_pool
            {
            public:
                typedef T                                                                            object_type;
                typedef _Lock                                                                        lock_type;
                typedef pilo::core::container::singly_linked_list_node<object_type>                  object_node;                
                typedef overlapped_memory_pool<sizeof(object_node), _Step, pilo::core::threading::dummy_mutex>  pool_type;
                typedef pilo::core::container::singly_linked_selflist<object_node>                   object_list;

            public:
                lock_type   m_lock;
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
                    if (node == nullptr)
                    {
                        return (object_type*) nullptr;
                    }
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
                        //ASSERT_I(!" !!!!!!!!!!! free the pointer more than once !!!!!!!!!!!");
                        return;
                    }

                    m_free_obj_list.push_back(node);
                }

#ifdef _DEBUG_MEM_OBJ_POOL
            public:
                std::string make_report(::pilo::u32_t flag = 0, size_t level = 0)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    std::string str;
                    str.clear();
                    ::pilo::core::string::fixed_astring<128> tmp_str;
                    std::string tab_str;
                    tab_str.clear();
                    for (int i = 0; i < level; i++)
                    {
                        tab_str += "    ";
                    }

                    tmp_str.format("%sObject_pool @ 0x%p:\n", tab_str.c_str(), this);
                    str += tmp_str.c_str();

                    size_t fl_count = m_free_obj_list.size();
                    tmp_str.format("%s    Total Free Count:%u\n", tab_str.c_str(), fl_count);
                    str += tmp_str.c_str();

                    size_t i = 0;
                    if (flag & MB_DEBUG_POOL_SHOW_FREELIST_DETAIL)
                    {
                        object_node* node = m_free_obj_list.begin();
                        for (; node != m_free_obj_list.end(); node = m_free_obj_list.next(node))
                        {
                            tmp_str.format("%s        Free object_%u\t: @ 0x%p\n", tab_str.c_str(), i++, node);
                            str += tmp_str.c_str();
                        }
                    }

                    tmp_str.format("%s    LowLevel Memory Pool:\n", tab_str.c_str());
                    str += tmp_str.c_str();

                    std::string str2 = m_memory_pool.make_report(flag, level + 1);
                    str += str2;

                    return str;

                }
#endif
            };

            template <typename T, size_t _Step, typename _Lock = pilo::core::threading::dummy_mutex>
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
                    M_ASSERT(p != nullptr);
                    if (p == nullptr) return (T*) nullptr;
                    return p->allocate();
                }

                static void deallocate(T* object)
                {
                    object_pool_type* p = portable_object_pool::pool();
                    M_ASSERT(p != nullptr);
                    if (p == nullptr) return;
                    p->deallocate(object);
                }
            };
        }
    }
}