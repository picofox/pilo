#pragma once

#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			std::string convert_to_str(bool v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::i8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::u8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::i16_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::u16_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::i32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::u32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::i64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::u64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::f32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(::pilo::f64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(const char* v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(const std::string* v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(const std::string& v,  ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			std::string convert_to_str(const ::pilo::tlv * v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
		}
	}
}