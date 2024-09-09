#include "./tlv_driver_interface.hpp"
#include "../../tlv.hpp"

namespace pilo {
    namespace core {
        namespace ml {
           
            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::i8_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                if (err != PILO_OK)
                    return err;
                v = vp->as_i8(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::u8_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                if (err != PILO_OK)
                    return err;
                v = vp->as_u8(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::i16_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                v = vp->as_i16(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::u16_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                v = vp->as_u16(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::i32_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                v = vp->as_i32(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::u32_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                v = vp->as_u32(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::i64_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                v = vp->as_i64(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::u64_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                v = vp->as_u64(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::f32_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                v = vp->as_f32(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, ::pilo::f64_t& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                v = vp->as_f64(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, std::string& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }

                vp->assign_as_str(v);
                v = vp->as_str(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, bool& v)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                v = vp->as_bool(&err);
                return err;
            }

            ::pilo::err_t tlv_driver_interface::get_value(const char* fqn, const char*& cstr, ::pilo::i32_t& len, bool* is_bytes_as_cstr)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = get_value_node(fqn, err);
                if (err != PILO_OK)
                    return err;
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }

                if (vp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
                {
                    cstr = vp->daynamic_data();
                    if (vp->test_flag(::pilo::tlv::FlagBytesAsCStr)) {
                        len = vp->size() - 1;
                        ::pilo::set_if_ptr_is_not_null(is_bytes_as_cstr, true);
                    }
                    else {
                        len = vp->size();
                        ::pilo::set_if_ptr_is_not_null(is_bytes_as_cstr, true);
                    }
                }
                else if (vp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    std::string* sp = (std::string*)vp->daynamic_data();
                    if (sp == nullptr) {
                        cstr = nullptr;
                        len = -1;
                    }
                    else {
                        cstr = sp->c_str();
                        len = (::pilo::i32_t)sp->size();
                    }
                    ::pilo::set_if_ptr_is_not_null(is_bytes_as_cstr, vp->test_flag(::pilo::tlv::FlagBytesAsCStr));
                }
                else {
                    err = ::pilo::mk_err(PERR_MIS_DATA_TYPE);
                }


                return err;
            }

        }
    }
}
