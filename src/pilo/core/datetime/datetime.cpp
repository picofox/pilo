#include "datetime.hpp"
#include "core/threading/nonrecursive_mutex.hpp"
#include "core/threading/mutex_locker.hpp"

namespace pilo
{
	namespace core
	{
		namespace datetime
		{  
            datetime::datetime() :m_epoch(::pilo::core::datetime::datetime::now_microsecs())
			{
                
			}

            datetime::datetime(pilo::i64_t tick, PiloDateTimeModeEnumeration mode)
			{
                set(tick,mode);
			}

			datetime::datetime(const local_datetime& ldt)
			{
				this->from_local_datetime(ldt);
			}

			datetime::datetime(const datetime& other) : m_epoch(other.m_epoch)
			{

			}

			datetime& datetime::operator=(const datetime& other)
			{
				if (this == &other) return *this;
				m_epoch = other.m_epoch;
				return *this;
			}

            datetime::~datetime()
            {

            }

            pilo::i32_t datetime::to_days() const
            {
                return (int)((m_epoch/1000000 - pilo::core::datetime::datetime::diff_seconds_local_to_utc()) / day_seconds + 1);
            }

            pilo::i32_t datetime::to_local_days() const
            {
                return (int)((m_epoch/1000000 - pilo::core::datetime::datetime::diff_seconds_local_to_utc()) / day_seconds + 1);
            }

            pilo::i32_t datetime::week_day() const
            {
                pilo::i32_t days = this->to_local_days();
                //1970年1月1日是星期四
                //周1返回1...周日返回7
                int n = (days + 3) % 7;
                return n ? n : 7;
            }

            pilo::i32_t datetime::month_day() const
            {
                local_datetime ldt;
                if (! this->to_local_datetime(ldt))
                {
                    return -1;
                }
                return ldt.date.day;
            }


            datetime& datetime::add_days(int days)
            {
                m_epoch += (days * day_seconds * 1000000);
                return *this;
            }

            datetime& datetime::add_seconds(int seconds)
            {
                m_epoch += (seconds * 1000000);
                return *this;
            }

            datetime& datetime::update()
            {
                m_epoch = ::pilo::core::datetime::datetime::now_microsecs();
                return *this;
            }

            size_t datetime::to_string(char* szBuffer, size_t sz, PiloDateFormatEnumeration date_mode, PiloTimeFormatEnumeration time_mode) const
            {
                pilo::core::datetime::local_datetime ldt;
                if (! this->to_local_datetime(ldt))
                {

                    ::pilo::core::string::string_util::copy(szBuffer, "[to_local_datetime()<-to_string() failed.]");
                    return ::pilo::core::string::string_util::length("[to_local_datetime()<-to_string() failed.]");
                }
                else
                {

                    return ldt.to_string(szBuffer,sz,date_mode, time_mode);
                }
                
            }

            bool datetime::from_string(const char * datetimeStr, bool use_milli)
            {
                local_datetime ldt;
                memset(&ldt, 0, sizeof(ldt));
                ldt.from_string(datetimeStr, use_milli);
                return this->from_local_datetime(ldt);
            }

            bool datetime::from_local_datetime(const local_datetime& ldt)
			{
				if (! ldt.valid())
				{
					return false;
				}

				::pilo::i64_t epoch_secs = ::pilo::core::datetime::datetime::calculate_year_initial_second_fast(ldt.date.year);
                if (epoch_secs < 0) epoch_secs = 0;

				for (int i = 1; i < ldt.date.month; i++)
				{
					int days = ::pilo::core::datetime::datetime::days_in_months(ldt.date.year, i);
                    epoch_secs += days * day_seconds;
				}

                epoch_secs += (ldt.date.day - 1) * day_seconds;
                epoch_secs += ldt.time.hour * hour_seconds;
                epoch_secs += ldt.time.minute * min_seconds;
                epoch_secs += ldt.time.second;

                m_epoch = epoch_secs * 1000000;

                if (ldt.time.microsecond > 0)
                {
                    m_epoch += ldt.time.microsecond;
                }
                
				return true;
			}

