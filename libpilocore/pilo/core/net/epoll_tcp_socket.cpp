#include "core/coredefs.hpp"

#if !defined(WIN32) && !defined(WINDOWS)

#include "epoll_tcp_socket.hpp"
#include "core/io/format_output.hpp"
#include "core/datetime/datetime.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {

            epoll_tcp_socket::epoll_tcp_socket()
            {
                m_peer_address.m_ip = 0;
                m_peer_address.m_port = 0;
            }

            epoll_tcp_socket::~epoll_tcp_socket()
            {

            }

            void epoll_tcp_socket::reset()
            {
                tcp_socket_buffer<>::reset();
                epoll_socket::reset();                
            }

            size_t epoll_tcp_socket::send(const void* packet, size_t len)
            {
                if(m_sock == MC_INVALID_SOCK_FD || !  m_ready_for_io)
                {
                    return 0;
                }

                size_t nbs = 0;
                {
                    pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex>   locker(m_lock);
                    nbs = m_outgoing_io_buffer.write(packet, len);
                }

                if(nbs < len)
                {// Socket的IOBuffer满了，关闭连接
                    //TODO log error
                    //Log(LOG_WARN, "warn: %s send io buffer full, close it", this->ToString().c_str());
                    this->close();
                    return 0;
                }

                return nbs;
            }

            size_t epoll_tcp_socket::send_to(const void* packet, size_t len, const pilo::core::net::ipv4_address& address)
            {
                M_ASSERT(false);
                return 0;
            }

            size_t epoll_tcp_socket::send_ndelay(const void* packet, size_t len)
            {
                if(!this->send(packet, len)) return 0;
                return len;
            }

            void epoll_tcp_socket::read()
            {
                while(true)
                {
                    size_t len = m_recv_buffer.available_bytes_for_writing();
                    char* buff = m_recv_buffer.write_buffer();
                    M_ASSERT(len > 0);

                    ssize_t readBytes = recv(m_sock, buff, len, 0);

                    if(readBytes > 0)	// 收到了数据，通知
                    {
                        m_bytes_received += readBytes;
                        m_latest_recv_tick = PiloGetTickCount64();

                        m_recv_buffer.m_wp += readBytes;

                        const void* pData = m_recv_buffer.read_buffer();
                        size_t dataLen = m_recv_buffer.available_bytes_for_reading();

                        
                        int nBytes = this->on_receiving_data(pData, dataLen, &m_peer_address);
                        if(nBytes > 0)
                        {
                            m_recv_buffer.m_rp += nBytes;
                            m_recv_buffer.compact();
                        }
                        else if (nBytes < 0)
                        {
                            this->on_invalid_packet(&m_peer_address, (EnumErrorPacketReason)nBytes);
                            m_recv_buffer.reset();
                            return;
                        }
                    }
                    else if (readBytes == 0) // 对端关闭了这个连接
                    {
                        //TODO log error
                        //Log(LOG_WARN, "warn: %s, peer closed, close it", this->ToString().c_str());
                        this->close();
                        return;
                    }
                    else
                    {
                        ::pilo::os_error_number_t errCode = ::pilo::get_last_os_error_number();

                        // The receive was interrupted by delivery of a signal before any data were available.
                        if (errCode == EINTR)
                        {
                            //TODO log error
                            //Log(LOG_WARN, "warn: %s, %s", this->ToString().c_str(), strerror_r2(errCode).c_str());
                            continue;
                        }

                        // The  socket  is  marked non-blocking and the receive operation would block,
                        // or a receive timeout had been
                        // set and the timeout expired before data was received.
                        if (errCode == EAGAIN) return;

                        this->on_socket_error(errCode);
                        return;
                    }
                }
            }

            void epoll_tcp_socket::write()
            {
                if(m_sock == MC_INVALID_SOCK_FD || !  m_ready_for_io)
                {
                    return;
                }

                if(!m_writable) return;
                if(m_send_paused) return;

                // 现在没有数据等待发送
                if(!this->has_pending_data()) return;

                // 别的线程正在发送
                if(m_in_writing) return;

                {
                    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> locker(m_lock);
                    if(m_in_writing)
                    {
                        return;
                    }
                    m_in_writing = true;
                }


                if(!m_writable)
                {
                    m_in_writing = false;
                    return;
                }

                while(true)
                {
                    size_t len = m_send_buffer.available_bytes_for_reading();
                    const char* pBuff = m_send_buffer.read_buffer();

                    if(len > 0)
                    {
                        //int wantSend = len < 256 ? len : 256;
                        int nBytes = ::send(m_sock, pBuff, len, MSG_NOSIGNAL);

                        if(nBytes == -1)
                        {
                            ::pilo::os_error_number_t errCode = ::pilo::get_last_os_error_number();

                            // A signal occurred before any data was transmitted.
                            if(errCode == EINTR)
                            {
                                //TODO log error
                                //Log(LOG_WARN, "warn: %s, %s", this->ToString().c_str(), strerror_r2(errCode).c_str());
                                continue;
                            }

                            // The socket is marked non-blocking and the requested operation would block.
                            if(errCode == EAGAIN || errCode == EWOULDBLOCK)
                            {
                                if(m_debug_state)
                                {
                                    //TODO log error
                                    //Log(LOG_WARN, "warn: %s would block, send buf:%d", this->ToString().c_str(), m_sendIOBuffer.Size());
                                }

                                this->on_would_block();
                                this->disable_writable();

                                break;
                            }

                            this->on_socket_error(errCode);
                            break;
                        }

                        m_bytes_sent += nBytes;
                        m_latest_send_tick = PiloGetTickCount64();

                        // 更改写缓冲区读指针
                        m_send_buffer.m_rp += nBytes;
                        m_send_buffer.compact();
                    }
                    else
                    {
                        if(this->load_send_buffer() < 1)
                            break;
                    }
                }

                m_in_writing = false;
            }

            void epoll_tcp_socket::on_start_working()
            {
                m_outgoing_io_buffer.reset();
                m_send_buffer.reset();
                m_recv_buffer.reset();
                epoll_socket::on_start_working();
            }

        }
    }
}

#endif