#ifndef _io_device_h_
#define _io_device_h_

#include "../../pilo.hpp"
#include "../../tlv.hpp"
#include "path.hpp"
#include "../memory/byte_buffer_interface.hpp"

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


            class device_interface
            {
            public:
                device_interface() :_m_state_and_error(0)
                    , _m_flags(dev_open_flags::none)
                    , _m_creation_mode(creation_mode::create_always)
                    , _m_access_perm(access_permission::none)
                {

                }
                virtual ~device_interface()
                {
                    _m_state_and_error = 0;
                    _m_flags = dev_open_flags::none;
                }

                device_interface(device_interface&& other) noexcept
                {
                    _m_state_and_error = other._m_state_and_error;
                    _m_flags = other._m_flags;
                    _m_creation_mode = other._m_creation_mode;
                    _m_access_perm = other._m_access_perm;
                    other._m_state_and_error = 0;
                    other._m_flags = dev_open_flags::none;
                    other._m_access_perm = access_permission::none;
                    other._m_creation_mode = creation_mode::create_always;
                }
                device_interface& operator =(device_interface&& other) noexcept
                {
                    if (this != &other)
                    {
                        _m_state_and_error = other._m_state_and_error;
                        _m_flags = other._m_flags;
                        _m_creation_mode = other._m_creation_mode;
                        _m_access_perm = other._m_access_perm;
                        other._m_state_and_error = 0;
                        other._m_flags = dev_open_flags::none;
                        other._m_access_perm = access_permission::none;
                        other._m_creation_mode = creation_mode::create_always;
                    }
                    return *this;
                }

            public:
                virtual ::pilo::err_t initialize(::pilo::tlv * param) = 0; 
                virtual ::pilo::err_t open(const char* path_str, creation_mode cm, access_permission perm, predefined_pilo_dir_enum prefix, dev_open_flags f) = 0;
                virtual ::pilo::err_t open(const ::pilo::core::io::path* p, creation_mode cm, access_permission perm, dev_open_flags f) = 0;
                virtual ::pilo::err_t open(const char* path_str, creation_mode cm, access_permission perm, dev_open_flags f) = 0;
                virtual ::pilo::err_t read(char * buffer, ::pilo::i64_t rbs, ::pilo::i64_t * n_read) = 0;
                virtual ::pilo::err_t read(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t rbs, ::pilo::i64_t* n_read) = 0;
                virtual ::pilo::err_t write(const char* buffer, ::pilo::i64_t wbs, ::pilo::i64_t * n_written) = 0;
                virtual ::pilo::err_t write(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t wbs, ::pilo::i64_t* n_written) = 0;
                virtual ::pilo::err_t seek(seek_whence whence, ::pilo::i64_t off) = 0;
                virtual ::pilo::err_t tell(::pilo::i64_t &off) const = 0;
                virtual ::pilo::err_t control(::pilo::i32_t cmd, ::pilo::tlv* param) = 0;
                virtual ::pilo::err_t sync(::pilo::i32_t mode) = 0;
                virtual ::pilo::err_t close() = 0;
                virtual ::pilo::err_t finalize() = 0;
                virtual ::pilo::err_t exist() const = 0;
                virtual ::pilo::err_t remove() = 0;

            public:   
                virtual void set_flag(dev_open_flags f)
                {
                    _m_flags = (dev_open_flags) ((::pilo::u8_t) _m_flags | (::pilo::u8_t) f);
                }

                virtual void clear_flag(dev_open_flags f)
                {
                    _m_flags = (dev_open_flags)((::pilo::u8_t)_m_flags & ~(::pilo::u8_t)f);
                }

                virtual bool test_flag(dev_open_flags f) const
                {
                    return ((::pilo::u8_t)_m_flags & (::pilo::u8_t) f);
                }            

                virtual bool append() const
                {
                    return test_flag(dev_open_flags::append);
                }

                virtual bool nonblocking() const
                {
                    return test_flag(dev_open_flags::nonblocking);
                }

                virtual bool temp() const
                {
                    return test_flag(dev_open_flags::temp);
                }

                virtual bool write_through() const
                {
                    return test_flag(dev_open_flags::write_through);
                }


                virtual void set_state(state_code st)
                {
                    _m_state_and_error &= ~0x7F;
                    _m_state_and_error = _m_state_and_error | (::pilo::u8_t) st;
                }

                virtual state_code state() const
                {
                    return (state_code)(_m_state_and_error & 0x7F);
                }

                virtual void set_error()
                {
                    _m_state_and_error = _m_state_and_error | (::pilo::u8_t)0x80;
                }

                virtual void clear_error()
                {
                    _m_state_and_error = _m_state_and_error & (::pilo::u8_t)~0x80;
                }

                virtual bool error() const
                {
                    return _m_state_and_error & 0x80;
                }

            protected:
                ::pilo::u8_t _m_state_and_error;
                dev_open_flags _m_flags;
                creation_mode _m_creation_mode;
                access_permission _m_access_perm;                
                
            };
        }
    }
}

#endif