#pragma once

#include	"pilo/core/testing/performance_test_case.hpp"

#define PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT	(1000000)

namespace pilo
{
	namespace performance_test
	{
		namespace core
		{
			namespace container
			{

				int case_bench_pivot_ceil(::pilo::core::testing::performance_test_case* p_case);
				int case_bench_pivot_floor(::pilo::core::testing::performance_test_case* p_case);
				int case_bench_pivot_cq(::pilo::core::testing::performance_test_case* p_case);



				std::string make_cq_score(const ::pilo::core::testing::performance_test_case* lowest_pivot_case
					, const ::pilo::core::testing::performance_test_case* highest_pivot_case
					, const ::pilo::core::testing::performance_test_case* case_ptr);
			}

			namespace memory
			{
				int case_linked_byte_buffer_sobj_host_st(::pilo::core::testing::performance_test_case* p_case);
				int case_fixed_byte_buffer_sobj_host_st(::pilo::core::testing::performance_test_case* p_case);
				int case_linked_byte_buffer_sobj_net_st(::pilo::core::testing::performance_test_case* p_case);
				int case_fixed_byte_buffer_sobj_net_st(::pilo::core::testing::performance_test_case* p_case);

				int case_fixed_byte_buffer_lobj_net_st(::pilo::core::testing::performance_test_case* p_case);
				int case_linked_byte_buffer_lobj_net_st(::pilo::core::testing::performance_test_case* p_case);
				int case_fixed_byte_buffer_lobj_host_st(::pilo::core::testing::performance_test_case* p_case);
				int case_linked_byte_buffer_lobj_host_st(::pilo::core::testing::performance_test_case* p_case);

				int case_linked_byte_buffer_spsc_host(::pilo::core::testing::performance_test_case* p_case);
				int case_fixed_byte_buffer_spsc_host(::pilo::core::testing::performance_test_case* p_case);
			}
		}
	}
}