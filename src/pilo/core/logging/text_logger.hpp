#ifndef _pilo_core_log_text_logger_hpp_
#define _pilo_core_log_text_logger_hpp_

#include <ctime>
#include "../string/string_operation.hpp"
#include "../io/text_file.hpp"
#include "../process/file_lock.hpp"
#include "../threading/read_write_mutex.hpp"
#include "../datetime/datetime.hpp"
#include "../datetime/timestamp.hpp"
#include "./logger_interface.hpp"

namespace pilo {
    namespace core {
        namespace logging {      

            template <typename PLOCK = ::pilo::core::process::dummy_file_lock
                , typename TLOCK = ::pilo::core::threading::dummy_read_write_lock>
            class text_logger : public logger_interface
            {
            public:
                static const char* s_level_to_name(::pilo::u32_t level)
                {
                    static const char* s_level_names[] = { "[none]", "[fatal]", "[error]", "[warn]", "[info]", "[debug]" };
                    if (level > (::pilo::u32_t) ::pilo::core::logging::level::debug) {
                        return "[na]";
                    }
                    return s_level_names[level];
                }
                

            public:
                text_logger(::pilo::core::logging::level lv = ::pilo::core::logging::level::debug
                    , ::pilo::u8_t outdevs = (::pilo::core::logging::DevLogFile | ::pilo::core::logging::DevStdOut)
                    , ::pilo::u32_t hdrs = 0
                    , ::pilo::u32_t fn_suffix = 0
                    , const char* logname = PILO_CONTEXT->process_basename()
                    , const char* line_seperator = SP_PMS_TEXT_FILE_DFL_LINE_WSEP)

                    : _m_level(lv)
                    , _m_outputs(outdevs)
                    , _m_volno(-1)
                    , _m_ts(0)
                    , _m_next_shift_ts(0)
                    , _m_line_count(0)
                    , _m_total_size(0)
                    , _m_headers(hdrs)
                    , _m_filename_suffix(fn_suffix)
                    , _m_logname(logname)
                    , _m_file(0,0, SP_PMS_TEXT_FILE_DFL_LINE_RSEP, line_seperator)
                    
                {
                    
                }

                virtual ~text_logger()
                {
                    _m_file.finalize();
                }

                ::pilo::err_t open()
                { 
                    if (this->_m_file.state() == ::pilo::core::io::state_code::opened) {
                        return PILO_OK;
                    }
                    char name_buf[256] = { 0 };
                    ::pilo::err_t err = _calc_log_filename(name_buf, sizeof(name_buf));
                    if (err != PILO_OK)
                        return err;

                    err = this->_m_file.open(name_buf
                        , ::pilo::core::io::creation_mode::open_always
                        , ::pilo::core::io::access_permission::write
                        , ::pilo::predefined_pilo_dir_enum::log
                        , ::pilo::core::io::dev_open_flags::append);
                    if (err != PILO_OK) {
                        return err;
                    }


                    return PILO_OK;
                }

                void add_filename_suffix(::pilo::u32_t e)
                {
                    this->_m_filename_suffix.mark_value(e);
                }

                void set_filename_suffix(::pilo::u32_t s)
                {
                    this->_m_filename_suffix.set(s);
                }

                void set_level(::pilo::core::logging::level lv)
                {
                    this->_m_level = lv;
                }

                inline ::pilo::err_t process_lock_shared(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_file.process_lock_shared(offset, length);
                }

                inline ::pilo::err_t process_lock(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_file.process_lock(offset, length);
                }

                inline ::pilo::err_t process_try_lock(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_file.process_try_lock(offset, length);
                }

                inline ::pilo::err_t process_try_lock_shared(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_file.process_try_lock_shared(offset, length);
                }

                inline ::pilo::err_t process_unlock(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_file.process_unlock(offset, length);
                }

                inline ::pilo::err_t process_unlock()
                {
                    return this->_m_file.process_unlock();
                }

                inline void thread_lock_shared()
                {
                    return this->_m_file.lock_shared();
                }

                inline void lock()
                {
                    return this->_m_file.lock();
                }

                inline bool try_lock()
                {
                    return this->_m_file.try_lock();
                }

                inline bool try_lock_shared()
                {
                    return this->_m_file.try_lock_shared();
                }

                inline void unlock()
                {
                    return this->_m_file.unlock();
                }

                inline void unlock_shared()
                {
                    return this->_m_file.unlock_shared();
                }




