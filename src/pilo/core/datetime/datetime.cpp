#include <mutex>
#include "datetime.hpp"
#include "./timestamp.hpp"


namespace pilo
{
	namespace core
	{
		namespace datetime
		{ 
            const static ::pilo::i64_t __cst_stc_year_1st_sec_cache[] =
            {
                0, 31536000, 63072000, 94694400, 126230400, 157766400, 189302400, 220924800,
                252460800, 283996800, 315532800, 347155200, 378691200, 410227200, 441763200, 473385600,
                504921600, 536457600, 567993600, 599616000, 631152000, 662688000, 694224000, 725846400,
                757382400, 788918400, 820454400, 852076800, 883612800, 915148800, 946684800, 978307200,
                1009843200, 1041379200, 1072915200, 1104537600, 1136073600, 1167609600, 1199145600, 1230768000,
                1262304000, 1293840000, 1325376000, 1356998400, 1388534400, 1420070400, 1451606400, 1483228800,
                1514764800, 1546300800, 1577836800, 1609459200, 1640995200, 1672531200, 1704067200, 1735689600,
                1767225600, 1798761600, 1830297600, 1861920000, 1893456000, 1924992000, 1956528000, 1988150400,
                2019686400, 2051222400, 2082758400, 2114380800, 2145916800, 2177452800, 2208988800, 2240611200,
                2272147200, 2303683200, 2335219200, 2366841600, 2398377600, 2429913600, 2461449600, 2493072000,
                2524608000, 2556144000, 2587680000, 2619302400, 2650838400, 2682374400, 2713910400, 2745532800,
                2777068800, 2808604800, 2840140800, 2871763200, 2903299200, 2934835200, 2966371200, 2997993600,
                3029529600, 3061065600, 3092601600, 3124224000, 3155760000, 3187296000, 3218832000, 3250454400,
                3281990400, 3313526400, 3345062400, 3376684800, 3408220800, 3439756800, 3471292800, 3502915200,
                3534451200, 3565987200, 3597523200, 3629145600, 3660681600, 3692217600, 3723753600, 3755376000,
                3786912000, 3818448000, 3849984000, 3881606400, 3913142400, 3944678400, 3976214400, 4007836800,
                4039372800, 4070908800, 4102444800, 4133980800, 4165516800, 4197052800, 4228588800, 4260211200,
                4291747200, 4323283200, 4354819200, 4386441600, 4417977600, 4449513600, 4481049600, 4512672000,
                4544208000, 4575744000, 4607280000, 4638902400, 4670438400, 4701974400, 4733510400, 4765132800,
                4796668800, 4828204800, 4859740800, 4891363200, 4922899200, 4954435200, 4985971200, 5017593600,
                5049129600, 5080665600, 5112201600, 5143824000, 5175360000, 5206896000, 5238432000, 5270054400,
                5301590400, 5333126400, 5364662400, 5396284800, 5427820800, 5459356800, 5490892800, 5522515200,
                5554051200, 5585587200, 5617123200, 5648745600, 5680281600, 5711817600, 5743353600, 5774976000,
                5806512000, 5838048000, 5869584000, 5901206400, 5932742400, 5964278400, 5995814400, 6027436800,
                6058972800, 6090508800, 6122044800, 6153667200, 6185203200, 6216739200, 6248275200, 6279897600,
                6311433600, 6342969600, 6374505600, 6406128000, 6437664000, 6469200000, 6500736000, 6532358400,
                6563894400, 6595430400, 6626966400, 6658588800, 6690124800, 6721660800, 6753196800, 6784819200,
                6816355200, 6847891200, 6879427200, 6911049600, 6942585600, 6974121600, 7005657600, 7037280000,
                7068816000, 7100352000, 7131888000, 7163510400, 7195046400, 7226582400, 7258118400, 7289654400,
                7321190400, 7352726400, 7384262400, 7415884800, 7447420800, 7478956800, 7510492800, 7542115200,
                7573651200, 7605187200, 7636723200, 7668345600, 7699881600, 7731417600, 7762953600, 7794576000,
                7826112000, 7857648000, 7889184000, 7920806400, 7952342400, 7983878400, 8015414400, 8047036800
            };


