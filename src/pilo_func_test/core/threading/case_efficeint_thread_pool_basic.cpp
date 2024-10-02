#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/threading/efficient_thread_pool.hpp"
#include "pilo/core/threading/threading.hpp"
#include "pilo/core/process/context.hpp"

using namespace ::pilo::func_test;

#define PILO_FBB_TEST_CNT_0 (100000)

namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace threading
			{
				static void start_cb(::pilo::core::threading::thread_pool_worker_interface* )
				{
					printf("start_cb\n");
				}
				static void run_cb(::pilo::core::threading::thread_pool_worker_interface* worker)
				{
					printf("%s running\n", worker->name().c_str());
				}
				static void stop_cb(::pilo::core::threading::thread_pool_worker_interface* worker)
				{
					printf("%s _on_cleaning\n", worker->name().c_str());
				}

				int case_efficeint_thread_pool_basic(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::err_t err = PILO_OK;
					const ::pilo::core::config::thread_pool_config& cfg = PILO_CONTEXT->core_config()->thread_pool();
					::pilo::core::threading::thread_pool_interface *pool =  new ::pilo::core::threading::efficient_thread_pool(&cfg, start_cb, run_cb, stop_cb);
					if (pool == nullptr) {
						return p_case->error(PERR_CREATE_OBJ_FAIL, "Create efficient_thread_pool failed");
					}

					err = pool->start();



					::pilo::core::threading::xpf_sleep(10);

					pool->stop();
					p_case->set_result(PILO_OK);
					return PILO_OK;
				}




			}
		}
	}
}