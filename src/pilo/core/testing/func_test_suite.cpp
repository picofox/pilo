#include "func_test_suite.hpp"
#include "core/io/formatted_io.hpp"
#include "../memory/util.hpp"
#include <iostream>

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			func_test_suite::func_test_suite()
			{
				_cases.clear();
				_formater.add_meta_field(8, 0, "Id");//0
				_formater.add_meta_field(40, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Name");//1
				_formater.add_meta_field(8, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Result");//2
				_formater.add_meta_field(10, 0, "Code");//3
				_formater.add_meta_field(14, 0, "Duration", " ", "%.8f");//4
				_formater.add_meta_field(40, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Desc");//5
			}

			func_test_suite::~func_test_suite()
			{
				::pilo::core::memory::clear_raw_ptr_linear_container(_cases, true);
			}

			::pilo::err_t func_test_suite::register_case(const char* name, func_test_case::case_function_type func, void* arg)
			{
				func_test_case* pcase = new func_test_case((::pilo::i32_t)_cases.size(), name, func, arg);
				_cases.push_back(pcase);
				return PILO_OK;
			}

			::pilo::i64_t func_test_suite::run_cases(bool stop_on_error)
			{
				std::cout << this->_formater.format_header();
				::pilo::i64_t rcnt = 0;
				for (::pilo::i64_t i = 0; i < (::pilo::i64_t) _cases.size(); i++)
				{

					std::cout << this->_formater.format_field<::pilo::u64_t>(0, i);
					std::cout << this->_formater.format_field(1, _cases[i]->name());

					if (_cases[i]->execute() != PILO_OK)
					{
						if (stop_on_error)
						{
							rcnt++;
							std::cout << this->_formater.format_field(2, "EXEC_ERR");
							std::cout << std::endl;
							return rcnt;
						}
					}

					if (_cases[i]->validate_result() != PILO_OK)
					{
						rcnt++;			
						std::cout << this->_formater.format_field(2, "FAILED");
						std::cout << this->_formater.format_field<::pilo::i32_t>(3, _cases[i]->result());
						std::cout << this->_formater.format_field<::pilo::f64_t>(4, _cases[i]->time_cost());
						std::cout << this->_formater.format_field(5, _cases[i]->desc());
						std::cout << std::endl;
						return rcnt;
					}
					else
					{
						std::cout << this->_formater.format_field(2, "OK");
						std::cout << this->_formater.format_field<::pilo::i32_t>(3, _cases[i]->result());
						std::cout << this->_formater.format_field<::pilo::f64_t>(4, _cases[i]->time_cost());
						std::cout << this->_formater.format_field(5, _cases[i]->desc());
						std::cout << std::endl;
					}

					rcnt++;
				}

				::pilo::core::io::file_formatted_output(stdout, "All %lld cases Passed\n", rcnt);
				return PILO_OK;
			}
		}
	}	
}


