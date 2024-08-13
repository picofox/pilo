#pragma once

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


namespace pilo
{
	namespace core
	{
		namespace datetime
		{           
			typedef enum
			{
				edf_YYYYdMMdDD = 0, ///"%04d-%02d-%02d"
				edf_YdMdD = 1, ///"%d-%d-%d",
				edf_YYYYsMMsDD = 2, ///"%04d/%02d/%02d"
				edf_YsMsD = 3,///"%0d/%0d/%0d",
				edf_YYYYpMMpDD = 4,///"%04d.%02d.%02d"
				edf_YpMpD = 5,///"%d.%d.%d",
				edf_YYYYwMMwDD = 6, ///"%04d %02d %02d",
				edf_YwMwD = 7, ///"%0d %0d %0d",
				edf_Count = 8,

			} date_format_enum;

			typedef enum
			{
				etf_HHcMMcSS = 0,
				etf_HHcMMcSSpMS = 1,
				etf_HHcMMcSSpMIL = 2,
				etf_Count = 3,

			} time_format_enum;

			typedef enum
			{
				edtp_Microsecond = 0,
				edtp_Millisecond = 1,
				edtp_Seconds = 2,
			} datetime_precision_enum;

            class datetime;

            static const char* __pilo_stc_local_date_fmt[] = {
                "%04d-%02d-%02d",
                "%d-%d-%d",
                "%04d/%02d/%02d",
                "%0d/%0d/%0d",
                "%04d.%02d.%02d",
                "%d.%d.%d",
                "%04d %02d %02d",
                "%0d %0d %0d",
            };

            static const char* __pilo_stc_local_date_fmt_with_space[] = {
                "%04d-%02d-%02d ",
                "%d-%d-%d ",
                "%04d/%02d/%02d ",
                "%0d/%0d/%0d ",
                "%04d.%02d.%02d ",
                "%d.%d.%d ",
                "%04d %02d %02d ",
                "%0d %0d %0d ",
            };

            struct local_date
            {
                local_date() : year(0), month(0), day(0), week_day(0), time_zone(0) {}
                ::pilo::i32_t    year;
                ::pilo::i8_t     month;
                ::pilo::i8_t     day;
                ::pilo::i8_t     week_day; 
                ::pilo::i8_t     time_zone;

                inline void reset(int timezone_mode = 0)
                {
                    year = 0;
                    month = 0;
                    day = 0;
                    week_day = 0;

                    if (timezone_mode > 0)
                    {
                        set_local_timezone();
                        
                    }
                    else if (timezone_mode < 0)
                    {
                        time_zone = 0;
                    }
                }

                inline void set_local_timezone()
                {
#ifdef      WINDOWS
                    pilo::i32_t t;
                    _get_timezone((long*)&t);
                    time_zone = (::pilo::i8_t)  (t / 3600);

#else
                    time_zone = (::pilo::i8_t)  (time_zone / 3600);
#endif
                }

                inline bool set(::pilo::i32_t y, ::pilo::i8_t m, ::pilo::i8_t d, ::pilo::i8_t tz)
                {
                    year = y;
                    month = m;
                    day = d;
                    time_zone = tz;
                    if (!valid())
                    {
                        return false;
                    }                    

                    return true;
                }

                inline bool set(::pilo::i32_t y, ::pilo::i8_t m, ::pilo::i8_t d)
                {
                    year = y;
                    month = m;
                    day = d;
                    if (!valid())
                    {
                        return false;
                    }

                    return true;
                }

                inline bool valid() const
                {
                    if (this->month < 1 || this->month > 12) return false;
                    if (this->day < 1 || this->day > 31) return false;
                    return true;
                }

                size_t to_string(char* szBuffer, size_t sz, ::pilo::core::datetime::date_format_enum fmt, bool append_space)
                {
                    if (append_space)
                    {
                        return ::pilo::core::io::string_formated_output(szBuffer, sz, __pilo_stc_local_date_fmt_with_space[fmt], year, month, day);
                    }
                    else
                    {
                        return ::pilo::core::io::string_formated_output(szBuffer, sz, __pilo_stc_local_date_fmt[fmt], year, month, day);
                    }                    
                }
            };
 
            struct local_time
            {
                ::pilo::u8_t      time_zone;
                ::pilo::i8_t      hour;
                ::pilo::i8_t      minute;
                ::pilo::i8_t      second;
                ::pilo::i32_t     microsecond;

