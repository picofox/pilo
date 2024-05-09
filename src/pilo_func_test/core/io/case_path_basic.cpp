#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/io/path.hpp"


using namespace ::pilo::func_test;

#define PILO_FBB_TEST_CNT_0 (100000)

namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace io
			{
				int case_get_paths(::pilo::core::testing::func_test_case* p_case)
				{
					char buffer[1024] = { 0 };
					::pilo::pathlen_t rplen = 0;
					char * ret = ::pilo::core::io::path::get_cwd(buffer, sizeof(buffer), &rplen, ::pilo::core::io::path::sepmode_needsep, 0);
					if (ret == nullptr)
					{
						return p_case->error(::pilo::make_core_error(PES_GETCWD, PEP_IS_NULL), "get cwd failed");
					}

					printf("len is %u\n", rplen);
					printf("\n%s\n", ret);


					p_case->set_result(PILO_OK);
					return 0;
				}
			}
		}
	}
}