#pragma once

#include "../../pilo.hpp"
#include "../io/formatted_io.hpp"
#include "../string/string_operation.hpp"
#include "../datetime/timestamp.hpp"
#include "../memory/serializable_interface.hpp"
#include "../memory/serializable_header_interface.hpp"
#include "./inner_sample_data.hpp"
#include "../memory/byte_buffer_interface.hpp"
#include "../memory/default_portable_object_pool.hpp"


namespace pilo
{
	namespace core
	{
		namespace testing
		{
			template<::pilo::i32_t SIZE>
			class sample_object : public ::pilo::core::memory::serializable_interface
				, public ::pilo::core::memory::default_portable_object_pool<sample_object<SIZE>>
			{
			public:
				sample_object()	: _is_server(false), _thread_no(0), _id(0), _i8v(0), _u8v(0)
					, _i16v(0), _u16v(0), _i32v(0), _i64v(0), _f32v(0), _f64v(0), _padding{0}
				{
				}

				void set()
				{
					static ::pilo::u64_t s_id;
					_id = s_id++;
					_i8v = (::pilo::i8_t)_id % 256;
					_u8v = (::pilo::u8_t)_id % 256;
					_i16v = (::pilo::i16_t)_id % 65536;
					_u16v = (::pilo::u16_t)_id % 65536;
					_i32v = (::pilo::i32_t)_id % PILO_INT32_MAX;
					_i64v = (::pilo::i64_t)_id;
					_f32v = ((::pilo::f32_t)_i32v / 3.0f);
					_f64v = ((::pilo::f64_t)_i64v / 3.0f);
					_padding[0] = '@';
					_padding[SIZE - 44] = '$';
				}

				::pilo::i64_t byte_size() const
				{
					return SIZE;
				}

				bool validate(::pilo::u64_t id) const
				{
					if (id != _id)
						return false;
					if (_i8v != (::pilo::i8_t)_id % 256)
						return false;
					if (_u8v != (::pilo::u8_t)_id % 256)
						return false;
					if (_i16v != (::pilo::i16_t)_id % 65536)
						return false;
					if (_u16v != (::pilo::u16_t)_id % 65536)
						return false;
					if (_i32v != (::pilo::i32_t)_id % PILO_INT32_MAX)
						return false;
					if (_i64v != (::pilo::i64_t)_id)
						return false;
					if (_f32v != ((::pilo::f32_t)_i32v / 3.0f))
						return false;
					if (_f64v != ((::pilo::f64_t)_i64v / 3.0f))
						return false;
					if (_padding[0] != '@' || _padding[SIZE - 44] != '$')
						return false;
					return true;
				}

				bool validate() const
				{
					return validate(_id);
				}

				::pilo::i64_t id() const { return _id; }

				// Inherited via serializable_interface
				::pilo::err_t serialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) const override
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
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_uint32(this->_thread_no) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_uint64(this->_id) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_int8(this->_i8v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_uint8(this->_u8v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_int16(this->_i16v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}


					if (byte_buffer->write_uint16(this->_u16v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_int32(this->_i32v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_int64(this->_i64v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_float32(this->_f32v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_float64(this->_f64v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
					}

					if (byte_buffer->write_raw_bytes(this->_padding, 0, SIZE-43) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_WRITE_FAIL);
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

				::pilo::err_t deserialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) override
				{
					::pilo::i64_t initial_length = byte_buffer->read_available();
					::pilo::err_t ret = PILO_OK;

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
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_uint32(this->_thread_no) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_uint64(this->_id) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_int8(this->_i8v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_uint8(this->_u8v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_int16(this->_i16v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_uint16(this->_u16v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_int32(this->_i32v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_int64(this->_i64v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_float32(this->_f32v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					if (byte_buffer->read_float64(this->_f64v) != PILO_OK)
					{
						return ::pilo::mk_perr( PERR_IO_READ_FAIL);
					}

					byte_buffer->read_raw_bytes(this->_padding, SIZE-43, 0, SIZE - 43, ret);
					if (ret != PILO_OK)
						return ret;

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


			private:
				bool			_is_server;
				::pilo::u32_t	_thread_no; //5
				::pilo::u64_t	_id; //13
				::pilo::i8_t	_i8v; //14
				::pilo::u8_t    _u8v; //15
				::pilo::i16_t	_i16v; //17
				::pilo::u16_t	_u16v;  //19
				::pilo::i32_t   _i32v; //23
				::pilo::i64_t	_i64v; //31
				::pilo::f32_t   _f32v; //35
				::pilo::f64_t   _f64v; //43
				char			_padding[SIZE - 43];
			};
		}
	}
}