            bool datetime::to_local_datetime(local_datetime& ref_ldt) const
            {
                enum { max_year_secs = 366 * day_seconds };

                //static const ::pilo::core::datetime::local_datetime emptyDatetime = { 0, 0, 0, 0, 0, 0, -1 };

                ::pilo::i64_t epoch_secs = m_epoch / 1000000;
                if (epoch_secs < 0)
                {
                    return false;
                }
                ::pilo::i32_t ms_left = (::pilo::i32_t) (m_epoch - (epoch_secs * 1000000));

                int ys = (int)(epoch_secs / max_year_secs);
                ::pilo::core::datetime::local_datetime ldt;
                //
                ldt.reset();
                ldt.date.year = ys;

                pilo::i64_t secs = datetime::calculate_year_initial_second_fast(ldt.date.year);
                if (secs < 0) 
                {
                    return false;
                }

                while (true)
                {
                    pilo::i64_t secs2 = datetime::calculate_year_initial_second_fast(ldt.date.year + 1);
                    if (secs2 < 0) 
                    {
                        return false;
                    }

                    if (epoch_secs < secs2)
                    {
                        break;
                    }
                    ldt.date.year += 1;
                    secs = secs2;
                }

                pilo::i64_t delta = epoch_secs - secs;
                for (; ldt.date.month <= 12; ldt.date.month++)
                {
                    int days = ::pilo::core::datetime::datetime::days_in_months(ldt.date.year, ldt.date.month);
                    if (delta >= days*day_seconds)
                    {
                        delta -= days*day_seconds;
                    }
                    else
                    {
                        for (; ldt.date.day <= days; ldt.date.day++)
                        {
                            if (delta >= day_seconds)
                            {
                                delta -= day_seconds;
                            }
                            else
                            {
                                ldt.time.hour = (pilo::i8_t)(delta / hour_seconds);
                                ldt.time.minute = (pilo::i8_t)((delta%hour_seconds) / min_seconds);
                                ldt.time.second = (pilo::i8_t)((delta%hour_seconds) % min_seconds);
                                ldt.time.microsecond =(pilo::i8_t) ms_left;
                                ref_ldt = ldt;
                                return true;
                            }
                        }
                    }
                }

                // 不可达
                return false;
            }

            void datetime::set(pilo::i64_t tick, PiloDateTimeModeEnumeration mode)
            {
                if (mode == ePDTM_Seconds)
                {
                    m_epoch = tick * 1000000;
                }
                else if (mode == ePDTM_Millisecond)
                {
                    m_epoch = tick * 1000;
                }
                else if (mode == ePDTM_Microsecond)
                {
                    m_epoch = tick;
                }
            }

            pilo::core::datetime::local_datetime datetime::now()
            {
                ::pilo::core::datetime::local_datetime ldt;
                ::pilo::core::datetime::datetime dt;
                if (! dt.to_local_datetime(ldt))
                {
                    ldt.date.day = -1;
                    ldt.time.hour = -1;
                }                

                return ldt;
            }

            ::pilo::i64_t datetime::epoch_time()
            {
#ifdef      WINDOWS
#   ifdef   __x86_64__                
                return (::pilo::i64_t) _time64(NULL);
#   else
                return (::pilo::i64_t) _time32(NULL);
#   endif
#else
             
                return (::pilo::i64_t) ::time(NULL);;

                
#endif

            }

            pilo::i64_t datetime::calculate_year_initial_second(int year)
			{
				struct tm stm = { 0, 0, 0, 1, 0, year - 1900, 0, 0, 0 };
				return mktime(&stm);
			}

			pilo::i64_t datetime::calculate_year_initial_second_fast(int year)
			{
#define year_count 200
				const int startYear = 1971;
				const int endYear = startYear + year_count;

				static ::pilo::core::threading::nonrecursive_mutex mx;
				static ::pilo::i64_t yearSecs[year_count];
				volatile static bool bInit = false;

				if (year < startYear || year >= endYear)
				{
					return ::pilo::core::datetime::datetime::calculate_year_initial_second(year);
				}
				else
				{
					if (bInit)
					{
						return yearSecs[year - startYear];
					}
					else
					{
						::pilo::core::threading::mutex_locker<::pilo::core::threading::nonrecursive_mutex> am(mx);
						if (bInit)
						{
							return yearSecs[year - startYear];
						}
						else
						{
							for (int i = 0; i < year_count; i++)
							{
								yearSecs[i] = ::pilo::core::datetime::datetime::calculate_year_initial_second(startYear + i);
							}
							bInit = true;
							return yearSecs[year - startYear];
						}
					}
				}
			}

            pilo::i64_t datetime::calculate_day_initial_second(pilo::i64_t sec)
            {
                datetime dt(sec, ePDTM_Seconds);
                local_datetime ldt; 
                if (! dt.to_local_datetime(ldt))
                {
                    return  (pilo::i64_t) -1;
                }

                ldt.time.hour = ldt.time.minute = ldt.time.second = 0;
                dt.from_local_datetime(ldt);
                return (pilo::i64_t)dt.seconds_since_epoch();
            }

