#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/rtti/wired_type_deque.hpp"
#include "pilo/core/rtti/wired_type.hpp"

using namespace ::pilo::func_test;


namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace rtti
			{
				int case_wired_deque_basic(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::err_t err = PILO_OK;

					std::deque<::pilo::i8_t> * deq = ::pilo::core::rtti::create_deque<::pilo::i8_t>({0,1,2,3,4}, false, &err);
					if (deq == nullptr)
					{
						return p_case->error(err, "explicitly create array for i8 failed");
					}					

					p_case->set_result(PILO_OK);
					return 0;
				}

				int case_wired_deque_pushback(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::err_t err = PILO_OK;

					char* deque = ::pilo::core::rtti::create_deque(::pilo::core::rtti::wired_type::value_type_i64, &err);
					if (deque == nullptr || err != PILO_OK)
					{
						return p_case->error(err, "create i64 deque failed.");
					}

					for (::pilo::i64_t i = 0; i < 10000000; i++)
					{
						err = ::pilo::core::rtti::deque_push_back(::pilo::core::rtti::wired_type::value_type_i64, deque, i * 10000, -1, false);
						if (deque == nullptr || err != PILO_OK)
						{
							return p_case->error(err, "push i64 deque failed.");
						}
					}

					::pilo::i64_t rcnt = reinterpret_cast<std::deque<::pilo::i64_t>*>(deque)->size();
					if (rcnt != 10000000)
					{
						return p_case->error(err, "push i64 deque count error. %lld", rcnt);
					}

					p_case->set_result(PILO_OK);
					return 0;
				}
			}
		}
	}
}