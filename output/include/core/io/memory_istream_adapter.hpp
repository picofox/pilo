#pragma once

#include "../../core/coredefs.hpp"
#include "../../core/io/istream_interface.hpp"
#include <string>

namespace pilo
{
    namespace core
    {
        namespace io
        {
            class memory_istream_adapter : public istream_interface
            {
            public:
                memory_istream_adapter() : m_pos(0) {}
                memory_istream_adapter(void* ptr, size_t sz, size_t pos = 0)
                {
                    attach(ptr, sz, pos);
                }
                virtual ~memory_istream_adapter() {}
                void attach(void* ptr, size_t sz, size_t pos = 0)
                {
                    m_buffer = (char*) ptr;
                    m_size = sz;
                    m_pos = pos;
                }
                void detach()
                {
                    m_buffer = nullptr;
                    m_size = 0;
                    m_pos = 0;
                }


            public: //base method
                inline virtual size_t read(void* value, size_t length) const
                {
                    return (this->read_bytes_array(value,length));
                }

				inline virtual size_t seek(size_t pos) const
				{
					if(pos > m_size) 
					{
						return MC_INVALID_SIZE;
					}
					
					m_pos ^= pos;
					pos ^= m_pos;
					m_pos ^= pos;

					return pos;
				}

				inline virtual size_t tell() const
				{
					return m_pos;
				}

				inline virtual EnumErrorCode close() { return pilo::EC_OK; }

			public: //accessor
				void* data() const { return (void*) m_buffer;  }
				size_t size() const      { return m_pos;     }
				size_t capacity() const  { return m_size; }

			public: //derivative methods read
				inline bool read_i64(pilo::i64_t& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}
				inline bool read_i32(pilo::i32_t& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}
				inline bool read_i16(pilo::i16_t& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}
				inline bool read_i8(pilo::i8_t& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}
				inline bool read_u64(pilo::u64_t& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}
				inline bool read_u32(pilo::u32_t& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}
				inline bool read_u16(pilo::u16_t& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}
				inline bool read_u8(pilo::u8_t& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}

				inline bool read_float(float& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}
				inline bool read_double(double& value) const
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(&value, m_buffer+m_pos, wantedBytes);
					m_pos += wantedBytes;
					return true;
				}

                inline size_t read_string8(char* value, size_t bufLength) const
                {
                    pilo::u8_t length;
                    if (!this->read_u8(length))
                        return MC_INVALID_SIZE;
                    if (bufLength < (size_t)(length + 1))
                        return MC_INVALID_SIZE;

                    if (length == 0)
                    {
                        value[0] = '\0';
                        return (size_t)0;
                    }

                    if (this->read_bytes_array((char*)value, length) != length)
                        return MC_INVALID_SIZE;

                    value[length] = '\0';
                    return (size_t)length;
                }

                inline size_t read_string16(char* value, size_t bufLength) const
                {
                    pilo::u16_t length;
                    if (!this->read_u16(length))
                        return MC_INVALID_SIZE;
                    if (bufLength < (size_t)(length + 1))
                        return MC_INVALID_SIZE;

                    if (length == 0)
                    {
                        value[0] = '\0';
                        return (size_t)0;
                    }

                    if (this->read_bytes_array((char*)value, length) != length)
                        return MC_INVALID_SIZE;

                    value[length] = '\0';
                    return (size_t) length;
                }

				inline size_t read_string32(char* value, size_t bufLength) const
				{
					pilo::u32_t length;
					if(!this->read_u32(length))
						return MC_INVALID_SIZE;
					if(bufLength < (size_t)(length+1))
                        return MC_INVALID_SIZE;

					if(length == 0)
					{
						value[0] = '\0';
						return (size_t) 0;
					}

					if(this->read_bytes_array((char*)value, length) != length)
                        return MC_INVALID_SIZE;

					value[length] = '\0';
					return (size_t) length;
				}
				
				

                inline size_t read_bytes_array(void* value, size_t length) const
                {
                    if (m_pos + length > m_size)
                    {
                        length = m_size - m_pos;
                    }
                    memcpy(value, m_buffer + m_pos, length);
                    m_pos += length;
                    return length;
                }
	
                //stream operators
                memory_istream_adapter& operator>>(pilo::i64_t& v)
                {
                    if (!read_i64(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_istream_adapter& operator>>(pilo::i32_t& v)
                {
                    if (!read_i32(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_istream_adapter& operator>>(pilo::i16_t& v)
                {
                    if (!read_i16(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_istream_adapter& operator>>(pilo::i8_t& v)
                {
                    if (!read_i8(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_istream_adapter& operator>>(pilo::u64_t& v)
                {
                    if (!read_u64(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_istream_adapter& operator>>(pilo::u32_t& v)
                {
                    if (!read_u32(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_istream_adapter& operator>>(pilo::u16_t& v)
                {
                    if (!read_u16(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_istream_adapter& operator>>(pilo::u8_t& v)
                {
                    if (!read_u8(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_istream_adapter& operator>>(float& v)
                {
                    if (!read_float(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_istream_adapter& operator>>(double& v)
                {
                    if (!read_double(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }

                memory_istream_adapter& operator>>(char* str)
                {
                    pilo::u32_t len;
                    if (!read_u32(len))
                    {
                        M_ASSERT(false);
                    }

                    if (len > 0)
                    {
                        if (!this->check_buffer(len))
                        {
                            M_ASSERT(false);
                        }

                        if (read_string32(str, len + 1) == MC_INVALID_SIZE)
                        {
                            M_ASSERT(false);
                        }

                    }

                    return *this;
                }

                memory_istream_adapter& operator>>(std::string& str)
                {
                    pilo::u32_t len = 0;
                    if (! read_u32(len))
                    {
                        M_ASSERT(false);
                    }

                    if (len == 0)
                    {
                        str.clear();
                    }

                    if (!this->check_buffer(len))
                    {
                        M_ASSERT(false);
                    }

                    str.assign(m_buffer + m_pos, (size_t) len);
                    m_pos += len;
                    

                    return *this;
                }


            protected:
                inline virtual bool check_buffer(size_t wantedBytes) const
                {
                    size_t length = m_pos + wantedBytes;
                    if (length > m_size)
                    {
                        M_ASSERT(false);
                        return false;
                    }
                    return true;
                }

            public: //members
                mutable size_t  m_pos;
                size_t          m_size;
                char*			m_buffer;

            };
        }
    }
}