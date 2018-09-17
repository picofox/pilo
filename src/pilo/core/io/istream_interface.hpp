#pragma once

#include "../../core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace io
        {
            class istream_interface
            {
            public:
                virtual ~istream_interface() {}
                virtual size_t read(void* value, size_t length) const = 0;
                virtual size_t seek(size_t pos) const = 0;
                virtual size_t tell() const = 0;
                virtual EnumErrorCode close() = 0;
				virtual void flush() {};

			public:
				virtual bool   read_i64(pilo::i64_t& value) const   = 0;
				virtual bool   read_i32(pilo::i32_t& value) const   = 0;
				virtual bool   read_i16(pilo::i16_t& value) const   = 0;
				virtual bool   read_i8(pilo::i8_t& value)  const   = 0;
				virtual bool   read_u64(pilo::u64_t& value) const   = 0;
				virtual bool   read_u32(pilo::u32_t& value) const   = 0;
				virtual bool   read_u16(pilo::u16_t& value) const   = 0;
				virtual bool   read_u8(pilo::u8_t& value)  const   = 0;
				virtual bool   read_float(float& value)     const   = 0;
				virtual bool   read_double(double& value)   const   = 0;
				virtual size_t read_string8(char* value, size_t bufLength) const = 0;
				virtual size_t read_string16(char* value, size_t bufLength) const = 0;
				virtual size_t read_string32(char* value, size_t bufLength) const = 0;
				virtual size_t read_bytes_array(void* value, size_t length) const = 0;

				template<typename T>
				bool read_pod(T& value) const
				{
					return (read_bytes_array((char*)&value, sizeof(T)) == sizeof(T));
			    }
            };
        }
    }
}