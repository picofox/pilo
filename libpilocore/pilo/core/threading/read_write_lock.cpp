#include "./read_write_lock.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
#ifdef WINDOWS
            void read_write_lock::lock_read()
            {
                ::AcquireSRWLockShared(&m_lock);
            }

            void read_write_lock::lock_write()
            {
                ::AcquireSRWLockExclusive(&m_lock);
            }

            bool read_write_lock::try_lock_read()
            {
                if (TRUE == ::TryAcquireSRWLockShared(&m_lock))
                {
                    return true;
                }
                return false;

            }

            bool read_write_lock::try_lock_write()
            {
                if (TRUE == ::TryAcquireSRWLockExclusive(&m_lock))
                {
                    return true;
                }
                return false;
            }

            void read_write_lock::unlock_read()
            {
                ::ReleaseSRWLockShared(&m_lock);
            }

            void read_write_lock::unlock_write()
            {
                ::ReleaseSRWLockExclusive(&m_lock);
            }

#else

            read_write_lock::~read_write_lock()
            {
                ::pthread_rwlock_destroy(&m_lock);
            }

            void read_write_lock::lock_read()
            {
                ::pthread_rwlock_rdlock(&m_lock);
            }

            void read_write_lock::lock_write()
            {
                ::pthread_rwlock_wrlock(&m_lock);
            }

            bool read_write_lock::try_lock_read()
            {
                if (0 == ::pthread_rwlock_tryrdlock(&m_lock))
                {
                    return true;
                }
                return false;

            }

            bool read_write_lock::try_lock_write()
            {
                if (0 == ::pthread_rwlock_trywrlock(&m_lock))
                {
                    return true;
                }
                return false;
            }

            void read_write_lock::unlock_read()
            {
                ::pthread_rwlock_unlock(&m_lock);
            }

            void read_write_lock::unlock_write()
            {
                ::pthread_rwlock_unlock(&m_lock);
            }



#endif

        }
    }
    
}