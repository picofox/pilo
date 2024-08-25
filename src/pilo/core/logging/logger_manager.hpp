#ifndef _pilo_core_log_logger_manager_hpp_
#define _pilo_core_log_logger_manager_hpp_

#include <vector>
#include <map>
#include <unordered_map>
#include "../config/core_config.hpp"


namespace pilo {
    namespace core {
        namespace logging {

            class logger_interface;

            class logger_manager
            {
            public:
                logger_manager();
                ~logger_manager();

                ::pilo::err_t initialize(const std::vector<::pilo::core::config::logger>& lgs);
                ::pilo::err_t create_logger(::pilo::i32_t & id, const ::pilo::core::config::logger& logger_cfg, bool b_open);
                logger_interface* at(::pilo::i32_t idx) { return _loggers[idx]; }
                logger_interface* find(std::string& key) { return _logger_by_names[key]; }



            private:
                std::vector<logger_interface*>  _loggers;
                std::map<std::string, logger_interface*> _logger_by_names;

            };



        }
    }
}

#endif