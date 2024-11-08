#ifndef _pilo_core_datetime_datetime_h_
#define _pilo_core_datetime_datetime_h_

#include "../../pilo.hpp"
#include <time.h>
#include <chrono>
#include "../string/string_operation.hpp"
#include "../io/formatted_io.hpp"


#ifdef WINDOWS

#       define    PMI_WIN_EPOCH_DIFF_SECS    (11644473600LL)
#       define    PMI_WIN_EPOCH_DIFF_HUNA   (116444736000000000LL)

#else
#include <sys/time.h>

#endif

#define PMB_PILO_DATETIME_IGNORE_DATE (1)
#define PMB_PILO_DATETIME_IGNORE_TIME (2)

#define PMI_PILO_DATETIME_MAX_YEAR (291672)
#define PMI_DATETIME_YEAR_1ST_SECS_CACHE_SIZE            (256)
#define PMI_DATETIME_YEAR_1ST_SECS_CACHE_MAX_YEAR        (1970+PMI_DATETIME_YEAR_1ST_SECS_CACHE_SIZE-1)

#define PMI_DATETIME_YEAR_MAX   (8191)
#define PMI_INVALID_DATETIME    ((::pilo::u64_t) ~0)
#define PMI_DATETIME_YEAR_MASK  ((::pilo::u64_t) 0x1FFF )
#define PMI_DATETIME_MON_MASK  ((::pilo::u64_t) 0xF )
#define PMI_DATETIME_MDAY_MASK  ((::pilo::u64_t) 0x1F )
#define PMI_DATETIME_HOUR_MASK  ((::pilo::u64_t) 0x1F )
#define PMI_DATETIME_MIN_MASK  ((::pilo::u64_t) 0x3F )
#define PMI_DATETIME_SEC_MASK  ((::pilo::u64_t) 0x3F )
#define PMI_DATETIME_USEC_MASK  ((::pilo::u64_t) 0xFFFFF )
#define PMI_DATETIME_TZOFF_MASK  ((::pilo::u64_t) 0x1F )

#define PMI_USE_SYSTEM_TIMEZONE ((::pilo::i8_t) 127)
#define PMI_USE_OVERRIDED_TIMEZONE ((::pilo::i8_t) 126)


#define PMF_TZ_USEC_OFF(h)   (((::pilo::i64_t)h)*3600000000)
#define PMF_TZ_SEC_OFF(h)   (((::pilo::i64_t)h)*3600)
#define PMI_USECS_OF_DAY    (86400000000)
#define PMI_USECS_OF_HOUR   (3600000000)
#define PMI_USECS_OF_SEC    (1000000)
#define PMI_MSECS_OF_SEC    (1000)
#define PMI_SECS_OF_MINUTE  (60)
#define PMI_SECS_OF_HOUR    (3600)

namespace pilo
{
	namespace core
	{
		namespace datetime
		{
            typedef std::chrono::system_clock				sysclock;
            typedef std::chrono::system_clock::time_point	systime;
            //year - 13
            //mon - 4
            //mday - 5
            //hour - 5
            //min - 6
            //sec - 6
            //usec - 20
            //tz - 5

            //%tz - -1,+8
            // %2tz - +08, -01
            //%us - 6
            //%6us - 000006
            //%ms - 1
            //%3ms - 001
            // 
            // 
            //%Y - 2024
            //%YY - 24
            // %M - 1
            // %MM - 01
            // %D - 1
            // %DD - 01
            //%h - 9
            //%hh - 08
            //%s - 9
            // %ss - 09
            //%h - 9
            //%m - 8
            //%s - 9
            // 
            
            
            ::pilo::u8_t days_of_months(::pilo::u8_t mon, bool is_leap);

