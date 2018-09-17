#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "./functional_test_module_class_basic_thread.hpp"
#include "core/threading/nonrecursive_mutex.hpp"
#include <string>


namespace pilo
{
    namespace test
    {

        static pilo::i32_t functional_test_100_threads_accumulation_accuracy(void* param);
        pilo::core::threading::nonrecursive_mutex g_mutex;



        unsigned long long _g_value_for_accu_accu_test = 0;

        pilo::test::testing_case g_functional_cases_basic_thread[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "100 thread accumulation accuracy              ", nullptr, functional_test_100_threads_accumulation_accuracy, 0, -1, (pilo::u32_t) - 1 },

            


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_test_100_threads_accumulation_accuracy(void* param)
        {
            M_UNUSED(param);
            pilo::test::c_test_100threads_accumulation_accuracy    thread_array[100];

            for (size_t i = 0; i < 100; i++)
            {
                thread_array[i].execute(MC_INVALID_SIZE);
            }

            for (size_t i = 0; i < 100; i++)
            {
                thread_array[i].wait();
            }

            if (_g_value_for_accu_accu_test != 100 * 100000)
            {
                return -1;
            }

            return 0;
        }

        pilo::i32_t c_test_100threads_accumulation_accuracy::on_run()
        {
            for (size_t i = 0; i < 100000; i++)
            {
                g_mutex.lock();
                _g_value_for_accu_accu_test++;
                g_mutex.unlock();
            }
            return 0;
        }

    }
}