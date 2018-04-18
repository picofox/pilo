#pragma once

#include "core/coredefs.hpp"
#if defined(WIN32) || defined(WINDOWS)
#include "./iocp_socket.hpp"
#include "core/io/format_output.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            template<size_t _SendBufferSize = 10240, size_t _RecvBufferSize = 10240, size_t _IOBufferSize = 2048>
            class iocp_udp_socket : public iocp_socket
            {
            public:
                iocp_udp_socket();
                virtual ~iocp_udp_socket();

            public:
                void set_send_buffer_size(size_t buffer_size);
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
                virtual bool on_closed_state(); // SOCKET在关闭状态下，每隔一定时间调用一次  主动连接的SOCKET可以在这个函数里面执行重连的动作 SockServer会根据这个函数的返回值，决定是否销毁这个对象(TRUE保留，FALSE销毁)
                virtual ::pilo::core::string::fixed_astring<256> to_string() const; // Socket对象的字符串表示                

            public:
                void on_start_working();

                // 从35000开始自动选择一个端口
                bool auto_listening_from_port(::pilo::u16_t port = 35000);

                // 发起各种类型的IO操作
                // 由程序逻辑保证实际的操作不会并发
                virtual bool initiate_send_operation();
                virtual bool initiate_receive_operation();
                virtual bool initiate_accept_operation();

                // 收到了IOCP的通知
                virtual bool on_success_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes);
                virtual bool on_fail_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes, ::pilo::error_number_t errCode);
                virtual bool on_close_operation(iocp_scoket_operation* pSockOperation);                

            public:
                // 向IOCP发起发送和接收操作的请求
                bool _initiate_send_operation();
                bool _initiate_receive_operation();

            public:
                ::pilo::core::memory::fixed_buffer<_SendBufferSize> m_send_buffer; // send() 时使用的缓冲区
                ::pilo::core::memory::fixed_buffer<_RecvBufferSize> m_recv_buffer;			// recv() 时使用的缓冲区
                ::pilo::core::memory::io_buffer<_IOBufferSize, ::pilo::core::memory::iobuffer_2kb_pool>    m_outgoing_io_buffer;

                iocp_udp_socket_send_operation m_send_op;		// 当前的发送操作
                iocp_udp_socket_receive_operation m_recv_op;    // 当前的接收操作
            };

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::iocp_udp_socket()
            {
                iocp_udp_socket::reset();
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::~iocp_udp_socket()
            {

            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            void pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::set_send_buffer_size(size_t buffer_size)
            {
                m_outgoing_io_buffer.set_capacity(buffer_size);
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::attach(os_socket_fd_t sock)
            {
                M_ASSERT(m_sock != INVALID_SOCKET);
                m_sock = sock;
                return true;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::listen(::pilo::u16_t port)
            {
                return this->listen(INADDR_ANY, port);
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::listen(::pilo::u32_t ip, ::pilo::u16_t port)
            {
                this->_close_nolock(); //method of basic_socket

                os_socket_fd_t sock = basic_socket::create_udp_socket();
                if (sock == INVALID_SOCKET)
                {
                    //TODO log this error
                    //                     DWORD errCode = GetLastError();
                    //                     Log(LOG_ERROR, "error: create udp socket, %s", strerror_r2(errCode).c_str());
                    return false;
                }

                ipv4_address addr = { ip, port };
                sockaddr_in sockAddr = ipv4_address::to_sockaddr(addr);
                int nameLength = sizeof(sockAddr);

                int nRet = bind(sock, (sockaddr*)&sockAddr, nameLength);
                if (nRet) // 出错了
                {
                    //TODO log this error
                    //                     DWORD errCode = GetLastError();
                    // 
                    //                     Log(LOG_ERROR, "error: bind udp socket at %s, %s",
                    //                         KSocketAddress::ToString(addr).c_str(),
                    //                         strerror_r2(errCode).c_str());

                    ::pilo::core::net::close_socket(sock);
                    return false;
                }

                m_sock = sock;
                return true;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            size_t pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::send_to(const void* packet, size_t len, const ::pilo::core::net::ipv4_address& address)
            {
                if (m_sock == INVALID_SOCKET || !m_ready_for_io)
                {
                    return 0;
                }

                size_t nBytes;
                bool bBufferFull = false;

                // 发送缓冲区中先写入目的地址，然后是包数据
                udp_packet_header packetHead;
                memcpy(&packetHead.m_remote_socket_address, &address, sizeof(ipv4_address));
                packetHead.m_length = (::pilo::u16_t) len;

                {
                    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex>   locker(m_lock);
                    nBytes = m_outgoing_io_buffer.write(&packetHead, sizeof(udp_packet_header));
                    if (nBytes == sizeof(udp_packet_header))
                    {
                        nBytes = m_outgoing_io_buffer.write(packet, len);
                        if (nBytes < len) bBufferFull = true;
                    }
                    else
                    {
                        bBufferFull = true;
                    }
                }

                if (bBufferFull) // 发送缓冲区满了，关闭这个链接，清空发送缓冲区
                {
                    //TODO log buffer full
                    //Log(LOG_WARN, "warn: %s send buffer full, close it", this->ToString().c_str());

                    {
                        ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex>   locker(m_lock);
                        m_outgoing_io_buffer.reset();
                    }

                    this->close();
                    return 0;
                }

                // 不发起发送操作
                //this->InitiateSendOperation();
                return len;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            size_t pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::send_to_ndelay(const void* packet, size_t len, const ::pilo::core::net::ipv4_address& address)
            {
                if (m_sock == INVALID_SOCKET || !m_ready_for_io)
                {
                    return 0;
                }

                size_t nBytes;
                bool bBufferFull = false;

                // 发送缓冲区中先写入目的地址，然后是包数据
                udp_packet_header packetHead;
                memcpy(&packetHead.m_remote_socket_address, &address, sizeof(ipv4_address));
                packetHead.m_length = (::pilo::u16_t) len;

                {
                    pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex>   locker(m_lock);
                    nBytes = m_outgoing_io_buffer.write(&packetHead, sizeof(udp_packet_header));
                    if (nBytes == sizeof(udp_packet_header))
                    {
                        nBytes = m_outgoing_io_buffer.write(packet, len);
                        if (nBytes < len) bBufferFull = true;
                    }
                    else
                    {
                        bBufferFull = true;
                    }
                }

                if (bBufferFull) // 发送缓冲区满了，关闭这个链接，清空发送缓冲区
                {
                    //TODO log buffer full
                    //Log(LOG_WARN, "warn: %s send buffer full, close it", this->ToString().c_str());

                    {
                        ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex>   locker(m_lock);
                        m_outgoing_io_buffer.reset();
                    }

                    this->close();
                    return 0;
                }

                // 发起发送操作
                this->initiate_send_operation();
                return len;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            void pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::reset()
            {
                m_send_buffer.reset();
                m_recv_buffer.reset();
                m_outgoing_io_buffer.reset();
                memset(&m_send_op, 0x00, sizeof(m_send_op));
                memset(&m_recv_op, 0x00, sizeof(m_recv_op));
                basic_socket::reset();
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::on_closed_state()
            {
                M_ASSERT(m_sock == INVALID_SOCKET);

                if (iocp_socket::on_closed_state())
                {
                    return true;
                }

                if (m_send_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation) return true;
                if (m_recv_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation) return true;

                return false;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            pilo::core::string::fixed_astring<256> pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::to_string() const
            {
                char cTmp[128];
                ::pilo::core::io::string_format_output(cTmp, sizeof(cTmp), "UDP %u", m_sock != INVALID_SOCKET ? m_sock : m_sock_dup);
                return cTmp;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            void pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::on_start_working()
            {
                m_outgoing_io_buffer.reset();
                m_send_buffer.reset();
                m_recv_buffer.reset();
                memset(&m_send_op, 0x00, sizeof(m_send_op));
                memset(&m_recv_op, 0x00, sizeof(m_recv_op));
                iocp_socket::on_start_working();
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::auto_listening_from_port(::pilo::u16_t port /*= 35000*/)
            {
                while (true)
                {
                    if (this->listen(port))
                    {
                        return true;
                    }
                    port += 1;
                }
                M_ASSERT(false);
                return false;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::initiate_send_operation()
            {
                if (m_sock == INVALID_SOCKET)
                {
                    return false;
                }

                if (m_outgoing_io_buffer.size() < 1)
                {
                    return true;
                }

                if (m_send_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                {
                    return true;
                }

                {
                    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex>   locker(m_lock);
                    if (m_send_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                    {
                        return true;
                    }
                    m_send_op.m_operation_type = iocp_scoket_operation::eISOT_SendOperation;
                }

                return this->_initiate_send_operation();
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::initiate_receive_operation()
            {
                if (m_sock == INVALID_SOCKET)
                {
                    return false;
                }

                if (m_recv_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                {
                    return true;
                }

                {
                    ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex>   locker(m_lock);
                    if (m_recv_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                    {
                        return true;
                    }
                    m_recv_op.m_operation_type = iocp_scoket_operation::eISOT_ReceiveOperation;
                }

                return this->_initiate_receive_operation();
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::initiate_accept_operation()
            {
                M_ASSERT(false);
                return false;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::on_success_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes)
            {
                if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_SendOperation)
                {
                    M_ASSERT(&m_send_op == pSockOperation);

                    m_bytes_sent += numberOfBytes;
                    m_latest_send_tick = ::GetTickCount64();

                    // UDP 是完整包发送的，不存在发送部分包的情况
                    const void* pData = m_send_buffer.read_buffer();
                    size_t dataLen = m_send_buffer.available_bytes_for_reading();
                    M_ASSERT(numberOfBytes == dataLen);

                    ipv4_address peerAddr = ipv4_address::from_sockaddr(m_send_op.m_remote_socket_address);
                    this->on_sending_data(pData, dataLen, &peerAddr);
                    m_send_buffer.reset();

                    return this->_initiate_send_operation();
                }
                else if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_ReceiveOperation)
                {
                    M_ASSERT(&m_recv_op == pSockOperation);

                    m_bytes_received += numberOfBytes;
                    m_latest_recv_tick = GetTickCount64();

                    ipv4_address peerAddr = ipv4_address::from_sockaddr(m_recv_op.m_originated_socket_address);
                    m_recv_buffer.inc_wp((size_t)(short)numberOfBytes);

                    // UDP 是完整包接收的，不存在接收部分包的情况
                    const void* pData = m_recv_buffer.read_buffer();
                    size_t dataLen = m_recv_buffer.available_bytes_for_reading();

                    size_t nBytes = this->on_receiving_data(pData, dataLen, &peerAddr);
                    if (nBytes > 0 && nBytes != MC_INVALID_SIZE)
                    {
                        m_recv_buffer.inc_rp(nBytes);
                        m_recv_buffer.reset();			// UDP一次只接收一个包
                    }
                    else if (nBytes < 0)
                    {
                        this->on_invalid_packet(&peerAddr, (EnumErrorPacketReason)nBytes);

                        // 重置接收缓冲区，UDP可能不会因为一个错误包而关闭连接
                        m_recv_buffer.reset();

                        // 重新发起接收操作
                        this->_initiate_receive_operation();
                        return true;
                    }

                    // 重新发起读操作
                    this->_initiate_receive_operation();
                    return true;
                }

                // 不可达
                M_ASSERT(false);
                return false;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::on_close_operation(iocp_scoket_operation* pSockOperation)
            {
                if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_SendOperation)
                {
                    if (&m_send_op == pSockOperation) // 是我的操作
                    {
                        this->close();
                        m_send_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    }
                    return true;
                }
                else if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_ReceiveOperation)
                {
                    if (&m_recv_op == pSockOperation)// 是我的操作
                    {
                        this->close();
                        m_recv_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    }
                    return true;
                }
                else
                {
                    M_ASSERT(false);
                }
                return true;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::on_fail_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes, pilo::error_number_t errCode)
            {
                M_UNUSED(numberOfBytes);
                if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_SendOperation)
                {
                    if (&m_send_op == pSockOperation) // 是我的操作
                    {
                        this->on_socket_error(errCode);
                        return this->_initiate_send_operation();
                    }
                    return true;
                }
                else if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_ReceiveOperation)
                {
                    if (&m_recv_op == pSockOperation)// 是我的操作
                    {
                        this->on_socket_error(errCode);
                        return this->_initiate_receive_operation();
                    }
                    return true;
                }
                else
                {
                    M_ASSERT(false);
                }
                return true;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::_initiate_send_operation()
            {
                if (m_sock == INVALID_SOCKET)
                {
                    m_send_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    return false;
                }

                // 也许是上次的数据未发送出去
                udp_packet_header packetHead;

                size_t nBytes;
                size_t rv = m_send_buffer.available_bytes_for_reading();
                if (rv < 1)
                {
                    m_send_buffer.reset();

                    char* pData = m_send_buffer.write_buffer();
                    //size_t len = m_send_buffer.available_bytes_for_writing();

                    {
                        ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex>   locker(m_lock);

                        // 先读取发送包的KUdpPacketHead信息
                        nBytes = m_outgoing_io_buffer.read(&packetHead, sizeof(udp_packet_header));
                        if (nBytes == sizeof(udp_packet_header))
                        {
                            // 读取包头信息，以决定整个包的长度
                            ::pilo::u16_t packetLength = packetHead.m_length;
                            nBytes = m_outgoing_io_buffer.read(pData, (size_t)packetLength);
                            M_ASSERT(nBytes == packetLength);

                            m_send_op.m_remote_socket_address = ipv4_address::to_sockaddr(packetHead.m_remote_socket_address);

                            m_send_buffer.inc_wp(packetLength);
                            rv = packetLength;
                        }
                        else
                        {
                            M_ASSERT(nBytes == 0);
                        }
                    }
                }

                if (rv < 1) // 没有数据需要发送
                {
                    m_send_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    return true;
                }

                m_send_op.m_initiataion_tick = ::GetTickCount64();
                m_send_op.m_wsa_buffer.buf = (char*)m_send_buffer.read_buffer();
                m_send_op.m_wsa_buffer.len = (ULONG) rv;
                //m_sendOperation.m_peerAddr = KSocketAddress::ToSockAddr(peerAddr);

                DWORD numberOfBytes = 0;
                int nRet = WSASendTo(m_sock,
                    &m_send_op.m_wsa_buffer,
                    1,
                    &numberOfBytes,
                    0,
                    (const sockaddr*)&m_send_op.m_remote_socket_address,
                    sizeof(m_send_op.m_remote_socket_address),
                    (WSAOVERLAPPED*)&m_send_op,
                    NULL);

                if (nRet == 0)
                {// 立即成功，IOCP将受到这个消息
                    // this->OnSuccessOperation(&m_sendOperation, numberOfBytes);
                    return true;
                }
                else
                {
                    DWORD errCode = WSAGetLastError();
                    if (errCode == WSA_IO_PENDING)
                    {
                        return true;
                    }

                    // 
                    this->on_fail_operation(&m_send_op, 0, errCode);
                    return false;
                }

                return true;
            }

            template<size_t _SendBufferSize /*= 10240*/, size_t _RecvBufferSize /*= 10240*/, size_t _IOBufferSize /*= 2048*/>
            bool pilo::core::net::iocp_udp_socket<_SendBufferSize, _RecvBufferSize, _IOBufferSize>::_initiate_receive_operation()
            {
                if (m_sock == INVALID_SOCKET)
                {
                    m_recv_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    return false;
                }

                //KPacketDeclare* pDecl = m_pSockServer->GetPacketDeclare();

                size_t wv = m_recv_buffer.available_bytes_for_writing();
                //ASSERT(wv >= pDecl->GetMaxLength());

                m_recv_op.m_initiataion_tick = ::GetTickCount64();
                m_recv_op.m_wsa_buffer.buf = m_recv_buffer.write_buffer();
                m_recv_op.m_wsa_buffer.len = (ULONG) wv;
                m_recv_op.m_address_length = sizeof(m_recv_op.m_originated_socket_address);

                DWORD numberOfBytes = 0;
                DWORD dwFlags = 0;
                int nRet = WSARecvFrom(m_sock,
                    &m_recv_op.m_wsa_buffer,
                    1,
                    &numberOfBytes,
                    &dwFlags,
                    (sockaddr*)&m_recv_op.m_originated_socket_address,
                    &m_recv_op.m_address_length,
                    (WSAOVERLAPPED*)&m_recv_op,
                    NULL);

                if (nRet == 0)
                {	// 立即成功
                    //this->OnSuccessOperation(&m_recvOperation, numberOfBytes);
                    return true;
                }
                else
                {
                    DWORD errCode = WSAGetLastError();
                    if (errCode == WSA_IO_PENDING)
                    {
                        return true;
                    }
                    this->close();
                    this->on_fail_operation(&m_recv_op, 0, errCode);
                    return false;
                }

                M_ASSERT(false);
                return false;
            }


        }
    }
}
#endif //defined(WIN32) || defined(WINDOWS)