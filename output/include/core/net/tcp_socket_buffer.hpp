#pragma once

#include "basic_socket.hpp"
#include "core/memory/fixed_buffer.hpp"
#include "core/memory/io_buffer.hpp"
#include "./ipv4_address.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            template<size_t _SendBufferSize = 1280, size_t _RecvBufferSize = 2048, size_t _IOBufferSize = 2048>
            class tcp_socket_buffer
            {
            public:
                tcp_socket_buffer()
                {
                    memset(&m_peer_address, 0, sizeof(m_peer_address));
                    m_current_bytes_sent = 0;
                }

                ~tcp_socket_buffer()
                {

                }

                void reset()
                {
                    m_send_buffer.reset();
                    m_recv_buffer.reset();
                    m_outgoing_io_buffer.reset();

                    m_current_bytes_sent = 0;
                    memset(&m_peer_address, 0, sizeof(m_peer_address));
                }

                void set_send_buffer_size(size_t sz)
                {
                    m_outgoing_io_buffer.set_capacity(sz);
                }
                void set_peer_address(pilo::u32_t ip, pilo::u16_t port)
                {
                    m_peer_address.m_ip = ip;
                    m_peer_address.m_port = port;
                }
                const pilo::core::net::ipv4_address& peer_address() const
                {
                    return m_peer_address;
                }

                virtual bool has_pending_data() const
                {
                    return m_outgoing_io_buffer.size() || (m_send_buffer.read_available());
                }

                //TOCHECK check with below method, can it be abstract
                virtual pilo::core::net::basic_socket* socket() { return  nullptr; }

                size_t load_send_buffer()
                {
                    size_t readed = 0;
                    pilo::core::net::basic_socket* pSock = this->socket();

                    m_send_buffer.reset();

                    size_t len = (size_t) m_send_buffer.available_bytes_for_writing();
                    char* p = m_send_buffer.write_buffer();

                    {
                        pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex>   locker(pSock->m_lock);
                        readed = m_outgoing_io_buffer.read(p, len); //copy to m_outgoing bubber
                    }
   

                    m_send_buffer.set_wp(readed);
                    return readed;
                }

                size_t load_send_buffer(size_t bytes)
                {
                    size_t readed = 0;
                    pilo::core::net::basic_socket* pSock = this->socket();

                    m_send_buffer.reset();
                    if (bytes == 0) return 0;

                    size_t len = (size_t)m_send_buffer.available_bytes_for_writing();
                    char* p = m_send_buffer.write_buffer();

                    if (len > bytes) len = bytes;

                    {
                        pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex>   locker(pSock->m_lock);
                        readed = m_outgoing_io_buffer.read(p, len);
                    }

                    m_send_buffer.set_wp(readed);
                    return readed;
                }


            public:
                pilo::core::memory::fixed_buffer<_SendBufferSize> m_send_buffer; // send() 时使用的缓冲区
                pilo::core::memory::fixed_buffer<_RecvBufferSize> m_recv_buffer;			// recv() 时使用的缓冲区
                pilo::core::memory::io_buffer<_IOBufferSize, pilo::core::memory::iobuffer_2kb_pool>    m_outgoing_io_buffer;
                pilo::core::net::ipv4_address           m_peer_address;
                size_t m_current_bytes_sent;						// 本次暂停后，已发送字节数
            };

        }
    }
}