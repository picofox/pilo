#include "basic_thread.hpp"
#include "core/datetime/datetime.hpp"
#undef M_CXX_FEATURE_THREAD
#ifdef M_CXX_FEATURE_THREAD
#include <chrono>
#include <thread>
#endif

#include <exception> 

namespace pilo
{
    namespace core
    {
        namespace threading
        {
//thread entrance (static) define
#ifdef WINDOWS
            thread_routine_return_t THREAD_ROUTINE_CALL ThreadStartRoutineCallback(void* param)
            {
                basic_thread* thisThread = (basic_thread*)param;

                thisThread->_m_state = eRunning;
                try
                {
                    thisThread->_m_result_code = thisThread->on_run();
                }
                catch (...)
                {
                    thisThread->_m_state = eAborted;
                    thisThread->_m_result_code = pilo::EC_THREAD_ABORTED;
                    _endthreadex(EC_THREAD_ABORTED);
                    return 0;
                }                
                thisThread->_m_state = eFinished;
                thisThread->on_finished(); // do Some User task
             
                return 0;
            }

#else
            thread_routine_return_t THREAD_ROUTINE_CALL ThreadStartRoutineCallback(void* param)
            {
                pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
                basic_thread* thisThread = (basic_thread*)param;
                struct CThreadAutoFinalizer
                {
                    basic_thread*    m_thread;
                    CThreadAutoFinalizer(basic_thread *thr) { m_thread = thr; }
                    ~CThreadAutoFinalizer()
                    {
                        if ((ePreparing == m_thread->_m_state) || (eRunning == m_thread->_m_state)) 
                        {
                            m_thread->_m_state = eAborted;
                            m_thread->_m_result_code = EC_THREAD_ABORTED;
                            try 
                            {
                                m_thread->on_post_killed();
                            }
                            catch (...) 
                            {

                            }
                        }
                        else if (eFinished == m_thread->_m_state) 
                        {

                        }
                        else 
                        {
                            throw EC_THREAD_STATE_ERROR;
                        }
                    }
                } AutoFinalizer(thisThread);

                pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
                pthread_testcancel();
                thisThread->_m_state = eRunning;
                thisThread->_m_result_code = thisThread->on_run();
                thisThread->_m_state = eFinished;

                thisThread->on_finished(); // do Some User task

                pthread_exit((void*)0);
                return 0;
            }
#endif // WINDOWS

// class method define
#ifdef M_CXX_FEATURE_THREAD    

            void basic_thread::sleep(unsigned long secs)
            {
                std::chrono::seconds dura(secs);
                std::this_thread::sleep_for(dura);
            }

            void basic_thread::msleep(unsigned long millisecs)
            {
                std::chrono::milliseconds dura(millisecs);
                std::this_thread::sleep_for(dura);
            }

            void basic_thread::usleep(unsigned long ms)
            {
                std::chrono::microseconds dura(ms);
                std::this_thread::sleep_for(dura);
            } 

            pilo::os_thread_id_t basic_thread::id()
            {
                return std::this_thread::get_id();
            }

#else

#   ifdef WINDOWS
            void basic_thread::sleep(unsigned long secs)
            {
                ::Sleep(secs * 1000);
            }

            void basic_thread::msleep(unsigned long millisecs)
            {
                ::Sleep(millisecs);
            }

            void basic_thread::usleep(unsigned long ms)
            {
                ::Sleep((ms / 1000) + 1);
            }

            pilo::os_thread_id_t basic_thread::id()
            {
                return ::GetCurrentThreadId();
            }

#   else
#include <sys/time.h>
            static void thread_sleep(struct timespec *ti)
            {
                pthread_mutex_t mtx;
                pthread_cond_t cnd;

                pthread_mutex_init(&mtx, 0);
                pthread_cond_init(&cnd, 0);

                pthread_mutex_lock(&mtx);
                (void) pthread_cond_timedwait(&cnd, &mtx, ti);
                pthread_mutex_unlock(&mtx);

                pthread_cond_destroy(&cnd);
                pthread_mutex_destroy(&mtx);
            }

            void basic_thread::sleep(unsigned long secs)
            {
                struct timeval tv;
                gettimeofday(&tv, 0);
                struct timespec ti;
                ti.tv_sec = tv.tv_sec + secs;
                ti.tv_nsec = (tv.tv_usec * 1000);
                thread_sleep(&ti);
            }

