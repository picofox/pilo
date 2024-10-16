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

				//static void run_cb(::pilo::core::threading::thread_pool_worker_interface* )
				//{
				//	//printf("%s running\n", worker->name().c_str());
				//}


				//static void tsr(::pilo::core::sched::task * t)
				//{
				//	::pilo::core::testing::stable_test_case* p_case = (::pilo::core::testing::stable_test_case*) (t->object());


				//	PILO_CONTEXT->deallocate_task(t);
				//	p_case->inc_trans_count();
				//}

				//static void s_dtor(::pilo::core::sched::task* task_ptr)
				//{
				//	if (task_ptr->param() != nullptr)
				//	{
				//		::pilo::tlv::deallocate((::pilo::tlv*)task_ptr->param());
				//	}
				//}

				int case_effcient_thread_pool_global_queue(::pilo::core::testing::stable_test_case* )
				{
					//::pilo::tlv* param = nullptr;
					//::pilo::core::config::thread_pool_config config;
					//config.set(false, true, 6, 2, 5000, 1000, "test_etp_gq");
					//::pilo::core::threading::thread_pool_interface* pool = new ::pilo::core::threading::efficient_thread_pool(&config, nullptr, run_cb, nullptr);
					//p_case->set_begin();
					//pool->start();

					//for (::pilo::i64_t i = 0; i < p_case->target_trans_count(); i++) {
					//	param = ::pilo::tlv::allocate_single(i);
					//	::pilo::core::sched::task* t = PILO_CONTEXT->allocate_task(tsr, p_case, param, nullptr, s_dtor);
					//	pool->post_task(t);
					//}


					//pool->stop();
					//if (p_case->target_trans_count() != p_case->trans_count()) {
					//	return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "count not meet %lld / %lld", p_case->trans_count(), p_case->target_trans_count());
					//}

					//if (!pool->stopped()) {
					//	return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "thread pool not stop");
					//}

					//p_case->set_result(PILO_OK);
					//char buffer[128] = { 0 };
					//::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%f", p_case->persist_seconds());
					//p_case->set_desc(buffer);
					return PILO_OK;
				}

				int case_effcient_thread_pool_global_nqueue(::pilo::core::testing::stable_test_case* )
				{
					//::pilo::tlv* param = nullptr;
					//::pilo::core::config::thread_pool_config config;
					//config.set(false, false, 6, 2, 5000, 1000, "test_etp_gq");
					//::pilo::core::threading::thread_pool_interface* pool = new ::pilo::core::threading::efficient_thread_pool(&config, nullptr, run_cb, nullptr);
					//p_case->set_begin();
					//pool->start();

					//for (::pilo::i64_t i = 0; i < p_case->target_trans_count(); i++) {
					//	param = ::pilo::tlv::allocate_single(i);
					//	::pilo::core::sched::task* t = PILO_CONTEXT->allocate_task(tsr, p_case, param, nullptr, s_dtor);
					//	pool->post_task(t);
					//}



					//pool->stop();
					//if (p_case->target_trans_count() != p_case->trans_count()) {
					//	return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "count not meet %lld / %lld", p_case->trans_count(), p_case->target_trans_count());
					//}

					//if (!pool->stopped()) {
					//	return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "thread pool not stop");
					//}
					//p_case->set_result(PILO_OK);
					//char buffer[128] = { 0 };
					//::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%f", p_case->persist_seconds());
					//p_case->set_desc(buffer);
					return PILO_OK;
				}

			} //io
		} // core
	} // stable test
} // pilo