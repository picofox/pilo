#pragma once
#include "core/coredefs.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/threading//dummy_mutex.hpp"
#include "core/memory/object_pool.hpp"
#include "core/threading/nonrecursive_mutex.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template<size_t _Capacity, typename _Lock = pilo::core::threading::dummy_mutex>
            class fixed_buffer
            {
            public:
                typedef _Lock   lock_type;

                size_t      m_rp;
                size_t      m_wp;
                char        m_buffer[_Capacity];
                mutable lock_type   m_lock;

            public:
                fixed_buffer() : m_rp(0), m_wp(0)
                {

                }

            public:
                char& operator[] (size_t pos)
                {
                    M_ASSERT(pos < capacity());
                    return m_buffer[pos];
                }
                const char& operator[] (size_t pos) const
                {
                    M_ASSERT(pos < capacity());
                    return m_buffer[pos];
                }

                inline void set_rp(size_t rp)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_rp = rp;
                }

                inline void inc_rp(size_t delta)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_rp += delta;
                }

                inline void dec_rp(size_t delta)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_rp -= delta;
                }

                inline void set_wp(size_t wp)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_wp = wp;
                }

                inline void inc_wp(size_t delta)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_wp += delta;
                }
                inline void dec_wp(size_t delta)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_wp -= delta;
                }
                inline size_t rp()
                {
                    return m_rp;
                }
                inline size_t wp()
                {
                    return m_wp;
                }
                
                inline size_t capacity() const
                {
                    return _Capacity;
                }

                inline bool read_available() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return m_rp < m_wp;
                }

                inline size_t available_bytes_for_reading() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return m_wp - m_rp;
                }
                
                inline bool write_available() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);                    
                    return m_wp < _Capacity;
                }

                inline size_t available_bytes_for_writing() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return _Capacity - m_wp;
                }

                inline const char* data() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return &m_buffer[0];
                }
                
                inline const char* read_buffer() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return &m_buffer[m_rp];
                }

                inline char* write_buffer()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return &m_buffer[m_wp];
                }

                inline size_t peek(void* pData, size_t want)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    size_t ravail = this->available_bytes_for_reading();
                    if (ravail < 1) return 0;
                    if (ravail < want) want = ravail;
                    memcpy(pData, &m_buffer[m_rp], want);
                    return want;
                }

                inline size_t read(void* pData, size_t want)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    size_t ravail = this->available_bytes_for_reading();
                    if (ravail < 1) return 0;
                    if (ravail < want) want = ravail;
                    memcpy(pData, &m_buffer[m_rp], want);
                    m_rp += want;
                    return want;
                }

                inline size_t write(const void* pData, size_t len)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    size_t wavail = this->available_bytes_for_writing();
                    if (wavail < 1) return 0;
                    if (wavail < len) len = wavail;
                    memcpy(&m_buffer[m_wp], pData, len);
                    m_wp += len;
                    return len;
                }

                inline void reset()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    m_rp = m_wp = 0;
                }
            
                void compact()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    if (m_rp)
                    {
                        if (m_wp > m_rp)
                        {
                            memmove(&m_buffer[0], &m_buffer[m_rp], m_wp - m_rp);
                            m_wp = m_wp - m_rp;
                            m_rp = 0;
                        }
                        else
                        {
                            m_rp = m_wp = 0;
                        }
                    }
                }
            };

            template <size_t _Capacity, typename _Lock = pilo::core::threading::dummy_mutex>
            struct fixed_buffer_node : public fixed_buffer<_Capacity, _Lock>
            {
                fixed_buffer_node* m_next;
            };

            typedef fixed_buffer_node<2048> fixed_2kb_buffer_node;
            typedef pilo::core::memory::object_pool<fixed_2kb_buffer_node, 128, pilo::core::threading::nonrecursive_mutex> iobuffer_2kb_pool;
        }
    }
}