            void basic_thread::msleep(unsigned long msecs)
            {
                struct timeval tv;
                gettimeofday(&tv, 0);
                struct timespec ti;

                ti.tv_nsec = (tv.tv_usec + (msecs % 1000) * 1000) * 1000;
                ti.tv_sec = tv.tv_sec + (msecs / 1000) + (ti.tv_nsec / 1000000000);
                ti.tv_nsec %= 1000000000;
                thread_sleep(&ti);
            }

            void basic_thread::usleep(unsigned long usecs)
            {
                struct timeval tv;
                gettimeofday(&tv, 0);
                struct timespec ti;

                ti.tv_nsec = (tv.tv_usec + (usecs % 1000000)) * 1000;
                ti.tv_sec = tv.tv_sec + (usecs / 1000000) + (ti.tv_nsec / 1000000000);
                ti.tv_nsec %= 1000000000;
                thread_sleep(&ti);
            }

            pilo::os_thread_id_t basic_thread::id()
            {
                return pthread_self();
            }

#   endif // WINDOS
#endif

#ifdef WINDOWS
            basic_thread::basic_thread() : _m_stacksize_on_creation(0), _m_detach_on_creation(false)
            {            
                _m_result_code = 0;
                _m_is_detached = false;
                _m_flag = 0;
                _m_state = eNotStarted;
                _m_thread_id = 0;
                _m_thread_handle = 0;
            }


            basic_thread::basic_thread(bool detach, size_t stack_size) : _m_detach_on_creation(detach), _m_stacksize_on_creation(stack_size) 
            {
                _m_result_code = 0;
                _m_is_detached = false;
                _m_flag = 0;
                _m_state = eNotStarted;
                _m_thread_id = 0;
                _m_thread_handle = 0;
            }


            basic_thread::~basic_thread()
            {
                if (_can_kill_no_mts())
                {
                    this->kill();
                }               
            }

            pilo::os_thread_id_t basic_thread::thread_id() const
            {
                if (_m_thread_handle == INVALID_HANDLE_VALUE)
                {
                    return GetThreadId(_m_thread_handle);
                }
                else
                {
                    return _m_thread_id;
                }
            }

            pilo::i32_t basic_thread::execute(uint_r timeoutTicks /*= (uint_r)-1*/)
            {
                advanced_mutex_locker<nonrecursive_mutex> guard(_m_mutex);

                //Can not run twice in the same time.
                if (!_is_stopped()) 
                {
                    return EC_THREAD_STATE_ERROR;
                }

                //reset detach state
                if (_m_is_detached) 
                {
                    _m_is_detached = false;
                }

                //reset termination command
                _m_flag &= ~eShouldTerminate;

                _m_state = ePreparing;

                //Create thread and start it, POSIX does not support create a "suspend" thread 
                _m_thread_handle = (HANDLE)_beginthreadex(
                    0, //Pointer to security descriptor
                    (unsigned int)_m_stacksize_on_creation, //Stacksize
                    ThreadStartRoutineCallback, //Start routine
                    this, //Argument
                    0, //flag
                    &_m_thread_id //ID of thread 
                    );

                if (_m_thread_handle == INVALID_HANDLE_VALUE) 
                {
                    _m_state = eNotStarted;
                    _m_thread_id = 0;
                    if (_m_detach_on_creation) {
                        _m_is_detached = false;
                    }
                    return MAKE_SYSERR(EC_START_THREAD_FAILED);
                }

                if (_wait_for_running(timeoutTicks) != EC_OK) 
                {
                    if (_m_thread_handle != INVALID_HANDLE_VALUE) 
                    {
                        ::CloseHandle(_m_thread_handle);
                        _m_thread_handle = INVALID_HANDLE_VALUE;
                    }
                    return EC_TIMEOUT;
                }

                // detach it if needed
                if (_m_detach_on_creation) 
                {
                    if (!::CloseHandle(_m_thread_handle)) 
                    {
                        return MAKE_SYSERR(EC_DETACH_THREAD_FAILED);
                    }
                    _m_thread_handle = INVALID_HANDLE_VALUE;
                    _m_is_detached = TRUE;
                }

                return EC_OK;
            }            

