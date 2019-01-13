#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_datetime.hpp"
#include "core/io/fixed_memory_istream.hpp"
#include "core/io/fixed_memory_ostream.hpp"
#include "core/io/memory_ostream_adapter.hpp"
#include "core/io/memory_istream_adapter.hpp"
#include "core/datetime/datetime.hpp"
#include <set>
#include <algorithm>

namespace pilo
{
    namespace test
    {
        static bool  stc_test_leap_year(int n)
        {

            if (((n % 100 != 0) && (n % 4 == 0)) || (n % 400 == 0))
            {
                return true;
            }
            else
            {
                return false;
            }
        }

        static ::pilo::i64_t stc_test_day_1st_sec(::pilo::i64_t secs)
        {
            struct tm tmv;
#ifdef WINDOWS
            localtime_s(&tmv, (time_t*) &secs);
#else
            localtime_r(&tmv, (time_t*) &secs);
#endif
            tmv.tm_hour = 0;
            tmv.tm_min = 0;
            tmv.tm_sec = 0;

            return mktime(&tmv);
        }

        static ::pilo::i64_t stc_test_1st_secs_year(int year)
        {
            if (year < 3000)
            {
                if (year == 1970)
                {
                    return -28800;
                }
                struct tm stm = { 0, 0, 0, 1, 0, year - 1900, 0, 0, 0 };
                pilo::i64_t v = mktime(&stm);
                if (v < 0)
                {
                    return INT64_MIN;
                }
                return v;
            }
            else
            {
                if (year == 3000)
                {
                    return 32503651200LL;
                }
                else if (year == 3001)
                {
                    return 32535187200LL;
                }
                else if (year == 3002)
                {
                    return 32566723200LL;
                }
                else if (year == 3003)
                {
                    return 32598259200LL;
                }
                else if (year == 3004)
                {
                    return 32629795200LL;
                }
                else if (year == 3005)
                {
                    return 32661417600LL;
                }
                else if (year == 3006)
                {
                    return 32692953600LL;
                }
                else if (year == 3007)
                {
                    return 32724489600LL;
                }
                return INT64_MIN;
            }

        }


        static struct tm stc_test_to_local(::pilo::i64_t ms, int& rms)
        {
            struct tm rtm = { 0 };
            ::pilo::i64_t s = ms / 1000000;
            rms = ms % 1000000;

#ifdef WINDOWS
            localtime_s(&rtm, (time_t*)&s);
#else
            localtime_r(&rtm, (time_t*)&s);
#endif
            return rtm;
        }


        
        static pilo::i32_t functional_is_leap(void* param);
        static pilo::i32_t functional_days_in_month(void* param);
        static pilo::i32_t functional_to_local_date(void * param);
        static pilo::i32_t functional_calculate_year_initial_second(void* param);
        static pilo::i32_t functional_calculate_day_initial_second(void* param);

        pilo::test::testing_case g_functional_cases_datetime[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "is_leap                                       ", nullptr, functional_is_leap, 0, -1, (pilo::u32_t) - 1 },
            { 2, "days_in_month                                 ", nullptr, functional_days_in_month, 0, -1, (pilo::u32_t) - 1 },
            { 3, "static calculate year initial seconds         ", nullptr, functional_calculate_year_initial_second, 0, -1, (pilo::u32_t) - 1 },
            { 4, "to_local_date                                 ", nullptr, functional_to_local_date, 0, -1, (pilo::u32_t) - 1 },            
            { 5, "static calculate day initial seconds          ", nullptr, functional_calculate_day_initial_second, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };
        

