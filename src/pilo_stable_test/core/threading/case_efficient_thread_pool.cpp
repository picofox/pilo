#include "../../pilo_stable_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/threading/efficient_thread_pool.hpp"
#include "pilo/core/threading/threading.hpp"
#include "pilo/core/process/context.hpp"

using namespace ::pilo::stable_test;
using namespace ::pilo::core::testing;


namespace pilo
{
	namespace stable_test
	{
		namespace core
		{
			namespace threading
			{				
				static void start_cb(::pilo::core::threading::thread_pool_worker_interface*)
				{
					
				}
				static void run_cb(::pilo::core::threading::thread_pool_worker_interface* )
				{
					//printf("%s running\n", worker->name().c_str());
				}
				static void stop_cb(::pilo::core::threading::thread_pool_worker_interface* )
				{
					//printf("%s _on_cleaning\n", worker->name().c_str());
				}

				static void tsr(void* gen_obj_ptr, ::pilo::tlv* in_param)
				{
					::pilo::core::testing::stable_test_case* p_case = (::pilo::core::testing::stable_test_case*)gen_obj_ptr;		


					p_case->inc_trans_count();
				}

				int case_effcient_thread_pool_global_queue(::pilo::core::testing::stable_test_case* p_case)
				{
					::pilo::core::config::thread_pool_config config;
					config.set(false, true, 6, 2, 5000, 1000, "test_etp_gq");
					::pilo::core::threading::thread_pool_interface* pool = new ::pilo::core::threading::efficient_thread_pool(&config, start_cb, run_cb, stop_cb);

					pool->start();

					for (::pilo::i64_t i = 0; i < p_case->target_trans_count(); i++) {
						::pilo::task* t = PILO_CONTEXT->allocate_task(tsr, p_case, nullptr, nullptr, PMB_TASK_DEFAULT_FLAG);
						pool->post_task(t);
					}



					pool->stop();
					if (!pool->stopped()) {
						p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "thread pool not stop");
					}
					p_case->set_result(PILO_OK);
					return PILO_OK;
				}

				int case_effcient_thread_pool_global_nqueue(::pilo::core::testing::stable_test_case* p_case)
				{
					::pilo::core::config::thread_pool_config config;
					config.set(false, false, 6, 2, 5000, 1000, "test_etp_gq");
					::pilo::core::threading::thread_pool_interface* pool = new ::pilo::core::threading::efficient_thread_pool(&config, start_cb, run_cb, stop_cb);

					pool->start();

					for (::pilo::i64_t i = 0; i < p_case->target_trans_count(); i++) {
						::pilo::task* t = PILO_CONTEXT->allocate_task(tsr, p_case, nullptr, nullptr, PMB_TASK_DEFAULT_FLAG);
						pool->post_task(t);
					}



					pool->stop();
					if (!pool->stopped()) {
						p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "thread pool not stop");
					}
					p_case->set_result(PILO_OK);
					return PILO_OK;
				}

			} //io
		} // core
	} // stable test
} // pilo