////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    //
//  .----------------.  .----------------.  .----------------.  .----------------.       Raid boss    //
//  | .--------------. || .--------------. || .--------------. || .--------------. |    Lv.85 缺德猫   //
//  | |   ______     | || |     _____    | || |   _____      | || |     ____     | |     |\.-"-./|    //
//  | |  |_   __ \   | || |    |_   _|   | || |  |_   _|     | || |   .'    `.   | |     \`     `/    //
//  | |    | |__) |  | || |      | |     | || |    | |       | || |  /  .--.  \  | |     |= ^Y^ =|    //
//  | |    |  ___/   | || |      | |     | || |    | |   _   | || |  | |    | |  | |     \__ ^ __/    //
//  | |   _| |_      | || |     _| |_    | || |   _| |__/ |  | || |  \  `- - '/  | |     /`=+o+=`\    //
//  | |  |_____|     | || |    |_____|   | || |  |________|  | || |   `.____.'   | |    |         |   //
//  | |              | || |              | || |              | || |              | |    | (     ) |   //
//  | '--------------' || '--------------' || '--------------' || '--------------' |    (,,)---(,,)   // 
//  '----------------'  '----------------'  '----------------'  '----------------'                    //
//                                                                                                    //  
////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/ml/json_tlv_driver.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/config/xls_config.hpp"


using namespace ::pilo::func_test;



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
						return p_case->error(xret, xconf_gen.log_set().latest().to_string(0).c_str());
					}

					xret = xconf_gen.parse();
					if (xret != PILO_OK) {
						return p_case->error(xret, xconf_gen.log_set().latest().to_string(0).c_str());
					}

					xret = xconf_gen.generate_server_config();
					if (xret != PILO_OK) {
						return p_case->error(xret, xconf_gen.log_set().latest().to_string(xconf_gen.log_set().content_mask()).c_str());
					}

					xconf_gen.log_set().travel(&xconf_gen, [](void * ctx, const ::pilo::core::logging::info_item& ii)
						{
							::pilo::core::config::xls_config_generator* ptr = (::pilo::core::config::xls_config_generator*) ctx;
							std::string sss = ii.to_string(ptr->log_set().content_mask());
							printf("%s\n", sss.c_str());
						});
										

					p_case->set_result(PILO_OK);
					return PILO_OK;
				}




			}
		}
	}
}