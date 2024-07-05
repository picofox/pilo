#include "func_test_case.hpp"
#include <chrono>
#include "../io/formatted_io.hpp"
#include <cstdio>

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			func_test_case::func_test_case(::pilo::i32_t idx, const char* name, case_function_type case_func, void* arg)
				:_index(idx), _name(name), _func(case_func), _arg(arg),  _result(::pilo::mk_perr(PERR_NOOP)), _desc(""), _nano_sec_cost(-1)
			{

			}


			func_test_case::~func_test_case()
			{

			}

			void func_test_case::reset()
			{
				_index = -1;
				_name.clear();
				_desc.clear();
				_func = nullptr;
				_arg = nullptr;
				_result = ::pilo::mk_perr(PERR_NOOP);
				_nano_sec_cost = -1;
			}

			bool func_test_case::is_valid() const
			{
				if ((_index < 0) || (_func == nullptr)) {
					return false;
				}

				return true;
			}

			::pilo::err_t func_test_case::execute()
			{
				if (is_valid())
				{
					_result = ::pilo::mk_perr(PERR_RETRY);
					std::chrono::steady_clock::time_point beg = std::chrono::steady_clock::now();					
					if (_func(this) != PILO_OK) 
					{
						_result = ::pilo::mk_perr(PERR_USER_CANCEL);
					}
					::std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
					_nano_sec_cost = ::std::chrono::duration_cast<::std::chrono::duration<long long, std::nano>>(end - beg).count();

					if (_desc.size() < 1 && _result.load() != PILO_OK)
					{
						_desc = ::pilo::str_err(_result.load(), "");
					}

					return PILO_OK;
				}
				return ::pilo::mk_perr(PERR_INV_OBJECT);

			}

			::pilo::err_t func_test_case::validate_result()
			{
				if (_nano_sec_cost == -1)
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

			const char* func_test_case::to_cstring(char* buffer, ::pilo::i64_t buf_len, const char* rstr)
			{
				::pilo::core::io::string_formated_output(buffer, buf_len, "%08d --[%s]--> [%s] Result: %d, cost %d ns", _index, rstr, _name.c_str(), _result.load(), _nano_sec_cost);

				return buffer;
			}


			bool func_test_case::check_error(::pilo::err_t err, ::pilo::err_t cerr, const char* fmt, ...)
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

			bool func_test_case::check_error(::pilo::err_t err, const char* fmt, ...)
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
			::pilo::err_t func_test_case::error(::pilo::err_t err, const char* fmt, ...)
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

