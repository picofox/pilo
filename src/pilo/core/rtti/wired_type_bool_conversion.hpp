#pragma once

#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			bool convert_to_bool(bool v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::i8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::u8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::i16_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::u16_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::i32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::u32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::i64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::u64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::f32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(::pilo::f64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(const char* v,   ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(const std::string* v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(const std::string& v, ::pilo::err_t* err =nullptr, ::pilo::i32_t len = -1);
			bool convert_to_bool(const::pilo::tlv* v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
		}
	}
}