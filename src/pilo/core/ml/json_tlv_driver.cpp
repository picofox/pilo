#include "json_tlv_driver.hpp"
#include "../../external/rapidjson/document.h"
#include "../../external/rapidjson/prettywriter.h"
#include "../../external/rapidjson/stringbuffer.h"
#include "../../tlv.hpp"
#include "../io/file.hpp"

namespace pilo {
    namespace core {
        namespace ml {
            json_tlv_driver::~json_tlv_driver()
            {
                if (_m_root_value != nullptr && _owner) {
                    ::pilo::tlv::deallocate(_m_root_value);
                    _m_root_value = nullptr;
                }
            }

            ::pilo::err_t json_tlv_driver::load(const::pilo::core::io::path* path_ptr)
            {
                ::pilo::core::io::file<> f;
                ::pilo::err_t err = f.open(path_ptr,::pilo::core::io::creation_mode::open_existing
                    , ::pilo::core::io::access_permission::read
                    , ::pilo::core::io::dev_open_flags::none);
                if (err != PILO_OK) {
                    return err;
                }

                char buffer[1024] = { 0 };
                ::pilo::i64_t data_len = 0;
                char* data = f.read_all(buffer, sizeof(buffer), &data_len, &err);
                if (err != PILO_OK) {
                    return err;
                }
                f.close();

                if (data == nullptr) {
                    return ::pilo::mk_perr(PERR_VAL_EMPTY);
                }                

                err = load(data, data_len);
                if (data != nullptr && data != buffer) {
                    ::pilo::core::io::file<>::s_free_all_buffer(&data);
                }

                return err;
            }

            ::pilo::err_t json_tlv_driver::save(const::pilo::core::io::path* dest_path)
            {
                if (this->_m_root_value == nullptr)
                    return ::pilo::mk_perr(PERR_NOOP);                

                ::pilo::core::io::path tmp_file_path;
                ::pilo::err_t err = PILO_OK;

                err = dest_path->make_temp(tmp_file_path, ".tmp", ::pilo::core::io::path::TFNRandPolicyNone);
                if (err != PILO_OK)
                    return err;

                ::rapidjson::Document root_jdoc;
                err = _write_json_object(root_jdoc, _m_root_value, root_jdoc.GetAllocator());
                if (err != PILO_OK)
                    return err;

                ::pilo::core::io::file<> tmp_file;
                err = tmp_file.open(&tmp_file_path, ::pilo::core::io::creation_mode::create_always
                    , ::pilo::core::io::access_permission::write
                    , ::pilo::core::io::dev_open_flags::append);
                rapidjson::StringBuffer jbuffer;
                rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(jbuffer);
                root_jdoc.Accept(writer);
                ::pilo::i64_t wlen = (::pilo::i64_t)jbuffer.GetSize();
                ::pilo::i64_t rlen = 0;
                err = tmp_file.write(jbuffer.GetString(), wlen, &rlen);
                if (err != PILO_OK || wlen < rlen) {
                    tmp_file.close();
                    return ::pilo::mk_err(PERR_IO_WRITE_FAIL);
                }

                tmp_file.close();

                err = ::pilo::core::io::xpf_move_fs_node(true, &tmp_file_path, true, dest_path, true);


                return err;
            }


