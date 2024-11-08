#include <mutex>
#include "datetime.hpp"
#include "./timestamp.hpp"
#include "../memory/bits_operation.hpp"
#include "../process/context.hpp"
#include <chrono>
#include <map>

namespace pilo
{
	namespace core
	{
		namespace datetime
		{ 
            const static std::map<std::string, ::pilo::u64_t> _cs_unit_map{
                std::pair<std::string, ::pilo::u64_t>("us", 1),
                std::pair<std::string, ::pilo::u64_t>("ms", 1000),
                std::pair<std::string, ::pilo::u64_t>("s", 1000000),
                std::pair<std::string, ::pilo::u64_t>("m", 60000000),
                std::pair<std::string, ::pilo::u64_t>("h", 3600000000),
            };

            const static ::pilo::i64_t __uct_stc_year_1st_sec_cache[] =
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

            ::pilo::u8_t days_of_months(::pilo::u8_t mon, bool is_leap)
            {
                if (mon == 4 || mon == 6 || mon == 9 || mon == 11) {
                    return 30;
                }
                else if (mon == 1 || mon == 3 || mon == 5 || mon == 7 || mon == 8 || mon == 10 || mon == 12) {
                    return 31;
                }
                else if (mon == 2){
                    if (is_leap) {
                        return 29;
                    }
                    else {
                        return 28;
                    }
                }

                return 0;
            }

