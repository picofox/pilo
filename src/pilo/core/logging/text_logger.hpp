#ifndef _pilo_core_log_text_logger_hpp_
#define _pilo_core_log_text_logger_hpp_

#include "../string/string_operation.hpp"
#include "../io/text_file.hpp"

namespace pilo {
    namespace core {
        namespace logging {

            enum class predef_item : ::pilo::u32_t
            {
                date        = 0x00000001,
                time        = 0x00000002,
                nano        = 0x00000004,
                mili        = 0x00000008,
                micro       = 0x00000010,
                timestamp   = 0x00000020,
                level       = 0x00000040,
                pid         = 0x00000080,
                std_tid     = 0x00000100,
                local_tid   = 0x00000200,
                file        = 0x00000400,
                line        = 0x00000800,
                procname    = 0x00001000,
            };

            enum class level : ::pilo::u32_t
            {
                none    = 0,
                fatal   = 1,
                error   = 2,
                warn    = 3,
                info    = 4,
                debug   = 5,
            };


            template <typename PLOCK = ::pilo::core::process::dummy_file_lock
                , typename TLOCK = ::pilo::core::threading::dummy_read_write_lock>
            class text_logger
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
                    , ::pilo::u32_t hdrs = 0
                    , ::pilo::u32_t fn_suffix = 0
                    , const char* logname = PILO_CONTEXT->process_name()
                    , const char* line_seperator = SP_PMS_TEXT_FILE_DFL_LINE_WSEP)

                    : _m_level(lv)
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

                ~text_logger()
                {
                    _m_file.finalize();
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

                


            protected:

                ::pilo::core::logging::level        _m_level;
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
        
        }
    }

}


#endif //! _pilo_core_log_text_logger_hpp_