            ::pilo::u64_t make_datetime(::pilo::u16_t year, ::pilo::u8_t mon, ::pilo::u8_t mday, ::pilo::u8_t hour, ::pilo::u8_t min, ::pilo::u8_t sec, ::pilo::u32_t usec, ::pilo::i8_t tzoff);
            ::pilo::u16_t year_of_datetime(::pilo::u64_t dtv);
            ::pilo::u8_t month_of_datetime(::pilo::u64_t dtv);
            ::pilo::u8_t mday_of_datetime(::pilo::u64_t dtv);
            ::pilo::u8_t hour_of_datetime(::pilo::u64_t dtv);
            ::pilo::u8_t min_of_datetime(::pilo::u64_t dtv);
            ::pilo::u8_t sec_of_datetime(::pilo::u64_t dtv);
            ::pilo::u32_t microsec_of_datetime(::pilo::u64_t dtv);
            ::pilo::i8_t timezone_hoff_of_datetime(::pilo::u64_t dtv);
            bool set_year_of_datetime(::pilo::u64_t &dtv, ::pilo::u16_t year);
            bool set_month_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t mon);
            bool set_mday_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t day);
            bool set_hour_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t hour);
            bool set_min_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t min);
            bool set_sec_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t sec);
            bool set_usec_of_datetime(::pilo::u64_t& dtv, ::pilo::u32_t usec);
            bool set_timezone_hoff_of_datetime(::pilo::u64_t& dtv, ::pilo::i8_t tzhoff);
            const char* format_datetime_to_cstring(char* buffer, ::pilo::i64_t sz, const char* fmt, ::pilo::u64_t dtv);
            ::pilo::u64_t timestamp_to_datetime(::pilo::i64_t t, ::pilo::i8_t tz);
            ::pilo::u64_t timestamp_to_datetime(const ::pilo::core::datetime::systime & systm, ::pilo::i8_t tz);
            ::pilo::i64_t datetime_to_timestamp(::pilo::u64_t dtv, ::pilo::i8_t tz);
            ::pilo::i64_t datetime_to_timestamp(::pilo::u64_t dtv);
            std::time_t datetime_to_timestamp_by_second(std::tm& date, ::pilo::i8_t tz);
            bool is_leap_year(::pilo::u16_t year);
            ::pilo::i64_t first_second_of_year(::pilo::u16_t year, ::pilo::i8_t tzhoff);
            ::pilo::i64_t first_second_of_day(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff);
            ::pilo::i64_t first_second_of_week(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff);
            ::pilo::i64_t first_second_of_month(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff);
            ::pilo::i64_t first_second_of_hour(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff);
            ::pilo::i64_t first_microsecond_of_day(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff);
            ::pilo::i64_t first_microsecond_of_week(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff);
            ::pilo::i64_t first_microsecond_of_month(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff);
            ::pilo::i64_t first_microsecond_of_hour(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff);

            ::pilo::i64_t first_second_of_month(::pilo::u64_t dtv);
            ::pilo::i64_t first_microsecond_of_month(::pilo::u64_t dtv);

            ::pilo::i64_t first_second_of_next_days(::pilo::i64_t ts, ::pilo::i8_t tz, ::pilo::i64_t ndays);
            ::pilo::i64_t first_second_of_next_hours(::pilo::i64_t ts, ::pilo::i8_t tz, ::pilo::i64_t nhours);

            ::pilo::i64_t first_microsecond_of_next_days(::pilo::i64_t ts, ::pilo::i8_t tz, ::pilo::i64_t ndays);
            ::pilo::i64_t first_microsecond_of_next_hours(::pilo::i64_t ts, ::pilo::i8_t tz, ::pilo::i64_t nhours);


            ::pilo::i64_t ceil_by_second(::pilo::i64_t ts);
            void ceil_by_second(::pilo::i64_t * ts);

            ::pilo::i64_t microsecond_from_cstr(const char* cstr, ::pilo::i64_t len);

            ::pilo::u8_t week_day(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff);
            ::pilo::u8_t mdays_in_months(::pilo::u16_t year, ::pilo::u8_t mon);

		    __inline struct tm* xpf_get_localtime(struct tm * tmsp,  time_t*  secs) {
#ifdef WINDOWS
                errno_t err = localtime_s(tmsp, secs);
                return 0 == err ? tmsp : nullptr;
#else
		        return localtime_r(secs, tmsp);
#endif
		    }

            __inline struct tm*  xpf_get_gmtime(struct tm* tmsp, const time_t* secs)
            {
#ifdef WINDOWS
                errno_t err = gmtime_s(tmsp, secs);
                return 0 == err ? tmsp : nullptr;
#else
                return gmtime_r(secs, tmsp);
#endif
            }



            __inline ::pilo::i64_t xpf_timezone_offset_seconds()
            {
#ifdef WINDOWS
                long sec = 0;
                _get_timezone(&sec);
                return (::pilo::i64_t) sec;
#else
                return (::pilo::i64_t) timezone;
#endif                
            }
		}
	}
}


#endif