            ::pilo::u64_t make_datetime(::pilo::u16_t year, ::pilo::u8_t mon, ::pilo::u8_t mday, ::pilo::u8_t hour, ::pilo::u8_t min, ::pilo::u8_t sec, ::pilo::u32_t usec, ::pilo::i8_t tz)
            {
                return 
                    ((((::pilo::u64_t) year) & PMI_DATETIME_YEAR_MASK )<< 51)
                        | ((((::pilo::u64_t) mon) & PMI_DATETIME_MON_MASK) << 47)
                        | ((((::pilo::u64_t) mday) & PMI_DATETIME_MDAY_MASK)  << 42)
                        | ((((::pilo::u64_t) hour) & PMI_DATETIME_HOUR_MASK) << 37)
                        | ((((::pilo::u64_t) min) & PMI_DATETIME_MIN_MASK) << 31)
                        | ((((::pilo::u64_t) sec) & PMI_DATETIME_SEC_MASK) << 25)
                        | ((((::pilo::u64_t) usec) & PMI_DATETIME_USEC_MASK) << 5)
                        | ((((::pilo::u64_t) (tz + 12)) & PMI_DATETIME_TZOFF_MASK));
            }
            ::pilo::u16_t year_of_datetime(::pilo::u64_t dtv)
            {
                return ((dtv >> 51) & PMI_DATETIME_YEAR_MASK);
            }
            ::pilo::u8_t month_of_datetime(::pilo::u64_t dtv)
            {
                return (::pilo::u8_t)((dtv >> 47) & PMI_DATETIME_MON_MASK);
            }
            ::pilo::u8_t mday_of_datetime(::pilo::u64_t dtv)
            {
                return (::pilo::u8_t)((dtv >> 42) & PMI_DATETIME_MDAY_MASK);
            }
            ::pilo::u8_t hour_of_datetime(::pilo::u64_t dtv)
            {
                return (::pilo::u8_t)((dtv >> 37) & PMI_DATETIME_HOUR_MASK);
            }
            ::pilo::u8_t min_of_datetime(::pilo::u64_t dtv)
            {
                return (::pilo::u8_t)((dtv >> 31) & PMI_DATETIME_MIN_MASK);
            }
            ::pilo::u8_t sec_of_datetime(::pilo::u64_t dtv)
            {
                return (::pilo::u8_t)((dtv >> 25) & PMI_DATETIME_SEC_MASK);
            }
            ::pilo::u32_t microsec_of_datetime(::pilo::u64_t dtv)
            {
                return (::pilo::u32_t)((dtv >> 5) & PMI_DATETIME_USEC_MASK);
            }
            ::pilo::i8_t timezone_hoff_of_datetime(::pilo::u64_t dtv)
            {
                return ((::pilo::i8_t)(dtv & PMI_DATETIME_TZOFF_MASK)) - 12;
            }
            bool set_year_of_datetime(::pilo::u64_t& dtv, ::pilo::u16_t year)
            {
                return ::pilo::core::memory::bits_operation::set_nbits(dtv, 0, 13, (::pilo::u64_t)year);
            }
            bool set_month_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t mon)
            {
                return ::pilo::core::memory::bits_operation::set_nbits(dtv, 13, 4, (::pilo::u64_t)mon);
            }
            bool set_mday_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t mday)
            {
                return ::pilo::core::memory::bits_operation::set_nbits(dtv, 17, 5, (::pilo::u64_t)mday);
            }
            bool set_hour_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t hour)
            {
                return ::pilo::core::memory::bits_operation::set_nbits(dtv, 22, 5, (::pilo::u64_t)hour);
            }
            bool set_min_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t min)
            {
                return ::pilo::core::memory::bits_operation::set_nbits(dtv, 27, 6, (::pilo::u64_t)min);
            }
            bool set_sec_of_datetime(::pilo::u64_t& dtv, ::pilo::u8_t sec)
            {
                return ::pilo::core::memory::bits_operation::set_nbits(dtv, 33, 6, (::pilo::u64_t)sec);
            }
            bool set_usec_of_datetime(::pilo::u64_t& dtv, ::pilo::u32_t usec)
            {
                return ::pilo::core::memory::bits_operation::set_nbits(dtv, 39, 20, (::pilo::u64_t)usec);
            }
            bool set_timezone_hoff_of_datetime(::pilo::u64_t& dtv, ::pilo::i8_t tzhoff)
            {
                return ::pilo::core::memory::bits_operation::set_nbits(dtv, 59, 5, (::pilo::u64_t) (tzhoff + 12));
            }
            const char* format_datetime_to_cstring(char* buffer, ::pilo::i64_t sz, const char* fmt, ::pilo::u64_t dtv)
            {        
                char tmp[10] = { 0 };

                if (::pilo::core::string::copyz(buffer, sz, fmt) < 0)
                    return nullptr;

                if (::pilo::core::string::find_substring(buffer, "%2tz", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%+02d", ::pilo::core::datetime::timezone_hoff_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%2tz", tmp, nullptr);
                }
                if (::pilo::core::string::find_substring(buffer, "%tz", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%+d", ::pilo::core::datetime::timezone_hoff_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%tz", tmp, nullptr);
                }

                if (::pilo::core::string::find_substring(buffer, "%6us", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%06u", ::pilo::core::datetime::microsec_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%6us", tmp, nullptr);
                }
                if (::pilo::core::string::find_substring(buffer, "%us", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%u", ::pilo::core::datetime::microsec_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%us", tmp, nullptr);
                }

                if (::pilo::core::string::find_substring(buffer, "%3ms", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%03u", ::pilo::core::datetime::microsec_of_datetime(dtv) / 1000);
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%3ms", tmp, nullptr);
                }
                if (::pilo::core::string::find_substring(buffer, "%ms", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%u", ::pilo::core::datetime::microsec_of_datetime(dtv) / 1000);
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%ms", tmp, nullptr);
                }


                if (::pilo::core::string::find_substring(buffer, "%ss", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%02u", ::pilo::core::datetime::sec_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%ss", tmp, nullptr);
                }
                if (::pilo::core::string::find_substring(buffer, "%mm", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%02u", ::pilo::core::datetime::min_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%mm", tmp, nullptr);
                }
                if (::pilo::core::string::find_substring(buffer, "%hh", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%02u", ::pilo::core::datetime::hour_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%hh", tmp, nullptr);
                }

                if (::pilo::core::string::find_substring(buffer, "%YY", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%02u", ::pilo::core::datetime::year_of_datetime(dtv) - 2000);
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%YY", tmp, nullptr);
                }
                if (::pilo::core::string::find_substring(buffer, "%MM", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%02u", ::pilo::core::datetime::month_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%MM", tmp, nullptr);
                }
                if (::pilo::core::string::find_substring(buffer, "%DD", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%02u", ::pilo::core::datetime::mday_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%DD", tmp, nullptr);
                }

                if (::pilo::core::string::find_substring(buffer, "%s", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%u", ::pilo::core::datetime::sec_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%s", tmp, nullptr);
                }                
                if (::pilo::core::string::find_substring(buffer, "%m", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%u", ::pilo::core::datetime::min_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%m", tmp, nullptr);
                }                
                if (::pilo::core::string::find_substring(buffer, "%h", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%u", ::pilo::core::datetime::hour_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%h", tmp, nullptr);
                }

                if (::pilo::core::string::find_substring(buffer, "%Y", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%04u", ::pilo::core::datetime::year_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%Y", tmp, nullptr);
                }
                if (::pilo::core::string::find_substring(buffer, "%M", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%0u", ::pilo::core::datetime::month_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%M", tmp, nullptr);
                }
                if (::pilo::core::string::find_substring(buffer, "%D", -1) != nullptr) {
                    ::pilo::core::io::string_formated_output(tmp, sizeof(tmp), "%0u", ::pilo::core::datetime::mday_of_datetime(dtv));
                    ::pilo::core::string::rescanable_replace_inplace(buffer, -1, sz, "%D", tmp, nullptr);
                }




                return buffer;
            }
            ::pilo::u64_t timestamp_to_datetime(::pilo::i64_t t, ::pilo::i8_t tz)
            {
                ::pilo::u64_t ret = PMI_INVALID_DATETIME;
                time_t sec_part = t / PMI_USECS_OF_SEC;
                time_t left = t - (sec_part * PMI_USECS_OF_SEC);
                struct  tm tms = { 0 };                
                if (tz == PMI_USE_SYSTEM_TIMEZONE) {
                    ::pilo::i8_t tt = PILO_CONTEXT->system_information()->system_timezone();
                    tz = tt;
                }
                else if (tz == PMI_USE_OVERRIDED_TIMEZONE) {
                    tz = PILO_CONTEXT->core_config()->overrided_timezone();
                }
                sec_part = sec_part + tz * 3600;
                xpf_get_gmtime(&tms, &sec_part);

                ret = make_datetime((::pilo::u16_t)tms.tm_year + 1900, (::pilo::u8_t)tms.tm_mon+1, (::pilo::u8_t)tms.tm_mday, (::pilo::u8_t)tms.tm_hour, (::pilo::u8_t)tms.tm_min, (::pilo::u8_t)tms.tm_sec, (::pilo::u32_t)left, (::pilo::i8_t)tz);
                return ret;
            }
            ::pilo::u64_t timestamp_to_datetime(const ::pilo::core::datetime::systime & systm, ::pilo::i8_t tz)
            {
                ::pilo::i64_t ts = std::chrono::duration_cast<std::chrono::microseconds>(systm.time_since_epoch()).count();
                return ::pilo::core::datetime::timestamp_to_datetime(ts, tz);
            }
            ::pilo::i64_t datetime_to_timestamp(::pilo::u64_t dtv, ::pilo::i8_t tz)
            {
                ::pilo::i64_t epoch_secs = ::pilo::core::datetime::first_second_of_year(::pilo::core::datetime::year_of_datetime(dtv), tz);

                ::pilo::u8_t mon = ::pilo::core::datetime::month_of_datetime(dtv);
                for (::pilo::u8_t i = 1; i < mon; i++)
                {
                    ::pilo::i64_t days = ::pilo::core::datetime::mdays_in_months(::pilo::core::datetime::year_of_datetime(dtv), i);
                    epoch_secs += days * 86400;
                }

                epoch_secs += ((::pilo::i64_t)::pilo::core::datetime::mday_of_datetime(dtv) - 1) * 86400;
                epoch_secs += ((::pilo::i64_t)::pilo::core::datetime::hour_of_datetime(dtv)) * 3600;
                epoch_secs += ((::pilo::i64_t)::pilo::core::datetime::min_of_datetime(dtv)) * 60;
                epoch_secs += ((::pilo::i64_t)::pilo::core::datetime::sec_of_datetime(dtv));
                ::pilo::i64_t epoch = epoch_secs * 1000000;
                epoch += ((::pilo::i64_t)::pilo::core::datetime::microsec_of_datetime(dtv));
                return epoch;
            }

            std::time_t datetime_to_timestamp_by_second(std::tm& date, ::pilo::i8_t tz)
            {
                ::pilo::u16_t year = (::pilo::u16_t) (date.tm_year + 1900);
                ::pilo::i64_t epoch_secs = ::pilo::core::datetime::first_second_of_year(year, tz);
                for (::pilo::u8_t i = 1; i < date.tm_mon + 1; i++)
                {
                    ::pilo::i64_t days = ::pilo::core::datetime::mdays_in_months(year, i);
                    epoch_secs += days * 86400;
                }
                epoch_secs += ((::pilo::i64_t) (date.tm_mday-1)) * 86400;
                epoch_secs += ((::pilo::i64_t) date.tm_hour) * 3600;
                epoch_secs += ((::pilo::i64_t) date.tm_min) * 60;
                epoch_secs += ((::pilo::i64_t) date.tm_sec);

                time_t t = epoch_secs + tz * PMI_SECS_OF_HOUR;
                ::pilo::core::datetime::xpf_get_gmtime(&date, &t);

                return epoch_secs;
            }

            ::pilo::i64_t datetime_to_timestamp(::pilo::u64_t dtv)
            {
                return ::pilo::core::datetime::datetime_to_timestamp(dtv, ::pilo::core::datetime::timezone_hoff_of_datetime(dtv));
            }
            bool is_leap_year(::pilo::u16_t year)
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


            ::pilo::i64_t first_second_of_year(::pilo::u16_t year, ::pilo::i8_t tzhoff)
            {
                if (year > PMI_DATETIME_YEAR_MAX)
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
                    ::pilo::i64_t secs = __uct_stc_year_1st_sec_cache[PMI_DATETIME_YEAR_1ST_SECS_CACHE_SIZE - 1];
                    secs -= (::pilo::i64_t)tzhoff * 3600;
                    for (::pilo::u16_t i = PMI_DATETIME_YEAR_1ST_SECS_CACHE_MAX_YEAR; i < year; i++)
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
                    ::pilo::i64_t secs = __uct_stc_year_1st_sec_cache[0];
                    secs -= (::pilo::i64_t) tzhoff * 3600;

                    for (::pilo::i16_t i = 1970; i > year; i--)
                    {
                        if (is_leap_year(i - 1))
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
                    return __uct_stc_year_1st_sec_cache[year - 1970] - ((::pilo::i64_t)tzhoff) * 3600;
                }
            }
            ::pilo::i64_t first_second_of_day(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff)
            {
                return first_microsecond_of_day(usec_ts, tzhoff) / PMI_USECS_OF_SEC;
            }
            ::pilo::i64_t first_second_of_week(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff)
            {
                return ::pilo::core::datetime::first_microsecond_of_week(usec_ts, tzhoff) / PMI_USECS_OF_SEC;
            }
            ::pilo::i64_t first_second_of_month(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff)
            {
                return ::pilo::core::datetime::first_microsecond_of_month(usec_ts, tzhoff) / PMI_USECS_OF_SEC;
            }
            ::pilo::i64_t first_second_of_hour(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff)
            {
                return ::pilo::core::datetime::first_microsecond_of_hour(usec_ts, tzhoff) / PMI_USECS_OF_SEC;
            }
            ::pilo::i64_t first_microsecond_of_day(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff)
            {
                usec_ts += PMF_TZ_USEC_OFF(tzhoff);
                ::pilo::i64_t remain = usec_ts % PMI_USECS_OF_DAY;
                return ((usec_ts - remain) - PMF_TZ_USEC_OFF(tzhoff));
            }
            ::pilo::i64_t first_microsecond_of_week(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff)
            {
                ::pilo::i64_t wd = ::pilo::core::datetime::week_day(usec_ts, tzhoff);
                ::pilo::i64_t day1st = ::pilo::core::datetime::first_microsecond_of_day(usec_ts, tzhoff);
                ::pilo::i64_t remain = usec_ts - day1st;
                ::pilo::i64_t off_from_week_initial = remain + wd * PMI_USECS_OF_DAY;
                return usec_ts - off_from_week_initial;
            }
            ::pilo::i64_t first_microsecond_of_month(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff)
            {
                ::pilo::i64_t dtv = ::pilo::core::datetime::timestamp_to_datetime(usec_ts, tzhoff);
                return ::pilo::core::datetime::first_microsecond_of_month(dtv);
            }
            ::pilo::i64_t first_microsecond_of_hour(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff)
            {
                ::pilo::i64_t today_ts = ::pilo::core::datetime::first_microsecond_of_day(usec_ts, tzhoff);
                ::pilo::u64_t dtv = ::pilo::core::datetime::timestamp_to_datetime(usec_ts, tzhoff);
                ::pilo::i64_t hours_off = ::pilo::core::datetime::hour_of_datetime(dtv);
                return today_ts + hours_off * PMI_USECS_OF_HOUR;
            }
            ::pilo::i64_t first_second_of_month(::pilo::u64_t dtv)
            {
                return ::pilo::core::datetime::first_microsecond_of_month(dtv) / PMI_USECS_OF_SEC;
            }
            ::pilo::i64_t first_microsecond_of_month(::pilo::u64_t dtv)
            {
                ::pilo::u64_t tmp_dtv = ::pilo::core::datetime::make_datetime(
                    ::pilo::core::datetime::year_of_datetime(dtv),
                    ::pilo::core::datetime::month_of_datetime(dtv),
                    1, 0, 0, 0, 0, ::pilo::core::datetime::timezone_hoff_of_datetime(dtv)
                );
                return ::pilo::core::datetime::datetime_to_timestamp(tmp_dtv);
            }
            ::pilo::i64_t first_second_of_next_days(::pilo::i64_t ts, ::pilo::i8_t tz, ::pilo::i64_t ndays)
            {
                return first_microsecond_of_next_days(ts, tz, ndays);
            }
            ::pilo::i64_t first_second_of_next_hours(::pilo::i64_t ts, ::pilo::i8_t tz, ::pilo::i64_t nhours)
            {
                return ::pilo::core::datetime::first_microsecond_of_next_hours(ts, tz, nhours) / PMI_USECS_OF_SEC;
            }
            ::pilo::i64_t first_microsecond_of_next_days(::pilo::i64_t ts, ::pilo::i8_t tz, ::pilo::i64_t ndays)
            {
                ::pilo::i64_t ret = ::pilo::core::datetime::first_microsecond_of_day(ts, tz);
                return ret + ndays * PMI_USECS_OF_DAY;
            }
            ::pilo::i64_t first_microsecond_of_next_hours(::pilo::i64_t ts, ::pilo::i8_t tz, ::pilo::i64_t nhours)
            {
                ::pilo::i64_t ret = ::pilo::core::datetime::first_microsecond_of_hour(ts, tz);
                return ret + nhours * PMI_USECS_OF_HOUR;
            }

            static ::pilo::u64_t leading_fraction(::pilo::f64_t& scale, char** rem ,const char* cstr) 
            {
                ::pilo::u64_t x = 0;
                int i = 0;
                scale = 1;
                bool overflow = false;
                ::pilo::i64_t len = ::pilo::core::string::character_count(cstr);
                for (; i < len; i++) {
                    char c = cstr[i];
                    if (c < '0' || c > '9') {
                        break;
                    }
                    if (overflow) {
                        continue;
                    }

                    if (x > ((1ULL << 63) - 1) / 10) {
                        // It's possible for overflow to give a positive number, so take care.
                        overflow = true;
                        continue;
                    }
                    ::pilo::u64_t y = x * 10 + (::pilo::u64_t)c - '0';
                    if (y > (1ULL << 63)) {
                        overflow = true;
                        continue;
                    }
                    x = y;
                    scale *= 10;
                }

                if (rem != nullptr)
                    *rem = ((char*)cstr) + i;

                return x;
            }


            ::pilo::i64_t microsecond_from_cstr(const char* cstr, ::pilo::i64_t len)
            {
                char* tmp_cstr_ptr = nullptr;
                if (len < 0)
                    len = ::pilo::core::string::character_count(cstr);
                // [-+]?([0-9]*(\.[0-9]*)?[a-z]+)+
                ::pilo::u64_t d = 0;
                bool neg = false;

                // Consume [-+]?
                if (len > 0) {
                    char c = cstr[0];
                    if (c == '-' || c == '+') {
                        neg = (c == '-');
                        cstr++;
                    }
                }
                len = ::pilo::core::string::character_count(cstr);
                // Special case: if all that is left is "0", this is zero.
                if (cstr[0] == '0' && cstr[1] == 0) {
                    return 0;
                }
                if (len <= 0)
                    return -1;

                while (*cstr != 0) {
                    ::pilo::u64_t v = 0;
                    ::pilo::u64_t f = 0;
                    ::pilo::f64_t scale = 1;

                    // The next character must be [0-9.]
                    if (!(cstr[0] == '.' || '0' <= cstr[0] && cstr[0] <= '9')) {
                        return -1;
                    }
                    // Consume [0-9]*
                    len = ::pilo::core::string::character_count(cstr);
                    v = ::strtoll(cstr, &tmp_cstr_ptr, 10);
                    cstr = tmp_cstr_ptr;
                    bool pre = false;
                    ::pilo::i64_t old_len = len;
                    len = ::pilo::core::string::character_count(cstr);
                    if (old_len != len)
                        pre = true;
                    else
                        pre = false;

                    // Consume (\.[0-9]*)?
                    bool post = false;               
                    if (len > 0 && cstr[0] == '.') {
                        cstr++;
                        len = ::pilo::core::string::character_count(cstr);
                        f = leading_fraction(scale, &tmp_cstr_ptr, cstr);
                        cstr = tmp_cstr_ptr;
                        old_len = len;
                        len = ::pilo::core::string::character_count(cstr);
                        if (old_len != len)
                            post = true;
                        else
                            post = false;          
                    }
                    if (!pre && !post){
                        // no digits (e.g. ".s" or "-.s")
                        return -1;
                    }

                    // Consume unit.
                    ::pilo::i64_t i = 0;
                    len = ::pilo::core::string::character_count(cstr);
                    for (; i < len; i++) {
                        char c = cstr[i];
                        if ((c == '.') || '0' <= c && c <= '9') {
                            break;
                        }
                    }
                    if (i == 0) {
                        return -1;
                    }
                    std::string u(cstr, i);
                    cstr = cstr + i;
                    std::map<std::string, ::pilo::u64_t>::const_iterator cit = _cs_unit_map.find(u);
                    if (cit == _cs_unit_map.cend()) {
                        return -1;
                    }
                    ::pilo::u64_t unit = cit->second;
                    if (v > ((1ULL << 63) / unit)) {
                        return -1;
                    }
                    v *= unit;
                    if (f > 0) {
                        // float64 is needed to be nanosecond accurate for fractions of hours.
                        // v >= 0 && (f*unit/scale) <= 3.6e+12 (ns/h, h is the largest unit)
                        v += (::pilo::u64_t)(((::pilo::f64_t)f) * ((::pilo::f64_t)unit / scale));
                        if (v > (1ULL << 63)) {
                            return -1;
                        }
                    }
                    d += v;
                    if (d > ((1ULL << 63))) {
                        return -1;
                    }                
                }

                if (neg) {
                    return 0 - d;
                }
                if (d > ((1ULL << 63) - 1)) {
                    return -1;
                }
                return d;
            }





            ::pilo::i64_t ceil_by_second(::pilo::i64_t ts)
            {
                return ts += PMI_USECS_OF_SEC - (ts % PMI_USECS_OF_SEC);
            }

            void ceil_by_second(::pilo::i64_t* ts)
            {
                *ts = *ts + PMI_USECS_OF_SEC - (*ts % PMI_USECS_OF_SEC);
            }

            ::pilo::u8_t week_day(::pilo::i64_t usec_ts, ::pilo::i8_t tzhoff)
            {
                ::pilo::i64_t ts = usec_ts;
                ts = ts + PMF_TZ_USEC_OFF(tzhoff);
                ::pilo::i64_t days = ts / PMI_USECS_OF_DAY;
                return  (::pilo::i8_t)((days + 4) % 7);
            }
            ::pilo::u8_t mdays_in_months(::pilo::u16_t year, ::pilo::u8_t month)
            {
                static ::pilo::i8_t  leapDays[] = { 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
                static ::pilo::i8_t  nonleapDays[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
                if (::pilo::core::datetime::is_leap_year(year))
                {
                    return leapDays[month - 1];
                }
                else
                {
                    return nonleapDays[month - 1];
                }
            }
}
	}
}