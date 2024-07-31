﻿#ifndef _pilo_core_config_configuator_interface_h_
#define _pilo_core_config_configuator_interface_h_

#include "../io/path.hpp"
#include "../io/file.hpp"
#include "../../tlv.hpp"


namespace pilo {
    namespace core {
        namespace config {
            class configuator_interface
            {
            public:
                virtual ::pilo::err_t load() = 0;
                virtual ::pilo::err_t load(const char* data, ::pilo::i64_t len) = 0;
                virtual ::pilo::err_t save() = 0;
                virtual ::pilo::tlv* root() = 0;
                virtual ::pilo::tlv* get_value(const char* fqn, ::pilo::err_t& err) = 0;
            };
        }
    }
}

#endif // !_pilo_core_config_config_interface_h_