            pilo::error_number_t basic_thread::wait(size_t timeout_ticks)
            {
                advanced_mutex_locker<nonrecursive_mutex> guard(_m_mutex);

                DWORD tmpExitState;
                // do not call this inside the thread
                if (_m_thread_id == ::GetCurrentThreadId()) 
                {
                    return EC_INVALID_CALLER; //It can't not waits on itself!
                }

                if (_m_is_detached) 
                {
                    if (INVALID_HANDLE_VALUE != _m_thread_handle) 
                    {
                        return EC_UNDEFINED; //Internal data error, i can't explain that for now. 
                    }
                    return EC_NONSENSE_OPERATION; //Wait on a detached thread in NO meaning.
                } 
                else 
                {
                    if (INVALID_HANDLE_VALUE == _m_thread_handle) 
                    {
                        return EC_UNDEFINED; //Internal data error, i can't explain that for now.
                    }
                }

                if ((eRunning != _m_state)
                    && (eFinished != _m_state)
                    && (eAborted != _m_state)) 
                {
                    return EC_THREAD_STATE_ERROR;
                }

                pilo::i32_t   rc;
                DWORD native_timedout_ticks = (timeout_ticks == MC_INVALID_SIZE) ? INFINITE : (DWORD) timeout_ticks;
                switch (::WaitForSingleObject(_m_thread_handle, native_timedout_ticks))
                { 
                    //Only a running and joinable thread could be waited
                case WAIT_OBJECT_0:
                    {
                        ::GetExitCodeThread(_m_thread_handle, (LPDWORD)&tmpExitState);
                        if (0xFFFFFFFF == tmpExitState)
                        {
                            _m_state = eAbortedAndJoined;
                            rc = EC_THREAD_ABORTED;
                        }
                        else
                        {
                            _m_state = eFinishedAndJoined;
                            rc = EC_OK;
                        }
                        ::CloseHandle(_m_thread_handle);
                        _m_thread_handle = INVALID_HANDLE_VALUE;
                        _m_thread_id = 0;
                    }                    
                    break;

                case WAIT_FAILED:
                    {
                        rc = MAKE_SYSERR(EC_SYNC_THREAD_FAILED);
                    }                    
                    break;

                case WAIT_ABANDONED:
                    {
                        rc = EC_USER_CANCELED;
                    }                    
                    break;

                case WAIT_TIMEOUT:
                    {
                        rc = EC_TIMEOUT;
                    }                    
                    break;

                default:
                    rc = EC_UNDEFINED;
                    break;
               }
               return rc;
            }

            pilo::i32_t basic_thread::detach()
            {
                advanced_mutex_locker<nonrecursive_mutex> guard(_m_mutex);

                if (eRunning != _m_state) 
                {
                    return EC_THREAD_STATE_ERROR;
                }

                if (!_m_is_detached) 
                {
                    //Do detach only if it is a running and joinable thread
                    if (!::CloseHandle(_m_thread_handle)) 
                    {
                        return MAKE_SYSERR(EC_DETACH_THREAD_FAILED);
                    }
                    _m_thread_handle = INVALID_HANDLE_VALUE;
                    _m_is_detached = true;
                    return EC_OK;
                }

                return EC_ALREADY_DONE;
            }

            pilo::i32_t basic_thread::kill()
            {

                if (_m_thread_id == ::GetCurrentThreadId()) 
                {
                    return EC_INVALID_CALLER; //It can't not waits on itself!
                }

                if (! _can_kill_no_mts())
                {
                    return EC_THREAD_STATE_ERROR;
                }

                try 
                { 
                    on_pre_killed();
                } catch (...) {

                }

                if (_m_is_detached) 
                {
                    //Process Detached threads
                    if (INVALID_HANDLE_VALUE != _m_thread_handle) 
                    {
                        return EC_UNDEFINED;
                    }

                    HANDLE tmpHandle = ::OpenThread(THREAD_TERMINATE, 0 , _m_thread_id);
                    if (NULL == tmpHandle) 
                    {
                        return EC_THREAD_NOT_EXIST;
                    }

                    if (TRUE != ::TerminateThread(tmpHandle, 0xFFFFFFFF)) 
                    {
                        return MAKE_SYSERR(EC_KILL_THREAD_FAILED);
                    }

                    _m_thread_id = 0;                    

                    ::CloseHandle(tmpHandle);

                } 
                else 
                {
                    if (TRUE != TerminateThread(_m_thread_handle, 0xFFFFFFFF)) 
                    {
                        return MAKE_SYSERR(EC_KILL_THREAD_FAILED);
                    }

                    _m_thread_id = 0;
                    _m_thread_handle = INVALID_HANDLE_VALUE;
                }

                _m_state = eAborted;

                try { 
                    on_post_killed(); 
                } catch (...) {

                }

                return EC_OK;
            }

#else //posix
            basic_thread::basic_thread() : _m_detach_on_creation(false),_m_stacksize_on_creation(0)
            {
                _m_result_code = 0;
                _m_is_detached = false;
                _m_flag = 0;
                _m_state = eNotStarted;
                _m_thread_id = 0;
            }

