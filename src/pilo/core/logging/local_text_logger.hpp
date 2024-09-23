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
#include "../memory/util.hpp"
#include "../threading/threading.hpp"
#include "../threading/exclusive_mutex_guard.hpp"
#include "../process/file_lock_guard.hpp"

namespace pilo {
    namespace core {
        namespace logging {      

            template <typename PLOCK = ::pilo::core::process::dummy_file_lock
                , typename TLOCK = ::pilo::core::threading::dummy_read_write_lock>
            class local_text_logger : public logger_interface
            {
            public:
                typedef TLOCK thread_lock_type;
                typedef PLOCK process_lock_type;

            public:
                local_text_logger(const ::pilo::core::config::logger& cfg)
                    : logger_interface(cfg)
                    , _m_last_ts(0)
                    , _m_open_ts(-1)
                    , _m_volno(0)
                    , _m_next_shift_ts(0)
                    , _m_line_count(0)
                    , _m_total_count(0)
                    , _m_total_size(0)
                    , _m_file(0,0, cfg.line_sep().c_str(), cfg.line_sep().c_str())
                    , _m_filename("")
                    
                {
                    _m_file.initialize(nullptr);
                }

                virtual ~local_text_logger()
                {
                    close();
                }

                virtual ::pilo::err_t open()
                {
                    ::pilo::core::threading::exclusive_mutex_guard<thread_lock_type> t_gurad(this->_m_file.thread_lock());
                    ::pilo::core::process::file_lock_guard<process_lock_type> p_gurad(this->_m_file.process_lock());

                    if (this->_m_file.state() == ::pilo::core::io::state_code::opened) {
                        return PILO_OK;
                    }

                    ::pilo::err_t err = _get_filename_for_opening();
                    if (err != PILO_OK) {
                        return err;
                    }
                    return _open();
                }

                virtual ::pilo::err_t close()
                {
                    ::pilo::core::threading::exclusive_mutex_guard<thread_lock_type> t_gurad(this->_m_file.thread_lock());
                    ::pilo::core::process::file_lock_guard<process_lock_type> p_gurad(this->_m_file.process_lock());
                    return _m_file.finalize();
                }

                virtual ::pilo::err_t bakup()
                {
                    ::pilo::core::threading::exclusive_mutex_guard<thread_lock_type> t_gurad(this->_m_file.thread_lock());
                    ::pilo::core::process::file_lock_guard<process_lock_type> p_gurad(this->_m_file.process_lock());
                    return _bakup(::pilo::core::datetime::timestamp_micro_system());

                }

