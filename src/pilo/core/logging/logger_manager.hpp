#ifndef _pilo_core_log_logger_manager_hpp_
#define _pilo_core_log_logger_manager_hpp_

#include "../../pilo.hpp"
#include "./logger_interface.hpp"
#include "../config/core_config.hpp"
#include <vector>

namespace pilo {
    namespace core {
        namespace logging {

            class logger_manager
            {
            public:
                logger_manager();
                ~logger_manager();

                ::pilo::err_t initialize();


            private:
                std::vector<logger_interface*>  _loggers;

            };



        }
    }
}

#endif