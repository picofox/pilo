#pragma once

#include "core/coredefs.hpp"
#include "core/datetime/datetime.hpp"

#if !defined(WIN32) && !defined(WINDOWS)

#include "epoll_socket.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            template<size_t _SendBufferSize = 10240, size_t _RecvBufferSize = 10240, size_t _IOBufferSize = 2048>
            class epoll_udp_socket : public epoll_socket
            {
            public:
                epoll_udp_socket();
                virtual ~epoll_udp_socket() {}

            public:
            void set_send_buffer_size(size_t buffer_size) { m_outgoing_io_buffer.set_capacity(buffer_size); }
                bool attach(os_socket_fd_t sock);
                bool listen(::pilo::u16_t port);
                bool listen(::pilo::u32_t  ip, ::pilo::u16_t port);

            public:
                virtual EnumSocketType socket_type() const { return basic_socket::eST_UDP; }

                virtual size_t send(const void* packet, size_t len) { M_UNUSED(packet); M_UNUSED(len); M_ASSERT(false); return 0; } // Tcp使用的发送函数
                virtual size_t send_to(const void* packet, size_t len, const ::pilo::core::net::ipv4_address& address); // UDP使用的发送函数
                virtual size_t send_ndelay(const void* packet, size_t len) { M_UNUSED(packet); M_UNUSED(len); M_ASSERT(false); return 0; }
                virtual size_t send_to_ndelay(const void* packet, size_t len, const ::pilo::core::net::ipv4_address& address);
                virtual void reset(); // 重置Socket对象的状态

                virtual ::pilo::core::string::fixed_astring<256> to_string() const; // Socket对象的字符串表示  

            public:
                // 从35000开始自动选择一个端口
                bool auto_listening_from_port(::pilo::u16_t port = 35000);
                virtual void read();
                virtual void write();
                virtual void on_start_working();

            public:
                ::pilo::core::net::ipv4_address m_peer_address;    // m_sendBuff中当前数据发送的目标
                ::pilo::core::memory::fixed_buffer<_SendBufferSize> m_send_buffer; // send() 时使用的缓冲区
                ::pilo::core::memory::fixed_buffer<_RecvBufferSize> m_recv_buffer;			// recv() 时使用的缓冲区
                ::pilo::core::memory::io_buffer<_IOBufferSize, ::pilo::core::memory::iobuffer_2kb_pool>    m_outgoing_io_buffer;
            };

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            void pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::on_start_working()
            {
                m_outgoing_io_buffer.reset();
                m_send_buffer.reset();
                m_recv_buffer.reset();
                epoll_socket::on_start_working();
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            void pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::write()
            {
                if(m_sock == MC_INVALID_SOCK_FD) return;

                if(!m_writable) return;
                if(m_send_paused) return;

                if(m_recv_buffer.available_bytes_for_reading() < 1 && m_outgoing_io_buffer.size() < 1) return;

                if(m_in_writing) return;

                {
                    ::pilo::core::threading::mutex_locker<::pilo::core::threading::nonrecursive_mutex> locker;
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

                sockaddr_in peerAddr;
                socklen_t addrLen = sizeof(peerAddr);

                while(true)
                {
                    int len = m_send_buffer.available_bytes_for_reading();
                    const char* pData = m_send_buffer.read_buffer();

                    if(len > 0)
                    {
                        peerAddr = ipv4_address::to_sockaddr(m_peer_address);
                        int nBytes = sendto(m_sock, pData, len, MSG_NOSIGNAL, (const sockaddr*)&peerAddr, addrLen);
                        if(nBytes > 0)
                        {
                            m_bytes_sent += nBytes;
                            m_latest_send_tick = PiloGetTickCount64();

                            M_ASSERT(nBytes == len);
                            m_send_buffer.reset();
                        }
                        else
                        {
                            M_ASSERT(nBytes == -1);
                            ::pilo::os_error_number_t errCode = ::pilo::get_last_os_error_number();

                            // The socket is marked non-blocking and the requested operation would block.
                            if(errCode == EAGAIN || errCode == EWOULDBLOCK)
                            {
                                //TODO log error
                                //Log(LOG_WARN, "warn: %s would block", this->ToString().c_str());
                                //m_lastBlockTick = GetTickCount();
                                //m_writable = FALSE;

                                this->disable_writable();
                                this->on_would_block();

                                break;
                            }

                            // A signal occurred before any data was transmitted.
                            if(errCode == EINTR)
                            {
                                //TODO log error
                                //Log(LOG_WARN, "warn: %s, %s", this->ToString().c_str(), strerror_r2(errCode).c_str());
                                continue;
                            }

                            this->on_socket_error(errCode);
                            m_send_buffer.reset();
                            break;
                        }

                        m_send_buffer.reset();
                    }
                    else
                    {
                        udp_packet_header packetHead;

                        m_send_buffer.reset();

                        {
                            ::pilo::core::threading::mutex_locker<::pilo::core::threading::nonrecursive_mutex> locker;

                            int nBytes = m_outgoing_io_buffer.read(&packetHead, sizeof(udp_packet_header));
                            if(nBytes < sizeof(udp_packet_header))
                            {
                                M_ASSERT(nBytes == 0);
                                break;
                            }

                            int packetLength = packetHead.m_length;
                            memcpy(&m_peer_address, &packetHead.m_remote_socket_address, sizeof(ipv4_address));

                            size_t len = m_send_buffer.available_bytes_for_writing();
                            char* pBuff = m_send_buffer.write_buffer();

                            nBytes = m_outgoing_io_buffer.read(pBuff, packetLength);
                            M_ASSERT(nBytes == packetLength);
                            m_send_buffer.m_wp = packetLength;
                        }
                    }
                }

                m_in_writing = false;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            void pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::read()
            {
                socklen_t addrLen;
                sockaddr_in peerAddr;
                ipv4_address sockAddr;

                while(true)
                {
                    addrLen = sizeof(peerAddr);

                    size_t len = m_recv_buffer.available_bytes_for_writing();
                    char* pBuff = m_recv_buffer.write_buffer();
                    size_t readBytes = recvfrom(m_sock, pBuff, len, 0, (sockaddr*)&peerAddr, &addrLen);

                    sockAddr = ipv4_address::from_sockaddr(peerAddr);

                    if(readBytes > 0)
                    {
                        m_bytes_received += readBytes;
                        m_latest_recv_tick = PiloGetTickCount64();

                        m_recv_buffer.m_wp += readBytes;

                        const void* pData = m_recv_buffer.read_buffer();
                        size_t dataLen = m_recv_buffer.available_bytes_for_reading();

                        size_t nBytes = this->on_receiving_data(pData, dataLen, &sockAddr);
                        if(nBytes > 0)
                        {
                            m_recv_buffer.m_rp += nBytes;
                            m_recv_buffer.compact();
                        }
                        else if(nBytes < 0)
                        {
                            this->on_invalid_packet(&sockAddr, (EnumErrorPacketReason)nBytes);
                            m_recv_buffer.reset();
                            //return; // 需要继续读取
                        }
                    }
                    else if(readBytes == 0)
                    {
                        //TODO log error  
                        //Log(LOG_WARN, "warn: %s peer closed, close it", this->ToString().c_str());
                        //this->Close();
                        //return;
                    }
                    else
                    {
                        ::pilo::os_error_number_t errCode = ::pilo::get_last_os_error_number();

                        // The  socket  is  marked non-blocking and the receive operation would block, 
                        // or a receive timeout had been set and the timeout expired before data was received.
                        if(errCode == EAGAIN) return;

                        // The receive was interrupted by delivery of a signal before any data were available.
                        if(errCode == EINTR)
                        {
                            //TODO log error
                            //Log(LOG_WARN, "warn: %s, %s", this->ToString().c_str(), strerror_r2(errCode).c_str());
                            return;
                        }

                        this->on_socket_error(errCode);
                        m_recv_buffer.reset();
                        return;
                    }
                }
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            pilo::core::string::fixed_astring<256> pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::to_string() const
            {
                char cTmp[128];
                snprintf(cTmp, sizeof(cTmp), "UDP %u", m_sock != MC_INVALID_SOCK_FD ? m_sock : m_sock_dup);
                return cTmp;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            void pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::reset()
            {
                m_peer_address.m_ip = 0;
                m_peer_address.m_port = 0;
                m_outgoing_io_buffer.reset();
                m_send_buffer.reset();
                m_recv_buffer.reset();
                epoll_socket::reset();
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            size_t pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::send_to_ndelay(const void* packet, size_t len, const ::pilo::core::net::ipv4_address& address)
            {
                if(!this->send_to(packet, len, address)) return 0;
                this->write();
                return len;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            size_t pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::send_to(const void* packet, size_t len, const ::pilo::core::net::ipv4_address& address)
            {
                if(m_sock == MC_INVALID_SOCK_FD || !this->m_ready_for_io) return 0;

                bool bBufferFull = false;
                size_t wanted = sizeof(udp_packet_header) + len;

                udp_packet_header packetHead;
                memcpy(&packetHead.m_remote_socket_address, &address, sizeof(ipv4_address));
                packetHead.m_length = len;

                {
                    ::pilo::core::threading::mutex_locker<::pilo::core::threading::nonrecursive_mutex> locker;
                    if(m_outgoing_io_buffer.write_available() < wanted)
                    {
                        bBufferFull = true;
                    }
                    else
                    {
                        int nBytes = m_outgoing_io_buffer.write(&packetHead, sizeof(udp_packet_header));
                        M_ASSERT(nBytes == sizeof(udp_packet_header));
                        nBytes = m_outgoing_io_buffer.write(packet, len);
                        M_ASSERT(nBytes == len);
                    }                    
                }

                if(bBufferFull)
                {
                    //TODO log error
                    //Log(LOG_WARN, "warn: %s send buffer full, close it", this->ToString().c_str());
                    //this->Close();
                    return 0;
                }

                //this->Write();
                return len;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::epoll_udp_socket()
            {
                m_peer_address.m_ip = 0;
                m_peer_address.m_port = 0;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::auto_listening_from_port(::pilo::u16_t port /*= 35000*/)
            {
/*                ::pilo::u16_t port = 35000;*/
                while(true)
                {
                    if(this->listen(port)) return true;
                    port += 1;
                }
                M_ASSERT(false);
                return false;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::listen(::pilo::u32_t ip, ::pilo::u16_t port)
            {
                this->_close();

                ::pilo::os_socket_fd_t sock = basic_socket::create_udp_socket();
                if(sock == MC_INVALID_SOCK_FD)
                {
                    ::pilo::os_error_number_t errCode = ::pilo::get_last_os_error_number();

                    //TODO log error
                    //Log(LOG_ERROR, "error: create udp socket, %s", strerror_r2(errCode).c_str());
                    return false;
                }

                ipv4_address addr = {ip, port};
                sockaddr_in sockAddr = ipv4_address::to_sockaddr(addr);
                int nameLength = sizeof(sockAddr);

                int nRet = bind(sock, (sockaddr*)&sockAddr, nameLength);
                if(nRet)
                {	// ??????
//                     DWORD errCode = GetLastError();
// 
//                     Log(LOG_ERROR, "error: bind udp socket at %s, %s",
//                         KSocketAddress::ToString(addr).c_str(),
//                         strerror_r2(errCode).c_str());
                    ::pilo::os_error_number_t errCode = ::pilo::get_last_os_error_number();
                    ::close(sock);
                    return false;
                }

                m_sock = sock;
                return true;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::listen(::pilo::u16_t port)
            {
                return this->listen(INADDR_ANY, port);
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::epoll_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::attach(os_socket_fd_t sock)
            {
                M_ASSERT(sock != MC_INVALID_SOCK_FD);
                m_sock = sock;
                return true;
            } 
        }
    }
}

#endif