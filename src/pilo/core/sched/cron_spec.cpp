#include "./cron_spec.hpp"

namespace pilo
{
    namespace core
    {
        namespace sched
        {
            ::pilo::core::datetime::systime spec_schedule::next(const ::pilo::core::datetime::systime& )
            {
                // General approach:
                // For Month, Day, Hour, Minute, Second:
                // Check if the time value matches.  If yes, continue to the next field.
                // If the field doesn't match the schedule, then increment the field until it matches.
                // While incrementing the field, a wrap-around brings it back to the beginning
                // of the field list (since it is necessary to re-verify previous field
                // values)

                // Start at the earliest possible time (the upcoming second).
                return ::pilo::core::datetime::systime();
            }
        }
    }
}