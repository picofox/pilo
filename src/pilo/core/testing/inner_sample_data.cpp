#include "../../pilo.hpp"
#include "../io/formatted_io.hpp"
#include "../string/string_operation.hpp"
#include "../datetime/timestamp.hpp"
#include "./inner_sample_data.hpp"
#include "../memory/byte_buffer_interface.hpp"

namespace pilo
{
	namespace core
	{
		namespace testing
		{
//			class ::pilo::core::memory::serializable_header_interface;
//			class ::pilo::core::memory::serializable_interface;

			inner_sample_data* inner_sample_data::deserialise(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer)
			{
				::pilo::core::testing::inner_sample_data* obj = ::pilo::core::testing::inner_sample_data::allocate();
				if (obj != nullptr)
				{
					if (obj->deserialize(header, byte_buffer) == PILO_OK)
					{
						return obj;
					}
				}
				return nullptr;
			}

			::pilo::err_t pilo::core::testing::inner_sample_data::serialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) const
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

				if (byte_buffer->write_int32(this->_seq) != PILO_OK)
				{
					return ::pilo::mk_perr( PERR_IO_READ_FAIL);
				}
				if (byte_buffer->write_string(this->_name) != PILO_OK)
				{
					return ::pilo::mk_perr( PERR_IO_READ_FAIL);
				}
				if (byte_buffer->write_float32(this->_sal) != PILO_OK)
				{
					return ::pilo::mk_perr( PERR_IO_READ_FAIL);
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

			::pilo::err_t pilo::core::testing::inner_sample_data::deserialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer)
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

				if (byte_buffer->read_int32(this->_seq) != PILO_OK)
				{
					return ::pilo::mk_perr( PERR_IO_READ_FAIL);
				}
				if (byte_buffer->read_string(this->_name) != PILO_OK)
				{
					return ::pilo::mk_perr( PERR_IO_READ_FAIL);
				}
				if (byte_buffer->read_float32(this->_sal) != PILO_OK)
				{
					return ::pilo::mk_perr( PERR_IO_READ_FAIL);
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

		}
	}
}
