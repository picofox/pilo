#pragma once

#include "core/coredefs.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/threading//dummy_mutex.hpp"
#include "core/memory/object_pool.hpp"
#include "core/threading/nonrecursive_mutex.hpp"
#include "core/container/singly_linked_selflist.hpp"
#include "core/memory/fixed_buffer.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  size_t _BufferPieceSize = 2048, 
                        typename _Pool = pilo::core::memory::object_pool<fixed_buffer_node<_BufferPieceSize>, 128, pilo::core::threading::dummy_mutex>
                     > //default fixed_2kb_buffer_node
            class io_buffer
            {
            public:
                typedef _Pool                                                               buffer_pool_type;
                typedef fixed_buffer_node<_BufferPieceSize>                                 buffer_type;
                typedef pilo::core::container::singly_linked_selflist<buffer_type>          buffer_queue_type;


            protected:
                volatile size_t     m_bytes; //bytes number of current buffer
                size_t              m_max_pieces; //max memory pieces of buffer
                size_t              m_max_bytes;
                buffer_queue_type   m_queue;
                buffer_pool_type*   m_pool;

            protected:
                size_t _write(const void* pData, size_t len)
                {
                    buffer_type* pPiece = m_queue.rbegin();
                    M_ASSERT(pPiece);

                    size_t nBytes = pPiece->write(pData, len);
                    m_bytes += nBytes;
                    return nBytes;
                }
                
                size_t _read(void* pData, size_t len)
                {
                    M_ASSERT(!m_queue.empty());

                    buffer_type* pPiece = m_queue.begin();
                    size_t nBytes = pPiece->read(pData, len);
                    m_bytes -= nBytes;

                    M_ASSERT(m_bytes >= 0);

                    this->_compact();
                    return nBytes;
                }

                size_t _write_available_in_current_buffer_piece()
                {
                    buffer_type* pPiece = m_queue.rbegin();
                    if (!pPiece) return 0;
                    return pPiece->available_bytes_for_writing();
                }

                bool _inflate(bool without_max_size_check = false)
                {
                    if ((!without_max_size_check) && m_queue.size() >= m_max_pieces)
                    {
                        return false; // 达到最多内存块的数量
                    }
                    
                    buffer_type* pPiece = m_pool->allocate();
                    if (nullptr == pPiece)
                    {
                        return false; // 内存分不出来了 !!
                    }

                    pPiece->reset();
                    m_queue.push_back(pPiece);

                    return true;
                }

                void _compact()
                {
                    if (m_queue.empty()) return;

                    buffer_type* pPiece = m_queue.begin();
                    if (pPiece->available_bytes_for_reading())
                    {
                        return;
                    }

                    // no data available for read
                    if (m_queue.begin() == m_queue.rbegin())
                    {
                        pPiece->reset();
                        return;
                    }

                    if (pPiece->write_available())
                    {
                        return;
                    }

                    m_queue.pop_front();
                    m_pool->deallocate(pPiece);

                    return;
                }

            public:
                io_buffer(size_t pieces = 5)
                {
                    m_bytes = 0;
                    m_pool = nullptr;
                    m_max_pieces = pieces;
                    m_max_bytes = m_max_pieces * _BufferPieceSize;
                }
                ~io_buffer()
                {
                    this->reset();
                }

                void reset()
                {
                    while (!m_queue.empty())
                    {
                        buffer_type* pPiece = m_queue.pop_front();
                        m_pool->deallocate(pPiece);
                    }
                    m_bytes = 0;
                }

                size_t peek(void* pData, size_t len)
                {
                    size_t lenOrg = len;
                    char* p = (char*)pData;

                    if (m_queue.empty()) return 0;

                    buffer_type* piece = m_queue.begin();
                    while (piece != nullptr)
                    {
                        int nBytes = piece->peek(p, len);
                        piece = m_queue.next(piece);
                        len -= nBytes;
                        p += nBytes;
                        if (len < 1) break;
                    }

                    return lenOrg - len;
                }

                inline size_t size() const
                {
                    return m_bytes;
                }

                inline void set_pool(buffer_pool_type& pool)
                {
                    m_pool = &pool;
                }

                bool set_capacity(int kSize)
                {
                    M_ASSERT(m_queue.empty());

                    m_max_pieces = (kSize + 1) / 2;
                    M_ASSERT(m_max_pieces >= 1);
                    m_max_bytes = m_max_pieces * _BufferPieceSize;
                    return true;
                }

                

                // 这个函数一直都是在锁的范围内被调用的
                // 如果第一片已经读取了一部分的话，那么他的 rp 指针需要考虑
                // 怀疑与外网 2009.11.28 一次 GameServer 崩溃相关
                size_t write_available() const
                {
                    if (m_queue.empty())
                    {
                        return m_max_bytes - m_bytes;
                    }
                    else
                    {
                        const buffer_type* pPiece = m_queue.begin(); //m_queue[0];
                        return m_max_bytes - m_bytes - pPiece->m_rp;
                    }
                }

                // 一次最多从一个Piece中读取
                size_t read(void* pData, size_t len)
                {
                    size_t lenOrg = len;
                    char* p = (char*)pData;

                    while (len > 0)
                    {
                        if (m_queue.empty())
                        {
                            break;
                        }
                        size_t nBytes = this->_read(p, len);
                        if (nBytes == 0)
                        {
                            break;
                        }

                        len -= nBytes;
                        p += nBytes;
                    }

                    return lenOrg - len;
                }

                size_t write(const void* pData, size_t len, bool without_max_size_check = false)
                {
                    size_t lenOrg = len;
                    const char* p = (const char*)pData;

                    while (len > 0)
                    {
                        size_t nBytes = this->_write_available_in_current_buffer_piece();
                        if (nBytes > 0)
                        {
                            nBytes = this->_write(p, len);
                            p += nBytes;
                            len -= nBytes;
                        }
                        else
                        {
                            if (!this->_inflate(without_max_size_check))
                            {
                                break;
                            }
                        }
                    }
                    return lenOrg - len;
                }                              
            };
        }
    }
}