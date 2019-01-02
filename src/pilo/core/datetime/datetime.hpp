#pragma once

#include "../../core/coredefs.hpp"
#include <time.h>
#include "core/io/format_output.hpp"
#include "core/string/fixed_astring.hpp"

#if (M_CXX_FEATURE_TIME)
#   include <chrono>
#endif



#ifdef WINDOWS

#       define    PiloGetTickCount32()   (pilo::u32_t) GetTickCount() 
#       define    PiloGetTickCount64()   (pilo::u64_t) GetTickCount64() 
#       define    MC_WIN_EPOCH_DIFF_SECS    (11644473600LL)
#       define    MC_WIN_EPOCH_DIFF_HUNA   (116444736000000000LL)

#else
#include <sys/time.h>

#include "tickcount_simu_thread.hpp" 
#endif

#define MB_PILO_DATETIME_IGNORE_DATE (1)
#define MB_PILO_DATETIME_IGNORE_TIME (2)

namespace pilo
{
	namespace core
	{
		namespace datetime
		{           
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
                local_date() : year(0), month(0), day(0), flags(0) {}
                ::pilo::i32_t    year;
                ::pilo::i8_t     month;
                ::pilo::i8_t     day;
                ::pilo::i16_t    flags; //unused

                inline void reset()
                {
                    year = 0;
                    month = 0;
                    day = 0;
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

                size_t to_string(char* szBuffer, size_t sz, PiloDateFormatEnumeration fmt, bool append_space)
                {
                    if (append_space)
                    {
                        return ::pilo::core::io::string_format_output(szBuffer, sz, __pilo_stc_local_date_fmt_with_space[fmt], year, month, day);
                    }
                    else
                    {
                        return ::pilo::core::io::string_format_output(szBuffer, sz, __pilo_stc_local_date_fmt[fmt], year, month, day);
                    }                    
                }
            };
 
            struct local_time
            {
                ::pilo::u8_t      flags;
                ::pilo::i8_t      hour;
                ::pilo::i8_t      minute;
                ::pilo::i8_t      second;
                ::pilo::i32_t     microsecond;

                local_time() : flags(0), hour(0), minute(0), second(0), microsecond(0) {}

                inline void reset()
                {
                    hour = 0;
                    minute = 0;
                    second = 0;
                    microsecond = 0;
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

                size_t to_string(char* szBuffer, size_t sz, PiloTimeFormatEnumeration mode)
                {
                    if (mode == ePTF_HHcMMcSS)
                    {
                        return ::pilo::core::io::string_format_output(szBuffer, sz, "%02d:%02d:%02d", (int) hour, (int)minute, (int)second);
                    }
                    else if (mode == ePTF_HHcMMcSSpMS)
                    {
                        return ::pilo::core::io::string_format_output(szBuffer, sz, "%02d:%02d:%02d", (int)hour, (int)minute, (int)second);                        
                    }
                    else if (mode == ePTF_HHcMMcSSpMIL)
                    {
                        return ::pilo::core::io::string_format_output(szBuffer, sz, "%02d:%02d:%02d.%03d", (int)hour, (int)minute, (int)second, microsecond / 1000);
                    }
                    return 0;
                }
            };

			struct local_datetime
			{
                local_date      date;
                local_time      time;

