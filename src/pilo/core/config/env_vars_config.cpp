#include "./env_vars_config.hpp"
#include "../process/context.hpp"

namespace pilo {
    namespace core {
        namespace config {
            ::pilo::err_t env_vars_config::load()
            {
                return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
            }
            ::pilo::err_t env_vars_config::save()
            {
                return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
            }
            ::pilo::err_t env_vars_config::set_default()
            {
                _vars_to_set.clear();
                _vars_to_append.clear();
                _vars_to_remove.clear();
                _vars_to_unset.clear();   

                char buffer[4][16] = { 0 };
                ::pilo::core::string::number_to_string(buffer[0], sizeof(buffer[0]), PILO_CONTEXT->major_version(), nullptr);
                ::pilo::core::string::number_to_string(buffer[1], sizeof(buffer[0]), PILO_CONTEXT->minor_version(), nullptr);
                ::pilo::core::string::number_to_string(buffer[2], sizeof(buffer[0]), PILO_CONTEXT->revision(), nullptr);                
                const ::pilo::core::io::path& tmp_path_tlv = PILO_CONTEXT->tmp_path(false);
                set_vars_to_set("PILO_VERSION", { std::string(buffer[0]),std::string(buffer[1]),std::string(buffer[2]), std::string(PILO_CONTEXT->stage_cstr())});
                
                set_vars_to_append("PATH", { PILO_CONTEXT->bin_path(false).fullpath(), PILO_CONTEXT->tmp_path(false).fullpath() });

                
                set_vars_to_remove("PATH", { tmp_path_tlv.fullpath() });
                set_vars_to_unset({"PILO"});

                //_vars_sync.clear();

                _vars_sync.insert("PATH");
                _vars_sync.insert("NAME");

                return PILO_OK;
            }
            void env_vars_config::clear(bool )
            {
                set_default();
            }
            ::pilo::err_t env_vars_config::save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::core::memory::object_array<char, 128>   cb;

                ::pilo::i32_t fqdn_len = (::pilo::i32_t) ::pilo::core::string::character_count(fqdn_path);
                cb.check_space(fqdn_len + 64);
                ::pilo::core::string::n_copyz(cb.begin(), cb.capacity(), fqdn_path, fqdn_len);
                cb.set_size(fqdn_len);


