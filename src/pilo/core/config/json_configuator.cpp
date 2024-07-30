#include "json_configuator.hpp"
#include "../../external/rapidjson/document.h"


namespace pilo {
    namespace core {
        namespace config {

            json_configuator::json_configuator(const::pilo::core::io::path* path_ptr)
            {
                if (path_ptr != nullptr) {
                    _m_file_ptr = new ::pilo::core::io::file<>();
                    _m_file_ptr->set_path(path_ptr);
                }
                else {
                    _m_file_ptr = nullptr;
                }
            }

            ::pilo::err_t core::config::json_configuator::load()
            {
                ::pilo::err_t err = this->_m_file_ptr->open(::pilo::core::io::creation_mode::open_existing
                    , ::pilo::core::io::access_permission::read
                    , ::pilo::core::io::dev_open_flags::none);
                if (err != PILO_OK) {
                    return err;
                }

                char buffer[1024] = { 0 };
                ::pilo::i64_t data_len = 0;
                const char* data = this->_m_file_ptr->read_all(buffer, sizeof(buffer), &data_len, &err);
                if (err != PILO_OK) {
                    return err;
                }
                if (data == nullptr) {
                    return ::pilo::mk_perr(PERR_VAL_EMPTY);
                }                

                return load(data, data_len);
            }

            ::pilo::err_t core::config::json_configuator::save()
            {
                return ::pilo::err_t();
            }

            void json_configuator::set_file(const::pilo::core::io::path* path_ptr)
            {
                if (_m_file_ptr != nullptr) {
                    return;
                }

                if (path_ptr != nullptr) {
                    _m_file_ptr = new ::pilo::core::io::file<>();
                    _m_file_ptr->set_path(path_ptr);
                }
                else {
                    _m_file_ptr = nullptr;
                }
            }

            ::pilo::err_t json_configuator::load(const char* data, ::pilo::i64_t len)
            {
                if (data == nullptr) {
                    return 0;
                }

                if (len < 1) {
                    return -1;
                }



                return PILO_OK;
            }


        }
    }
}