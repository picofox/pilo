#ifndef _pilo_core_config_core_config_h_
#define _pilo_core_config_core_config_h_

#include <vector>
#include <map>
#include "../ml/json_tlv_driver.hpp"
#include "./configuation_interface.hpp"
#include "../string/string_operation.hpp"
#include "../logging/logger_def.hpp"
#include "../io/path.hpp"
#include "./logger_config.hpp"
#include "./cmdline_args_config.hpp"
#include "./env_vars_config.hpp"
#include "./thread_pool_config.hpp"
#include "./service_config.hpp"

namespace pilo {
    namespace core {
        namespace config {

            class core_config : public configuation_interface 
            {
            public:
                core_config();
                virtual ~core_config()
                {
                    clear();
                }
                
                virtual ::pilo::err_t load();
                virtual ::pilo::err_t save() override;
                virtual void clear(bool purge = true);
                virtual ::pilo::err_t set_default() override;
                virtual ::pilo::err_t save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* driver);
                virtual ::pilo::err_t load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* driver);
                virtual ::pilo::err_t load_or_save_default()
                {
                    ::pilo::err_t err = load();
                    if (err == PILO_OK) 
                        return PILO_OK;

                    ::pilo::err_t perr = ::pilo::ex_perr(err);
                    if (perr != PERR_IO_OPEN_FAIL) {
                        return err;
                    }

                    err = this->set_default();
                    if (err != PILO_OK)
                        return err;

                    return this->save();                    
                }

                virtual const ::pilo::core::io::path* file_path() const;
                virtual bool invalid() const;

            public:
                const logger* logger_at(::pilo::u32_t idx) const
                {
                    if (idx < _loggers.size())
                        return &_loggers[idx];
                    return nullptr;
                }

                const std::string& cwd() const
                {
                    return _cwd;
                }

                const ::std::vector<logger>& loggers() const
                {
                    return _loggers;
                }

                const cmdline_args_config& cmdline_arg_spec() const { return _cmdline_args;  }

                const env_vars_config& env_vars() const { return _env_vars;  }
                const std::map<::pilo::service_group_id, service_config>& core_services() const { return _core_services; }
                const thread_pool_config& thread_pool() const { return _thread_pool;  }
            
            public:
                inline void set_cwd(const char* cwd_cstr) { _cwd = cwd_cstr; }


            private:                
                ::std::string                           _cwd;
                ::std::vector<logger>                   _loggers;      
                cmdline_args_config                     _cmdline_args;
                env_vars_config                         _env_vars;
                std::map<::pilo::i16_t, service_config> _core_services;
                thread_pool_config                      _thread_pool;

            };


        }
    }
}

#endif // !_pilo_core_config_config_core_config_h_
