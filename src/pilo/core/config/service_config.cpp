#include "./service_config.hpp"

namespace pilo
{
    namespace core
    {
        namespace config
        {
            ::pilo::err_t service_config::load()
            {
                return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
            }
            ::pilo::err_t service_config::save()
            {
                return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
            }
            ::pilo::err_t service_config::set_default()
            {
                clear(false);
                return PILO_OK;
            }
            void service_config::clear(bool)
            {             
                _name.clear();
                _pulse_msec = 0;
                _type_id = -1;
                _pulse_msec = SP_PMI_SERVICE_PULSE_DEFAULT;
                if (_param != nullptr) {
                    ::pilo::tlv::deallocate(_param);
                    _param = nullptr;
                }
            }
            ::pilo::err_t service_config::save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::core::memory::object_array<char, 128>   cb;

                ::pilo::i32_t fqdn_len = (::pilo::i32_t) ::pilo::core::string::character_count(fqdn_path);
                cb.check_space(fqdn_len + 64);
                ::pilo::core::string::n_copyz(cb.begin(), cb.capacity(), fqdn_path, fqdn_len);
                cb.set_size(fqdn_len);

                ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".name", 5);
                PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), this->_name, true));

                ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".pulse_msec", 11);
                PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), this->_pulse_msec, true));


                ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".param.", 7);
                cb.add_size(7);

                if (_param != nullptr) {
                    const std::map<std::string, ::pilo::tlv*>* m = (const std::map<std::string, ::pilo::tlv*>*) _param->daynamic_data();
                    if (m != nullptr) {
                        std::map<std::string, ::pilo::tlv*>::const_iterator pcit = m->cbegin();
                        for (; pcit != m->cend(); pcit ++) {
                            cb.check_more_space((::pilo::i32_t)pcit->first.size() + 1);
                            ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), pcit->first.c_str(), pcit->first.size());
                            PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), pcit->second->as_str(&err, nullptr), true));
                        }

                    }
                }

                return PILO_OK;
            }
            ::pilo::err_t service_config::load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::core::memory::object_array<char, 128>   cb;
                ::pilo::tlv* vp = nullptr;

                ::pilo::i32_t fqdn_len = (::pilo::i32_t) ::pilo::core::string::character_count(fqdn_path);
                cb.check_space(fqdn_len + 64);
                ::pilo::core::string::n_copyz(cb.begin(), cb.capacity(), fqdn_path, fqdn_len);
                cb.set_size(fqdn_len);

                ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".name", 5);                
                PILO_CHKERR_RET(err, configuator_driver->get_value(cb.begin(), this->_name));

                ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".pulse_msec", 11);
                PILO_CHKERR_RET(err, configuator_driver->get_value(cb.begin(), this->_pulse_msec));


                ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".param", 6);
                cb.add_size(6);
                vp = configuator_driver->get_value_node(cb.begin(), err);
                if (vp != nullptr && vp->daynamic_data() != nullptr && vp->size() > 0) {
                    if (vp->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_dict) {
                        return ::pilo::mk_err(PERR_MIS_DATA_TYPE);
                    }                    
                    _param = vp->clone();
                }
                else {
                    _param = nullptr;
                }
                return PILO_OK;
            }
            const::pilo::core::io::path* service_config::file_path() const
            {
                return nullptr;
            }
            bool service_config::invalid() const
            {
                return false;
            }
        }
    }
}