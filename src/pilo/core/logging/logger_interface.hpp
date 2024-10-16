#ifndef _pilo_core_log_logger_interface_hpp_
#define _pilo_core_log_logger_interface_hpp_

#include "../config/core_config.hpp"
#include "./logger_def.hpp"


namespace pilo {
    namespace core {
        namespace logging {


            class logger_interface
            {
            public:
                logger_interface(const ::pilo::core::config::logger& logger_config) : _m_config(logger_config) {};
                virtual ~logger_interface() {};

            public:
                virtual ::pilo::err_t open() = 0;
                virtual ::pilo::err_t close() = 0;
                virtual ::pilo::err_t bakup() = 0;               
                virtual ::pilo::err_t log(::pilo::core::logging::level lv, const char* fmt, ...) = 0;
                virtual ::pilo::err_t log_debug(const char* filename, ::pilo::i32_t lineno, ::pilo::core::logging::level lv, const char* fmt, ...) = 0;
                virtual ::pilo::err_t log_raw(const char* cstr, ::pilo::i64_t len) = 0;
                virtual void set_level(::pilo::core::logging::level lv) { _m_config.set_level(lv); }

            protected:
                ::pilo::core::config::logger   _m_config;

            };

        }
    }
}





#endif