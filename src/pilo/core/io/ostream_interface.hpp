#pragma once

#include "../../core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace io
        {
            class ostream_interface
            {
            public:
                virtual ~ostream_interface() {}
				virtual size_t write(const void* value, size_t length) = 0;
                virtual size_t seek(size_t pos) const = 0;
                virtual size_t tell() const = 0;
                virtual EnumErrorCode close() = 0;
				virtual void flush() {};

			public:
				virtual bool write_i64(pilo::i64_t value) = 0;
				virtual bool write_i32(pilo::i32_t value) = 0;
				virtual bool write_i16(pilo::i16_t value) = 0;
				virtual bool write_i8(pilo::i8_t value)  = 0;
				virtual bool write_u64(pilo::u64_t value) = 0;
				virtual bool write_u32(pilo::u32_t value) = 0;
				virtual bool write_u16(pilo::u16_t value) = 0;
				virtual bool write_u8(pilo::u8_t value)  = 0;
				virtual bool write_float(float value)     = 0;
				virtual bool write_double(double value)   = 0;
				virtual size_t write_string8 (const char* value, size_t length, bool force)  = 0;
				virtual size_t write_string16(const char* value, size_t length, bool force) = 0;
				virtual size_t write_string32(const char* value, size_t length, bool force) = 0;
				virtual size_t write_bytes_array(const void* value, size_t length, bool force) = 0;

				template<typename T>
				bool write_pod(const T& value)
				{
					return (write_bytes_array(&value, sizeof(T), false) == sizeof(T));
				}             
            };
        }
    }
}