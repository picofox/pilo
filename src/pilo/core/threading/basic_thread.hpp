#pragma once
#include "core/coredefs.hpp"
#include "core/threading/nonrecursive_mutex.hpp"
#include "core/threading/advanced_mutex_locker.hpp"
using namespace pilo;

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            enum EnumThreadState
            {
                //Normal
                eNotStarted = 0, //The initial state of a thread, once it changed it never changed back.
                ePreparing = 1, //The time between create a thread and onRun() is called
                eRunning = 2, //The hole lifetime of onRun()
                eFinished = 3, //Thread transit to this state after onRun() is returned. This is the last phase of state.for detached threads. 
                eFinishedAndJoined = 4, //Thread transit to this state after it was joined by waitThread successfully. 
                //This  is the last phase of state.for joinable threads.

                //Abnormal
                eAborted = 0x80000003,
                eAbortedAndJoined = 0x80000004,
            };

            enum EnumThreadFlag
            {
                eShouldTerminate = 0x00000001,
            };

            class basic_thread
            {
            public: //static function
                static void sleep(unsigned long);
                static void msleep(unsigned long);
                static void usleep(unsigned long);

                static os_thread_id_t id();
                

            public: //friends function
                friend thread_routine_return_t THREAD_ROUTINE_CALL ThreadStartRoutineCallback(void* param);

            public:
                // ctors and dtors
                basic_thread();
                basic_thread(bool detach, size_t stack_size);
                virtual ~basic_thread();

                os_thread_id_t thread_id() const;

                //Thread Creation Parameters
                size_t stacksize_on_creation() const
                {
                    return _m_stacksize_on_creation;
                }

                bool detached_on_creation() const
                {
                    return _m_detach_on_creation;
                }
                i32_t set_detach_on_creation(bool do_deatach);

                EnumThreadState state() const
                {
                    return _m_state;
                }

                bool detached() const
                {
                    return _m_is_detached;
                }

                i32_t execute(uint_r timeoutTicks = (uint_r)-1);

                virtual void stop_sync(bool auto_reset = true);

                ::pilo::error_number_t wait(size_t timeout_ticks = MC_INVALID_SIZE);

                i32_t get_result(i32_t& resultCode);

                bool can_kill() const;

                

                //Thread command
                virtual bool termination_is_requested() const { return _m_flag & eShouldTerminate; }
                void request_advisory_termination(bool should_term);

                i32_t detach();
                i32_t kill();

            protected:
                //This method is the routine of the thread, must be rewrote
                virtual i32_t on_run() = 0;

                //This method will be called when run() is Finished normally
                virtual void on_finished() {}

                //This method will be called just before thread is killed.
                virtual void on_pre_killed() {}

                //This method will be called just after thread is killed.
                virtual void on_post_killed() {}


            protected: // member vars
                //Thread parameters
                const bool                          _m_detach_on_creation;
                const size_t                        _m_stacksize_on_creation;               

                //Thread status
                bool                                _m_is_detached;
                volatile EnumThreadState            _m_state;

                //Thread command
                volatile u32_t                      _m_flag;

                //Result code
                volatile i32_t                      _m_result_code;

                mutable ::pilo::core::threading::nonrecursive_mutex  _m_mutex;
                
#           ifdef WINDOWS
                HANDLE    _m_thread_handle;
                unsigned  _m_thread_id;

#           else
                pthread_t _m_thread_id;

#           endif

            protected:                
                bool _can_kill_no_mts() const;

            private: //method
                bool _is_stopped() const;
                i32_t _wait_for_running(uint_r tick = (uint_r)-1);
                
            };
        }
    }
}
