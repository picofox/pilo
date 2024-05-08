#include "./read_write_mutex.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
#ifdef WINDOWS
            void read_write_mutex::lock_shared()
            {
                ::AcquireSRWLockShared(&m_lock);
            }

            void read_write_mutex::lock()
            {
                ::AcquireSRWLockExclusive(&m_lock);
            }

            bool read_write_mutex::try_lock_shared()
            {
                if (TRUE == ::TryAcquireSRWLockShared(&m_lock))
                {
                    return true;
                }
                return false;

            }

            bool read_write_mutex::try_lock()
            {
                if (TRUE == ::TryAcquireSRWLockExclusive(&m_lock))
                {
                    return true;
                }
                return false;
            }

            void read_write_mutex::unlock_shared()
            {
                ::ReleaseSRWLockShared(&m_lock);
            }

            void read_write_mutex::unlock()
            {
                ::ReleaseSRWLockExclusive(&m_lock);
            }

#else

            read_write_mutex::~read_write_mutex()
            {
                ::pthread_rwlock_destroy(&m_lock);
            }

            void read_write_mutex::lock_shared()
            {
                ::pthread_rwlock_rdlock(&m_lock);
            }

            void read_write_mutex::lock()
            {
                ::pthread_rwlock_wrlock(&m_lock);
            }

            bool read_write_mutex::try_lock_shared()
            {
                if (0 == ::pthread_rwlock_tryrdlock(&m_lock))
                {
                    return true;
                }
                return false;

            }

            bool read_write_mutex::try_lock()
            {
                if (0 == ::pthread_rwlock_trywrlock(&m_lock))
                {
                    return true;
                }
                return false;
            }

            void read_write_mutex::unlock_shared()
            {
                ::pthread_rwlock_unlock(&m_lock);
            }

            void read_write_mutex::unlock()
            {
                ::pthread_rwlock_unlock(&m_lock);
            }



#endif

        }
    }
    
}