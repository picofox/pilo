#pragma once

#include "../../pilo.hpp"


namespace pilo
{
    namespace core
    {
        namespace memory
        {
            class memory_pool_allocator_interface
            {
            public:
                virtual ~memory_pool_allocator_interface() {};
                virtual ::pilo::pointer allocate() = 0;
                virtual void deallocate(::pilo::pointer p) = 0;
                virtual ::pilo::i64_t unit_size() const = 0;
                virtual ::pilo::i32_t init(::pilo::i64_t, ::pilo::i64_t) { return PILO_OK;  }                
            };

        }
    }
}