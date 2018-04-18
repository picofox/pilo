#pragma once
#include "memory.hpp"
#include <cstdlib>

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            struct heap_memory_allocator
            {
                typedef size_t  size_type;
                typedef void* pointer;
                typedef const void* const_pointer;

                inline pointer allocate(size_type size)
                {
                    return ::malloc(size);
                }

                inline void deallocate(pointer p, size_type size)
                {
                    M_UNUSED(size);
                    ::free(p);
                }

                inline size_type unit_size() const
                {
                    return 1;
                }
                
                inline size_type max_size() const
                {
                    return MC_INVALID_SIZE;
                }

                inline void clear()
                {
                    return;
                }

            };
        }
    }
}