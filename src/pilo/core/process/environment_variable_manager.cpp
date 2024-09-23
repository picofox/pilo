#include "./environment_variable_manager.hpp"
#include "context.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            static int s_i = 0;
            static ::pilo::err_t _s_read_envs(const char* key, ::pilo::i32_t key_len, const char* val, ::pilo::i32_t val_len, void* ctx)
            {
                environment_variable_manager* evm_ptr = (environment_variable_manager*) ctx;
                evm_ptr->insert(key, key_len, val, val_len);
                return PILO_OK;
            }

            static ::pilo::err_t _s_split_value(const char* , const char* ptr, ::pilo::i64_t len, void* ctx)
            {
                ::pilo::tlv* tlvp = (::pilo::tlv*)ctx;
                tlvp->push_back<std::string>(std::string(ptr, len));
                return PILO_OK;
            }

            environment_variable_manager::~environment_variable_manager()
            {
                std::map<std::string, ::pilo::tlv*>::const_iterator cit = _vars.cbegin();
                for (; cit != _vars.cend(); cit++) {
                    if (cit->second != nullptr)
                        ::pilo::tlv::deallocate(cit->second);
                }
                _vars.clear();
            }

            ::pilo::err_t environment_variable_manager::initialize()
            {
                ::pilo::err_t err = ::pilo::core::process::xpf_iterate_enviroment_variable(_s_read_envs, this, false);

                const std::map<std::string, std::vector<std::string>>& map_vec = PILO_CONTEXT->core_config()->env_vars().vars_to_set();
                std::map<std::string, std::vector<std::string>>::const_iterator map_vec_iter = map_vec.cbegin();
                for (; map_vec_iter != map_vec.cend(); map_vec_iter ++) {
                    err = this->set(map_vec_iter->first, map_vec_iter->second);
                    if (err != PILO_OK) {
                        PMC_ASSERT(false);
                        return err;
                    }                        
                }

                const std::map<std::string, std::vector<std::string>>& map_vec_append = PILO_CONTEXT->core_config()->env_vars().vars_to_append();
                map_vec_iter = map_vec_append.cbegin();
                for (; map_vec_iter != map_vec_append.cend(); map_vec_iter++) {
                    err = this->append(map_vec_iter->first, map_vec_iter->second);
                    if (err != PILO_OK) {
                        PMC_ASSERT(false);
                        return err;
                    }
                }

                const std::map<std::string, std::vector<std::string>>& map_vec_removed = PILO_CONTEXT->core_config()->env_vars().vars_to_remove();
                map_vec_iter = map_vec_removed.cbegin();
                for (; map_vec_iter != map_vec_removed.cend(); map_vec_iter++) {                    
                    err = this->remove(map_vec_iter->first, map_vec_iter->second);
                    if (err != PILO_OK) {
                        PMC_ASSERT(false);
                        return err;
                    }
                }

                const std::vector<std::string >& vec_unset = PILO_CONTEXT->core_config()->env_vars().vars_to_unset();
                for (int i = 0; i < vec_unset.size(); i++) {
                    err = this->unset(vec_unset[i]);
                    if (err != PILO_OK) {
                        PMC_ASSERT(false);
                        return err;
                    }
                }

                return err;
            }

            ::pilo::err_t environment_variable_manager::insert(const char* key, ::pilo::i32_t key_len, const char* val, ::pilo::i32_t val_len)
            {
                std::string k;
                PILO_CONTEXT->core_config()->env_vars().key_cstr_to_str(k, key, key_len);

                ::pilo::tlv* tlvp = ::pilo::tlv::allocate(
                    ::pilo::core::rtti::wired_type::wrapper_array
                    , ::pilo::core::rtti::wired_type::key_type_na
                    , ::pilo::core::rtti::wired_type::value_type_str);
                if (tlvp == nullptr) {
                    return ::pilo::mk_perr(PERR_CREATE_TLV_FAIL);
                }

                ::pilo::core::string::iteratable_split(val, val_len, PMI_ENV_VAR_SEP_STR, 1, _s_split_value, tlvp,false, true, true, true);


               


                std::map<std::string, ::pilo::tlv*>::iterator it = _vars.find(k);
                if (it == _vars.end()) {
                    _vars.insert(std::pair<std::string, ::pilo::tlv*>(k, tlvp));
                }
                else {
                    if (it->second != nullptr) {
                        ::pilo::tlv::deallocate(it->second);
                    }
                    _vars[k] = tlvp;
                }
                return PILO_OK;
            }

            ::pilo::err_t environment_variable_manager::set(const std::string& orig_key, const std::vector<std::string>& vec)
            {
                std::stringstream ss;
                std::string ckey;
                const std::string& key_ref = PILO_CONTEXT->core_config()->env_vars().key_str_to_str(ckey, orig_key);
                std::map<std::string, ::pilo::tlv*>::iterator it = _vars.find(key_ref);
                if (it == _vars.end()) {
                    ::pilo::tlv* tlvp = ::pilo::tlv::allocate(
                        ::pilo::core::rtti::wired_type::wrapper_array
                        , ::pilo::core::rtti::wired_type::key_type_na
                        , ::pilo::core::rtti::wired_type::value_type_str);
                    if (tlvp == nullptr) {
                        return ::pilo::mk_perr(PERR_CREATE_TLV_FAIL);
                    }

                    for (int i = 0; i < vec.size(); i++) {
                        tlvp->push_back<std::string>(vec[i]);
                        if (PILO_CONTEXT->core_config()->env_vars().need_sync(key_ref)) {
                            if (i > 0)
                                ss << PMI_ENV_VAR_SEP;
                            ss << vec[i];     
                        }
                    }
                    _vars.insert(std::pair<std::string, ::pilo::tlv*>(key_ref, tlvp));
                }
                else {
                    it->second->clear();
                    for (int i = 0; i < vec.size(); i++) {
                        it->second->push_back<std::string>(vec[i]);
                        if (PILO_CONTEXT->core_config()->env_vars().need_sync(key_ref)) {
                            if (i > 0)
                                ss << PMI_ENV_VAR_SEP;
                            ss << vec[i];
                        }
                    }
                }

                if (PILO_CONTEXT->core_config()->env_vars().need_sync(key_ref)) {
                    ::pilo::core::process::xpf_set_environment_variable(key_ref.c_str(), ss.str().c_str());
                }
                
                return PILO_OK;
            }

            ::pilo::err_t environment_variable_manager::append(const std::string& orig_key, const std::vector<std::string>& vec)
            {
                std::stringstream ss;
                std::string ckey;
                ::pilo::err_t err = PILO_OK;                
                const std::string& key_ref = PILO_CONTEXT->core_config()->env_vars().key_str_to_str(ckey, orig_key);
                bool sync = PILO_CONTEXT->core_config()->env_vars().need_sync(key_ref);
                std::map<std::string, ::pilo::tlv*>::iterator it = _vars.find(key_ref);
                if (it == _vars.end()) {
                    ::pilo::tlv* tlvp = ::pilo::tlv::allocate(
                        ::pilo::core::rtti::wired_type::wrapper_array
                        , ::pilo::core::rtti::wired_type::key_type_na
                        , ::pilo::core::rtti::wired_type::value_type_str);
                    if (tlvp == nullptr) {
                        return ::pilo::mk_perr(PERR_CREATE_TLV_FAIL);
                    }

                    for (int i = 0; i < vec.size(); i++) {
                        tlvp->push_back<std::string>(vec[i]);
                        if (sync) {
                            if (i > 0)
                                ss << PMI_ENV_VAR_SEP;
                            ss << vec[i];
                        }
                    }
                    _vars.insert(std::pair<std::string, ::pilo::tlv*>(key_ref, tlvp));
                }
                else {   
                    for (int i = 0; i < vec.size(); i++) {
                        it->second->push_back<std::string>(vec[i]);
                    }
                    if (sync) {
                        for (int i = 0; i < it->second->size(); i++) {
                            if (i > 0)
                                ss << PMI_ENV_VAR_SEP;    
                            std::string str0 = it->second->get<std::string>(i, &err);
                            if (err != PILO_OK) {
                                return err;
                            }
                            ss << str0;
                        }
                    }
                }

                if (sync) {
                    ::pilo::core::process::xpf_set_environment_variable(key_ref.c_str(), ss.str().c_str());
                }

                return PILO_OK;
            }

            ::pilo::err_t environment_variable_manager::remove(const std::string& orig_key, const std::vector<std::string>& vec)
            {
                std::stringstream ss;
                std::string ckey;
                ::pilo::err_t err = PILO_OK;
                const std::string& key_ref = PILO_CONTEXT->core_config()->env_vars().key_str_to_str(ckey, orig_key);
                std::map<std::string, ::pilo::tlv*>::iterator it = _vars.find(key_ref);
                if (it == _vars.end()) {
                    return PILO_OK;
                }

                if (vec.size() < 1) {
                    return PILO_OK;
                }

                for (int i = (int) vec.size() - 1; i >= 0; i--) {
                    for (int j = ((int)it->second->size()) - 1; j >= 0; j--) {
                        std::string valstr = it->second->get<std::string>(j, &err);
                        if (vec.at(i) == valstr) {
                            it->second->erase<std::string>(j); 
                            break;
                        }
                    }
                }

                if (PILO_CONTEXT->core_config()->env_vars().need_sync(key_ref)) {
                    for (int i = 0; i < it->second->size(); i++) {
                        if (i > 0)
                            ss << PMI_ENV_VAR_SEP;
                        std::string str0 = it->second->get<std::string>(i, &err);
                        if (err != PILO_OK) {
                            return err;
                        }
                        ss << str0;
                    }
                    ::pilo::core::process::xpf_set_environment_variable(key_ref.c_str(), ss.str().c_str());
                }

                return err;
            }

            ::pilo::err_t environment_variable_manager::unset(const std::string& orig_key)
            {
                std::stringstream ss;
                std::string ckey;
                const std::string& key_ref = PILO_CONTEXT->core_config()->env_vars().key_str_to_str(ckey, orig_key);
                std::map<std::string, ::pilo::tlv*>::iterator it = _vars.find(key_ref);
                if (it == _vars.end()) {
                    return PILO_OK;
                }
                else {
                    _vars.erase(it);
                }

                if (PILO_CONTEXT->core_config()->env_vars().need_sync(key_ref)) {
                    return ::pilo::core::process::xpf_unset_environment_variable(key_ref.c_str());
                }                

                return PILO_OK;
            }



        }
    }
}
