#include "core/threading/nonrecursive_mutex.hpp"
#include "../../pilo_perf_common.hpp"
#include "core/threading/basic_thread.hpp"
#include "core/io/format_output.hpp"
#include "core/datetime/datetime.hpp"

#if (M_CXX_FEATURE_MUTEX)
#   include <mutex>
#endif 
namespace pilo
{
    namespace test
    {
        static const ::pilo::u64_t ThreadCount = 4;
        static const ::pilo::u64_t CalPerThread1M =   1000000;
        static const ::pilo::u64_t CalPerThread10M =  10000000;
        static const ::pilo::u64_t CalPerThread100M = 100000000000;

        template<typename LOCK, typename THREAD_T>
        bool _do_perf_mutex(LOCK* mx, ::pilo::u64_t calc_per_thread, THREAD_T* workers, ::pilo::u64_t& ref_diff, double& ref_fper)
        {
            ::pilo::u64_t value = 0;
            
            for (int i = 0; i < ThreadCount; i++)
            {
                workers[i].reset(calc_per_thread, &value, mx);
            }

            ::pilo::i64_t u_start = ::pilo::core::datetime::datetime::steady_now_microsecs();

            for (int i = 0; i < ThreadCount; i++)
            {
                workers[i].execute(MC_INVALID_SIZE);
            }

            for (int i = 0; i < ThreadCount; i++)
            {
                workers[i].wait(MC_INVALID_SIZE);
            }
            
            ::pilo::i64_t u_finshed = ::pilo::core::datetime::datetime::steady_now_microsecs();
            ref_diff = u_finshed - u_start;
            ref_fper = ((double) ref_diff) / (value);
            
            if (calc_per_thread*ThreadCount != value)
            {
                return false;
            }

            return true;
        }

        ::pilo::error_number_t perf_nonrecursive_mutex()
        {
            ::pilo::core::io::console_format_output("\nPerf [nonrecursively_mutex] test lock():\n");

            ::pilo::core::threading::nonrecursive_mutex rmx;
            ::pilo::test::perf_test_mtx<::pilo::core::threading::nonrecursive_mutex> mtx[ThreadCount];

            ::pilo::u64_t diff_1M = 0;
            ::pilo::u64_t diff_10M = 0;
            ::pilo::u64_t diff_100M = 0;
            double fper_1M = 0.0f;
            double fper_10M = 0.0f;
            double fper_100M = 0.0f;
            bool b_ok = false;

            b_ok = _do_perf_mutex<::pilo::core::threading::nonrecursive_mutex>(&rmx, CalPerThread1M, mtx, diff_1M, fper_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 1M Test\n");
            }

            b_ok = _do_perf_mutex<::pilo::core::threading::nonrecursive_mutex>(&rmx, CalPerThread10M, mtx, diff_10M, fper_10M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 10M Test\n");
            }

            b_ok = _do_perf_mutex<::pilo::core::threading::nonrecursive_mutex>(&rmx, CalPerThread100M, mtx, diff_100M, fper_100M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 100M Test\n");
            } 
            ::pilo::core::io::console_format_output("\tPilo NR-Mutex  1M=(%llu:%f) 10M=(%llu:%f) 100M=(%llu:%f) \n",
                                                 diff_1M,fper_1M, diff_10M,fper_10M, diff_100M, fper_100M);
#if (M_CXX_FEATURE_MUTEX)
            std::mutex std_rmx;
            ::pilo::test::perf_test_mtx<std::mutex> std_mtx[ThreadCount];
            b_ok = _do_perf_mutex<std::mutex>(&std_rmx, CalPerThread1M, std_mtx, diff_1M, fper_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 1M Test\n");
            }
            b_ok = _do_perf_mutex<std::mutex>(&std_rmx, CalPerThread10M, std_mtx, diff_10M, fper_10M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 10M Test\n");
            }
            b_ok = _do_perf_mutex<std::mutex>(&std_rmx, CalPerThread100M, std_mtx, diff_100M, fper_100M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 100M Test\n");
            }

            ::pilo::core::io::console_format_output("\tSTD  NR-Mutex  1M=(%llu:%f) 10M=(%llu:%f) 100M=(%llu:%f) \n",
                                                      diff_1M, fper_1M, diff_10M, fper_10M, diff_100M, fper_100M);
#endif

            return ::pilo::EC_OK;
        }

        ::pilo::error_number_t perf_nonrecursive_mutex_try()
        {
            ::pilo::core::io::console_format_output("\nPerf [nonrecursively_mutex] test try_lock():\n");

            ::pilo::core::threading::nonrecursive_mutex rmx;
            ::pilo::test::perf_test_mtx_try<::pilo::core::threading::nonrecursive_mutex> mtx[ThreadCount];

            ::pilo::u64_t diff_1M = 0;
            ::pilo::u64_t diff_10M = 0;
            ::pilo::u64_t diff_100M = 0;
            double fper_1M = 0.0f;
            double fper_10M = 0.0f;
            double fper_100M = 0.0f;
            bool b_ok = false;

            b_ok = _do_perf_mutex<::pilo::core::threading::nonrecursive_mutex>(&rmx, CalPerThread1M, mtx, diff_1M, fper_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 1M Test\n");
            }

            b_ok = _do_perf_mutex<::pilo::core::threading::nonrecursive_mutex>(&rmx, CalPerThread10M, mtx, diff_10M, fper_10M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 10M Test\n");
            }

            b_ok = _do_perf_mutex<::pilo::core::threading::nonrecursive_mutex>(&rmx, CalPerThread100M, mtx, diff_100M, fper_100M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 100M Test\n");
            }
            ::pilo::core::io::console_format_output("\tPilo NR-Mutex  1M=(%llu:%f) 10M=(%llu:%f) 100M=(%llu:%f)\n",
                                                    diff_1M, fper_1M, diff_10M, fper_10M, diff_100M, fper_100M);
#if (M_CXX_FEATURE_MUTEX)
            std::mutex std_rmx;
            ::pilo::test::perf_test_mtx_try<std::mutex> std_mtx[ThreadCount];
            b_ok = _do_perf_mutex<std::mutex>(&std_rmx, CalPerThread1M, std_mtx, diff_1M, fper_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 1M Test\n");
            }
            b_ok = _do_perf_mutex<std::mutex>(&std_rmx, CalPerThread10M, std_mtx, diff_10M, fper_10M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 10M Test\n");
            }
            b_ok = _do_perf_mutex<std::mutex>(&std_rmx, CalPerThread100M, std_mtx, diff_100M, fper_100M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 100M Test\n");
            }

            ::pilo::core::io::console_format_output("\tSTD  NR-Mutex  1M=(%llu:%f) 10M=(%llu:%f) 100M=(%llu:%f)\n",
                                                    diff_1M, fper_1M, diff_10M, fper_10M, diff_100M, fper_100M);
#endif

            return ::pilo::EC_OK;
        }
    }
}

