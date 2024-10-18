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

#define	PMI_TEST_TIMER_CNT	(100000)
#define	PMI_TEST_TIMER_REP_CNT	(4)
#define	PMI_TEST_TIMER_THR_CNT	(5)



namespace pilo
{
	namespace stable_test
	{
		namespace core
		{
			namespace service
			{
				static ::std::atomic<::pilo::i64_t> s_cnt = 0;
				static void timer_task(::pilo::core::sched::task* )
				{
					//::pilo::core::sched::timer* ptimer_ref = task_ptr->context<::pilo::core::sched::timer>();
					//PLOG(::pilo::core::logging::level::info, "timer due: %s", ptimer_ref->to_string().c_str());
					s_cnt++;					
				}



				static void s_tsr_add_timer()
				{
					for (int j = 0; j < PMI_TEST_TIMER_CNT; j++) {
						::pilo::i64_t tid = PILO_CONTEXT->add_rel_milli_timer(1000, PMI_TEST_TIMER_REP_CNT, 1000, timer_task, nullptr, nullptr, nullptr);
						if (tid < 0) {
							PLOG(::pilo::core::logging::level::info, "add timer failed id=%lld", tid);
						}
					}					
				}

				int case_timer_service_milli_single(::pilo::core::testing::stable_test_case* p_case)
				{
					s_cnt.store(0);
					for (int i = 0; i < p_case->target_trans_count(); i++) {									
						::pilo::i64_t tid = PILO_CONTEXT->add_rel_milli_timer(10, PMI_TEST_TIMER_REP_CNT, 10, timer_task, nullptr, nullptr, nullptr);
						if (tid < 0) {
							PLOG(::pilo::core::logging::level::info, "add timer failed id=%lld", tid);
						}
					}

					for (;;) {
						if (s_cnt == p_case->target_trans_count() * (PMI_TEST_TIMER_REP_CNT + 1)) {
							break;
						}
						::pilo::core::threading::xpf_sleep(1);
					}


					p_case->set_result(PILO_OK);
					return 0;
				}

				int case_timer_service_milli_multi(::pilo::core::testing::stable_test_case* p_case)
				{
					std::vector<::pilo::core::threading::auto_join_thread> threads;

					s_cnt.store(0);

					for (int i = 0; i < PMI_TEST_TIMER_THR_CNT; i++) {						
						::pilo::core::threading::auto_join_thread	thr(s_tsr_add_timer);
						threads.emplace_back(std::move(thr));
					}

					for (int i = 0; i < PMI_TEST_TIMER_THR_CNT; i++) {
						threads[i].join();
					}
	
					for (;;) {
						if (s_cnt == PMI_TEST_TIMER_CNT * (PMI_TEST_TIMER_REP_CNT + 1) * PMI_TEST_TIMER_THR_CNT) {
							break;
						}
						::pilo::core::threading::xpf_sleep(1);
					}

					p_case->set_result(PILO_OK);
					return 0;					
				}

			} //service
		} // core
	} // stable test
} // pilo