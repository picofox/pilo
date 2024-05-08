#pragma once

#include "../../pilo.hpp"
#include <sstream>
#include "../io/formatted_io.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  typename T>
            class object_pool_heap_allocator_adapter
            {
            public:
                typedef T object_type;
                object_pool_heap_allocator_adapter()
                {

                };
                ~object_pool_heap_allocator_adapter()
                {
                    this->reset();
                };

                inline void reset()
                {

                }

                inline void clear()
                {

                }

                inline object_type* allocate()
                {
                    return new object_type();
                }

                inline void deallocate(object_type* obj)
                {
                    delete obj;
                }

                void compact()
                {

                }

                std::string to_string()
                {
                    return std::string("object_pool_heap_allocator_adapter");
                }
            };

            template <typename T>
            class default_portable_object_pool
            {
            public:
                typedef  object_pool_heap_allocator_adapter<T>       object_pool_type;
                static object_pool_type* pool()
                {
                    static object_pool_type _pool;
                    return &_pool;
                }

                static T* allocate()
                {
                    object_pool_type* p = default_portable_object_pool::pool();
                    PMC_ASSERT(p != nullptr);
                    if (p == nullptr)
                    {
                        return nullptr;
                    }
                    return p->allocate();
                }

                static void deallocate(T* object)
                {
                    object_pool_type* p = default_portable_object_pool::pool();
                    PMC_ASSERT(p != nullptr);
                    if (p == nullptr)
                    {
                        return;
                    }
                    p->deallocate(object);
                }

                static void compact()
                {
                    object_pool_type* p = default_portable_object_pool::pool();
                    if (p != nullptr)
                    {
                        p->compact();
                    }
                }

                static std::string to_string()
                {
                    object_pool_type* p = default_portable_object_pool::pool();
                    PMC_ASSERT(p != nullptr);
                    if (p == nullptr) return "";
                    return p->to_string();
                }

            };
        }
    }
}