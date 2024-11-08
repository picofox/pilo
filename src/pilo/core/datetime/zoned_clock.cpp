#include "zoned_clock.hpp"
#include <sstream>
#include <iomanip>
#include <ctime>
#include "./datetime.hpp"
#include "../process/context.hpp"

namespace pilo {
	namespace core {
		namespace datetime {

			std::string zoned_clock::format_to_string(::pilo::i64_t t, const char* fmt, ::pilo::i8_t tz)
			{
				::pilo::u64_t dtv = timestamp_to_datetime(t, tz);
				char tmp[128] = { 0 };
				format_datetime_to_cstring(tmp, sizeof(tmp), fmt, dtv);
				return std::string(tmp);
			}
			zoned_clock::zoned_clock(::pilo::i8_t tz) :_m_timestamp(0), _m_datetime(0)
			{
				set_timezone_hours_offset(tz);
			}
			zoned_clock::zoned_clock() :_m_timestamp(0), _m_datetime(PMI_INVALID_DATETIME)
			{
			}
			zoned_clock::zoned_clock(::pilo::i64_t ts, ::pilo::i8_t tz) : _m_timestamp(ts)
			{
				_m_datetime = timestamp_to_datetime(_m_timestamp, tz);
			}
			::pilo::i8_t zoned_clock::days_in_month() const
			{
				return ::pilo::core::datetime::days_of_months(::pilo::core::datetime::month_of_datetime(_m_datetime), this->is_leap());
			}
			void zoned_clock::from_milliseconds_epoch(::pilo::i64_t milliseconds)
			{
				from_microseconds_epoch(milliseconds * 1000);
			}

			void zoned_clock::from_microseconds_epoch(::pilo::i64_t usec)
			{
				_m_timestamp = usec;
				_m_datetime = timestamp_to_datetime(_m_timestamp, timezone());
			}

			void zoned_clock::set_timezone_hours_offset(::pilo::i8_t tz)
			{
				if (tz == PMI_USE_SYSTEM_TIMEZONE) {
					::pilo::core::datetime::set_timezone_hoff_of_datetime(_m_datetime, PILO_CONTEXT->system_information()->system_timezone());
				}
				else if (tz == PMI_USE_OVERRIDED_TIMEZONE) {
					::pilo::core::datetime::set_timezone_hoff_of_datetime(_m_datetime, PILO_CONTEXT->core_config()->overrided_timezone());
				}
				else {
					::pilo::core::datetime::set_timezone_hoff_of_datetime(_m_datetime, tz);
				}
			}

			::pilo::i64_t zoned_clock::first_second_of_year() const
			{
				return ::pilo::core::datetime::first_second_of_year(::pilo::core::datetime::year_of_datetime(this->_m_datetime), this->timezone());
			}

			::pilo::i64_t zoned_clock::first_second_of_week() const
			{
				return ::pilo::core::datetime::first_second_of_week(_m_timestamp, this->timezone());
			}

			::pilo::i64_t zoned_clock::first_second_of_day() const
			{
				return ::pilo::core::datetime::first_second_of_day(_m_timestamp, this->timezone());
			}

			::pilo::i64_t zoned_clock::first_second_of_month() const
			{
				return this->first_microsecond_of_month();
			}

			::pilo::i64_t zoned_clock::first_microsecond_of_day() const
			{
				return ::pilo::core::datetime::first_microsecond_of_day(this->_m_timestamp, this->timezone());
			}

			::pilo::i64_t zoned_clock::first_microsecond_of_week() const
			{
				return 	::pilo::core::datetime::first_microsecond_of_week(this->_m_timestamp, this->timezone());
			}

			::pilo::i64_t zoned_clock::first_microsecond_of_month() const
			{
				return ::pilo::core::datetime::first_microsecond_of_month(_m_datetime) / PMI_USECS_OF_SEC;
			}

			::pilo::i64_t zoned_clock::first_microsecond_of_hour() const
			{
				::pilo::i64_t today_ts = ::pilo::core::datetime::first_microsecond_of_day(_m_timestamp, this->timezone());
				::pilo::i64_t hours_off = ::pilo::core::datetime::hour_of_datetime(_m_datetime);
				return today_ts + hours_off * PMI_USECS_OF_HOUR;
			}

			::pilo::i64_t zoned_clock::first_second_of_hour() const
			{
				return this->first_microsecond_of_hour() / PMI_USECS_OF_SEC;
			}

			::pilo::i64_t zoned_clock::first_second_of_next_hours(::pilo::i64_t n) const
			{
				return this->first_microsecond_of_next_hours(n) / PMI_USECS_OF_SEC;
			}

			::pilo::i64_t zoned_clock::first_microsecond_of_next_hours(::pilo::i64_t n) const
			{
				return ::pilo::core::datetime::first_microsecond_of_next_hours(_m_timestamp, this->timezone(), n);
			}