        pilo::i32_t functional_to_local_date(void * param)
        {
            M_UNUSED(param);

            ::pilo::core::datetime::datetime dt(0, ePDTM_Seconds);
            ::pilo::core::datetime::local_datetime ldt;

            dt.to_local_datetime(ldt);
            if (ldt.time.hour != 8)
            {
                return -1;
            }

            int msleft = 0;
            for (::pilo::i64_t i = 28800; i < 32503680000; i += 11237)
            {
                pilo::i64_t tick = (pilo::i64_t)i * 1000000;
                dt.set(tick, ePDTM_Microsecond);

                struct tm tmv = stc_test_to_local(dt.epoch_microseconds(), msleft);
                dt.to_local_datetime(ldt);

                if (tmv.tm_year + 1900 != ldt.date.year)
                {
                    return -10;
                }
                if (tmv.tm_mon + 1 != ldt.date.month)
                {
                    return -20;
                }
                if (tmv.tm_mday  != ldt.date.day)
                {
                    return -30;
                }
                if (tmv.tm_hour != ldt.time.hour)
                {
                    return -40;
                }
                if (tmv.tm_min != ldt.time.minute)
                {
                    return -50;
                }
                if (tmv.tm_sec != ldt.time.second)
                {
                    return -60;
                }
                if (msleft != ldt.time.microsecond)
                {
                    return -70;
                }
            }




            return 0;
        }


        pilo::i32_t functional_calculate_day_initial_second(void* param)
        {
            M_UNUSED(param);

            ::pilo::i64_t v = ::pilo::core::datetime::datetime::calculate_day_initial_second_local(86400);
            if (v == -1)
            {
                return  -1;
            }
           
            ::pilo::i64_t start = ::pilo::core::datetime::datetime::now_millisecs() / 1000;

            for (int i = (int) start; i < INT_MAX - 12345 -1; i += 12345)
            {
                ::pilo::i64_t v1 = ::pilo::core::datetime::datetime::calculate_day_initial_second_local(i);
                ::pilo::i64_t v2 = stc_test_day_1st_sec(i);

                if (v1 != v2)
                {
                    return -1;
                }
            }

            return 0;
        }

        

        pilo::i32_t functional_is_leap(void* param)
        {
            M_UNUSED(param);

            for (int y = 0; y < 100000; y++)
            {
                bool bl1 = stc_test_leap_year(y);
                bool bl2 = ::pilo::core::datetime::datetime::is_leap_year(y);
                if (bl1 != bl2)
                {
                    return -1;
                }
            }
            return 0;
        }

        pilo::i32_t functional_days_in_month(void* param)
        {
            M_UNUSED(param);

            for (int y = 1970; y < 13000; y++)
            {
                bool bl = stc_test_leap_year(y);
                for (int i = 1; i < 13; i++)
                {
                    ::pilo::i8_t v = ::pilo::core::datetime::datetime::days_in_months(y, i);
                    int cv = 0;
                    if (i == 1 || i == 3 || i == 5 || i == 7 || i == 8 || i == 10 || i == 12)
                    {
                        cv = 31;
                    }
                    else if (i == 4 || i == 6 || i == 9 || i == 11)
                    {
                        cv = 30;
                    }
                    else
                    {
                        if (bl)
                        {
                            cv = 29;
                        }
                        else
                        {
                            cv = 28;
                        }
                    }

                    if (cv != v)
                    {
                        return -1;
                    }
                }
            }

            return 0;
        }

        

        pilo::i32_t functional_calculate_year_initial_second(void* param)
        {
            M_UNUSED(param);
            
            for (int i = 1970; i < 3007; i++)
            {
                ::pilo::i64_t v1 = stc_test_1st_secs_year(i);
                ::pilo::i64_t v2 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(i);

                if (v2 != v1)
                {
                    return -1;
                }
            }

            ::pilo::i64_t t1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(1969);
            if (t1 != -31564800LL)
            {
                return -2;
            }
            t1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(1900);
            if (t1 != -2209017600LL)
            {
                return -3;
            }
            t1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(1234);
            if (t1 != -23225904000LL)
            {
                return -4;
            }
            t1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(777);
            if (t1 != -37647446400LL)
            {
                return -5;
            }
            t1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(600);
            if (t1 != -43233033600LL)
            {
                return -6;
            }
            t1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(500);
            if (t1 != -46388707200LL)
            {
                return -7;
            }
            t1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(450);
            if (t1 != -47966544000LL)
            {
                return -8;
            }
            t1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(401);
            if (t1 != -49512844800LL)
            {
                return -9;
            }
            t1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(1600);
            if (t1 != -11676124800LL)
            {
                return -9;
            }

            return 0;
        }
    }
}