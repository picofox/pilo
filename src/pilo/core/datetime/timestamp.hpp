#pragma once

#include "../../pilo.hpp"
#include <chrono>
#include "../stat/system_information.hpp"
#include "../pattern/singleton.hpp"



#ifdef WINDOWS
#       define    _PMI_WIN_EPOCH_DIFF_SECS			(11644473600LL)
#       define    _PMI_MC_WIN_EPOCH_DIFF_HUNA		(116444736000000000LL)

#else
#		include <sys/time.h>
#endif

namespace pilo {
	namespace core {
		namespace datetime {			
			

			static inline ::pilo::i64_t std_time_point_to_timestamp(const std::chrono::system_clock::time_point& tp)
			{
				return std::chrono::time_point_cast<std::chrono::seconds>(tp).time_since_epoch().count();
			}
			
			static inline ::pilo::i64_t std_time_point_to_timestamp_milli(const std::chrono::system_clock::time_point& tp)
			{
				return std::chrono::time_point_cast<std::chrono::milliseconds>(tp).time_since_epoch().count();
			}

			static inline ::pilo::i64_t std_time_point_to_timestamp_micro(const std::chrono::system_clock::time_point& tp)
			{
				return std::chrono::time_point_cast<std::chrono::microseconds>(tp).time_since_epoch().count();
			}

			static inline ::pilo::i64_t std_time_point_to_timestamp_nano(const std::chrono::system_clock::time_point& tp)
			{
				return std::chrono::time_point_cast<std::chrono::nanoseconds>(tp).time_since_epoch().count();
			}


#ifdef WINDOWS

			/**
			* @brief                        get time stamp by steady clock, not affected by system clock.
			* @return                       current nano secs.
			* @see                          pea_calc_timestamp_micro_steady  pea_calc_timestamp_milli_steady
			* @note
			* @attention                    [MTS]
			*/
			static inline ::pilo::i64_t timestamp_nano_steady() //nano in windws
			{
				LARGE_INTEGER pcounter;
				QueryPerformanceCounter(&pcounter);

				return (long long)((double)(1000000000.0L / ::pilo::core::pattern::singleton<::pilo::core::stat::system_information>::instance()->freq()) * pcounter.QuadPart);
			}

			/**
			* @brief                        get time stamp by steady clock, not affected by system clock.
			* @return                       current micro secs.
			* @see                          pea_calc_timestamp_nano_steady  pea_calc_timestamp_milli_steady
			* @note
			* @attention                    [MTS]
			*/
			static inline ::pilo::i64_t timestamp_micro_steady()
			{
				LARGE_INTEGER pcounter;
				QueryPerformanceCounter(&pcounter);
				return (long long)((double)(1000000.0L / ::pilo::core::pattern::singleton<::pilo::core::stat::system_information>::instance()->freq()) * pcounter.QuadPart);
			}

			/**
			* @brief                        get time stamp by steady clock, not affected by system clock.
			* @return                       current milli secs.
			* @see                          pea_calc_timestamp_nano_steady pea_calc_timestamp_micro_steady
			* @note
			* @attention                    [MTS]
			*/
			static inline ::pilo::i64_t timestamp_milli_steady()
			{
				LARGE_INTEGER pcounter;
				QueryPerformanceCounter(&pcounter);
				return (long long)((double)(1000.0L / ::pilo::core::pattern::singleton<::pilo::core::stat::system_information>::instance()->freq()) * pcounter.QuadPart);
			}

			/**
			* @brief                        get time stamp by system clock, affected by system clock.
			* @return                       current ticks of system clock, in windows is by 100 nano secs, in linux is nano secs.
			* @see                          pea_calc_timestamp_nano_system pea_calc_timestamp_micro_system pea_calc_timestamp_milli_system
			* @note                         in windows, the pivot is not epoch. offset is _PEA_MI_MC_WIN_EPOCH_DIFF_HUNA
			* @attention                    [MTS]
			*/
			static inline ::pilo::i64_t timestamp_default_system()
			{
				FILETIME ft;
				GetSystemTimeAsFileTime(&ft);
				return (((long long)ft.dwHighDateTime << 32) | ft.dwLowDateTime);
			}

