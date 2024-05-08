#pragma once
#include <chrono>
#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class dummy_mutex
            {
            public:
                //Tries to lock the mutex. Returns immediately. On successful lock acquisition returns true, otherwise returns false. 
                inline void lock() {}
                inline bool try_lock() { return true;  }
                template< class Rep, class Period >
                inline bool try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration) { return true; }
                template< class Clock, class Duration >
                inline bool try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time) { return true; }
                inline void unlock() {}


            };
        }
    }
}
