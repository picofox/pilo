#pragma once

#include "../../pilo.hpp"
#include <functional>
#include <atomic>
#include "../datetime/timestamp.hpp"

namespace pilo
{
	namespace core 
	{
		namespace testing
		{
			class stable_test_case {
			public: //types
				typedef std::function<int(stable_test_case* case_ptr)> case_function_type;

			public: //constructors and destructors
				stable_test_case(::pilo::i32_t idx, const char* name, case_function_type case_func, void* arg, ::pilo::i64_t n_trans, ::pilo::i64_t n_secs);
				~stable_test_case();
				void reset();
				bool is_valid() const;
				::pilo::err_t execute();
				::pilo::err_t validate_result();
				const char* to_cstring(char* buffer, ::pilo::i64_t buf_len, const char* rstr);
				inline ::pilo::i32_t	index() const { return _index; }
				inline const char* name() const { return _name.c_str(); }
				inline ::pilo::i64_t target_trans_count() const { return _target_trans_count;  }
				inline ::pilo::i64_t target_persist_millisecs() const { return _target_persist_millisecs; }
				inline const char* desc() const { return _desc.c_str(); }
				inline ::pilo::err_t result() const { return _result.load(); }
				inline void set_desc(const char* desc) { _desc = desc; }
				inline void inc_trans_count() { _trans_count++; }
				void inc_print_progress();
				inline ::pilo::f64_t persist_seconds() { return ((::pilo::f64_t) _persist_milli_seconds) / 1000.f;  }
				inline ::pilo::i64_t trans_count() const { return _trans_count;  }
				inline void set_result(::pilo::err_t result)
				{ 
					this->_persist_milli_seconds = ::pilo::core::datetime::timestamp_milli_steady() - _begin_ts;
					_result = result; 
				}
				inline void set_begin() 
				{ 
					_result = ::pilo::mk_perr(PERR_RETRY);
					_begin_ts = ::pilo::core::datetime::timestamp_milli_steady(); 
				}
				inline bool timeout() const 
				{ 
					if (_target_persist_millisecs <= 0)
						return false;
					else
						if (::pilo::core::datetime::timestamp_milli_steady() - _begin_ts < _target_persist_millisecs)
							return false;
					return true;
				}

				inline ::pilo::f64_t tps() const
				{
					return  ((::pilo::f64_t)_trans_count.load()) / ((::pilo::f64_t)_persist_milli_seconds / 1000.f);
				}

				bool check_error(::pilo::err_t err, ::pilo::err_t cerr, const char* format, ...);
				bool check_error(::pilo::err_t err, const char* format, ...);
				::pilo::err_t error(::pilo::err_t err, const char* format, ...);


			private:
				::pilo::i32_t		_index;
				::std::string		_name;
				case_function_type	_func;
				void* _arg;
				::pilo::i64_t		_target_trans_count;
				::pilo::i64_t		_target_persist_millisecs;
				::pilo::i32_t		_prog_count;


				std::atomic<::pilo::err_t>		_result;
				std::atomic<::pilo::i64_t>		_trans_count;
				
				::pilo::i64_t		_begin_ts;
				::pilo::i64_t		_persist_milli_seconds;
				::std::string		_desc;
			};
		}
	}	
}