                inline void reset()
                {
                    date.reset();
                    time.reset();
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

                bool valid() const
                {
                    if (!pilo_test_flag_bit_by_value<::pilo::u8_t>(time.flags, (::pilo::u8_t)MB_PILO_DATETIME_IGNORE_DATE))
                    {
                        if (! date.valid()) return false;
                    }

                    if (!pilo_test_flag_bit_by_value<::pilo::u8_t>(time.flags, (::pilo::u8_t)MB_PILO_DATETIME_IGNORE_TIME))
                    {
                        if (!time.valid()) return false;
                    }

                    return true;
                }

                size_t to_string(char* szBuffer, size_t sz, PiloDateFormatEnumeration date_mode, PiloTimeFormatEnumeration time_mode)
                {
                    size_t index = date.to_string(szBuffer, sz, date_mode, true);
                    index += time.to_string(szBuffer + index, sz - index, time_mode);
                    return index;
                }
                
                bool from_string(const char * datetimeStr, bool use_millisecond)
                {
                    char* ss[7];
                    char cTmp[128] = {0};

                    int len = (int)::pilo::core::string::string_util::length(datetimeStr);
                    if (len >= 128) len = 100;

                    memcpy(cTmp, datetimeStr, len);

                    size_t n = ::pilo::core::string::string_util::split(cTmp, "+-:/\\ \t.", ss, 7);
                    if (n < 3) return false;

                    this->date.year = ::pilo::core::string::string_util::cstr_to_int32(ss[0]);
                    this->date.month =  (::pilo::i8_t) ::pilo::core::string::string_util::cstr_to_int32(ss[1]);
                    this->date.day = (::pilo::i8_t)::pilo::core::string::string_util::cstr_to_int32(ss[2]);

                    if (n >= 4) this->time.hour = (::pilo::i8_t)::pilo::core::string::string_util::cstr_to_int32(ss[3]);
                    if (n >= 5) this->time.minute = (::pilo::i8_t)::pilo::core::string::string_util::cstr_to_int32(ss[4]);
                    if (n >= 6) this->time.second = (::pilo::i8_t) ::pilo::core::string::string_util::cstr_to_int32(ss[5]);
                    if (n >= 7)
                    {
                        if (use_millisecond)
                        {
                            this->time.microsecond = ::pilo::core::string::string_util::cstr_to_int32(ss[6]) * 1000;
                        }
                        else
                        {
                            this->time.microsecond = ::pilo::core::string::string_util::cstr_to_int32(ss[6]);
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
				static int  days_in_months(int year, int month);
				static bool is_leap_year(int year);
                static ::pilo::core::datetime::local_datetime now();
                static ::pilo::i64_t epoch_time();


#ifdef WINDOWS

                inline ::pilo::i64_t ticks_to_epoch_secs(::pilo::i64_t v)
                {
                    return (v / 10000000);
                }

                inline static ::pilo::i64_t steady_now_ticks() //nano in windws
                {
                    LARGE_INTEGER freq;
                    LARGE_INTEGER pcounter;
                    QueryPerformanceFrequency(&freq);
                    QueryPerformanceCounter(&pcounter);                    
                    return (::pilo::i64_t) ((double)(1000000000.0L / freq.QuadPart) * pcounter.QuadPart);
                }

                inline static ::pilo::i64_t steady_now_microsecs()
                {
                    LARGE_INTEGER freq;
                    LARGE_INTEGER pcounter;
                    QueryPerformanceFrequency(&freq);
                    QueryPerformanceCounter(&pcounter);
                    return (::pilo::i64_t) ((double)(1000000.0L / freq.QuadPart) * pcounter.QuadPart);
                }

                inline static ::pilo::i64_t steady_now_millisecs()
                {
                    LARGE_INTEGER freq;
                    LARGE_INTEGER pcounter;
                    QueryPerformanceFrequency(&freq);
                    QueryPerformanceCounter(&pcounter);
                    return (::pilo::i64_t) ((double)(1000.0L / freq.QuadPart) * pcounter.QuadPart);
                }

#if (M_CXX_FEATURE_TIME)
                inline static std::chrono::steady_clock::time_point steady_now_chrono()
                {
                    LARGE_INTEGER freq;
                    LARGE_INTEGER pcounter;
                    QueryPerformanceFrequency(&freq);
                    QueryPerformanceCounter(&pcounter);

                    return std::chrono::steady_clock::time_point(std::chrono::steady_clock::duration(
                        static_cast<std::chrono::steady_clock::rep>(((double)(1000000000.0L / freq.QuadPart) * pcounter.QuadPart))));
                }
#endif
                

                inline static ::pilo::i64_t now_ticks()
                {
                    FILETIME ft;
                    GetSystemTimeAsFileTime(&ft);
                    return ((static_cast<::pilo::i64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime) - MC_WIN_EPOCH_DIFF_HUNA;
                }

                inline static ::pilo::i64_t now_microsecs()
                {
                    FILETIME ft;
                    GetSystemTimeAsFileTime(&ft);
                    return (((static_cast<::pilo::i64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime) - MC_WIN_EPOCH_DIFF_HUNA) / 10;
                }

                inline static ::pilo::i64_t now_millisecs()
                {
                    FILETIME ft;
                    GetSystemTimeAsFileTime(&ft);
                    return (((static_cast<::pilo::i64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime) - MC_WIN_EPOCH_DIFF_HUNA) / 10000;
                }

#if (M_CXX_FEATURE_TIME)
                inline static std::chrono::system_clock::time_point now_chrono()
                {
                    FILETIME ft;
                    GetSystemTimeAsFileTime(&ft);                    

                    return std::chrono::system_clock::time_point(
                                std::chrono::steady_clock::duration (
                                static_cast<std::chrono::steady_clock::rep>((((static_cast<::pilo::i64_t>(ft.dwHighDateTime) << 32) | ft.dwLowDateTime) - MC_WIN_EPOCH_DIFF_HUNA))
                                                                    )
                                                                );
                }
#endif

#else


                inline ::pilo::i64_t ticks_to_epoch_secs(::pilo::i64_t v)
                {
                    return v / 1000000000;
                }

                inline static ::pilo::i64_t steady_now_ticks() //nano in windws
                {
                    timespec ts;
                    if ( ::clock_gettime( CLOCK_MONOTONIC, &ts ) )
                    {
                        M_ASSERT(0 && "clock_gettime error");
                    }

                    ::pilo::i64_t v = ts.tv_sec * 1000000000 +  ts.tv_nsec;
                    return v;
                }

                inline static ::pilo::i64_t steady_now_microsecs()
                {
                    timespec ts;
                    if ( ::clock_gettime( CLOCK_MONOTONIC, &ts ) )
                    {
                        M_ASSERT(0 && "clock_gettime error");
                    }

                    ::pilo::i64_t v = ts.tv_sec * 1000000000 +  ts.tv_nsec;
                    return (v / 1000);
                }

                inline static ::pilo::i64_t steady_now_millisecs()
                {
                    timespec ts;
                    if ( ::clock_gettime( CLOCK_MONOTONIC, &ts ) )
                    {
                        M_ASSERT(0 && "clock_gettime error");
                    }

                    ::pilo::i64_t v = ts.tv_sec * 1000000000 +  ts.tv_nsec;
                    return (v / 1000000);
                }

                inline static std::chrono::steady_clock::time_point steady_now_chrono()
                {
                    timespec ts;
                    if ( ::clock_gettime( CLOCK_MONOTONIC, &ts ) )
                    {
                        M_ASSERT(0 && "clock_gettime error");
                    }

                    return std::chrono::time_point<std::chrono::steady_clock, std::chrono::duration<long long, std::nano> >(
                        std::chrono::duration<long long, std::nano>(static_cast<std::chrono::steady_clock::rep>(ts.tv_sec) * 1000000000 + ts.tv_nsec)
                        );
                }


                inline static ::pilo::i64_t now_ticks()
                {
                    timespec ts;
                    if ( ::clock_gettime( CLOCK_REALTIME, &ts ) )
                    {
                        M_ASSERT(0 && "clock_gettime error");
                    }
                    ::pilo::i64_t v = ts.tv_sec * 1000000000 +  ts.tv_nsec;
                    return v;
                }

                inline static ::pilo::i64_t now_microsecs()
                {
                    timespec ts;
                    if ( ::clock_gettime( CLOCK_REALTIME, &ts ) )
                    {
                        M_ASSERT(0 && "clock_gettime error");
                    }
                    ::pilo::i64_t v = ts.tv_sec * 1000000000 +  ts.tv_nsec;
                    return (v/1000);
                }

                inline static ::pilo::i64_t now_millisecs()
                {
                    timespec ts;
                    if ( ::clock_gettime( CLOCK_REALTIME, &ts ) )
                    {
                        M_ASSERT(0 && "clock_gettime error");
                    }
                    ::pilo::i64_t v = ts.tv_sec * 1000000000 +  ts.tv_nsec;
                    return (v / 1000000);
                }
                inline static std::chrono::system_clock::time_point now_chrono()
                {
                    timespec ts;
                    if ( ::clock_gettime( CLOCK_REALTIME, &ts ) )
                    {
                        M_ASSERT(0 && "clock_gettime error");
                    }                    

                    return std::chrono::time_point<std::chrono::system_clock, std::chrono::duration<long long, std::nano> >(
                        std::chrono::duration<long long, std::nano>(static_cast<std::chrono::system_clock::rep>(ts.tv_sec) * 1000000000 + ts.tv_nsec)
                        );
                }

#endif // WINDOWS
                

				static pilo::i64_t calculate_year_initial_second(int year);
				static pilo::i64_t calculate_year_initial_second_fast(int year);
                static pilo::i64_t calculate_day_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_week_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_month_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_next_day_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_next_week_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_next_month_initial_second(pilo::i64_t sec);
                static pilo::i64_t calculate_next_year_initial_second(pilo::i64_t sec);
                
			public:
				datetime();
                datetime(pilo::i64_t tick, PiloDateTimeModeEnumeration mode);
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

                size_t to_string(char* szBuffer, size_t sz, PiloDateFormatEnumeration date_mode, PiloTimeFormatEnumeration time_mode) const;
                bool from_string(const char * datetimeStr, bool use_milli);
                

            public:
				bool from_local_datetime(const local_datetime& ldt);
                bool to_local_datetime(local_datetime& ldt) const;  
                void set(pilo::i64_t tick, PiloDateTimeModeEnumeration mode);

			protected:
                ::pilo::i64_t   m_epoch;               
			};
		}
	}
}