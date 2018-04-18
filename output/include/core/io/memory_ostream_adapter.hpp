#pragma once

#include "../../core/coredefs.hpp"
#include "../../core/io/ostream_interface.hpp"
#include "../../core/string/string_util.hpp"
#include <string>

namespace pilo
{
    namespace core
    {
        namespace io
        {
            class memory_ostream_adapter : public ostream_interface
            {
            public:
                memory_ostream_adapter() : m_pos(0),m_size(0),m_buffer(nullptr) {}
                memory_ostream_adapter(void* ptr, size_t sz, size_t pos = 0)
                {
                    attach(ptr,sz,pos);
                }
                virtual ~memory_ostream_adapter() {}
                void attach(void* ptr, size_t sz, size_t pos = 0)
                {
                    m_buffer = (char*)ptr;
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
                inline virtual size_t write(const void* value, size_t length)
                {
                    return (this->write_bytes_array(value, length, false));
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

			public: //derivative methods write
                inline size_t write_bytes_array(const void* value, size_t length, bool force)
                {
                    if (length == 0) return 0;

					if(!this->check_buffer(length)) 
					{
						if (! force )
						{
							return false;
						}
						else
						{
							length = m_size - m_pos;
                            if (length == 0) return 0;
						}
					}

                    memcpy(m_buffer + m_pos, value, length);
                    m_pos += length;
                    return length;
                }

				inline bool write_i64(pilo::i64_t value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

				inline bool write_i32(pilo::i32_t value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

				inline bool write_i16(pilo::i16_t value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

				inline bool write_i8(pilo::i8_t value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

				inline bool write_u64(pilo::u64_t value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

				inline bool write_u32(pilo::u32_t value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

				inline bool write_u16(pilo::u16_t value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

				inline bool write_u8(pilo::u8_t value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

				inline bool write_float(float value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

				inline bool write_double(double value)
				{
					size_t wantedBytes = sizeof(value);
					if(!this->check_buffer(wantedBytes)) return false;
					memcpy(m_buffer+m_pos, &value, wantedBytes);
					m_pos += wantedBytes;					
					return true;
				}

                inline size_t write_string8(const char* value, size_t length, bool force)
                {
                    if (value == nullptr) return MC_INVALID_SIZE;                    

                    if(MC_INVALID_SIZE == length)
                    {
                        length = strlen(value);
                    }

                    if(length > 0xff)
                    {
                        if (! force)
                        {
                            return MC_INVALID_SIZE;
                        }
                        else
                        {
                            length = 0xff;
                        }
                    }

                    if (! check_buffer(length + 1))
                    {
                        if (! force)
                        {
                            return MC_INVALID_SIZE;
                        }
                        else //truncate
                        {
                            length = m_size - m_pos;
                            if (length == 0)
                            {
                                return MC_INVALID_SIZE;
                            }
                            else if (length == 1)
                            {
                                if(!this->write_u8((pilo::u8_t)0))
                                {
                                    return MC_INVALID_SIZE;
                                }
                                return 1;
                            }
                            else
                            {
                                length -= 1;
                            }
                        }
                    }

                    if(!this->write_u8((pilo::u8_t)length))
                    {
                        return MC_INVALID_SIZE;
                    }

                    size_t ret = this->write_bytes_array((char*)value, length, force);

                    if ( ret == MC_INVALID_SIZE)
                    {
                        return MC_INVALID_SIZE;
                    }

                    return ret + 1;
                    
                }

                inline size_t write_string16(const char* value, size_t length, bool force)
                {
                    if (value == nullptr) return MC_INVALID_SIZE;                    

                    if(MC_INVALID_SIZE == length)
                    {
                        length = strlen(value);
                    }

                    if(length > 0xffff)
                    {
                        if (! force)
                        {
                            return MC_INVALID_SIZE;
                        }
                        else
                        {
                            length = 0xffff;
                        }
                    }

                    if (! check_buffer(length + 2))
                    {
                        if (! force)
                        {
                            return MC_INVALID_SIZE;
                        }
                        else //truncate
                        {
                            length = m_size - m_pos;
                            if (length < 2)
                            {
                                return MC_INVALID_SIZE;
                            }
                            else if (length == 2)
                            {
                                if(!this->write_u16((pilo::u16_t)0))
                                {
                                    return MC_INVALID_SIZE;
                                }
                                return 2;
                            }
                            else
                            {
                                length -= 2;
                            }
                        }
                    }

                    if(!this->write_u16((pilo::u16_t)length))
                    {
                        return MC_INVALID_SIZE;
                    }

                    size_t ret = this->write_bytes_array((char*)value, length, force);

                    if ( ret == MC_INVALID_SIZE)
                    {
                        return MC_INVALID_SIZE;
                    }

                    return ret + 2;
                }

                inline size_t write_string32(const char* value, size_t length, bool force)
                {
                    if (value == nullptr) return MC_INVALID_SIZE;                    

                    if(MC_INVALID_SIZE == length)
                    {
                        length = strlen(value);
                    }

                    if(length > 0xffffffff)
                    {
                        if (! force)
                        {
                            return MC_INVALID_SIZE;
                        }
                        else
                        {
                            length = 0xffffffff;
                        }
                    }

                    if (! check_buffer(length + 4))
                    {
                        if (! force)
                        {
                            return MC_INVALID_SIZE;
                        }
                        else //truncate
                        {
                            length = m_size - m_pos;
                            if (length < 4)
                            {
                                return MC_INVALID_SIZE;
                            }
                            else if (length == 4)
                            {
                                if(!this->write_u32((pilo::u32_t)0))
                                {
                                    return MC_INVALID_SIZE;
                                }
                                return 4;
                            }
                            else
                            {
                                length -= 4;
                            }
                        }
                    }

                    if(!this->write_u32((pilo::u32_t)length))
                    {
                        return MC_INVALID_SIZE;
                    }

                    size_t ret = this->write_bytes_array((char*)value, length, force);

                    if ( ret == MC_INVALID_SIZE)
                    {
                        return MC_INVALID_SIZE;
                    }

                    return ret + 4;
                }

                //stream operators
                memory_ostream_adapter& operator<<(const pilo::i64_t& v)
                {
                    if (!write_i64(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_ostream_adapter& operator<<(const pilo::i32_t& v)
                {
                    if (!write_i32(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_ostream_adapter& operator<<(const pilo::i16_t& v)
                {
                    if (!write_i16(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_ostream_adapter& operator<<(const pilo::i8_t& v)
                {
                    if (!write_i8(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_ostream_adapter& operator<<(const pilo::u64_t& v)
                {
                    if (!write_u64(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_ostream_adapter& operator<<(const pilo::u32_t& v)
                {
                    if (!write_u32(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_ostream_adapter& operator<<(const pilo::u16_t& v)
                {
                    if (!write_u16(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_ostream_adapter& operator<<(const pilo::u8_t& v)
                {
                    if (!write_u8(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_ostream_adapter& operator<<(const float& v)
                {
                    if (!write_float(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                memory_ostream_adapter& operator<<(const double& v)
                {
                    if (!write_double(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }

                memory_ostream_adapter& operator<<(const std::string& str)
                {
                    if (str.empty())
                    {
                        if (!write_u32(0))
                        {
                            M_ASSERT(false);
                        }
                    }
                    else
                    {
                        if (str.size() != write_string32(str.c_str(), (size_t)str.size(), true))
                        {
                            M_ASSERT(false);
                        }
                    }

                    return *this;
                }

                memory_ostream_adapter& operator<<(const char* str)
                {
                    if (str == nullptr || *str == 0)
                    {
                        if (!write_u32(0))
                        {
                            M_ASSERT(false);
                        }
                    }
                    else
                    {
                        pilo::u32_t len = (pilo::u32_t) pilo::core::string::string_util::length(str);
                        if (len != write_string32(str, (size_t)len, true))
                        {
                            M_ASSERT(false);
                        }
                    }

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