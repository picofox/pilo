#ifndef _pilo_core_config_config_core_config_h_
#define _pilo_core_config_config_core_config_h_

#include "./json_configuator.hpp"
#include "../logging/logger_interface.hpp"
#include <vector>


namespace pilo {
    namespace core {
        namespace config {

            class core_config {

                //-----------------------------
                class logger {
                public:
                    logger() : _type(::pilo::core::logging::logger_type::local_spst_text)                        
                        , _level(::pilo::core::logging::level::debug)
                        , _splition_type(::pilo::core::logging::splition_type::by_day)
                        , _outputs(::pilo::core::logging::DevLogFile)
                        , _name_suffix(0)
                        , _headers(::pilo::core::logging::DefaultHeaders)
                        , _split_every(0)
                        , _flags(::pilo::core::logging::DefaultFlags)
                        ,_size_quota(0)
                        ,_piece_quata(0)
                        ,_name("")
                        ,_bak_dir("")
                        , _bak_name_suffix("")
                    {
                    }


                private:                    
                    ::pilo::core::logging::logger_type _type;
                    ::pilo::core::logging::level _level;
                    ::pilo::core::logging::splition_type _splition_type;
                    ::pilo::bit_flag<::pilo::u8_t> _outputs;                    
                    ::pilo::bit_flag<::pilo::u32_t> _name_suffix;
                    ::pilo::bit_flag<::pilo::u32_t> _headers;
                    ::pilo::i32_t _split_every;
                    ::pilo::bit_flag<::pilo::u64_t> _flags;
                    ::pilo::i64_t _size_quota;
                    ::pilo::i64_t _piece_quata;
                    std::string _name;
                    std::string _bak_dir;
                    std::string _bak_name_suffix;
                };
                //-------------------------------

            public:
                core_config(configurator_type ct = configurator_type::json) 
                    :_configuator(nullptr)
                { 
                    if (ct == configurator_type::json) {
                        _configuator = new ::pilo::core::config::json_configuator();
                    }
                }
                ~core_config()
                {
                    delete this->_configuator;
                    this->_configuator = nullptr;
                }

                ::pilo::err_t load();


            private:
                configuator_interface* _configuator;
                ::std::vector<logger> _loggers;

            };





        }
    }
}

#endif // !_pilo_core_config_config_core_config_h_
