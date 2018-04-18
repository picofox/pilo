#include "core/threading/nonrecursive_timed_mutex.hpp"
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
        static const int ThreadCount = 4;
        static const int CalPerThread1M = 1000000;
        static const int CalPerThread10M = 10000000;
        static const int CalPerThread100M = 100000000;

        template<typename LOCK, typename THREAD_T>
        bool _do_perf_nonrecursive_timed_mutex(LOCK* mx, int calc_per_thread, THREAD_T* workers, ::pilo::u64_t& ref_diff, double& ref_fper, ::pilo::u64_t& failed_count)
        {
            ::pilo::u64_t value = 0;
            failed_count = 0;

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
               // printf("Main Wait %d - %u\n", i, workers[i].thread_id());
                workers[i].wait(MC_INVALID_SIZE);
            }

            ::pilo::i64_t u_finshed = ::pilo::core::datetime::datetime::steady_now_microsecs();
            ref_diff = u_finshed - u_start;
            ref_fper = ((double)ref_diff) / (value);

            if (calc_per_thread*ThreadCount != value)
            {
                return false;
            }

            failed_count = 0;
            for (int i = 0; i < ThreadCount; i++)
            {
                failed_count += workers[i].m_failed_times;
            }

            return true;
        }

        
        ::pilo::error_number_t perf_nonrecursively_timed_mutex_try_lock_until()
        {
            ::pilo::core::io::console_format_output("\nPerf [nonrecursive_timed_mutex] test: try_lock_until()\n");

            ::pilo::core::threading::nonrecursive_timed_mutex rmx;           

            ::pilo::u64_t diff_1M = 0;
            ::pilo::u64_t diff_10M = 0;
            ::pilo::u64_t diff_100M = 0;
            ::pilo::u64_t failed_count_1M = 0;
            ::pilo::u64_t failed_count_10M = 0;
            double fper_1M = 0.0f;
            double fper_10M = 0.0f;
            double fper_100M = 0.0f;
            bool b_ok = false;

#if (M_CXX_FEATURE_TIME)
            ::pilo::test::perf_test_time_mtx_until_chrono<::pilo::core::threading::nonrecursive_timed_mutex> mtx[ThreadCount];
            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread1M, mtx, diff_1M, fper_1M, failed_count_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 1M Test\n");
            }

            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread10M, mtx, diff_10M, fper_10M, failed_count_10M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 10M Test\n");
            }

            //              b_ok = _do_perf_recursive_timed_mutex_timed<::pilo::core::threading::recursive_timed_mutex>(&rmx, CalPerThread100M, mtx, diff_100M, fper_100M);
            //              if (!b_ok)
            //              {
            //                  ::pilo::core::io::console_format_output("->Failed! PILO 100M Test\n");
            //              }
            ::pilo::core::io::console_format_output("\tPilo NRT-Mutex  1M=(%llu:%f f=%llu) 10M=(%llu:%f f=%llu) 100M=(%llu:%f)  (Chrono)\n",
                                                    diff_1M, fper_1M, failed_count_1M, diff_10M, fper_10M, failed_count_1M, diff_100M, fper_100M);
#endif

            ::pilo::test::perf_test_time_mtx_until_secs<::pilo::core::threading::nonrecursive_timed_mutex> mtx_milli[ThreadCount];
            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread1M, mtx_milli, diff_1M, fper_1M, failed_count_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 1M Test\n");
            }

            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread10M, mtx_milli, diff_10M, fper_10M, failed_count_10M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 10M Test\n");
            }
            ::pilo::core::io::console_format_output("\tPilo NRT-Mutex  1M=(%llu:%f f=%llu) 10M=(%llu:%f f=%llu) 100M=(%llu:%f)  (Secs)\n",
                                                    diff_1M, fper_1M, failed_count_1M, diff_10M, fper_10M, failed_count_1M, diff_100M, fper_100M);


            //              b_ok = _do_perf_recursive_timed_mutex_timed<::pilo::core::threading::recursive_timed_mutex>(&rmx, CalPerThread100M, mtx, diff_100M, fper_100M);
            //              if (!b_ok)
            //              {
            //                  ::pilo::core::io::console_format_output("->Failed! PILO 100M Test\n");
            //              }


#if (M_CXX_FEATURE_MUTEX)
            std::timed_mutex std_rmx;
            ::pilo::test::perf_test_time_mtx_until_chrono<std::timed_mutex> std_mtx[ThreadCount];
            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread1M, std_mtx, diff_1M, fper_1M, failed_count_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 1M Test\n");
            }

            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread10M, std_mtx, diff_10M, fper_10M, failed_count_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 10M Test\n");
            }
            //             b_ok = _do_perf_recursive_timed_mutex_timed<std::recursive_timed_mutex>(&std_rmx, CalPerThread100M, std_mtx, diff_100M, fper_100M);
            //             if (!b_ok)
            //             {
            //                 ::pilo::core::io::console_format_output("->Failed! STD 100M Test\n");
            //             }

            ::pilo::core::io::console_format_output("\tStd  NRT-Mutex  1M=(%llu:%f f=%llu) 10M=(%llu:%f f=%llu) 100M=(%llu:%f)  (Chrono)\n",
                                                    diff_1M, fper_1M, failed_count_1M, diff_10M, fper_10M, failed_count_1M, diff_100M, fper_100M);