            pilo::i64_t datetime::calculate_week_initial_second(pilo::i64_t sec)
            {
                ::pilo::core::datetime::datetime dt(sec, ePDTM_Seconds);

                ::pilo::core::datetime::local_datetime ldt;
                if (!dt.to_local_datetime(ldt))
                {
                    return  (pilo::i64_t) -1;
                }

                ldt.time.hour = ldt.time.minute = ldt.time.second = 0;
                dt.from_local_datetime(ldt);

                int weekDay = dt.week_day();
                if (weekDay != 1)
                {
                    dt.add_days(1 - weekDay);
                }
                
                return (pilo::i64_t)dt.seconds_since_epoch();
            }

            pilo::i64_t datetime::calculate_month_initial_second(pilo::i64_t sec)
            {
                ::pilo::core::datetime::datetime dt(sec, ePDTM_Seconds);
                ::pilo::core::datetime::local_datetime ldt;
                dt.to_local_datetime(ldt);
                ldt.time.reset();
                dt.from_local_datetime(ldt);

                dt.add_days(1 - ldt.date.day);

                return (pilo::i64_t)dt.seconds_since_epoch();
            }

            pilo::i64_t datetime::calculate_next_day_initial_second(pilo::i64_t sec)
            {
                return ::pilo::core::datetime::datetime::calculate_day_initial_second(sec) + day_seconds;
            }

            pilo::i64_t datetime::calculate_next_week_initial_second(pilo::i64_t sec)
            {
                return ::pilo::core::datetime::datetime::calculate_week_initial_second(sec) + day_seconds * 7;
            }

            pilo::i64_t datetime::calculate_next_month_initial_second(pilo::i64_t sec)
            {
                ::pilo::core::datetime::datetime dt(sec, ePDTM_Seconds);
                ::pilo::core::datetime::local_datetime ldt;
                dt.to_local_datetime(ldt);
                ldt.time.reset();
                dt.from_local_datetime(ldt);
                dt.add_days(1 - ldt.date.day);

                int mdays = ::pilo::core::datetime::datetime::days_in_months(ldt.date.year, ldt.date.month);
                return ::pilo::core::datetime::datetime::calculate_day_initial_second(sec) + day_seconds*mdays;                
            }

            pilo::i64_t datetime::calculate_next_year_initial_second(pilo::i64_t sec)
            {
                ::pilo::core::datetime::datetime dt(sec, ePDTM_Seconds);
                ::pilo::core::datetime::local_datetime ldt;
                dt.to_local_datetime(ldt);

                return ::pilo::core::datetime::datetime::calculate_year_initial_second(ldt.date.year+1);
            }

            bool datetime::operator==(const ::pilo::core::datetime::datetime& other) const
            {
                return m_epoch == other.m_epoch;
            }
    
            bool datetime::operator!=(const ::pilo::core::datetime::datetime& other) const
            {
                return m_epoch != other.m_epoch;
            }

            bool datetime::operator>(const ::pilo::core::datetime::datetime& other) const
            {
                return m_epoch > other.m_epoch;
            }

            bool datetime::operator<(const ::pilo::core::datetime::datetime& other) const
            {
                return m_epoch < other.m_epoch;
            }

            bool datetime::operator>=(const ::pilo::core::datetime::datetime& other) const
            {
                return m_epoch >= other.m_epoch;
            }

            bool datetime::operator<=(const ::pilo::core::datetime::datetime& other) const
            {
                return m_epoch <= other.m_epoch;
            }

            pilo::i32_t datetime::diff_seconds_local_to_utc()
            {
#ifdef      WINDOWS
                pilo::i32_t t;
                _get_timezone((long*)&t);
                return t;
#else
                return timezone;
#endif
            }

            int datetime::days_in_months(int year, int month)
			{
				static int leapDays[] =    { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
				static int nonleapDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
				if (::pilo::core::datetime::datetime::is_leap_year(year))
				{
					return leapDays[month - 1];
				}
				else
				{
					return nonleapDays[month - 1];
				}
			} 

			bool datetime::is_leap_year(int year)
			{
				if ((year % 4) == 0)
				{
					if ((year % 100) == 0 && (year % 400) != 0)
					{
						return false;
					}
					else
					{
						return true;
					}
				}
				else
				{
					return false;
				}
			}
        }
	}
}