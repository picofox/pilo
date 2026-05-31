#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::u8_t convert_to_u8(bool v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (v) ? 1 : 0;
			}
			::pilo::u8_t convert_to_u8(::pilo::i8_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (::pilo::i8_t) v;
			}
			::pilo::u8_t convert_to_u8(::pilo::u8_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return v;
			}
			::pilo::u8_t convert_to_u8(::pilo::i16_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v < 0 || v > PILO_UINT8_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::u8_t) v;
			}
			::pilo::u8_t convert_to_u8(::pilo::u16_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v < 0 || v > PILO_UINT8_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::u8_t)v;
			}
			::pilo::u8_t convert_to_u8(::pilo::i32_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v < 0 || v > PILO_UINT8_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::u8_t)v;
			}
			::pilo::u8_t convert_to_u8(::pilo::u32_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v < 0 || v > PILO_UINT8_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::u8_t)v;
			}
			::pilo::u8_t convert_to_u8(::pilo::i64_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v < 0 || v > PILO_UINT8_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::u8_t)v;
			}
			::pilo::u8_t convert_to_u8(::pilo::u64_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v < 0 || v > PILO_UINT8_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::u8_t)v;
			}
			::pilo::u8_t convert_to_u8(::pilo::f32_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v < 0 || v > (::pilo::f32_t) PILO_UINT8_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::u8_t)v;
			}
			::pilo::u8_t convert_to_u8(::pilo::f64_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v < 0 || v >(::pilo::f64_t) PILO_UINT8_MAX)
				{
					if (err != nullptr) { *err = ::pilo::mk_perr(PERR_OK_WITH_INFO); }
				}
				return (::pilo::u8_t)v;
			}
			::pilo::u8_t convert_to_u8(const char* v, ::pilo::err_t* err, ::pilo::i32_t len)
			{
				::pilo::i32_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v, len);
				else
					::pilo::core::string::string_to_number(iv, v, len);
				return convert_to_u8(iv, err, len);
			}
			::pilo::u8_t convert_to_u8(const std::string* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				::pilo::i32_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				else
					::pilo::core::string::string_to_number(iv, v->c_str(), v->size());
				return convert_to_u8(iv, err);
			}
			::pilo::u8_t convert_to_u8(const std::string& v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				::pilo::i32_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				else
					::pilo::core::string::string_to_number(iv, v.c_str(), v.size());
				return convert_to_u8(iv, err);
			}
			::pilo::u8_t convert_to_u8(const::pilo::tlv* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::mk_perr(PERR_OK_WITH_INFO);
					return 0;
				}
				else
				{
					return v->as_u8(err);
				}
			}
		}
	}
}