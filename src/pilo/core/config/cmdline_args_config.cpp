#include "./cmdline_args_config.hpp"

namespace pilo 
{
    namespace core 
    {
        namespace config 
        {
            cmdline_spec::cmdline_spec(char short_name, ::pilo::u8_t wt, char key_type_char, char val_type_char, bool is_required, ::pilo::i32_t min_sa, ::pilo::i32_t max_sa, const char* long_name)
            {
                _short_name = short_name;
                _wrapper_type = wt;
                _key_type = ::pilo::core::rtti::wired_type::s_char_to_key_type(key_type_char);
                _val_type = ::pilo::core::rtti::wired_type::s_char_to_value_type(val_type_char);
                if (is_required)
                    _flags.mark_value(Required);
                else
                    _flags.clear_value(Required);
                _min_sub_arg_count = min_sa;
                _max_sub_arg_count = max_sa;
                _long_name = long_name;
            }

            cmdline_spec::cmdline_spec(const char* spec_str_fld, ::pilo::i64_t len)
            {
                from_cstring(spec_str_fld, len);
            }

            const char* cmdline_spec::to_cstring(char* buffer, ::pilo::i64_t bufsz)
            {
                char typestr[8] = { 0 };
                ::pilo::err_t err = ::pilo::core::rtti::wired_type::s_to_wired_type_cstr(typestr, 8, _wrapper_type, _key_type, _val_type);
                if (err != PILO_OK && err != PERR_NOOP)
                    return nullptr;
                if (_short_name > 0) {
                    ::pilo::core::io::string_formated_output(buffer, bufsz, "%c:%s:%s:%d:%d:%s", _short_name, _long_name.c_str(), typestr, _min_sub_arg_count, _max_sub_arg_count, required() ? "true" : "false");
                }
                else {
                    ::pilo::core::io::string_formated_output(buffer, bufsz, ":%s:%s:%d:%d:%s", _long_name.c_str(), typestr, _min_sub_arg_count, _max_sub_arg_count, required() ? "true" : "false");
                }
                
                return buffer;
            }

            ::pilo::err_t cmdline_spec::from_cstring(const char* spec_str_fld, ::pilo::i64_t len)
            {
                _val_type = ::pilo::core::rtti::wired_type::value_type_na;
                ::pilo::err_t err = PILO_OK;
                ::pilo::cstr_ref<char> parts[6];
                ::pilo::i64_t cnt = ::pilo::core::string::split_fixed(spec_str_fld, len, ":", 1, parts, 6, false, true, true, true);

                if (cnt < 6) return ::pilo::mk_perr(PERR_INVALID_PARAM);

                if (parts[0].length < 1)
                    _short_name = -1;
                else
                    _short_name = parts[0].ptr[0];

                if (parts[1].length < 2)
                    _long_name.clear();
                else
                    _long_name.assign(parts[1].ptr, parts[1].length);

                if (_short_name == 0 && _long_name.empty()) 
                    ::pilo::mk_perr(PERR_INVALID_PARAM);


                PILO_CHKERR_RET(err, ::pilo::core::rtti::wired_type::s_parse_cstr_type(this->_wrapper_type, this->_key_type, this->_val_type, parts[2].ptr, parts[2].length));
                PILO_CHKERR_RET(err, ::pilo::core::string::string_to_number(this->_min_sub_arg_count, parts[3].ptr, parts[3].length));
                PILO_CHKERR_RET(err, ::pilo::core::string::string_to_number(this->_max_sub_arg_count, parts[4].ptr, parts[4].length));

                bool isreq = false;
                PILO_CHKERR_RET(err, ::pilo::core::rtti::wired_type::s_parse_cstr_bool(isreq, parts[5].ptr, parts[5].length));

                if (isreq)
                    _flags.mark_value(Required);
                else
                    _flags.clear_value(Required);

                return PILO_OK;
            }

            cmdline_args_config::~cmdline_args_config()
            {
                clear(false);
            }
            ::pilo::err_t cmdline_args_config::load()
            {
                return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
            }

            ::pilo::err_t cmdline_args_config::save()
            {
                return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
            }

            ::pilo::err_t cmdline_args_config::set_default()
            {
                memset(_short_specs, 0x00, sizeof(_short_specs));
                _long_specs.clear();
                _targets.clear();


                cmdline_spec * sp = new cmdline_spec("d:deamon::0:0:false", -1);
                add_spec(sp);
                sp = new cmdline_spec("D:debug:d:1:1:false", -1);
                add_spec(sp);
                sp = new cmdline_spec("b:bind-address:*s:1:10:false", -1);
                add_spec(sp);
                sp = new cmdline_spec(":ext-vars:#ss:1:-1:false", -1);
                add_spec(sp);
                sp = new cmdline_spec("z:::0:0:false", -1);
                add_spec(sp);
                sp = new cmdline_spec("m:multi-thread::0:0:false", -1);
                add_spec(sp);

                _targets.from_cstring("::s:1:-1:true", -1);

                return PILO_OK;
            }

