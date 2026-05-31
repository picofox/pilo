#pragma once

#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::u8_t convert_to_u8(bool v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::i8_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::u8_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::i16_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::u16_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::i32_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::u32_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::i64_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::u64_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::f32_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(::pilo::f64_t v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(const char* v,  ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(const std::string* v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(const std::string& v,  ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
			::pilo::u8_t convert_to_u8(const ::pilo::tlv * v, ::pilo::err_t* err=nullptr, ::pilo::i32_t len = -1 );
		}
	}
}