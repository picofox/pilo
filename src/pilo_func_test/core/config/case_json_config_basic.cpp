#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/config/json_configuator.hpp"
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
					::pilo::core::config::json_configuator jct;
					std::string fn = "/cnf/";
					fn += PILO_CONTEXT->process_basename();
					fn += ".json";

					::pilo::core::io::path p(fn.c_str(), ::pilo::predefined_pilo_dir_enum::tmp);
					jct.set_file(&p);

					::pilo::err_t err = jct.load();
					if (err != PILO_OK) {
						return p_case->error(err, "load json file %s failed.", fn.c_str());
					}
					//	
					//std::string str = jct.root()->to_string();
					//printf("(%s)\n", str.c_str());

					::pilo::tlv* v0 = jct.get_value("player.name",err);
					if (v0 == nullptr)
					{
						return p_case->error(err, "load json file %s failed.", fn.c_str());
					}


					p_case->set_result(PILO_OK);
					return PILO_OK;
				}




			}
		}
	}
}