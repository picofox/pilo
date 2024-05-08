﻿#pragma once

#include	"pilo/core/testing/stable_test_case.hpp"

namespace pilo
{
	namespace stable_test
	{
		int case_tlv_serialization(::pilo::core::testing::stable_test_case* p_case);
		int case_tlv_acc_dict(::pilo::core::testing::stable_test_case* p_case);
		int case_tlv_acc_array(::pilo::core::testing::stable_test_case* p_case);
		int case_tlv_acc_single(::pilo::core::testing::stable_test_case* p_case);
		int case_tlv_acc(::pilo::core::testing::stable_test_case* p_case);

		namespace core
		{
			namespace container
			{
				int case_tcp_psm_128_queue_1g_count(::pilo::core::testing::stable_test_case* p_case);
				int case_tcp_stdm_128_queue_1g_count(::pilo::core::testing::stable_test_case* p_case);
				int case_concurrent_queue_for_1g_count(::pilo::core::testing::stable_test_case* p_case);
			}
		}
	}
}