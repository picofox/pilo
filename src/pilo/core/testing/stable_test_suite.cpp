#include "stable_test_suite.hpp"
#include "core/io/formatted_io.hpp"
#include <iostream>

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			stable_test_suite::stable_test_suite()
			{
				_cases.clear();
				//::pilo::core::io::file_formatted_output(stdout, "%s# %s %s %s %s %s %s\n", istr, name_fw_str, tcstr, msstr, rstr, value_type_cstr, pstr);

				_formater.add_meta_field(8, 0, "Id");
				_formater.add_meta_field(24, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Name");
				_formater.add_meta_field(14, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "T-Count", " ", "%.1f%s");
				_formater.add_meta_field(10, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "T-Duration", " ", "%.1f%s");
				_formater.add_meta_field(8, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Result");
				_formater.add_meta_field(8, 0, "Code");
				_formater.add_meta_field(14, 0, "Count");
				_formater.add_meta_field(14, 0, "Duration");
				_formater.add_meta_field(10, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "TPS", " ", "%.2f%s");
				_formater.add_meta_field(40, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Desc");
			}

			stable_test_suite::~stable_test_suite()
			{
				for (::pilo::i64_t i = 0; i < (::pilo::i64_t)_cases.size(); i++)
				{
					delete _cases[i];
				}
				_cases.clear();
				_cases.shrink_to_fit();
			}

			::pilo::err_t stable_test_suite::register_case(const char* name, stable_test_case::case_function_type func, void* arg,  ::pilo::i64_t n_trans, ::pilo::i64_t n_secs)
			{
				stable_test_case* pcase = new stable_test_case((::pilo::i32_t)_cases.size(), name, func, arg, n_trans, n_secs);
				_cases.push_back(pcase);
				return PILO_OK;
			}

			::pilo::i64_t stable_test_suite::run_cases(bool stop_on_error)
			{
				std::cout << this->_formater.format_header();


				::pilo::i64_t rcnt = 0;
				for (::pilo::i64_t i = 0; i < (::pilo::i64_t)_cases.size(); i++)
				{
					std::cout << this->_formater.format_field<::pilo::u64_t>(0, i);
					std::cout << this->_formater.format_field(1, _cases[i]->name());
					if (_cases[i]->target_trans_count() == PILO_INT64_MAX) 
						std::cout << this->_formater.format_field(2, "Max");						
					else
						std::cout << this->_formater.format_field<::pilo::i64_t>(2, _cases[i]->target_trans_count());
					if (_cases[i]->target_persist_millisecs() == PILO_INT64_MAX)
						std::cout << this->_formater.format_field(3, "Max");
					else
						std::cout << this->_formater.format_field<::pilo::i64_t>(3, _cases[i]->target_persist_millisecs() / 1000);

					fflush(stdout);
					
					if (_cases[i]->execute() != PILO_OK)
					{
						if (stop_on_error)
						{
							rcnt++;
							std::cout << this->_formater.format_field(4, "EXEC_ERR");
							std::cout << std::endl;
							return rcnt;
						}
					}

					if (_cases[i]->validate_result() != PILO_OK)
					{
						std::cout << this->_formater.format_field(4, "FAILED");
						std::cout << this->_formater.format_field<::pilo::i32_t>(5, _cases[i]->result());
						std::cout << this->_formater.format_field<::pilo::i64_t>(6, _cases[i]->trans_count());
						std::cout << this->_formater.format_field<::pilo::f64_t>(7, _cases[i]->persist_seconds());
						std::cout << this->_formater.format_field<::pilo::f64_t>(8, _cases[i]->tps());
						std::cout << this->_formater.format_field(9, _cases[i]->desc());
						rcnt++;
						std::cout << std::endl;
						return rcnt;
					}
					else
					{	
						std::cout << this->_formater.format_field(4, "OK");
						std::cout << this->_formater.format_field<::pilo::i32_t>(5, _cases[i]->result());
						std::cout << this->_formater.format_field<::pilo::i64_t>(6, _cases[i]->trans_count());
						std::cout << this->_formater.format_field<::pilo::f64_t>(7, _cases[i]->persist_seconds());
						std::cout << this->_formater.format_field<::pilo::f64_t>(8, _cases[i]->tps());
						std::cout << this->_formater.format_field(8, _cases[i]->desc());
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


