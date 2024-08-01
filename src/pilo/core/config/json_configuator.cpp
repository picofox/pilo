﻿#include "json_configuator.hpp"
#include "../../external/rapidjson/document.h"
#include "../../external/rapidjson/writer.h"
#include "../../external/rapidjson/stringbuffer.h"



namespace pilo {
    namespace core {
        namespace config {

            json_configuator::json_configuator(const::pilo::core::io::path* path_ptr)
            {
                if (path_ptr != nullptr) {
                    _m_file_ptr = new ::pilo::core::io::file<>();
                    _m_file_ptr->set_path(path_ptr);
                }
                else {
                    _m_file_ptr = nullptr;
                }

                _m_root_value = nullptr;
            }

            json_configuator::~json_configuator()
            {
                if (_m_root_value != nullptr) {
                    ::pilo::tlv::deallocate(_m_root_value);
                    _m_root_value = nullptr;
                }
            }

            ::pilo::err_t core::config::json_configuator::load()
            {
                ::pilo::err_t err = this->_m_file_ptr->open(::pilo::core::io::creation_mode::open_existing
                    , ::pilo::core::io::access_permission::read
                    , ::pilo::core::io::dev_open_flags::none);
                if (err != PILO_OK) {
                    return err;
                }

                char buffer[1024] = { 0 };
                ::pilo::i64_t data_len = 0;
                char* data = this->_m_file_ptr->read_all(buffer, sizeof(buffer), &data_len, &err);
                if (err != PILO_OK) {
                    this->_m_file_ptr->close();
                    return err;
                }
                this->_m_file_ptr->close();

                if (data == nullptr) {
                    return ::pilo::mk_perr(PERR_VAL_EMPTY);
                }                

                err = load(data, data_len);
                if (data != nullptr && data != buffer) {
                    ::pilo::core::io::file<>::s_free_all_buffer(&data);
                }

                return err;
            }

            ::pilo::err_t core::config::json_configuator::save()
            {
                if (this->_m_root_value == nullptr)
                    return ::pilo::mk_perr(PERR_NOOP);

                if (_m_file_ptr == nullptr) {
                    return ::pilo::mk_perr(PERR_NULL_PTR);
                }

                ::pilo::core::io::path pth;
                ::pilo::err_t err = PILO_OK;

                err = _m_file_ptr->path()->make_temp(pth,".tmp", ::pilo::core::io::path::TFNRandPolicyNone);
                if (err != PILO_OK)
                    return err;               

                ::rapidjson::Document root_jdoc; 
                err = _write_json_object(root_jdoc, _m_root_value, root_jdoc.GetAllocator());
                if (err != PILO_OK)
                    return err;

                ::pilo::core::io::file<> tmp_file;
                err = tmp_file.open(&pth, ::pilo::core::io::creation_mode::create_always
                    , ::pilo::core::io::access_permission::write
                    , ::pilo::core::io::dev_open_flags::append);
                rapidjson::StringBuffer jbuffer;
                rapidjson::Writer<rapidjson::StringBuffer> writer(jbuffer);
                root_jdoc.Accept(writer);
                ::pilo::i64_t wlen = (::pilo::i64_t)jbuffer.GetSize();
                ::pilo::i64_t rlen = 0;
                err = tmp_file.write(jbuffer.GetString(), wlen, &rlen);
                if (err != PILO_OK || wlen < rlen) {
                    tmp_file.close();
                    return ::pilo::mk_err(PERR_IO_WRITE_FAIL);
                }

                tmp_file.close();

                return err;
            }


