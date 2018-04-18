#pragma once

#include "core/coredefs.hpp"
#include "core/threading/recursive_mutex.hpp"
#include "core/threading/mutex_locker.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
#ifdef WINDOWS
            class sync_condition_variable_any
            {
                public:
                sync_condition_variable_any()
                {
                    InitializeConditionVariable(&_m_os_handle);
                }

                ~sync_condition_variable_any()
                {

                }

                ::pilo::os_event_handle_t& native_handle()
                {
                    return _m_os_handle;
                }

                void notify_one()
                {
                    ::WakeConditionVariable(&_m_os_handle);
                }
                void notify_all()
                {
                    ::WakeAllConditionVariable(&_m_os_handle);
                }

                template <typename T>
                void wait(T& mutex)
                {
                    //::pilo::core::threading::mutex_locker<T> locker(mutex);
                    ::SleepConditionVariableCS(&_m_os_handle, &(mutex.native_handle()), INFINITE);
                }

                template <typename T>
                bool wait_for(T& mutex, size_t nTimeout = MC_INVALID_SIZE)
                {
                    if (nTimeout == MC_INVALID_SIZE) nTimeout = INFINITE;

                    //::pilo::core::threading::mutex_locker<T> locker(mutex);
                    if (::SleepConditionVariableCS(&_m_os_handle, &(mutex.native_handle()), (DWORD) nTimeout))
                    {
                        return true;
                    }
                    return false;
                }

                private:
                ::pilo::os_event_handle_t                                       _m_os_handle;
            };

#else
            class sync_condition_variable_any
            {
                public:
                sync_condition_variable_any()
                {
                    ::pthread_cond_init(&_m_os_handle, 0);
                }
                ~sync_condition_variable_any()
                {
                    ::pthread_cond_destroy(&_m_os_handle);
                }

                ::pilo::os_event_handle_t& native_handle()
                {
                    return _m_os_handle;
                }

                void notify_one()
                {
                    ::pthread_cond_signal(&_m_os_handle);
                }
                void notify_all()
                {
                    ::pthread_cond_broadcast(&_m_os_handle);
                }

                template <typename T>
                void wait(T& mutex)
                {
                    //::pilo::core::threading::mutex_locker<T> locker(mutex);
                    ::pthread_cond_wait(&_m_os_handle, &(mutex.native_handle()));
                }

                template <typename T>
                bool wait_for(T& mutex, size_t nTimeout = MC_INVALID_SIZE)
                {
                    //::pilo::core::threading::mutex_locker<T> locker(mutex);
                    if (nTimeout == MC_INVALID_SIZE)
                    {
                        if (0 != ::pthread_cond_wait(&_m_os_handle, &(mutex.native_handle())))
                        {
                            return false;
                        }
                    }
                    else
                    {
                        timespec ts;
                        ts.tv_sec = nTimeout / 1000;
                        ts.tv_nsec = (nTimeout % 1000) * 1000000;

                        if (0 != ::pthread_cond_timedwait(&_m_os_handle, &(mutex.native_handle()), &ts))
                        {
                            return false;
                        }
                    }

                    return true;
                }


                protected:
                ::pilo::os_event_handle_t 	_m_os_handle;
            };

#endif
        }
    }    
}