                virtual ::pilo::err_t log(::pilo::core::logging::level lv, const char* fmt, ...)
                {                    
                    ::pilo::core::threading::exclusive_mutex_guard<thread_lock_type> t_gurad(this->_m_file.thread_lock());
                    ::pilo::core::process::file_lock_guard<process_lock_type> p_gurad(this->_m_file.process_lock());

                    if (!this->available())
                        return ::pilo::mk_perr(PERR_INC_DATA);

                    if (lv > this->_m_config.level()) {
                        return PILO_OK;
                    }

                    ::pilo::i64_t ts = ::pilo::core::datetime::timestamp_micro_system();
                    PILO_ERRRET(this->_check_file(ts));

                    ::pilo::i64_t iret = 0;
                    va_list args;
                    _m_last_ts = ts;
                    _write_header(lv);
                    va_start(args, fmt);
                    if (this->_m_config.headers().test_value(::pilo::core::logging::SrcFile) || this->_m_config.headers().test_value(::pilo::core::logging::SrcLine)) {
                        iret = this->_m_file.v_formatted_output(false, fmt, args);
                        va_end(args);
                        this->_m_total_size += iret;
                        this->_write_footer();
                    }
                    else {
                        iret = this->_m_file.v_formatted_output(true, fmt, args);
                        va_end(args);
                        this->_m_total_size += iret;
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

                inline bool available() const
                {
                    if (this->_m_file.state() != ::pilo::core::io::state_code::opened
                        || this->_m_file.error()) {
                        return false;
                    }
                    return true;
                }


            protected:
                ::pilo::err_t _open()
                {
                    ::pilo::err_t err = this->_m_file.open(this->_m_filename.c_str()
                        , ::pilo::core::io::creation_mode::open_always
                        , ::pilo::core::io::access_permission::write
                        , ::pilo::predefined_pilo_path::log
                        , ::pilo::core::io::dev_open_flags::append);
                    if (err != PILO_OK) {
                        return err;
                    }

                    this->_m_open_ts = ::pilo::core::datetime::timestamp_micro_system();
                    if (this->_m_config.splition_type() == ::pilo::core::logging::splition_type::none) {
                        this->_m_next_shift_ts = 0;
                    }
                    else if (this->_m_config.splition_type() == ::pilo::core::logging::splition_type::by_day) {
                        this->_m_next_shift_ts = ::pilo::core::datetime::datetime::calculate_next_day_initial_second(this->_m_open_ts / 1000000) * 1000000;
                    }
                    else if (this->_m_config.splition_type() == ::pilo::core::logging::splition_type::by_hour) {
                        this->_m_next_shift_ts = ::pilo::core::datetime::datetime::calculate_next_hours_initial_second(this->_m_open_ts / 1000000) * 1000000;
                    }
                    
                    this->_m_line_count = 0;
                    err = this->_m_file.size(this->_m_total_size);
                    if (err != PILO_OK) {
                        //TODO clean
                        return err;
                    }                   
                    

                    return PILO_OK;
                }

                ::pilo::err_t _get_filename_for_opening()
                {
                    if (_m_filename.size() > 0 && this->_m_config.name_suffix().empty()) {
                        return PILO_OK;
                    }
                    _m_filename.clear();
                    std::tm lt = { 0 };
                    _m_filename = this->_m_config.name();
                    char buffer[16] = { 0 };
                    if (_m_config.name_suffix().test_value(::pilo::core::logging::Pid)) {
                        ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "-%08x", PILO_CONTEXT->process_id());
                        _m_filename += buffer;
                    }
                    _m_filename += ".log";                    
                    return PILO_OK;
                }



                ::pilo::err_t _bakup(::pilo::i64_t current_ts)
                {
                    std::tm lt = { 0 };
                    std::tm open_lt = { 0 };
                    char buffer[16] = { 0 };
                    _m_bak_path.clear();
                    if (!_m_config.bak_dir().empty()) {
                        ::pilo::err_t err = _m_bak_path.set(_m_config.bak_dir().c_str(), _m_config.bak_dir().size(), 0, ::pilo::predefined_pilo_path::log);
                        if (err != PILO_OK)
                            return err;
                    }
                    

                    std::string str;
                    size_t pos = _m_filename.find(".");
                    str.assign(_m_filename.c_str(), pos);

  
                    if (_m_config.splition_type() != ::pilo::core::logging::splition_type::none) {
                        std::time_t unix_timestamp = _m_open_ts / 1000000;
#ifdef WINDOWS
                        localtime_s(&open_lt, &unix_timestamp);
#else
                        localtime_r(&unix_timestamp, &open_lt);
#endif   
                        if (_m_config.splition_type() == ::pilo::core::logging::splition_type::by_day) {
                            if (_m_config.bak_name_suffix().test_value(::pilo::core::logging::SavedDate)) {
                                ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "-%04d%02d%02d", open_lt.tm_year + 1900, open_lt.tm_mon + 1, open_lt.tm_mday);
                                str += buffer;
                            }

                        }
                        else if (_m_config.splition_type() == ::pilo::core::logging::splition_type::by_hour) {
                            if (_m_config.bak_name_suffix().test_value(::pilo::core::logging::SavedDate)) {
                                ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "-%04d%02d%02d%02d", open_lt.tm_year + 1900, open_lt.tm_mon + 1, open_lt.tm_mday, open_lt.tm_hour);
                                str += buffer;
                            }
                        }
                    }                    

                    if (_m_config.bak_name_suffix().test_value(::pilo::core::logging::SavedDate)
                        || _m_config.bak_name_suffix().test_value(::pilo::core::logging::SavedTime)) {                            

                        std::time_t unix_timestamp = current_ts / 1000000;
#ifdef WINDOWS
                        localtime_s(&lt, &unix_timestamp);
#else
                        localtime_r(&unix_timestamp, &lt);
#endif     
                        str += "-";
                        if (_m_config.bak_name_suffix().test_value(::pilo::core::logging::SavedDate)) {
                            ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%04d%02d%02d", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday);
                            str += buffer;
                        }
                        if (_m_config.bak_name_suffix().test_value(::pilo::core::logging::SavedTime)) {
                            ::pilo::core::io::string_formated_output(buffer, sizeof(buffer),  "%02d%02d%02d", lt.tm_hour, lt.tm_min + 1, lt.tm_sec);
                            str += buffer;
                        }
                    }

                    if (_m_config.size_quota() > 0 || _m_config.piece_quota() > 0) {
                        ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "-%04d", this->_m_volno);
                        str += buffer;
                        this->_m_volno++;
                    }

