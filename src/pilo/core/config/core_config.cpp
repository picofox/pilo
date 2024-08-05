#include "core_config.hpp"
#include "../process/context.hpp"

namespace pilo {
    namespace core {
        namespace config {

            core_config::core_config(configurator_type ct)
                :_configuator(nullptr)
                , _cwd("")
            {
                std::string str;

                if (ct == configurator_type::json) {
                    _configuator = new ::pilo::core::config::json_configuator();
                    char buffer[128] = { 0 };
                    ::pilo::i32_t rlen = 0;
                    char* cret = ::pilo::core::process::xpf_get_proc_name(buffer, sizeof(buffer), &rlen);
                    if (cret != nullptr) {
                        const char* point_ptr = ::pilo::core::string::find_char(cret, rlen, '.');
                        ::pilo::i64_t basename_len = rlen;
                        if (point_ptr != nullptr)
                            basename_len = point_ptr - cret;
                        str.assign(cret, basename_len);
                        PMF_COMPARE_HEAP_FREE(cret, buffer);
                        str += ".json";
                        _configuator->set_file(str.c_str(), (::pilo::pathlen_t) str.size(), ::pilo::predefined_pilo_dir::bin);
                    }                    
                }
            }

            ::pilo::err_t core_config::load()
            {
                ::pilo::err_t err = PILO_OK;

                if (this->_configuator == nullptr) {
                    return ::pilo::mk_err(PERR_IO_INIT_FAIL);
                }
                
                ::pilo::core::io::path* p = this->_configuator->file_path();
                if (p == nullptr || p->invalid()) {
                    char buffer[128] = { 0 };
                    ::pilo::i64_t flen = ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%s.json", PILO_CONTEXT->process_basename());
                    err = this->_configuator->set_file(buffer, (::pilo::pathlen_t) flen, ::pilo::predefined_pilo_dir::bin);
                    if (err != PILO_OK)
                        return err;
                }

                err = this->_configuator->load();
                if (err != PILO_OK)
                    return err;
               
                err = this->load_from_configurator();

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
                ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%s_prof", core_conf_file_name.c_str());


                core_config::logger tmp_logger;
                tmp_logger.set_type(::pilo::core::logging::logger_type::local_spmt_text);
                tmp_logger.set_name(core_conf_file_name.c_str());
                tmp_logger.set_bak_name_suffix(::pilo::core::logging::DefaultBakNameSuffix);
                this->_loggers.emplace_back(tmp_logger);
                tmp_logger.set_defualt();
                tmp_logger.set_type(::pilo::core::logging::logger_type::local_spst_text);
                tmp_logger.set_name(buffer);
                tmp_logger.set_bak_name_suffix(::pilo::core::logging::DefaultBakNameSuffix);
                this->_loggers.emplace_back(tmp_logger);

                return PILO_OK;
            }

            ::pilo::err_t core_config::save()
            {
                if (this->_configuator == nullptr)
                    return ::pilo::mk_perr(PERR_NULL_PARAM);
                if (this->_configuator->file_path() == nullptr
                    || this->_configuator->file_path()->invalid()) {
                    return ::pilo::mk_perr(PERR_INVALID_PATH);
                }

                ::pilo::err_t err = save_to_configurator();
                if (err != PILO_OK) {
                    return err;
                }

                return this->_configuator->save();
            }

            void core_config::clear(bool purge)
            {
                this->_loggers.clear();
                if (purge) {
                    this->_loggers.shrink_to_fit();
                }
            }

            ::pilo::err_t core_config::load_from_configurator()
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = nullptr;
                ::pilo::tlv* svp = nullptr;
                ::pilo::tlv* svp2 = nullptr;
                logger tmp_logger;

                vp = this->_configuator->get_value("cwd", err);
                PILO_ERRRET(err);

                err = vp->assign_as_str(this->_cwd);
                PILO_ERRRET(err);

                vp = this->_configuator->get_value("loggers", err);
                PILO_ERRRET(err);
                if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_array) {
                    return ::pilo::mk_err(PERR_MIS_DATA_TYPE);
                }
                for (::pilo::i32_t i = 0; i < vp->size(); i++) {
                    svp = vp->get<::pilo::tlv*>(i, &err);
                    PILO_ERRRET(err);
                    if (svp == nullptr)
                        return ::pilo::mk_err(PERR_NULL_PTR);                    
                    if (svp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_dict) {
                        return ::pilo::mk_err(PERR_MIS_DATA_TYPE);                        
                    }
                    err = svp->get<std::string, ::pilo::tlv*>(std::string("type"), svp2);
                    PILO_ERRRET(err);
                    err = ::pilo::core::string::str_to_id<::pilo::core::logging::logger_type, ::pilo::u8_t>(tmp_logger._type, svp2->daynamic_data(), svp2->as_str_length()
                        , ::pilo::core::logging::g_logger_type_names, (::pilo::u8_t) (PMF_COUNT_OF(::pilo::core::logging::g_logger_type_names)), true);
                    PILO_ERRRET(err);


                }


                return PILO_OK;
            }


            ::pilo::err_t core_config::save_to_configurator()
            {
                ::pilo::err_t err = PILO_OK;
                PILO_CHKERR_RET(err, this->_configuator->set_value("cwd", _cwd.c_str(), (::pilo::i32_t) _cwd.size(), false, true, true));
                char flag_buffer[128] = { 0 };
                char* flag_buffer_ptr = flag_buffer;
                
                char buffer[64] = { 0 };
                for (int i = 0; i < _loggers.size(); i++) {
                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].type", i);
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, _loggers[i].type_name(), -1, false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].level", i);
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, _loggers[i].level_name(), -1, false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].splition_type", i);
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, _loggers[i].splition_type_name(), -1, false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].outputs", i);
                    flag_buffer_ptr = this->logger_at(i)->get_outputs_devs(flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].name_suffix", i);
                    flag_buffer_ptr = this->logger_at(i)->get_predef_elements(_loggers[i].outputs(), flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].headers", i);
                    flag_buffer_ptr = this->logger_at(i)->get_predef_elements(_loggers[i].headers(), flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].split_every", i);
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, _loggers[i].split_every(), true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].flags", i);
                    flag_buffer_ptr = this->logger_at(i)->get_predef_elements(_loggers[i].flags(), flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].bak_name_suffix", i);
                    flag_buffer_ptr = this->logger_at(i)->get_predef_elements(_loggers[i].bak_name_suffix(), flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].size_quota", i);
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, _loggers[i].size_quota(), true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].name", i);
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, _loggers[i].name().c_str(), (::pilo::i32_t) _loggers[i].name().size(), false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].bak_dir", i);
                    PILO_CHKERR_RET(err, this->_configuator->set_value(buffer, _loggers[i].bak_dir().c_str(), (::pilo::i32_t) _loggers[i].bak_dir().size(), false, true, true));

                }


                return PILO_OK;
            }

            
        
        }
    }
}