			::pilo::i64_t zoned_clock::first_second_of_next_days(::pilo::i64_t n) const
			{
				return ::pilo::core::datetime::first_second_of_next_days(_m_timestamp, this->timezone(), n);
			}

			::pilo::i64_t zoned_clock::first_microsecond_of_next_days(::pilo::i64_t n) const
			{
				return ::pilo::core::datetime::first_microsecond_of_next_days(_m_timestamp, this->timezone(), n);
			}


			::pilo::i64_t zoned_clock::microsecond_epoch() const
			{
				return _m_timestamp;
			}

			void zoned_clock::set(::pilo::u16_t year, ::pilo::u8_t mon, ::pilo::u8_t mday, ::pilo::u8_t hour, ::pilo::u8_t min, ::pilo::u8_t sec, ::pilo::u32_t usec)
			{
				this->_m_datetime = ::pilo::core::datetime::make_datetime(year, mon, mday, hour, min, sec, usec, this->timezone());
				this->_m_timestamp = ::pilo::core::datetime::datetime_to_timestamp(this->_m_datetime, this->timezone());				
			}

			::pilo::i64_t zoned_clock::add_months(::pilo::i64_t months)
			{
				::pilo::u8_t m = ::pilo::core::datetime::month_of_datetime(this->_m_datetime);
				::pilo::u8_t d = ::pilo::core::datetime::mday_of_datetime(this->_m_datetime);
				::pilo::i64_t total_days = 0;
				::pilo::i16_t y = ::pilo::core::datetime::year_of_datetime(this->_m_datetime);
				bool b_leap = ::pilo::core::datetime::is_leap_year(y);

				for (::pilo::i64_t i = 0; i < months; i++) {
					if (m == 12) {
						total_days += 31;
						y++;
						b_leap = ::pilo::core::datetime::is_leap_year(y);
						m = 1;
					}
					else if (m == 1) {
						if (d <= 28) {
							total_days += 31;
							m++;
						}
						else {
							if (d == 29) {
								if (b_leap) {
									total_days += 31;
									m++;
								}
								else {
									total_days += 30;
									m++;
								}								
							}
							else if (d == 30) {
								if (b_leap) {
									total_days += 30;
									m++;
								}
								else {
									total_days += 29;
									m++;
								}
							}
							else if (d == 31) {
								if (b_leap) {
									total_days += 29;
									m++;
								}
								else {
									total_days += 28;
									m++;
								}
							}						
						}
					}
					else if (m == 2) {
						::pilo::u8_t dtmp = 28;
						if (b_leap)
							dtmp = 29;
						total_days += dtmp;
						m++;
					}
					else if (m == 3 || m==5 || m == 8 || m == 10) {
						if (d < 31) {
							total_days += 31;
							m++;
						}
						else {
							total_days += 30;
							m++;
						}
					}
					else if (m == 4 || m == 6 || m == 9 || m == 11) {
						total_days += 30;
						m++;
					}
					else if (m == 7) {
						total_days += 31;
						m++;
					}					
				}

				this->add_days(total_days);
				return total_days;
			}

			void zoned_clock::add_days(::pilo::i64_t days)
			{
				this->_m_timestamp += PMI_USECS_OF_DAY * (::pilo::i64_t) days;
				_m_datetime = timestamp_to_datetime(_m_timestamp, timezone());
			}

			::pilo::u8_t zoned_clock::week_day() const
			{
				return ::pilo::core::datetime::week_day(this->_m_timestamp, this->timezone());
			}

			void zoned_clock::add_seconds(::pilo::i64_t sec)
			{
				this->_m_timestamp += (sec * PMI_USECS_OF_SEC);
				_m_datetime = timestamp_to_datetime(_m_timestamp, this->timezone());
			}

			void zoned_clock::add_hours(::pilo::i64_t hours)
			{
				this->add_seconds(hours*PMI_SECS_OF_HOUR);
			}

			void zoned_clock::add_minutes(::pilo::i64_t mins)
			{
				this->add_seconds(mins*PMI_SECS_OF_MINUTE);
			}

			void zoned_clock::add_milliseconds(::pilo::i64_t msec)
			{
				this->_m_timestamp += (msec * PMI_MSECS_OF_SEC);
				_m_datetime = timestamp_to_datetime(_m_timestamp, this->timezone());
			}

			void zoned_clock::add_microseconds(::pilo::i64_t usec)
			{
				this->_m_timestamp += usec;
				_m_datetime = timestamp_to_datetime(_m_timestamp, this->timezone());
			}



			std::string zoned_clock::to_string(const char* fmt)
			{
				if (fmt == nullptr)
					fmt = "%Y-%M-%D %hh:%mm:%ss.%6us (%2tz)";
				return zoned_clock::format_to_string(_m_timestamp, fmt, this->timezone());
			}
		}
	}
}