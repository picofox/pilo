#include "../../pilo.hpp"
#include "stable_test_case.hpp"
#include <functional>
#include <vector>
#include "../string/string_operation.hpp"
#include "../string/fixed_width_line_formater.hpp"

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			class stable_test_suite
			{
			public:
				stable_test_suite();
				~stable_test_suite();

				::pilo::err_t register_case(const char* name, stable_test_case::case_function_type func, void* arg,::pilo::i64_t n_trans, ::pilo::i64_t n_secs);
				::pilo::i64_t run_cases(bool stop_on_error);

			private:
				::pilo::core::string::fixed_width_line_formater _formater;
				std::vector<stable_test_case*> _cases;
				

			public:
				template <typename T> 
				static ::pilo::i64_t validate_cstr_refs(::pilo::cstr_ref<T>* parts, const char** cstrs, ::pilo::i64_t cstrs_cnt)
				{
					for (::pilo::i64_t i = 0; i < cstrs_cnt; i++)
					{
						if (parts[i].length != ::pilo::core::string::character_count(cstrs[i])) 
						{
							return i;
						}

						if (::pilo::core::string::strict_compare(parts[i].ptr, 0, cstrs[i], 0, parts[i].length) != 0)
						{
							return i;
						}
					}

					return cstrs_cnt;
				}
			};
		}
	}
	
}