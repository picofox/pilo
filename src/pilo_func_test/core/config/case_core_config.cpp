#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/ml/json_tlv_driver.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/config/core_config.hpp"

#include "pilo/core/logging/logger_interface.hpp"
#include "pilo/core/process/process.hpp"

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
				::pilo::err_t pritnenv(const char* key, ::pilo::i32_t key_len, const char* val, ::pilo::i32_t , void* )
				{
					std::string k(key, key_len);
					printf("%s -> [%s]\n",k.c_str(), val);
					return PILO_OK;
				}

				int case_core_config(::pilo::core::testing::func_test_case* p_case)
				{

					::pilo::core::process::xpf_iterate_enviroment_variable(pritnenv, nullptr, false);



					p_case->set_result(PILO_OK);

					return PILO_OK;
				}




			}
		}
	}
}