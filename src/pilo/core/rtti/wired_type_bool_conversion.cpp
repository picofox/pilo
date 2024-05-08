#include "../../pilo.hpp"
#include "../../tlv.hpp"
#include <cmath>

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			bool convert_to_bool(bool v, ::pilo::err_t* , ::pilo::i32_t)
			{
				return v;
			}
			bool convert_to_bool(::pilo::i8_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				return (v == 0) ? false : true;
			}
			bool convert_to_bool(::pilo::u8_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				return (v == 0) ? false : true;
			}
			bool convert_to_bool(::pilo::i16_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				return (v == 0) ? false : true;
			}
			bool convert_to_bool(::pilo::u16_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				return (v == 0) ? false : true;
			}
			bool convert_to_bool(::pilo::i32_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				return (v == 0) ? false : true;
			}
			bool convert_to_bool(::pilo::u32_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				return (v == 0) ? false : true;
			}
			bool convert_to_bool(::pilo::i64_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				return (v == 0) ? false : true;
			}
			bool convert_to_bool(::pilo::u64_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				return (v == 0) ? false : true;
			}
			bool convert_to_bool(::pilo::f32_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				if (fabs(v) < std::numeric_limits<float>::epsilon())
					return false;
				return true;
			}
			bool convert_to_bool(::pilo::f64_t v, ::pilo::err_t* , ::pilo::i32_t)
			{
				if (fabs(v) < std::numeric_limits<double>::epsilon())
					return false;
				return true;
			}
			bool convert_to_bool(const char* v, ::pilo::err_t* err, ::pilo::i32_t len)
			{
				if (v == nullptr || *v == 0)
					return false;				

				if (len < 0)
					len = (::pilo::i32_t)::pilo::core::string::character_count(v) + 1;
				if (len < 2)
					return false;
					
				if (::pilo::core::string::i_compare(v, 0, "T", 0, 1))
					return true;
				else if (::pilo::core::string::i_compare(v, 0, "F", 0, 1))
					return false;				

				if (len > 4)
				{
					if (::pilo::core::string::i_compare(v, 0, "true", 0, 4))
						return true;
					if (len > 5)
					{
						if (::pilo::core::string::i_compare(v, 0, "false", 0, 4))
							return false;
					}
				}
				::pilo::i32_t iv;
				if (err != nullptr)
					*err = ::pilo::core::string::string_to_number(iv, v, len-1);
				else
					::pilo::core::string::string_to_number(iv, v, len - 1);
				return convert_to_bool(iv, err);
			}
			bool convert_to_bool(const std::string* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return convert_to_bool(v->c_str(), err, (::pilo::i32_t) v->size());
			}
			bool convert_to_bool(const std::string& v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return convert_to_bool(v.c_str(), err, (::pilo::i32_t)v.size());
			}
			bool convert_to_bool(const::pilo::tlv* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO);
					return 0;
				}
				else
				{
					return v->as_bool(err);
				}
			}
		}
	}
}