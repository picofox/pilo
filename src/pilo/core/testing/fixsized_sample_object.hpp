#pragma once

#include "../../pilo.hpp"
#include <string>
#include "../string/constants.hpp"
#include "../string/string_operation.hpp"

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			template <::pilo::i64_t _UnitSize>
			class fixsized_sample_object
			{
			public:
				fixsized_sample_object()
				{
					_m_data[0] = '!';
				}

				inline bool is_set() const { return _m_data[0] == '!'; }

				inline void set(::pilo::i64_t index_begin = -1)
				{
					if (index_begin != -1)
					{
						index_begin = index_begin % ::pilo::core::string::constants<char>::alpha_num_length();
					}
					else
					{
						index_begin = 0;
					}					

					for (size_t i = 0;  i < sizeof(this->_m_data); i++)
					{
						_m_data[i] = ::pilo::core::string::constants<char>::alpha_num()[index_begin];
						index_begin++;
						if (index_begin >= ::pilo::core::string::constants<char>::alpha_num_length())
							index_begin %= ::pilo::core::string::constants<char>::alpha_num_length();
					}
				}

				inline bool validate(::pilo::i64_t index_begin = -1) const
				{
					if (index_begin == -1) 
					{
						if (std::isupper(_m_data[0]))
							index_begin = _m_data[0] - 'A';
						else if (std::islower(_m_data[0]))
							index_begin = _m_data[0] - 'a' + 26;
						else if (std::isdigit(_m_data[0]))
							index_begin = _m_data[0] - 'a' + 52;
						else
							return false;
					}
					else
					{
						index_begin = index_begin % ::pilo::core::string::constants<char>::alpha_num_length();
					}

					for (size_t i = 0; i < sizeof(this->_m_data); i++)
					{
						if (_m_data[i] != ::pilo::core::string::constants<char>::alpha_num()[index_begin])
						{
							return false;
						}
						index_begin++;
						if (index_begin >= ::pilo::core::string::constants<char>::alpha_num_length())
							index_begin %= ::pilo::core::string::constants<char>::alpha_num_length();
					}

					
					return true;
				}

			protected:
				char _m_data[_UnitSize];

			private:
			};

			typedef ::pilo::core::testing::fixsized_sample_object<128> sample_128b_object;
			typedef ::pilo::core::testing::fixsized_sample_object<64> sample_64b_object;
			typedef ::pilo::core::testing::fixsized_sample_object<32> sample_32b_object;

			typedef ::pilo::core::testing::fixsized_sample_object<129> sample_129b_object;
			typedef ::pilo::core::testing::fixsized_sample_object<65> sample_65b_object;
			typedef ::pilo::core::testing::fixsized_sample_object<33> sample_33b_object;
		}
	}
}