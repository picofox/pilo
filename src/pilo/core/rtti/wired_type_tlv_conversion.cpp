#include "wired_type_tlv_conversion.hpp"
#include <functional>
#include "../../tlv.hpp"

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            template<typename T>
            static ::pilo::tlv* stc_convert_bool_to_tlv(T v, ::pilo::err_t* err)
            {
                ::pilo::tlv* t = ::pilo::tlv::allocate_single(v);
                if (t == nullptr)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                    return nullptr;
                }
                return t;
            }

            ::pilo::tlv* convert_to_tlv(bool v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::i8_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::u8_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::i16_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::u16_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::i32_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::u32_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::i64_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::u64_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::f32_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(::pilo::f64_t v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                return stc_convert_bool_to_tlv(v, err);
            }

            ::pilo::tlv* convert_to_tlv(const char* v, ::pilo::err_t* err, ::pilo::i32_t len)
            {
                ::pilo::tlv* t = ::pilo::tlv::allocate_single(v, len, false, false);
                if (t == nullptr)
                {
                    if (err != nullptr)
                        *err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                }
                    
                return t;
            }

            ::pilo::tlv* convert_to_tlv(const std::string* v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                ::pilo::tlv* t = ::pilo::tlv::allocate_single(*v);
                if (t == nullptr)
                {
                    if (err != nullptr)
                        *err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                }
                if (err != nullptr) *err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                return nullptr;
            }

            ::pilo::tlv* convert_to_tlv(const std::string& v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                ::pilo::tlv* t = ::pilo::tlv::allocate();
                t->set_single_type(::pilo::core::rtti::wired_type::value_type_na);
                if (t != nullptr)
                {
                    t->set(v);
                    return t;
                }
                if (err != nullptr) *err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                return nullptr;
            }

            ::pilo::tlv* convert_to_tlv(const::pilo::tlv* v, ::pilo::err_t* err, ::pilo::i32_t)
            {
                if (v == nullptr)
                {
                    if (err != nullptr) *err = ::pilo::mk_perr(PERR_NULL_PARAM);
                    return nullptr;
                }                    
                else
                {
                    ::pilo::tlv* ret = v->clone();
                    if (ret == nullptr)
                    {
                        if (err != nullptr) *err = ::pilo::mk_perr(PERR_NULL_PARAM);
                        return nullptr;
                    }
                    return ret;
                }
            }

            ::pilo::tlv* convert_to_tlv(::pilo::tlv* v, ::pilo::err_t* , ::pilo::i32_t)
            {
                return v;
            }
        }
    }
}