			/**
			* @brief                        get time stamp by system clock, affected by system clock.
			* @return                       current nano secs of system clock.
			* @see                          pea_calc_timestamp_default_system pea_calc_timestamp_micro_system pea_calc_timestamp_milli_system
			* @note
			* @attention                    [MTS]
			*/
			static inline ::pilo::i64_t timestamp_nano_system()
			{
				FILETIME ft;
				GetSystemTimeAsFileTime(&ft);
				return (((((long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime) - _PMI_MC_WIN_EPOCH_DIFF_HUNA) * 100;
			}

			/**
			* @brief                        get time stamp by system clock, affected by system clock.
			* @return                       current micro secs of system clock.
			* @see                          pea_calc_timestamp_default_system pea_calc_timestamp_nano_system pea_calc_timestamp_milli_system
			* @note
			* @attention                    [MTS]
			*/
			static inline ::pilo::i64_t timestamp_micro_system()
			{
				FILETIME ft;
				GetSystemTimeAsFileTime(&ft);
				return (((((long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime) - _PMI_MC_WIN_EPOCH_DIFF_HUNA) / 10;
			}

			/**
			* @brief                        get time stamp by system clock, affected by system clock.
			* @return                       current milli secs of system clock.
			* @see                          pea_calc_timestamp_default_system pea_calc_timestamp_nano_system pea_calc_timestamp_micro_system
			* @note
			* @attention                    [MTS]
			*/
			static inline ::pilo::i64_t timestamp_milli_system()
			{
				FILETIME ft;
				GetSystemTimeAsFileTime(&ft);
				return (((((long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime) - _PMI_MC_WIN_EPOCH_DIFF_HUNA) / 10000;
			}

			/**
			* @brief                        get time stamp by system clock, affected by system clock.
			* @return                       current secs of system clock.
			* @see                          pea_calc_timestamp_default_system pea_calc_timestamp_nano_system pea_calc_timestamp_micro_system
			* @note
			* @attention                    [MTS]
			*/
			static inline ::pilo::i64_t timestamp_system()
			{
				FILETIME ft;
				GetSystemTimeAsFileTime(&ft);
				return (((((long long)ft.dwHighDateTime) << 32) | ft.dwLowDateTime) - _PMI_MC_WIN_EPOCH_DIFF_HUNA) / 10000000;
			}

#else

			static inline ::pilo::i64_t timestamp_nano_steady() //nano in windws
			{
				struct timespec ts;
				if (clock_gettime(CLOCK_MONOTONIC, &ts))
				{
					PMC_ASSERT(0 && "clock_gettime error");
				}

				return ts.tv_sec * 1000000000 + ts.tv_nsec;
			}

			static inline ::pilo::i64_t timestamp_micro_steady()
			{
				struct timespec ts;
				if (clock_gettime(CLOCK_MONOTONIC, &ts))
				{
					PMC_ASSERT(0 && "clock_gettime error");
				}

				return (ts.tv_sec * 1000000 + ts.tv_nsec / 1000);
			}

			static inline ::pilo::i64_t timestamp_milli_steady()
			{
				struct timespec ts;
				if (clock_gettime(CLOCK_MONOTONIC, &ts))
				{
					PMC_ASSERT(0 && "clock_gettime error");
				}

				return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
			}

			static inline ::pilo::i64_t timestamp_default_system()
			{
				struct timespec ts;
				if (clock_gettime(CLOCK_REALTIME, &ts))
				{
					PMC_ASSERT(0 && "clock_gettime error");
				}
				return ts.tv_sec * 1000000000 + ts.tv_nsec;
			}

			static inline ::pilo::i64_t timestamp_nano_system()
			{
				struct timespec ts;
				if (clock_gettime(CLOCK_REALTIME, &ts))
				{
					PMC_ASSERT(0 && "clock_gettime error");
				}
				return ts.tv_sec * 1000000000 + ts.tv_nsec;
			}

			static inline ::pilo::i64_t timestamp_micro_system()
			{
				struct timespec ts;
				if (clock_gettime(CLOCK_REALTIME, &ts))
				{
					PMC_ASSERT(0 && "clock_gettime error");
				}
				return ts.tv_sec * 1000000 + ts.tv_nsec / 1000;
			}

			static inline ::pilo::i64_t timestamp_milli_system()
			{
				struct timespec ts;
				if (clock_gettime(CLOCK_REALTIME, &ts))
				{
					PMC_ASSERT(0 && "clock_gettime error");
				}
				long long v = ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
				return v;
			}

			static inline ::pilo::i64_t timestamp_system()
			{
				struct timespec ts;
				if (clock_gettime(CLOCK_REALTIME, &ts))
				{
					PMC_ASSERT(0 && "clock_gettime error");
				}
				long long v = ts.tv_sec * 1000 + ts.tv_nsec / 1000000000;
				return v;
			}


#endif
		
		}
	}
}