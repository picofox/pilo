#ifndef _pilo_core_config_logger_config_h_
#define _pilo_core_config_logger_config_h_

#include "../logging/logger_def.hpp"
#include "../io/path.hpp"
#include "../ml/json_tlv_driver.hpp"
#include "./configuation_interface.hpp"

namespace pilo {
    namespace core {
        namespace config {


            class logger : public configuation_interface {
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

                ::pilo::err_t save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface * driver);
                ::pilo::err_t load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator);

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
                inline char* get_flags(char* buffer, ::pilo::i32_t capacity) const
                {
                    if (buffer != nullptr)
                        buffer[0] = 0;
                    return ::pilo::core::string::extract_flags_to_strlist(buffer, capacity, this->_flags.data(), ",", 1
                        , ::pilo::core::logging::g_flags, PMF_COUNT_OF(::pilo::core::logging::g_flags));

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
                inline void set_field_sep(const char* fld_sep)
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
                    _split_every = 1;
                    _flags = ::pilo::core::logging::DefaultFlags;
                    _bak_name_suffix = 0;
                    _size_quota = BGIGA(1);
                    _piece_quota = 0;
                    _name = "";
                    _bak_dir = "baks";
                    _line_sep = "\n";
                    _field_sep = SP_PMS_LOG_FILE_DFL_FLD_SEP;
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



                // Inherited via configuation_interface
                ::pilo::err_t load() override;

                ::pilo::err_t save() override;

                ::pilo::err_t set_default() override;

                void clear(bool purge) override;

                const ::pilo::core::io::path* file_path() const override;
                bool invalid() const;

            };
        }
    }
}



#endif