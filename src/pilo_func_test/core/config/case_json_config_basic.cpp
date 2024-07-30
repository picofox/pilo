#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/config/json_configuator.hpp"

using namespace ::pilo::func_test;

#define PILO_FBB_TEST_CNT_0 (100000)

namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace config
			{
				int case_json_config_basic(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::core::config::json_configuator jct;

					::pilo::core::io::path p("f:\\xinguanghemei_bak.rar");
					jct.set_file(&p);

					::pilo::err_t r = jct.load();
					if (r == PILO_OK)
						return 0;



					p_case->set_result(PILO_OK);
					return PILO_OK;
				}




			}
		}
	}
}