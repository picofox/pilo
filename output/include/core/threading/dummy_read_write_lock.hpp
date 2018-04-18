#pragma once

#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class dummy_read_write_lock
            {
                public:
                inline void lock_read() {};
                inline void lock_write() {};
                inline bool try_lock_read() {};
                inline bool try_lock_write() {};
                inline void unlock_read() {};
                inline void unlock_write() {};
            };
        }
    }
    
}


