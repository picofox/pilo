#pragma once

#include "../../core/coredefs.hpp"
#include "../../core/io/istream_interface.hpp"

namespace pilo
{
    namespace core
    {
        namespace io
        {
            template<size_t _Capacity>
            class fixed_memory_istream : public istream_interface
            {
            public:
                fixed_memory_istream() : m_pos(0) {}
                virtual ~fixed_memory_istream() {}


            public: //base method
                inline virtual size_t read(void* value, size_t length) const
                {
                    return (this->read_bytes_array(value,length));
                }

				inline virtual size_t seek(size_t pos) const
				{
					if(pos > _Capacity) 
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
				size_t capacity() const  { return _Capacity; }

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
                    return (size_t)length;
                }

                inline size_t read_string32(char* value, size_t bufLength) const
                {
                    pilo::u32_t length;
                    if (!this->read_u32(length))
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



				

                inline size_t read_bytes_array(void* value, size_t length) const
                {
                    if (m_pos + length > _Capacity)
                    {
                        length = _Capacity - m_pos;
                    }
                    memcpy(value, m_buffer + m_pos, length);
                    m_pos += length;
                    return length;
                }


                //stream operators
                fixed_memory_istream& operator>>(pilo::i64_t& v)
                {
                    if (!read_i64(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_istream& operator>>(pilo::i32_t& v)
                {
                    if (!read_i32(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_istream& operator>>(pilo::i16_t& v)
                {
                    if (!read_i16(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_istream& operator>>(pilo::i8_t& v)
                {
                    if (!read_i8(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_istream& operator>>(pilo::u64_t& v)
                {
                    if (!read_u64(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_istream& operator>>(pilo::u32_t& v)
                {
                    if (!read_u32(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_istream& operator>>(pilo::u16_t& v)
                {
                    if (!read_u16(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_istream& operator>>(pilo::u8_t& v)
                {
                    if (!read_u8(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_istream& operator>>(float& v)
                {
                    if (!read_float(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_istream& operator>>(double& v)
                {
                    if (!read_double(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }      

                fixed_memory_istream& operator>>(char* str)
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

                        if (!read_string32(str, len + 1))
                        {
                            M_ASSERT(false);
                        }
                        
                    }

                    return *this;
                }

                fixed_memory_istream& operator>>(std::string& str)
                {
                    pilo::u32_t len;
                    if (!read_u32(len))
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

                    str.assign(m_buffer + m_pos, (size_t)len);
                    m_pos += len;


                    return *this;
                }

            protected:
                inline virtual bool check_buffer(size_t wantedBytes) const
                {
                    size_t length = m_pos + wantedBytes;
                    if (length > _Capacity)
                    {
                        M_ASSERT(false);
                        return false;
                    }
                    return true;
                }

            public: //members
                mutable size_t  m_pos;
                char			m_buffer[_Capacity];

            };
        }
    }
}