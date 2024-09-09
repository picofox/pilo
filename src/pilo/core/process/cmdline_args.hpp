#ifndef _pilo_core_process_cmdline_args_hpp_
#define _pilo_core_process_cmdline_args_hpp_

#include "../../pilo.hpp"
#include "../../types.hpp"
#include "../rtti/wired_type.hpp"
#include "../string/string_operation.hpp"
#include <unordered_map>
#include <vector>
#include "../../tlv.hpp"

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

                ::pilo::err_t parse(int argc, char *argv[]);
                
            private:
                ::pilo::err_t _make_target(const char* argv, ::pilo::i32_t len);


            private:
                ::pilo::tlv*                                        _short_args[128];
                std::unordered_map<std::string, ::pilo::tlv*>       _long_args;
                std::vector<::pilo::tlv*>                           _targets;
            };
        }
    }
}

#endif