#pragma once

#include "../../pilo.hpp"
#include <functional>
#include "../threading/spin_mutex.hpp"

namespace pilo
{
	namespace core 
	{
		namespace testing
		{
			class func_test_case {
			public: //types
				typedef std::function<int(func_test_case* case_ptr)> case_function_type;

			public: //constructors and destructors
				func_test_case(::pilo::i32_t idx, const char* name, case_function_type case_func, void* arg);
				~func_test_case();
				void reset();
				bool is_valid() const;
				::pilo::err_t execute();
				::pilo::err_t validate_result();
				const char* to_cstring(char* buffer, ::pilo::i64_t buf_len, const char* rstr);
				inline ::pilo::i32_t	index() const { return _index; }
				inline const char* name() const { return _name.c_str(); }
				inline const char* desc() const { return _desc.c_str(); }
				inline ::pilo::err_t result() const { return _result.load(); }
				inline ::pilo::f64_t time_cost() const { return ((double)_nano_sec_cost) / 1000000000.f; }
				inline void set_desc(const char* desc) { _desc = desc; }
				inline void set_result(::pilo::err_t e) { _result.store(e); }

				inline ::pilo::core::threading::spin_mutex* get_lock() { return &_mutex;  }
				bool check_error(::pilo::err_t err, ::pilo::err_t cerr, const char* format, ...);
				bool check_error(::pilo::err_t err, const char* format, ...);
				::pilo::err_t error(::pilo::err_t err, const char* format, ...);

			private:
				::pilo::i32_t		_index;
				::std::string		_name;
				case_function_type	_func;
				void* _arg;


				std::atomic<::pilo::err_t> _result;
				::std::string		_desc;
				::pilo::i64_t		_nano_sec_cost;
				::pilo::core::threading::spin_mutex _mutex;
				
			};
		}
	}	
}