            datetime::datetime() :m_epoch(::pilo::core::datetime::timestamp_micro_system())
			{
                
			}

            datetime::datetime(pilo::i64_t tick, datetime_precision_enum mode)
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
                m_epoch += ((::pilo::i64_t) ((::pilo::i64_t) days * day_seconds * 1000000LL));
                return *this;
            }

            datetime& datetime::add_seconds(int seconds)
            {
                m_epoch += ((::pilo::i64_t) seconds * 1000000LL);
                return *this;
            }

            datetime& datetime::update()
            {
                m_epoch = ::pilo::core::datetime::timestamp_micro_system();
                return *this;
            }

            size_t datetime::to_string(char* szBuffer, size_t sz, date_format_enum date_mode, time_format_enum time_mode) const
            {
                pilo::core::datetime::local_datetime ldt;
                if (! this->to_local_datetime(ldt))
                {

                    ::pilo::core::string::copyz(szBuffer, sz, "[to_local_datetime()<-to_string() failed.]");
                    return ::pilo::core::string::character_count("[to_local_datetime()<-to_string() failed.]");
                }
                else
                {

                    return ldt.to_string(szBuffer,sz,date_mode, time_mode);
                }
                
            }

            bool datetime::from_string(const char * datetimeStr, bool use_milli)
            {
                local_datetime ldt;
                ldt.from_string(datetimeStr, use_milli);
                return this->from_local_datetime(ldt);
            }

