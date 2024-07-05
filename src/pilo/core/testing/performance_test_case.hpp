#pragma once

#include "../../pilo.hpp"
#include <functional>
#include <atomic>
#include "../datetime/timestamp.hpp"
#include "../threading/spin_mutex.hpp"
#include "../threading/native_mutex.hpp"
namespace pilo
{
	namespace core 
	{
		namespace testing
		{
			class performance_test_case {
			public: //types
				typedef std::function<int(performance_test_case* case_ptr)> case_function_type;
				typedef std::function<std::string(const performance_test_case * lowest_pivot_case, const performance_test_case * highest_pivot_case, const performance_test_case* case_ptr)> score_function_type;
				typedef ::pilo::core::threading::native_mutex lock_type;

			public: //constructors and destructors
				performance_test_case(::pilo::i32_t idx, const char* name, case_function_type case_func, void* arg, ::pilo::i64_t n_trans, ::pilo::i64_t n_secs, ::pilo::i32_t concurrency, performance_test_case::score_function_type score_func);
				~performance_test_case();
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

				
				inline ::pilo::f64_t persist_seconds() const  { return ((::pilo::f64_t) _persist_milli_seconds) / 1000.f;  }
				inline ::pilo::i64_t in_trans_count() const { return _in_trans_count;  }
				inline ::pilo::i64_t out_trans_count() const { return _out_trans_count; }
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
					
				inline ::pilo::i64_t tps_in() const
				{
					return  (::pilo::i64_t) (((::pilo::f64_t)_in_trans_count.load()) / ((::pilo::f64_t)_in_milli_seconds / 1000.f));
				}

				inline ::pilo::i64_t tps_out() const
				{
					return (::pilo::i64_t) (((::pilo::f64_t)_out_trans_count.load()) / ((::pilo::f64_t)_out_milli_seconds / 1000.f));
				}

				inline ::pilo::i64_t bps_in() const
				{
					return  (::pilo::i64_t)(((::pilo::f64_t)_in_bytes_count.load()) / ((::pilo::f64_t)_in_milli_seconds / 1000.f));
				}

				inline ::pilo::i64_t bps_out() const
				{
					return  (::pilo::i64_t)(((::pilo::f64_t)_out_bytes_count.load()) / ((::pilo::f64_t)_out_milli_seconds / 1000.f));
				}

				inline ::pilo::i64_t tps_overall() const
				{
					return  (::pilo::i64_t)(((::pilo::f64_t)_out_trans_count.load()) / this->persist_seconds() );
				}

				inline ::pilo::i64_t bps_overall() const
				{
					return  (::pilo::i64_t)(((::pilo::f64_t)_out_bytes_count.load()) / this->persist_seconds());
				}

				inline void inc_in_trans_count(::pilo::i64_t val) { _in_trans_count.fetch_add(val); }
				inline void inc_out_trans_count(::pilo::i64_t val) { _out_trans_count.fetch_add(val); }
				inline void inc_in_bytes_count(::pilo::i64_t bs) { _in_bytes_count.fetch_add(bs); }
				inline void inc_out_bytes_count(::pilo::i64_t bs) { _out_bytes_count.fetch_add(bs); }
				inline void inc_in_milli_seconds(::pilo::i64_t t) { _in_milli_seconds.fetch_add(t); }
				inline void inc_out_milli_seconds(::pilo::i64_t t) { _out_milli_seconds.fetch_add(t); }
				inline ::pilo::i32_t concurrency() const { return _concurrency;  }
				inline ::pilo::i64_t in_milli_seconds() const { return _in_milli_seconds;  }
				inline ::pilo::i64_t out_milli_seconds() const { return _out_milli_seconds; }
				inline score_function_type score_function() { return _score_func;  }
				inline lock_type& get_lock() { return _mutex; }

			private:
				::pilo::i32_t		_index;				
				::std::string		_name;
				case_function_type	_func;
				void* _arg;
				::pilo::i64_t		_target_trans_count;
				::pilo::i64_t		_target_persist_millisecs;
				::pilo::i32_t		_concurrency;
				performance_test_case::score_function_type _score_func;

				std::atomic<::pilo::err_t>		_result;
				std::atomic<::pilo::i64_t>		_in_trans_count;
				std::atomic<::pilo::i64_t>		_out_trans_count;
				std::atomic<::pilo::i64_t>		_in_bytes_count;
				std::atomic<::pilo::i64_t>		_out_bytes_count;
				std::atomic<::pilo::i64_t>		_in_milli_seconds;
				std::atomic<::pilo::i64_t>		_out_milli_seconds;
				::pilo::i64_t		_persist_milli_seconds;
				::pilo::i64_t		_begin_ts;			
				::std::string		_desc;
				mutable lock_type _mutex;
			};
		}
	}	
}
