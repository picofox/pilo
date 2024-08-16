#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/io/path.hpp"
#include "pilo/core/i18n/encoding_conversion.hpp"
#include <iostream>
#include "pilo/core/process/context.hpp"
#include "pilo/core/testing/case_utils.hpp"
#include "pilo/core/memory/util.hpp"
#include "pilo/core/logging/local_text_logger.hpp"

using namespace ::pilo::func_test;

#define PILO_FBB_TEST_CNT_0 (100000)

namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace logging
			{
				int case_text_logger_basic(::pilo::core::testing::func_test_case* p_case)
				{
					for (int i = 0; i < 200; i++) {
						PILO_CONTEXT->logger(0)->log(::pilo::core::logging::level::error, "%s index is %d", "server starting", i);
					}

					

					p_case->set_result(PILO_OK);
					return PILO_OK;
				}




			}
		}
	}
}