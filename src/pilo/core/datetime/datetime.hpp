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

namespace pilo
{
	namespace core
	{
		namespace datetime
		{           

			struct local_time
			{
				pilo::i32_t      hour;
                pilo::i32_t      minute;
                pilo::i32_t      second;
                pilo::i32_t      millisecond;

                inline bool is_valid() const
                {
                    if (this->hour < 0 || this->hour > 24) return false;
                    if (this->minute < 0 || this->minute > 60) return false;
                    if (this->second < 0 || this->second > 60) return false;

                    if (this->millisecond != (pilo::i32_t)(-1))
                    {
                        if (this->millisecond < 0 || this->millisecond > 1000) return false;
                    }
                    return true;
                }                

                pilo::core::string::fixed_astring<24> to_string() const
                {
                    pilo::core::string::fixed_astring<24> str;

                    if (is_valid())
                    {
                        if (this->millisecond != -1)
                        {
                            str.format("%02d:%02d:%02d.%03d", hour, minute, second, millisecond);
                        }
                        else
                        {
                            str.format("%02d:%02d:%02d", hour, minute, second);
                        }
                    }
                    else
                    {
                        str.format("<Invalid Format of local_time@%p>", this);
                    }
                    
                    return str;
                }
			};

			struct local_datetime
			{
                pilo::i32_t     year;
                pilo::i32_t     month;
                pilo::i32_t     day;
                pilo::i32_t     hour;
                pilo::i32_t     minute;
                pilo::i32_t     second;
                pilo::i32_t     millisecond;

                bool is_valid() const;
                
                bool from_string(const char * datetimeStr)
                {
                    char* ss[7];
                    char cTmp[128];

                    int len = (int)::pilo::core::string::string_util::length(datetimeStr);
                    if (len >= 128) len = 100;

                    memcpy(cTmp, datetimeStr, len);
                    cTmp[len] = '\0';

                    size_t n = ::pilo::core::string::string_util::split(cTmp, "+-:/\\ \t.", ss, 7);
                    if (n < 3) return false;

                    this->year = ::pilo::core::string::string_util::cstr_to_int32(ss[0]);
                    this->month = ::pilo::core::string::string_util::cstr_to_int32(ss[1]);
                    this->day = ::pilo::core::string::string_util::cstr_to_int32(ss[2]);

                    if (n >= 4) this->hour = ::pilo::core::string::string_util::cstr_to_int32(ss[3]);
                    if (n >= 5) this->minute = ::pilo::core::string::string_util::cstr_to_int32(ss[4]);
                    if (n >= 6) this->second = ::pilo::core::string::string_util::cstr_to_int32(ss[5]);
                    if (n >= 7) this->millisecond = ::pilo::core::string::string_util::cstr_to_int32(ss[6]);

                    return true;
                }

                ::pilo::core::string::fixed_astring<48> to_string() const
                {
                    pilo::core::string::fixed_astring<48> str;
        
                    if (is_valid())
                    {
                        if (millisecond != -1)
                        {
                            str.format("%04d-%02d-%02d %02d:%02d:%02d.%03d", year, month, day, hour, minute, second, millisecond);                            
                        }
                        else
                        {
                            str.format("%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
                        }                        
                    }
                    else
                    {
                        str.format("<Invalid Format of local_datetime@%p>", this);
                    }                    
                    
                    return str;
                }
                
			};
			class datetime
			{
			public:
				enum   { day_seconds = 24 * 3600, hour_seconds = 3600, min_seconds = 60 };
                static pilo::i32_t diff_seconds_local_to_utc();
				static int  days_in_months(int year, int month);
				static bool is_leap_year(int year);
				static bool is_valid_local_datetime(const local_datetime& ldt);
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
				datetime(pilo::i64_t t);
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
                datetime& refresh();                

                pilo::core::string::fixed_astring<48> to_string() const;
                bool from_string(const char * datetimeStr);
                

            public:
				bool from_local_datetime(const local_datetime& ldt);
                bool to_local_datetime(local_datetime& ldt) const;  

			protected:
                ::pilo::i64_t   m_epoch;               
			};
		}
	}
}