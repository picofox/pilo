#include "../../pilo.hpp"
#include "../../tlv.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			template<typename T>
			char* stc_create_buffer_write_number(T nv, ::pilo::i32_t need_capa, ::pilo::err_t* err)
			{
				char* ret = (char*)PMF_HEAP_MALLOC(need_capa);
				if (ret == nullptr)
				{
					*err = ::pilo::make_core_error(PES_MEM, PEP_INSUFF);
					return nullptr;
				}
				::pilo::core::string::number_to_string(ret, need_capa, nv);
				return ret;
			}

			char* convert_to_bytes(bool v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				char* ret = nullptr;
				if (v)
				{
					ret = (char*)PMF_HEAP_MALLOC(5);
					if (ret == nullptr)
					{
						*err = ::pilo::make_core_error(PES_MEM, PEP_INSUFF);
						return nullptr;
					}
					::pilo::core::string::n_copyz(ret, 5, "true", 4);
				}
				else
				{
					ret = (char*)PMF_HEAP_MALLOC(6);
					if (ret == nullptr)
					{
						*err = ::pilo::make_core_error(PES_MEM, PEP_INSUFF);
						return nullptr;
					}
					::pilo::core::string::n_copyz(ret, 6, "false", 5);					
				}
				return ret;
			}
			char* convert_to_bytes(::pilo::i8_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 8, err);
			}
			char* convert_to_bytes(::pilo::u8_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 8, err);
			}
			char* convert_to_bytes(::pilo::i16_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 8, err);
			}
			char* convert_to_bytes(::pilo::u16_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 8, err);
			}
			char* convert_to_bytes(::pilo::i32_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 16, err);
			}
			char* convert_to_bytes(::pilo::u32_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 16, err);
			}
			 char* convert_to_bytes(::pilo::i64_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 32, err);
			}
			 char* convert_to_bytes(::pilo::u64_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 32, err);
			}
			 char* convert_to_bytes(::pilo::f32_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 32, err);
			}
			 char* convert_to_bytes(::pilo::f64_t v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				return stc_create_buffer_write_number(v, 64, err);
			}
			char* convert_to_bytes(const char* v, ::pilo::err_t* err, ::pilo::i32_t len)
			{	
				if (v == nullptr)
				{
					return nullptr;
				}
				else if (len < 0)
				{
					len = (::pilo::i32_t) ::pilo::core::string::character_count(v);
					return ::pilo::core::string::duplicate(v, len);
				}	
				else
				{
					char* ret = (char*)PMF_HEAP_MALLOC(len);
					if (ret == nullptr)
					{
						*err = ::pilo::make_core_error(PES_MEM, PEP_INSUFF);
						return nullptr;
					}
					memcpy(ret, v, len);
					return ret;
				}				
			}
			 char* convert_to_bytes(const std::string* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr)
						*err = ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
					return nullptr;
				}
				return convert_to_bytes(v->c_str(), err, (::pilo::i32_t) v->size());
			}
			 char* convert_to_bytes(const std::string& v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				 return convert_to_bytes(v.c_str(), err, (::pilo::i32_t)v.size());
			}
			 char* convert_to_bytes(const::pilo::tlv* v, ::pilo::err_t* err, ::pilo::i32_t)
			{
				if (v == nullptr)
				{
					if (err != nullptr) *err = ::pilo::make_core_error(PES_PARAM, PEP_OK_WITH_INFO);
					return nullptr;
				}
				else
				{
					return v->as_cstr_dup(err,nullptr);
				}
			}
		}
	}
}