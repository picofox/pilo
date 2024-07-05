#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::f64_t convert_to_f64(bool v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (v) ? 1.0f : 0.0f;
			}
			::pilo::f64_t convert_to_f64(::pilo::i8_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f64_t)v;
			}
			::pilo::f64_t convert_to_f64(::pilo::u8_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f64_t) v;
			} 
			::pilo::f64_t convert_to_f64(::pilo::i16_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f64_t)v;
			}
			::pilo::f64_t convert_to_f64(::pilo::u16_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f64_t)v;
			}
			::pilo::f64_t convert_to_f64(::pilo::i32_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f64_t)v;
			}
			::pilo::f64_t convert_to_f64(::pilo::u32_t v, ::pilo::err_t*, ::pilo::i32_t)
			{				
				return (::pilo::f64_t)v;
			}
			::pilo::f64_t convert_to_f64(::pilo::i64_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f64_t)v;
			}
			::pilo::f64_t convert_to_f64(::pilo::u64_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f64_t)v;
			}
			::pilo::f64_t convert_to_f64(::pilo::f32_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::f64_t)v;
			}
			::pilo::f64_t convert_to_f64(::pilo::f64_t v, ::pilo::err_t*, ::pilo::i32_t)
			{			
				return (::pilo::f64_t)v;
			}
			::pilo::f64_t convert_to_f64(const char* v, ::pilo::err_t* err, ::pilo::i32_t len)
			{
				::pilo::f64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v, len);
				else
					::pilo::core::string::string_to_number(iv, v, len - 1);
				return iv;
			}
			::pilo::f64_t convert_to_f64(const std::string* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				::pilo::f64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				else
					::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				return iv;
			}
			::pilo::f64_t convert_to_f64(const std::string& v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				::pilo::f64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				else
					::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				return iv;
			}
			::pilo::f64_t convert_to_f64(const::pilo::tlv* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::mk_perr(PERR_OK_WITH_INFO);
					return 0;
				}
				else
				{
					return v->as_f64(err);
				}
			}
		}
	}
}