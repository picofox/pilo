#ifndef _pilo_core_sched_cron_spec_hpp_
#define _pilo_core_sched_cron_spec_hpp_

#include <map>
#include "../../pilo.hpp"
#include "../datetime/timestamp.hpp"
#include "../datetime/datetime.hpp"
#include "../datetime/system_clock.hpp"

namespace pilo
{
    namespace core
    {
        namespace sched
        {
            // spec_schedule specifies a duty cycle (to the second granularity), based on a
            // spec_schedule crontab specification. It is computed initially and stored as bit sets.
            class spec_schedule
            {
            public:
                spec_schedule() : _second(0), _minute(0), _hour(0), _dom(0), _month(0), _dow(0) {}
                spec_schedule(::pilo::u64_t sec, ::pilo::u64_t min, ::pilo::u64_t hour, ::pilo::u64_t dom, ::pilo::u64_t mon, ::pilo::u64_t dow)
                    : _second(sec), _minute(min), _hour(hour), _dom(dom), _month(mon), _dow(dow) {}

                ::pilo::u64_t second() const { return _second; }
                ::pilo::u64_t minute() const { return _minute; }
                ::pilo::u64_t hour() const { return _hour; }
                ::pilo::u64_t dom() const { return _dom; }
                ::pilo::u64_t month() const { return _month; }
                ::pilo::u64_t dow() const { return _dow; }

                void set_second(::pilo::u64_t second) { _second = second; }
                void set_minute(::pilo::u64_t minute) { _minute = minute; }
                void set_hour(::pilo::u64_t hour) { _hour = hour; }
                void set_dom(::pilo::u64_t dom) { _dom = dom; }
                void set_month(::pilo::u64_t month) { _month = month; }
                void set_dow(::pilo::u64_t dow) { _dow = dow; }

            public:                
                ::pilo::core::datetime::systime next(const ::pilo::core::datetime::systime& t);

            private:
                ::pilo::u64_t           _second;
                ::pilo::u64_t           _minute;
                ::pilo::u64_t           _hour;
                ::pilo::u64_t           _dom;
                ::pilo::u64_t           _month;
                ::pilo::u64_t           _dow;
            };

            // bounds provides a range of acceptable values (plus a map of name to value).
            class bounds
            {
            public:
                bounds() : _mininum(0), _maximum(0) {};
                bounds(::pilo::u32_t min, ::pilo::u32_t max, std::map<std::string, ::pilo::u32_t> names) : _mininum(min), _maximum(max) {};
                bounds(::pilo::u32_t min, ::pilo::u32_t max) : _mininum(min), _maximum(max) 
                {
                    _names.clear();
                };

                ::pilo::u32_t mininum() const { return _mininum; }
                ::pilo::u32_t maximum() const { return _maximum; }
                const std::map<std::string, ::pilo::u32_t> names() const { return _names; }

                void set_mininum(::pilo::u32_t v) { _mininum = v; }
                void set_maximum(::pilo::u32_t v) { _maximum = v; }

            private:
                ::pilo::u32_t                           _mininum;
                ::pilo::u32_t                           _maximum;
                std::map<std::string, ::pilo::u32_t>    _names;
            };

            const static bounds second_bound{ 0, 59 };
            const static bounds minute_bound{ 0, 59 };
            const static bounds hour_bound{ 0, 23 };
            const static bounds dom_bound{ 1, 31 };
            const static bounds months_bound{ 1, 12, {
                std::pair<std::string, ::pilo::u32_t>(std::string("jan"), 1),
                std::pair<std::string, ::pilo::u32_t>(std::string("feb"), 2),
                std::pair<std::string, ::pilo::u32_t>(std::string("mar"), 3),
                std::pair<std::string, ::pilo::u32_t>(std::string("apr"), 4),
                std::pair<std::string, ::pilo::u32_t>(std::string("may"), 5),
                std::pair<std::string, ::pilo::u32_t>(std::string("jun"), 6),
                std::pair<std::string, ::pilo::u32_t>(std::string("jul"), 7),
                std::pair<std::string, ::pilo::u32_t>(std::string("aug"), 8),
                std::pair<std::string, ::pilo::u32_t>(std::string("sep"), 9),
                std::pair<std::string, ::pilo::u32_t>(std::string("oct"), 10),
                std::pair<std::string, ::pilo::u32_t>(std::string("nov"), 11),
                std::pair<std::string, ::pilo::u32_t>(std::string("dec"), 12),
            } };
            const static bounds dow_bound{ 0, 6, {
                std::pair<std::string, ::pilo::u32_t>(std::string("sun"), 0),
                std::pair<std::string, ::pilo::u32_t>(std::string("mon"), 1),
                std::pair<std::string, ::pilo::u32_t>(std::string("tue"), 2),
                std::pair<std::string, ::pilo::u32_t>(std::string("wed"), 3),
                std::pair<std::string, ::pilo::u32_t>(std::string("thu"), 4),
                std::pair<std::string, ::pilo::u32_t>(std::string("fri"), 5),
                std::pair<std::string, ::pilo::u32_t>(std::string("sat"), 6),    
            } };

            const static ::pilo::u64_t star_bit = 1ULL << 63;   

        }
    }
}


#endif