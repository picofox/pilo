#pragma once

#include "pilo/core/testing/func_test_case.hpp"

namespace pilo
{
	namespace func_test
	{		
		
		int case_base64_static(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_path_set(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_dict(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_clone(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_tlv(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_bytes(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_str(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_f64(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_f32(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_u64(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_i64(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_u32(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_i32(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_u16(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_i16(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_u8(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_i8(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array_bool(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_array(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_basic(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_str(::pilo::core::testing::func_test_case* p_case);
		int case_tlv_float(::pilo::core::testing::func_test_case* p_case);
		


		namespace core
		{
			namespace io
			{
				int case_text_file_rdwrbuf(::pilo::core::testing::func_test_case* p_case);
				int case_text_file_nb(::pilo::core::testing::func_test_case* p_case);
				int case_file_openmode(::pilo::core::testing::func_test_case* p_case);
				int case_path_basic(::pilo::core::testing::func_test_case* p_case);
				int case_path_fs_op(::pilo::core::testing::func_test_case* p_case);
				int case_get_paths(::pilo::core::testing::func_test_case* p_case);
				int case_paths_attr(::pilo::core::testing::func_test_case* p_case);
			}

			namespace config
			{
				int case_json_config_basic(::pilo::core::testing::func_test_case* p_case);
			}

			namespace rtti
			{
				int case_wired_deque_pushback(::pilo::core::testing::func_test_case* p_case);
				int case_wired_deque_basic(::pilo::core::testing::func_test_case* p_case);
				int case_map_keyi8(::pilo::core::testing::func_test_case* p_case);
				int case_map_serialize(::pilo::core::testing::func_test_case* p_case);
			}

			namespace string
			{
				int case_find_substring(::pilo::core::testing::func_test_case* p_case);
				int case_split_fixed(::pilo::core::testing::func_test_case* p_case);	
				
			}

			namespace logging 
			{
				int case_text_logger_basic(::pilo::core::testing::func_test_case* p_case);
			}

			namespace memory
			{
				

				int case_fixed_byte_buffer_serialize_host(::pilo::core::testing::func_test_case* p_case);
				int case_fixed_byte_buffer_serialize_net(::pilo::core::testing::func_test_case* p_case);


				int case_linked_byte_buffer_mt_4k_local(::pilo::core::testing::func_test_case* p_case);
				int case_linked_byte_buffer_serialize_host(::pilo::core::testing::func_test_case* p_case);
				int case_linked_byte_buffer_serialize_net(::pilo::core::testing::func_test_case* p_case);
				int case_linked_byte_buffer_serialize_1(::pilo::core::testing::func_test_case* p_case);
				int case_linked_byte_buffer_basic(::pilo::core::testing::func_test_case* p_case);
				int case_linked_byte_buffer_mt_4k_net(::pilo::core::testing::func_test_case* p_case);
			}
		}
	}
}