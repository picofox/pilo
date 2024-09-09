#ifndef _pilo_core_config_configuation_interface_h_
#define _pilo_core_config_configuation_interface_h_

#include "../ml/json_tlv_driver.hpp"


namespace pilo {
    namespace core {

        namespace io {
            class path;
        }

        namespace config {


            class configuation_interface
            {
            public:
                virtual ~configuation_interface() {};
                virtual ::pilo::err_t load() = 0;
                virtual ::pilo::err_t save() = 0;
                virtual ::pilo::err_t set_default() = 0;
                virtual void clear(bool purge = true) = 0;
                virtual ::pilo::err_t save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface * configuator_driver) = 0;
                virtual ::pilo::err_t load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver) = 0;
                virtual const ::pilo::core::io::path* file_path() const = 0;
                virtual bool invalid() const = 0;
            };
        }
    }
}

#endif // !_pilo_core_config_configuation_interface_h_
