#include <iostream>
#include "pilo/pilo.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo_func_test.hpp"
#include "pilo/core/testing/fixsized_sample_object.hpp"
#include "pilo/core/datetime/stop_watch.hpp"
#include <thread>
#include "pilo/core/datetime/datetime.hpp"
#include "pilo/core/stat/system_information.hpp"
#include "pilo/core/threading/order_awared_mutex_adaptor.hpp"
#include "pilo/core/threading/auto_join_thread.hpp"
#include "pilo/core/container/concurrent_queue.hpp"
#include "pilo/core/process/context.hpp"
#include "pilo/core/memory/linked_byte_buffer.hpp"
#include "pilo/core/pattern/singleton.hpp"
#include "pilo/core/testing/inner_sample_data.hpp"
#include "pilo/error.hpp"
#include "pilo/tlv.hpp"

#include "pilo/core/pattern/resource_cleaner.hpp"


using namespace ::pilo::core::testing;

void load_cases(func_test_suite* suite);
//
//unsigned long long gettickcount()
//{
//    unsigned int lo, hi;
//    __asm__ volatile("rdtsc" : "=a"(lo), "=d"(hi));
//    return ((unsigned long long)hi << 32) | lo;
//}

int main(int argc, char * argv[])
{
	PMC_UNUSED(argc);
	PMC_UNUSED(argv);

	PILO_CONTEXT->initialize();

	func_test_suite suite_default;

	load_cases(&suite_default);

	suite_default.run_cases(true);

	std::string mrep;
	std::cout << ::pilo::core::memory::linked_byte_buffer< 4096, 1024, true>::buffer_node::to_string() << std::endl;
	std::cout << ::pilo::core::testing::inner_sample_data::to_string() << std::endl;

	getchar();
	return 0;
}

void load_cases(func_test_suite * suite)
{	
	suite->register_case("path_basic", ::pilo::func_test::core::io::case_path_basic, nullptr);
	suite->register_case("fs_op", ::pilo::func_test::core::io::case_path_fs_op, nullptr);

	suite->register_case("get_paths", ::pilo::func_test::core::io::case_get_paths, nullptr);
	suite->register_case("paths_attr", ::pilo::func_test::core::io::case_paths_attr, nullptr);
	
	suite->register_case("case_map_serialize", ::pilo::func_test::core::rtti::case_map_serialize, nullptr);
	suite->register_case("case_wired_deque_pushback", ::pilo::func_test::core::rtti::case_wired_deque_pushback, nullptr);
	suite->register_case("case_wired_deque_basic", ::pilo::func_test::core::rtti::case_wired_deque_basic, nullptr);
	suite->register_case("case_map_keyi8", ::pilo::func_test::core::rtti::case_map_keyi8, nullptr);
	
	suite->register_case("base64_static", ::pilo::func_test::case_base64_static, nullptr);
	
	 suite->register_case("tlv_path_set", ::pilo::func_test::case_tlv_path_set, nullptr);
	 suite->register_case("tlv_dict", ::pilo::func_test::case_tlv_dict, nullptr);
	 suite->register_case("tlv_clone", ::pilo::func_test::case_tlv_clone, nullptr);
	 suite->register_case("tlv_list", ::pilo::func_test::case_tlv_array, nullptr);
	 suite->register_case("tlv_basic", ::pilo::func_test::case_tlv_basic, nullptr);
	 suite->register_case("tlv_str", ::pilo::func_test::case_tlv_str, nullptr);
	 suite->register_case("tlv_float", ::pilo::func_test::case_tlv_float, nullptr);
	 suite->register_case("tlv_array_bool", ::pilo::func_test::case_tlv_array_bool, nullptr);
	 suite->register_case("tlv_array_i8", ::pilo::func_test::case_tlv_array_i8, nullptr);
	 suite->register_case("tlv_array_u8", ::pilo::func_test::case_tlv_array_u8, nullptr);
	 suite->register_case("tlv_array_i16", ::pilo::func_test::case_tlv_array_i16, nullptr);
	 suite->register_case("tlv_array_u16", ::pilo::func_test::case_tlv_array_u16, nullptr);
	 suite->register_case("tlv_array_i32", ::pilo::func_test::case_tlv_array_i32, nullptr);
	 suite->register_case("tlv_array_i32", ::pilo::func_test::case_tlv_array_u32, nullptr);
	 suite->register_case("tlv_array_i64", ::pilo::func_test::case_tlv_array_i64, nullptr);
	 suite->register_case("tlv_array_u64", ::pilo::func_test::case_tlv_array_u64, nullptr);
	 suite->register_case("tlv_array_f32", ::pilo::func_test::case_tlv_array_f32, nullptr);
	 suite->register_case("tlv_array_f64", ::pilo::func_test::case_tlv_array_f64, nullptr);
	 suite->register_case("tlv_array_str", ::pilo::func_test::case_tlv_array_str, nullptr);
	 suite->register_case("tlv_array_bytes", ::pilo::func_test::case_tlv_array_bytes, nullptr);
	 suite->register_case("tlv_array_tlv", ::pilo::func_test::case_tlv_array_tlv, nullptr);
	
	 suite->register_case("case_fixed_byte_buffer_serialize_net", ::pilo::func_test::core::memory::case_fixed_byte_buffer_serialize_net, nullptr);
	 suite->register_case("case_fixed_byte_buffer_serialize_host", ::pilo::func_test::core::memory::case_fixed_byte_buffer_serialize_host, nullptr);
	
	 suite->register_case("split_fixed<>", ::pilo::func_test::core::string::case_split_fixed, nullptr);
	 suite->register_case("case_linked_byte_buffer_basic",::pilo::func_test::core::memory::case_linked_byte_buffer_basic, nullptr);
	 suite->register_case("case_linked_byte_buffer_serialize_net", ::pilo::func_test::core::memory::case_linked_byte_buffer_serialize_net, nullptr);
	 suite->register_case("case_linked_byte_buffer_serialize_host", ::pilo::func_test::core::memory::case_linked_byte_buffer_serialize_host, nullptr);
	
	 suite->register_case("case_linked_byte_buffer_serialize_1", ::pilo::func_test::core::memory::case_linked_byte_buffer_serialize_1, nullptr);
	
	 suite->register_case("linked_byte_buffer_mt_4k_local", ::pilo::func_test::core::memory::case_linked_byte_buffer_mt_4k_local, nullptr);
	 suite->register_case("linked_byte_buffer_mt_4k_net", ::pilo::func_test::core::memory::case_linked_byte_buffer_mt_4k_net, nullptr);

}