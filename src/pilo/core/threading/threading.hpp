#ifndef __pilo_core_threading_hpp_
#define __pilo_core_threading_hpp_



#include "../../types.hpp"




namespace pilo
{
    namespace core
    {
        namespace threading
        {
#ifdef WINDOWS
            __inline ::pilo::os_thread_id_t xpf_current_thread_id()
            {
                return  ::GetCurrentThreadId();
            }
#else
            __inline ::pilo::os_thread_id_t xpf_current_thread_id()
            {
#       ifdef __linux__
                return syscall(__NR_gettid);
#       else
                return pthread_self();
#       endif
        }

#endif
        }
    }
}












#endif