#include "../../pilo_stable_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/threading/efficient_thread_pool.hpp"
#include "pilo/core/threading/threading.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/sched/timer.hpp"
#include "pilo/core/algorithm/uint_sequence_generator.hpp"
#include <limits>
#include "pilo/core/sched/time_wheel.hpp"
#include "pilo/core/threading/spin_mutex.hpp"
#include "pilo/core/datetime/datetime.hpp"
#include "pilo/core/datetime/system_clock.hpp"

using namespace ::pilo::stable_test;
using namespace ::pilo::core::testing;

namespace pilo
{
	namespace stable_test
	{
		namespace core
		{
			namespace service
			{
				
				int case_cron_service_base(::pilo::core::testing::stable_test_case* p_case)
				{
					::pilo::core::datetime::systime t;
					t = std::chrono::system_clock::now();
					
					std::string str = ::pilo::core::datetime::system_clock::format_to_string(t, nullptr);


					p_case->set_result(PILO_OK);
					return 0;
				}


			} //service
		} // core
	} // stable test
} // pilo