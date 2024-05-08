#pragma once

#include "../../pilo.hpp"
#include "memory_allocator_interface.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            class std_memory_allocator : public memory_allocator_interface
            {
            public:
                ~std_memory_allocator()
                {

                }

                ::pilo::pointer allocate(::pilo::i64_t req, ::pilo::i64_t* ret)
                {
#if PMI_MEMORY_ALLOCATOR_RECORDER_ENABLE_LEVEL > 0
                    this->_m_recorder.on_allocated(req);
#endif
                    
                    return PMF_HEAP_MALLOC(req);
                }

                ::pilo::pointer reallocate(::pilo::pointer p, ::pilo::i64_t old_sz, ::pilo::i64_t neo_sz, ::pilo::i64_t* ret)
                {
#if PMI_MEMORY_ALLOCATOR_RECORDER_ENABLE_LEVEL > 0
                    ::pilo::pointer np = PMF_HEAP_REALLOC(p, neo_sz);
                    if (np == p)
                    {
                        this->_m_recorder.on_allocated(neo_sz - old_sz);
                    }
                    else
                    {
                        this->_m_recorder.on_allocated(neo_sz);
                        this->_m_recorder.on_deallocated(old_sz);
                    }
#endif
                    return PMF_HEAP_REALLOC(p, neo_sz);
                }


                void deallocate(::pilo::pointer p, ::pilo::i64_t sz)
                {
#if PMI_MEMORY_ALLOCATOR_RECORDER_ENABLE_LEVEL > 0
                    this->_m_recorder.on_deallocated(sz);
#endif
                    return PMF_HEAP_FREE(p);
                }
            };
        }
    }
}