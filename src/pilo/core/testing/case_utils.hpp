#pragma once

#include "../../pilo.hpp"
#include "../../tlv.hpp"
#include <memory>
#include "../io/path.hpp"

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			::pilo::i32_t create_random_memory_sample(std::string& str, ::pilo::i32_t beg_size, ::pilo::i32_t end_size);
			char* create_random_memory_sample(char* buffer, ::pilo::i32_t capa, ::pilo::i32_t & rlen,  ::pilo::i32_t beg_size, ::pilo::i32_t end_size);
			::pilo::tlv* create_random_array_tlv(::pilo::i32_t level =0 );
			::pilo::tlv* create_random_dict_tlv(::pilo::u8_t, ::pilo::u16_t, ::pilo::i32_t);
			::pilo::tlv* create_random_single_tlv(::pilo::u16_t type = ::pilo::core::rtti::wired_type::value_type_na);

			int validate_path(::pilo::core::io::path* p, const char* pc, ::pilo::pathlen_t len
				, const char* fn, const char* en, const char* pardir, const char* bn, ::pilo::i8_t at);
		}
	}
}