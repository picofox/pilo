#ifndef _pilo_core_process_cmdline_args_hpp_
#define _pilo_core_process_cmdline_args_hpp_

#include "../../pilo.hpp"
#include "../../types.hpp"
#include "../rtti/wired_type.hpp"
#include "../string/string_operation.hpp"
#include <unordered_map>
#include <vector>
#include "../../tlv.hpp"
#include "../config/cmdline_args_config.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
        
            class cmdline_arg
            {
            public:
                cmdline_arg() : _short_args{nullptr} {}
                ~cmdline_arg();

                ::pilo::err_t parse(int argc, char *argv[], std::string& errmsg);
                std::string args_to_string() const;
                std::string targets_to_string() const;
                ::pilo::tlv* find_arg(char s_name, const std::string& str) const;
                
            private:
                ::pilo::err_t _validate(std::string & errmsg);
                ::pilo::err_t _make_target(const char* argv, ::pilo::i32_t len);
                ::pilo::err_t _set_arg(const ::pilo::core::config::cmdline_spec* spec, std::string & errmsg, const char* argv, ::pilo::i32_t len, bool * is_all_set);
                void _compose_errmsg(std::string& errmsg, const char* part1, const char* which_arg, const char* part2, const char* val);
                bool _is_all_flag(const char* str, int len);
                 
            private:
                ::pilo::tlv*                                        _short_args[128];
                std::map<std::string, ::pilo::tlv*>       _long_args;
                std::vector<::pilo::tlv*>                           _targets;
            };
        }
    }
}

#endif