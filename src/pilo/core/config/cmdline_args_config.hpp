#ifndef _pilo_core_config_cmdline_args_config_h_
#define _pilo_core_config_cmdline_args_config_h_

#include "../logging/logger_def.hpp"
#include "../io/path.hpp"
#include "../ml/json_tlv_driver.hpp"
#include "./configuation_interface.hpp"
#include <unordered_map>

namespace pilo {
    namespace core {
        namespace config {

            class cmdline_spec
            {
            public:
                const ::pilo::u8_t Required = 0x1;


            public:
                cmdline_spec()
                    : _short_name(-1)
                    , _wrapper_type(::pilo::core::rtti::wired_type::wrapper_na)
                    , _key_type(::pilo::core::rtti::wired_type::key_type_na)
                    , _val_type(::pilo::core::rtti::wired_type::value_type_na)
                    , _flags(0)
                    , _min_sub_arg_count(0)
                    , _max_sub_arg_count(-1)
                    , _long_name("")
                {}

                cmdline_spec(char short_name, ::pilo::u8_t wt, char key_type_char, char val_type_char, bool is_required, ::pilo::i32_t min_sa, ::pilo::i32_t max_sa, const char* long_name);
                cmdline_spec(const char* spec_str_fld, ::pilo::i64_t len);
                inline void clear()
                {
                    _short_name = -1;
                    _wrapper_type = ::pilo::core::rtti::wired_type::wrapper_na;
                    _key_type = ::pilo::core::rtti::wired_type::key_type_na;
                    _val_type = ::pilo::core::rtti::wired_type::value_type_na;
                    _flags = 0;
                    _min_sub_arg_count = 0;
                    _max_sub_arg_count = -1;
                    _long_name = "";
                }

                bool is_flag() const { return _wrapper_type == ::pilo::core::rtti::wired_type::wrapper_na;  }
                bool is_single() const { return _wrapper_type == ::pilo::core::rtti::wired_type::wrapper_single; }
                bool is_list() const { return _wrapper_type == ::pilo::core::rtti::wired_type::wrapper_array; }
                bool is_dict() const { return _wrapper_type == ::pilo::core::rtti::wired_type::wrapper_dict; }
                ::pilo::u8_t wrapper_type() const { return _wrapper_type; }
                ::pilo::u8_t key_type() const { return _key_type; }
                ::pilo::u16_t value_type() const { return this->_val_type;  }
                ::pilo::i32_t min_count() const { return _min_sub_arg_count; }
                ::pilo::i32_t max_count() const { return _max_sub_arg_count; }

                ::pilo::err_t can_append_sub_arg(::pilo::i32_t current_cnt) const
                {
                    if (_max_sub_arg_count >= 0 && current_cnt >= _max_sub_arg_count) {

                        return ::pilo::mk_perr(PERR_LEN_TOO_LARGE);
                    }
                    return PILO_OK;
                }

                //l:log-level:u8:1:1:true

            public:
                const char* to_cstring(char* buffer, ::pilo::i64_t bufsz); 
                ::pilo::err_t from_cstring(const char* spec_str_fld, ::pilo::i64_t len);

                inline bool invalid() const 
                { 
                    if (_short_name < 0 && _long_name.empty())
                        return true;
                    return false;
                }
                inline char short_name() const { return _short_name; }
                inline const std::string& long_name() const { return _long_name; }
                inline bool required() const { return _flags.test_value(Required);  }

                std::string name() const
                {
                    std::string str;
                    if (_short_name >= 0 && !_long_name.empty()) {
                        str += _short_name;
                        str += ":";
                        str += _long_name;
                    }
                    else if (_short_name >= 0) {
                        str += _short_name;
                    }
                    else if (!_long_name.empty()) {
                        str += _long_name;
                    }
                    return str;
                }
                
            private:
                char                            _short_name;
                ::pilo::u8_t                    _wrapper_type;
                ::pilo::u8_t                    _key_type;
                ::pilo::u16_t                   _val_type;
                ::pilo::bit_flag<::pilo::u8_t>  _flags;
                ::pilo::i32_t                   _min_sub_arg_count;
                ::pilo::i32_t                   _max_sub_arg_count;
                std::string                     _long_name;
            };




            class cmdline_args_config : public configuation_interface {
            public:
                friend class core_config;
            public:
                cmdline_args_config() : _short_specs{ nullptr } {}
                ~cmdline_args_config();

                ::pilo::err_t add_spec(cmdline_spec* spec)
                {
                    if (spec->short_name() >= 0) {
                        if (_short_specs[spec->short_name()] != nullptr) {
                            return ::pilo::mk_perr(PERR_EXIST);
                        }
                        _short_specs[spec->short_name()] = spec;
                    }               

                    if (!spec->long_name().empty()) {
                        if (_long_specs.count(spec->long_name()) > 0) {
                            return ::pilo::mk_perr(PERR_EXIST);
                        }

                        _long_specs.insert(std::pair<std::string, cmdline_spec*>(spec->long_name(), spec));
                    }
                    return PILO_OK;
                }

                // Inherited via configuation_interface
                ::pilo::err_t load() override;

                ::pilo::err_t save() override;

                ::pilo::err_t set_default() override;

                void clear(bool purge) override;

                ::pilo::err_t save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver) override;

                ::pilo::err_t load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver) override;

                const::pilo::core::io::path* file_path() const override;

                bool invalid() const override;

                inline bool has_target() const {
                    return _targets.value_type() != ::pilo::core::rtti::wired_type::value_type_na;
                }

                const cmdline_spec& target_spec() const { return _targets;  }


                bool has_arg() const;
                const cmdline_spec* spec(char c) const { return _short_specs[c]; }
                const cmdline_spec* spec(const std::string& l) const 
                { 
                    std::map<std::string, cmdline_spec*>::const_iterator it = _long_specs.find(l);
                    if (it == _long_specs.end())
                        return nullptr;
                    return it->second; 
                }

            private:
                cmdline_spec* _short_specs[128];
                std::map<std::string, cmdline_spec*>      _long_specs;
                cmdline_spec    _targets;
            };

        }
    }
}



#endif