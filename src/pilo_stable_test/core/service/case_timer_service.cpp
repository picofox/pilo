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
				int case_timer_service(::pilo::core::testing::stable_test_case* p_case)
				{
					PILO_CONTEXT->start();

					::pilo::core::service::service_manager* mgr = PILO_CONTEXT->service_manger();
					if (mgr == nullptr) {
						return -1;
					}

					p_case->set_result(PILO_OK);
					return 0;
					
				}

			} //service
		} // core
	} // stable test
} // pilo