                    str += ".log";

                    ::pilo::err_t err = _m_bak_path.append(str.c_str(), str.size(), 0, ::pilo::predefined_pilo_path::log);
                    PILO_ERRRET(err);

                    _m_file.close();                    
                    err = ::pilo::core::io::xpf_move_fs_node(true, _m_file.path(), true, &_m_bak_path, true);
                    PILO_ERRRET(err);

                    PILO_CHKERR_RET(err, _open());
                    

                    return PILO_OK;

                }


                ::pilo::err_t _check_file(::pilo::i64_t current_ts)
                {
                    bool do_change = false;
                    if (_m_next_shift_ts > 0 && current_ts >= _m_next_shift_ts) {
                        do_change = true;
                        _m_volno = 0;
                    }
                    else {
                        if (_m_config.size_quota() > 0) {
                            if (this->_m_total_size >= _m_config.size_quota()) {
                                do_change = true;
                            }
                        }

                        if ( _m_config.piece_quota() > 0) {
                            if (this->_m_line_count >= _m_config.piece_quota()) {
                                do_change = true;
                            }
                        }

                    }

                    if (do_change) {
                        return _bakup(current_ts);
                    }

                    return PILO_OK;
                }                

                void _write_footer()
                {
                    ::pilo::i64_t hlen = 0;
                    ::pilo::i64_t tlen = 0;
                    const char* cstr = nullptr;
                    const char* cstr2 = nullptr;
                    if (this->_m_config.headers().test_value(::pilo::core::logging::SrcFile) || this->_m_config.headers().test_value(::pilo::core::logging::SrcFile)) {
                        cstr = ::pilo::core::string::rfind_char(__FILE__, -1, PMI_PATH_SEP);
                        if (cstr == nullptr)
                            cstr = __FILE__;
                        else {
                            cstr++;
                            if (*cstr == 0) {
                                cstr = __FILE__;
                            }
                            else {
                                cstr2 = ::pilo::core::string::find_char(cstr, -1, PMI_PATH_OTHER_SEP);
                                if (cstr2 != nullptr && (*(cstr2+1) != 0)) {
                                    cstr = cstr2+1;
                                }
                            }
                        }                       
                    }

                    if (this->_m_config.headers().test_value(::pilo::core::logging::SrcFile)) {                      

                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%s(%s:%d)%s", this->_m_config.field_sep().c_str(), cstr, __LINE__, this->_m_config.line_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%s(%s:%d)%s", this->_m_config.field_sep().c_str(), cstr, __LINE__, this->_m_config.line_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%s(%s:%d)%s", this->_m_config.field_sep().c_str(), cstr, __LINE__,  this->_m_config.line_sep().c_str());
                        }
                        hlen += tlen;
                    }



                    this->_m_total_size += hlen;
                }

                void _write_header(::pilo::core::logging::level lv)
                {
                    std::tm lt = { 0 };
                    ::pilo::i64_t micro_seconds = 0;
                    ::pilo::i64_t hlen = 0;
                    ::pilo::i64_t tlen = 0;
                    
                    if (this->_m_config.headers().test_value(::pilo::core::logging::Date)
                        || this->_m_config.headers().test_value(::pilo::core::logging::Time)
                        || this->_m_config.headers().test_value(::pilo::core::logging::TimeStamp)                        
                        ) {
                       
                        std::time_t unix_timestamp = _m_last_ts / 1000000;
                        micro_seconds = _m_last_ts - unix_timestamp * 1000000;
#ifdef WINDOWS
                        localtime_s(&lt, &unix_timestamp);
#else
                        localtime_r(&unix_timestamp, &lt);
#endif        
                    }
                                       
                    
                    if (this->_m_config.headers().test_value(::pilo::core::logging::Date)) {
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%04d-%02d-%02d%s", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, this->_m_config.field_sep().c_str());
                        } 
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%04d-%02d-%02d%s", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%04d-%02d-%02d%s", lt.tm_year + 1900, lt.tm_mon + 1, lt.tm_mday, this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    } 

