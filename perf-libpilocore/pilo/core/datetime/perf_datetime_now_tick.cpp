#include "../../pilo_perf_common.hpp"
#include "core/io/format_output.hpp"
#include "core/datetime/datetime.hpp"

#if (M_CXX_FEATURE_TIME)
#include <chrono>
#endif

#ifdef TEST_VS_BOOST
#include <boost/chrono/chrono.hpp>
#endif


namespace pilo
{
    namespace test
    {
        static const int TickTimes       = 1000000000;
        static const int SteadyTickTimes = 100000000;

        static void _do_perf_now_ticks()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < TickTimes; i++)
            {
                ::pilo::core::datetime::datetime::now_ticks();
            }            
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPilo  now_ticks():     %f MS PER, %f S Total\n",
                         (double )nDelta / (double) TickTimes, (double) nDelta/(double)1000000);            
        }
#if (M_CXX_FEATURE_TIME)
        static void _do_perf_now_chrono()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < TickTimes; i++)
            {
                ::pilo::core::datetime::datetime::now_chrono();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPilo  now_chrono():    %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)TickTimes, (double)nDelta / (double)1000000);
        }
#endif
        static void _do_perf_now_microsecs()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < TickTimes; i++)
            {
                ::pilo::core::datetime::datetime::now_microsecs();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPilo  now_microsecs(): %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)TickTimes, (double)nDelta / (double)1000000);
        }
        static void _do_perf_now_millisecs()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < TickTimes; i++)
            {
                ::pilo::core::datetime::datetime::now_millisecs();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPilo  now_millisecs(): %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)TickTimes, (double)nDelta / (double)1000000);
        }
#if (M_CXX_FEATURE_TIME)
        static void _do_perf_now_std()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < TickTimes; i++)
            {
                std::chrono::system_clock::now();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tSTD   now():           %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)TickTimes, (double)nDelta / (double)1000000);
        }
#endif
#ifdef TEST_VS_BOOST
        static void _do_perf_now_boost()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < TickTimes; i++)
            {
                boost::chrono::system_clock::now();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tBOOST now():           %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)TickTimes, (double)nDelta / (double)1000000);
        }
#endif

        static void _do_perf_tickcount32()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < TickTimes; i++)
            {
                PiloGetTickCount32();                
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPILO  TickCount32():   %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)TickTimes, (double)nDelta / (double)1000000);
        }

        static void _do_perf_tickcount64()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < TickTimes; i++)
            {
                PiloGetTickCount64();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPILO  TickCount64():   %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)TickTimes, (double)nDelta / (double)1000000);
        }


        ::pilo::error_number_t perf_now_ticks()
        {
            ::pilo::core::io::console_format_output("\n\nPerf [datetime] test system clock - now_xxx(): cnt=%uM\n", TickTimes/1000000);
            _do_perf_tickcount32();
            _do_perf_tickcount64();
            _do_perf_now_ticks();
#if (M_CXX_FEATURE_TIME)
            _do_perf_now_chrono();
#endif
            _do_perf_now_microsecs();
            _do_perf_now_millisecs();
#if (M_CXX_FEATURE_TIME)
            _do_perf_now_std();
#endif
#ifdef TEST_VS_BOOST
            _do_perf_now_boost();
#endif

            return ::pilo::EC_OK;
        }

    
        static void _do_perf_steady_now_ticks()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < SteadyTickTimes; i++)
            {
                ::pilo::core::datetime::datetime::steady_now_ticks();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPilo  now_ticks():     %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)SteadyTickTimes, (double)nDelta / (double)1000000);
        }
#if (M_CXX_FEATURE_TIME)
        static void _do_perf_steady_now_chrono()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < SteadyTickTimes; i++)
            {
                ::pilo::core::datetime::datetime::steady_now_chrono();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPilo  now_chrono():    %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)SteadyTickTimes, (double)nDelta / (double)1000000);
        }
#endif
        static void _do_perf_steady_now_microsecs()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < SteadyTickTimes; i++)
            {
                ::pilo::core::datetime::datetime::steady_now_microsecs();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPilo  now_microsecs(): %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)SteadyTickTimes, (double)nDelta / (double)1000000);
        }
        static void _do_perf_steady_now_millisecs()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < SteadyTickTimes; i++)
            {
                ::pilo::core::datetime::datetime::steady_now_millisecs();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tPilo  now_millisecs(): %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)SteadyTickTimes, (double)nDelta / (double)1000000);
        }
#if (M_CXX_FEATURE_TIME)
        static void _do_perf_steady_now_std()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < SteadyTickTimes; i++)
            {
                std::chrono::steady_clock::now();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tSTD   now():           %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)SteadyTickTimes, (double)nDelta / (double)1000000);
        }
#endif
#ifdef TEST_VS_BOOST
        static void _do_perf_steady_now_boost()
        {
            ::pilo::i64_t nStart = ::pilo::core::datetime::datetime::steady_now_microsecs();
            for (int i = 0; i < SteadyTickTimes; i++)
            {
                boost::chrono::steady_clock::now();
            }
            ::pilo::i64_t nDelta = ::pilo::core::datetime::datetime::steady_now_microsecs() - nStart;

            ::pilo::core::io::console_format_output("\tBOOST now():           %f MS PER, %f S Total\n",
                                                    (double)nDelta / (double)SteadyTickTimes, (double)nDelta / (double)1000000);
        }
#endif

        ::pilo::error_number_t perf_steady_now_ticks()
        {
            ::pilo::core::io::console_format_output("\n\nPerf [datetime] test steady clock - now_xxx(): cnt=%uM\n", SteadyTickTimes/1000000);

            _do_perf_steady_now_ticks();
#if (M_CXX_FEATURE_TIME)
            _do_perf_steady_now_chrono();
#endif
            _do_perf_steady_now_microsecs();
            _do_perf_steady_now_millisecs();
#if (M_CXX_FEATURE_TIME)
            _do_perf_steady_now_std();
#endif
#ifdef TEST_VS_BOOST
            _do_perf_steady_now_boost();
#endif

            return ::pilo::EC_OK;
        }

//         ::pilo::error_number_t perf_tickcount()
//         {
//             ::pilo::core::io::console_format_output("\n\nPerf [Tickcount] test Durable\n");
//             ::pilo::test::perf_TickCount ptc_thr[4];
// 
//             for (int i = 0; i < 4; i++)
//             {
//                 ptc_thr[i].execute();
//             }
// 
//             for (int i = 0; i < 4; i++)
//             {
//                 ptc_thr[i].wait();
//             }
//             
//             return ::pilo::EC_OK;
//         }
    }
}