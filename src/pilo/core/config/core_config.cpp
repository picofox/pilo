#include "core_config.hpp"
#include "../process/context.hpp"
#include "../../tlv.hpp"
#include "../process/process.hpp"
#include "../ml/json_tlv_driver.hpp"

namespace pilo {
    namespace core {
        namespace config {

            core_config::core_config()
                : _cwd("")
            {
                
            }

            ::pilo::err_t core_config::load()
            {
                ::pilo::err_t err = PILO_OK;

                ::pilo::core::ml::json_tlv_driver jct;
                err = jct.load(file_path());
                if (err != PILO_OK)
                    return err;
               
                err = this->load_from_configurator(nullptr, &jct);

                return err;
            }

            ::pilo::err_t core_config::set_default()
            {
                char buffer[128] = { 0 };
                ::pilo::i32_t rlen = 0;
                std::string core_conf_file_name;
                char* cret = ::pilo::core::process::xpf_get_proc_name(buffer, sizeof(buffer), &rlen);
                if (cret != nullptr) {
                    const char* point_ptr = ::pilo::core::string::find_char(cret, rlen, '.');
                    ::pilo::i64_t basename_len = rlen;
                    if (point_ptr != nullptr)
                        basename_len = point_ptr - cret;
                    core_conf_file_name.assign(cret, basename_len);
                    PMF_COMPARE_HEAP_FREE(cret, buffer);
                }
                this->clear(false);
                logger tmp_logger;
                tmp_logger.set_default();
                tmp_logger.set_name(core_conf_file_name.c_str());
                this->_loggers.emplace_back(std::move(tmp_logger));

                this->_cmdline_args.set_default();
                this->_env_vars.set_default();
                this->_thread_pool.set_default();

                return PILO_OK;
            }

            const ::pilo::core::io::path* core_config::file_path() const
            {
                return &(PILO_CONTEXT->core_cfg_path());
            }

            ::pilo::err_t core_config::save()
            {
                ::pilo::core::ml::json_tlv_driver jct;

                ::pilo::err_t err = save_to_configurator(nullptr, &jct);
                if (err != PILO_OK) {
                    return err;
                }

                return jct.save(file_path());
            }

            bool core_config::invalid() const
            {
                if (_loggers.empty())
                    return true;

                for (int i = 0; i < _loggers.size(); i++) {
                    if (_loggers[i].invalid())
                        return true;
                }

                return false;
            }

            void core_config::clear(bool purge)
            {
                this->_cwd.clear();
                this->_loggers.clear();
                this->_cmdline_args.clear(false);
                this->_thread_pool.clear(false);

                if (purge) {
                    this->_loggers.shrink_to_fit();
                }
            }

            ::pilo::err_t core_config::load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* driver)
            {
                PMC_UNUSED(fqdn_path);
                ::pilo::err_t err = PILO_OK;
                logger tmp_logger;
                char fqdn_buffer[64] = { 0 };

                this->clear(false);

                PILO_CHKERR_RET(err, driver->get_value("cwd", this->_cwd));

                ::pilo::tlv * vp = driver->get_value_node("loggers", err);
                if (vp != nullptr) {
                    if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_array) {
                        return ::pilo::mk_err(PERR_MIS_DATA_TYPE);
                    }

                    for (::pilo::i32_t i = 0; i < vp->size(); i++) {
                        ::pilo::core::io::string_formated_output(fqdn_buffer, sizeof(fqdn_buffer), "loggers.[%d]", i);
                        err = tmp_logger.load_from_configurator(fqdn_buffer, driver);
                        PILO_ERRRET(err);
                        this->_loggers.push_back(tmp_logger);
                    }
                }
                
                if (this->_loggers.size() < 1) {
                    ::pilo::core::io::file_formatted_output(stdout, "[Warn] no logger is configured. make a default one (console logger).");
                    tmp_logger.set_default();
                    this->_loggers.push_back(tmp_logger);
                }

                PILO_CHKERR_RET(err, this->_cmdline_args.load_from_configurator("cmdline_args", driver));
                PILO_CHKERR_RET(err, this->_env_vars.load_from_configurator("env_vars", driver));
                PILO_CHKERR_RET(err, this->_thread_pool.load_from_configurator("thread_pool", driver));

                return PILO_OK;
            }


            ::pilo::err_t core_config::save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface * driver)
            {
                PMC_UNUSED(fqdn_path);
                ::pilo::err_t err = PILO_OK;
                PILO_CHKERR_RET(err, driver->set_value("cwd", _cwd.c_str(), (::pilo::i32_t) _cwd.size(), false, true, true));
                char buffer[128] = { 0 };                

                for (int i = 0; i < _loggers.size(); i++) {
                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].", i);
                    _loggers[i].save_to_configurator(buffer, driver);
                }

                this->_cmdline_args.save_to_configurator("cmdline_args", driver);
                this->_env_vars.save_to_configurator("env_vars", driver);
                this->_thread_pool.save_to_configurator("thread_pool", driver);
                return PILO_OK;
            }

            
        
        }
    }
}