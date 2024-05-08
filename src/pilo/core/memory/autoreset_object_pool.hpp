#pragma once

#include    <mutex>
#include "../threading/dummy_mutex.hpp"
#include "../container/singly_linked_selflist.hpp"
#include "../memory/memory_pool.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  typename T,
                        ::pilo::i64_t _Step,
                        typename _Lock = pilo::core::threading::dummy_mutex >
            class autoreset_object_pool
            {
            public:
                typedef T object_type;
                typedef _Lock lock_type;
				typedef pilo::core::container::singly_linked_list_node<object_type>					 object_node;
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
                    std::lock_guard<lock_type> locker(m_mutex);
                    m_memory_pool.reset();
                }

                void clear()
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    m_memory_pool.clear();
                }

                object_type* allocate()
                {
                    std::lock_guard<lock_type> locker(m_mutex);

                    object_type* node = (object_type*)m_memory_pool.allocate();
                    PMC_ASSERT(node != nullptr);
                    new (node) object_type;
                    return (object_type*)node;
                }

                void deallocate(object_type* obj)
                {
                    std::lock_guard<lock_type> locker(m_mutex);

                    unit_node* u = (unit_node*)obj;
					PMC_ASSERT(u->m_next != (unit_node*)-1);
                    obj->~object_type();

                    m_memory_pool.deallocate(obj);
                }                
            };

            template <typename T, ::pilo::i64_t _Step, typename _Lock = pilo::core::threading::dummy_mutex>
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
                    object_pool_type* p = portable_autoreset_object_pool::pool();
					PMC_ASSERT(p != nullptr);
                    return p->allocate();
                }

                static void deallocate(T* object)
                {
                    object_pool_type* p = portable_autoreset_object_pool::pool();
					PMC_ASSERT(p != nullptr);
                    p->deallocate(object);
                }
            };
        }
    }
}