            basic_thread::basic_thread(bool detach, size_t stack_size) : _m_detach_on_creation(detach), _m_stacksize_on_creation(stack_size) 
            {
                _m_result_code = 0;
                _m_is_detached = false;
                _m_flag = 0;
                _m_state = eNotStarted;
                _m_thread_id = 0;
            }

            basic_thread::~basic_thread()
            {
                if (!_m_is_detached)
                {
                    if (_m_state == ePreparing || _m_state == eRunning || _m_state == eFinished)
                    {
                        this->wait();
                    }
                }
            }

            i32_t basic_thread::execute(uint_r timeoutTicks /*= (uint_r)-1*/)
            {
                advanced_mutex_locker<nonrecursive_mutex> guard(_m_mutex);

                //CAutoThreadMutexLocker autoLocker(&_m_lock);

                //Can not execute again when excuting
                if (!_is_stopped())
                {
                    return EC_THREAD_STATE_ERROR;
                }

                //reset detach state
                if (_m_is_detached)
                {
                    _m_is_detached = false;
                }

                //reset termination command
                _m_flag &= ~eShouldTerminate;
                _m_state = ePreparing;

                pthread_attr_t attr;
                pthread_attr_init(&attr);

                if (_m_stacksize_on_creation != 0)
                {
                    if (0 != pthread_attr_setstacksize(&attr, _m_stacksize_on_creation))
                    {
                        pthread_attr_destroy(&attr);
                        return MAKE_SYSERR(EC_SET_THREAD_INFO_FAILED);
                    }
                }

                if (_m_detach_on_creation)
                {
                    if (0 != pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED))
                    {
                        pthread_attr_destroy(&attr);
                        return MAKE_SYSERR(EC_SET_THREAD_INFO_FAILED);
                    }
                    _m_is_detached = false;
                }
                else
                {
                    if (0 != pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE))
                    {
                        pthread_attr_destroy(&attr);
                        return MAKE_SYSERR(EC_SET_THREAD_INFO_FAILED);
                    }
                    _m_is_detached = false;
                }

                int rc = pthread_create(
                    &_m_thread_id,
                    &attr,
                    ThreadStartRoutineCallback,
                    this
                    );
                pthread_attr_destroy(&attr);

                if (0 != rc)
                {
                    if (_m_detach_on_creation)
                    {
                        _m_is_detached = false;
                    }
                    _m_thread_id = 0;
                    _m_state = eNotStarted;
                    return MAKE_SYSERR(EC_START_THREAD_FAILED);
                }

                if (_wait_for_running(timeoutTicks) != EC_OK)
                {
                    _m_thread_id = 0;
                    return EC_TIMEOUT;
                }

                return EC_OK;
            }
            pilo::error_number_t basic_thread::wait(size_t timeout_ticks)
            {
                advanced_mutex_locker<nonrecursive_mutex> guard(_m_mutex);

                if (_m_is_detached) 
                {
                    return EC_NONSENSE_OPERATION;
                }

                if (_m_thread_id == pthread_self()) 
                {
                    return EC_INVALID_CALLER;
                }

                if ((ePreparing != _m_state) 
                    && (eRunning != _m_state) 
                    && (eFinished != _m_state)
                    && (eAborted != _m_state)) 
                {
                    return EC_THREAD_STATE_ERROR;
                }                               

                if (timeout_ticks == MC_INVALID_SIZE)
                {
                    if (0 !=pthread_join(_m_thread_id,0)) 
                    {
                        return MAKE_SYSERR(EC_SYNC_THREAD_FAILED);
                    }
                }
                else
                {
                    struct timespec ts;
                    if (clock_gettime(CLOCK_REALTIME, &ts) == -1) 
                    {
                        return  MAKE_SYSERR(EC_SYNC_THREAD_FAILED);
                    }
                    
                    ts.tv_sec += timeout_ticks / 1000;
                    ts.tv_nsec += (timeout_ticks % 1000) * 1000 * 1000;
                      
                    int s = pthread_timedjoin_np(_m_thread_id, NULL, &ts); 
                    if (0 != s)
                    {
                        return ::pilo::EC_TIMEOUT;
                    }
                    return  MAKE_SYSERR(EC_SYNC_THREAD_FAILED);
                }
                

                if (eAborted == _m_state) 
                {
                    _m_state = eAbortedAndJoined;
                } else if (eFinished == _m_state) {
                    _m_state = eFinishedAndJoined;
                }
                else {
                    return EC_THREAD_STATE_ERROR;
                }

                return EC_OK;
            }            

