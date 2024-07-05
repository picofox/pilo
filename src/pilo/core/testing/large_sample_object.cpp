#include "large_sample_object.hpp"
#include "../memory/byte_buffer_interface.hpp"


//::pilo::u64_t pilo::core::testing::large_sample_object::s_id = 1;

::pilo::core::testing::large_sample_object* pilo::core::testing::large_sample_object::deserialise(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer)
{
    ::pilo::core::testing::large_sample_object* obj = new ::pilo::core::testing::large_sample_object();
    if (obj != nullptr)
    {
        if (obj->deserialize(header, byte_buffer) == PILO_OK)
        {
            return obj;
        }
    }
    return nullptr;
}


::pilo::err_t pilo::core::testing::large_sample_object::serialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) const
{
    ::pilo::i64_t header_begin_pos = byte_buffer->write_pos();
    ::pilo::err_t ret = PILO_OK;

    
    if (header != nullptr)
    {
        ret = header->begin_serializing(byte_buffer);
        if (ret != PILO_OK)
        {
            return ret;
        }
    }
    

    if (byte_buffer->write_bool(this->_is_server) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint32(this->_thread_no) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint64(this->_id) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int8(this->_i8v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint8(this->_u8v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int16(this->_i16v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }
        

    if (byte_buffer->write_uint16(this->_u16v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int32(this->_i32v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int64(this->_i64v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_float32(this->_f32v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }        

    if (byte_buffer->write_float64(this->_f64v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_cstring(this->_dcstr, 0, _PILO_TEST_DC_STR_LEN) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_cstring(this->_fcstr, 0, ::pilo::core::string::character_count(this->_fcstr)) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_bytes(this->_bin_data, 0, 37) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_bytes(this->_bin_data_fiexed, 0, 37) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_bool_array(this->_db_arr, 0, 11) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_bool_array(this->_fb_arr, 0, 11) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int8_array(this->_i8_d_arr, 0, 15) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int8_array(this->_i8_f_arr, 0, 15) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint8_array(this->_u8_d_arr, 0, 11) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint8_array(this->_u8_f_arr, 0, 11) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_float32_array(this->_f32_d_arr, 0, 11) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_float32_array(this->_f32_f_arr, 0, 11) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_float64_array(this->_f64_d_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_float64_array(this->_f64_f_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int64_array(this->_i64_d_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int64_array(this->_i64_f_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint64_array(this->_u64_d_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint64_array(this->_u64_f_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int32_array(this->_i32_d_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int32_array(this->_i32_f_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint32_array(this->_u32_d_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint32_array(this->_u32_f_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int16_array(this->_i16_d_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_int16_array(this->_i16_f_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint16_array(this->_u16_d_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_uint16_array(this->_u16_f_arr, 0, 10) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_object(&this->_inner_f) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_object(this->_inner_d) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_object(this->_inner_null) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_object_array(this->_inner_f_arr, 0, 7) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_object_ptr_array((const ::pilo::core::memory::serializable_interface**)this->_inner_d_arr, 0, 7) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (byte_buffer->write_object_ptr_array((const ::pilo::core::memory::serializable_interface**)this->_inner_d2_arr, 0, 7) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_WRITE_FAIL);
    }

    if (header != nullptr)
    {
        ret = header->end_serializing(byte_buffer, header_begin_pos, byte_buffer->write_pos() - header_begin_pos - header->header_length());
        if (ret != PILO_OK)
        {
            return ret;
        }
    }

    

    

    return PILO_OK;
}

::pilo::err_t pilo::core::testing::large_sample_object::deserialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer)
{
    ::pilo::i64_t initial_length = byte_buffer->read_available();
    ::pilo::err_t ret = PILO_OK;
    ::pilo::i64_t rlen = 0;
    void* rc_ptr = nullptr;

    if (header != nullptr)
    {
        ret = header->begin_deserializing(byte_buffer);
        if (ret != PILO_OK) 
        {
            return ret;
        }
    }

    if (byte_buffer->read_bool(this->_is_server) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_uint32(this->_thread_no) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_uint64(this->_id) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_int8(this->_i8v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_uint8(this->_u8v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_int16(this->_i16v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_uint16(this->_u16v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_int32(this->_i32v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_int64(this->_i64v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_float32(this->_f32v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    if (byte_buffer->read_float64(this->_f64v) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    this->_dcstr = byte_buffer->read_cstring(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK) 
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_cstring(_fcstr,sizeof(_fcstr), 0, rlen, ret);
    if (ret != PILO_OK || _fcstr != rc_ptr) 
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_bin_data = byte_buffer->read_bytes(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_bytes(_bin_data_fiexed, sizeof(_bin_data_fiexed), 0, rlen, ret);
    if (ret != PILO_OK || _bin_data_fiexed != rc_ptr)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_db_arr = byte_buffer->read_bool_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_bool_array(_fb_arr, 11, 0, rlen, ret);
    if (ret != PILO_OK || rc_ptr != _fb_arr)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_i8_d_arr = byte_buffer->read_int8_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_int8_array(this->_i8_f_arr, 15, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_u8_d_arr = byte_buffer->read_uint8_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_uint8_array(this->_u8_f_arr, 11, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_f32_d_arr = byte_buffer->read_float32_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_float32_array(this->_f32_f_arr, 11, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_f64_d_arr = byte_buffer->read_float64_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_float64_array(this->_f64_f_arr, 10, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_i64_d_arr = byte_buffer->read_int64_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_int64_array(this->_i64_f_arr, 10, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_u64_d_arr = byte_buffer->read_uint64_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_uint64_array(this->_u64_f_arr, 10, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_i32_d_arr = byte_buffer->read_int32_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_int32_array(this->_i32_f_arr, 10, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_u32_d_arr = byte_buffer->read_uint32_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_uint32_array(this->_u32_f_arr, 10, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_i16_d_arr = byte_buffer->read_int16_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_int16_array(this->_i16_f_arr, 10, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_u16_d_arr = byte_buffer->read_uint16_array(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    rc_ptr = byte_buffer->read_uint16_array(this->_u16_f_arr, 10, 0, rlen, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    if (byte_buffer->read_object(&this->_inner_f) != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_inner_d = byte_buffer->read_object<::pilo::core::testing::inner_sample_data>(nullptr, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    this->_inner_null = byte_buffer->read_object<::pilo::core::testing::inner_sample_data>(nullptr, ret);
    if (ret != PILO_OK)
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);

    if (byte_buffer->read_object_array<::pilo::core::testing::inner_sample_data>(this->_inner_f_arr, 7, 0, rlen) != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    byte_buffer->read_object_ptr_array<::pilo::core::testing::inner_sample_data>(this->_inner_d_arr, 7, 0, rlen, ret);
    if (ret != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }

    this->_inner_d2_arr = byte_buffer->read_object_ptr_array<::pilo::core::testing::inner_sample_data>(nullptr, 0, 0, rlen, ret);
    if (ret != PILO_OK)
    {
        return ::pilo::mk_perr(PERR_IO_READ_FAIL);
    }


    if (header != nullptr)
    {
        ret = header->end_deserializing(byte_buffer, initial_length - byte_buffer->read_available() - header->header_length());
        if (ret != PILO_OK)
        {
            return ret;
        }
    }    

    return PILO_OK;
}
