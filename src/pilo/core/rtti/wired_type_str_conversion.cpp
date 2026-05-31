#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			std::string convert_to_str(bool v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return (v) ? std::string("false") : std::string("true");
			}
			std::string convert_to_str(::pilo::i8_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[16] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(::pilo::u8_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[16] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(::pilo::i16_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[16] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(::pilo::u16_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[16] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(::pilo::i32_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[32] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(::pilo::u32_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[32] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(::pilo::i64_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[32] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(::pilo::u64_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[32] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(::pilo::f32_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[32] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(::pilo::f64_t v, ::pilo::err_t*, ::pilo::i32_t)
			{
				char buffer[32] = { 0 };
				::pilo::core::string::number_to_string(buffer, sizeof(buffer), v, nullptr);
				return std::string(buffer);
			}
			std::string convert_to_str(const char* v, ::pilo::err_t* err, ::pilo::i32_t len)
			{	
				return ::pilo::core::string::cstr_to_str<::pilo::i32_t>(v, len, err);
			}
			std::string convert_to_str(const std::string* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr(PERR_NULL_PARAM);
					return std::string("");
				}
				return std::string(*v);
			}
			std::string convert_to_str(const std::string& v, ::pilo::err_t*, ::pilo::i32_t)
			{
				return std::string(v);
			}
			std::string convert_to_str(const::pilo::tlv* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::mk_perr(PERR_OK_WITH_INFO);
					return std::string("");
				}
				else
				{
					return v->as_str(err);
				}
			}
		}
	}
}