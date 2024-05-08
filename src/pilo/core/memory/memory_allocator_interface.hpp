#pragma once

#include "../../pilo.hpp"
#include "byte_allocator_recorder.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            class memory_allocator_interface
            {
            public:
                virtual ~memory_allocator_interface() {}
                virtual ::pilo::pointer allocate(::pilo::i64_t req, ::pilo::i64_t * ret) = 0;
                virtual ::pilo::pointer reallocate(::pilo::pointer p, ::pilo::i64_t old_sz, ::pilo::i64_t neo_sz, ::pilo::i64_t* ret) = 0;
                virtual void deallocate(::pilo::pointer p, ::pilo::i64_t sz) = 0;



            
#if PMI_MEMORY_ALLOCATOR_RECORDER_ENABLE_LEVEL > 0
            protected:
                byte_allocator_recorder _m_recorder;

#endif // 

            };
        }
    }
}