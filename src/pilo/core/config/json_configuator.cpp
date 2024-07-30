#include "json_configuator.hpp"
#include "../../external/rapidjson/document.h"


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
                    return err;
                }
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
                return ::pilo::err_t();
            }

            void json_configuator::set_file(const::pilo::core::io::path* path_ptr)
            {
                if (_m_file_ptr != nullptr) {
                    return;
                }

                if (path_ptr != nullptr) {
                    _m_file_ptr = new ::pilo::core::io::file<>();
                    _m_file_ptr->set_path(path_ptr);
                }
                else {
                    _m_file_ptr = nullptr;
                }
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

        }
    }
}