#include <iostream>
#include <thread>
#include "pilo/pilo.hpp"
#include "pilo/core/testing/stable_test_suite.hpp"
#include "pilo_stable_test.hpp"
#include "pilo/core/testing/fixsized_sample_object.hpp"
#include "pilo/core/datetime/stop_watch.hpp"
#include "pilo/core/datetime/datetime.hpp"
#include "pilo/core/stat/system_information.hpp"
#include "pilo/core/threading/auto_join_thread.hpp"
#include "pilo/core/container/concurrent_queue.hpp"
#include "pilo/core/memory/linked_byte_buffer.hpp"
#include "pilo/core/threading/auto_join_thread.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/memory/linked_byte_buffer.hpp"
#include "pilo/core/testing/inner_sample_data.hpp"
#include "pilo/error.hpp"
#include "pilo/tlv.hpp"

#include "pilo/core/config/xls_config.hpp"
#include "pilo/core/rtti/meta_variable.hpp"
#include "pilo/core/rtti/meta_function.hpp"
#include "pilo/core/rtti/meta_enum.hpp"
#include "pilo/core/rtti/meta_klass.hpp"

using namespace ::pilo::core::testing;

void load_cases(stable_test_suite* suite);




int main(int argc, char * argv[])
{
	PILO_STARTUP_INIT(argc, argv);
	stable_test_suite suite_default;
	load_cases(&suite_default);
	PILO_CONTEXT->start();

/*
	::pilo::core::config::xls_config_set xcfg;
	std::string errmsg;
	::pilo::err_t err = xcfg.parse("hero", "D:\\projects\\git_repo\\libpilo\\output\\win-vs17\\pilo\\tmp\\test_xls\\hero.xlsx", ::pilo::predefined_pilo_path::count, errmsg);
	if (err != PILO_OK) {
		printf("\n%s\n", errmsg.c_str());
		return -1;
	}

	err = xcfg.save_config_file(::pilo::core::config::xls_config_set::server, "app_config/server", ::pilo::predefined_pilo_path::cnf);
	err = xcfg.save_config_file(::pilo::core::config::xls_config_set::client, "app_config/client", ::pilo::predefined_pilo_path::cnf);
	printf("%s\n", xcfg.to_string().c_str());

	*/

	std::stringstream ss;

	::pilo::core::rtti::meta_klass c1(1, 0, "player", "gameobject", ::pilo::core::rtti::mod_protected);
	::pilo::u64_t mod = ::pilo::core::rtti::mod_inline;
	mod = 0;
	c1.add_basetype_member_variable(::pilo::core::rtti::mod_private | mod, ::pilo::core::rtti::getter_rtype | ::pilo::core::rtti::setter_vtype, "id", "unsigned long long", "-1");
	c1.add_string_member_variable(::pilo::core::rtti::mod_private | mod, ::pilo::core::rtti::getter_rtype | ::pilo::core::rtti::setter_vtype, "title", "std::string", "");
	c1.add_basetype_member_variable(::pilo::core::rtti::mod_private | mod, ::pilo::core::rtti::getter_rtype | ::pilo::core::rtti::setter_vtype, "vip_level", "unsigned char", "0");
	c1.add_basetype_member_variable(::pilo::core::rtti::mod_protected | mod, ::pilo::core::rtti::getter_rtype | ::pilo::core::rtti::setter_vtype, "obj_type", "short", "0");
	c1.add_basetype_member_variable(::pilo::core::rtti::mod_public | mod, ::pilo::core::rtti::getter_rtype | ::pilo::core::rtti::setter_vtype, "coin", "long long", "0");
	c1.add_basetype_member_variable(::pilo::core::rtti::mod_private | mod | ::pilo::core::rtti::mod_isstr, ::pilo::core::rtti::getter_rtype | ::pilo::core::rtti::setter_vtype, "desc", "std::string", "no desc");
	c1.add_desstructor(::pilo::core::rtti::mod_public | ::pilo::core::rtti::mod_virtual);

	::pilo::core::rtti::meta_function* confp = c1.add_constructor(::pilo::core::rtti::mod_public | mod, { "GmaeObjectTypeEnum::player", "\"fox\"" });
	confp->add_param(::pilo::core::rtti::mod_map_to_member | ::pilo::core::rtti::mod_private, "id", "unsigned long long", "0");
	if (confp == nullptr)
		return -1;
	confp->add_param(::pilo::core::rtti::mod_map_to_member | ::pilo::core::rtti::mod_private | ::pilo::core::rtti::mod_cost_str, "title", "std::string&", "player title here");
	if (confp == nullptr)
		return -1;
	confp->add_param(::pilo::core::rtti::mod_map_to_member | ::pilo::core::rtti::mod_private, "vip_level", "unsigned char", "0");
	if (confp == nullptr)
		return -1;
	confp->add_param(::pilo::core::rtti::mod_map_to_member | ::pilo::core::rtti::mod_protected, "obj_type", "short", "0");
	if (confp == nullptr)
		return -1;

	confp->add_param(::pilo::core::rtti::mod_map_to_member | ::pilo::core::rtti::mod_public, "coin", "long long", "0");
	if (confp == nullptr)
		return -1;


	c1.append_to_stringstream_cpp(ss, ::pilo::core::rtti::oflag_dec, "", nullptr);
	c1.append_to_stringstream_cpp(ss, 0, "", nullptr);
	
	printf("\n-------------------------------\n");
	printf("%s",ss.str().c_str());
 	printf("\n-------------------------------\n");


	//auto wks = doc.workbook().worksheet("hero");
	//std::string str1 = wks.cell(OpenXLSX::XLCellReference("A1")).value().get<std::string>();




	//suite_default.run_cases(true);
	//std::string str = PILO_CONTEXT->pool_object_stat_mgr().to_updated_string();
	//std::cout << str << std::endl;

	getchar();
	return 0;
}

