#pragma once
#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class user_mutex
            {
            public:
                user_mutex()
                {
#ifdef WINDOWS
                    InitializeCriticalSection(&internal_handle());
#else
                    pthread_mutexattr_t attr;
                    pthread_mutexattr_init(&attr);
                    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
                    pthread_mutex_init(&internal_handle(), &attr);
#endif
                }

                ~user_mutex()
                {
#ifdef WINDOWS
                    DeleteCriticalSection(&internal_handle());
#else
                    pthread_mutex_destroy(&internal_handle());
#endif
                }

                inline void lock() const
                {
#ifdef WINDOWS
                    EnterCriticalSection((CRITICAL_SECTION*)&internal_handle());
#else
                    pthread_mutex_lock((pthread_mutex_t*)&internal_handle());
#endif
                }

                inline void unlock() const
                {
#ifdef WINDOWS
                    LeaveCriticalSection((CRITICAL_SECTION*)&internal_handle());
#else
                    pthread_mutex_unlock((pthread_mutex_t*)&internal_handle());
#endif
                }

                inline bool try_lock() const
                {
#ifdef WINDOWS
                    if (TryEnterCriticalSection((CRITICAL_SECTION*)&internal_handle()))
                    {
                        return true;                           
                    }

#else 
                    if (0 == pthread_mutex_trylock((pthread_mutex_t*)&internal_handle())) 
                    {
                        return true;
                    }
#endif
                    return false;
                }

#ifdef WINDOWS
                CRITICAL_SECTION & internal_handle() const
                {
                    return _lock;
                }
#else
                pthread_mutex_t & internal_handle() const
                {
                    return _lock;
                }
#endif

            protected:
#ifdef WINDOWS
                mutable CRITICAL_SECTION _lock;
#else
                mutable pthread_mutex_t _lock;
#endif
            };
        }
    }
}