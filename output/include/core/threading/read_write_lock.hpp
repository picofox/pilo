#pragma once

#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class read_write_lock
            {
                public:
#ifdef WINDOWS
                read_write_lock()
                {
                    ::InitializeSRWLock(&m_lock);
                }
#else
                read_write_lock()
                {
                    ::pthread_rwlock_init(&m_lock, NULL);
                }
                ~read_write_lock();
#endif 

                void lock_read();
                void lock_write();
                bool try_lock_read();
                bool try_lock_write();
                void unlock_read();
                void unlock_write();

                protected:
#ifdef WINDOWS
                SRWLOCK m_lock;
#else
                pthread_rwlock_t        m_lock;
#endif 

            };
        }
    }
    
}


