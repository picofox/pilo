#include "performance_test_case.hpp"
#include <chrono>
#include "../io/formatted_io.hpp"
#include "../datetime/timestamp.hpp"


namespace pilo
{
	namespace core
	{
		namespace testing
		{
			performance_test_case::performance_test_case(::pilo::i32_t idx, const char* name, case_function_type case_func, void* arg,  ::pilo::i64_t n_trans, ::pilo::i64_t n_secs, ::pilo::i32_t concurrency, performance_test_case::score_function_type score_func)
				:_index(idx), _name(name), _func(case_func), _arg(arg),  _target_trans_count(n_trans), _target_persist_millisecs(n_secs), _concurrency(concurrency), _score_func(score_func)
				, _result(::pilo::mk_perr(PERR_NOOP)), _in_trans_count(0), _out_trans_count(0), _in_bytes_count(0), _out_bytes_count(0), _in_milli_seconds(0), _out_milli_seconds(0)
				, _persist_milli_seconds(0), _begin_ts(-1), _desc("")
			{
				if (_target_persist_millisecs < 1)
					_target_persist_millisecs = LLONG_MAX;
				if (_target_trans_count < 1)
					_target_trans_count = LLONG_MAX;
			}


			performance_test_case::~performance_test_case()
			{

			}

			void performance_test_case::reset()
			{
				_index = -1;
				_name.clear();				
				_func = nullptr;
				_arg = nullptr;
				_target_trans_count = -1;
				_target_persist_millisecs = -1;
				_concurrency = 1;
				_score_func = nullptr;
				_result = ::pilo::mk_perr(PERR_NOOP);
				_in_trans_count = 0;
				_out_bytes_count = 0;
				_in_bytes_count = 0;
				_out_bytes_count = 0;
				_in_milli_seconds = 0;
				_out_milli_seconds = 0;
				_persist_milli_seconds = 0;
				_begin_ts = 0;
				_desc.clear();
			}

			bool performance_test_case::is_valid() const
			{
				if ((_index < 0) || (_func == nullptr)) {
					return false;
				}

				return true;
			}

			::pilo::err_t performance_test_case::execute()
			{
				if (is_valid())
				{
					_persist_milli_seconds = -1;
					::pilo::i64_t beg = ::pilo::core::datetime::timestamp_milli_steady();
					_func(this);
					_persist_milli_seconds = ::pilo::core::datetime::timestamp_milli_steady() - beg;					
					return PILO_OK;
				}
				return ::pilo::mk_perr(PERR_INV_OBJECT);

			}

			::pilo::err_t performance_test_case::validate_result()
			{
				if (_persist_milli_seconds == -1)
				{
					return ::pilo::mk_perr(PERR_RETRY);
				}

				if (_result != PILO_OK)
				{
					return ::pilo::mk_perr(PERR_USER_CANCEL);
				}
				else
				{
					return PILO_OK;
				}
			}
			const char* performance_test_case::to_cstring(char* buffer, ::pilo::i64_t buf_len, const char* rstr)
			{
				::pilo::core::io::string_formated_output(buffer, buf_len, "%d : %s", this->_index, rstr);
				return buffer;
			}
		}
	}	
}