                local_time() : time_zone(0), hour(0), minute(0), second(0), microsecond(0) {}

                inline void reset(int timezone_mode = 0)
                {
                    hour = 0;
                    minute = 0;
                    second = 0;
                    microsecond = 0;

                    if (timezone_mode > 0)
                    {
                        set_local_timezone();

                    }
                    else if (timezone_mode < 0)
                    {
                        time_zone = 0;
                    }
                }

                inline bool set(::pilo::i8_t h, ::pilo::i8_t m, ::pilo::i8_t s, ::pilo::i32_t ms)
                {
                    hour = h;
                    minute = m;
                    second = s;
                    microsecond = ms;

                    if (!valid())
                    {
                        return false;
                    }

                    return true;
                }

                inline bool set(::pilo::i8_t h, ::pilo::i8_t m, ::pilo::i8_t s, ::pilo::i32_t ms, ::pilo::i8_t tz)
                {
                    hour = h;
                    minute = m;
                    second = s;
                    microsecond = ms;
                    time_zone = tz;

                    if (!valid())
                    {
                        return false;
                    }

                    return true;
                }

                inline bool valid() const
                {
                    if (this->hour < 0 || this->hour > 23) return false;
                    if (this->minute < 0 || this->minute > 59) return false;
                    if (this->second < 0 || this->second > 59) return false;

                    if (this->microsecond != (pilo::i32_t)(-1))
                    {
                        if (this->microsecond < 0 || this->microsecond > 999999) return false;
                    }
                    return true;
                }

                inline void set_local_timezone()
                {
#ifdef      WINDOWS
                    pilo::i32_t t;
                    _get_timezone((long*)&t);
                    time_zone = (::pilo::i8_t)  (t / 3600);

#else
                    time_zone = (::pilo::i8_t)  (time_zone / 3600);
#endif
                }

                size_t to_string(char* szBuffer, size_t sz, time_format_enum mode)
                {
                    if (mode == etf_HHcMMcSS)
                    {
                        return ::pilo::core::io::string_formated_output(szBuffer, sz, "%02d:%02d:%02d", (int) hour, (int)minute, (int)second);
                    }
                    else if (mode == etf_HHcMMcSSpMS)
                    {
                        return ::pilo::core::io::string_formated_output(szBuffer, sz, "%02d:%02d:%02d", (int)hour, (int)minute, (int)second);                        
                    }
                    else if (mode == etf_HHcMMcSSpMIL)
                    {
                        return ::pilo::core::io::string_formated_output(szBuffer, sz, "%02d:%02d:%02d.%03d", (int)hour, (int)minute, (int)second, microsecond / 1000);
                    }
                    return 0;
                }
            };

			struct local_datetime
			{
                local_date      date;
                local_time      time;

                inline void reset(int timezone_mode = 0)
                {
                    date.reset(timezone_mode);
                    time.reset(timezone_mode);
                }

                inline bool set(::pilo::i32_t Y, ::pilo::i8_t M, ::pilo::i8_t D, ::pilo::i8_t h, ::pilo::i8_t m, ::pilo::i8_t s, ::pilo::i32_t ms)
                {
                    date.set(Y, M, D);
                    time.set(h, m, s, ms);
                    if (!valid())
                    {
                        return false;
                    }

                    return true;
                }

                inline bool set(::pilo::i32_t Y, ::pilo::i8_t M, ::pilo::i8_t D, ::pilo::i8_t h, ::pilo::i8_t m, ::pilo::i8_t s, ::pilo::i32_t ms, ::pilo::i8_t tz)
                {
                    date.set(Y, M, D,tz);
                    time.set(h, m, s, ms, tz);
                    if (!valid())
                    {
                        return false;
                    }

                    return true;
                }

                bool valid(::pilo::u32_t flags = 0) const
                {
                    if (! ::pilo::bit_flag<::pilo::u32_t>::test_value(flags, (::pilo::u8_t)PMB_PILO_DATETIME_IGNORE_DATE))
                    {
                        if (! date.valid()) return false;
                    }
                    if (!::pilo::bit_flag<::pilo::u32_t>::test_value(flags, (::pilo::u8_t)PMB_PILO_DATETIME_IGNORE_TIME))
                    {
                        if (!time.valid()) return false;
                    }

                    return true;
                }

                size_t to_string(char* szBuffer, size_t sz, date_format_enum date_mode, time_format_enum time_mode)
                {
                    size_t index = date.to_string(szBuffer, sz, date_mode, true);
                    index += time.to_string(szBuffer + index, sz - index, time_mode);
                    return index;
                }
                
