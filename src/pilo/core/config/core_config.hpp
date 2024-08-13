#ifndef _pilo_core_config_config_core_config_h_
#define _pilo_core_config_config_core_config_h_

#include "./json_configuator.hpp"
#include "./configuation_interface.hpp"
#include "../string/string_operation.hpp"
#include "../logging/logger_def.hpp"
#include <vector>




namespace pilo {
    namespace core {
        namespace config {

            class core_config : public configuation_interface {
            public:
                //-----------------------------
                class logger {
                public:
                    friend class core_config;
                public:
                    logger() : _type(::pilo::core::logging::logger_type::local_spst_text)
                        , _level(::pilo::core::logging::level::debug)
                        , _splition_type(::pilo::core::logging::splition_type::by_day)
                        , _outputs(::pilo::core::logging::DevLogFile)
                        , _name_suffix(0)
                        , _headers(::pilo::core::logging::DefaultHeaders)
                        , _split_every(0)
                        , _flags(::pilo::core::logging::DefaultFlags)
                        , _bak_name_suffix(0)
                        , _size_quota(0)
                        , _piece_quota(0)
                        , _name("")
                        , _bak_dir("")
                        , _line_sep("\n")
                        , _field_sep(", ")
                    {
                    }


                public:
                    inline void set_type(::pilo::core::logging::logger_type t)
                    {
                        _type = t;
                    }
                    inline void set_name(const char* n)
                    {
                        _name = n;
                    }
                    inline ::pilo::err_t set_bak_dir(const char* dn, ::pilo::pathlen_t len, ::pilo::predefined_pilo_dir predir)
                    {
                        ::pilo::core::io::path p(dn, len, predir);
                        if (p.invalid()) {
                            return ::pilo::mk_err(PERR_INVALID_PATH);
                        }
                        _bak_dir.assign(p.fullpath(), p.length());

                        return PILO_OK;
                    }   
                    inline ::pilo::err_t set_bak_dir(const char* dn)
                    {
                        _bak_dir = dn;
                        return PILO_OK;
                    }
                    inline void set_bak_name_suffix(const char* n)
                    {
                        _name = n;
                    }
                    inline void set_size_quota(::pilo::i64_t q)
                    {
                        _size_quota = q;
                    }
                    inline void set_bak_name_suffix(::pilo::u32_t v)
                    {
                        _bak_name_suffix = v;
                    }
                    inline ::pilo::core::logging::logger_type type() const
                    {
                        return _type;
                    }
                    inline const char* type_name() const
                    {
                        return ::pilo::core::logging::g_logger_type_names[(::pilo::u8_t)_type];
                    }
                    inline const char* level_name() const
                    {
                        return ::pilo::core::logging::g_level_names[(::pilo::u8_t)_level];
                    }
                    inline ::pilo::core::logging::level level() const
                    {
                        return _level;
                    }
                    inline void set_level(::pilo::core::logging::level lv)
                    {
                        _level = lv;
                    }
                    inline const char* splition_type_name()
                    {
                        return ::pilo::core::logging::g_splition_type_names[(::pilo::u8_t)_splition_type];
                    }
                    inline ::pilo::core::logging::splition_type  splition_type() const
                    {
                        return _splition_type;
                    }
                    inline char* get_outputs_devs(char* buffer, ::pilo::i32_t capacity) const
                    {
                        if (buffer != nullptr)
                            buffer[0] = 0;
                        return ::pilo::core::string::extract_flags_to_strlist(buffer, capacity, this->_outputs.data(), ",", 1
                            , ::pilo::core::logging::g_output_dev_names, PMF_COUNT_OF(::pilo::core::logging::g_output_dev_names));
                    }
                    inline char* get_predef_elements(::pilo::u32_t v, char* buffer, ::pilo::i32_t capacity) const
                    {
                        if (buffer != nullptr)
                            buffer[0] = 0;
                        return ::pilo::core::string::extract_flags_to_strlist(buffer, capacity, v, ",", 1
                            , ::pilo::core::logging::g_predef_elment_names, PMF_COUNT_OF(::pilo::core::logging::g_predef_elment_names));
                    }
                    inline ::pilo::bit_flag<::pilo::u8_t> outputs() const
                    {
                        return _outputs;
                    }
                    inline const ::pilo::bit_flag<::pilo::u32_t> name_suffix() const
                    {
                        return _name_suffix;
                    }
                    inline ::pilo::bit_flag<::pilo::u32_t> headers() const
                    {
                        return _headers;
                    }

