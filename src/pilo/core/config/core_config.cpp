#include "core_config.hpp"
#include "../process/context.hpp"
#include "../../tlv.hpp"

namespace pilo {
    namespace core {
        namespace config {

            core_config::core_config()
                : _cwd("")
            {
                char buffer[128] = { 0 };
                ::pilo::i32_t rlen = 0;
                char* cret = ::pilo::core::process::xpf_get_proc_basename(buffer, sizeof(buffer), &rlen, ".json", 5);
                this->_file_path.set(cret, rlen, 0, ::pilo::predefined_pilo_dir::bin);
                PMF_COMPARE_HEAP_FREE(cret, buffer);
            }

            ::pilo::err_t core_config::load()
            {
                ::pilo::err_t err = PILO_OK;

                json_configuator jct;
               err = jct.load(&_file_path);
                if (err != PILO_OK)
                    return err;
               
                err = this->load_from_configurator(&jct);

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
                tmp_logger.set_bak_dir("baks");
                tmp_logger.set_size_quota(1024 * 1024);
                tmp_logger.set_field_sep(SP_PMS_LOG_FILE_DFL_FLD_SEP);

                this->_loggers.emplace_back(std::move(tmp_logger));
                tmp_logger.set_defualt();
                tmp_logger.set_type(::pilo::core::logging::logger_type::local_spst_text);
                tmp_logger.set_name(buffer);
                tmp_logger.set_bak_name_suffix(::pilo::core::logging::DefaultBakNameSuffix);
                tmp_logger.set_field_sep(SP_PMS_LOG_FILE_DFL_FLD_SEP);
                
                this->_loggers.emplace_back(std::move(tmp_logger));

                return PILO_OK;
            }

            ::pilo::err_t core_config::save()
            {
                json_configuator jct;

                ::pilo::err_t err = save_to_configurator(&jct);
                if (err != PILO_OK) {
                    return err;
                }

                return jct.save(&_file_path);
            }

            void core_config::clear(bool purge)
            {
                this->_cwd.clear();
                this->_loggers.clear();
                if (purge) {
                    this->_loggers.shrink_to_fit();
                }
            }

            ::pilo::err_t core_config::load_from_configurator(configuator_interface* configuator)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::tlv* vp = nullptr;
                ::pilo::tlv* svp = nullptr;
                ::pilo::tlv* svp2 = nullptr;
                logger tmp_logger;



                PILO_CHKERR_RET(err, configuator->get_value("cwd", this->_cwd));

                vp = configuator->get_value_node("loggers", err);
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
                    err = svp2->to_enum_id(tmp_logger._type, ::pilo::core::logging::g_logger_type_names, (::pilo::u8_t)(PMF_COUNT_OF(::pilo::core::logging::g_logger_type_names)), true);
                    PILO_ERRRET(err);
                    
