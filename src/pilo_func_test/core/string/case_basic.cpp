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
						return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID, 0);
					}

					src = "id,name,job,salary,,address";
					cnt = ::pilo::core::string::split_fixed<char>(src, -1, ",", 1, parts, 16, false, true, true, true);
					if (cnt != 6)
					{
						::pilo::core::io::string_formated_output(errmsg_buffer, 1024, "Invalid split cnt %d : %d", cnt, 5);
						p_case->set_desc(errmsg_buffer);
						return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID, 0);
					}

					src = "id,name,job,salary,,address";
					cnt = ::pilo::core::string::split_fixed<char>(src, -1, ",", 1, parts, 16, true, true, true, true);
					if (cnt != 5)
					{
						::pilo::core::io::string_formated_output(errmsg_buffer, 1024, "Invalid split cnt %d : %d", cnt, 5);
						p_case->set_desc(errmsg_buffer);
						return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID, 0);
					}





					p_case->set_result(PILO_OK);

					return PILO_OK;
				}
			}
		}
	}
}