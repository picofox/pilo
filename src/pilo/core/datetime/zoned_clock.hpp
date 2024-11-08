#ifndef _pilo_core_datetime_system_clock_hpp_
#define _pilo_core_datetime_system_clock_hpp_


#include	"../../pilo.hpp"
#include	"./datetime.hpp"
#include	"./timestamp.hpp"


namespace pilo {
	namespace core {
		namespace datetime {

					

			class zoned_clock
			{
			public:
				static zoned_clock invalid_zoned_clock;

			public:
				inline static systime s_now_systime()
				{
					return std::chrono::system_clock::now();
				}

				static std::string format_to_string(::pilo::i64_t t, const char* fmt, ::pilo::i8_t tz);
			
			public:
				zoned_clock(::pilo::i8_t tz);
				zoned_clock();
				zoned_clock(::pilo::i64_t ts, ::pilo::i8_t tz);

				::pilo::u16_t year() const { return ::pilo::core::datetime::year_of_datetime(_m_datetime); }
				::pilo::u8_t month() const { return ::pilo::core::datetime::month_of_datetime(_m_datetime); }
				::pilo::u8_t month_day() const { return ::pilo::core::datetime::mday_of_datetime(_m_datetime); }
				::pilo::u8_t week_day() const;
				::pilo::u8_t hour() const { return ::pilo::core::datetime::hour_of_datetime(_m_datetime); }
				::pilo::u8_t minute() const { return ::pilo::core::datetime::min_of_datetime(_m_datetime); }
				::pilo::u8_t second() const { return ::pilo::core::datetime::sec_of_datetime(_m_datetime); }
				::pilo::u32_t microsecond() const { return ::pilo::core::datetime::microsec_of_datetime(_m_datetime); }
				::pilo::u32_t millisecond() const { return ::pilo::core::datetime::microsec_of_datetime(_m_datetime) / 1000; }
				::pilo::i8_t timezone() const { return ::pilo::core::datetime::timezone_hoff_of_datetime(_m_datetime); }
				::pilo::i64_t timezone_milliseconds_offset() const { return PMF_TZ_USEC_OFF(::pilo::core::datetime::timezone_hoff_of_datetime(_m_datetime)); }

				bool is_leap() const { return ::pilo::core::datetime::is_leap_year(this->year());  }
				::pilo::i8_t days_in_month() const;
				
				void from_milliseconds_epoch(::pilo::i64_t msec);
				void from_microseconds_epoch(::pilo::i64_t usec);
				void set_timezone_hours_offset(::pilo::i8_t tz);

				void set(::pilo::i64_t ts)
				{
					_m_timestamp = ts;
					_m_datetime = timestamp_to_datetime(_m_timestamp, timezone());
				}

				void update() 
				{ 
					_m_timestamp = ::pilo::core::datetime::timestamp_micro_system();
					_m_datetime = timestamp_to_datetime(_m_timestamp, timezone());
				}

				void set_invalid()
				{
					_m_datetime = PMI_INVALID_DATETIME;
				}

				bool is_invalid() const {
					return (_m_datetime == PMI_INVALID_DATETIME);
				}

				::pilo::u64_t datetime() const { return _m_datetime; }
				::pilo::i64_t timestamp() const { return _m_timestamp; }
				::pilo::i64_t first_second_of_year() const;
				::pilo::i64_t first_second_of_week() const;
				::pilo::i64_t first_second_of_day() const;
				::pilo::i64_t first_second_of_month() const;
				::pilo::i64_t first_second_of_hour() const;
				::pilo::i64_t first_microsecond_of_day() const;
				::pilo::i64_t first_microsecond_of_week() const;
				::pilo::i64_t first_microsecond_of_month() const;
				::pilo::i64_t first_microsecond_of_hour() const;

				::pilo::i64_t first_second_of_next_hours(::pilo::i64_t n) const;
				::pilo::i64_t first_microsecond_of_next_hours(::pilo::i64_t n) const;
				::pilo::i64_t first_second_of_next_days(::pilo::i64_t n) const;
				::pilo::i64_t first_microsecond_of_next_days(::pilo::i64_t n) const;

				::pilo::i64_t microsecond_epoch() const;

				void ceil_by_second() 
				{ 
					::pilo::core::datetime::ceil_by_second(&_m_timestamp); 
					_m_datetime = timestamp_to_datetime(_m_timestamp, timezone());
				}

				::pilo::i64_t add_months(::pilo::i64_t months);
				void add_days(::pilo::i64_t days);
				void add_seconds(::pilo::i64_t msec);
				void add_hours(::pilo::i64_t hours);
				void add_minutes(::pilo::i64_t mins);
				void add_milliseconds(::pilo::i64_t msec);
				void add_microseconds(::pilo::i64_t usec);

				void set(::pilo::u16_t year, ::pilo::u8_t mon, ::pilo::u8_t mday, ::pilo::u8_t hour, ::pilo::u8_t min, ::pilo::u8_t sec, ::pilo::u32_t usec);


				std::string to_string(const char* fmt = nullptr);

			protected:
				::pilo::i64_t		_m_timestamp;
				::pilo::u64_t		_m_datetime;				
			};


		}
	}
}


#endif