#include "../memory/util.hpp"
#include "logger_manager.hpp"

namespace pilo {
    namespace core {
        namespace logging {


            logger_manager::logger_manager()
            {

            }

            logger_manager::~logger_manager()
            {
                ::pilo::core::memory::clear_raw_ptr_linear_container(this->_loggers, false);
            }

            ::pilo::err_t logger_manager::initialize()
            {
                
                return ::pilo::err_t();
            }

        }
    }
}