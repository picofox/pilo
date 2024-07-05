#include "stable_test_case.hpp"
#include <chrono>
#include "../io/formatted_io.hpp"
#include "../datetime/timestamp.hpp"

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			stable_test_case::stable_test_case(::pilo::i32_t idx, const char* name, case_function_type case_func, void* arg,  ::pilo::i64_t n_trans, ::pilo::i64_t n_secs)
				:_index(idx), _name(name), _func(case_func), _arg(arg),  _target_trans_count(n_trans), _target_persist_millisecs(n_secs), _prog_count(0)
				,  _result(::pilo::mk_perr(PERR_NOOP)), _trans_count(0), _begin_ts(-1), _persist_milli_seconds(0), _desc("")
			{
				if (_target_persist_millisecs < 1)
					_target_persist_millisecs = LLONG_MAX;
				if (_target_trans_count < 1)
					_target_trans_count = LLONG_MAX;
			}


			stable_test_case::~stable_test_case()
			{

			}

			void stable_test_case::reset()
			{
				_index = -1;
				_name.clear();				
				_func = nullptr;
				_arg = nullptr;
				_result = ::pilo::mk_perr(PERR_NOOP);
				_trans_count = 0;
				_persist_milli_seconds = 0;
				_desc.clear();
			}

			bool stable_test_case::is_valid() const
			{
				if ((_index < 0) || (_func == nullptr)) {
					return false;
				}

				return true;
			}

			::pilo::err_t stable_test_case::execute()
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

			::pilo::err_t stable_test_case::validate_result()
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

			const char* stable_test_case::to_cstring(char* buffer, ::pilo::i64_t buf_len, const char* rstr)
			{
				::pilo::core::io::string_formated_output(buffer, buf_len, "%08d --[%s]--> [%s] Result: %d, count: %d, persist: %d ms", _index, rstr, _name.c_str(), _result.load(), _trans_count.load(), _persist_milli_seconds);

				return buffer;
			}

			void stable_test_case::inc_print_progress()
			{
				static const char* prchars = "-\\|/";
				static int idx = 0;
				_trans_count++;				
			    if (_trans_count >= _target_trans_count)
				{
					printf("\b\b\b\b");
					_prog_count = 0;
					return;
				}

				double v = (double) ((double) _trans_count / (double)_target_trans_count) * 100.0;
				int iv = (int)v;
				if (iv > _prog_count)
				{
					if (_prog_count > 0)
					{
						printf("\b\b\b\b");
					}

					printf(" %c%02d", prchars[idx % 4],  iv);
					idx++;
					_prog_count = iv;
				}

				fflush(stdout);
			}

			bool stable_test_case::check_error(::pilo::err_t err, ::pilo::err_t cerr, const char* fmt, ...)
			{
				if (err != cerr)
				{
					char buffer[256] = { 0 };
					va_list args;
					va_start(args, fmt);
#               if defined(WINDOWS)
					_vsnprintf_s(buffer, 256, _TRUNCATE, fmt, args);
#               else
					vsnprintf(buffer, 256, fmt, args);
#               endif
					va_end(args);
					set_result(err);
					set_desc(buffer);
					return true;
				}
				return false;
			}

			bool stable_test_case::check_error(::pilo::err_t err, const char* fmt, ...)
			{
				if (err != PILO_OK)
				{
					char buffer[256] = { 0 };
					va_list args;
					va_start(args, fmt);
#               if defined(WINDOWS)
					_vsnprintf_s(buffer, 256, _TRUNCATE, fmt, args);
#               else
					vsnprintf(buffer, sizeof(buffer), fmt, args);
#               endif
					va_end(args);
					set_result(err);
					set_desc(buffer);
					return true;
				}
				return false;
			}
			::pilo::err_t stable_test_case::error(::pilo::err_t err, const char* fmt, ...)
			{
				char buffer[256] = { 0 };
				va_list args;
				va_start(args, fmt);
#               if defined(WINDOWS)
				_vsnprintf_s(buffer, 256, _TRUNCATE, fmt, args);
#               else
				vsnprintf(buffer, sizeof(buffer), fmt, args);
#               endif
				va_end(args);
				set_result(err);
				set_desc(buffer);
				return PILO_OK;
			}

		}
	}	
}

