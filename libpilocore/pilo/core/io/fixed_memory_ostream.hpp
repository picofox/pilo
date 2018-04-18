#pragma once

#include "../../core/coredefs.hpp"
#include "../../core/io/ostream_interface.hpp"

namespace pilo
{
    namespace core
    {
        namespace io
        {
            template<size_t _Capacity>
            class fixed_memory_ostream : public ostream_interface
            {
            public:
                fixed_memory_ostream() : m_pos(0) {}
                virtual ~fixed_memory_ostream() {}


            public: //base method
                inline virtual size_t write(const void* value, size_t length)
                {
                    return (this->write_bytes_array(value, length, false));
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

			public: //derivative methods write
                inline size_t write_bytes_array(const void* value, size_t length,bool force)
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
                            length = _Capacity - m_pos;
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

                inline size_t write_string8(const char* value, size_t length , bool force)
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
                            length = _Capacity - m_pos;
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

                    if(length > 0xFFFF)
                    {
                        if (! force)
                        {
                            return MC_INVALID_SIZE;
                        }
                        else
                        {
                            length = 0xFFFF;
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
                            length = _Capacity - m_pos;
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

                    if(length > 0xFFFFFFFF)
                    {
                        if (! force)
                        {
                            return MC_INVALID_SIZE;
                        }
                        else
                        {
                            length = 0xFFFFFFFF;
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
                            length = _Capacity - m_pos;
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

                    if (ret == MC_INVALID_SIZE)
                    {
                        return MC_INVALID_SIZE;
                    }

                    return ret + 4;
                }


                //stream operators
                fixed_memory_ostream& operator<<(const pilo::i64_t& v)
                {
                    if (!write_i64(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_ostream& operator<<(const pilo::i32_t& v)
                {
                    if (!write_i32(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_ostream& operator<<(const pilo::i16_t& v)
                {
                    if (!write_i16(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_ostream& operator<<(const pilo::i8_t& v)
                {
                    if (!write_i8(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_ostream& operator<<(const pilo::u64_t& v)
                {
                    if (!write_u64(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_ostream& operator<<(const pilo::u32_t& v)
                {
                    if (!write_u32(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_ostream& operator<<(const pilo::u16_t& v)
                {
                    if (!write_u16(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_ostream& operator<<(const pilo::u8_t& v)
                {
                    if (!write_u8(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_ostream& operator<<(const float& v)
                {
                    if (!write_float(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }
                fixed_memory_ostream& operator<<(const double& v)
                {
                    if (!write_double(v))
                    {
                        M_ASSERT(false);
                    }
                    return *this;
                }

                fixed_memory_ostream& operator<<(const std::string& str)
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
                        if (!write_string32(str.c_str(), (size_t)str.size(), true))
                        {
                            M_ASSERT(false);
                        }
                    }

                    return *this;
                }

                fixed_memory_ostream& operator<<(const char* str)
                {
                    if (str == nullptr || *str == 0)
                    {
                        if (! write_u32(0))
                        {
                            M_ASSERT(false);
                        }
                    }
                    else
                    {
                        pilo::u32_t len = (pilo::u32_t) pilo::core::string::string_util::length(str);
                        if (! write_string32(str, (size_t)len, true))
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