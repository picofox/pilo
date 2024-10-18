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

using namespace ::pilo::core::testing;

void load_cases(stable_test_suite* suite);


int main(int argc, char * argv[])
{
	PILO_STARTUP_INIT(argc, argv);

	stable_test_suite suite_default;

	load_cases(&suite_default);

	PILO_CONTEXT->start();
	suite_default.run_cases(true);


	std::string str = PILO_CONTEXT->pool_object_stat_mgr().to_updated_string();
	std::cout << str << std::endl;

	getchar();
	return 0;
}

void load_cases(stable_test_suite * suite)
{
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