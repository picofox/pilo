#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/ml/json_tlv_driver.hpp"
#include "pilo/core/process/context.hpp"

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
					const char* str = "放浪no贵公子!";
					int len = (int)::pilo::core::string::character_count(str);
					for (int i = 0; i < len; i++) {
						printf("%02x ", (unsigned char) str[i]);
					}
					
					printf("\n");

					p_case->set_result(PILO_OK);
					return PILO_OK;
				}




			}
		}
	}
}