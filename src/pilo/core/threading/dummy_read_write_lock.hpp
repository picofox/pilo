#pragma once

#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class dummy_read_write_lock
            {
                public:
                inline void lock_shared() {}
                inline void lock() {}
                inline bool try_lock() { return true; }
                inline bool try_lock_shared() { return true; }
                inline void unlock() {}
                inline void unlock_shared() {}
            };
        }
    }
    
}


