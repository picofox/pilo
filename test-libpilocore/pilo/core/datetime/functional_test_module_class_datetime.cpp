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
        static pilo::i32_t functional_static_now(void* param);
        static pilo::i32_t functional_static_calculate_initials(void* param);
        static pilo::i32_t functional_format(void* param);

        

        

        pilo::i32_t functional_format(void* param)
        {
            M_UNUSED(param);
            ::pilo::core::datetime::datetime dt;
            bool bret = dt.from_string("2018-07-05 17:15:00");
            if (!bret)
            {
                return -1;
            }

            ::pilo::core::datetime::local_datetime ldt;
            bret = ldt.from_string("2018-07-05 17:15:00.876");
            if (!bret)
            {
                return -10;
            }

            pilo::core::string::fixed_astring<48> str = ldt.to_string();

            if (str != pilo::core::string::fixed_astring<48>("2018-07-05 17:15:00.876"))
            {
                return -20;
            }

            ::pilo::core::datetime::datetime dt2;
            bret = dt2.from_local_datetime(ldt);
            if (!bret)
            {
                return -30;
            }

            ::pilo::core::datetime::local_datetime ldt2;
            bret = dt2.to_local_datetime(ldt2);
            if (!bret)
            {
                return -50;
            }

            if (dt2 != dt)
            {
               return -40;
            }            

            return 0;
        }

        pilo::test::testing_case g_functional_cases_datetime[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "static now                                    ", nullptr, functional_static_now, 0, -1, (pilo::u32_t) - 1 },
            { 2, "static calculate initial seconds              ", nullptr, functional_static_calculate_initials, 0, -1, (pilo::u32_t) - 1 },
            { 3, "static format string                          ", nullptr, functional_format, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };

        pilo::i32_t functional_static_now(void* param)
        {
            M_UNUSED(param);
            ::pilo::core::datetime::local_datetime ldt = ::pilo::core::datetime::datetime::now();

            if (!ldt.is_valid())
            {
                return -1;
            }

            return 0;
        }

        pilo::i32_t functional_static_calculate_initials(void* param)
        {
            M_UNUSED(param);
            ::pilo::i64_t ept2000 = ::pilo::core::datetime::datetime::calculate_year_initial_second(2000);
            ::pilo::i64_t ept2009 = ::pilo::core::datetime::datetime::calculate_year_initial_second(2009);

            ::pilo::i64_t diff = ept2009 - ept2000;

            if (diff != 86400 * 365 * 9 + 3 * 86400)
            {
                return -1;
            }

            ept2000 = ::pilo::core::datetime::datetime::calculate_year_initial_second_fast(2000);
            ept2009 = ::pilo::core::datetime::datetime::calculate_year_initial_second_fast(2009);
            diff = ept2009 - ept2000;
            if (diff != 86400 * 365 * 9 + 3 * 86400)
            {
                return -10;
            }

            ::pilo::i64_t ept2010 = ::pilo::core::datetime::datetime::calculate_year_initial_second_fast(2010);
            ::pilo::i64_t ept2019 = ::pilo::core::datetime::datetime::calculate_year_initial_second_fast(2019);
            diff = ept2019 - ept2010;
            if (diff != 86400 * 365 * 9 + 2 * 86400)
            {
                return -20;
            }

            return 0;
        }
    }
}