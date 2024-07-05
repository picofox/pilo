#pragma once

#include "byte_buffer_interface.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template<bool TA_BIGENDIAN>
            class abstract_byte_buffer : public byte_buffer_interface
            {
            public:

            };

            template<>
            class abstract_byte_buffer<true> : public byte_buffer_interface
            {
            public:
                virtual ~abstract_byte_buffer() {}     

                virtual ::pilo::err_t read_int16(::pilo::i16_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohs(iv);
                    return err;
                }
                virtual ::pilo::err_t peek_int16(::pilo::i64_t off, ::pilo::i16_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohs(iv);
                    return err;
                }
                virtual ::pilo::err_t write_int16(::pilo::i16_t iv)
                {
                    iv = htons(iv);
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_int16(::pilo::i64_t off, ::pilo::i16_t iv)
                {
                    iv = htons(iv);
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t read_uint16(::pilo::u16_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohs(iv);
                    return err;
                }
                virtual ::pilo::err_t peek_uint16(::pilo::i64_t off, ::pilo::u16_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohs(iv);
                    return err;
                }
                virtual ::pilo::err_t write_uint16(::pilo::u16_t iv)
                {
                    iv = htons(iv);
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_uint16(::pilo::i64_t off, ::pilo::u16_t iv)
                {
                    iv = htons(iv);
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t read_int32(::pilo::i32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohl(iv);
                    return err;
                }
                virtual ::pilo::err_t peek_int32(::pilo::i64_t off, ::pilo::i32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohl(iv);
                    return err;
                }
                virtual ::pilo::err_t write_int32(::pilo::i32_t iv)
                {
                    iv = htonl(iv);
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_int32(::pilo::i64_t off, ::pilo::i32_t iv)
                {
                    iv = htonl(iv);
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t read_uint32(::pilo::u32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohl(iv);
                    return err;
                }
                virtual ::pilo::err_t peek_uint32(::pilo::i64_t off, ::pilo::u32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohl(iv);
                    return err;
                }
                virtual ::pilo::err_t write_uint32(::pilo::u32_t iv)
                {
                    iv = htonl(iv);
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t write_uint32_pc(::pilo::u32_t iv)
                {
                    iv = htonl(iv);
                    return write_raw_bytes_pc((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_uint32(::pilo::i64_t off, ::pilo::u32_t iv)
                {
                    iv = htonl(iv);
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t set_pc_uint32(::pilo::u32_t iv)
                {
                    iv = htonl(iv);
                    return set_pc_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t read_int64(::pilo::i64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (nullptr != read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err))
                    {
                        iv = ntohll(iv);
                        return PILO_OK;
                    }
                    return err;
                }
                virtual ::pilo::err_t peek_int64(::pilo::i64_t off, ::pilo::i64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (nullptr != peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err))
                    {
                        iv = ntohll(iv);
                        return PILO_OK;
                    }
                    return err;
                }
                virtual ::pilo::err_t write_int64(::pilo::i64_t iv)
                {
                    iv = htonll(iv);
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_int64(::pilo::i64_t off, ::pilo::i64_t iv)
                {
                    iv = htonll(iv);
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t read_uint64(::pilo::u64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohll(iv);
                    return err;
                }
                virtual ::pilo::err_t peek_uint64(::pilo::i64_t off, ::pilo::u64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    iv = ntohll(iv);
                    return err;
                }
                virtual ::pilo::err_t write_uint64(::pilo::u64_t iv)
                {
                    iv = htonll(iv);
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));

                }
                virtual ::pilo::err_t set_uint64(::pilo::i64_t off, ::pilo::u64_t iv)
                {
                    iv = htonll(iv);
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t read_float32(::pilo::f32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t peek_float32(::pilo::i64_t off, ::pilo::f32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_float32(::pilo::f32_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_float32(::pilo::i64_t off, ::pilo::f32_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t read_float64(::pilo::f64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t peek_float64(::pilo::i64_t off, ::pilo::f64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_float64(::pilo::f64_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_float64(::pilo::i64_t off, ::pilo::f64_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }      

                virtual ::pilo::i64_t* read_int64_array(::pilo::i64_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {                    
                    ::pilo::i64_t* ret_ptr = this->_read_int64_array(arr, arr_cap, off, rlen, err);
                    if (err == PILO_OK)
                    {
                        for (::pilo::i64_t i = off; i < off + rlen; i++)
                        {
                            ret_ptr[i] = ntohll(ret_ptr[i]);
                        }
                    }                    
                    return ret_ptr;
                }

                virtual ::pilo::err_t write_int64_array(const ::pilo::i64_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    if (arr == nullptr)
                    {
                        return this->write_int32((::pilo::i32_t)-1);
                    }
                    if (len < 0)
                    {
                        return this->write_int32((::pilo::i32_t)-1);
                    }
                    else if (len > PILO_INT32_MAX)
                    {
                        return ::pilo::mk_perr(PERR_PARAM_OOR_U);
                    }
                    else if (len == 0)
                    {
                        return this->write_int32((::pilo::i32_t)0);
                    }
                    pilo::err_t ret = this->write_int32((::pilo::i32_t)len);
                    for (::pilo::i64_t i = off; i < len; i++)
                    {
                        ret = this->write_int64(arr[i]);
                        if (ret != PILO_OK)
                            return ret;
                    }
                    return PILO_OK;
                }

                virtual ::pilo::u64_t* read_uint64_array(::pilo::u64_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::u64_t* ret_ptr = (::pilo::u64_t *) this->_read_int64_array((::pilo::i64_t *) arr, arr_cap, off, rlen, err);
                    if (err == PILO_OK)
                    {
                        for (::pilo::i64_t i = off; i < off + rlen; i++)
                        {
                            ret_ptr[i] = ntohll(ret_ptr[i]);
                        }
                    }
                    return ret_ptr;
                }
                virtual ::pilo::err_t write_uint64_array(const ::pilo::u64_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    return this->write_int64_array((const ::pilo::i64_t*)arr, off, len);
                }

                virtual ::pilo::i32_t* read_int32_array(::pilo::i32_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t* ret_ptr = this->_read_int32_array(arr, arr_cap, off, rlen, err);
                    if (err == PILO_OK)
                    {
                        for (::pilo::i64_t i = off; i < off + rlen; i++)
                        {
                            ret_ptr[i] = ntohl(ret_ptr[i]);
                        }
                    }
                    return ret_ptr;
                }
                virtual ::pilo::err_t write_int32_array(const ::pilo::i32_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    if (arr == nullptr)
                    {
                        return this->write_int32((::pilo::i32_t)-1);
                    }
                    if (len < 0)
                    {
                        return this->write_int32((::pilo::i32_t)-1);
                    }
                    else if (len > PILO_INT32_MAX)
                    {
                        return ::pilo::mk_perr(PERR_PARAM_OOR_U);
                    }
                    else if (len == 0)
                    {
                        return this->write_int32((::pilo::i32_t)0);
                    }
                    pilo::err_t ret = this->write_int32((::pilo::i32_t)len);
                    for (::pilo::i64_t i = off; i < len; i++)
                    {
                        ret = this->write_int32(arr[i]);
                        if (ret != PILO_OK)
                            return ret;
                    }
                    return PILO_OK;
                }
                virtual ::pilo::u32_t* read_uint32_array(::pilo::u32_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::u32_t* ret_ptr = (::pilo::u32_t*)this->_read_int32_array((::pilo::i32_t*)arr, arr_cap, off, rlen, err);
                    if (err == PILO_OK)
                    {
                        for (::pilo::i64_t i = off; i < off + rlen; i++)
                        {
                            ret_ptr[i] = ntohl(ret_ptr[i]);
                        }
                    }
                    return ret_ptr;
                }
                virtual ::pilo::err_t write_uint32_array(const ::pilo::u32_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    return this->write_int32_array((const ::pilo::i32_t*)arr, off, len);
                }



                virtual ::pilo::i16_t* read_int16_array(::pilo::i16_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i16_t* ret_ptr = this->_read_int16_array(arr, arr_cap, off, rlen, err);
                    if (err == PILO_OK)
                    {
                        for (::pilo::i64_t i = off; i < off + rlen; i++)
                        {
                            ret_ptr[i] = ntohs(ret_ptr[i]);
                        }
                    }
                    return ret_ptr;
                }
                virtual ::pilo::err_t write_int16_array(const ::pilo::i16_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    if (arr == nullptr)
                    {
                        return this->write_int32((::pilo::i32_t)-1);
                    }
                    if (len < 0)
                    {
                        return this->write_int32((::pilo::i32_t)-1);
                    }
                    else if (len > PILO_INT32_MAX)
                    {
                        return ::pilo::mk_perr(PERR_PARAM_OOR_U);
                    }
                    else if (len == 0)
                    {
                        return this->write_int32((::pilo::i32_t)0);
                    }
                    pilo::err_t ret = this->write_int32((::pilo::i32_t)len);
                    for (::pilo::i64_t i = off; i < len; i++)
                    {
                        ret = this->write_int16(arr[i]);
                        if (ret != PILO_OK)
                            return ret;
                    }
                    return PILO_OK;
                }
                virtual ::pilo::u16_t* read_uint16_array(::pilo::u16_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::u16_t* ret_ptr = (::pilo::u16_t*)this->_read_int16_array((::pilo::i16_t*)arr, arr_cap, off, rlen, err);
                    if (err == PILO_OK)
                    {
                        for (::pilo::i64_t i = off; i < off + rlen; i++)
                        {
                            ret_ptr[i] = ntohs(ret_ptr[i]);
                        }
                    }
                    return ret_ptr;
                }
                virtual ::pilo::err_t write_uint16_array(const ::pilo::u16_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    return this->write_int16_array((const ::pilo::i16_t*)arr, off, len);
                }
                
            };

            template<>
            class abstract_byte_buffer<false> : public byte_buffer_interface
            {
            public:
                virtual ~abstract_byte_buffer() {};               

                virtual ::pilo::err_t read_int16(::pilo::i16_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t peek_int16(::pilo::i64_t off, ::pilo::i16_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_int16(::pilo::i16_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_int16(::pilo::i64_t off, ::pilo::i16_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t read_uint16(::pilo::u16_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;                    
                }
                virtual ::pilo::err_t peek_uint16(::pilo::i64_t off, ::pilo::u16_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_uint16(::pilo::u16_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_uint16(::pilo::i64_t off, ::pilo::u16_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t read_int32(::pilo::i32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t peek_int32(::pilo::i64_t off, ::pilo::i32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_int32(::pilo::i32_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_int32(::pilo::i64_t off, ::pilo::i32_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t read_uint32(::pilo::u32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t peek_uint32(::pilo::i64_t off, ::pilo::u32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_uint32(::pilo::u32_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t write_uint32_pc(::pilo::u32_t iv)
                {                   
                    return write_raw_bytes_pc((const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t set_uint32(::pilo::i64_t off, ::pilo::u32_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t set_pc_uint32(::pilo::u32_t iv)
                {
                    return set_pc_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t read_int64(::pilo::i64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t peek_int64(::pilo::i64_t off, ::pilo::i64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_int64(::pilo::i64_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_int64(::pilo::i64_t off, ::pilo::i64_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t read_uint64(::pilo::u64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t peek_uint64(::pilo::i64_t off, ::pilo::u64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_uint64(::pilo::u64_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_uint64(::pilo::i64_t off, ::pilo::u64_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t read_float32(::pilo::f32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t peek_float32(::pilo::i64_t off, ::pilo::f32_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_float32(::pilo::f32_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_float32(::pilo::i64_t off, ::pilo::f32_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::err_t read_float64(::pilo::f64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t peek_float64(::pilo::i64_t off, ::pilo::f64_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err);
                    return err;
                }
                virtual ::pilo::err_t write_float64(::pilo::f64_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_float64(::pilo::i64_t off, ::pilo::f64_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }

                virtual ::pilo::i64_t* read_int64_array(::pilo::i64_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    return this->_read_int64_array(arr, arr_cap, off, rlen, err);
                }

                virtual ::pilo::err_t write_int64_array(const ::pilo::i64_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    return this->_write_int64_array(arr, off, len);
                }

                virtual ::pilo::u64_t* read_uint64_array(::pilo::u64_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    return (::pilo::u64_t*) this->_read_int64_array((::pilo::i64_t*) arr, arr_cap, off, rlen, err);
                }
                virtual ::pilo::err_t write_uint64_array(const ::pilo::u64_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    return this->_write_int64_array((const ::pilo::i64_t*) arr, off, len);
                }
                virtual ::pilo::i32_t* read_int32_array(::pilo::i32_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    return this->_read_int32_array(arr, arr_cap, off, rlen, err);
                }
                virtual ::pilo::err_t write_int32_array(const ::pilo::i32_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    return this->_write_int32_array(arr, off, len);
                }
                virtual ::pilo::u32_t* read_uint32_array(::pilo::u32_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    return (::pilo::u32_t*)this->_read_int32_array((::pilo::i32_t*)arr, arr_cap, off, rlen, err);
                }
                virtual ::pilo::err_t write_uint32_array(const ::pilo::u32_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    return this->_write_int32_array((const ::pilo::i32_t*)arr, off, len);
                }

                virtual ::pilo::i16_t* read_int16_array(::pilo::i16_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    return this->_read_int16_array(arr, arr_cap, off, rlen, err);
                }
                virtual ::pilo::err_t write_int16_array(const ::pilo::i16_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    return this->_write_int16_array(arr, off, len);
                }
                virtual ::pilo::u16_t* read_uint16_array(::pilo::u16_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    return (::pilo::u16_t*)this->_read_int16_array((::pilo::i16_t*)arr, arr_cap, off, rlen, err);
                }
                virtual ::pilo::err_t write_uint16_array(const ::pilo::u16_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    return this->_write_int16_array((const ::pilo::i16_t*)arr, off, len);
                }
            };
        }
    }
}

