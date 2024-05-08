#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::u32_t convert_to_u32(bool v, ::pilo::err_t*, pilo::i32_t)
			{
				return (v) ? 1 : 0;
			}
			::pilo::u32_t convert_to_u32(::pilo::i8_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::u32_t)v;
			}
			::pilo::u32_t convert_to_u32(::pilo::u8_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::u32_t) v;
			} 
			::pilo::u32_t convert_to_u32(::pilo::i16_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::u32_t)v;
			}
			::pilo::u32_t convert_to_u32(::pilo::u16_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::u32_t)v;
			}
			::pilo::u32_t convert_to_u32(::pilo::i32_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::u32_t)v;
			}
			::pilo::u32_t convert_to_u32(::pilo::u32_t v, ::pilo::err_t*, pilo::i32_t)
			{				

				return (::pilo::u32_t)v;
			}
			::pilo::u32_t convert_to_u32(::pilo::i64_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v < 0 || v > PILO_UINT32_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				}
				return (::pilo::u32_t)v;
			}
			::pilo::u32_t convert_to_u32(::pilo::u64_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v > PILO_UINT32_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				}
				return (::pilo::u32_t)v;
			}
			::pilo::u32_t convert_to_u32(::pilo::f32_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v < 0 || v > PILO_UINT32_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				}
				return (::pilo::u32_t)v;
			}
			::pilo::u32_t convert_to_u32(::pilo::f64_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v < 0 || v > PILO_UINT32_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				}
				return (::pilo::u32_t)v;
			}
			::pilo::u32_t convert_to_u32(const char* v, ::pilo::err_t* err, pilo::i32_t len)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v, len);
				else
					::pilo::core::string::string_to_number(iv, v, len);
				return convert_to_u32(iv, err);
			}
			::pilo::u32_t convert_to_u32(const std::string* v, ::pilo::err_t* err, pilo::i32_t)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				else
					::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				return convert_to_u32(iv, err);
			}
			::pilo::u32_t convert_to_u32(const std::string& v, ::pilo::err_t* err, pilo::i32_t)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				else
					::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				return convert_to_u32(iv, err);
			}
			::pilo::u32_t convert_to_u32(const::pilo::tlv* v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO);
					return 0;
				}
				else
				{
					return v->as_i32(err);
				}
			}
		}
	}
}