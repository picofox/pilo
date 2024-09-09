#include "wired_type.hpp"
#include "../string/string_operation.hpp"

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            static const char s_key_type_chars[] = "-cCwWdDqQs";
            static const char s_value_type_intrin_chars[] = "-cCwWdDqQbfFBst";

            ::pilo::u8_t wired_type::s_char_to_key_type(char ch)
            {
                for (int i = 0; i < sizeof(s_key_type_chars); i++) {
                    if (s_key_type_chars[i] == ch)
                        return (::pilo::u8_t) i;
                }
                return ::pilo::core::rtti::wired_type::key_type_na;
            }

            ::pilo::u16_t wired_type::s_char_to_value_type(char ch)
            {
                for (int i = 0; i < sizeof(s_value_type_intrin_chars); i++) {
                    if (s_value_type_intrin_chars[i] == ch)
                        return (::pilo::u16_t) i;
                }
                return ::pilo::core::rtti::wired_type::value_type_na;
            }

            char wired_type::s_key_type_to_char(::pilo::u8_t tp)
            {
                return s_key_type_chars[tp];
            }

            char wired_type::s_val_type_to_char(::pilo::u16_t tp)
            {
                return s_value_type_intrin_chars[tp];
            }

            ::pilo::err_t wired_type::s_parse_cstr_type(::pilo::u8_t& wt, ::pilo::u8_t& kt, ::pilo::u16_t& vt, const char* cstr, ::pilo::i64_t len)
            {
                if (cstr == nullptr)
                    return ::pilo::mk_perr(PERR_NULL_PARAM);

                if (len < 0)
                    len = ::pilo::core::string::character_count(cstr);

                if (len == 0) {
                    wt = ::pilo::core::rtti::wired_type::wrapper_na;
                    kt = ::pilo::core::rtti::wired_type::key_type_na;
                    vt = ::pilo::core::rtti::wired_type::value_type_na;
                }
                else if (len == 1) {
                    wt = ::pilo::core::rtti::wired_type::wrapper_single;
                    kt = ::pilo::core::rtti::wired_type::key_type_na;
                    vt = s_char_to_value_type(cstr[0]);
                    if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);                                              
                    }                        
                }
                else if (len == 2)
                {
                    if (cstr[0] != '*')
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                    wt = ::pilo::core::rtti::wired_type::wrapper_array;
                    kt = ::pilo::core::rtti::wired_type::key_type_na;
                    vt = s_char_to_value_type(cstr[1]);
                    if (vt == ::pilo::core::rtti::wired_type::value_type_na)
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                else {
                    if (cstr[0] != '#')
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                    wt = ::pilo::core::rtti::wired_type::wrapper_dict;
                    kt = s_char_to_key_type(cstr[1]);
                    if (kt == ::pilo::core::rtti::wired_type::key_type_na)
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                    vt = s_char_to_value_type(cstr[2]);
                    if (vt == ::pilo::core::rtti::wired_type::value_type_na)
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }
                
                

                return PILO_OK;
            }

            ::pilo::err_t wired_type::s_to_wired_type_cstr(char* buffer, ::pilo::i64_t capa, ::pilo::u8_t wt, ::pilo::u8_t kt, ::pilo::u16_t vt)
            {
                if (wt == ::pilo::core::rtti::wired_type::wrapper_na) {
                    if (kt == ::pilo::core::rtti::wired_type::key_type_na && vt == ::pilo::core::rtti::wired_type::value_type_na) {
                        buffer[0] = 0;                        
                    }
                    else {
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                    }                    
                }
                else if (wt == ::pilo::core::rtti::wired_type::wrapper_dict) {
                    if (capa < 4) {
                        return ::pilo::mk_perr(PERR_LEN_TOO_SAMLL);
                    }
                    buffer[0] = '#';
                    if (kt == ::pilo::core::rtti::wired_type::key_type_na) {
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                    }
                    buffer[1] = s_key_type_to_char(kt);
                    if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                    }
                    buffer[2] = s_val_type_to_char(vt);
                    buffer[3] = 0;
                }
                else if (wt == ::pilo::core::rtti::wired_type::wrapper_array) {
                    if (capa < 3) {
                        return ::pilo::mk_perr(PERR_LEN_TOO_SAMLL);
                    }
                    buffer[0] = '*';
                    if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
                        return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                    }
                    buffer[1] = s_val_type_to_char(vt);
                    buffer[2] = 0;
                }
                else if (wt == ::pilo::core::rtti::wired_type::wrapper_single) {
                    if (vt == ::pilo::core::rtti::wired_type::value_type_na) {
                        buffer[0] = 0;
                    }
                    else {
                        if (capa < 2) {
                            return ::pilo::mk_perr(PERR_LEN_TOO_SAMLL);
                        }
                        buffer[0] = s_val_type_to_char(vt);
                        buffer[1] = 0;
                    }                    
                }

                return PILO_OK;
            }

            ::pilo::err_t wired_type::s_parse_cstr_bool(bool& bv, const char* cstr, ::pilo::i64_t len)
            {
                if (cstr == nullptr)
                    return ::pilo::mk_perr(PERR_NULL_PARAM);

                if (len <= 0)
                    len = ::pilo::core::string::character_count(cstr);

                if (len == 1) {
                    if (cstr[0] == 'T' || cstr[0] == 't') {
                        bv = true;
                        return PILO_OK;
                    } else if (cstr[0] == 'F' || cstr[0] == 'f') {
                        bv = false;
                        return PILO_OK;
                    }                        
                }
                else if (len >= 5) {
                    if (0 == ::pilo::core::string::i_compare(cstr, 0, "false", 0, 5)) {
                        bv = false;
                        return PILO_OK;
                    }                        
                } else if (len >= 4) {
                    if (0 == ::pilo::core::string::i_compare(cstr, 0, "true", 0, 4)) {
                        bv = true;
                        return PILO_OK;
                    }
                }

                return ::pilo::mk_perr(PERR_INVALID_PARAM);
            }



        }
    }
}
