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


        static pilo::i32_t functional_is_leap(void* param);
        static pilo::i32_t functional_days_in_month(void* param);
        static pilo::i32_t functional_calculate_year_initial_second(void* param);
        static pilo::i32_t functional_calculate_day_initial_second(void* param);

        pilo::test::testing_case g_functional_cases_datetime[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "is_leap                                       ", nullptr, functional_is_leap, 0, -1, (pilo::u32_t) - 1 },
            { 2, "days_in_month                                 ", nullptr, functional_days_in_month, 0, -1, (pilo::u32_t) - 1 },
            { 3, "static calculate year initial seconds         ", nullptr, functional_calculate_year_initial_second, 0, -1, (pilo::u32_t) - 1 },
            { 4, "static calculate day initial seconds          ", nullptr, functional_calculate_day_initial_second, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };
        

        pilo::i32_t functional_calculate_day_initial_second(void* param)
        {
            M_UNUSED(param);

           
            ::pilo::i64_t start = ::pilo::core::datetime::datetime::now_millisecs() / 1000;

            for (int i = (int) start; i < INT_MAX - 12345 -1; i += 12345)
            {
                ::pilo::i64_t v1 = ::pilo::core::datetime::datetime::calculate_day_initial_second(i);
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
            
            for (int i = 0; i < 300000; i++)
            {
                ::pilo::i64_t v1 = ::pilo::core::datetime::datetime::calculate_year_initial_second_fast(i);
                ::pilo::i64_t v2 = ::pilo::core::datetime::datetime::calculate_year_initial_second(i);

                if (v2 != v1)
                {
                    return -1;
                }
            }

            return 0;
        }
    }
}