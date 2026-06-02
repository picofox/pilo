////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                                                                                    //
//  .----------------.  .----------------.  .----------------.  .----------------.       Raid boss    //
//  | .--------------. || .--------------. || .--------------. || .--------------. |    Lv.85 缺德猫   //
//  | |   ______     | || |     _____    | || |   _____      | || |     ____     | |     |\.-"-./|    //
//  | |  |_   __ \   | || |    |_   _|   | || |  |_   _|     | || |   .'    `.   | |     \`     `/    //
//  | |    | |__) |  | || |      | |     | || |    | |       | || |  /  .--.  \  | |     |= ^Y^ =|    //
//  | |    |  ___/   | || |      | |     | || |    | |   _   | || |  | |    | |  | |     \__ ^ __/    //
//  | |   _| |_      | || |     _| |_    | || |   _| |__/ |  | || |  \  `- - '/  | |     /`=+o+=`\    //
//  | |  |_____|     | || |    |_____|   | || |  |________|  | || |   `.____.'   | |    |         |   //
//  | |              | || |              | || |              | || |              | |    | (     ) |   //
//  | '--------------' || '--------------' || '--------------' || '--------------' |    (,,)---(,,)   //
//  '----------------'  '----------------'  '----------------'  '----------------'                    //
//                                                                                                    //
////////////////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _pilo_core_config_json_tlv_driver_h_
#define _pilo_core_config_json_tlv_driver_h_

#include "./tlv_driver_interface.hpp"

#include "../../external/rapidjson/document.h"
#include "../../tlv.hpp"

namespace pilo {

    class tlv;

    namespace core {
        namespace ml {
        
        
            class json_tlv_driver : public tlv_driver_interface
            {
            public:
                json_tlv_driver() : _m_root_value(nullptr), _owner(true) {}
                json_tlv_driver(::pilo::tlv* root_ref) : _m_root_value(root_ref), _owner(false) {}
                virtual ~json_tlv_driver();


            public:
                // Inherited via tlv_driver_interface
                ::pilo::err_t load(const::pilo::core::io::path* path_ptr) override;
                ::pilo::err_t load(const char* data, ::pilo::i64_t len) override;
                ::pilo::err_t save(const ::pilo::core::io::path* dest_path) override;
                ::pilo::tlv* root() override;
                ::pilo::tlv* get_value_node(const char* fqn, ::pilo::err_t& err) override;

                ::pilo::err_t set_value(const char* fqn, bool is_force);
                ::pilo::err_t set_value(const char* fqn, bool iv, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::i8_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::i16_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::i32_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::i64_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::u8_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::u16_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::u32_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::u64_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::f32_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::f64_t v, bool is_force);            
                ::pilo::err_t set_value(const char* fqn, const std::string & v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, const char* value, ::pilo::i32_t len, bool adopt, bool is_cstr, bool is_force);

            public:
                




            protected:
                template<typename T>
                ::pilo::err_t _set_trivil_type(const char* fqn, T&& value, bool is_force)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (_m_root_value == nullptr) {
                        _m_root_value = ::pilo::tlv::allocate();
                    }

                    if (fqn == nullptr || *fqn == 0)
                    {
                        return _m_root_value->set(value);
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
                        err = t->set(value);
                    }

                    return err;
                }