                    if (this->_m_config.headers().test_value(::pilo::core::logging::Time)) {
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%02d:%02d:%02d.%06d%s", lt.tm_hour, lt.tm_min, lt.tm_sec, micro_seconds, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%02d:%02d:%02d.%06d%s", lt.tm_hour, lt.tm_min, lt.tm_sec, micro_seconds, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%02d:%02d:%02d.%06d%s", lt.tm_hour, lt.tm_min, lt.tm_sec, micro_seconds, this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    }


                    if (this->_m_config.headers().test_value(::pilo::core::logging::Seq)) {
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%lld%s", this->_m_line_count, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%lld%s", this->_m_line_count, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%lld%s", this->_m_line_count, this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    }

                    if (this->_m_config.headers().test_value(::pilo::core::logging::TotalSeq)) {
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%lld%s",  this->_m_total_count, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%lld%s",  this->_m_total_count, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%lld%s", this->_m_total_count, this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    }

                    if (this->_m_config.headers().test_value(::pilo::core::logging::TimeStamp)) {
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%lld%s", _m_last_ts, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%lld%s", _m_last_ts, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%lld%s", _m_last_ts, this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    }

                    if (this->_m_config.headers().test_value(::pilo::core::logging::Level)) {
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%s%s", g_level_names[(::pilo::u8_t)lv], this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%s%s", g_level_names[(::pilo::u8_t)lv], this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%s%s", g_level_names[(::pilo::u8_t)lv], this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                
                    }

                    if (this->_m_config.headers().test_value(::pilo::core::logging::Pid)) {

                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%d%s", PILO_CONTEXT->process_id(), this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%d%s", PILO_CONTEXT->process_id(), this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%d%s", PILO_CONTEXT->process_id(), this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    }
                    if (this->_m_config.headers().test_value(::pilo::core::logging::PPid)) {

                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%d%s", PILO_CONTEXT->xpf_parent_process_id(), this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%d%s", PILO_CONTEXT->xpf_parent_process_id(), this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%d%s", PILO_CONTEXT->xpf_parent_process_id(), this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    }
                    if (this->_m_config.headers().test_value(::pilo::core::logging::StdTid)) {
                        std::stringstream ss;
                        ss << std::this_thread::get_id();
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%s%s", ss.str().c_str(), this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%s%s", ss.str().c_str(), this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%s%s", ss.str().c_str(), this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    }
                    if (this->_m_config.headers().test_value(::pilo::core::logging::LocalTid)) {
                        ::pilo::os_thread_id_t tid = ::pilo::core::threading::xpf_current_thread_id();                        
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%d%s", (::pilo::i32_t) tid, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%d%s", (::pilo::i32_t)tid, this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%d%s", (::pilo::i32_t) tid, this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    }
                    if (this->_m_config.headers().test_value(::pilo::core::logging::ProcName)) {

                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevLogFile)) {
                            tlen = this->_m_file.formatted_output(false, "%s%s", PILO_CONTEXT->process_name().c_str(), this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdOut)) {
                            tlen = ::pilo::core::io::file_formatted_output(stdout, "%s%s", PILO_CONTEXT->process_name().c_str(), this->_m_config.field_sep().c_str());
                        }
                        if (this->_m_config.outputs().test_value(::pilo::core::logging::DevStdErr)) {
                            tlen = ::pilo::core::io::file_formatted_output(stderr, "%s%s", PILO_CONTEXT->process_name().c_str(), this->_m_config.field_sep().c_str());
                        }
                        hlen += tlen;
                    }


                    this->_m_total_count++;
                    this->_m_line_count++;
                    this->_m_total_size += hlen;
                }
                


            protected:
                ::pilo::i64_t                       _m_last_ts;
                ::pilo::i64_t                       _m_open_ts;
                ::pilo::i64_t                       _m_volno;
                ::pilo::i64_t                       _m_next_shift_ts;
                ::pilo::i64_t                       _m_line_count;
                ::pilo::i64_t                       _m_total_count;
                ::pilo::i64_t                       _m_total_size;                                
                ::pilo::core::io::text_file<PLOCK, TLOCK>             _m_file;
                ::pilo::core::io::path              _m_bak_path;
                ::std::string                       _m_filename;


            private:
            PMC_DISABLE_COPY(local_text_logger)
            };       

            typedef class local_text_logger<::pilo::core::process::dummy_file_lock, ::pilo::core::threading::dummy_read_write_lock>   local_spst_text_logger;
            typedef class local_text_logger<::pilo::core::process::file_lock, ::pilo::core::threading::dummy_read_write_lock>   local_mpst_text_logger;
            typedef class local_text_logger<::pilo::core::process::dummy_file_lock, ::pilo::core::threading::read_write_mutex>   local_spmt_text_logger;
            typedef class local_text_logger<::pilo::core::process::file_lock, ::pilo::core::threading::read_write_mutex>   local_mpmt_text_logger;
        
        }
    }

}


#endif //! _pilo_core_log_text_logger_hpp_