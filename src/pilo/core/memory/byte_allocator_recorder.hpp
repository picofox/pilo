#pragma once

#include "../../pilo.hpp"
#include "../io/formatted_io.hpp"
#include <atomic>

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            class byte_allocator_recorder
            {
            public:
                byte_allocator_recorder() 
                {
                    _allocated_bytes.store(0);
                    _deallocated_bytes.store(0);
                }

                inline void on_allocated(::pilo::i64_t sz)
                {
                    _allocated_bytes += sz;
                }

                inline void on_deallocated(::pilo::i64_t sz)
                {
                    _deallocated_bytes += sz;
                }

                inline ::pilo::i64_t banlaced_bytes() const
                {
                    return _allocated_bytes - _deallocated_bytes;
                }

                inline ::pilo::i64_t allocated_bytes() const
                {
                    return _allocated_bytes;
                }

                inline ::pilo::i64_t deallocated_bytes() const
                {
                    return _deallocated_bytes;
                }

                inline ::pilo::i64_t get(::pilo::i64_t & albs, ::pilo::i64_t & dlbs)
                {
                    albs = _allocated_bytes;
                    dlbs = _deallocated_bytes;
                    return albs - dlbs;
                }

                inline const char* to_cstring(char* buffer, ::pilo::i64_t len)
                {
                    ::pilo::i64_t albs, dlbs;
                    ::pilo::i64_t b = this->get(albs, dlbs);
                    ::pilo::core::io::string_formated_output(buffer, len, "%lld-%lld=%lld", albs, dlbs, b);
                    return buffer;
                }

            private:
                std::atomic<::pilo::i64_t> _allocated_bytes;
                std::atomic<::pilo::i64_t> _deallocated_bytes;
                
            };
        }
    }
}