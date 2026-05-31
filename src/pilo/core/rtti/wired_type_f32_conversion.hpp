#pragma once

#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::f32_t convert_to_f32(bool v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::i8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::u8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::i16_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::u16_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::i32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::u32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::i64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::u64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::f32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(::pilo::f64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(const char* v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(const std::string* v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(const std::string& v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::f32_t convert_to_f32(const ::pilo::tlv * v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
		}
	}
}