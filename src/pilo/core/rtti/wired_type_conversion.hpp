#pragma once

#include "./wired_type_tlv_conversion.hpp"
#include "./wired_type_i8_conversion.hpp"
#include "./wired_type_u8_conversion.hpp"
#include "./wired_type_i16_conversion.hpp"
#include "./wired_type_u16_conversion.hpp"
#include "./wired_type_i32_conversion.hpp"
#include "./wired_type_u32_conversion.hpp"
#include "./wired_type_i64_conversion.hpp"
#include "./wired_type_u64_conversion.hpp"
#include "./wired_type_str_conversion.hpp"
#include "./wired_type_f32_conversion.hpp"
#include "./wired_type_f64_conversion.hpp"
#include "./wired_type_bool_conversion.hpp"
#include "./wired_type_bytes_conversion.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			

			template<typename T>
			inline ::pilo::u8_t to_wired_key_type()
			{	
				PMC_ASSERT(false);
				exit(-1);
				return wired_type::key_type_na;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::i8_t>()
			{
				return wired_type::key_type_i8;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::u8_t>()
			{
				return wired_type::key_type_u8;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::i16_t>()
			{
				return wired_type::key_type_i16;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::u16_t>()
			{
				return wired_type::key_type_u16;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::i32_t>()
			{
				return wired_type::key_type_i32;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::u32_t>()
			{
				return wired_type::key_type_u32;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::i64_t>()
			{
				return wired_type::key_type_i64;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::u64_t>()
			{
				return wired_type::key_type_u64;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<std::string>()
			{
				return wired_type::key_type_str;
			}

			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::i8_t&>()
			{
				return wired_type::key_type_i8;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::u8_t&>()
			{
				return wired_type::key_type_u8;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::i16_t&>()
			{
				return wired_type::key_type_i16;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::u16_t&>()
			{
				return wired_type::key_type_u16;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::i32_t&>()
			{
				return wired_type::key_type_i32;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::u32_t&>()
			{
				return wired_type::key_type_u32;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::i64_t&>()
			{
				return wired_type::key_type_i64;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<::pilo::u64_t&>()
			{
				return wired_type::key_type_u64;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<std::string&>()
			{
				return wired_type::key_type_str;
			}

			template<>
			inline ::pilo::u8_t to_wired_key_type<const ::pilo::i8_t&>()
			{
				return wired_type::key_type_i8;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<const ::pilo::u8_t&>()
			{
				return wired_type::key_type_u8;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<const ::pilo::i16_t&>()
			{
				return wired_type::key_type_i16;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<const ::pilo::u16_t&>()
			{
				return wired_type::key_type_u16;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<const ::pilo::i32_t&>()
			{
				return wired_type::key_type_i32;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<const ::pilo::u32_t&>()
			{
				return wired_type::key_type_u32;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<const ::pilo::i64_t&>()
			{
				return wired_type::key_type_i64;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<const ::pilo::u64_t&>()
			{
				return wired_type::key_type_u64;
			}
			template<>
			inline ::pilo::u8_t to_wired_key_type<const std::string&>()
			{
				return wired_type::key_type_str;
			}

			template<typename T> inline ::pilo::u16_t to_wired_value_type(bool* ) 
			{			
				PMC_ASSERT(false);
				return wired_type::value_type_na;
			}

			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::i8_t&>(bool*) { return wired_type::value_type_i8; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::u8_t&>(bool*) { return wired_type::value_type_u8; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::i16_t&>(bool*) { return wired_type::value_type_i16; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::u16_t&>(bool*) { return wired_type::value_type_u16; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::i32_t&>(bool*) { return wired_type::value_type_i32; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::u32_t&>(bool*) { return wired_type::value_type_u32; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::i64_t&>(bool*) { return wired_type::value_type_i64; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::u64_t&>(bool*) { return wired_type::value_type_u64; }
			template<> inline ::pilo::u16_t to_wired_value_type<bool&>(bool*) { return wired_type::value_type_boolean; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::f32_t&>(bool*) { return wired_type::value_type_f32; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::f64_t&>(bool*) { return wired_type::value_type_f64; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::i8_t>(bool*) { return wired_type::value_type_i8; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::u8_t>(bool*) { return wired_type::value_type_u8; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::i16_t>(bool*) { return wired_type::value_type_i16; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::u16_t>(bool*) { return wired_type::value_type_u16; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::i32_t>(bool*) { return wired_type::value_type_i32; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::u32_t>(bool*) { return wired_type::value_type_u32; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::i64_t>(bool*) { return wired_type::value_type_i64; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::u64_t>(bool*) { return wired_type::value_type_u64; }
			template<> inline ::pilo::u16_t to_wired_value_type<bool>(bool*) { return wired_type::value_type_boolean; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::f32_t>(bool*) { return wired_type::value_type_f32; }
			template<> inline ::pilo::u16_t to_wired_value_type<::pilo::f64_t>(bool*) { return wired_type::value_type_f64; }
			template<> 
			inline ::pilo::u16_t to_wired_value_type<char*>(bool* is_ptr) 
			{ 
				if (is_ptr != nullptr) *is_ptr = true;
				return wired_type::value_type_bytes;
			}
			template<>
			inline ::pilo::u16_t to_wired_value_type<unsigned char*>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = true;
				return wired_type::value_type_bytes;
			}
			template<>
			inline ::pilo::u16_t to_wired_value_type<const char*>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = true;
				return wired_type::value_type_bytes;
			}
			template<>
			inline ::pilo::u16_t to_wired_value_type<const unsigned char*>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = true;
				return wired_type::value_type_bytes;
			}
			template<>
			inline ::pilo::u16_t to_wired_value_type<const ::pilo::tlv*>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = true;
				return wired_type::value_type_tlv;
			}
			template<>
			inline ::pilo::u16_t to_wired_value_type<::pilo::tlv*>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = true;
				return wired_type::value_type_tlv;
			}
			template<>
			inline ::pilo::u16_t to_wired_value_type<std::string *>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = true;
				return wired_type::value_type_str;
			}
			template<>
			inline ::pilo::u16_t to_wired_value_type<std::string>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = false;
				return wired_type::value_type_str;
			}
			template<>
			inline ::pilo::u16_t to_wired_value_type<std::string &>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = false;
				return wired_type::value_type_str;
			}

			template<>
			inline ::pilo::u16_t to_wired_value_type<const std::string&>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = false;
				return wired_type::value_type_str;
			}

			template<>
			inline ::pilo::u16_t to_wired_value_type<std::string&&>(bool* is_ptr)
			{
				if (is_ptr != nullptr) *is_ptr = false;
				return wired_type::value_type_str;
			}
		}
	}
}