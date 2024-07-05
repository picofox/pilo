#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::i32_t convert_to_i32(bool v, ::pilo::err_t*, pilo::i32_t)
			{
				return (v) ? 1 : 0;
			}
			::pilo::i32_t convert_to_i32(::pilo::i8_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::i32_t)v;
			}
			::pilo::i32_t convert_to_i32(::pilo::u8_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::i32_t) v;
			}
			::pilo::i32_t convert_to_i32(::pilo::i16_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::i32_t)v;
			}
			::pilo::i32_t convert_to_i32(::pilo::u16_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::i32_t)v;
			}
			::pilo::i32_t convert_to_i32(::pilo::i32_t v, ::pilo::err_t*, pilo::i32_t)
			{
				return (::pilo::i32_t)v;
			}
			::pilo::i32_t convert_to_i32(::pilo::u32_t v, ::pilo::err_t*, pilo::i32_t)
			{				

				return (::pilo::i32_t)v;
			}
			::pilo::i32_t convert_to_i32(::pilo::i64_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v <PILO_INT32_MIN || v > PILO_INT32_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::i32_t)v;
			}
			::pilo::i32_t convert_to_i32(::pilo::u64_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v > PILO_INT32_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::i32_t)v;
			}
			::pilo::i32_t convert_to_i32(::pilo::f32_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v <PILO_INT32_MIN || v > PILO_INT32_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::i32_t)v;
			}
			::pilo::i32_t convert_to_i32(::pilo::f64_t v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v <PILO_INT32_MIN || v > PILO_INT32_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				} 
				return (::pilo::i32_t)v;
			}
			::pilo::i32_t convert_to_i32(const char* v,  ::pilo::err_t* err, pilo::i32_t len)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v, len);
				else
					::pilo::core::string::string_to_number(iv, v, len);
				return convert_to_i32(iv, err);
			}
			::pilo::i32_t convert_to_i32(const std::string* v, ::pilo::err_t* err, pilo::i32_t)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				else
					::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				return convert_to_i32(iv, err);
			}
			::pilo::i32_t convert_to_i32(const std::string& v, ::pilo::err_t* err, pilo::i32_t)
			{
				::pilo::i64_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				else
					::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				return convert_to_i32(iv, err);
			}
			::pilo::i32_t convert_to_i32(const::pilo::tlv* v, ::pilo::err_t* err, pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::mk_perr(PERR_OK_WITH_INFO);
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