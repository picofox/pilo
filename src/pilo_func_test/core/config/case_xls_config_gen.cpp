#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/ml/json_tlv_driver.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/config/xls_config.hpp"

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
				int case_xls_config_gen(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::core::config::xls_config_generator xconf_gen;
					::pilo::err_t xret = xconf_gen.set("test_xls", ::pilo::predefined_pilo_path::tmp
						, "test_xls/json_conf/server", ::pilo::predefined_pilo_path::tmp
						, "test_xls/json_conf/client", ::pilo::predefined_pilo_path::tmp
						, "test_xls/conf_src/server", ::pilo::predefined_pilo_path::tmp
						, "test_xls/conf_src/client", ::pilo::predefined_pilo_path::tmp);
					if (xret != PILO_OK) {
						return p_case->error(xret, xconf_gen.logs().back().to_string().c_str());
					}

					xret = xconf_gen.parse();
					if (xret != PILO_OK) {
						for (size_t i = 0; i < xconf_gen.logs().size(); i++) {
							printf("%s\n", xconf_gen.logs().at(i).to_string().c_str());
						}
					}

					for (size_t i = 0; i < xconf_gen.logs().size(); i++) {
						printf("%s\n", xconf_gen.logs().at(i).to_string().c_str());
					}

					p_case->set_result(PILO_OK);
					return PILO_OK;
				}




			}
		}
	}
}