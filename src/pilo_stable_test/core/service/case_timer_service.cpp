#include "../../pilo_stable_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/threading/efficient_thread_pool.hpp"
#include "pilo/core/threading/threading.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/timer.hpp"
#include "pilo/core/algorithm/uint_sequence_generator.hpp"
#include <limits>

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

					::pilo::core::algorithm::uint_sequence_generator<::pilo::i64_t> idgen(0);

					for (int i = 0; i < 10; i++) {
						printf("%08I64x\n", idgen.next());
					}


					p_case->set_result(PILO_OK);
					return PILO_OK;
				}

			} //service
		} // core
	} // stable test
} // pilo