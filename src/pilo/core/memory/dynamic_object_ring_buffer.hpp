#pragma once
#include "core/coredefs.hpp"
#include "core/threading/mutex_locker.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {

            template <class ValueType, typename _Lock = pilo::core::threading::dummy_mutex> 
            class dynamic_object_ring_buffer
            {
            public:
                typedef _Lock   lock_type;

            public:
                ValueType*  m_buffer;
                size_t      m_buffer_capacity;
                size_t      m_data_begin_pos;
                size_t      m_data_length;
                lock_type   m_lock;


            public:
                dynamic_object_ring_buffer()
                {
                    m_buffer_capacity = 0;
                    m_data_begin_pos = 0;
                    m_data_length = 0;
                    m_buffer = nullptr;
                }
                dynamic_object_ring_buffer(size_t capacity)
                {
                    m_buffer_capacity = capacity;
                    m_data_begin_pos = 0;
                    m_data_length = 0;
                    m_buffer = new ValueType[m_buffer_capacity];
                    if (!m_buffer) throw EC_INSUFFICIENT_MEMORY;
                }
                ~dynamic_object_ring_buffer()
                {
                    if (m_buffer)
                    {
                        delete[] m_buffer;
                        m_buffer = nullptr;
                    }
                }
            public:
                inline bool set_capacity(size_t capacity)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    if (m_buffer_capacity > 0) return false;
                    m_buffer_capacity = capacity;
                    m_buffer = new ValueType[m_buffer_capacity];
                    return (m_buffer != nullptr);
                }

                inline size_t read_available() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    return m_data_length;
                }

                inline size_t write_available() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    return m_buffer_capacity - m_data_length;
                }

                inline bool read(ValueType* pOut, size_t length)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    if (this->read_available() < (size_t)length)
                    {
                        return false
                    }

                    if (m_data_begin_pos + length <= m_buffer_capacity)
                    {
                        size_t nBytes = length * sizeof(ValueType);
                        memcpy(pOut, m_buffer + m_data_begin_pos, nBytes);
                        m_data_begin_pos += length;
                        m_data_length -= length;
                        if (m_data_begin_pos == m_buffer_capacity)
                            m_data_begin_pos = 0;
                    }
                    else
                    {//·ÖÁ½¶Î¶Á
                        size_t num = m_buffer_capacity - m_data_begin_pos;
                        this->read(num, pOut);
                        return this->read(length - num, pOut + num);
                    }
                    return true;
                }
                inline bool write(const ValueType* pData, size_t length)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    if (this->write_available() < length)
                    {
                        return false;
                    }

                    size_t begPos = m_data_begin_pos + m_data_length;
                    if (begPos >= m_buffer_capacity) begPos -= m_buffer_capacity;
                    if (begPos + length <= m_buffer_capacity)
                    {
                        size_t nBytes = length * sizeof(ValueType);
                        memcpy(m_buffer + begPos, pData, nBytes);
                        m_data_length += length;
                    }
                    else
                    {
                        size_t num = m_buffer_capacity - begPos;
                        this->write(pData, num);
                        return this->write(pData + num, length - num);
                    }
                    return true;
                }            
            };

            
        }
    }
}