            protected:
                ::pilo::err_t _calc_log_filename(char* buffer, ::pilo::i64_t capa)
                {
                    if (this->_m_filename_suffix.empty()) {
                        ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%s.log", this->_m_logname.c_str());
                        return PILO_OK;
                    }
                    ::pilo::i64_t remain_capa = capa - 5;
                    ::pilo::i64_t idx = 0;
                    if (remain_capa < (::pilo::i64_t) this->_m_logname.size())
                        return ::pilo::mk_perr(PERR_LEN_TOO_LARGE);
                    ::pilo::core::string::n_copyz(buffer, remain_capa, this->_m_logname.c_str(), this->_m_logname.size());
                    idx += this->_m_logname.size();
                    remain_capa -= this->_m_logname.size();
                    std::tm lt = { 0 };
                    this->_m_ts = ::pilo::core::datetime::timestamp_micro_system();
                    std::time_t unix_timestamp = this->_m_ts / 1000000;
#ifdef WINDOWS
                    localtime_s(&lt, &unix_timestamp);
#else
                    localtime_r(&unix_timestamp, &lt);
#endif    

                    if (remain_capa < 9)
                        return ::pilo::mk_perr(PERR_LEN_TOO_LARGE);
                    if (this->_m_filename_suffix.test_value(Date)) {
                        ::pilo::core::io::string_formated_output(buffer + idx, remain_capa, "_%04d%02d%02d", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday);
                        idx += 9;
                        remain_capa -= 9;
                    }

                    if (this->_m_filename_suffix.test_value(Pid)) {
                        ::pilo::core::io::string_formated_output(buffer + idx, remain_capa, "_%d", PILO_CONTEXT->process_id());
                    }

                    ::pilo::core::string::n_concatenate_inplace(buffer, capa, ".log", 4);

                    return PILO_OK;                    
                }


                void _write_header()
                {
                    std::tm lt = { 0 };
                    if (this->_m_headers.test_value(predef_item::timestamp)
                        || this->_m_headers.test_value(predef_item::date)
                        || this->_m_headers.test_value(predef_item::time)
                        || this->_m_headers.test_value(predef_item::micro)
                        ) {
                       
                        this->_m_ts = ::pilo::core::datetime::timestamp_micro_system();
                        std::time_t unix_timestamp = this->_m_ts / 1000000;
                        ::pilo::i64_t micro_seconds = this->_m_ts - unix_timestamp * 1000000;
       
#ifdef WINDOWS
                        localtime_s(&lt, &unix_timestamp);
#else
                        localtime_r(&unix_timestamp, &lt);
#endif        
                    }

                    if (this->_m_headers.test_value(predef_item::date)) {
                        if (_m_outputs.test_value(::pilo::core::logging::output_dev::log_file)) {
                            this->_m_file.formatted_output(false, "%04d-%02d-%02d\t", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday);
                        } 
                        if (_m_outputs.test_value(::pilo::core::logging::output_dev::std_output)) {
                            ::pilo::core::io::file_formatted_output(stdout, "%04d-%02d-%02d\t", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday);
                        }
                        if (_m_outputs.test_value(::pilo::core::logging::output_dev::std_err_output)) {
                            ::pilo::core::io::file_formatted_output(stderr, "%04d-%02d-%02d\t", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday);
                        }
                    } 


                }
                


            protected:

                ::pilo::core::logging::level        _m_level;
                ::pilo::bit_flag<::pilo::u8_t>     _m_outputs;
                ::pilo::i32_t                       _m_volno;
                ::pilo::i64_t                       _m_ts;
                ::pilo::i64_t                       _m_next_shift_ts;
                ::pilo::i64_t                       _m_line_count;
                ::pilo::i64_t                       _m_total_size;
                ::pilo::bit_flag<::pilo::u32_t>     _m_headers;
                ::pilo::bit_flag<::pilo::u32_t>     _m_filename_suffix;
                ::std::string                       _m_logname;                
                ::pilo::core::io::text_file<PLOCK, TLOCK>             _m_file;
                ::pilo::core::io::path              _m_bak_path;



            private:
            PMC_DISABLE_COPY(text_logger)
            };       

            typedef class text_logger<::pilo::core::process::dummy_file_lock, ::pilo::core::threading::dummy_read_write_lock>   spst_text_logger;
            typedef class text_logger<::pilo::core::process::file_lock, ::pilo::core::threading::dummy_read_write_lock>   mpst_text_logger;
            typedef class text_logger<::pilo::core::process::dummy_file_lock, ::pilo::core::threading::read_write_mutex>   spmt_logger;
            typedef class text_logger<::pilo::core::process::file_lock, ::pilo::core::threading::read_write_mutex>   mpmt_text_logger;
        
        }
    }

}


#endif //! _pilo_core_log_text_logger_hpp_