            i32_t basic_thread::detach()
            {
                advanced_mutex_locker<nonrecursive_mutex> guard(_m_mutex);

                if (eRunning != _m_state)
                {
                    return EC_THREAD_STATE_ERROR;
                }

                if (!_m_is_detached)
                {
                    //Do detach only if it is a running and joinable thread
                    if (0 != pthread_detach(_m_thread_id))
                    {
                        return MAKE_SYSERR(EC_DETACH_THREAD_FAILED);
                    }
                    _m_is_detached = true;
                    return EC_OK;
                }

                return EC_ALREADY_DONE;
            }

            pilo::i32_t basic_thread::kill()
            {
                if (eRunning != this->_m_state) 
                {
                    return EC_THREAD_STATE_ERROR;
                }

                try 
                {
                    on_pre_killed();
                }
                catch (...) 
                {

                }

                _m_thread_id = 0;
                if (pthread_cancel(_m_thread_id) != 0) 
                {
                    return MAKE_SYSERR(EC_KILL_THREAD_FAILED);
                }

                _m_state = eAborted;

                try 
                {
                    on_post_killed();
                }
                catch (...) {

                }

                return EC_OK;
            }

#endif //WINDOWS

            void basic_thread::stop_sync(bool auto_reset)
            {
                basic_thread::request_advisory_termination(true);
                wait();
                if (auto_reset)
                {
                    basic_thread::request_advisory_termination(false);
                }
            }

            i32_t basic_thread::get_result(i32_t& resultCode)
            {
                if ((_m_state >> 31) & 1)
                { //Aborted
                    return EC_THREAD_ABORTED;
                }
                else 
                {
                    if ((eFinished != _m_state) && (eFinishedAndJoined != _m_state)) {
                        return EC_THREAD_STATE_ERROR;
                    }
                }

                resultCode = _m_result_code;
                return EC_OK;
            }

            void basic_thread::request_advisory_termination(bool should_term)
            {                
                if (should_term)
                {
                    _m_flag |= eShouldTerminate;
                }
                else
                {
                    _m_flag &= ~eShouldTerminate;
                }
            }

            bool basic_thread::_is_stopped() const
            {
                if (eNotStarted == _m_state)
                {
                    return true;
                }

                if (_m_is_detached)
                {
                    if ((eFinished == _m_state) ||
                        (eAborted == _m_state))
                    {
                        return true;
                    }
                }
                else
                {
                    if ((eFinishedAndJoined == _m_state) ||
                        (eAbortedAndJoined == _m_state))
                    {
                        return true;
                    }

                }
                return false;
            }

            i32_t basic_thread::_wait_for_running(uint_r tick /*= (uint_r)-1*/)
            {
                pilo::tick64_t startTick = PiloGetTickCount64();
                while (ePreparing == _m_state) 
                {
                    basic_thread::msleep(0);
                    pilo::tick64_t checkTick = PiloGetTickCount64();
                    if (checkTick - startTick >= tick) 
                    {
                        return EC_TIMEOUT;
                    }
                }
                return EC_OK;
            }


            bool basic_thread::can_kill() const
            {
                advanced_mutex_locker<nonrecursive_mutex> guard(_m_mutex);
                return _can_kill_no_mts();
            }

            bool basic_thread::_can_kill_no_mts() const
            {
                if (_m_thread_id != 0)
                {
                    if (_m_state == ePreparing || _m_state == eRunning || _m_state == eFinished)
                    {
                        return true;
                    }
                }
                return false;
            }

        } //end of thread
    } // end of core
} // end of pilo