                    inline ::pilo::i32_t split_every() const
                    {
                        return _split_every;
                    }
                    inline ::pilo::i64_t size_quota() const
                    {
                        return _size_quota;
                    }
                    inline ::pilo::i64_t piece_quota() const
                    {
                        return _piece_quota;
                    }
                    inline const std::string& name() const
                    {
                        return _name;
                    }
                    inline const std::string& bak_dir() const
                    {
                        return _bak_dir;
                    }
                    inline ::pilo::u32_t flags() const
                    {
                        return _flags.data();
                    }
                    inline const ::pilo::bit_flag<::pilo::u32_t> bak_name_suffix() const
                    {
                        return _bak_name_suffix;
                    }
                    inline const std::string& line_sep() const
                    {
                        return _line_sep;
                    }
                    inline const std::string& field_sep() const
                    {
                        return _field_sep;
                    }
                    inline void set_field_sep(const char * fld_sep)
                    {
                        _field_sep = fld_sep;
                    }
                    inline void set_defualt()
                    {
                        _type = ::pilo::core::logging::logger_type::local_spst_text;
                        _level = ::pilo::core::logging::level::debug;
                        _splition_type = ::pilo::core::logging::splition_type::by_day;
                        _outputs = ::pilo::core::logging::DevLogFile;
                        _name_suffix = 0;
                        _headers = ::pilo::core::logging::DefaultHeaders;
                        _split_every = 0;
                        _flags = ::pilo::core::logging::DefaultFlags;
                        _bak_name_suffix = 0;
                        _size_quota = 0;
                        _piece_quota = 0;
                        _name = "";
                        _bak_dir = "";
                        _line_sep = "\n";
                        _field_sep = ", ";
                    }

                private:                    
                    ::pilo::core::logging::logger_type _type;
                    ::pilo::core::logging::level _level;
                    ::pilo::core::logging::splition_type _splition_type;
                    ::pilo::bit_flag<::pilo::u8_t> _outputs;                    
                    ::pilo::bit_flag<::pilo::u32_t> _name_suffix;

                    ::pilo::bit_flag<::pilo::u32_t> _headers;
                    ::pilo::i32_t _split_every;

                    ::pilo::bit_flag<::pilo::u32_t> _flags;
                    ::pilo::bit_flag<::pilo::u32_t> _bak_name_suffix;

                    ::pilo::i64_t _size_quota;
                    ::pilo::i64_t _piece_quota;

                    std::string _name;
                    std::string _bak_dir;
                    std::string _line_sep;
                    std::string _field_sep;
                    

                };
                //-------------------------------

            public:
                core_config();
                virtual ~core_config()
                {
                    clear();
                }
                
                virtual ::pilo::err_t load();
                virtual ::pilo::err_t save() override;
                virtual void core_config::clear(bool purge = true);
                virtual ::pilo::err_t set_default() override;
                virtual ::pilo::err_t save_to_configurator(configuator_interface* configuator);
                virtual ::pilo::err_t load_from_configurator(configuator_interface* configuator);
                virtual ::pilo::err_t load_or_save_default()
                {
                    ::pilo::err_t err = load();
                    if (err == PILO_OK) 
                        return PILO_OK;

                    ::pilo::err_t perr = ::pilo::ex_perr(err);
                    if (perr != PERR_IO_OPEN_FAIL) {
                        return err;
                    }

                    err = this->set_default();
                    if (err != PILO_OK)
                        return err;

                    return this->save();                    
                }

                virtual const ::pilo::core::io::path& file_path() const
                {
                    return _file_path;
                }


            public:
                const logger* logger_at(::pilo::u32_t idx) const
                {
                    if (idx < _loggers.size())
                        return &_loggers[idx];
                    return nullptr;
                }

                const std::string& cwd() const
                {
                    return _cwd;
                }

                const ::std::vector<logger>& loggers() const
                {
                    return _loggers;
                }
                
            
            public:
                inline void set_cwd(const char* cwd_cstr) { _cwd = cwd_cstr; }


            private:
                ::pilo::core::io::path  _file_path;
                
                ::std::string           _cwd;
                ::std::vector<logger> _loggers;


                

};





        }
    }
}

#endif // !_pilo_core_config_config_core_config_h_
