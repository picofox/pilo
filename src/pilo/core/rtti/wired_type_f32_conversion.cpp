#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::f32_t convert_to_f32(bool v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (v) ? 1.0f : 0.0f;
			}
			::pilo::f32_t convert_to_f32(::pilo::i8_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f32_t)v;
			}
			::pilo::f32_t convert_to_f32(::pilo::u8_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f32_t) v;
			} 
			::pilo::f32_t convert_to_f32(::pilo::i16_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f32_t)v;
			}
			::pilo::f32_t convert_to_f32(::pilo::u16_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f32_t)v;
			}
			::pilo::f32_t convert_to_f32(::pilo::i32_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f32_t)v;
			}
			::pilo::f32_t convert_to_f32(::pilo::u32_t v, ::pilo::err_t*, ::pilo::i32_t)
			{				
				return (::pilo::f32_t)v;
			}
			::pilo::f32_t convert_to_f32(::pilo::i64_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f32_t)v;
			}
			::pilo::f32_t convert_to_f32(::pilo::u64_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f32_t)v;
			}
			::pilo::f32_t convert_to_f32(::pilo::f32_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f32_t)v;
			}
			::pilo::f32_t convert_to_f32(::pilo::f64_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }				
				return (::pilo::f32_t)v;
			}
			::pilo::f32_t convert_to_f32(const char* v, ::pilo::err_t* err, ::pilo::i32_t len)
			{
				::pilo::f32_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v, len);
				else
					::pilo::core::string::string_to_number(iv, v, len - 1);
				return iv;
			}
			::pilo::f32_t convert_to_f32(const std::string* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				::pilo::f32_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				else
					::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				return iv;
			}
			::pilo::f32_t convert_to_f32(const std::string& v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				::pilo::f32_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				else
					::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				return iv;
			}
			::pilo::f32_t convert_to_f32(const::pilo::tlv* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO);
					return 0;
				}
				else
				{
					return v->as_f32(err);
				}
			}
		}
	}
}