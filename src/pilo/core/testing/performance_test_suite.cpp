#include "performance_test_suite.hpp"
#include "core/io/formatted_io.hpp"
#include <iostream>
#include "../memory/util.hpp"

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			performance_test_suite::performance_test_suite()
			{
				_cases.clear();
				_formater.add_meta_field(8, 0, "Id");//0
				_formater.add_meta_field(24, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Name");//1
				_formater.add_meta_field(14, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "T-Count", " ", "%.1f%s");//2
				_formater.add_meta_field(12, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "T-Dura", " ", "%.1f%s");//3
				_formater.add_meta_field(4, 0, "CC", " ", "");//4

				_formater.add_meta_field(8, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Result");//5
				_formater.add_meta_field(8, 0, "Code");//6
				_formater.add_meta_field(8, 0, "Duration");//7
				_formater.add_meta_field(8, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "TPS-In", " ", "%.2f%s");//8
				_formater.add_meta_field(8, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "TPS-Out", " ", "%.2f%s");//9
				_formater.add_meta_field(8, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "BPS-In", " ", "%.2f%s");//10
				_formater.add_meta_field(8, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "BPS-Out", " ", "%.2f%s");//11
				_formater.add_meta_field(8, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "TPS", " ", "%.2f%s");//12
				_formater.add_meta_field(8, PMI_FIXED_WIDTH_LINE_FMT_READABLIZE, "BPS", " ", "%.2f%s");//13
				_formater.add_meta_field(10, 0, "Bench");//14
				_formater.add_meta_field(40, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Desc");//15
			}

			performance_test_suite::~performance_test_suite()
			{
				::pilo::core::memory::clear_raw_ptr_linear_container(_cases, true);
			}

			::pilo::err_t performance_test_suite::register_case(const char* name, performance_test_case::case_function_type func, void* arg,  ::pilo::i64_t n_trans, ::pilo::i64_t n_secs, ::pilo::i32_t concurrency, performance_test_case::score_function_type score_func)
			{
				performance_test_case* pcase = new performance_test_case((::pilo::i32_t)_cases.size(), name, func, arg, n_trans, n_secs, concurrency, score_func);
				_cases.push_back(pcase);
				return PILO_OK;
			}

			::pilo::i64_t performance_test_suite::run_cases(bool stop_on_error)
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
					
					std::cout << this->_formater.format_field<::pilo::i32_t>(4, _cases[i]->concurrency());

					if (_cases[i]->execute() != PILO_OK)
					{
						if (stop_on_error)
						{
							rcnt++;
							std::cout << this->_formater.format_field(5, "EXEC_ERR");
							std::cout << std::endl;
							return rcnt;
						}
					}

					if (_cases[i]->validate_result() != PILO_OK)
					{
						std::cout << this->_formater.format_field(5, "FAILED");
						std::cout << this->_formater.format_field<::pilo::i32_t>(6, _cases[i]->result());
						std::cout << this->_formater.format_field<::pilo::f64_t>(7, _cases[i]->persist_seconds());
						std::cout << this->_formater.format_field<::pilo::i64_t>(8, _cases[i]->tps_in());
						std::cout << this->_formater.format_field<::pilo::i64_t>(9, _cases[i]->tps_out());
						std::cout << this->_formater.format_field<::pilo::i64_t>(10, _cases[i]->bps_in());
						std::cout << this->_formater.format_field<::pilo::i64_t>(11, _cases[i]->bps_out());
						std::cout << this->_formater.format_field<::pilo::i64_t>(12, _cases[i]->tps_overall());
						std::cout << this->_formater.format_field<::pilo::i64_t>(13, _cases[i]->bps_overall());
						std::cout << this->_formater.format_field(14, "-");
						std::cout << this->_formater.format_field(15, _cases[i]->desc());
						rcnt++;
						std::cout << std::endl;
						return rcnt;
					}
					else
					{
						std::cout << this->_formater.format_field(5, "OK");
						std::cout << this->_formater.format_field<::pilo::i32_t>(6, _cases[i]->result());
						std::cout << this->_formater.format_field<::pilo::f64_t>(7, _cases[i]->persist_seconds());
						std::cout << this->_formater.format_field<::pilo::i64_t>(8, _cases[i]->tps_in());
						std::cout << this->_formater.format_field<::pilo::i64_t>(9, _cases[i]->tps_out());
						std::cout << this->_formater.format_field<::pilo::i64_t>(10, _cases[i]->bps_in());
						std::cout << this->_formater.format_field<::pilo::i64_t>(11, _cases[i]->bps_out());
						std::cout << this->_formater.format_field<::pilo::i64_t>(12, _cases[i]->tps_overall());
						std::cout << this->_formater.format_field<::pilo::i64_t>(13, _cases[i]->bps_overall());

						std::string score;
						if (_cases[i]->score_function() != nullptr)
						{
							score = _cases[i]->score_function()(_cases[i - 2], _cases[i - 1], _cases[i]);
							std::cout << this->_formater.format_field(14, score.c_str());
						}
						else
							std::cout << this->_formater.format_field(14, "-");


						std::cout << this->_formater.format_field(14, _cases[i]->desc());
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


