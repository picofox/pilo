#pragma once
#include "../coredefs.hpp"
#include "../datetime/datetime.hpp"
#include "core/threading/atomic.hpp"


#if (M_CXX_FEATURE_TIME)
#include <chrono>
#endif

namespace pilo
{
    namespace core
    {
        namespace threading
        {
                class nonrecursive_timed_mutex
                {
                public:
                    nonrecursive_timed_mutex();
                    ~nonrecursive_timed_mutex();

                    bool try_lock();
                    void lock();
                    void unlock();

#               ifdef WINDOWS
#if             (M_CXX_FEATURE_TIME)
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
                            void* const sem = get_event();

                            do
                            {
                                auto now_tp = Clock::now();
                                if (timeout_time <= now_tp)
                                {
                                    PILO_WIN_INTERLOCKED_DECREMENT(&_m_active_count);
                                    return false;
                                }

                                std::chrono::milliseconds rel_time = std::chrono::duration_cast<std::chrono::milliseconds>(timeout_time - now_tp);
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
                            }
                            while (!lock_acquired);
                        }

                        return true;
                    }
#                   else
                    bool try_lock_for(pilo::u32_t dura)
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
                            void* const sem = get_event();

                            do
                            {
                                if (dura <= 0)
                                {
                                    PILO_WIN_INTERLOCKED_DECREMENT(&_m_active_count);
                                    return false;
                                }

                                if (::WaitForSingleObjectEx(sem, dura, 0) != 0)
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
                            }
                            while (!lock_acquired);
                        }

                        return true;
                    }

                    bool try_lock_until(time_t secs)
                    {       
                        ::pilo::i64_t dura = ::pilo::core::datetime::datetime::now_millisecs();
                        dura = secs*1000 - dura;
                        return try_lock_for((::pilo::u32_t) dura);
                    }

#                   endif
#else
                    bool try_lock_for(pilo::u32_t dura)
                    {
                        return try_lock_until(std::chrono::system_clock::now() + std::chrono::milliseconds(dura));        
                    }
                    template< class Rep, class Period >
                    bool try_lock_for(const std::chrono::duration<Rep, Period>& timeout_duration)
                    {
                        return try_lock_until(std::chrono::system_clock::now() + timeout_duration);                        
                    }
                    bool try_lock_until(time_t secs)
                    {
                        struct timespec ts;
                        ts.tv_sec = secs;
                        ts.tv_nsec= 0;
                        return _do_try_lock_until(ts);
                    }  
                    template< class Clock, class Duration >
                    bool try_lock_until(const std::chrono::time_point<Clock, Duration>& timeout_time)
                    {
                        std::chrono::nanoseconds d = timeout_time.time_since_epoch();
                        struct timespec ts =  _nano_to_timespec(d);
                        return _do_try_lock_until(ts);
                    }

#endif

                protected:

#               ifdef WINDOWS
                    PILO_STATIC_CONSTANT(unsigned char, lock_flag_bit = 31);
                    PILO_STATIC_CONSTANT(unsigned char, event_set_flag_bit = 30);
                    PILO_STATIC_CONSTANT(long, lock_flag_value = 1 << lock_flag_bit);
                    PILO_STATIC_CONSTANT(long, event_set_flag_value = 1 << event_set_flag_bit);
                    void* _m_event;
                    long  _m_active_count;

                void* get_event();

#           else
                inline timespec _nano_to_timespec(std::chrono::nanoseconds const& ns)
                {
                    struct timespec ts;
                    ts.tv_sec = static_cast<long>(std::chrono::duration_cast<std::chrono::seconds>(ns).count());
                    ts.tv_nsec = static_cast<long>((ns - std::chrono::duration_cast<std::chrono::seconds>(ns)).count());
                    return ts;
                }
                bool _do_try_lock_until(struct timespec const &timeout);
//                 inline void _convert_rel_millisecs_to_tp(struct timespec& tp_result, ::pilo::u32_t dura)
//                 {
//                     clock_gettime(CLOCK_REALTIME, &tp_result);
//                     ::pilo::u32_t secs = dura / 1000;
//                     ::pilo::u32_t milli = dura % 1000;
//                     tp_result.tv_sec += secs;
//                     tp_result.tv_nsec += milli * 1000000;
// //                     if (tp_result.tv_nsec >= 1000000000)
// //                     {
// //                         tp_result.tv_sec += 1;
// //                         tp_result.tv_nsec -= 1000000000;
// //                     }                    
//                 }
                pthread_mutex_t _m_mutex;
#           endif


				private:
					M_DISABLE_COPY(nonrecursive_timed_mutex)

				};






        }
    }
}