                std::map<std::string, std::vector<std::string>>::const_iterator cit = _vars_to_set.cbegin();
                for (; cit != _vars_to_set.cend(); cit++) {
                    cb.check_more_space(16 + (::pilo::i32_t)cit->first.size());
                    for (int i = 0; i < (int) cit->second.size(); i++) {
                        ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".to_set.%s.[]", cit->first.c_str());
                        PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), cit->second.at(i), true));
                    }
                    
                }
                cb.set_size(fqdn_len);
                cit = _vars_to_append.cbegin();
                for (; cit != _vars_to_append.cend(); cit++) {
                    cb.check_more_space(16 + (::pilo::i32_t)cit->first.size());
                    for (int i = 0; i < (int)cit->second.size(); i++) {
                        ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".to_append.%s.[]", cit->first.c_str());
                        PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), cit->second.at(i), true));
                    }

                }
                cit = _vars_to_remove.cbegin();
                for (; cit != _vars_to_remove.cend(); cit++) {
                    cb.check_more_space(16 + (::pilo::i32_t)cit->first.size());
                    for (int i = 0; i < (int)cit->second.size(); i++) {
                        ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".to_remove.%s.[]", cit->first.c_str());
                        PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), cit->second.at(i), true));
                    }
                }

                for (int i = 0; i < _vars_to_unset.size(); i++) {
                    ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".to_unset.[]");
                    PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), _vars_to_unset.at(i), true));
                }

                for (auto v : _vars_sync) {
                    ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".sync_vars.[]");
                    PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), v, true));
                }

                return PILO_OK;
            }

            ::pilo::err_t env_vars_config::load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver)
            {
                ::pilo::err_t err = PILO_OK;
                
                ::pilo::tlv* ev_tlv = configuator_driver->get_value_node(fqdn_path, err);
                if (ev_tlv == nullptr) {
                    return PILO_OK;
                }
                ::pilo::tlv* val_tlv = nullptr;
                std::map<std::string, ::pilo::tlv*>::const_iterator cit;
                ::pilo::tlv* to_tlv = ev_tlv->get_tlv("to_set", err);
                if (to_tlv != nullptr) {
                    val_tlv = nullptr;
                    cit = ((std::map<std::string, ::pilo::tlv*> *) to_tlv->daynamic_data())->cbegin();
                    for (; cit != ((std::map<std::string, ::pilo::tlv*> *) to_tlv->daynamic_data())->cend(); cit++) {
                        for (int i = 0; i < cit->second->size(); i++) {
                            val_tlv = cit->second->get<::pilo::tlv*>(i, &err);
                            this->add_var_to_set(cit->first, val_tlv->daynamic_data(), val_tlv->size() - 1);
                        }
                    }
                }                

                to_tlv = ev_tlv->get_tlv("to_append", err);
                if (to_tlv != nullptr) {
                    val_tlv = nullptr;
                    cit = ((std::map<std::string, ::pilo::tlv*> *) to_tlv->daynamic_data())->cbegin();
                    for (; cit != ((std::map<std::string, ::pilo::tlv*> *) to_tlv->daynamic_data())->cend(); cit++) {
                        for (int i = 0; i < cit->second->size(); i++) {
                            val_tlv = cit->second->get<::pilo::tlv*>(i, &err);
                            this->add_var_to_append(cit->first, val_tlv->daynamic_data(), val_tlv->size() - 1);
                        }
                    }
                }
                
                to_tlv = ev_tlv->get_tlv("to_remove", err);
                if (to_tlv != nullptr) {
                    val_tlv = nullptr;
                    cit = ((std::map<std::string, ::pilo::tlv*> *) to_tlv->daynamic_data())->cbegin();
                    for (; cit != ((std::map<std::string, ::pilo::tlv*> *) to_tlv->daynamic_data())->cend(); cit++) {
                        for (int i = 0; i < cit->second->size(); i++) {
                            val_tlv = cit->second->get<::pilo::tlv*>(i, &err);
                            this->add_var_to_remove(cit->first, val_tlv->daynamic_data(), val_tlv->size() - 1);
                        }
                    }
                }

                to_tlv = ev_tlv->get_tlv("to_unset", err);
                if (to_tlv != nullptr) {
                    for (int i = 0; i < to_tlv->size(); i++) {
                        val_tlv = to_tlv->get<::pilo::tlv*>(i, &err);
                        this->add_var_to_unset(val_tlv->daynamic_data(), val_tlv->size() - 1);
                    }                
                }

                to_tlv = ev_tlv->get_tlv("sync_vars", err);
                if (to_tlv != nullptr) {
                    for (int i = 0; i < to_tlv->size(); i++) {
                        val_tlv = to_tlv->get<::pilo::tlv*>(i, &err);
                        this->add_var_to_sync(val_tlv->daynamic_data(), val_tlv->size() - 1);
                    }
                }

                return ::pilo::err_t();
            }

            const::pilo::core::io::path* env_vars_config::file_path() const
            {
                return nullptr;
            }

            bool env_vars_config::invalid() const
            {
                return false;
            }
            void env_vars_config::add_vars_to_set(const std::string& orig_key, std::initializer_list<std::string> var_list)
            {
                std::string ckey;
                const std::string& key_ref = key_str_to_str(ckey, orig_key);
                for (std::string s : var_list)
                {
                    _vars_to_set[key_ref].push_back(s);
                }
            }

            void env_vars_config::add_var_to_set(const std::string& orig_key, const char* s, ::pilo::i32_t len)
            {
                std::string ckey;
                const std::string& key_ref = key_str_to_str(ckey, orig_key);
                if (s == nullptr || len < 1) {
                    _vars_to_set[key_ref].push_back(std::string(""));
                }
                else {
                    _vars_to_set[key_ref].push_back(std::string(s, len));
                }                
            }

            void env_vars_config::add_vars_to_append(const std::string& orig_key, std::initializer_list<std::string> var_list)
            {
                std::string ckey;
                const std::string& key_ref = key_str_to_str(ckey, orig_key);
                for (std::string s : var_list)
                {
                    _vars_to_append[key_ref].push_back(s);
                }
            }

            void env_vars_config::add_var_to_append(const std::string& orig_key, const char* s, ::pilo::i32_t len)
            {
                std::string ckey;
                const std::string& key_ref = key_str_to_str(ckey, orig_key);
                if (s == nullptr || len < 1) {
                    _vars_to_append[key_ref].push_back(std::string(""));
                }
                else {
                    _vars_to_append[key_ref].push_back(std::string(s, len));
                }
            }

            void env_vars_config::add_vars_to_remove(const std::string& orig_key, std::initializer_list<std::string> var_list)
            {
                std::string ckey;
                const std::string& key_ref = key_str_to_str(ckey, orig_key);
                for (std::string s : var_list)
                {
                    _vars_to_remove[key_ref].push_back(s);
                }
            }

            void env_vars_config::add_var_to_remove(const std::string& orig_key, const char* s, ::pilo::i32_t len)
            {
                std::string ckey;
                const std::string& key_ref = key_str_to_str(ckey, orig_key);
                if (s == nullptr || len < 1) {
                    _vars_to_remove[key_ref].push_back(std::string(""));
                }
                else {
                    _vars_to_remove[key_ref].push_back(std::string(s, len));
                }
            }

            void env_vars_config::set_vars_to_set(const std::string& orig_key, std::initializer_list<std::string> var_list)
            {
                std::string ckey;
                const std::string& key_ref = key_str_to_str(ckey, orig_key);
                if (_vars_to_set.count(key_ref) > 0)
                    _vars_to_set[key_ref].clear();
                for (std::string s : var_list)
                {
                    _vars_to_set[key_ref].push_back(s);
                }
            }

            void env_vars_config::set_vars_to_append(const std::string& orig_key, std::initializer_list<std::string> var_list)
            {
                std::string ckey;
                const std::string& key_ref = key_str_to_str(ckey, orig_key);
                if (_vars_to_append.count(key_ref) > 0)
                    _vars_to_append[key_ref].clear();
                for (std::string s : var_list)
                {
                    _vars_to_append[key_ref].push_back(s);
                }
            }

            void env_vars_config::set_vars_to_remove(const std::string& orig_key, std::initializer_list<std::string> var_list)
            {
                std::string ckey;
                const std::string& key_ref = key_str_to_str(ckey, orig_key);
                if (_vars_to_remove.count(key_ref) > 0)
                    _vars_to_remove[key_ref].clear();
                for (std::string s : var_list)
                {
                    _vars_to_remove[key_ref].push_back(s);
                }
            }

            void env_vars_config::add_vars_to_unset(std::initializer_list<std::string> name_list)
            {
                std::string ckey;                
                for (std::string s : name_list)
                {
                    const std::string& key_ref = key_str_to_str(ckey, s);
                    _vars_to_unset.push_back(key_ref);
                }
            }

            void env_vars_config::add_var_to_unset(const char* s, ::pilo::i32_t len)
            {
                if (s == nullptr || len < 1) {

                }
                else {
                    std::string ckey;
                    key_cstr_to_str(ckey, s, len);
                    _vars_to_unset.push_back(ckey);
                }
            }

            void env_vars_config::set_vars_to_unset(std::initializer_list<std::string> name_list)
            {
                _vars_to_unset.clear();
                for (std::string s : name_list)
                {
                    std::string ckey;
                    const std::string& key_ref = key_str_to_str(ckey, s);
                    _vars_to_unset.push_back(key_ref);
                }
            }

            void env_vars_config::add_var_to_sync(const char* s, ::pilo::i32_t len)
            {
                if (s == nullptr || len < 1) {

                }
                else {
                    std::string ckey;
                    key_cstr_to_str(ckey, s, len);
                    _vars_sync.insert(ckey);
                }
            }

            ::pilo::err_t env_vars_config::key_cstr_to_str(std::string& str, const char* cstr, ::pilo::i32_t len) const
            {
                if (cstr == nullptr)
                    return ::pilo::mk_perr(PERR_NULL_PARAM);
                if (len < 0)
                    len = (::pilo::i32_t) ::pilo::core::string::character_count(cstr);
#ifdef WINDOWS
                for (int i = 0; i < len; i++) {
                    str.push_back((char)std::toupper(cstr[i]));
                }
#else
                str.append(cstr, len);
#endif

                return PILO_OK;
            }

            const std::string& env_vars_config::key_str_to_str(std::string& dst, const std::string& str) const
            {
#ifdef WINDOWS
                for (int i = 0; i < str.size(); i++) {
                    dst.push_back((char)std::toupper(str[i]));
                }
                return dst;
#else
                return str;
#endif      
            }




        }
    }
}