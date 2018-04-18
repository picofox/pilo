#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_class_nonrecursive_mutex.hpp"
#include "core/threading/nonrecursive_mutex.hpp"
#include <string>

#include <mutex>

using namespace pilo::core::threading;
namespace pilo
{
    namespace test
    {


        static pilo::i32_t functional_test_lock_and_unlock(void* param);
        static pilo::i32_t functional_test_trylock(void* param);


        pilo::test::testing_case g_functional_cases_nonrecursive_mutex[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "lock and unlock                               ", nullptr, functional_test_lock_and_unlock, 0, -1, (pilo::u32_t) - 1 },
            { 2, "trylock                                       ", nullptr, functional_test_trylock, 0, -1, (pilo::u32_t) - 1 },

            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_test_lock_and_unlock(void* param)
        {
            M_UNUSED(param);

            const pilo::u32_t AddTimes = 20000000;
            const pilo::u32_t NumThreads = 4;

            pilo::uint_r value_to_add = 0;


            nonrecursive_mutex mutex;

            c_test_mutex_lock_and_unlock<nonrecursive_mutex> thr_test_lock[NumThreads];


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

            nonrecursive_mutex mutex;
            mutex.lock();

            for (int i = 0; i < 1000; i++)
            {
                basic_thread::msleep(10);
                if (mutex.try_lock() != false)
                {
                    mutex.unlock();
                    return -1;
                }
            }

            mutex.try_lock();

            mutex.unlock();
            return 0;
        }   
    }
}