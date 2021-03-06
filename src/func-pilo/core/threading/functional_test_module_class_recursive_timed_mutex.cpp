#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_class_recursive_timed_mutex.hpp"
#include "core/threading/recursive_timed_mutex.hpp"
#include <string>
#include "core/threading/atomic.hpp"


using namespace pilo::core::threading;
namespace pilo
{
    namespace test
    {


        static pilo::i32_t functional_test_lock_and_unlock(void* param);
        static pilo::i32_t functional_test_lock_and_unlock_recur(void* param);

        
        static pilo::i32_t functional_test_trylock(void* param);
        static pilo::i32_t functional_test_time_lock(void* param);

        pilo::test::testing_case g_functional_cases_recursive_timed_mutex[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "lock and unlock                               ", nullptr, functional_test_lock_and_unlock, 0, -1, (pilo::u32_t) - 1 },
            { 2, "lock and unlock recursively                   ", nullptr, functional_test_lock_and_unlock_recur, 0, -1, (pilo::u32_t) - 1 },
            { 2, "trylock                                       ", nullptr, functional_test_trylock, 0, -1, (pilo::u32_t) - 1 },
            { 3, "try_lock_for                                  ", nullptr, functional_test_time_lock, 0, -1, (pilo::u32_t) - 1 },

            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_test_lock_and_unlock_recur(void* param)
        {
            M_UNUSED(param);

            const pilo::uint_r nloops = 2000;

            pilo::uint_r value = 0;

            recursive_timed_mutex   mutex;
            c_test_mutex_lock_and_unlock_recur<recursive_timed_mutex> test_thr;
            test_thr.m_loops = nloops;
            test_thr.m_value = &value;
            test_thr.m_mutex = &mutex;


            test_thr.execute();

            for (int i = 0; i < (int) nloops; i++)
            {
                for (int j = 0; j < 100; j++)
                {
                    mutex.lock();
                }

                value ++;
                for (int j = 0; j < 100; j++)
                {
                    mutex.unlock();
                }
            }       

            test_thr.wait();

            if (value != nloops * 2)
            {
                return  -1;
            }

            return 0;
        }


        pilo::i32_t functional_test_lock_and_unlock(void* param)
        {
            M_UNUSED(param);

            const pilo::u32_t AddTimes = 20000;
            const pilo::u32_t NumThreads = 4;

            pilo::uint_r value_to_add = 0;


            recursive_timed_mutex mutex;

            c_test_mutex_lock_and_unlock<recursive_timed_mutex> thr_test_lock[NumThreads];


            for (int i = 0; i < (int) MF_COUNT_OF(thr_test_lock); i++)
            {
                thr_test_lock[i].m_value = &value_to_add;
                thr_test_lock[i].m_mutex = &mutex;
                thr_test_lock[i].m_loops = AddTimes;
                thr_test_lock[i].execute(MC_INVALID_SIZE);
            }

            for (int i = 0; i < (int) MF_COUNT_OF(thr_test_lock); i++)
            {
                thr_test_lock[i].wait();
            }

            if (value_to_add != (AddTimes * NumThreads))
            {
                return -1;
            }

            return 0;
        }

        pilo::i32_t functional_test_trylock(void* param)
        {
            M_UNUSED(param);

            recursive_timed_mutex mutex;
            c_test_mutex_trylock_for_false<recursive_timed_mutex> try_thr;
            try_thr.m_mutex = &mutex;
            try_thr.m_loops = 2000;

            mutex.lock();
            try_thr.execute();

            for (int i = 0; i < 1000; i++)
            {
                basic_thread::msleep(10);
                if (mutex.try_lock() != true)
                {                    
                    return -1;
                }
            }

            for (int i = 0; i < 1000; i++)
            {
                mutex.unlock();
            }

            try_thr.wait();
            pilo::i32_t result;
            if (try_thr.get_result(result) != pilo::EC_OK)
            {
                return -10;
            }

            if (result != 0)
            {
                return -20;
            }

            mutex.unlock();


            return 0;
        }

        pilo::i32_t functional_test_time_lock(void* param)
        {
            M_UNUSED(param);
            recursive_timed_mutex mutex;

            pilo::test::c_test_mutex_lock_only<recursive_timed_mutex>* thr_lock_only = new pilo::test::c_test_mutex_lock_only<recursive_timed_mutex>;
            thr_lock_only->m_mutex = &mutex;
            thr_lock_only->execute();

            basic_thread::sleep(5);

            for (int i = 0; i < 100; i++)
            {
                pilo::tick64_t start = PiloGetTickCount64();

                if (mutex.try_lock_for(150) != false)
                {
                    mutex.unlock();
                    return -1;
                }   
                pilo::tick64_t end = PiloGetTickCount64();
                
                pilo::tick64_t diff = end - start;

            
                if (diff > 200 || diff < 100)
                {            
                    mutex.unlock();
                    return -2;
                }

            }
            
            time_t now = time(0);
            mutex.try_lock_until(now + 3);
            time_t cur = time(0);
            time_t diff = cur - now;

            MP_CHECK_RANGE_OC_RETURN(diff, 0, 3 + 1, -3);

            thr_lock_only->m_stop = 1;

            thr_lock_only->wait();

            delete thr_lock_only;

            return 0;
        }
    }
}