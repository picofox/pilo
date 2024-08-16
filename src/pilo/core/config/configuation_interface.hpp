#ifndef _pilo_core_config_configuation_interface_h_
#define _pilo_core_config_configuation_interface_h_

#include "./configuator_interface.hpp"


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
                virtual ::pilo::err_t save_to_configurator(configuator_interface * configuator) = 0;
                virtual ::pilo::err_t load_from_configurator(configuator_interface* configuator) = 0;
                virtual const ::pilo::core::io::path* file_path() const = 0;
            };
        }
    }
}

#endif // !_pilo_core_config_configuation_interface_h_
