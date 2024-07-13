#pragma once

#include <mutex>
#include <algorithm>
#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include "../container/singly_linked_selflist.hpp"
#include "../memory/object_pool.hpp"
#include "../memory/compactable_object_pool.hpp"
#include "../algorithm/minmax.hpp"
#include "../memory/abstract_byte_buffer.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template<::pilo::i64_t TA_INIT_SIZE, bool TA_BIGENDIAN>
            class fixed_byte_buffer : public abstract_byte_buffer<TA_BIGENDIAN>
            {
            public:
                fixed_byte_buffer() : _m_capacity(TA_INIT_SIZE), _m_begin_pos(0), _m_length(0), _m_data((char*)PMF_HEAP_MALLOC(TA_INIT_SIZE))
                {
                    
                }



                bool validate(std::string& info, ::pilo::i64_t cap, ::pilo::i64_t beg, ::pilo::i64_t len, ::pilo::i64_t list_size)
                {
                    std::stringstream ss;
                    if (this->_m_capacity != cap)
                    {
                        ss << "capa invliad expect " << cap << " got " << this->_m_capacity;
                        info = ss.str();
                        return false;
                    }
                    if (this->_m_begin_pos != beg)
                    {
                        ss << "beg invliad expect " << beg << " got " << this->_m_begin_pos;
                        info = ss.str();
                        return false;
                    }
                    if (this->_m_length != len)
                    {
                        ss << "len invliad expect " << len << " got " << this->_m_length;
                        info = ss.str();
                        return false;
                    }
                    return true;
                }

                virtual ~fixed_byte_buffer()
                {
                    clear();
                    PMF_HEAP_FREE(this->_m_data);
                }

                virtual void clear()
                {
                    this->_m_begin_pos = 0;
                    this->_m_length = 0;
                }

                virtual void reset()
                {
                    clear();
                }

                virtual ::pilo::err_t compact(::pilo::ioop_type_enum ioop)
                {
                    PMC_UNUSED(ioop);
                    return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
                }

                virtual ::pilo::i64_t read_available() const
                {
                    return this->_m_length;
                }

                virtual ::pilo::i64_t write_available() const
                {
                    return this->_m_capacity - this->_m_begin_pos - this->_m_length;
                }

                virtual ::pilo::i64_t read_pos() const
                {
                    return this->_m_begin_pos;
                }

                virtual ::pilo::i64_t write_pos() const
                {                    
                    return this->_m_begin_pos + this->_m_length;
                }

                virtual ::pilo::i64_t capacity() const
                {
                    return this->_m_capacity;
                }

                virtual ::pilo::err_t readable_seek(::pilo::seek_whence_enum whence, ::pilo::i64_t off, bool compact, ::pilo::i64_t* old_pos)
                {
                    PMC_UNUSED(compact);

                    ::pilo::set_if_ptr_is_not_null<::pilo::i64_t>(old_pos, this->read_pos());
                    if (whence == ::pilo::seek_whence_enum::begin)
                    {
                        if (off < 0 || off > this->_m_begin_pos + this->_m_length)
                        {
                            return ::pilo::mk_perr( PERR_INV_OFF);
                        }
                        off = off - this->_m_length; //convert to relative
                    }
                    else if (whence == ::pilo::seek_whence_enum::end)
                    {
                        if (off < (0 - this->_m_begin_pos) || off > this->_m_length)
                        {
                            return ::pilo::mk_perr( PERR_INV_OFF);
                        }
                    }
                    else if (whence == ::pilo::seek_whence_enum::end)
                    {
                        return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
                    }
                    if (off == 0)
                        return PILO_OK;

                    this->_m_begin_pos = this->_m_begin_pos + off;
                    this->_m_length = this->_m_length - off;

                    return PILO_OK;
                }
                virtual ::pilo::err_t writable_seek(::pilo::seek_whence_enum whence, ::pilo::i64_t off, bool compact, ::pilo::i64_t* old_pos)
                {
                    PMC_UNUSED(compact);
                    ::pilo::set_if_ptr_is_not_null<::pilo::i64_t>(old_pos, this->_m_begin_pos + this->_m_length);
                    if (whence == ::pilo::seek_whence_enum::begin)
                    {
                        if (off < this->_m_begin_pos)
                        {
                            return ::pilo::mk_perr( PERR_INV_OFF);
                        }
                        off = off - this->_m_length - this->_m_begin_pos; //convert to relative
                    }
                    else if (whence == ::pilo::seek_whence_enum::end)
                    {
                        return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
                    }
                    if (off == 0)
                        return PILO_OK;
                    else if (off < 0)
                    {
                        this->_m_length += off;
                    }
                    else
                    {
                        this->_m_length += off;
                    }

                    return PILO_OK;
                }

                virtual std::string to_string() const
                {
                    std::stringstream ss;
                    ss << _m_capacity << "," << _m_begin_pos << "," << _m_length;
                    return ss.str();
                }

                virtual char* read_raw_bytes(char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t read_length, ::pilo::err_t& err)
                {
                    if (read_length < 0)
                    {
                        err = ::pilo::mk_perr(PERR_INV_LEN);
                        return nullptr;
                    }
                    char* ret_buffer_ptr = dst_buffer;
                    if (read_length > 0)
                    {
                        if (dst_buffer == nullptr || dst_buffer_size - dst_pos < read_length)
                        {
                            ::pilo::i64_t neo_sz = read_length + dst_pos;
                            ret_buffer_ptr = (char*)PMF_HEAP_MALLOC(neo_sz);
                            if (ret_buffer_ptr == nullptr)
                            {
                                err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                                return nullptr;
                            }

                            if (dst_buffer != nullptr && dst_buffer_size > 0)
                            {
                                memcpy(ret_buffer_ptr, dst_buffer, dst_buffer_size);
                            }

                            dst_buffer_size = neo_sz;
                        }
                        
                        memcpy(ret_buffer_ptr + dst_pos, this->_m_data + this->_m_begin_pos, read_length);
                        _m_begin_pos += read_length;
                        _m_length -= read_length;
                    }
                    err = PILO_OK;
                    return ret_buffer_ptr;
                }

                virtual char* peek_raw_bytes(::pilo::i64_t peek_off, char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t peek_length, ::pilo::err_t& err)
                {
                    if (peek_length < 0)
                    {
                        err = ::pilo::mk_perr(PERR_INV_LEN);
                        return nullptr;
                    }
                    if (peek_length > 0)
                    {
                        if (dst_buffer_size - dst_pos < peek_length)
                        {
                            err = ::pilo::mk_perr( PERR_VAL_TOO_SAMLL);
                            return nullptr;
                        }
                        memcpy(dst_buffer + dst_pos, this->_m_data + this->_m_begin_pos + peek_off, peek_length);
                    }                    
                    err = PILO_OK;
                    return dst_buffer;               
                }

                virtual ::pilo::err_t write_raw_bytes_pc(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    if (write_len <= 0)
                        return PILO_OK;

                    _m_pos_cached = _m_begin_pos + this->_m_length;
                    memcpy(_m_data + _m_pos_cached, write_buffer+write_off, write_len);
                    _m_length += write_len;
                    return PILO_OK;
                }

                virtual ::pilo::err_t write_raw_bytes(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    if (write_len < 0)
                    {
                        return ::pilo::mk_perr(PERR_INV_LEN);
                    }

                    if (write_len > 0)
                    {
                        memcpy(_m_data + _m_begin_pos + _m_length, write_buffer + write_off, write_len);
                        _m_length += write_len;
                    }                    
                    
                    return PILO_OK;
                }

                virtual ::pilo::err_t set_raw_bytes(::pilo::i64_t buffer_off, const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    if (write_len < 0)
                    {
                        return ::pilo::mk_perr(PERR_INV_LEN);
                    }

                    if (write_len > 0)
                    {
                        memcpy(_m_data + _m_begin_pos + _m_length + buffer_off, write_buffer + write_off, write_len);
                    }

                    return PILO_OK;
                }

                virtual ::pilo::err_t set_pc_raw_bytes(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    return set_raw_bytes(this->_m_pos_cached - _m_begin_pos - _m_length, write_buffer, write_off, write_len);
                }

                virtual char* read_bytes(char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& bytes_read, ::pilo::err_t& err)
                {
                    ::pilo::i32_t blen = 0;
                    err = this->peek_int32(0, blen);
                    if (err != PILO_OK)
                        return nullptr;
                    if (blen < 0)
                    {
                        bytes_read = 0;
                        this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        return nullptr;
                    }
                    else if (blen == 0)
                    {
                        bytes_read = 0;
                        this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        return dst_buffer;
                    }
                    if (this->read_available() < ((::pilo::i64_t)blen + 4))
                    {
                        err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                        bytes_read = 0;
                        return nullptr;
                    }
                    this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                    char* dst_buffer_ptr = this->read_raw_bytes(dst_buffer, dst_buffer_size, dst_pos, blen, err);
                    if (dst_buffer_ptr == nullptr || err != PILO_OK)
                    {
                        bytes_read = 0;
                        return nullptr;
                    }
                    bytes_read = blen;
                    return dst_buffer_ptr;
                }

                virtual char* peek_bytes(::pilo::i64_t peek_off, char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& bytes_peeked, ::pilo::err_t& err)
                {
                    ::pilo::i32_t blen = 0;
                    err = this->peek_int32(peek_off, blen);
                    if (err != PILO_OK)
                        return nullptr;
                    if (blen < 0)
                    {
                        bytes_peeked = 0;
                        return nullptr;
                    }
                    else if (blen == 0)
                    {
                        bytes_peeked = 0;
                        return dst_buffer;
                    }
                    if (this->read_available() - peek_off < ((::pilo::i64_t)blen + 4))
                    {
                        err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                        bytes_peeked = 0;
                        return nullptr;
                    }

                    char* dst_buffer_ptr = this->peek_raw_bytes(sizeof(::pilo::i32_t) + peek_off, dst_buffer, dst_buffer_size, dst_pos, blen, err);
                    if (dst_buffer_ptr != nullptr || err != PILO_OK)
                    {
                        bytes_peeked = 0;
                        return nullptr;
                    }
                    bytes_peeked = blen;
                    return dst_buffer_ptr;
                }

                virtual ::pilo::err_t write_bytes(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    PMC_ASSERT(write_len <= PILO_INT32_MAX);
                    if (write_buffer == nullptr)
                    {
                        return this->write_int32((::pilo::i32_t)-1);
                    }
                    if (write_len <= 0)
                    {
                        return this->write_int32((::pilo::i32_t)0);
                    }

                    if (this->write_int32((::pilo::i32_t)write_len) == PILO_OK)
                    {
                        return this->write_raw_bytes(write_buffer, write_off, write_len);
                    }
                    return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                }

                virtual ::pilo::err_t set_bytes(::pilo::i64_t buffer_off, const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len)
                {
                    PMC_ASSERT(write_len <= PILO_INT32_MAX);
                    if (this->set_int32(buffer_off, (::pilo::i32_t)write_len) == PILO_OK)
                    {
                        return this->set_raw_bytes(4 + buffer_off, write_buffer, write_off, write_len);
                    }
                    return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                }

                virtual char* read_cstring(char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& str_len, ::pilo::err_t& err)
                {
                    ::pilo::i32_t blen = 0;
                    err = this->peek_int32(0, blen);
                    if (err != PILO_OK)
                        return nullptr;
                    if (blen < 1)
                    {
                        str_len = -1;
                        this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        return nullptr;
                    }
                    else if (blen == 1)
                    {
                        str_len = 0;
                        this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        return dst_buffer;
                    }
                    if (this->read_available() < ((::pilo::i64_t)blen + 4))
                    {
                        err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                        str_len = 0;
                        return nullptr;
                    }
                    this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                    char* dst_buffer_ptr = this->read_raw_bytes(dst_buffer, dst_buffer_size, dst_pos, blen, err);
                    if (dst_buffer_ptr == nullptr || err != PILO_OK)
                    {
                        str_len = 0;
                        return nullptr;
                    }
                    str_len = (::pilo::i64_t)(blen - 1);
                    return dst_buffer_ptr;
                }

                virtual char* peek_cstring(::pilo::i64_t peek_off, char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& str_peeked_len, ::pilo::err_t& err)
                {
                    ::pilo::i32_t blen = 0;
                    err = this->peek_int32(peek_off, blen);
                    if (err != PILO_OK)
                        return nullptr;
                    if (blen < 1)
                    {
                        str_peeked_len = 0;
                        return nullptr;
                    }
                    else if (blen == 1)
                    {
                        str_peeked_len = 0;
                        return dst_buffer;
                    }
                    if (this->read_available() - peek_off < (blen + (::pilo::i64_t)sizeof(::pilo::i32_t)))
                    {
                        err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                        str_peeked_len = 0;
                        return nullptr;
                    }

                    char* dst_buffer_ptr = this->peek_raw_bytes(sizeof(::pilo::i32_t) + peek_off, dst_buffer, dst_buffer_size, dst_pos, blen, err);
                    if (dst_buffer_ptr != nullptr || err != PILO_OK)
                    {
                        str_peeked_len = 0;
                        return nullptr;
                    }
                    str_peeked_len = (::pilo::i64_t)(blen - 1);
                    return dst_buffer_ptr;
                }

                virtual ::pilo::err_t write_cstring(const char* value_type_cstr, ::pilo::i64_t str_off, ::pilo::i64_t str_len)
                {
                    PMC_ASSERT(str_len < PILO_INT32_MAX);
                    if (str_len == -1)
                    {
                        if (value_type_cstr == nullptr)
                            return ::pilo::mk_perr(PERR_NULL_PARAM);
                        str_len = ::pilo::core::string::character_count(value_type_cstr) - str_off;
                    }

                    if (value_type_cstr != nullptr)
                    {
                        if (*value_type_cstr != 0)
                        {
                            if (str_len > 0)
                            {
                                if (this->write_int32((::pilo::i32_t)(str_len + 1)) == PILO_OK)
                                {
                                    if (this->write_raw_bytes(value_type_cstr, str_off, str_len) == PILO_OK)
                                    {
                                        if (this->write_int8(0) == PILO_OK)
                                        {
                                            return PILO_OK;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (this->write_int32((::pilo::i32_t)(1)) == PILO_OK)
                                {
                                    if (this->write_int8(0) == PILO_OK)
                                    {
                                        return PILO_OK;
                                    }
                                }
                            }
                            return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                        }
                    }

                    if (this->write_int32((::pilo::i32_t)(-1)) == PILO_OK)
                    {
                        return PILO_OK;
                    }

                    return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                }
                virtual ::pilo::err_t set_cstring(::pilo::i64_t buffer_off, const char* value_type_cstr, ::pilo::i64_t str_off, ::pilo::i64_t str_len)
                {
                    PMC_ASSERT(str_len < PILO_INT32_MAX);
                    if (str_len == -1)
                    {
                        if (value_type_cstr == nullptr)
                            return ::pilo::mk_perr(PERR_NULL_PARAM);
                        str_len = ::pilo::core::string::character_count(value_type_cstr) - str_off;
                    }

                    if (value_type_cstr != nullptr)
                    {
                        if (*value_type_cstr != 0)
                        {
                            if (str_len > 0)
                            {
                                if (this->set_int32(buffer_off, (::pilo::i32_t)(str_len + 1)) == PILO_OK)
                                {
                                    if (this->set_raw_bytes(buffer_off, value_type_cstr, str_off, str_len) == PILO_OK)
                                    {
                                        if (this->set_int8(buffer_off, 0) == PILO_OK)
                                        {
                                            return PILO_OK;
                                        }
                                    }
                                }
                            }
                            else
                            {
                                if (this->set_int32(buffer_off, (::pilo::i32_t)(1)) == PILO_OK)
                                {
                                    if (this->set_int8(buffer_off, 0) == PILO_OK)
                                    {
                                        return PILO_OK;
                                    }
                                }
                            }
                            return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                        }
                    }

                    if (this->set_int32(buffer_off, (::pilo::i32_t)(-1)) == PILO_OK)
                    {
                        return PILO_OK;
                    }

                    return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                }

                virtual ::pilo::err_t read_string(std::string& str)
                {
                    char dst_buffer[64] = { 0 };
                    ::pilo::i32_t blen = 0;
                    ::pilo::err_t err = PILO_OK;
                    str.clear();
                    if (this->read_int32(blen) == PILO_OK)
                    {
                        if (blen > 0)
                        {
                            while ((::pilo::i64_t )blen > (::pilo::i64_t) sizeof(dst_buffer))
                            {
                                char* dst_buffer_ptr = this->read_raw_bytes(dst_buffer, sizeof(dst_buffer), 0, sizeof(dst_buffer), err);
                                if (dst_buffer_ptr == nullptr)
                                {
                                    return err;
                                }
                                str.append(dst_buffer, 0, sizeof(dst_buffer));
                                blen -= sizeof(dst_buffer);
                            }
                            if (blen > 0)
                            {
                                char* dst_buffer_ptr = this->read_raw_bytes(dst_buffer, sizeof(dst_buffer), 0, blen, err);
                                if (dst_buffer_ptr == nullptr)
                                {
                                    return err;
                                }
                                str.append(dst_buffer, 0, blen);
                            }
                            return PILO_OK;
                        }
                        else
                        {
                            str.append("");
                            return PILO_OK;
                        }
                    }

                    return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                }
                virtual ::pilo::err_t peek_string(::pilo::i64_t peek_off, std::string& str)
                {
                    char dst_buffer[64] = { 0 };
                    ::pilo::i32_t blen = 0;
                    ::pilo::i64_t idx = 0;
                    ::pilo::err_t err = PILO_OK;
                    if (this->peek_int32(peek_off, blen) == PILO_OK)
                    {
                        if (blen > 0)
                        {
                            while (blen > (::pilo::i32_t) sizeof(dst_buffer))
                            {
                                char* dst_buffer_ptr = this->peek_raw_bytes(peek_off + 4 + idx, dst_buffer, sizeof(dst_buffer), 0, sizeof(dst_buffer), err);
                                if (dst_buffer_ptr == nullptr)
                                {
                                    return err;
                                }
                                idx += sizeof(dst_buffer);
                                str.append(dst_buffer, 0, sizeof(dst_buffer));
                                blen -= sizeof(dst_buffer);
                            }
                            if (blen > 0)
                            {
                                char* dst_buffer_ptr = this->peek_raw_bytes(peek_off + 4 + idx, dst_buffer, sizeof(dst_buffer), 0, blen, err);
                                if (dst_buffer_ptr == nullptr)
                                {
                                    return err;
                                }
                                str.append(dst_buffer, 0, blen);
                            }
                            return PILO_OK;
                        }
                        else
                        {
                            str.append("");
                            return PILO_OK;
                        }

                    }

                    return ::pilo::mk_perr( PERR_IO_READ_FAIL);
                }
                virtual ::pilo::err_t write_string(const std::string& str)
                {
                    return this->write_bytes(str.c_str(), 0, str.size());
                }
                virtual ::pilo::err_t set_string(::pilo::i64_t buffer_off, std::string& str)
                {
                    return this->set_cstring(buffer_off, str.c_str(), 0, str.size());
                }

                virtual ::pilo::err_t iterate(::pilo::core::memory::byte_buffer_interface::iterate_func_type iter, void* ctx, ::pilo::i64_t max_bytes, ::pilo::i64_t* out_bytes, bool ign_err)
                {
                    ::pilo::i64_t l = max_bytes < _m_length ? max_bytes : _m_length;
                    ::pilo::err_t err = iter(this, _m_data +  _m_begin_pos, l, ctx);
                    if (err != PILO_OK) {
                        ::pilo::set_if_ptr_is_not_null(out_bytes, (::pilo::i64_t) 0);
                        return err;
                    }
                    ::pilo::set_if_ptr_is_not_null(out_bytes, l);
                    return PILO_OK;
                }

            protected:
                ::pilo::i64_t _m_capacity;
                ::pilo::i64_t _m_begin_pos;
                ::pilo::i64_t _m_length;
                ::pilo::i64_t _m_pos_cached;
                char* _m_data;
            };

        }
    }
}