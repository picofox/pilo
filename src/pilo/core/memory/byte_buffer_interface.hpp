#pragma once

#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include "./serializable_interface.hpp"
#include <deque>
#include "../../tlv.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            class byte_buffer_interface
            {
            public:
                typedef ::pilo::err_t(*iterate_func_type)(byte_buffer_interface* buf, char* buffer, ::pilo::i64_t b_len, void* ctx);

            public:
                virtual ~byte_buffer_interface() {}

                virtual ::pilo::err_t iterate(iterate_func_type iter, void* ctx, ::pilo::i64_t max_bytes, ::pilo::i64_t * out_bytes, bool ign_err) = 0;

                virtual ::pilo::i64_t read_available() const = 0;
                virtual ::pilo::i64_t write_available() const = 0;
                virtual ::pilo::i64_t read_pos() const = 0;
                virtual ::pilo::i64_t write_pos() const = 0;
                virtual ::pilo::i64_t capacity() const = 0;
                virtual std::string to_string() const = 0;

                virtual ::pilo::err_t compact(::pilo::ioop_type_enum ioop) = 0;

                virtual ::pilo::err_t readable_seek(::pilo::seek_whence_enum whence, ::pilo::i64_t off, bool compact, ::pilo::i64_t* old_pos) = 0;
                virtual ::pilo::err_t writable_seek(::pilo::seek_whence_enum whence, ::pilo::i64_t off, bool conpact, ::pilo::i64_t* old_pos) = 0;


                virtual void clear() = 0;
                virtual void reset() = 0;

                virtual char* read_raw_bytes(char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t read_length, ::pilo::err_t & err) = 0;
                virtual char* peek_raw_bytes(::pilo::i64_t peek_off, char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t peek_length, ::pilo::err_t & err) = 0;
                virtual ::pilo::err_t write_raw_bytes(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len) = 0;
                virtual ::pilo::err_t set_raw_bytes(::pilo::i64_t buffer_off, const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len) = 0;

                virtual ::pilo::err_t write_raw_bytes_pc(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len) = 0;
                virtual ::pilo::err_t set_pc_raw_bytes(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len) = 0;

                virtual ::pilo::err_t read_bool(bool& iv)
                {                    
                    ::pilo::i8_t v;
                    ::pilo::err_t ret = read_int8(v);
                    if (ret == PILO_OK)
                    {
                        iv = (v != 0);
                        return PILO_OK;
                    }
                    return ret;
                }
                virtual ::pilo::err_t peek_bool(::pilo::i64_t off, bool& iv)
                {
                    ::pilo::i8_t v;
                    ::pilo::err_t ret = peek_int8(off, v);
                    if (ret == PILO_OK)
                    {
                        iv = (v != 0);
                        return PILO_OK;
                    }
                    return ret;
                }
                virtual ::pilo::err_t write_bool(bool iv)
                {
                    return write_int8(iv ? 1 : 0);
                }
                virtual ::pilo::err_t set_bool(::pilo::i64_t off, bool iv)
                {
                    return set_int8(off, iv ? 1 : 0);
                }

                virtual ::pilo::err_t read_int8(::pilo::i8_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (nullptr != read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv), err))
                        return PILO_OK;
                    return err;
                }
                virtual ::pilo::err_t peek_int8(::pilo::i64_t off, ::pilo::i8_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (nullptr != peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err))
                        return PILO_OK;
                    return err;
                }
                virtual ::pilo::err_t write_int8(::pilo::i8_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_int8(::pilo::i64_t off, ::pilo::i8_t iv)
                {
                    return  set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t read_uint8(::pilo::u8_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (nullptr != read_raw_bytes((char*)&iv, sizeof(iv), 0, sizeof(iv),err))
                        return PILO_OK;
                    return err;
                }
                virtual ::pilo::err_t peek_uint8(::pilo::i64_t off, ::pilo::u8_t& iv)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (nullptr != peek_raw_bytes(off, (char*)&iv, sizeof(iv), 0, sizeof(iv), err))
                        return PILO_OK;
                    return err;
                }
                virtual ::pilo::err_t write_uint8(::pilo::u8_t iv)
                {
                    return write_raw_bytes((const char*)&iv, 0, sizeof(iv));
                }
                virtual ::pilo::err_t set_uint8(::pilo::i64_t off, ::pilo::u8_t iv)
                {
                    return set_raw_bytes(off, (const char*)&iv, 0, sizeof(iv));
                }


                virtual ::pilo::err_t read_int16(::pilo::i16_t& iv) = 0;
                virtual ::pilo::err_t peek_int16(::pilo::i64_t off, ::pilo::i16_t& iv) = 0;
                virtual ::pilo::err_t write_int16(::pilo::i16_t iv) = 0;
                virtual ::pilo::err_t set_int16(::pilo::i64_t off, ::pilo::i16_t iv) = 0;
                virtual ::pilo::err_t read_uint16(::pilo::u16_t& iv) = 0;
                virtual ::pilo::err_t peek_uint16(::pilo::i64_t off, ::pilo::u16_t& iv) = 0;
                virtual ::pilo::err_t write_uint16(::pilo::u16_t iv) = 0;
                virtual ::pilo::err_t set_uint16(::pilo::i64_t off, ::pilo::u16_t iv) = 0;
                virtual ::pilo::err_t read_int32(::pilo::i32_t& iv) = 0;
                virtual ::pilo::err_t peek_int32(::pilo::i64_t off, ::pilo::i32_t& iv) = 0;
                virtual ::pilo::err_t write_int32(::pilo::i32_t iv) = 0;
                virtual ::pilo::err_t set_int32(::pilo::i64_t off, ::pilo::i32_t iv) = 0;
                virtual ::pilo::err_t read_uint32(::pilo::u32_t& iv) = 0;
                virtual ::pilo::err_t peek_uint32(::pilo::i64_t off, ::pilo::u32_t& iv) = 0;
                virtual ::pilo::err_t write_uint32(::pilo::u32_t iv) = 0;
                virtual ::pilo::err_t write_uint32_pc(::pilo::u32_t iv) = 0;
                virtual ::pilo::err_t set_pc_uint32(::pilo::u32_t iv) = 0;
                virtual ::pilo::err_t set_uint32(::pilo::i64_t off, ::pilo::u32_t iv) = 0;
                virtual ::pilo::err_t read_int64(::pilo::i64_t& iv) = 0;
                virtual ::pilo::err_t peek_int64(::pilo::i64_t off, ::pilo::i64_t& iv) = 0;
                virtual ::pilo::err_t write_int64(::pilo::i64_t iv) = 0;
                virtual ::pilo::err_t set_int64(::pilo::i64_t off, ::pilo::i64_t iv) = 0;
                virtual ::pilo::err_t read_uint64(::pilo::u64_t& iv) = 0;
                virtual ::pilo::err_t peek_uint64(::pilo::i64_t off, ::pilo::u64_t& iv) = 0;
                virtual ::pilo::err_t write_uint64(::pilo::u64_t iv) = 0;
                virtual ::pilo::err_t set_uint64(::pilo::i64_t off, ::pilo::u64_t iv) = 0;
                virtual ::pilo::err_t read_float32(::pilo::f32_t& iv) = 0;
                virtual ::pilo::err_t peek_float32(::pilo::i64_t off, ::pilo::f32_t& iv) = 0;
                virtual ::pilo::err_t write_float32(::pilo::f32_t iv) = 0;
                virtual ::pilo::err_t set_float32(::pilo::i64_t off, ::pilo::f32_t iv) = 0;
                virtual ::pilo::err_t read_float64(::pilo::f64_t& iv) = 0;
                virtual ::pilo::err_t peek_float64(::pilo::i64_t off, ::pilo::f64_t& iv) = 0;
                virtual ::pilo::err_t write_float64(::pilo::f64_t iv) = 0;
                virtual ::pilo::err_t set_float64(::pilo::i64_t off, ::pilo::f64_t iv) = 0;
                           

                ::pilo::err_t read(::pilo::i8_t & v) { return this->read_int8(v); }
                ::pilo::err_t read(::pilo::u8_t& v) { return this->read_uint8(v); }
                ::pilo::err_t read(::pilo::i16_t& v) { return this->read_int16(v); }
                ::pilo::err_t read(::pilo::u16_t& v) { return this->read_uint16(v); }
                ::pilo::err_t read(::pilo::i32_t& v) { return this->read_int32(v); }
                ::pilo::err_t read(::pilo::u32_t& v) { return this->read_uint32(v); }
                ::pilo::err_t read(::pilo::i64_t& v) { return this->read_int64(v); }
                ::pilo::err_t read(::pilo::u64_t& v) { return this->read_uint64(v); }
                ::pilo::err_t read(bool & v) 
                { 
                    ::pilo::u8_t iv;
                    ::pilo::err_t err = this->read_uint8(iv);
                    if (iv == 0)
                        v = false;
                    else
                        v = true;
                    return err;
                }
                ::pilo::err_t read(::pilo::f32_t& v) { return this->read_float32(v); }
                ::pilo::err_t read(::pilo::f64_t& v) { return this->read_float64(v); }
                ::pilo::err_t read(char* & cstr) 
                { 
                    ::pilo::i64_t rlen = 0;
                    ::pilo::err_t err = PILO_OK;
                    cstr =  this->read_cstring(nullptr, 0, 0, rlen, err);

                    return err;
                }
                ::pilo::err_t read(std::string& str) { return this->read_string(str); }
                inline ::pilo::err_t read(::pilo::tlv*& t) 
                {  
                    ::pilo::err_t err = PILO_OK;
                    t = this->read_tlv(err);
                    return err;
                }
                ::pilo::err_t read(::pilo::core::memory::serializable_interface* obj) { return obj->deserialize(nullptr, this); }
                template <typename T>
                ::pilo::err_t read(T& obj)
                {
                    return obj.deserialize(nullptr, this);
                }
                template <typename T>
                ::pilo::err_t read(T* obj)
                {
                    return obj->deserialize(nullptr, this);
                }
                


                ::pilo::err_t write(::pilo::i8_t v) { return this->write_int8(v); }
                ::pilo::err_t write(::pilo::u8_t v) { return this->write_uint8(v); }
                ::pilo::err_t write(::pilo::i16_t v) { return this->write_int16(v); }
                ::pilo::err_t write(::pilo::u16_t v) { return this->write_uint16(v); }
                ::pilo::err_t write(::pilo::i32_t v) { return this->write_int32(v); }
                ::pilo::err_t write(::pilo::u32_t v) { return this->write_uint32(v); }
                ::pilo::err_t write(::pilo::i64_t v) { return this->write_int64(v); }
                ::pilo::err_t write(::pilo::u64_t v) { return this->write_uint64(v); }
                ::pilo::err_t write(bool b) { return this->write_uint8( b ? (::pilo::u8_t)1 : (::pilo::u8_t)0); }
                ::pilo::err_t write(::pilo::f32_t v) { return this->write_float32(v); }
                ::pilo::err_t write(::pilo::f64_t v) { return this->write_float64(v); }
                ::pilo::err_t write(const char* cstr) { return this->write_cstring(cstr, 0, -1); }
                ::pilo::err_t write(const std::string & v) { return this->write_string(v); }
                ::pilo::err_t write(::pilo::core::memory::serializable_interface * obj) { return obj->serialize(nullptr, this); }
                ::pilo::err_t write(const ::pilo::tlv* t) 
                { 
                    if (t == nullptr)
                    {
                        this->write_int8(-1);
                        return PILO_OK;
                    }
                    else
                    {
                        this->write_int8(1);
                    }
                    return t->serialize(nullptr, this); 
                }
                
                inline ::pilo::tlv* read_tlv(::pilo::err_t& err)
                {
                    ::pilo::i8_t ind = 0;
                    err = this->read_int8(ind);
                    if (err != PILO_OK) return nullptr;
                    if (ind != 1)
                    {
                        err = PILO_OK;
                        return nullptr;
                    }
                    ::pilo::tlv* ret = ::pilo::tlv::allocate();
                    if (ret == nullptr)
                    {
                        err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                        return nullptr;

                    }

                    err = ret->deserialize(nullptr, this);
                    if (err != PILO_OK)
                    {
                        ::pilo::tlv::deallocate(ret);
                    }
                    return ret;
                }

                //complex type
                virtual char* read_bytes(char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& bytes_read, ::pilo::err_t & err) = 0;                
                virtual char* peek_bytes(::pilo::i64_t peek_off, char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& bytes_peeked, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t write_bytes(const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len) = 0;
                virtual ::pilo::err_t set_bytes(::pilo::i64_t buffer_off, const char* write_buffer, ::pilo::i64_t write_off, ::pilo::i64_t write_len) = 0;
                virtual char* read_cstring(char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& str_len, ::pilo::err_t& err) = 0;
                virtual char* peek_cstring(::pilo::i64_t peek_off, char* dst_buffer, ::pilo::i64_t dst_buffer_size, ::pilo::i64_t dst_pos, ::pilo::i64_t& str_peeked_len, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t write_cstring(const char* cstr, ::pilo::i64_t str_off, ::pilo::i64_t str_len) = 0;                
                virtual ::pilo::err_t set_cstring(::pilo::i64_t buffer_off, const char* cstr, ::pilo::i64_t str_off, ::pilo::i64_t str_len) = 0;   
                virtual ::pilo::err_t read_string(std::string& str) = 0;                
                virtual ::pilo::err_t peek_string(::pilo::i64_t peek_off, std::string& str) = 0;                
                virtual ::pilo::err_t write_string(const std::string & str) = 0;
                virtual ::pilo::err_t set_string(::pilo::i64_t buffer_off, std::string & str) = 0;

                virtual ::pilo::i64_t* read_int64_array(::pilo::i64_t * arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t write_int64_array(const ::pilo::i64_t* arr, ::pilo::i64_t off, ::pilo::i64_t len) = 0;
                virtual ::pilo::u64_t* read_uint64_array(::pilo::u64_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t write_uint64_array(const ::pilo::u64_t* arr, ::pilo::i64_t off, ::pilo::i64_t len) = 0;
                virtual ::pilo::i32_t* read_int32_array(::pilo::i32_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t write_int32_array(const ::pilo::i32_t* arr, ::pilo::i64_t off, ::pilo::i64_t len) = 0;
                virtual ::pilo::u32_t* read_uint32_array(::pilo::u32_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t write_uint32_array(const ::pilo::u32_t* arr, ::pilo::i64_t off, ::pilo::i64_t len) = 0;


                virtual ::pilo::i16_t* read_int16_array(::pilo::i16_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t write_int16_array(const ::pilo::i16_t* arr, ::pilo::i64_t off, ::pilo::i64_t len) = 0;
                virtual ::pilo::u16_t* read_uint16_array(::pilo::u16_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t write_uint16_array(const ::pilo::u16_t* arr, ::pilo::i64_t off, ::pilo::i64_t len) = 0;


                virtual bool* read_bool_array(bool* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off ,::pilo::i64_t &rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t len = 0;
                    err = this->peek_int32(0, len);
                    if (err != PILO_OK)
                        return nullptr;
                    bool* ret_arr = arr;

                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return nullptr;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return arr;
                    }
                    else
                    {
                        if (read_available() < (pilo::i64_t) len + 4)
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }

                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {                            
                            return nullptr;
                        }

                        if ((arr == nullptr) || (len > arr_cap - off))
                        {
                            ret_arr = (bool*)PMF_HEAP_CALLOC(len + off, sizeof(bool));
                            if (ret_arr == nullptr)
                            {
                                err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                                return nullptr;
                            }
                            if (arr != nullptr && arr_cap > 0)
                            {
                                memcpy(ret_arr, arr, sizeof(bool) * arr_cap);
                            } 
                        }

                        for (::pilo::i32_t i = 0; i < len; i++)
                        {
                            err = this->read_bool(ret_arr[off + i]);
                            if (err != PILO_OK)
                            {
                                PMF_FREE_IF_DIFF_ADDR(arr, ret_arr);
                                return nullptr;
                            }                                
                        }
                        rlen = len;
                        return ret_arr;
                    } 
                }

                virtual ::pilo::err_t write_bool_array(const bool* arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                        ret = this->write_bool(arr[i]);
                        if (ret != PILO_OK)
                            return ret;
                    }
                    return PILO_OK;
                }

                virtual ::pilo::i8_t* read_int8_array(::pilo::i8_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t len = 0;
                    err = this->peek_int32(0, len);
                    if (err != PILO_OK)
                        return nullptr;
                    ::pilo::i8_t* ret_arr = arr;

                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return nullptr;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return arr;
                    }
                    else
                    {
                        if (read_available() < (pilo::i64_t)len + 4)
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }

                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }
                        ret_arr = (::pilo::i8_t*)this->read_raw_bytes((char*)arr, arr_cap, off, len, err);
                        rlen = len;
                        return ret_arr;
                    }
                }

                virtual ::pilo::err_t write_int8_array(const ::pilo::i8_t * arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                    if (ret != PILO_OK)
                        return ret;
                    return this->write_raw_bytes((const char*)arr, off, len);
                }

                
                char* read_wired_rac(::pilo::err_t & err);

                template <typename TA_CONTAINER> ::pilo::err_t read_rac(TA_CONTAINER* arr)
                {
                    if (arr == nullptr)
                    {
                        return ::pilo::mk_perr(PERR_NULL_PARAM);
                    }

                    ::pilo::i32_t cnt = 0;
                    ::pilo::err_t err = this->read_int32(cnt);
                    if (err != PILO_OK)
                        return err;

                    if (cnt < 0)
                    {
                        return PILO_OK;
                    }
                    else if (cnt == 0)
                    {

                    }
                    else
                    {
                        for (::pilo::i32_t i = 0; i < cnt; i++)
                        {
                            typename TA_CONTAINER::value_type v = ::pilo::_type_default_value_<typename TA_CONTAINER::value_type>::get();
                            err = this->read(v);
                            if (err != PILO_OK)
                            {
                                return err;
                            }
                            arr->push_back(v);
                        }
                    }
                    return PILO_OK;
                }  


                char* read_wired_map(::pilo::err_t& err);

                ::pilo::err_t write_wired_rac(::pilo::u16_t vt, char* arr);

                template <typename TA_CONTAINER>
                ::pilo::err_t write_wired_rac(const TA_CONTAINER* arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    ::pilo::err_t err = PILO_OK;
                    ::pilo::u16_t vt = ::pilo::core::rtti::to_wired_value_type<typename TA_CONTAINER::value_type>(nullptr);
                    if (vt >= ::pilo::core::rtti::wired_type::value_type_intrincs_count)
                    {
                        return ::pilo::mk_perr(PERR_INV_PARAM_DT);
                    }

                    err = this->write_uint16(vt);
                    if (err != PILO_OK)
                    {
                        return err;
                    }

                    if (arr == nullptr)
                    {
                        err = this->write_int32((::pilo::i32_t)-1);
                        if (err != PILO_OK)
                        {
                            return err;
                        }
                        return PILO_OK;
                    }

                    if (len < 0)
                    {
                        len = arr->size() - off;
                    }

                    if (off < 0 || off + len >(::pilo::i64_t) arr->size())
                    {
                        err = this->write_int32((::pilo::i32_t)0);
                        if (err != PILO_OK)
                        {
                            return err;
                        }
                        return PILO_OK;
                    }

                    if (off + len > PILO_INT32_MAX)
                    {
                        return ::pilo::mk_perr(PERR_LEN_TOO_LARGE);
                    }
                    ::pilo::i32_t count = (::pilo::i32_t)(off + len);
                    err = this->write_int32(count);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    typename TA_CONTAINER::const_iterator cit = arr->cbegin() + off;
                    for (; cit < arr->cbegin() + off + len; cit++)
                    {
                        this->write(*cit);                        
                    }
                    return PILO_OK;
                }

                template<typename TA_MAP>
                ::pilo::err_t write_wired_map(const TA_MAP* map)
                {
                    ::pilo::err_t err = PILO_OK;
                    ::pilo::u8_t kt = ::pilo::core::rtti::to_wired_key_type<typename TA_MAP::key_type>();
                    ::pilo::u16_t vt = ::pilo::core::rtti::to_wired_value_type<typename TA_MAP::mapped_type>(nullptr);
                    if (kt == ::pilo::core::rtti::wired_type::key_type_na || kt >= ::pilo::core::rtti::wired_type::key_type_count)
                    {
                        return ::pilo::mk_perr(PERR_INV_KEY_TYPE);
                    }
                    if (vt == ::pilo::core::rtti::wired_type::value_type_na)
                    {
                        return ::pilo::mk_perr(PERR_INV_VAL_TYPE);
                    }
                    else if (vt >= ::pilo::core::rtti::wired_type::value_type_intrincs_count)
                    {
                        PMC_ASSERT(false);
                        return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
                    }

                    err = this->write_uint8(kt);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    err = this->write_uint16(vt);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    ::pilo::i32_t count = (::pilo::i32_t)(map->size());
                    err = this->write_int32(count);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    typename TA_MAP::const_iterator cit = map->cbegin();
                    for (; cit != map->cend(); cit++)
                    {
                        this->write(cit->first);
                        this->write(cit->second);
                    }

                    return PILO_OK;
                }
                ::pilo::err_t write_wired_map(::pilo::u8_t kt, ::pilo::u16_t vt, const char* map_char_ptr);



                template <typename TA_CONTAINER>
                ::pilo::err_t write_rac(const TA_CONTAINER * arr, ::pilo::i64_t off, ::pilo::i64_t len)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (arr == nullptr)
                    {
                        err = this->write_int32((::pilo::i32_t)-1);
                        if (err != PILO_OK)
                        {
                            return err;
                        }
                        return PILO_OK;
                    }

                    if (len < 0)
                    {
                        len = arr->size() - off;
                    }

                    if (off < 0 || off + len > (::pilo::i64_t) arr->size())
                    {
                        err = this->write_int32((::pilo::i32_t)0);
                        if (err != PILO_OK)
                        {
                            return err;
                        }
                        return PILO_OK;
                    }

                    if (off + len > PILO_INT32_MAX)
                    {
                        return ::pilo::mk_perr(PERR_LEN_TOO_LARGE);
                    }
                    ::pilo::i32_t count = (::pilo::i32_t) (off + len);                    
                    err = this->write_int32(count);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    typename TA_CONTAINER::const_iterator cit = arr->cbegin() + off;
                    for (; cit < arr->cbegin() + off + len; cit++)
                    {
                        this->write(*cit);
                    }
                    return PILO_OK;                    
                }


                virtual ::pilo::u8_t* read_uint8_array(::pilo::u8_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t len = 0;
                    err = this->peek_int32(0, len);
                    if (err != PILO_OK)
                        return nullptr;
                    ::pilo::u8_t* ret_arr = arr;

                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return nullptr;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return arr;
                    }
                    else
                    {
                        if (read_available() < (pilo::i64_t)len + 4)
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }

                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }
                        ret_arr = (::pilo::u8_t*)this->read_raw_bytes((char*)arr, arr_cap, off, len, err);
                        rlen = len;
                        return ret_arr;
                    }
                }

                virtual ::pilo::err_t write_uint8_array(const ::pilo::u8_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                    if (ret != PILO_OK)
                        return ret;
                    return this->write_raw_bytes((const char*)arr, off, len);
                }

                virtual ::pilo::f32_t* read_float32_array(::pilo::f32_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t len = 0;
                    err = this->peek_int32(0, len);
                    if (err != PILO_OK)
                        return nullptr;
                    ::pilo::f32_t* ret_arr = arr;

                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return nullptr;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return arr;
                    }
                    else
                    {
                        if (read_available() < (pilo::i64_t)(len*sizeof(::pilo::f32_t)) + 4)
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }

                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }
                        ret_arr = (::pilo::f32_t*)this->read_raw_bytes((char*)arr, arr_cap*sizeof(::pilo::f32_t), off * sizeof(::pilo::f32_t), len*sizeof(::pilo::f32_t), err);
                        rlen = len;
                        return ret_arr;
                    }
                }

                virtual ::pilo::err_t write_float32_array(const ::pilo::f32_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                    if (ret != PILO_OK)
                        return ret;
                    return this->write_raw_bytes((const char*)arr, off * sizeof(::pilo::f32_t), len * sizeof(::pilo::f32_t));
                }


                virtual ::pilo::f64_t* read_float64_array(::pilo::f64_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t len = 0;
                    err = this->peek_int32(0, len);
                    if (err != PILO_OK)
                        return nullptr;
                    ::pilo::f64_t* ret_arr = arr;

                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return nullptr;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return arr;
                    }
                    else
                    {
                        if (read_available() < (pilo::i64_t) (len*(sizeof(::pilo::f64_t)) + 4))
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }

                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }
                        ret_arr = (::pilo::f64_t*)this->read_raw_bytes((char*)arr, arr_cap * sizeof(::pilo::f64_t), off * sizeof(::pilo::f64_t), len * sizeof(::pilo::f64_t), err);
                        rlen = len;
                        return ret_arr;
                    }
                }



                virtual ::pilo::err_t write_float64_array(const ::pilo::f64_t * arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                    if (ret != PILO_OK)
                        return ret;
                    return this->write_raw_bytes((const char*)arr, off * sizeof(::pilo::f64_t), len * sizeof(::pilo::f64_t));
                }

                virtual ::pilo::err_t write_object(const ::pilo::core::memory::serializable_interface * object)
                {
                    if (object != nullptr)
                    {
                        this->write_int8(1);
                        return object->serialize(nullptr, this);
                    }
                    else
                    {
                        return this->write_int8(-1);
                    }
                }


                virtual ::pilo::err_t read_object(::pilo::core::memory::serializable_interface* object)
                {
                    ::pilo::i8_t ind = 0;
                    ::pilo::err_t err = this->peek_int8(0, ind);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    if (ind != 1)
                    {
                        return ::pilo::mk_perr( PERR_INC_DATA);
                    }

                    err = this->readable_seek(::pilo::seek_whence_enum::current, 1, true, nullptr);
                    if (err != PILO_OK)
                    {
                        return err;
                    }
                    return object->deserialize(nullptr, this);
                }

                template<typename T>
                T* read_object(T* object_ptr, ::pilo::err_t& err)
                {
                    if (object_ptr == nullptr)
                    {
                        ::pilo::i8_t ind = 0;
                        err = this->peek_int8(0, ind);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 1, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }
                        if (ind == -1)                        
                            return nullptr;                        
                        else if (ind == 1)
                            return T::deserialise(nullptr, this);
                        else
                        {
                            err = ::pilo::mk_perr( PERR_INC_DATA);
                            return nullptr;
                        }                            
                    }
                    else
                    {
                        err = read_object(object_ptr);
                        return object_ptr;
                    }
                }

                template<typename T>
                T** read_object_ptr_array(T** arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t len = 0;
                    err = this->peek_int32(0, len);
                    T** ret_tr = arr;
                    if (err != PILO_OK)
                        return nullptr;
                    T ** ret_arr = arr;
                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return PILO_OK;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return PILO_OK;
                    }
                    else
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }

                        if (arr == nullptr)
                        {
                            ret_arr = (T**)PMF_HEAP_CALLOC(len + off, sizeof(T*));
                            if (ret_arr == nullptr)
                            {
                                err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                                return nullptr;
                            }
                            if (off > 0)
                            {
                                for (::pilo::i32_t i = 0; i < off; i++)
                                {
                                    ret_arr[i] = nullptr;
                                }
                            }
                        }
                        else
                        {
                            if (len > arr_cap - off)
                            {
                                ret_arr = (T**)PMF_HEAP_CALLOC(len + off, sizeof(T*));
                                if (ret_arr == nullptr)
                                {
                                    err = ::pilo::mk_perr( PERR_INSUF_HEAP);
                                    return nullptr;
                                }
                                if (arr_cap > 0)
                                {
                                    for (::pilo::i32_t i = 0; i < arr_cap; i++)
                                    {
                                        ret_arr[i] = arr[i];
                                        arr[i] = nullptr;
                                    }
                                }
                            }
                        }

                        for (::pilo::i32_t i = 0; i < len; i++)
                        {
                            ret_arr[off + i] = this->read_object<T>(nullptr, err);
                            if (err != PILO_OK)
                            {
                                for (::pilo::i32_t j = 0; j <= i; j++)
                                {
                                    if (ret_arr[j] != nullptr)
                                        T::deallocate(ret_arr[j]);
                                }
                                PMF_FREE_IF_DIFF_ADDR(arr, ret_tr);
                                return nullptr;
                            }
                        }
                        rlen = len;
                        return ret_arr;
                    }
                }

                template<typename T>
                ::pilo::err_t read_object_array(T* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen)
                {
                    ::pilo::i32_t len = 0;
                    ::pilo::err_t err = this->peek_int32(0, len);
                    if (err != PILO_OK)
                        return err;
                    T* ret_arr = arr;
                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return PILO_OK;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return PILO_OK;
                    }
                    else
                    {

                        if (read_available() < (pilo::i64_t)len + 4)
                        {
                            return ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                        }                    
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return err;
                        }

                        if (arr == nullptr)
                        {
                            return ::pilo::mk_perr( PERR_VAL_TOO_SAMLL);
                        }
                        else
                        {
                            if (len > arr_cap - off)
                            {
                                rlen = len;
                                return ::pilo::mk_perr( PERR_VAL_TOO_SAMLL);
                            }
                        }

                        for (::pilo::i32_t i = 0; i < len; i++)
                        {
                            this->read_object<T>(&ret_arr[off + i], err);
                            if (err != PILO_OK)
                            {
                                return err;
                            }
                        }
                        rlen = len;
                        return PILO_OK;
                    }
                }

                template<typename T>
                ::pilo::err_t write_object_array(T* arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                    if (ret != PILO_OK)
                        return ret;
                    for (::pilo::i64_t i = off; i < len; i++)
                    {
                        ret = this->write_object(&(arr[i]));
                        if (ret != PILO_OK)
                            return ret;
                    }
                    return PILO_OK;
                }

                virtual ::pilo::err_t write_object_ptr_array(const ::pilo::core::memory::serializable_interface ** arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                    if (ret != PILO_OK)
                        return ret;
                    for (::pilo::i64_t i = off; i < len; i++)
                    {
                        ret = this->write_object(arr[i]);
                        if (ret != PILO_OK)
                            return ret;
                    }
                    return PILO_OK;
                }





            protected:
                ::pilo::i64_t* _read_int64_array(::pilo::i64_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t len = 0;
                    err = this->peek_int32(0, len);
                    if (err != PILO_OK)
                        return nullptr;
                    ::pilo::i64_t* ret_arr = arr;

                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return nullptr;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return arr;
                    }
                    else
                    {
                        if (read_available() < (pilo::i64_t)(len*sizeof(::pilo::i64_t)) + 4)
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }

                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }
                        ret_arr = (::pilo::i64_t*)this->read_raw_bytes((char*)arr, arr_cap * sizeof(::pilo::i64_t), off * sizeof(::pilo::i64_t), len * sizeof(::pilo::i64_t), err);
                        rlen = len;
                        return ret_arr;
                    }
                }

                ::pilo::err_t _write_int64_array(const ::pilo::i64_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                    if (ret != PILO_OK)
                        return ret;
                    return this->write_raw_bytes((const char*)arr, off * sizeof(::pilo::i64_t), len * sizeof(::pilo::i64_t));
                }

                ::pilo::i32_t* _read_int32_array(::pilo::i32_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t len = 0;
                    err = this->peek_int32(0, len);
                    if (err != PILO_OK)
                        return nullptr;
                    ::pilo::i32_t* ret_arr = arr;

                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return nullptr;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return arr;
                    }
                    else
                    {
                        if (read_available() < (pilo::i64_t)(len * sizeof(::pilo::i32_t) + 4))
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }

                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }
                        ret_arr = (::pilo::i32_t*)this->read_raw_bytes((char*)arr, arr_cap * sizeof(::pilo::i32_t), off * sizeof(::pilo::i32_t), len * sizeof(::pilo::i32_t), err);
                        rlen = len;
                        return ret_arr;
                    }
                }

                ::pilo::err_t _write_int32_array(const ::pilo::i32_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                    if (ret != PILO_OK)
                        return ret;
                    return this->write_raw_bytes((const char*)arr, off * sizeof(::pilo::i32_t), len * sizeof(::pilo::i32_t));
                }

                ::pilo::i16_t* _read_int16_array(::pilo::i16_t* arr, ::pilo::i64_t arr_cap, ::pilo::i64_t off, ::pilo::i64_t& rlen, ::pilo::err_t& err)
                {
                    ::pilo::i32_t len = 0;
                    err = this->peek_int32(0, len);
                    if (err != PILO_OK)
                        return nullptr;
                    ::pilo::i16_t* ret_arr = arr;

                    if (len < 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = -1;
                        return nullptr;
                    }
                    else if (len == 0)
                    {
                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        rlen = 0;
                        return arr;
                    }
                    else
                    {
                        if (read_available() < ((pilo::i64_t)(len * sizeof(::pilo::i16_t)) + 4))
                        {
                            err = ::pilo::mk_perr(PERR_RD_PARTIAL_DATA);
                            return nullptr;
                        }

                        err = this->readable_seek(::pilo::seek_whence_enum::current, 4, true, nullptr);
                        if (err != PILO_OK)
                        {
                            return nullptr;
                        }
                        ret_arr = (::pilo::i16_t*)this->read_raw_bytes((char*)arr, arr_cap * sizeof(::pilo::i16_t), off * sizeof(::pilo::i16_t), len * sizeof(::pilo::i16_t), err);
                        rlen = len;
                        return ret_arr;
                    }
                }

                ::pilo::err_t _write_int16_array(const ::pilo::i16_t* arr, ::pilo::i64_t off, ::pilo::i64_t len)
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
                    if (ret != PILO_OK)
                        return ret;
                    return this->write_raw_bytes((const char*)arr, off * sizeof(::pilo::i16_t), len * sizeof(::pilo::i16_t));
                }




            };

            template <>
            inline ::pilo::err_t byte_buffer_interface::read_rac(std::deque<char*>* arr)
            {
                if (arr == nullptr)
                {
                    return ::pilo::mk_perr(PERR_NULL_PARAM);
                }

                ::pilo::i32_t cnt = 0;
                ::pilo::err_t err = this->read_int32(cnt);
                if (err != PILO_OK)
                    return err;

                if (cnt < 0)
                {
                    return PILO_OK;
                }
                else if (cnt > 0)
                {
                    for (::pilo::i32_t i = 0; i < cnt; i++)
                    {
                        ::pilo::i64_t rlen = 0;
                        char* v = this->read_cstring(nullptr, 0, 0, rlen, err);
                        if (err != PILO_OK)
                        {
                            return err;
                        }
                        arr->push_back(v);
                    }
                }
                return PILO_OK;
            }

            template <>
            inline ::pilo::err_t byte_buffer_interface::read_rac(std::deque<::pilo::tlv*>* arr)
            {
                if (arr == nullptr)
                {
                    return ::pilo::mk_perr(PERR_NULL_PARAM);
                }

                ::pilo::i32_t cnt = 0;
                ::pilo::err_t err = this->read_int32(cnt);
                if (err != PILO_OK)
                    return err;

                if (cnt < 0)
                {
                    return PILO_OK;
                }
                else if (cnt > 0)
                {
                    for (::pilo::i32_t i = 0; i < cnt; i++)
                    {
                        ::pilo::tlv* t = ::pilo::tlv::allocate();
                        err = this->read(t);
                        if (err != PILO_OK)
                        {
                            return err;
                        }
                        arr->push_back(t);
                    }
                }
                return PILO_OK;
            }

        }
    }
}

