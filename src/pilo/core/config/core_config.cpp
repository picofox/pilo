#include "core_config.hpp"
#include "../process/context.hpp"

namespace pilo {
    namespace core {
        namespace config {

            ::pilo::err_t core_config::load()
            {
                ::pilo::err_t err = PILO_OK;
                
                ::pilo::core::io::path* p = this->_configuator->file_path();
                if (p == nullptr || p->invalid()) {
                    char buffer[128] = { 0 };
                    ::pilo::i64_t flen = ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%s.json", PILO_CONTEXT->process_basename());
                    err = this->_configuator->set_file(buffer, (::pilo::pathlen_t) flen, ::pilo::predefined_pilo_dir::bin);
                    return err;
                }

                    
               

                return PILO_OK;
            }

        
        
        }
    }
}