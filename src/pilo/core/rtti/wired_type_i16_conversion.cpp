#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::i16_t convert_to_i16(bool v, ::pilo::err_t*, pilo::i32_t)
			{
				return (v) ? 1 : 0;
			}
			::pilo::i16_t convert_to_i16(::pilo::i8_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::i16_t)v;
			}
			::pilo::i16_t convert_to_i16(::pilo::u8_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::i16_t) v;
			}
			::pilo::i16_t convert_to_i16(::pilo::i16_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::i16_t)v;
			}
			::pilo::i16_t convert_to_i16(::pilo::u16_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::i16_t)v;
			}
			::pilo::i16_t convert_to_i16(::pilo::i32_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v <PILO_INT16_MIN || v > PILO_INT16_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				}
				return (::pilo::i16_t)v;
			}
			::pilo::i16_t convert_to_i16(::pilo::u32_t v, ::pilo::err_t*  err, pilo::i32_t)
			{			
				if (v > PILO_INT16_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				}
				return (::pilo::i16_t)v;
			}
			::pilo::i16_t convert_to_i16(::pilo::i64_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v <PILO_INT16_MIN || v > PILO_INT16_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				}
				return (::pilo::i16_t)v;
			}
			::pilo::i16_t convert_to_i16(::pilo::u64_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v > PILO_INT16_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				}
				return (::pilo::i16_t)v;
			}
			::pilo::i16_t convert_to_i16(::pilo::f32_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v <PILO_INT16_MIN || v > PILO_INT16_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				}
				return (::pilo::i16_t)v;
			}
			::pilo::i16_t convert_to_i16(::pilo::f64_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v <PILO_INT16_MIN || v > PILO_INT16_MAX)
				{
					if (err != nullptr) { *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO); }
				} 
				return (::pilo::i16_t)v;
			}
			::pilo::i16_t convert_to_i16(const char* v, ::pilo::err_t* err, pilo::i32_t len)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v, len);
				else
					::pilo::core::string::string_to_number(iv, v, len);
				return convert_to_i16(iv, err);
			}
			::pilo::i16_t convert_to_i16(const std::string* v, ::pilo::err_t* err, pilo::i32_t)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				else
					::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				return convert_to_i16(iv, err);
			}
			::pilo::i16_t convert_to_i16(const std::string& v, ::pilo::err_t* err, pilo::i32_t)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				else
					::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				return convert_to_i16(iv, err);
			}
			::pilo::i16_t convert_to_i16(const::pilo::tlv* v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO);
					return 0;
				}
				else
				{
					return v->as_i16(err);
				}
			}
		}
	}
}