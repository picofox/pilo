#include "recursive_mutex.hpp"
#include "core/threading/atomic.hpp"


#   ifdef WINDOWS
        pilo::core::threading::recursive_mutex::recursive_mutex()
        {
            _m_recursion_count = 0;
            _m_locking_thread_id = 0;
            _m_active_count = 0;
            _m_event = 0;
        }

        pilo::core::threading::recursive_mutex::~recursive_mutex()
        {

#pragma warning(push)
#pragma warning(disable:4312)
            void* const old_event = PILO_WIN_INTERLOCKED_EXCHANGE_POINTER(&_m_event, 0);
#pragma warning(pop)

            if (old_event)
            {
                ::CloseHandle(old_event);
            }
        }

        bool pilo::core::threading::recursive_mutex::try_lock()
        {
            long const current_thread_id = ::GetCurrentThreadId();
            return _try_recursive_lock(current_thread_id) || _try_basic_lock(current_thread_id);
        }

        void pilo::core::threading::recursive_mutex::lock()
        {
            long const current_thread_id = ::GetCurrentThreadId();
            if (!_try_recursive_lock(current_thread_id))
            {
                __lock();
                PILO_WIN_INTERLOCKED_EXCHANGE(&_m_locking_thread_id, current_thread_id);
                _m_recursion_count = 1;
            }
        }

        void pilo::core::threading::recursive_mutex::unlock()
        {
            if (!--_m_recursion_count)
            {
                PILO_WIN_INTERLOCKED_EXCHANGE(&_m_locking_thread_id, 0);
                __unlock();
            }
        }


        bool pilo::core::threading::recursive_mutex::_try_recursive_lock(long current_thread_id)
        {
            if (interlocked_read_acquire(&_m_locking_thread_id) == current_thread_id)
            {
                ++_m_recursion_count;
                return true;
            }
            return false;
        }


        void* pilo::core::threading::recursive_mutex::___get_event()
        {
            void* current_event = ::interlocked_read_acquire(&_m_event);

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

        void pilo::core::threading::recursive_mutex::___mark_waiting_and_try_lock(long& old_count)
        {
            for (;;)
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
        }


        void pilo::core::threading::recursive_mutex::___clear_waiting_and_try_lock(long& old_count)
        {
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
        }

        bool pilo::core::threading::recursive_mutex::__try_lock()
        {
            return (_interlockedbittestandset(&_m_active_count, lock_flag_bit) == 0);
        }


        void pilo::core::threading::recursive_mutex::__lock()
        {
            if (__try_lock())
            {
                return;
            }
            long old_count = _m_active_count;
            ___mark_waiting_and_try_lock(old_count);

            if (old_count&lock_flag_value)
            {
                bool lock_acquired = false;
                void* const sem = ___get_event();

                do
                {
                    unsigned const retval(::WaitForSingleObjectEx(sem, INFINITE, 0));
                    M_ASSERT(0 == retval || WAIT_ABANDONED == retval);
                    ___clear_waiting_and_try_lock(old_count);
                    lock_acquired = !(old_count&lock_flag_value);
                } while (!lock_acquired);
            }
        }


        void pilo::core::threading::recursive_mutex::__unlock()
        {
            long const offset = lock_flag_value;
            long const old_count = PILO_WIN_INTERLOCKED_EXCHANGE_ADD(&_m_active_count, lock_flag_value);
            if (!(old_count&event_set_flag_value) && (old_count > offset))
            {
                if (! _interlockedbittestandset(&_m_active_count, event_set_flag_bit))
                {
                    ::SetEvent(___get_event());
                }
            }
        }

        bool pilo::core::threading::recursive_mutex::_try_basic_lock(long current_thread_id)
        {
            if (__try_lock())
            {
                PILO_WIN_INTERLOCKED_EXCHANGE(&_m_locking_thread_id, current_thread_id);
                _m_recursion_count=1;
                return true;
            }
            return false;
        }

#else
        pilo::core::threading::recursive_mutex::recursive_mutex()
        {
            pthread_mutexattr_t attr;
            pthread_mutexattr_init(&attr);
            pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_RECURSIVE);
            pthread_mutex_init(&_m_mutex, &attr);
        }

        pilo::core::threading::recursive_mutex::~recursive_mutex()
        {
            pthread_mutex_destroy(&_m_mutex);
        }
        bool pilo::core::threading::recursive_mutex::try_lock()
        {
            if (0 == pthread_mutex_trylock(&_m_mutex)) {
                return true;
            }
            return false;
        }

        void pilo::core::threading::recursive_mutex::lock()
        {
            ::pthread_mutex_lock(&_m_mutex);
        }

        void pilo::core::threading::recursive_mutex::unlock()
        {
            ::pthread_mutex_unlock(&_m_mutex);
        }
#endif //WINDOWS