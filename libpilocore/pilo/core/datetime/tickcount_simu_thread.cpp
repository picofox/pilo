#include "../../core/coredefs.hpp"

#ifndef WINDOWS

#	include <sys/time.h>
#	include <unistd.h>

using namespace pilo;

pilo::u64_t __GetMillisecondTickCount()
{
    timeval tv;
    pilo::u64_t ms = 0;
    gettimeofday(&tv, NULL);

    static time_t  firstSec = tv.tv_sec;
    tv.tv_sec -= firstSec;

    ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;

    return ms;
}

volatile pilo::u64_t __glb_tick_count = 0;
volatile int __glb_tick_count_simulative_thread_started = 0;

class tickcount_simu_thread
{
public:
    tickcount_simu_thread();
    ~tickcount_simu_thread();

    friend void* _tickcount_timer_callback(void* param);

    void run();

protected:
    volatile int _m_stopTimerRequired;
    pthread_t    _m_threadID;

};

void *
_tickcount_timer_callback(void* param)
{
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);

    tickcount_simu_thread* thread = (tickcount_simu_thread*)param;
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_testcancel();
    thread->run();
    return 0;
}

tickcount_simu_thread::tickcount_simu_thread()
{
    _m_stopTimerRequired = 0;

    int rc = -1;
    rc = pthread_create(&_m_threadID,
        (pthread_attr_t*) 0,
        _tickcount_timer_callback,
        (void*) this);
    if (rc != 0) {
        throw EC_START_THREAD_FAILED;
    }

    while (__glb_tick_count_simulative_thread_started == 0) {
        usleep(100);
    }
}

tickcount_simu_thread::~tickcount_simu_thread()
{
    _m_stopTimerRequired = 1;
    pthread_join(_m_threadID, 0);
}


void tickcount_simu_thread::run()
{
    struct timeval tv;
    
    pilo::u64_t nowTick;

    nowTick = __GetMillisecondTickCount();
    __glb_tick_count = nowTick;
    __glb_tick_count_simulative_thread_started = 1;

    while (_m_stopTimerRequired == 0) 
    {
        tv.tv_sec = 0; tv.tv_usec = 100;

        if (select(1, NULL, NULL, NULL, &tv) != 0) {
            usleep(100);
        }
        nowTick = __GetMillisecondTickCount();
        if (nowTick > __glb_tick_count) {
            __glb_tick_count = nowTick;
        }
    }

    //FormatOutputToConsole("timer thread stopped\n");
    return;
}


tickcount_simu_thread timer;


#endif