                    err = svp->get<std::string, ::pilo::tlv*>(std::string("level"), svp2);
                    PILO_ERRRET(err);
                    err = svp2->to_enum_id(tmp_logger._level, ::pilo::core::logging::g_level_names, (::pilo::u8_t)(PMF_COUNT_OF(::pilo::core::logging::g_level_names)), true);
                    PILO_ERRRET(err);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("splition_type"), svp2);
                    PILO_ERRRET(err);
                    err = svp2->to_enum_id(tmp_logger._splition_type, ::pilo::core::logging::g_splition_type_names, (::pilo::u8_t)(PMF_COUNT_OF(::pilo::core::logging::g_splition_type_names)), true);
                    PILO_ERRRET(err);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("outputs"), svp2);
                    PILO_ERRRET(err);
                    svp2->to_flags<::pilo::u8_t, PMF_COUNT_OF(::pilo::core::logging::g_output_dev_names)>(tmp_logger._outputs, ",", 1, ::pilo::core::logging::g_output_dev_names);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("name_suffix"), svp2);
                    PILO_ERRRET(err);
                    svp2->to_flags<::pilo::u32_t, PMF_COUNT_OF(::pilo::core::logging::g_predef_elment_names)>(tmp_logger._name_suffix, ",", 1, ::pilo::core::logging::g_predef_elment_names);


                    err = svp->get<std::string, ::pilo::tlv*>(std::string("headers"), svp2);
                    PILO_ERRRET(err);
                    svp2->to_flags<::pilo::u32_t, PMF_COUNT_OF(::pilo::core::logging::g_predef_elment_names)>(tmp_logger._headers, ",", 1, ::pilo::core::logging::g_predef_elment_names);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("split_every"), svp2);
                    tmp_logger._split_every = svp2->as_i32(&err);
                    PILO_ERRRET(err);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("flags"), svp2);
                    PILO_ERRRET(err);
                    svp2->to_flags<::pilo::u32_t, PMF_COUNT_OF(::pilo::core::logging::g_flags)>(tmp_logger._flags, ",", 1, ::pilo::core::logging::g_flags);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("bak_name_suffix"), svp2);
                    PILO_ERRRET(err);
                    svp2->to_flags<::pilo::u32_t, PMF_COUNT_OF(::pilo::core::logging::g_predef_elment_names)>(tmp_logger._bak_name_suffix, ",", 1, ::pilo::core::logging::g_predef_elment_names);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("size_quota"), svp2);
                    tmp_logger._size_quota = svp2->as_i64(&err);
                    PILO_ERRRET(err);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("piece_quota"), svp2);
                    tmp_logger._piece_quota = svp2->as_i64(&err);
                    PILO_ERRRET(err);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("name"), svp2);
                    PILO_ERRRET(err);
                    err = svp2->assign_as_str(tmp_logger._name);
                    PILO_ERRRET(err);
                    
                    err = svp->get<std::string, ::pilo::tlv*>(std::string("bak_dir"), svp2);
                    PILO_ERRRET(err);
                    err = svp2->assign_as_str(tmp_logger._bak_dir);
                    PILO_ERRRET(err);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("line_sep"), svp2);
                    PILO_ERRRET(err);
                    err = svp2->assign_as_str(tmp_logger._line_sep);
                    PILO_ERRRET(err);

                    err = svp->get<std::string, ::pilo::tlv*>(std::string("field_sep"), svp2);
                    PILO_ERRRET(err);
                    err = svp2->assign_as_str(tmp_logger._field_sep);
                    PILO_ERRRET(err);


                    this->_loggers.push_back(tmp_logger);
                }


                return PILO_OK;
            }


            ::pilo::err_t core_config::save_to_configurator(configuator_interface * configuator)
            {
                ::pilo::err_t err = PILO_OK;
                PILO_CHKERR_RET(err, configuator->set_value("cwd", _cwd.c_str(), (::pilo::i32_t) _cwd.size(), false, true, true));
                char flag_buffer[128] = { 0 };
                char* flag_buffer_ptr = flag_buffer;
                
                char buffer[64] = { 0 };
                for (int i = 0; i < _loggers.size(); i++) {
                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].type", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].type_name(), -1, false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].level", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].level_name(), -1, false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].splition_type", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].splition_type_name(), -1, false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].outputs", i);
                    flag_buffer_ptr = this->logger_at(i)->get_outputs_devs(flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].name_suffix", i);
                    flag_buffer_ptr = this->logger_at(i)->get_predef_elements(_loggers[i].name_suffix().data(), flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].headers", i);
                    flag_buffer_ptr = this->logger_at(i)->get_predef_elements(_loggers[i].headers().data(), flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].split_every", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].split_every(), true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].flags", i);
                    flag_buffer_ptr = this->logger_at(i)->get_predef_elements(_loggers[i].flags(), flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].bak_name_suffix", i);
                    flag_buffer_ptr = this->logger_at(i)->get_predef_elements(_loggers[i].bak_name_suffix().data(), flag_buffer, sizeof(flag_buffer));
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, flag_buffer_ptr, -1, false, true, true));
                    PMF_COMPARE_HEAP_FREE(flag_buffer_ptr, flag_buffer);

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].size_quota", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].size_quota(), true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].piece_quota", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].piece_quota(), true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].name", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].name().c_str(), (::pilo::i32_t) _loggers[i].name().size(), false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].bak_dir", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].bak_dir().c_str(), (::pilo::i32_t) _loggers[i].bak_dir().size(), false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].line_sep", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].line_sep().c_str(), (::pilo::i32_t)_loggers[i].line_sep().size(), false, true, true));

                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "loggers.[%d].field_sep", i);
                    PILO_CHKERR_RET(err, configuator->set_value(buffer, _loggers[i].field_sep().c_str(), (::pilo::i32_t)_loggers[i].field_sep().size(), false, true, true));

                }


                return PILO_OK;
            }

            
        
        }
    }
}