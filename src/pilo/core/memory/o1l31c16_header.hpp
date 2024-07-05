#pragma once

#include "../../pilo.hpp"
#include "serializable_header_interface.hpp"
#include <sstream>

#define PMO_O1l31C16_MSG_MAX_BODY_LENGTH    (0x7FFFFFFF)

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            class o1l31c16_header : public serializable_header_interface
            {
            public:
                o1l31c16_header() : _m_o_len(0), _m_command(0) {}

                static ::pilo::err_t deserialise(o1l31c16_header & header, byte_buffer_interface* byte_buffer)
                {
                    if (byte_buffer->read_available() >= 6)
                    {
                        if (byte_buffer->peek_uint32(0, header._m_o_len) != PILO_OK)
                        {
                            return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                        }
                        if (byte_buffer->peek_uint16(4, header._m_command) != PILO_OK)
                        {
                            return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                        }

                        if (byte_buffer->read_available() >= header.body_length() + header.header_length())
                        {
                            return PILO_OK;
                        }

                    }
                    return ::pilo::mk_perr(PERR_RETRY);
                }

            public:
                virtual ::pilo::err_t begin_serializing(byte_buffer_interface * byte_buffer)
                {
                    if (byte_buffer->write_uint32_pc(0) != PILO_OK)
                    {
                        return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                    }
                    if (byte_buffer->write_uint16(_m_command))
                    {
                        return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                    }

                    
                    return PILO_OK;
                }

                virtual ::pilo::err_t end_serializing(byte_buffer_interface* byte_buffer, ::pilo::i64_t header_pos, ::pilo::i64_t total_len)
                {
                    PMC_UNUSED(header_pos);
                    this->set_body_length(total_len);
                    return byte_buffer->set_pc_uint32(this->_m_o_len);
                }

                virtual ::pilo::err_t begin_deserializing(byte_buffer_interface * byte_buffer)
                {
                    if (this->body_length() + this->header_length() <= byte_buffer->read_available())
                    {
                        return byte_buffer->readable_seek(::pilo::seek_whence_enum::current, this->header_length(), true, nullptr);
                    }

                    return ::pilo::mk_perr(PERR_RETRY);
                }

                virtual ::pilo::err_t end_deserializing(byte_buffer_interface * byte_buffer, ::pilo::i64_t actual_read_bytes)
                {
                    PMC_UNUSED(byte_buffer);
                    if (actual_read_bytes != body_length())
                    {
                        return ::pilo::mk_perr(PERR_INV_LEN);
                    }
                    return PILO_OK;
                }

                virtual ::pilo::i64_t header_length() const
                {
                    return 6;
                }

                virtual ::pilo::i64_t body_length() const
                {
                    return (::pilo::i64_t) (_m_o_len & 0x7FFFFFFF);
                }

                virtual ::pilo::err_t set_body_length(::pilo::i64_t body_len)
                {
                    PMC_ASSERT(body_len <= 0x7FFFFFFF);
                    if (body_len < PMO_O1l31C16_MSG_MAX_BODY_LENGTH)
                    {
                        this->_m_o_len = ((::pilo::u32_t)this->gourp_type()) | (((::pilo::u32_t)body_len) & 0x7FFFFFFF);
                        return PILO_OK;
                    }
                    return ::pilo::mk_perr(PERR_LEN_TOO_LARGE);
                }

                virtual std::string to_string()
                {
                    std::stringstream ss;
                    ss << gourp_type() << command() << body_length();
                    return ss.str();
                }

                ::pilo::i8_t gourp_type() const
                {
                    return (::pilo::i8_t)(this->_m_o_len >> 31);
                }

                ::pilo::u16_t command() const
                {
                    return this->_m_command;
                }

                virtual const char* to_cstring(char* buffer, ::pilo::i64_t sz)
                {
                    ::pilo::core::io::string_formated_output(buffer, sz, "%d,%d,%d", gourp_type(), command(), body_length());
                    return buffer;
                }


                ::pilo::u32_t& ref_field_option_and_len() { return _m_o_len;  }
                ::pilo::u16_t& ref_field_command() { return _m_command; }

            protected:
                ::pilo::u32_t   _m_o_len;
                ::pilo::u16_t   _m_command;

            };
            
        }        
    }
}