            ::pilo::err_t json_tlv_driver::load(const char* data, ::pilo::i64_t len)
            {
                ::pilo::err_t err = PILO_OK;
                if (data == nullptr) {
                    return 0;
                }

                if (len < 1) {
                    return -1;
                }

                ::rapidjson::Document jdoc_root;
                ::rapidjson::ParseResult pres = jdoc_root.Parse(data);
                if (!pres) {
                    return ::pilo::mk_perr(PERR_DIR_CREAET_FAIL);
                }

                if (_m_root_value != nullptr) {
                    ::pilo::tlv::deallocate(_m_root_value);
                    _m_root_value = nullptr;
                }
                _m_root_value = ::pilo::tlv::allocate();

                if (jdoc_root.IsNull()) {
                    _m_root_value->reset();
                }
                else if (jdoc_root.IsBool()) {                    
                    _m_root_value->set(jdoc_root.GetBool());
                }
                
                else if (jdoc_root.IsInt()) {
                    _m_root_value->set((::pilo::i64_t)jdoc_root.GetInt());
                }
                else if (jdoc_root.IsInt64()) {
                    _m_root_value->set((::pilo::i64_t )jdoc_root.GetInt64());
                }
                else if (jdoc_root.IsUint()) {
                    _m_root_value->set((::pilo::u64_t)jdoc_root.GetUint());
                }
                else if (jdoc_root.IsUint64()) {
                    _m_root_value->set((::pilo::u64_t)jdoc_root.GetUint64());
                }
                else if (jdoc_root.IsFloat()) {
                    _m_root_value->set(jdoc_root.GetFloat());
                }
                else if (jdoc_root.IsDouble()) {
                    _m_root_value->set(jdoc_root.GetDouble());
                }
                else if (jdoc_root.IsString()) {
                    if (jdoc_root.GetString() == nullptr) {
                        _m_root_value->set_cstr(nullptr, (::pilo::i64_t) -1, false );
                    }
                    else {
                        _m_root_value->set_cstr(jdoc_root.GetString(), (::pilo::i64_t)jdoc_root.GetStringLength(), false);
                    }                    
                }
                else if (jdoc_root.IsObject()) {
                    _m_root_value->set_dict_type(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_tlv);
                    err = _parse_json_object(jdoc_root, _m_root_value);
                    if (err != PILO_OK) {
                        return err;
                    }
                } 
                else if (jdoc_root.IsArray()) {

                }

                return PILO_OK;
            }

            ::pilo::err_t json_tlv_driver::_parse_json_array(::rapidjson::Value& obj, ::pilo::tlv* parent_tlv)
            {
                ::pilo::err_t err = PILO_OK;
                for (::rapidjson::SizeType i = 0; i < obj.Size(); i++) {
                    ::pilo::tlv* tlv = ::pilo::tlv::allocate();
                    if (obj[i].IsNull()) {
                        tlv->set_single_type(::pilo::core::rtti::wired_type::value_type_na);
                    }
                    else if (obj[i].IsBool()) {
                        tlv->set(obj[i].GetBool());
                    }
                    else if (obj[i].IsInt()) {
                        tlv->set(obj[i].GetInt());
                    }
                    else if (obj[i].IsUint()) {
                        tlv->set(obj[i].GetUint());
                    }
                    else if (obj[i].IsInt64()) {
                        tlv->set((::pilo::i64_t) obj[i].GetInt64());
                    }
                    else if (obj[i].IsUint64()) {
                        tlv->set((::pilo::u64_t) obj[i].GetUint64());
                    }
                    else if (obj[i].IsDouble()) {
                        tlv->set(obj[i].GetDouble());
                    }
                    else if (obj[i].IsFloat()) {
                        tlv->set(obj[i].GetFloat());
                    }
                    else if (obj[i].IsString()) {
                        if (obj[i].GetString() == nullptr) {
                            tlv->set_cstr(nullptr, (::pilo::i64_t)-1, false);
                        }
                        else {
                            tlv->set_cstr(obj[i].GetString(), (::pilo::i32_t)obj[i].GetStringLength(), false);
                        }
                    }
                    else if (obj[i].IsObject()) {
                        tlv->set_dict_type(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_tlv);
                        err = _parse_json_object((::rapidjson::Value&)obj[i], tlv);
                        if (err != PILO_OK) {
                            return err;
                        }
                    }
                    else if (obj[i].IsArray()) {
                        tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
                        err = _parse_json_array((::rapidjson::Value&)obj[i], tlv);
                        if (err != PILO_OK) {
                            return err;
                        }
                    }
                    err = parent_tlv->push_back(tlv, -1, true);
                    if (err != PILO_OK) {
                        return err;
                    }
                }

                return PILO_OK;
            }