                ::pilo::err_t _write_json_object(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType & allocator) ;
                ::pilo::err_t _parse_json_object(::rapidjson::Value & obj, ::pilo::tlv* parent_tlv);
                ::pilo::err_t _parse_json_array(::rapidjson::Value& obj, ::pilo::tlv* parent_tlv);

                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::i8_t  iv) const { value_obj.SetInt((int) iv); }
                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::u8_t  iv) const { value_obj.SetUint((unsigned int) iv); }
                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::i16_t iv) const { value_obj.SetInt((int) iv); }
                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::u16_t iv) const { value_obj.SetUint((unsigned int) iv); }
                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::i32_t iv) const { value_obj.SetInt(iv); }
                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::u32_t iv) const { value_obj.SetUint(iv); }
                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::i64_t iv) const { value_obj.SetInt64(iv); }
                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::u64_t iv) const { value_obj.SetUint64(iv); }
                void _set_obj_values(::rapidjson::Value& value_obj, bool          bv) const { value_obj.SetBool(bv); }
                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::f32_t fv) const { value_obj.SetFloat(fv); }
                void _set_obj_values(::rapidjson::Value& value_obj, ::pilo::f64_t fv) const { value_obj.SetDouble(fv); }
                void _set_obj_values(::rapidjson::Value& value_obj, char* cstrp, ::pilo::i32_t len) const { value_obj.SetString(cstrp, (rapidjson::SizeType) len); }
                void _set_obj_values(::rapidjson::Value& value_obj, const std::string & sv) const { value_obj.SetString(sv.c_str(), (rapidjson::SizeType)sv.size()); }

                template<typename VT>
                ::pilo::err_t _set_str_key_dict_object(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
                {
                    std::map<std::string, VT>* map_ptr = (std::map<std::string, VT>*) tlvp->daynamic_data();
                    if (map_ptr == nullptr) {
                        return PILO_OK;
                    }
                    std::map<std::string, VT>::const_iterator cit = map_ptr->cbegin();
                    for (; cit != map_ptr->cend(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(cit->first.c_str(), allocator);
                        _set_obj_values(tmp_val, cit->second);
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                    return PILO_OK;
                }

                template<typename KT, typename VT>
                ::pilo::err_t _set_dict_object(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
                {
                    std::map<KT, VT>* map_ptr = (std::map<KT, VT>*) tlvp->daynamic_data();
                    if (map_ptr == nullptr) {
                        return PILO_OK;
                    }
                    std::map<KT, VT>::const_iterator cit = map_ptr->cbegin();
                    for (; cit != map_ptr->cend(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        _set_obj_values(tmp_val, (VT) cit->second);
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                    return PILO_OK;
                }

                template<typename KT>
                ::pilo::err_t _set_dict_object_str(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
                {
                    std::map<KT, std::string>* map_ptr = (std::map<KT, std::string>*) tlvp->daynamic_data();
                    if (map_ptr == nullptr) {
                        return PILO_OK;
                    }
                    std::map<KT, std::string>::const_iterator cit = map_ptr->cbegin();
                    for (; cit != map_ptr->cend(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        _set_obj_values(tmp_val, cit->second);
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                    return PILO_OK;
                }

                ::pilo::err_t _set_str_key_dict_object_str(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
                {
                    std::map<std::string, std::string>* map_ptr = (std::map<std::string, std::string>*) tlvp->daynamic_data();
                    if (map_ptr == nullptr) {
                        return PILO_OK;
                    }
                    std::map<std::string, std::string>::const_iterator cit = map_ptr->cbegin();
                    for (; cit != map_ptr->cend(); cit++) {
                        ::rapidjson::Value tmp_val;
                        ::rapidjson::Value key_value(cit->first.c_str(), allocator);
                        _set_obj_values(tmp_val, cit->second);
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                    return PILO_OK;
                }

                ::pilo::err_t _set_str_key_dict_object_bytes(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
                {
                    std::map<std::string, char*>* map_ptr = (std::map<std::string, char*>*) tlvp->daynamic_data();
                    if (map_ptr == nullptr) {
                        return PILO_OK;
                    }
                    std::map<std::string, char*>::const_iterator cit = map_ptr->cbegin();
                    for (; cit != map_ptr->cend(); cit++) {
                        ::rapidjson::Value tmp_val;   
                        ::rapidjson::Value key_value(cit->first.c_str(), allocator);
                        if (cit->second == nullptr) {
                            tmp_val.SetNull();
                        } else {
                            _set_obj_values(tmp_val, cit->second, (rapidjson::SizeType) ::pilo::core::string::character_count(cit->second));
                        }
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                    return PILO_OK;
                
                }
                

                template<typename KT>
                ::pilo::err_t _set_dict_object_bytes(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator)
                {
                    std::map<KT, char*>* map_ptr = (std::map<KT, char*>*) tlvp->daynamic_data();
                    if (map_ptr == nullptr) {
                        return PILO_OK;
                    }
                    std::map<KT, char*>::const_iterator cit = map_ptr->cbegin();
                    for (; cit != map_ptr->cend(); cit++) {
                        ::rapidjson::Value tmp_val;   
                        ::rapidjson::Value key_value(std::to_string(cit->first).c_str(), allocator);
                        if (cit->second == nullptr) {
                            tmp_val.SetNull();
                        } else {
                            _set_obj_values(tmp_val, cit->second, (rapidjson::SizeType) ::pilo::core::string::character_count(cit->second));
                        }
                        obj.AddMember(key_value, tmp_val, allocator);
                    }
                    return PILO_OK;
                
                }

                ::pilo::err_t _set_dict_of_key_i8(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator);
                ::pilo::err_t _set_dict_of_key_u8(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator);
                ::pilo::err_t _set_dict_of_key_i16(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator);
                ::pilo::err_t _set_dict_of_key_u16(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator);
                ::pilo::err_t _set_dict_of_key_i32(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator);
                ::pilo::err_t _set_dict_of_key_u32(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator);
                ::pilo::err_t _set_dict_of_key_i64(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator);
                ::pilo::err_t _set_dict_of_key_u64(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator);
                ::pilo::err_t _set_dict_of_key_str(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType& allocator);

            protected:

                ::pilo::tlv* _m_root_value;
                bool _owner;

            private:
            PMC_DISABLE_COPY(json_tlv_driver)

   

                // Inherited via tlv_driver_interface
                ::pilo::tlv* make_value_node(const char* fqn, ::pilo::err_t& err) override;

            };
        
        }
    }
}


#endif // !_pilo_core_config_json_reader_h_
