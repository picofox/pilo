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
                inline void lock_write() {}
                inline bool try_lock_read() { return true; }
                inline bool try_lock_write() { return true; }
                inline void unlock_read() {}
                inline void unlock_write() {}
            };
        }
    }
    
}


