#pragma once
#include "../coredefs.hpp"
#include "../datetime/datetime.hpp"
#include "core/threading/atomic.hpp"

#if (M_CXX_FEATURE_TIME)
#   include <chrono>
#endif

namespace pilo
{
    namespace core
    {
        namespace threading
        {


            class recursive_timed_mutex
            {
            public:
                recursive_timed_mutex();
                ~recursive_timed_mutex();

                bool try_lock();
                void lock();
                void unlock();

#ifdef WINDOWS                

#   if (M_CXX_FEATURE_TIME)
                bool try_lock_for(pilo::u32_t dura)
                {
                    std::chrono::milliseconds ms(dura);
                    return try_lock_until(std::chrono::system_clock::now() + ms);
                }

                bool try_lock_until(time_t secs)
                {

                    std::chrono::time_point<std::chrono::system_clock> then_tp = std::chrono::system_clock::from_time_t(secs);
                    return try_lock_until(then_tp);
                }

                template< class Rep, class Period >
                bool try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration)
                {
                    return try_lock_until(std::chrono::system_clock::now() + timeout_duration);
                }

                template< class Clock, class Duration >
                bool try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
                {
                    long const current_thread_id = ::GetCurrentThreadId();
                    return _try_recursive_lock(current_thread_id) || _try_timed_lock_until(current_thread_id, timeout_time);
                }
#   else
                bool try_lock_for(pilo::u32_t dura)
                {
                    long const current_thread_id = ::GetCurrentThreadId();
                    return _try_recursive_lock(current_thread_id) || _try_timed_lock_for(current_thread_id, dura);
                }

                bool try_lock_until(time_t secs)
                {
                    ::pilo::i64_t dura = ::pilo::core::datetime::datetime::now_millisecs();
                    dura = secs*1000 - dura;
                    long const current_thread_id = ::GetCurrentThreadId();  
                    return _try_recursive_lock(current_thread_id) || _try_timed_lock_for(current_thread_id, (::pilo::u32_t) dura);
                }
#   endif
                
#else
                bool try_lock_for(pilo::u32_t dura)
                {
                    return try_lock_until(std::chrono::system_clock::now() + std::chrono::milliseconds(dura));    
                }                
                
                bool try_lock_until(time_t secs)
                {
                    struct timespec ts;
                    ts.tv_sec = secs;
                    ts.tv_nsec = 0;
                    return _do_try_lock_until(ts);
                }  
#   if (M_CXX_FEATURE_TIME)
                template< class Clock, class Duration >
                bool try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
                {
                    std::chrono::nanoseconds d = timeout_time.time_since_epoch();
                    struct timespec ts = _nano_to_timespec(d);
                    return _do_try_lock_until(ts);
                }

                template< class Rep, class Period >
                bool try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration)
                {
                    return try_lock_until(std::chrono::system_clock::now() + timeout_duration);
                }
#   endif

#endif // WINDOWS            

            protected:
#           ifdef WINDOWS
                long _m_recursion_count;
                long _m_locking_thread_id;
                PILO_STATIC_CONSTANT(unsigned char, lock_flag_bit = 31);
                PILO_STATIC_CONSTANT(unsigned char, event_set_flag_bit = 30);
                PILO_STATIC_CONSTANT(long, lock_flag_value = 1 << lock_flag_bit);
                PILO_STATIC_CONSTANT(long, event_set_flag_value = 1 << event_set_flag_bit);
                long _m_active_count;
                void* _m_event;

                void* ___get_event();
                inline void ___mark_waiting_and_try_lock(long& old_count)
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
                inline void ___clear_waiting_and_try_lock(long& old_count)
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
                inline bool __try_lock()
                {
                    return (_interlockedbittestandset(&_m_active_count, lock_flag_bit) == 0);
                }

                void __lock();
                void __unlock();
                bool _try_recursive_lock(long current_thread_id);                
                bool _try_basic_lock(long current_thread_id);
#if (M_CXX_FEATURE_TIME)
                template< class Clock, class Duration >
                bool __try_timed_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
                {
                    if ((_interlockedbittestandset(&_m_active_count, lock_flag_bit) == 0))
                    {
                        return true;
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
                        void* const sem = ___get_event();

                        do
                        {
                            //chrono::time_point<chrono::system_clock, chrono::system_clock::duration> now = chrono::system_clock::now();
                            auto tp_now = Clock::now();
                            if (timeout_time <= tp_now) {
                                PILO_WIN_INTERLOCKED_DECREMENT(&_m_active_count);
                                return false;
                            }

                            std::chrono::milliseconds rel_time = std::chrono::duration_cast<std::chrono::milliseconds>(timeout_time - tp_now);
                            if (rel_time.count() <= 0)
                            {
                                PILO_WIN_INTERLOCKED_DECREMENT(&_m_active_count);
                                return false;
                            }

                            if (::WaitForSingleObjectEx(sem, static_cast<unsigned long>(rel_time.count()), 0) != 0)
                            {
                                PILO_WIN_INTERLOCKED_DECREMENT(&_m_active_count);
                                return false;
                            }

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
                    return true;
                }
                template <typename TP>
                bool _try_timed_lock_until(long current_thread_id, TP const& target)
                {
                    if (__try_timed_lock_until(target))
                    {
                        PILO_WIN_INTERLOCKED_EXCHANGE(&_m_locking_thread_id, current_thread_id);
                        _m_recursion_count = 1;
                        return true;
                    }
                    return false;
                }  
#else
                bool __try_timed_lock_for(::pilo::u32_t millisecs)
                {
                    if ((_interlockedbittestandset(&_m_active_count, lock_flag_bit) == 0))
                    {
                        return true;
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
                        void* const sem = ___get_event();

                        do
                        {
                            if (millisecs <= 0)
                            {
                                PILO_WIN_INTERLOCKED_DECREMENT(&_m_active_count);
                                return false;
                            }

                            if (::WaitForSingleObjectEx(sem, (DWORD) millisecs, 0) != 0)
                            {
                                PILO_WIN_INTERLOCKED_DECREMENT(&_m_active_count);
                                return false;
                            }

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
                    return true;
                }

                bool _try_timed_lock_for(long current_thread_id, ::pilo::u32_t millisecs)
                {
                    if (__try_timed_lock_for(millisecs))
                    {
                        PILO_WIN_INTERLOCKED_EXCHANGE(&_m_locking_thread_id, current_thread_id);
                        _m_recursion_count = 1;
                        return true;
                    }
                    return false;
                }  

#endif

#           else
                inline timespec _nano_to_timespec(std::chrono::nanoseconds const& ns)
                {
                    struct timespec ts;
                    ts.tv_sec = static_cast<long>(std::chrono::duration_cast<std::chrono::seconds>(ns).count());
                    ts.tv_nsec = static_cast<long>((ns - std::chrono::duration_cast<std::chrono::seconds>(ns)).count());
                    return ts;
                }
                bool _do_try_lock_until(struct timespec const &timeout);
                pthread_mutex_t _m_mutex;

#           endif

            private:
                M_DISABLE_COPY(recursive_timed_mutex)

            };
        }
    }
}