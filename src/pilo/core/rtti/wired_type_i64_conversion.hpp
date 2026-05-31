#pragma once

#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::i64_t convert_to_i64(bool v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::i8_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::u8_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::i16_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::u16_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::i32_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::u32_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::i64_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::u64_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::f32_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(::pilo::f64_t v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(const char* v, ::pilo::i64_t len, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(const std::string* v, ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(const std::string& v,  ::pilo::err_t* err);
			::pilo::i64_t convert_to_i64(const ::pilo::tlv * v, ::pilo::err_t* err);
		}
	}
}