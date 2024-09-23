#ifndef _pilo_core_config_env_vars_config_hpp_
#define _pilo_core_config_env_vars_config_hpp_

#include "../logging/logger_def.hpp"
#include "../io/path.hpp"
#include "../ml/json_tlv_driver.hpp"
#include "./configuation_interface.hpp"
#include <map>
#include <vector>
#include <set>

namespace pilo {
    namespace core {
        namespace config {

            class env_vars_config : public configuation_interface
            {
            public:    
                // Inherited via configuation_interface
                ::pilo::err_t load() override;

                ::pilo::err_t save() override;

                ::pilo::err_t set_default() override;

                void clear(bool purge) override;

                ::pilo::err_t save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver) override;

                ::pilo::err_t load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver) override;

                const::pilo::core::io::path* file_path() const override;

                bool invalid() const override;

                void add_vars_to_set(const std::string& key, std::initializer_list<std::string> var_list);
                void add_var_to_set(const std::string& key, const char* s, ::pilo::i32_t len);
                void add_vars_to_append(const std::string& key, std::initializer_list<std::string> var_list);
                void add_var_to_append(const std::string& key, const char* s, ::pilo::i32_t len);
                void add_vars_to_remove(const std::string& key, std::initializer_list<std::string> var_list);
                void add_var_to_remove(const std::string& key, const char* s, ::pilo::i32_t len);
                void set_vars_to_set(const std::string& key, std::initializer_list<std::string> var_list);
                void set_vars_to_append(const std::string& key, std::initializer_list<std::string> var_list);
                void set_vars_to_remove(const std::string& key, std::initializer_list<std::string> var_list);
                void add_vars_to_unset(std::initializer_list<std::string> name_list);
                void add_var_to_unset(const char* s, ::pilo::i32_t len);
                void set_vars_to_unset(std::initializer_list<std::string> name_list);
                void add_var_to_sync(const char* s, ::pilo::i32_t len);

                const std::map<std::string, std::vector<std::string>>& vars_to_set() const { return _vars_to_set;  }
                const std::map<std::string, std::vector<std::string>>& vars_to_append() const { return _vars_to_append; }
                const std::map<std::string, std::vector<std::string>>& vars_to_remove() const { return _vars_to_remove; }
                const std::vector<std::string>& vars_to_unset() const { return _vars_to_unset; }

                bool need_sync_raw(const std::string& orig_key) const
                {
                    std::string ckey;
                    const std::string& key_ref = key_str_to_str(ckey, orig_key);
                    if (_vars_sync.count(key_ref) < 1) {
                        return false;
                    }
                    return true;
                }

                bool need_sync(const std::string& raw_key) const
                {
                    if (_vars_sync.count(raw_key) < 1) {
                        return false;
                    }
                    return true;
                }

                ::pilo::err_t key_cstr_to_str(std::string& str, const char* cstr, ::pilo::i32_t len) const;
                const std::string& key_str_to_str(std::string& dst, const std::string& str) const;


            private:

                std::map<std::string, std::vector<std::string>>  _vars_to_set;
                std::map<std::string, std::vector<std::string>>  _vars_to_append;
                std::map<std::string, std::vector<std::string>>  _vars_to_remove;
                std::vector<std::string>                         _vars_to_unset;
                std::set<std::string>                            _vars_sync;


            };
        }
    }
}


#endif