            void cmdline_args_config::clear(bool purge)
            {
                PMC_UNUSED(purge);
                for (int i = 0; i < 128; i++) {
                    if (_short_specs[i] != nullptr) {
                        if (!_short_specs[i]->long_name().empty()) {
                            _long_specs.erase(_short_specs[i]->long_name());
                        }
                        delete _short_specs[i];
                        _short_specs[i] = nullptr;
                    }
                }

                std::unordered_map<std::string, cmdline_spec*>::const_iterator cit = _long_specs.cbegin();
                for (; cit != _long_specs.cend(); cit++) {
                    delete cit->second;
                }
                _long_specs.clear();
            }

            ::pilo::err_t cmdline_args_config::save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* driver)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::core::memory::object_array<char, 128>   cb;

                ::pilo::i32_t fqdn_len = (::pilo::i32_t) ::pilo::core::string::character_count(fqdn_path);
                cb.check_space(fqdn_len + 32);
                ::pilo::core::string::n_copyz(cb.begin(), cb.capacity(), fqdn_path, fqdn_len);
                cb.set_size(fqdn_len);

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".args", 5);
                cb.add_size(5);

                int idx = 0;
                char one_arg_sp_buf[128] = { 0 };
                for (int i = 0; i < 128; i++) {
                    if (_short_specs[i] != nullptr) {
                        _short_specs[i]->to_cstring(one_arg_sp_buf, sizeof(one_arg_sp_buf));
                        ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".[%d]", idx++);
                        PILO_CHKERR_RET(err, driver->set_value(cb.begin(), one_arg_sp_buf, (::pilo::i32_t)-1, false, true, true));
                    }
                }
                std::unordered_map<std::string, cmdline_spec*>::const_iterator cit = _long_specs.cbegin();
                for (; cit != _long_specs.cend(); cit++) {
                    if (cit->second->short_name() >= 0) {
                        continue;
                    }
                    if (cit->second->short_name() < 0) {
                        cit->second->to_cstring(one_arg_sp_buf, sizeof(one_arg_sp_buf));
                        ::pilo::core::io::string_formated_output(cb.ptr(), cb.space_available(), ".[%d]", idx++);
                        PILO_CHKERR_RET(err, driver->set_value(cb.begin(), one_arg_sp_buf, (::pilo::i32_t)-1, false, true, true));
                    }
                }

                cb.add_size(-5);
                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".targets", 8);

                if (_targets.is_single()) {
                    _targets.to_cstring(one_arg_sp_buf, sizeof(one_arg_sp_buf));
                    PILO_CHKERR_RET(err, driver->set_value(cb.begin(), one_arg_sp_buf, (::pilo::i32_t)-1, false, true, true));
                }
                else {
                    PILO_CHKERR_RET(err, driver->set_value(cb.begin(), "", (::pilo::i32_t)0, false, true, true));
                }
                    

                return PILO_OK;
            }

            ::pilo::err_t cmdline_args_config::load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* driver)
            {
                ::pilo::err_t err = PILO_OK;
                ::pilo::core::memory::object_array<char, 128>   cb;

                ::pilo::i32_t fqdn_len = (::pilo::i32_t) ::pilo::core::string::character_count(fqdn_path);
                cb.check_space(fqdn_len + 32);
                ::pilo::core::string::n_copyz(cb.begin(), cb.capacity(), fqdn_path, fqdn_len);
                cb.set_size(fqdn_len);

                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".args", 5);
                cb.add_size(5);

                ::pilo::tlv* arg_tlv = driver->get_value_node(cb.begin(), err);
                if (arg_tlv != nullptr) { 
                    for (int i = 0; i < arg_tlv->size(); i++) {
                        ::pilo::tlv* tlvp = arg_tlv->get<::pilo::tlv*>(i, &err);
                        if (tlvp == nullptr)
                            return err;
                        if (tlvp->size() < 1)
                            return ::pilo::mk_perr(PERR_VAL_EMPTY);

                        cmdline_spec* sp = new cmdline_spec(tlvp->daynamic_data(), tlvp->size());
                        if (sp->invalid()) {
                            return ::pilo::mk_perr(PERR_INV_OBJECT);
                        }
                        this->add_spec(sp);
                    }                    
                }
                
                cb.add_size(-5);
                ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), ".targets", 8);
                cb.add_size(8);
                
                ::pilo::tlv* tgt_tlv = driver->get_value_node(cb.begin(), err);
                if (tgt_tlv != nullptr) {
                    if (tgt_tlv->size() > 0 && tgt_tlv->daynamic_data()[0] != 0) {
                        this->_targets.from_cstring(tgt_tlv->daynamic_data(), tgt_tlv->size());
                        if (_targets.value_type() == ::pilo::core::rtti::wired_type::value_type_na) {
                            return ::pilo::mk_perr(PERR_INV_OBJECT);
                        }
                    }
                }

                return PILO_OK;
            }

            const::pilo::core::io::path* cmdline_args_config::file_path() const
            {
                return nullptr;
            }

            bool cmdline_args_config::invalid() const
            {
                return false;
            }

            bool cmdline_args_config::has_arg() const
            {
                for (int i = 0; i < 128; i++) {
                    if (_short_specs[i] != nullptr) {
                        return true;
                    }
                }
                if (!_long_specs.empty())
                    return true;
                return false;
            }

        }
    }
}