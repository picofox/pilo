#ifndef _pilo_io_hpp_
#define _pilo_io_hpp_

#include "../../pilo.hpp"


namespace pilo
{
    namespace core
    {
        namespace io
        {
            enum class creation_mode : ::pilo::u8_t
            {
                create_always = 0,
                open_existing = 1,
                create_neo = 2,
                open_always = 3,
                open_trunc = 4
            };

            enum class access_permission : ::pilo::u8_t
            {
                none = 0,
                exec = 1,
                write = 2,
                write_exec = 3,
                read = 4,
                read_exec = 5,
                read_write = 6,
                all = 7,
            };

            enum class state_code : ::pilo::u8_t
            {
                uninitialized = 0,
                initialized = 1,
                opened = 2,
                closed = 3,
            };

            enum class seek_whence
            {
                current = -1,
                begin = 0,
                end = 1,
            };

            enum class dev_open_flags : ::pilo::u8_t
            {
                none = 0x00,
                nonblocking = 0x01,
                temp = 0x02,
                write_through = 0x04,
                append = 0x08,
            };

            enum class flush_level
            {
                all = 0,
                partial = 1,
            };

            ::pilo::os_file_handle_t xpf_open_file(const char* path_str, creation_mode cm, access_permission perm, dev_open_flags f);
            void xpf_close_file(::pilo::os_file_handle_t* fd);
            ::pilo::err_t xpf_seek_file(::pilo::os_file_handle_t fd, seek_whence whence, ::pilo::i64_t off);
            ::pilo::err_t xpf_tell_file(::pilo::os_file_handle_t fd, ::pilo::i64_t & off);
            ::pilo::err_t xpf_flush_file(::pilo::os_file_handle_t fd, flush_level lv = flush_level::all);
            ::pilo::err_t xpf_write_file(::pilo::os_file_handle_t fd, const char* buffer, ::pilo::i64_t wbs, ::pilo::i64_t* n_written);
            ::pilo::err_t xpf_read_file(::pilo::os_file_handle_t fd, char* buffer, ::pilo::i64_t rbs, ::pilo::i64_t* n_read);
            ::pilo::err_t xpf_set_size(::pilo::os_file_handle_t fd, ::pilo::i64_t sz);

            ::pilo::err_t xpf_get_file_size(::pilo::os_file_handle_t fd, ::pilo::i64_t &sz);


        } // end of name space
    }
}



#endif