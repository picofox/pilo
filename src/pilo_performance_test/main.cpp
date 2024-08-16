#include "pilo/pilo.hpp"
#include "pilo/core/testing/performance_test_suite.hpp"
#include "pilo_performance_test.hpp"
#include "pilo/core/testing/fixsized_sample_object.hpp"
#include "pilo/core/datetime/stop_watch.hpp"
#include "pilo/core/datetime/datetime.hpp"
#include "pilo/core/stat/system_information.hpp"
#include "pilo/core/threading/order_awared_mutex_adaptor.hpp"
#include "pilo/core/threading/auto_join_thread.hpp"
#include "pilo/core/container/concurrent_queue.hpp"
#include "pilo/core/memory/linked_byte_buffer.hpp"
#include "pilo/core/process/context.hpp"


using namespace ::pilo::core::testing;

void load_cases(performance_test_suite* suite);


int main(int argc, char * argv[])
{
	PMC_UNUSED(argc);
	PMC_UNUSED(argv);

	::pilo::err_t err = PILO_STARTUP_INIT;

	if (err != PILO_OK) {
		fprintf(stderr, "PILO Initilization Failed: %s", ::pilo::str_err(err, nullptr, true).c_str());
		return -1;
	}

	performance_test_suite suite_default;

	load_cases(&suite_default);

	suite_default.run_cases(true);


	return 0;
}



void load_cases(performance_test_suite * suite)
{
	suite->register_case("FBB_spsc_host", ::pilo::performance_test::core::memory::case_fixed_byte_buffer_spsc_host, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);
	suite->register_case("LBB_spsc_host", ::pilo::performance_test::core::memory::case_linked_byte_buffer_spsc_host, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);


	suite->register_case("FBB_sobj_h_st", ::pilo::performance_test::core::memory::case_fixed_byte_buffer_sobj_host_st, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);
	suite->register_case("LBB_sobj_h_st", ::pilo::performance_test::core::memory::case_linked_byte_buffer_sobj_host_st, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);
	suite->register_case("FBB_sobj_n_st", ::pilo::performance_test::core::memory::case_fixed_byte_buffer_sobj_net_st, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);
	suite->register_case("LBB_sobj_n_st", ::pilo::performance_test::core::memory::case_linked_byte_buffer_sobj_net_st, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);


	suite->register_case("FBB_lobj_h_st", ::pilo::performance_test::core::memory::case_fixed_byte_buffer_lobj_host_st, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);
	suite->register_case("LBB_lobj_h_st", ::pilo::performance_test::core::memory::case_linked_byte_buffer_lobj_host_st, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);
	suite->register_case("FBB_lobj_n_st", ::pilo::performance_test::core::memory::case_fixed_byte_buffer_lobj_net_st, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);
	suite->register_case("LBB_lobj_n_st", ::pilo::performance_test::core::memory::case_linked_byte_buffer_lobj_net_st, nullptr, PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, -1, 2, nullptr);

	//
	suite->register_case("prof_conncurrent_queue_F", ::pilo::performance_test::core::container::case_bench_pivot_floor, nullptr, 20000000, -1, 4, nullptr);
	suite->register_case("prof_conncurrent_queue_C", ::pilo::performance_test::core::container::case_bench_pivot_ceil,   nullptr, 100000000, -1, 4, nullptr);
	suite->register_case("prof_conncurrent_queue", ::pilo::performance_test::core::container::case_bench_pivot_cq,   nullptr, 100000000, -1, 4, ::pilo::performance_test::core::container::make_cq_score);
}