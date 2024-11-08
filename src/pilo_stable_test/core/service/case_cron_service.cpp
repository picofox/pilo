﻿#include "../../pilo_stable_test.hpp"
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
#include "pilo/core/datetime/zoned_clock.hpp"
#include "pilo/core/container/sorted_deque.hpp"
#include "pilo/core/sched/cron.hpp"
#include <memory>
#include "pilo/core/sched/cron_resolving.hpp"
#include "pilo/core/datetime/stop_watch.hpp"


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
				const static ::pilo::i64_t s_cron_test_count = 5000;
				static ::pilo::i64_t s_results[s_cron_test_count];

				static void s_test(::pilo::core::sched::task* t)
				{
					if (::pilo::core::sched::cron::check_and_delete_job(t->context<void*>()) != PILO_OK) {
						printf("deletes\n");
						return;
					}					
					::pilo::i64_t idx = (::pilo::i64_t)t->param();
					if (s_results[idx] <= 2) {
						
						s_results[idx]++;

					}

				}
				

				
				int case_cron_service_base(::pilo::core::testing::stable_test_case* p_case)
				{
					::pilo::i64_t ids[s_cron_test_count] = { 0 };
					for (::pilo::i64_t i = 0; i < s_cron_test_count; i++) {
						ids[i] = PILO_CONTEXT->add_cron_job(8, "*/ * * * * ?", s_test, nullptr, (void *) i , nullptr);
						if (ids[i] <= 0) {
							return p_case->error(PERR_TESTCASE_FAIL, "create job failed %lld", i);
						}
					}

					while (true)
					{
						for (::pilo::i64_t i = 0; i < s_cron_test_count; i++) {
							if (s_results[i] == 2) {
								printf("job %lld - %lld should del", i, ids[i]);
								PILO_CONTEXT->delete_cron_job(ids[i]);
								s_results[i] = -1;
							} 							
						}

						bool alldone = true;
						alldone = true;
						for (::pilo::i64_t i = 0; i < s_cron_test_count; i++) {
							if (s_results[i] != -1) {
								alldone = false;
								break;
							}
						}
						if (alldone)
							break;
					}
					

					p_case->set_result(PILO_OK);
					return 0;
				}


			} //service
		} // core
	} // stable test
} // pilo