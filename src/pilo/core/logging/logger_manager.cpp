#include "../memory/util.hpp"
#include "logger_manager.hpp"
#include "../process/context.hpp"
#include "./local_text_logger.hpp"

namespace pilo {
    namespace core {
        namespace logging {


            logger_manager::logger_manager()
            {

            }

            logger_manager::~logger_manager()
            {
                ::pilo::core::memory::clear_raw_ptr_linear_container(this->_loggers, false);
                _logger_by_names.clear();
            }

            ::pilo::err_t logger_manager::initialize(const std::vector<::pilo::core::config::logger> &lgs)
            {
                ::pilo::i32_t id = 0;
                for (::pilo::i32_t i = 0; i < lgs.size(); i++) {
                    const ::pilo::core::config::logger& lg = lgs.at(i);
                    ::pilo::err_t err = create_logger(id, lg, true);
                    if (err != PILO_OK) {
                        return err;
                    }
                }

                return PILO_OK;
            }

            ::pilo::err_t logger_manager::create_logger(::pilo::i32_t & id, const::pilo::core::config::logger& logger_cfg, bool b_open)
            {
                if (_logger_by_names.count(logger_cfg.name()) > 0) {
                    return ::pilo::mk_perr(PERR_EXIST);
                }
                

                ::pilo::core::logging::logger_interface* li = nullptr;
                if (logger_cfg.type() == ::pilo::core::logging::logger_type::local_spst_text) {
                    li = new ::pilo::core::logging::local_spst_text_logger(logger_cfg);
                }
                else if (logger_cfg.type() == ::pilo::core::logging::logger_type::local_spmt_text) {
                    li = new ::pilo::core::logging::local_spmt_text_logger(logger_cfg);
                }
                else if (logger_cfg.type() == ::pilo::core::logging::logger_type::local_mpst_text) {
                    li = new ::pilo::core::logging::local_mpst_text_logger(logger_cfg);
                }
                else if (logger_cfg.type() == ::pilo::core::logging::logger_type::local_mpmt_text) {
                    li = new ::pilo::core::logging::local_mpmt_text_logger(logger_cfg);
                }
                else {
                    return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
                }

                if (b_open) {
                    PILO_ERRRET(li->open());                    
                }

                id = (::pilo::i32_t)_loggers.size();
                _loggers.push_back(li);
                _logger_by_names.insert(std::pair<::std::string, logger_interface*>(logger_cfg.name(), li));

                return PILO_OK;

            }

        }
    }
}