void load_cases(stable_test_suite * suite)
{
	suite->register_case("case_cron_service_base", ::pilo::stable_test::core::service::case_cron_service_base, nullptr, 10000000, -1);
	suite->register_case("timer_service_single", ::pilo::stable_test::core::service::case_timer_service_milli_single, nullptr, 10000000, -1);
	suite->register_case("timer_service_multi", ::pilo::stable_test::core::service::case_timer_service_milli_multi, nullptr, 100000000, -1);

	//suite->register_case("p_thread_pool_NQ", ::pilo::stable_test::core::threading::case_performance_thread_pool_global_nqueue, nullptr, 100000000, -1);
	//suite->register_case("p_thread_pool_Q", ::pilo::stable_test::core::threading::case_performance_thread_pool_global_queue, nullptr, 100000000, -1);
	  
	//suite->register_case("e_thread_pool_NQ", ::pilo::stable_test::core::threading::case_effcient_thread_pool_global_nqueue, nullptr, 100000000, -1);
	//suite->register_case("e_thread_pool_Q", ::pilo::stable_test::core::threading::case_effcient_thread_pool_global_queue, nullptr, 100000000, -1);
	suite->register_case("conncurrent_queue@1g", ::pilo::stable_test::core::container::case_concurrent_queue_for_1g_count, nullptr, 10000000, -1);
	suite->register_case("utf_unicode", ::pilo::stable_test::core::i18n::case_utf8_unicode, nullptr, 100000, -1);
	suite->register_case("path_creation", ::pilo::stable_test::core::io::case_path_creation, nullptr, 1000000, -1);

	suite->register_case("tlv_pathset", ::pilo::stable_test::case_tlv_pathset, nullptr, 100000, -1);
	suite->register_case("tlv_serialization", ::pilo::stable_test::case_tlv_serialization, nullptr, 1000, -1);
	suite->register_case("tlv_acc_dict", ::pilo::stable_test::case_tlv_acc_dict, nullptr, 1000000, -1);
	suite->register_case("tlv_acc", ::pilo::stable_test::case_tlv_acc, nullptr, 10000, -1);
	suite->register_case("tlv_acc_array", ::pilo::stable_test::case_tlv_acc_array, nullptr, 100000, -1);
	suite->register_case("tlv_acc_single", ::pilo::stable_test::case_tlv_acc_single, nullptr, 100000, -1);
	

	suite->register_case("tcq_128_sm@1g", ::pilo::stable_test::core::container::case_tcp_psm_128_queue_1g_count, nullptr, 300000, -1);
	suite->register_case("tcq_128_stdm@1g", ::pilo::stable_test::core::container::case_tcp_stdm_128_queue_1g_count, nullptr, 30000, -1);
   

}
