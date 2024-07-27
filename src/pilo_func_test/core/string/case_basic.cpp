#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"


using namespace ::pilo::func_test;


namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace string
			{
				int case_find_substring(::pilo::core::testing::func_test_case* p_case)
				{
					char buffer[4096] = { 0 };
					memset(buffer, 'x', 4000);
					buffer[4000] = '\r';
					buffer[4001] = '\n';
					::pilo::i64_t nidx = 0;
					const char* ret = ::pilo::core::string::find_substring(buffer, 4002, "\r\n", 2, nidx);
					if (ret == nullptr) {
						return p_case->error(PERR_NULL_PTR, "line sep not find");
					}
					if (nidx != 2) {
						return p_case->error(PERR_NULL_PTR, "idx not 2");
					}
					ret = ::pilo::core::string::find_substring(buffer, 4001, "\r\n", 2, nidx);
					if (ret != nullptr) {
						return p_case->error(PERR_NULL_PTR, "line sep find");
					}
					if (nidx != 1) {
						return p_case->error(PERR_NULL_PTR, "idx not 1");
					}

					ret = ::pilo::core::string::find_substring(buffer, 4002, "x\r\n", 3, nidx);
					if (ret == nullptr) {
						return p_case->error(PERR_NULL_PTR, "line sep not find");
					}
					if (nidx != 3) {
						return p_case->error(PERR_NULL_PTR, "idx not 3");
					}
					ret = ::pilo::core::string::find_substring(buffer, 4001, "x\r\n", 3, nidx);
					if (ret != nullptr) {
						return p_case->error(PERR_NULL_PTR, "line sep find");
					}
					if (nidx != 2) {
						return p_case->error(PERR_NULL_PTR, "idx not 1");
					}


					p_case->set_result(PILO_OK);

					return PILO_OK;
				}

				int case_split_fixed(::pilo::core::testing::func_test_case* p_case)
				{
					const char* src = "id,name,job,salary,address";
					::pilo::cstr_ref<char> parts[16];
					char errmsg_buffer[1024] = { 0 };

					::pilo::i64_t cnt = ::pilo::core::string::split_fixed<char>(src, -1 , ",", 1, parts, 16, false, true, true, true);
	
					if (cnt != 5)
					{
						::pilo::core::io::string_formated_output(errmsg_buffer, 1024, "Invalid split cnt %d : %d", cnt, 5);
						p_case->set_desc(errmsg_buffer);
						return ::pilo::mk_perr(PERR_INVALID_PARAM);
					}

					src = "id,name,job,salary,,address";
					cnt = ::pilo::core::string::split_fixed<char>(src, -1, ",", 1, parts, 16, false, true, true, true);
					if (cnt != 6)
					{
						::pilo::core::io::string_formated_output(errmsg_buffer, 1024, "Invalid split cnt %d : %d", cnt, 5);
						p_case->set_desc(errmsg_buffer);
						return ::pilo::mk_perr(PERR_INVALID_PARAM);
					}

					src = "id,name,job,salary,,address";
					cnt = ::pilo::core::string::split_fixed<char>(src, -1, ",", 1, parts, 16, true, true, true, true);
					if (cnt != 5)
					{
						::pilo::core::io::string_formated_output(errmsg_buffer, 1024, "Invalid split cnt %d : %d", cnt, 5);
						p_case->set_desc(errmsg_buffer);
						return ::pilo::mk_perr(PERR_INVALID_PARAM);
					}


					p_case->set_result(PILO_OK);

					return PILO_OK;
				}
			}
		}
	}
}