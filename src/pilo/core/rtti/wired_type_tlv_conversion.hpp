#pragma once

#include "../../pilo.hpp"



namespace pilo
{
	class tlv;
	namespace core
	{
		namespace rtti
		{
			::pilo::tlv* convert_to_tlv(bool v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::i8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::u8_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::i16_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::u16_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::i32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::u32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::i64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::u64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::f32_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::f64_t v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(const char* v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(const std::string* v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(const std::string& v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(const ::pilo::tlv* v, ::pilo::err_t* err = nullptr, ::pilo::i32_t len = -1);
			::pilo::tlv* convert_to_tlv(::pilo::tlv* v, ::pilo::err_t*, ::pilo::i32_t);
		}
	}
}