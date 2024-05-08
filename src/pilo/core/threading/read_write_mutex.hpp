#pragma once

#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class read_write_mutex
            {
                public:
#ifdef WINDOWS
                read_write_mutex()
                {
                    ::InitializeSRWLock(&m_lock);
                }
#else
                read_write_mutex()
                {
                    ::pthread_rwlock_init(&m_lock, NULL);
                }
                ~read_write_mutex();
#endif 

                void lock_shared();
                void lock();
                bool try_lock_shared();
                bool try_lock();
                void unlock_shared();
                void unlock();

                protected:
#ifdef WINDOWS
                SRWLOCK					m_lock;
#else
                pthread_rwlock_t        m_lock;
#endif 

            };
        }
    }
    
}


