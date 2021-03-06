#pragma once

#include "core/threading/dummy_mutex.hpp"
#include "core/container/singly_linked_selflist.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/memory/memory_pool.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  typename T,
                        size_t _Step,
                        typename _Lock = pilo::core::threading::dummy_mutex >
            class autoreset_object_pool
            {
            public:
                typedef T object_type;
                typedef _Lock lock_type;
                typedef memory_pool<sizeof(object_node), _Step, pilo::core::threading::dummy_mutex>  pool_type;
                typedef typename pool_type::unit_node unit_node;

            public:
                lock_type   m_lock;
                pool_type   m_memory_pool;

            public:
                autoreset_object_pool() {}
                ~autoreset_object_pool() {}

                void reset()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_memory_pool.reset();
                }

                void clear()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_memory_pool.clear();
                }

                object_type* allocate()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    object_type* node = (object_type*)m_memory_pool.allocate();
                    M_ASSERT(node != nullptr);
                    new (node) object_type;
                    return (object_type*)node;
                }

                void deallocate(object_type* obj)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    unit_node* u = (unit_node*)obj;
                    M_ASSERT(u->m_next != (unit_node*)MC_INVALID_PTR);
                    obj->~object_type();

                    m_memory_pool.deallocate(obj);
                }                
            };

            template <typename T, size_t _Step, typename _Lock = pilo::core::threading::dummy_mutex>
            class portable_autoreset_object_pool
            {
            public:
                typedef  autoreset_object_pool<T, _Step, _Lock>       object_pool_type;
                static object_pool_type* pool()
                {
                    static object_pool_type _pool;
                    return &_pool;
                }

                static T* allocate()
                {
                    object_pool_type* p = portable_object_pool::pool();
                    M_ASSERT(p != nullptr);
                    return p->allocate();
                }

                static void deallocate(T* object)
                {
                    object_pool_type* p = portable_object_pool::pool();
                    M_ASSERT(p != nullptr);
                    p->deallocate(object);
                }
            };
        }
    }
}