#pragma once

#include <vector>
#include <string>
#include <iomanip>
#include <sstream>
#include <mutex>
#include "../../pilo.hpp"
#include "../threading/dummy_mutex.hpp"
#include "./timestamp.hpp"


namespace pilo
{
	namespace core
	{
		namespace datetime
		{
			template <typename _Lock = pilo::core::threading::dummy_mutex>
			class stop_watch
			{
			public:
				typedef _Lock lock_type;
				stop_watch()
				{

				}


				void begin(const char* str)
				{
					std::lock_guard<lock_type> locker(_mutex);
					this->_descs.clear();
					this->_records.clear();
					this->_records.push_back(::pilo::core::datetime::timestamp_micro_steady());
					this->_descs.push_back(::std::string(str));
				}

				void stop(const char* str)
				{
					std::lock_guard<lock_type> locker(_mutex);
					this->_records.push_back(::pilo::core::datetime::timestamp_micro_steady());
					this->_descs.push_back(::std::string(str));
				}

				void mark(const char* str)
				{
					std::lock_guard<lock_type> locker(_mutex);
					this->_records.push_back(::pilo::core::datetime::timestamp_micro_steady());
					this->_descs.push_back(::std::string(str));
				}

				::pilo::i64_t record_diff_begin(::pilo::i64_t idx)
				{
					std::lock_guard<lock_type> locker(_mutex);
					return _records[idx] - _records[0];
				}

				::pilo::i64_t record_diff_begin()
				{
					std::lock_guard<lock_type> locker(_mutex);
					return _records[_records.size() - 1] - _records[0];
				}

				::pilo::i64_t record_at(::pilo::i64_t idx)
				{
					std::lock_guard<lock_type> locker(_mutex);
					return _records[idx];
				}

				::pilo::i64_t record_diff_last(::pilo::i64_t idx)
				{
					std::lock_guard<lock_type> locker(_mutex);
					return _records[idx] - _records[idx-1];
				}

				std::string to_string() const
				{
					std::lock_guard<lock_type> locker(_mutex);
					::std::stringstream ss;
					if (_records.size() != _descs.size())
					{
						return std::string("[SNH] broken stop watch data.");
					}
					if (_records.size() == 1)
					{
						ss << "Begin @";
						ss << _records[0];
						return ss.str();
					}
					else if (_records.size() == 1)
					{
						return std::string("Empty stop watch data.");
					}

					for (::pilo::i64_t i = 1; i < _records.size(); i++)
					{
						::pilo::i64_t diff = _records[i] - _records[i - 1];
						ss << '[' << _descs[i - 1] << '-' << _descs[i] << "] diff_us:(" << diff << ')';
						if (i < _records.size() - 1)
							ss << ", ";
						else
						{
							::pilo::f64_t fdiff = ((::pilo::f64_t)(_records[i] - _records[0])) / 1000000.0f;
							ss << " total_sec: (" << std::setprecision(6) << fdiff << ")" << std::endl;
						}
					}
					return ss.str();
				}



			private:
				::std::vector<::pilo::i64_t>	_records;
				::std::vector<std::string>	_descs;
				mutable lock_type	_mutex;
			};
		}
	}
}

