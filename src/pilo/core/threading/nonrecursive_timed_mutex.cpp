#include "nonrecursive_timed_mutex.hpp"
#include "atomic.hpp"
#include "../datetime/datetime.hpp"


namespace pilo
{
    namespace core
    {
        namespace threading
        {

#   ifdef WINDOWS

            pilo::core::threading::nonrecursive_timed_mutex::nonrecursive_timed_mutex()
            {
                _m_active_count = 0;
                _m_event = 0;
            }

            pilo::core::threading::nonrecursive_timed_mutex::~nonrecursive_timed_mutex()
            {
#ifdef _PILO_DEBUG
                long old_count = _m_active_count;
                bool const was_locked = (old_count&lock_flag_value) ? true : false;
                if (was_locked)
                {
                    M_ASSERT(false && "lock busy");
                }
#endif

#pragma warning(push)
#pragma warning(disable:4312)
                void* const old_event = PILO_WIN_INTERLOCKED_EXCHANGE_POINTER(&_m_event, 0);
#pragma warning(pop)

                if (old_event)
                {
                    ::CloseHandle(old_event);
                }
            }

            bool pilo::core::threading::nonrecursive_timed_mutex::try_lock()
            {
                return (_interlockedbittestandset(&_m_active_count, lock_flag_bit) == 0);
            }

            void pilo::core::threading::nonrecursive_timed_mutex::lock()
            {
                if ((_interlockedbittestandset(&_m_active_count, lock_flag_bit) == 0))
                {
                    return;
                }

                long old_count = _m_active_count;
                for (;;) //fcmt: spin and spin util...
                {
                    bool const was_locked = (old_count&lock_flag_value) ? true : false;
                    long const new_count = was_locked ? (old_count + 1) : (old_count | lock_flag_value);
                    long const current = PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE(&_m_active_count, new_count, old_count);
                    if (current == old_count)
                    {
                        if (was_locked)
                            old_count = new_count;
                        break;
                    }
                    old_count = current;
                }

                if (old_count&lock_flag_value)
                {
                    bool lock_acquired = false;
                    void* const sem = get_event();

                    do
                    {
                        unsigned const retval(WaitForSingleObjectEx(sem, INFINITE, 0));
                        M_ASSERT(0 == retval || WAIT_ABANDONED == retval);

                        old_count &= ~lock_flag_value;
                        old_count |= event_set_flag_value;

                        for (;;)
                        {
                            long const new_count = ((old_count&lock_flag_value) ? old_count : ((old_count - 1) | lock_flag_value))&~event_set_flag_value;
                            long const current = PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE(&_m_active_count, new_count, old_count);
                            if (current == old_count)
                            {
                                break;
                            }
                            old_count = current;
                        }

                        lock_acquired = !(old_count&lock_flag_value);
                    } while (!lock_acquired);       
                }

                return ;
            }

            void pilo::core::threading::nonrecursive_timed_mutex::unlock()
            {
                long const offset = lock_flag_value;
                long const old_count = PILO_WIN_INTERLOCKED_EXCHANGE_ADD(&_m_active_count, lock_flag_value);
                if (!(old_count&event_set_flag_value) && (old_count > offset))
                {
                    if (!_interlockedbittestandset(&_m_active_count, event_set_flag_bit))
                    {
                        ::SetEvent(get_event());
                    }
                }
            }

            void* pilo::core::threading::nonrecursive_timed_mutex::get_event()
            {

                void* current_event = interlocked_read_acquire(&_m_event);

                if (!current_event)
                {
                    void* const new_event = ::CreateEvent(0, FALSE, FALSE, 0);

#pragma warning(push)
#pragma warning(disable:4311)
#pragma warning(disable:4312)
                    void* const old_event = PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE_POINTER(&_m_event, new_event, 0);
#pragma warning(pop)

                    if (old_event != 0)
                    {
                        ::CloseHandle(new_event);
                        return old_event;
                    }
                    else
                    {
                        return new_event;
                    }
                }
                return current_event;
            }



#   else
            nonrecursive_timed_mutex::nonrecursive_timed_mutex()
            {
                pthread_mutexattr_t attr;
                pthread_mutexattr_init(&attr);
                pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_NORMAL);
                pthread_mutex_init(&_m_mutex, &attr);
            }

            nonrecursive_timed_mutex::~nonrecursive_timed_mutex()
            {
                pthread_mutex_destroy(&_m_mutex);
            }

            bool nonrecursive_timed_mutex::try_lock()
            {
                if (0 == pthread_mutex_trylock(&_m_mutex)) 
                {
                    return true;
                }
                return false;
            }

            void nonrecursive_timed_mutex::lock()
            {
                ::pthread_mutex_lock(&_m_mutex);
            }

            void nonrecursive_timed_mutex::unlock()
            {
                ::pthread_mutex_unlock(&_m_mutex);
            }

            bool nonrecursive_timed_mutex::_do_try_lock_until(struct timespec const &timeout)
            {
                int const err=pthread_mutex_timedlock(&_m_mutex,&timeout);
                M_ASSERT(!err || err==ETIMEDOUT);
//                 if (err != 0)
//                 {
//                     if(err != ETIMEDOUT)
//                     {
//                         M_ASSERT(false);
//                     }
//                     return false;
//                 }
                return (! err);                
            }

#endif
       

        }
    }
}
