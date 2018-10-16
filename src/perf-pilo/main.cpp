#include <cstring>
#include "core/io/format_output.hpp"
#include "core/fs/fs_util.hpp"
#include "core/process/process_util.hpp"
#include "core/random/simple_random.hpp"
#include "core/datetime/datetime.hpp"
#include "core/pattern/lazy_singletion_mts.hpp"
#include <iostream>
#include <limits>
#include "pilo_perf_common.hpp"

#include "core/threading/recursive_timed_mutex.hpp"
#include "core/threading/basic_thread.hpp"
#include "core/threading/nonrecursive_mutex.hpp"

#ifdef TEST_VS_BOOST
#   ifdef WINDOWS
#       ifdef _DEBUG
#           pragma comment(lib,"libboost_chrono-vc120-mt-gd-x64-1_66.lib")
#       else
#           pragma comment(lib,"libboost_chrono-vc120-mt-s-x64-1_66.lib")
#       endif
#   endif
#endif


int
main(int argc, char *argv[])
{
    M_UNUSED(argc);
    M_UNUSED(argv);

    //Test now tick
  //  ::pilo::test::perf_now_ticks();
   // ::pilo::test::perf_steady_now_ticks();
    

    //Test mutexes
    //nonrecursive_mutex 
    ::pilo::test::perf_nonrecursive_mutex();
    ::pilo::test::perf_nonrecursive_mutex_try();
    
    //nonrecursive_timed_mutex
    ::pilo::test::perf_nonrecursively_timed_mutex();
    ::pilo::test::perf_nonrecursively_timed_mutex_try_lock_until();
    ::pilo::test::perf_nonrecursively_timed_mutex_try_lock_for();
    ::pilo::test::perf_nonrecursive_mutex_try();
 
    //recursive_mutex
    ::pilo::test::perf_recursively_mutex();
    ::pilo::test::perf_recursively_mutex_try();
 
     //recursive_timed_mutex
    ::pilo::test::perf_recursively_timed_mutex();
    ::pilo::test::perf_recursively_timed_mutex_try_lock_until();
    ::pilo::test::perf_recursively_timed_mutex_try_lock_for();
    ::pilo::test::perf_recursively_timed_mutex_try();
     


    getchar();
    
    return  0;
}