#endif
            return ::pilo::EC_OK;
        }

        ::pilo::error_number_t perf_nonrecursively_timed_mutex_try_lock_for()
        {
            ::pilo::core::io::console_format_output("\nPerf [nonrecursive_timed_mutex] test: try_lock_for()\n");

            ::pilo::core::threading::nonrecursive_timed_mutex rmx;           

            ::pilo::u64_t diff_1M = 0;
            ::pilo::u64_t diff_10M = 0;
            ::pilo::u64_t diff_100M = 0;
            ::pilo::u64_t failed_count_1M = 0;
            ::pilo::u64_t failed_count_10M = 0;
            double fper_1M = 0.0f;
            double fper_10M = 0.0f;
            double fper_100M = 0.0f;
            bool b_ok = false;
#if (M_CXX_FEATURE_TIME)
            ::pilo::test::perf_test_time_mtx_for_chrono<::pilo::core::threading::nonrecursive_timed_mutex> mtx[ThreadCount];
            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread1M, mtx, diff_1M, fper_1M, failed_count_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 1M Test\n");
            }
 
            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread10M, mtx, diff_10M, fper_10M, failed_count_10M);
             if (!b_ok)
             {
                 ::pilo::core::io::console_format_output("->Failed! PILO 10M Test\n");
             }
 
//              b_ok = _do_perf_recursive_timed_mutex_timed<::pilo::core::threading::recursive_timed_mutex>(&rmx, CalPerThread100M, mtx, diff_100M, fper_100M);
//              if (!b_ok)
//              {
//                  ::pilo::core::io::console_format_output("->Failed! PILO 100M Test\n");
//              }
             ::pilo::core::io::console_format_output("\tPilo NRT-Mutex  1M=(%llu:%f f=%llu) 10M=(%llu:%f f=%llu) 100M=(%llu:%f)  (Chrono)\n",
                                                     diff_1M, fper_1M, failed_count_1M, diff_10M, fper_10M, failed_count_1M, diff_100M, fper_100M);
#endif

             ::pilo::test::perf_test_time_mtx_for_milli<::pilo::core::threading::nonrecursive_timed_mutex> mtx_milli[ThreadCount];
             b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread1M, mtx_milli, diff_1M, fper_1M, failed_count_1M);
             if (!b_ok)
             {
                 ::pilo::core::io::console_format_output("->Failed! PILO 1M Test\n");
             }

             b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread10M, mtx_milli, diff_10M, fper_10M, failed_count_10M);
             if (!b_ok)
             {
                 ::pilo::core::io::console_format_output("->Failed! PILO 10M Test\n");
             }
             ::pilo::core::io::console_format_output("\tPilo NRT-Mutex  1M=(%llu:%f f=%llu) 10M=(%llu:%f f=%llu) 100M=(%llu:%f)  (Millisecs)\n",
                                                     diff_1M, fper_1M, failed_count_1M, diff_10M, fper_10M, failed_count_1M, diff_100M, fper_100M);


             //              b_ok = _do_perf_recursive_timed_mutex_timed<::pilo::core::threading::recursive_timed_mutex>(&rmx, CalPerThread100M, mtx, diff_100M, fper_100M);
             //              if (!b_ok)
             //              {
             //                  ::pilo::core::io::console_format_output("->Failed! PILO 100M Test\n");
             //              }

#if (M_CXX_FEATURE_MUTEX)
            std::timed_mutex std_rmx;
            ::pilo::test::perf_test_time_mtx_for_chrono<std::timed_mutex> std_mtx[ThreadCount];
            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread1M, std_mtx, diff_1M, fper_1M, failed_count_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 1M Test\n");
            }

            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread10M, std_mtx, diff_10M, fper_10M, failed_count_1M);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 10M Test\n");
            }
//             b_ok = _do_perf_recursive_timed_mutex_timed<std::recursive_timed_mutex>(&std_rmx, CalPerThread100M, std_mtx, diff_100M, fper_100M);
//             if (!b_ok)
//             {
//                 ::pilo::core::io::console_format_output("->Failed! STD 100M Test\n");
//             }

            ::pilo::core::io::console_format_output("\tStd  NRT-Mutex  1M=(%llu:%f f=%llu) 10M=(%llu:%f f=%llu) 100M=(%llu:%f)  (Chrono)\n",
                                                    diff_1M, fper_1M, failed_count_1M, diff_10M, fper_10M, failed_count_1M, diff_100M, fper_100M);

