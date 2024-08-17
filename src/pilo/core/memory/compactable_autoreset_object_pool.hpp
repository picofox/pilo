#pragma once

#include <mutex>
#include "../threading/dummy_mutex.hpp"
#include "../container/singly_linked_selflist.hpp"
#include "../memory/compactable_memory_pool.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  typename T,
                ::pilo::i64_t _Step,
                typename _Lock = pilo::core::threading::dummy_mutex >
            class compactable_autoreset_object_pool
            {
            public:
                typedef T object_type;
                typedef _Lock lock_type;
				typedef pilo::core::container::singly_linked_list_node<object_type>									object_node;
                typedef compactable_memory_pool<sizeof(object_node), _Step,  pilo::core::threading::dummy_mutex>	pool_type;
                typedef typename pool_type::unit_node																unit_node;

            public:
                mutable lock_type   m_mutex;
				pool_type   m_memory_pool;

            protected:
                bool m_manual_compact;

            public:
                compactable_autoreset_object_pool() : m_memory_pool(),m_manual_compact(false)
                {
                }
                ~compactable_autoreset_object_pool() 
                {
                }

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
                    PMC_ASSERT(node);
                    new (node)object_type;
                    return (object_type*)node;
                }

                void deallocate(object_type* obj)
                {
                    std::lock_guard<lock_type> locker(m_mutex);

                    unit_node* u = (unit_node*)obj;
					PMC_ASSERT(u->m_flag != PMB_MEM_POOL_UNIT_FREED_FLAG);
                    obj->~object_type();

                    m_memory_pool.deallocate(obj);
                }
                
                void compact()
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    m_memory_pool.compact();
                }

                std::string to_string() const
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    ::pilo::i64_t avail = 0;
                    ::pilo::i64_t total = 0;
                    this->m_memory_pool.calc_available_units_nolock(avail, total, 0);
                    std::stringstream ss;
                    ss << "compactable_autoreset_object_pool<" << sizeof(object_node) << "," << _Step << ">: " << "us=" << avail << "/" << total << ", free_nodes=" << 0;
                    ss << ",size=" << (double)total * sizeof(object_node) / 1024.0f / 1024.0f << "MB";
                    return ss.str();
                }
            };

            template <typename T, ::pilo::i64_t _Step, typename _Lock = pilo::core::threading::dummy_mutex>
            class _portable_compactable_autoreset_object_pool
            {
            public:
                typedef  compactable_autoreset_object_pool<T, _Step, _Lock>       object_pool_type;
                static object_pool_type* pool()
                {
                    static object_pool_type _pool;
                    return &_pool;
                }

                static T* allocate()
                {
                    object_pool_type* p = _portable_compactable_autoreset_object_pool::pool();
					PMC_ASSERT(p != nullptr);
                    return p->allocate();
                }

                static void deallocate(T* object)
                {
                    object_pool_type* p = _portable_compactable_autoreset_object_pool::pool();
					PMC_ASSERT(p != nullptr);
                    p->deallocate(object);
                }

                static std::string to_string()
                {
                    object_pool_type* p = _portable_compactable_autoreset_object_pool::pool();
                    PMC_ASSERT(p != nullptr);
                    return p->to_string();
                }
            };
        }
    }
}