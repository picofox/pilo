#ifndef _pilo_core_config_configuator_interface_h_
#define _pilo_core_config_configuator_interface_h_

#include "../io/path.hpp"
#include "../io/file.hpp"
#include "../../tlv.hpp"


namespace pilo {
    namespace core {
        namespace config {

            enum class configurator_type : ::pilo::i32_t
            {
                json = 0,
            };

            class configuator_interface
            {
            public:
                virtual ~configuator_interface() {};
                virtual ::pilo::err_t load() = 0;
                virtual ::pilo::err_t load(const char* data, ::pilo::i64_t len) = 0;
                virtual ::pilo::err_t save() = 0;
                virtual ::pilo::tlv* root() = 0;
                virtual ::pilo::tlv* get_value(const char* fqn, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t set_file(const char* file, ::pilo::pathlen_t len, ::pilo::predefined_pilo_dir predef_dir) = 0;
                virtual ::pilo::core::io::path* file_path() = 0;
            };
        }
    }
}

#endif // !_pilo_core_config_config_interface_h_