                bool from_string(const char * datetimeStr, bool use_millisecond)
                {
                    char* ss[7];
                    char cTmp[128] = {0};

                    int len = (int)::pilo::core::string::character_count(datetimeStr);
                    if (len >= 128) len = 100;

                    memcpy(cTmp, datetimeStr, len);

                    size_t n = ::pilo::core::string::split_inplace<char>(cTmp, "+-:/\\ \t.", -1, ss, 7);
                    if (n < 3) return false;

					::pilo::i32_t tmp = 0;
                    ::pilo::core::string::string_to_number(this->date.year, ss[0]);
                    ::pilo::core::string::string_to_number(tmp, ss[1]);
					this->date.month = (::pilo::i8_t)  tmp;
                    ::pilo::core::string::string_to_number(tmp, ss[2]);
					this->date.day = (::pilo::i8_t)  tmp;

					if (n >= 4)
					{
						::pilo::core::string::string_to_number( tmp, ss[3]);
						this->time.hour = (::pilo::i8_t) tmp;
					}						
					if (n >= 5)
					{
						::pilo::core::string::string_to_number(tmp, ss[4]);
						this->time.minute = (::pilo::i8_t) tmp;
					}						
					if (n >= 6)
					{
						::pilo::core::string::string_to_number(tmp, ss[5]);
						this->time.second = (::pilo::i8_t) tmp;
					}						
                    if (n >= 7)
                    {
                        if (use_millisecond)
                        {
                             ::pilo::core::string::string_to_number(this->time.microsecond, ss[6]);
							 this->time.microsecond *= 1000;
                        }
                        else
                        {
							::pilo::core::string::string_to_number(this->time.microsecond, ss[6]);
                        }
                    }

                    if (!valid())
                    {
                        return false;
                    }                                        

                    return true;
                }
			};
			class datetime
			{
			public:
				enum   { day_seconds = 24 * 3600, hour_seconds = 3600, min_seconds = 60 };
                static pilo::i32_t diff_seconds_local_to_utc();
                static ::pilo::i8_t  days_in_months(int year, int month);
				static bool is_leap_year(int year);
                static ::pilo::core::datetime::local_datetime now();
                static ::pilo::i64_t epoch_time();

				static pilo::i64_t calculate_year_initial_second_local(::pilo::i32_t year, ::pilo::i32_t timezone = INT_MAX);
                static pilo::i64_t calculate_day_initial_second_local(pilo::i64_t sec);
                static pilo::i64_t calculate_week_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_month_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_next_hours_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_next_day_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_next_week_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_next_month_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_next_year_initial_second(pilo::i64_t sec);
                
			public:
				datetime();
                datetime(pilo::i64_t tick, datetime_precision_enum mode);
				datetime(const local_datetime& ldt);
				datetime(const datetime& other);
				datetime& operator=(const datetime& other);
				~datetime();

            public: //operators
                bool operator==(const ::pilo::core::datetime::datetime& other) const;
                bool operator!=(const ::pilo::core::datetime::datetime& other) const;
                bool operator>(const ::pilo::core::datetime::datetime& other) const;
                bool operator<(const ::pilo::core::datetime::datetime& other) const;
                bool operator>=(const ::pilo::core::datetime::datetime& other) const;
                bool operator<=(const ::pilo::core::datetime::datetime& other) const;

                pilo::i64_t seconds_since_epoch() const { return m_epoch / 1000000; }
                pilo::i64_t tick_since_epoch() const { return m_epoch; }
                pilo::i32_t to_days() const;
                pilo::i32_t to_local_days() const;
                pilo::i32_t week_day() const;
                pilo::i32_t month_day() const;
                datetime& add_days(int days);
                datetime& add_seconds(int seconds);
                datetime& update();                

                size_t to_string(char* szBuffer, size_t sz, date_format_enum date_mode, time_format_enum time_mode) const;
                bool from_string(const char * datetimeStr, bool use_milli);
                

            public:
				bool from_local_datetime(const local_datetime& ldt);
                bool to_local_datetime(local_datetime& ldt) const;  
                void set(pilo::i64_t tick, datetime_precision_enum mode);
                ::pilo::i64_t epoch_microseconds() const { return m_epoch;  }

			protected:
                ::pilo::i64_t   m_epoch;               
			};
		}
	}
}