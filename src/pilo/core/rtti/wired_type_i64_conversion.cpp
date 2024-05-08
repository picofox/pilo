#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::i64_t convert_to_i64(bool v, ::pilo::err_t*)
			{
				return (v) ? 1 : 0;
			}
			::pilo::i64_t convert_to_i64(::pilo::i8_t v, ::pilo::err_t*)
			{
				return (::pilo::i64_t) v;
			}
			::pilo::i64_t convert_to_i64(::pilo::u8_t v, ::pilo::err_t*)
			{
				return (::pilo::i64_t) v;
			}
			::pilo::i64_t convert_to_i64(::pilo::i16_t v, ::pilo::err_t*)
			{
				return (::pilo::i64_t) v;
			}
			::pilo::i64_t convert_to_i64(::pilo::u16_t v, ::pilo::err_t*)
			{
				return (::pilo::i64_t)v;
			}
			::pilo::i64_t convert_to_i64(::pilo::i32_t v, ::pilo::err_t*)
			{
				printf("i32 to i64\n");
				return (::pilo::i64_t)v;
			}
			::pilo::i64_t convert_to_i64(::pilo::u32_t v, ::pilo::err_t*)
			{
				return (::pilo::i64_t)v;
			}
			::pilo::i64_t convert_to_i64(::pilo::i64_t v, ::pilo::err_t*)
			{
				return (::pilo::i64_t)v;
			}
			::pilo::i64_t convert_to_i64(::pilo::u64_t v, ::pilo::err_t*)
			{
				return (::pilo::i64_t)v;
			}
			::pilo::i64_t convert_to_i64(::pilo::f32_t v, ::pilo::err_t* )
			{
				return (::pilo::i64_t)v;
			}
			::pilo::i64_t convert_to_i64(::pilo::f64_t v, ::pilo::err_t* )
			{
				return (::pilo::i64_t)v;
			}
			::pilo::i64_t convert_to_i64(const char* v, ::pilo::i64_t len, ::pilo::err_t* err)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v, len-1);
				else
					::pilo::core::string::string_to_number(iv, v, len - 1);
				return iv;
			}
			::pilo::i64_t convert_to_i64(const std::string* v, ::pilo::err_t* err)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				else
					::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				return iv;
			}
			::pilo::i64_t convert_to_i64(const std::string& v, ::pilo::err_t* err)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				else
					::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				return iv;
			}
			::pilo::i64_t convert_to_i64(const::pilo::tlv* v, ::pilo::err_t* err)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO);
					return 0;
				}
				else
				{
					return v->as_i64(err);
				}
			}
		}
	}
}