            ::pilo::err_t json_tlv_driver::_set_dict_of_key_i8(::rapidjson::Value& obj, const::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::i8_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::u8_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::i16_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::u16_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::i32_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::u32_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::i64_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::u64_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                    err = _set_dict_object<::pilo::i8_t, bool>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::f32_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                    err = _set_dict_object<::pilo::i8_t, ::pilo::f64_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                    err = _set_dict_object_bytes<::pilo::i8_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    err = _set_dict_object_str<::pilo::i8_t>(obj, tlvp, allocator);
                } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                    std::map<::pilo::i8_t, ::pilo::tlv*>* map_ptr = (std::map<::pilo::i8_t, ::pilo::tlv*>*) tlvp->daynamic_data();
                    auto cit = map_ptr->begin();
                    for (; cit != map_ptr->end(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        err = _write_json_object(tmp_val, cit->second, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                }
                return err;
            }

            ::pilo::err_t json_tlv_driver::_set_dict_of_key_u8(::rapidjson::Value& obj, const::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::i8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                    err = _set_dict_object<::pilo::u8_t, bool>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::f32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                    err = _set_dict_object<::pilo::u8_t, ::pilo::f64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                    err = _set_dict_object_bytes<::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    err = _set_dict_object_str<::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                    std::map<::pilo::u8_t, ::pilo::tlv*>* map_ptr = (std::map<::pilo::u8_t, ::pilo::tlv*>*) tlvp->daynamic_data();
                    auto cit = map_ptr->begin();
                    for (; cit != map_ptr->end(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        err = _write_json_object(tmp_val, cit->second, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                }
                return err;
            }

            ::pilo::err_t json_tlv_driver::_set_dict_of_key_i16(::rapidjson::Value& obj, const::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::i8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                    err = _set_dict_object<::pilo::i16_t, bool>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::f32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                    err = _set_dict_object<::pilo::i16_t, ::pilo::f64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                    err = _set_dict_object_bytes<::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    err = _set_dict_object_str<::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                    std::map<::pilo::i16_t, ::pilo::tlv*>* map_ptr = (std::map<::pilo::i16_t, ::pilo::tlv*>*) tlvp->daynamic_data();
                    auto cit = map_ptr->begin();
                    for (; cit != map_ptr->end(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        err = _write_json_object(tmp_val, cit->second, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                }
                return err;
            }

            ::pilo::err_t json_tlv_driver::_set_dict_of_key_u16(::rapidjson::Value& obj, const::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::i8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                    err = _set_dict_object<::pilo::u16_t, bool>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::f32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                    err = _set_dict_object<::pilo::u16_t, ::pilo::f64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                    err = _set_dict_object_bytes<::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    err = _set_dict_object_str<::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                    std::map<::pilo::u16_t, ::pilo::tlv*>* map_ptr = (std::map<::pilo::u16_t, ::pilo::tlv*>*) tlvp->daynamic_data();
                    auto cit = map_ptr->begin();
                    for (; cit != map_ptr->end(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        err = _write_json_object(tmp_val, cit->second, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                }
                return err;
            }

            ::pilo::err_t json_tlv_driver::_set_dict_of_key_i32(::rapidjson::Value& obj, const::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::i8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                    err = _set_dict_object<::pilo::i32_t, bool>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::f32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                    err = _set_dict_object<::pilo::i32_t, ::pilo::f64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                    err = _set_dict_object_bytes<::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    err = _set_dict_object_str<::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                    std::map<::pilo::i32_t, ::pilo::tlv*>* map_ptr = (std::map<::pilo::i32_t, ::pilo::tlv*>*) tlvp->daynamic_data();
                    auto cit = map_ptr->begin();
                    for (; cit != map_ptr->end(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        err = _write_json_object(tmp_val, cit->second, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                }
                return err;
            }

            ::pilo::err_t json_tlv_driver::_set_dict_of_key_u32(::rapidjson::Value& obj, const::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::i8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                    err = _set_dict_object<::pilo::u32_t, bool>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::f32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                    err = _set_dict_object<::pilo::u32_t, ::pilo::f64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                    err = _set_dict_object_bytes<::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    err = _set_dict_object_str<::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                    std::map<::pilo::u32_t, ::pilo::tlv*>* map_ptr = (std::map<::pilo::u32_t, ::pilo::tlv*>*) tlvp->daynamic_data();
                    auto cit = map_ptr->begin();
                    for (; cit != map_ptr->end(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        err = _write_json_object(tmp_val, cit->second, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                }
                return err;
            }

            ::pilo::err_t json_tlv_driver::_set_dict_of_key_i64(::rapidjson::Value& obj, const::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::i8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                    err = _set_dict_object<::pilo::i64_t, bool>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::f32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                    err = _set_dict_object<::pilo::i64_t, ::pilo::f64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                    err = _set_dict_object_bytes<::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    err = _set_dict_object_str<::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                    std::map<::pilo::i64_t, ::pilo::tlv*>* map_ptr = (std::map<::pilo::i64_t, ::pilo::tlv*>*) tlvp->daynamic_data();
                    auto cit = map_ptr->begin();
                    for (; cit != map_ptr->end(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        err = _write_json_object(tmp_val, cit->second, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                }
                return err;
            }

            ::pilo::err_t json_tlv_driver::_set_dict_of_key_u64(::rapidjson::Value& obj, const::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::i8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                    err = _set_dict_object<::pilo::u64_t, bool>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::f32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                    err = _set_dict_object<::pilo::u64_t, ::pilo::f64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                    err = _set_dict_object_bytes<::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    err = _set_dict_object_str<::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                    std::map<::pilo::u64_t, ::pilo::tlv*>* map_ptr = (std::map<::pilo::u64_t, ::pilo::tlv*>*) tlvp->daynamic_data();
                    auto cit = map_ptr->begin();
                    for (; cit != map_ptr->end(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        err = _write_json_object(tmp_val, cit->second, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                }
                return err;
            }

            ::pilo::err_t json_tlv_driver::_set_dict_of_key_str(::rapidjson::Value& obj, const::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                    err = _set_str_key_dict_object<::pilo::i8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                    err = _set_str_key_dict_object<::pilo::u8_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                    err = _set_str_key_dict_object<::pilo::i16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                    err = _set_str_key_dict_object<::pilo::u16_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                    err = _set_str_key_dict_object<::pilo::i32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                    err = _set_str_key_dict_object<::pilo::u32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                    err = _set_str_key_dict_object<::pilo::i64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                    err = _set_str_key_dict_object<::pilo::u64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                    err = _set_str_key_dict_object<bool>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                    err = _set_str_key_dict_object<::pilo::f32_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                    err = _set_str_key_dict_object<::pilo::f64_t>(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                    err = _set_str_key_dict_object_bytes(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                    err = _set_str_key_dict_object_str(obj, tlvp, allocator);
                }
                else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                    std::map<std::string, ::pilo::tlv*>* map_ptr = (std::map<std::string, ::pilo::tlv*>*) tlvp->daynamic_data();
                    auto cit = map_ptr->begin();
                    for (; cit != map_ptr->end(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(cit->first.c_str(), allocator);
                        err = _write_json_object(tmp_val, cit->second, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                }
                else {
                    return mk_perr(PERR_INV_PARAM_DT);
                }
                return err;
            }

            ::pilo::tlv* json_tlv_driver::make_value_node(const char* fqn, ::pilo::err_t& err)
            {
                return this->_m_root_value->set_tlv(fqn, err);
            }

            ::pilo::err_t json_tlv_driver::_write_json_object(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)  
            {
                ::pilo::err_t err = PILO_OK;
                if (tlvp->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_single)
                {
                    if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_na)
                    {
                        obj.SetNull();
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                        obj.SetInt((::pilo::i32_t)tlvp->as_i8(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                        obj.SetInt((::pilo::i32_t)tlvp->as_i16(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                        obj.SetInt(tlvp->as_i32(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                        obj.SetInt64(tlvp->as_i64(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                        obj.SetUint((::pilo::u32_t)tlvp->as_u8(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                        obj.SetUint((::pilo::u32_t)tlvp->as_u16(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                        obj.SetUint(tlvp->as_u32(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                        obj.SetUint64(tlvp->as_u64(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                        obj.SetBool(tlvp->as_bool(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                        obj.SetFloat(tlvp->as_f32(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                        obj.SetDouble(tlvp->as_f64(&err));
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                        if (tlvp->daynamic_data() == nullptr) {
                            obj.SetNull();
                        }
                        else {
                            if (tlvp->test_flag(::pilo::tlv::FlagBytesAsCStr)) {
                                ::pilo::i32_t tmp_slen = tlvp->as_str_length();
                                if (tmp_slen < 0) {
                                    obj.SetNull();
                                }
                                else if (tmp_slen == 0) {
                                    obj.SetString("", 0);
                                }
                                else {
                                    obj.SetString(tlvp->daynamic_data(), tmp_slen);
                                }
                                
                            }
                            else {
                                obj.SetString(tlvp->daynamic_data(), tlvp->size());
                            }
                            
                        }                                               
                    }
                    else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                        const std::string* strp = tlvp->as_str_ptr(&err);
                        if (strp == nullptr) {
                            obj.SetNull();
                        }
                        else {
                            obj.SetString(strp->c_str(), (::rapidjson::SizeType) strp->size());
                        }
                    }
                    else
                    {
                        err = ::pilo::mk_err(PERR_INV_VAL_TYPE);
                    }
                }
                else if (tlvp->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
                {
                    obj.SetArray();
                    for (::pilo::i32_t i = 0; i < tlvp->size(); i++) {
                        ::rapidjson::Value tmp_val;
                        if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i8) {
                            ::pilo::i8_t iv = tlvp->get<::pilo::i8_t>(i, &err);
                            tmp_val.SetInt((int)iv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u8) {
                            ::pilo::u8_t iv = tlvp->get<::pilo::u8_t>(i, &err);
                            tmp_val.SetUint((int)iv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i16) {
                            ::pilo::i16_t iv = tlvp->get<::pilo::i16_t>(i, &err);
                            tmp_val.SetInt((int)iv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u16) {
                            ::pilo::u16_t iv = tlvp->get<::pilo::u16_t>(i, &err);
                            tmp_val.SetUint((int)iv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i32) {
                            ::pilo::i32_t iv = tlvp->get<::pilo::i32_t>(i, &err);
                            tmp_val.SetInt((int)iv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u32) {
                            ::pilo::u32_t iv = tlvp->get<::pilo::u32_t>(i, &err);
                            tmp_val.SetUint((int)iv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_i64) {
                            ::pilo::i64_t iv = tlvp->get<::pilo::i64_t>(i, &err);
                            tmp_val.SetInt64((int)iv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_u64) {
                            ::pilo::u64_t iv = tlvp->get<::pilo::u64_t>(i, &err);
                            tmp_val.SetInt64((int)iv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_boolean) {
                            bool bv = tlvp->get<bool>(i, err);
                            tmp_val.SetBool(bv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f32) {
                            ::pilo::f32_t fv = tlvp->get<::pilo::f32_t>(i, &err);
                            tmp_val.SetFloat(fv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_f64) {
                            ::pilo::f64_t fv = tlvp->get<::pilo::f64_t>(i, &err);
                            tmp_val.SetDouble(fv);
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
                            const char* cstrv = tlvp->get_bytes(i, &err);
                            if (cstrv == nullptr) {
                                tmp_val.SetNull();
                            }
                            else {
                                tmp_val.SetString(cstrv, (rapidjson::SizeType) ::pilo::core::string::character_count(cstrv));
                            }
                        } else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
                            const std::string& strp = tlvp->get<std::string>(i, &err);
                            if (strp == ::pilo::_invalid_object_<std::string>::get()) {
                                tmp_val.SetNull();
                            }
                            else {
                                tmp_val.SetString(strp.c_str(), (::rapidjson::SizeType)strp.size());
                            }
                        }
                        else if (tlvp->value_type() == ::pilo::core::rtti::wired_type::value_type_tlv) {
                            const ::pilo::tlv* sub_tlv = tlvp->get<::pilo::tlv*>(i, &err);
                            err = _write_json_object(tmp_val, sub_tlv, allocator);
                            if (err != PILO_OK)
                                return err;
                        }
                        else {
                            err = PERR_MIS_DATA_TYPE;
                        }

                        if (err != PILO_OK) {
                            return err;
                        }

                        obj.PushBack(tmp_val, allocator);                        
                    }
                }
                else if (tlvp->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
                {
                    obj.SetObject();
                    if (tlvp->daynamic_data() != nullptr) {
                        if (tlvp->key_type() == ::pilo::core::rtti::wired_type::key_type_i8) {
                            err = this->_set_dict_of_key_i8(obj, tlvp, allocator);
                        } else if (tlvp->key_type() == ::pilo::core::rtti::wired_type::key_type_u8) {
                            err = this->_set_dict_of_key_u8(obj, tlvp, allocator);
                        } else if (tlvp->key_type() == ::pilo::core::rtti::wired_type::key_type_i16) {
                            err = this->_set_dict_of_key_i16(obj, tlvp, allocator);
                        } else if (tlvp->key_type() == ::pilo::core::rtti::wired_type::key_type_u16) {
                            err = this->_set_dict_of_key_u16(obj, tlvp, allocator);
                        } else if (tlvp->key_type() == ::pilo::core::rtti::wired_type::key_type_i32) {
                            err = this->_set_dict_of_key_i32(obj, tlvp, allocator);
                        } else if (tlvp->key_type() == ::pilo::core::rtti::wired_type::key_type_u32) {
                            err = this->_set_dict_of_key_u32(obj, tlvp, allocator);
                        } else if (tlvp->key_type() == ::pilo::core::rtti::wired_type::key_type_i64) {
                            err = this->_set_dict_of_key_i64(obj, tlvp, allocator);
                        } else if (tlvp->key_type() == ::pilo::core::rtti::wired_type::key_type_u64) {
                            err = this->_set_dict_of_key_u64(obj, tlvp, allocator);
                        } else if (tlvp->key_type() == ::pilo::core::rtti::wired_type::key_type_str) {
                            err = this->_set_dict_of_key_str(obj, tlvp, allocator);
                        }

                                           
                    }

                }
                else {
                    err = ::pilo::mk_err(PERR_INV_VAL_TYPE);
                }


                return PILO_OK;
            }

            ::pilo::err_t json_tlv_driver::_parse_json_object(::rapidjson::Value& obj, ::pilo::tlv* parent_tlv)
            {
                ::pilo::err_t err = PILO_OK;
                for (::rapidjson::Value::ConstMemberIterator itr = obj.MemberBegin(); itr != obj.MemberEnd(); ++itr) {
                    const char* tmp_sub_name = itr->name.GetString();
                    ::pilo::tlv* tlv = ::pilo::tlv::allocate();
                    if (itr->value.IsNull()) {
                        tlv->set_single_type(::pilo::core::rtti::wired_type::value_type_na);
                    }
                    else if (itr->value.IsBool()) {
                        tlv->set(itr->value.GetBool());  
                    }  
                    else if (itr->value.IsInt()) {
                        tlv->set((::pilo::i64_t)itr->value.GetInt());
                    }
                    else if (itr->value.IsUint()) {
                        tlv->set((::pilo::u64_t)itr->value.GetUint());
                    }
                    else if (itr->value.IsInt64()) {
                        tlv->set((::pilo::i64_t) itr->value.GetInt64());
                    }
                    else if (itr->value.IsUint64()) {
                        tlv->set((::pilo::u64_t) itr->value.GetUint64());
                    }
                    else if (itr->value.IsDouble()) {
                        tlv->set(itr->value.GetDouble());
                    }
                    else if (itr->value.IsFloat()) {
                        tlv->set((::pilo::f32_t)itr->value.GetFloat());
                    }
                    else if (itr->value.IsString()) {
                        if (itr->value.GetString() == nullptr) {
                            tlv->set_cstr(nullptr, (::pilo::i64_t)-1, false);
                        }
                        else {
                            tlv->set_cstr(itr->value.GetString(), (::pilo::i64_t)itr->value.GetStringLength(), false);
                        }
                    }
                    else if (itr->value.IsObject()) {
                        tlv->set_dict_type(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_tlv);
                        err = _parse_json_object((::rapidjson::Value&) itr->value, tlv);
                        if (err != PILO_OK) {
                            return err;
                        }
                    }
                    else if (itr->value.IsArray()) {
                        tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
                        err = _parse_json_array((::rapidjson::Value&)itr->value, tlv);
                        if (err != PILO_OK) {
                            return err;
                        }                                      
                    }
                    err = parent_tlv->insert(std::string(tmp_sub_name, itr->name.GetStringLength()), tlv, false);
                    if (err != PILO_OK) {
                        return err;
                    }
                }

                return PILO_OK;
            }

            ::pilo::tlv* json_tlv_driver::root()
            {
                return _m_root_value;
            }

            ::pilo::tlv* json_tlv_driver::get_value_node(const char* fqn, ::pilo::err_t& err)
            {                
                if (_m_root_value == nullptr) {
                    err = ::pilo::mk_perr(PERR_NULL_PTR);
                    return nullptr;
                }

                ::pilo::tlv* tlv_p = _m_root_value->get_tlv<32>(fqn, err);
                return tlv_p;
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, bool is_force)
            {
                ::pilo::err_t err = PILO_OK;
                if (_m_root_value == nullptr) {
                    _m_root_value = ::pilo::tlv::allocate();
                }
                _m_root_value->set(fqn, err, is_force);
                return err;
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, bool iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::i8_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::i16_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::i32_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::i64_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::u8_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::u16_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::u32_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::u64_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::f32_t fv, bool is_force)
            {
                return _set_trivil_type(fqn, fv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, ::pilo::f64_t fv, bool is_force)
            {
                return _set_trivil_type(fqn, fv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, const std::string& sv, bool is_force)
            {
                return _set_trivil_type(fqn, sv, is_force);
            }

            ::pilo::err_t json_tlv_driver::set_value(const char* fqn, const char* value, ::pilo::i32_t len, bool adopt, bool is_cstr, bool is_force)
            {
                ::pilo::err_t err = PILO_OK;
                if (_m_root_value == nullptr) {
                    _m_root_value = ::pilo::tlv::allocate();
                }

                if (value == nullptr) {
                    return set_value(fqn, is_force);
                }

                if (fqn == nullptr || *fqn == 0)
                {
                    if (is_cstr) {
                        err = _m_root_value->set_cstr(value, len, adopt);
                    }
                    else {
                        err = _m_root_value->set_bytes(value, len, adopt);
                    }

                    
                    return ::pilo::mk_perr(PERR_NON_EXIST);
                }

                if (!is_force)
                {
                    ::pilo::tlv* tmp = _m_root_value->get_tlv<32>(fqn, err);
                    if (tmp != nullptr)
                    {
                        return ::pilo::mk_perr(PERR_EXIST);
                    }
                    err = PILO_OK;
                }

                ::pilo::tlv* t = _m_root_value->set_tlv<32>(fqn, err);
                if (t != nullptr)
                {
                    if (is_cstr) {
                        err = t->set_cstr(value, len, adopt);
                    }
                    else {
                        err = t->set_bytes(value, len, adopt);
                    }
                }


                return err;
            }

        }
    }
}