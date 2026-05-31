#pragma once

#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			 char* convert_to_bytes(bool v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::i8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::u8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::i16_t v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::u16_t v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::i32_t v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::u32_t v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::i64_t v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::u64_t v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::f32_t v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(::pilo::f64_t v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(const char* v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(const std::string* v, ::pilo::err_t* err =  nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(const std::string& v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			 char* convert_to_bytes(const ::pilo::tlv * v,::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
		}
	}
}