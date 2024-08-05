#ifndef _pilo_core_config_configuation_interface_h_
#define _pilo_core_config_configuation_interface_h_

#include "../io/path.hpp"
#include "../io/file.hpp"
#include "../../tlv.hpp"


namespace pilo {
    namespace core {
        namespace config {


            class configuation_interface
            {
            public:
                virtual ~configuation_interface() {};
                virtual ::pilo::err_t load() = 0;
                virtual ::pilo::err_t save() = 0;
                virtual ::pilo::err_t set_default() = 0;
                virtual void clear(bool purge = true) = 0;
                virtual ::pilo::err_t save_to_configurator() = 0;
                virtual ::pilo::err_t load_from_configurator() = 0;
            };
        }
    }
}

#endif // !_pilo_core_config_configuation_interface_h_