            ::pilo::err_t json_configuator::load(const char* data, ::pilo::i64_t len)
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
                    _m_root_value->set(jdoc_root.GetInt());
                }
                else if (jdoc_root.IsInt64()) {
                    _m_root_value->set(jdoc_root.GetInt64());
                }
                else if (jdoc_root.IsUint()) {
                    _m_root_value->set(jdoc_root.GetUint());
                }
                else if (jdoc_root.IsUint64()) {
                    _m_root_value->set(jdoc_root.GetUint64());
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

            ::pilo::err_t json_configuator::_parse_json_array(::rapidjson::Value& obj, ::pilo::tlv* parent_tlv)
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
                        tlv->set(obj[i].GetInt64());
                    }
                    else if (obj[i].IsUint64()) {
                        tlv->set(obj[i].GetUint64());
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
                            tlv->set_cstr(obj[i].GetString(), (::pilo::i64_t)obj[i].GetStringLength(), false);
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

            ::pilo::core::io::path* json_configuator::file_path()
            {
                if (this->_m_file_ptr != nullptr) {
                    return this->_m_file_ptr->path();
                }
                return nullptr;
            }

            ::pilo::err_t json_configuator::_write_json_object(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
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
                        char tmp_buffer[32] = { 0 };
                        ::pilo::i32_t rlen = 0;
                        const char* pret = tlvp->as_bytes(tmp_buffer, sizeof(tmp_buffer), &err, &rlen);
                        if (pret == nullptr) {
                            obj.SetNull();
                        }
                        else {
                            obj.SetString(pret, rlen);
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
                        const ::pilo::tlv* sub_tlv = tlvp->get<::pilo::tlv*>(i, &err);
                        err = _write_json_object(tmp_val, sub_tlv, allocator);
                        if (err != PILO_OK)
                            return err;
                        obj.PushBack(tmp_val, allocator);                        
                    }
                }
                else if (tlvp->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
                {
                    obj.SetObject();
                    if (tlvp->daynamic_data() != nullptr) {
                        if (tlvp->key_type() != ::pilo::core::rtti::wired_type::key_type_str
                            || tlvp->value_type() != ::pilo::core::rtti::wired_type::value_type_tlv) {
                            return ::pilo::mk_perr(PERR_INV_KEY_TYPE);
                        }

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

                }
                else {
                    err = ::pilo::mk_err(PERR_INV_VAL_TYPE);
                }


                return PILO_OK;
            }

            ::pilo::err_t json_configuator::_parse_json_object(::rapidjson::Value& obj, ::pilo::tlv* parent_tlv)
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
                        tlv->set(itr->value.GetInt());
                    }
                    else if (itr->value.IsUint()) {
                        tlv->set(itr->value.GetUint());
                    }
                    else if (itr->value.IsInt64()) {
                        tlv->set(itr->value.GetInt64());
                    }
                    else if (itr->value.IsUint64()) {
                        tlv->set(itr->value.GetUint64());
                    }
                    else if (itr->value.IsDouble()) {
                        tlv->set(itr->value.GetDouble());
                    }
                    else if (itr->value.IsFloat()) {
                        tlv->set(itr->value.GetFloat());
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

            ::pilo::tlv* json_configuator::root()
            {
                return _m_root_value;
            }

            ::pilo::tlv* json_configuator::get_value(const char* fqn, ::pilo::err_t& err)
            {                
                if (_m_root_value == nullptr) {
                    err = ::pilo::mk_perr(PERR_NULL_PTR);
                    return nullptr;
                }

                ::pilo::tlv* tlv_p = _m_root_value->get_tlv<32>(fqn, err);
                return tlv_p;
            }

            ::pilo::err_t json_configuator::set_file(const char* file_path_cstr, ::pilo::pathlen_t len, ::pilo::predefined_pilo_dir predef_dir)
            {
                if (_m_file_ptr != nullptr) {
                    return ::pilo::mk_perr(PERR_EXIST) ;
                }

                if (file_path_cstr == nullptr) {
                    return ::pilo::mk_perr(PERR_NULL_PTR);
                }

                _m_file_ptr = new ::pilo::core::io::file<>();

                ::pilo::i8_t abs_type = ::pilo::core::io::path::absolute_type(file_path_cstr, false, len);
                if (abs_type == ::pilo::core::io::path::absolute) {
                    return _m_file_ptr->path()->set(file_path_cstr, len, 0, ::pilo::predefined_pilo_dir::count);
                }
                else {
                    return _m_file_ptr->path()->set(file_path_cstr, len, 0, predef_dir);
                }
                
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, bool is_force)
            {
                ::pilo::err_t err = PILO_OK;
                if (_m_root_value == nullptr) {
                    _m_root_value = ::pilo::tlv::allocate();
                }
                _m_root_value->set(fqn, err, is_force);
                return err;
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, bool iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::i8_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::i16_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::i32_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::i64_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::u8_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::u16_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::u32_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::u64_t iv, bool is_force)
            {
                return _set_trivil_type(fqn, iv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::f32_t fv, bool is_force)
            {
                return _set_trivil_type(fqn, fv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, ::pilo::f64_t fv, bool is_force)
            {
                return _set_trivil_type(fqn, fv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, const std::string& sv, bool is_force)
            {
                return _set_trivil_type(fqn, sv, is_force);
            }

            ::pilo::err_t json_configuator::set_value(const char* fqn, const char* value, ::pilo::i32_t len, bool adopt, bool is_force)
            {
                ::pilo::err_t err = PILO_OK;
                if (_m_root_value == nullptr) {
                    _m_root_value = ::pilo::tlv::allocate();
                }

                if (fqn == nullptr || *fqn == 0)
                {
                    err = _m_root_value->set_cstr(value, len, adopt);
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
                    t->set_cstr(value, len, adopt);
                }


                return err;
            }

        }
    }
}