            bool datetime::from_local_datetime(const local_datetime& ldt)
			{
				if (! ldt.valid())
				{
					return false;
				}

				::pilo::i64_t epoch_secs = ::pilo::core::datetime::datetime::calculate_year_initial_second_local(ldt.date.year);
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

                ref_ldt.reset(1);

                ::pilo::i64_t epoch_secs = m_epoch / 1000000;
                if (epoch_secs < 0)
                {
                    return false;
                }
                ::pilo::i32_t ms_left = (::pilo::i32_t) (m_epoch - (epoch_secs * 1000000));

                int ys = (int)(epoch_secs / max_year_secs);
                ::pilo::core::datetime::local_datetime ldt;
                //
                ldt.set(ys+1970, 1,1,0,0,0,0);
                ldt.date.year = ys + 1970;

                pilo::i64_t secs = datetime::calculate_year_initial_second_local(ldt.date.year);
                if (secs == INT64_MIN) 
                {
                    return false;
                }

                while (true)
                {
                    pilo::i64_t secs2 = datetime::calculate_year_initial_second_local(ldt.date.year + 1);
                    if (secs2 == INT64_MIN)
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
                                ldt.time.microsecond =(pilo::i32_t) ms_left;
                                ref_ldt = ldt;
                                return true;
                            }
                        }
                    }
                }

                // 不可达
                return false;
            }

            void datetime::set(pilo::i64_t tick, datetime_precision_enum mode)
            {
                if (mode == edtp_Seconds)
                {
                    m_epoch = tick * 1000000;
                }
                else if (mode == edtp_Millisecond)
                {
                    m_epoch = tick * 1000;
                }
                else if (mode == edtp_Microsecond)
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


            pilo::i64_t datetime::calculate_year_initial_second_local(int year, ::pilo::i32_t timezone)
			{

                if (year > PMI_PILO_DATETIME_MAX_YEAR)
                {
                    return INT64_MIN;
                }
                else if (year < 1)
                {
                    return INT64_MIN;
                }

				const int startYear = 1970;
                const int endYear = startYear + PMI_DATETIME_YEAR_1ST_SECS_CACHE_SIZE;

				if (year >= endYear)
				{
                    ::pilo::i64_t secs = __cst_stc_year_1st_sec_cache[PMI_DATETIME_YEAR_1ST_SECS_CACHE_SIZE - 1];
                    if (timezone == INT_MAX)
                    {                        
                        secs += datetime::diff_seconds_local_to_utc();
                    }
                    else
                    {
                        secs += timezone;
                    }

                    
                    for (int i = PMI_DATETIME_YEAR_1ST_SECS_CACHE_MAX_YEAR; i < year; i++)
                    {
                        if (is_leap_year(i))
                        {
                            secs += (366 * 86400);
                        }
                        else
                        {
                            secs += (365 * 86400);
                        }
                    }
                    return secs;
                   
				}
                else if (year < startYear)
                {
                    ::pilo::i64_t secs = __cst_stc_year_1st_sec_cache[0];
                    if (timezone == INT_MAX)
                    {
                        secs += datetime::diff_seconds_local_to_utc();
                    }
                    else
                    {
                        secs += timezone;
                    }

                    for (int i = 1970; i > year; i--)
                    {
                        if (is_leap_year(i-1))
                        {
                            secs -= (366 * 86400);
                        }
                        else
                        {
                            secs -= (365 * 86400);
                        }
                    }
                    return secs;
                }
				else
				{
                    if (timezone == INT_MAX)
                    {
                        int diff = datetime::diff_seconds_local_to_utc();
                        return __cst_stc_year_1st_sec_cache[year - 1970] + diff;
                    }
                    else
                    {
                        return __cst_stc_year_1st_sec_cache[year - 1970] + timezone;
                    }                    
				}
			}

            pilo::i64_t datetime::calculate_day_initial_second_local(pilo::i64_t sec)
            {
                datetime dt(sec, edtp_Seconds);
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
                ::pilo::core::datetime::datetime dt(sec, edtp_Seconds);

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
                ::pilo::core::datetime::datetime dt(sec, edtp_Seconds);
                ::pilo::core::datetime::local_datetime ldt;
                dt.to_local_datetime(ldt);
                ldt.time.reset();
                dt.from_local_datetime(ldt);

                dt.add_days(1 - ldt.date.day);

                return (pilo::i64_t)dt.seconds_since_epoch();
            }

            pilo::i64_t datetime::calculate_next_day_initial_second(pilo::i64_t sec)
            {
                return ::pilo::core::datetime::datetime::calculate_day_initial_second_local(sec) + day_seconds;
            }

            pilo::i64_t datetime::calculate_next_week_initial_second(pilo::i64_t sec)
            {
                return ::pilo::core::datetime::datetime::calculate_week_initial_second(sec) + day_seconds * 7;
            }

            pilo::i64_t datetime::calculate_next_month_initial_second(pilo::i64_t sec)
            {
                ::pilo::core::datetime::datetime dt(sec, edtp_Seconds);
                ::pilo::core::datetime::local_datetime ldt;
                dt.to_local_datetime(ldt);
                ldt.time.reset();
                dt.from_local_datetime(ldt);
                dt.add_days(1 - ldt.date.day);

                int mdays = ::pilo::core::datetime::datetime::days_in_months(ldt.date.year, ldt.date.month);
                return ::pilo::core::datetime::datetime::calculate_day_initial_second_local(sec) + day_seconds*mdays;                
            }

            pilo::i64_t datetime::calculate_next_year_initial_second(pilo::i64_t sec)
            {
                ::pilo::core::datetime::datetime dt(sec, edtp_Seconds);
                ::pilo::core::datetime::local_datetime ldt;
                dt.to_local_datetime(ldt);

                return ::pilo::core::datetime::datetime::calculate_year_initial_second_local(ldt.date.year+1);
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
                int n = _get_timezone((long*)&t);
                if (n != 0)
                {
                    return -1;
                }
                return t;
#else
                return timezone;
#endif
            }

            ::pilo::i8_t datetime::days_in_months(int year, int month)
			{
                static ::pilo::i8_t  leapDays[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
                static ::pilo::i8_t  nonleapDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
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