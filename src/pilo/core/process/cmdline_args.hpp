#ifndef _pilo_core_process_cmdline_args_hpp_
#define _pilo_core_process_cmdline_args_hpp_

#include "../../pilo.hpp"
#include "../../types.hpp"
#include "../rtti/wired_type.hpp"
#include "../string/string_operation.hpp"
#include <unordered_map>

namespace pilo
{
    namespace core
    {
        namespace process
        {
            class cmdline_spec
            {
            public:
                const ::pilo::u8_t Required = 0x1;
   

            public:
                cmdline_spec()
                    : _short_name(0)
                    , _wrapper_type(::pilo::core::rtti::wired_type::wrapper_na)
                    , _key_type(::pilo::core::rtti::wired_type::key_type_na)
                    , _val_type(::pilo::core::rtti::wired_type::value_type_na)
                    , _flags(0)
                    , _min_sub_arg_count(0)
                    , _max_sub_arg_count(0)
                    , _long_name("")
                    {}

                cmdline_spec(char short_name, ::pilo::u8_t wt, char key_type_char, char val_type_char, bool is_required, ::pilo::i32_t min_sa, ::pilo::i32_t max_sa, const char* long_name);
                cmdline_spec(const char* spec_str_fld, ::pilo::i64_t len);
                


                //l:log-level:u8:1:1:true

            public:
                bool valid() const { return (_val_type != ::pilo::core::rtti::wired_type::value_type_na);  }
                inline char short_name() const { return _short_name;  }
                inline const std::string& long_name() const { return _long_name; }

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

            class cmdline_arg
            {
            public:
                cmdline_arg() {}
                ~cmdline_arg();

                ::pilo::err_t parse(const char* cstr, ::pilo::i64_t cslen);
                


                ::pilo::err_t add_spec(cmdline_spec* spec)
                {
                    if (spec->short_name() < 0) {
                        _short_specs[spec->short_name()] = spec;
                    }
                    if (!spec->long_name().empty()) {                        
                        _long_specs.insert(std::pair<std::string, cmdline_spec*>(spec->long_name(), spec));
                    }
                    return PILO_OK;
                }
            private:
                cmdline_spec *                                      _short_specs[128];
                std::unordered_map<std::string, cmdline_spec*>      _long_specs;


            };
        }
    }
}

#endif