#endif
            return ::pilo::EC_OK;
        }

  

        ::pilo::error_number_t perf_nonrecursively_timed_mutex()
        {
            ::pilo::core::io::console_format_output("\nPerf [nonrecursive_timed_mutex] test lock():\n");

            ::pilo::core::threading::nonrecursive_timed_mutex rmx;
            ::pilo::test::perf_test_mtx<::pilo::core::threading::nonrecursive_timed_mutex> mtx[ThreadCount];

            ::pilo::u64_t diff_1M = 0;
            ::pilo::u64_t diff_10M = 0;
            ::pilo::u64_t diff_100M = 0;
            double fper_1M = 0.0f;
            double fper_10M = 0.0f;
            double fper_100M = 0.0f;
            bool b_ok = false;

            ::pilo::u64_t dummy_failed_count = 0;

            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread1M, mtx, diff_1M, fper_1M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 1M Test\n");
            }

            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread10M, mtx, diff_10M, fper_10M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 10M Test\n");
            }

            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread100M, mtx, diff_100M, fper_100M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 100M Test\n");
            }
            ::pilo::core::io::console_format_output("\tPilo NRT-Mutex  1M=(%llu:%f) 10M=(%llu:%f) 100M=(%llu:%f) \n",
                                                    diff_1M, fper_1M, diff_10M, fper_10M, diff_100M, fper_100M);
#if (M_CXX_FEATURE_MUTEX)
            std::timed_mutex std_rmx;
            ::pilo::test::perf_test_mtx<std::timed_mutex> std_mtx[ThreadCount];
            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread1M, std_mtx, diff_1M, fper_1M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 1M Test\n");
            }
            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread10M, std_mtx, diff_10M, fper_10M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 10M Test\n");
            }
            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread100M, std_mtx, diff_100M, fper_100M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 100M Test\n");
            }

            ::pilo::core::io::console_format_output("\tSTD  NRT-Mutex  1M=(%llu:%f) 10M=(%llu:%f) 100M=(%llu:%f) \n",
                                                    diff_1M, fper_1M, diff_10M, fper_10M, diff_100M, fper_100M);
#endif
            return ::pilo::EC_OK;
        }


        ::pilo::error_number_t perf_nonrecursively_timed_mutex_try()
        {
            ::pilo::core::io::console_format_output("\nPerf [nonrecursive_timed_mutex] test try_lock():\n");

            ::pilo::core::threading::nonrecursive_timed_mutex rmx;
            ::pilo::test::perf_test_mtx_try<::pilo::core::threading::nonrecursive_timed_mutex> mtx[ThreadCount];

            ::pilo::u64_t diff_1M = 0;
            ::pilo::u64_t diff_10M = 0;
            ::pilo::u64_t diff_100M = 0;
            double fper_1M = 0.0f;
            double fper_10M = 0.0f;
            double fper_100M = 0.0f;
            bool b_ok = false;

            ::pilo::u64_t dummy_failed_count = 0;

            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread1M, mtx, diff_1M, fper_1M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 1M Test\n");
            }

            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread10M, mtx, diff_10M, fper_10M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 10M Test\n");
            }

            b_ok = _do_perf_nonrecursive_timed_mutex<::pilo::core::threading::nonrecursive_timed_mutex>(&rmx, CalPerThread100M, mtx, diff_100M, fper_100M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! PILO 100M Test\n");
            }
            ::pilo::core::io::console_format_output("\tPilo NRT-Mutex  1M=(%llu:%f) 10M=(%llu:%f) 100M=(%llu:%f)\n",
                                                    diff_1M, fper_1M, diff_10M, fper_10M, diff_100M, fper_100M);
#if (M_CXX_FEATURE_MUTEX)
            std::timed_mutex std_rmx;
            ::pilo::test::perf_test_mtx_try<std::timed_mutex> std_mtx[ThreadCount];
            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread1M, std_mtx, diff_1M, fper_1M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 1M Test\n");
            }
            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread10M, std_mtx, diff_10M, fper_10M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 10M Test\n");
            }
            b_ok = _do_perf_nonrecursive_timed_mutex<std::timed_mutex>(&std_rmx, CalPerThread100M, std_mtx, diff_100M, fper_100M, dummy_failed_count);
            if (!b_ok)
            {
                ::pilo::core::io::console_format_output("->Failed! STD 100M Test\n");
            }

            ::pilo::core::io::console_format_output("\tSTD  NRT-Mutex  1M=(%llu:%f) 10M=(%llu:%f) 100M=(%llu:%f)\n",
                                                    diff_1M, fper_1M, diff_10M, fper_10M, diff_100M, fper_100M);

#endif
            return ::pilo::EC_OK;
        }
    }
}

