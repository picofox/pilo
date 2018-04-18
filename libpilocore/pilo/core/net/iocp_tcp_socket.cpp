#include "core/coredefs.hpp"
#if defined(WIN32) || defined(WINDOWS)

#include "./iocp_tcp_socket.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {

            iocp_tcp_socket::iocp_tcp_socket()
            {
                this->reset();
            }

            iocp_tcp_socket::~iocp_tcp_socket()
            {

            }

            size_t iocp_tcp_socket::send(const void* packet, size_t len)
            {
                if (m_sock == INVALID_SOCKET || !m_ready_for_io)
                {
                    return 0;
                }

                size_t nbs = 0;
                {
                    pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex>   locker(m_lock);
                    nbs = m_outgoing_io_buffer.write(packet, len);
                }

                if (nbs < len)
                {	// 发送缓冲区满了，断开这个连接
                    //TODO log
                    //Log(LOG_WARN, "warn: %s send buffer full, close it", this->ToString().c_str());
                    this->close();
                    return 0;
                }

                return len;
            }

            size_t iocp_tcp_socket::send_ndelay(const void* packet, size_t len)
            {
                if (m_sock == INVALID_SOCKET || !m_ready_for_io)
                {
                    return 0;
                }

                size_t nbs = 0;
                {
                    pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex>   locker(m_lock);
                    nbs = m_outgoing_io_buffer.write(packet, len);
                }

                if (nbs < len)
                {	// 发送缓冲区满了，断开这个连接
                    //TODO log
                    //Log(LOG_WARN, "warn: %s send buffer full, close it", this->ToString().c_str());
                    this->close();
                    return 0;
                }

                this->initiate_send_operation();
                return len;
            }

            size_t iocp_tcp_socket::send_to(const void* packet, size_t len, const pilo::core::net::ipv4_address& address)
            {
                M_UNUSED(address);
                M_UNUSED(packet);
                M_UNUSED(len);
                M_ASSERT(false);
                return 0;
            }

            size_t iocp_tcp_socket::send_to_ndelay(const void* packet, size_t len, const pilo::core::net::ipv4_address& address)
            {
                M_UNUSED(address);
                M_UNUSED(packet);
                M_UNUSED(len);
                //ASSERT(FALSE);
                return 0;
            }


            void iocp_tcp_socket::reset()
            {
                tcp_socket_buffer::reset();
                memset(&m_send_op, 0, sizeof(m_send_op)); // clear send operation
                memset(&m_recv_op, 0, sizeof(m_recv_op)); // clear recv operation
                basic_socket::reset();
            }

            bool iocp_tcp_socket::on_closed_state()
            {
                M_ASSERT(m_sock == INVALID_SOCKET);

                if (iocp_socket::on_closed_state())
                    return true;

                if (m_send_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation) return true;
                if (m_recv_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation) return true;

                return false;
            }

            bool iocp_tcp_socket::initiate_send_operation()
            {
                if (m_sock == INVALID_SOCKET) return false;
                if (!this->has_pending_data()) return true;

                if (m_send_paused)
                {
                    return true;
                }

                if (m_send_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                {
                    return true;
                }

                {
                    pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex>   locker(m_lock);
                    if (m_send_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                    {
                        return true;
                    }
                    m_send_op.m_operation_type = iocp_scoket_operation::eISOT_SendOperation;
                }

                return this->_initiate_send_operation();
            }

            bool iocp_tcp_socket::initiate_receive_operation()
            {
                if (m_sock == INVALID_SOCKET) return false;

                if (m_recv_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                {
                    return true;
                }

                {
                    pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex>   locker(m_lock);
                    if (m_recv_op.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                    {
                        return true;
                    }
                    m_recv_op.m_operation_type = iocp_scoket_operation::eISOT_ReceiveOperation;
                }
                return this->_initiate_receive_operation();
            }

            bool iocp_tcp_socket::initiate_accept_operation()
            {
                M_ASSERT(FALSE);
                return false;
            }

            bool iocp_tcp_socket::on_success_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes)
            {
                if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_SendOperation)
                {
                    M_ASSERT(&m_send_op == pSockOperation);

                    if (numberOfBytes == 0)
                    {	
                        //TODO log this warn
                        // 一个成功发送操作，只发了0字节，不知是否代表对端关闭 ?
                        //Log(LOG_WARN, "warn: %s send 0 byte, close it", this->ToString().c_str());
                        m_send_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                        this->close();
                        return true;
                    }

                    m_bytes_sent += numberOfBytes;
                    m_latest_send_tick = ::GetTickCount64();

                    //KPacketDeclare* pDecl = m_pSockServer->GetPacketDeclare();

                    const void* pData = m_send_buffer.read_buffer();
                    this->on_sending_data(pData, numberOfBytes, &m_peer_address);

                    // 更新发送缓冲区的读指针
                    m_send_buffer.inc_rp((short)numberOfBytes);
                    m_send_buffer.compact();

                    return this->initiate_send_operation();
                }
                else if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_ReceiveOperation)
                {
                    M_ASSERT(&m_recv_op == pSockOperation);

                    m_bytes_received += numberOfBytes;
                    m_latest_recv_tick = ::GetTickCount64();

                    if (numberOfBytes == 0) // 对方关闭这个连接了
                    {
                        //TODO log error
                        //Log(LOG_WARN, "warn: %s peer closed, close it", this->ToString().c_str());

                        pSockOperation->m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;

                        this->close();
                        return true;
                    }

                    //KPacketDeclare* pDecl = m_pSockServer->GetPacketDeclare();

                    // 更新接收缓冲区的写指针
                    m_recv_buffer.inc_wp((short)numberOfBytes);

                    const char* pData = m_recv_buffer.read_buffer();
                    size_t dataLen = m_recv_buffer.available_bytes_for_reading();

                    size_t nBytes = this->on_receiving_data(pData, dataLen, &m_peer_address);
                    if (nBytes == MC_INVALID_SIZE)
                    {	// 出错了
                        this->on_invalid_packet(&m_peer_address, (EnumErrorPacketReason)nBytes);
                        m_recv_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                        return false;
                    }
                    else if (nBytes > 0)
                    {
                        m_recv_buffer.inc_rp(nBytes);
                        m_recv_buffer.compact();
                    }

                    // 继续向IOCP发起接收操作
                    return this->_initiate_receive_operation();
                }

                M_ASSERT(false);
                return false;
            }

            bool iocp_tcp_socket::on_fail_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes, pilo::error_number_t errCode)
            {
                M_UNUSED(numberOfBytes);
                if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_SendOperation)
                {
                    if (&m_send_op == pSockOperation)
                    {
                        this->on_socket_error(errCode);
                        m_send_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    }
                }
                else if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_ReceiveOperation)
                {
                    if (&m_recv_op == pSockOperation)
                    {
                        this->on_socket_error(errCode);
                        m_recv_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    }
                }
                else
                {
                    M_ASSERT(FALSE);
                }

                return true;
            }

            bool iocp_tcp_socket::on_close_operation(iocp_scoket_operation* pSockOperation)
            {
                if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_SendOperation)
                {
                    if (&m_send_op == pSockOperation)
                    {
                        this->close();
                        m_send_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    }
                }
                else if (pSockOperation->m_operation_type == iocp_scoket_operation::eISOT_ReceiveOperation)
                {
                    if (&m_recv_op == pSockOperation)
                    {
                        this->close();
                        m_recv_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    }
                }
                else
                {
                    M_ASSERT(false);
                }

                return true;
            }

            void iocp_tcp_socket::on_start_working()
            {
                m_outgoing_io_buffer.reset();
                m_send_buffer.reset();
                m_recv_buffer.reset();
                memset(&m_send_op, 0, sizeof(m_send_op));
                memset(&m_recv_op, 0, sizeof(m_recv_op));
                iocp_socket::on_start_working();
            }

            bool iocp_tcp_socket::_initiate_send_operation()
            {
                size_t rv = m_send_buffer.available_bytes_for_reading();

                if (rv < 1)
                {
                    rv = this->load_send_buffer();
                }

                if (rv < 1) // 没有数据可发
                {
                    m_send_op.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    return true;
                }

                m_send_op.m_initiataion_tick = GetTickCount64();
                m_send_op.m_wsa_buffer.buf = (char*)m_send_buffer.read_buffer();
                m_send_op.m_wsa_buffer.len = (ULONG) rv;

                DWORD numberOfBytes = 0;
                int nRet = WSASend(m_sock,
                    &m_send_op.m_wsa_buffer,
                    1,
                    &numberOfBytes,
                    0,
                    (WSAOVERLAPPED*)&m_send_op,
                    NULL);

                if (nRet == 0) // 立刻完成了
                {
                    // IOCP 会收到这个通知，这里不需要处理
                    return true;
                }
                else
                {
                    DWORD errCode = WSAGetLastError();
                    if (errCode == WSA_IO_PENDING)
                    {
                        return true;
                    }

                    // 操作失败了
                    this->on_fail_operation(&m_send_op, 0, errCode);
                    return false;
                }

                // 不可达
                //return true;
            }

            bool iocp_tcp_socket::_initiate_receive_operation()
            {
                size_t wv = m_recv_buffer.available_bytes_for_writing();

                m_recv_op.m_initiataion_tick = GetTickCount64();
                m_recv_op.m_wsa_buffer.buf = m_recv_buffer.write_buffer();
                m_recv_op.m_wsa_buffer.len = (ULONG) wv;

                DWORD dwFlags = 0;
                DWORD numberOfBytes = 0;
                int nRet = WSARecv(m_sock,
                    &m_recv_op.m_wsa_buffer,
                    1,
                    &numberOfBytes,
                    &dwFlags,
                    (WSAOVERLAPPED*)&m_recv_op,
                    NULL);

                if (nRet == 0) // 立刻完成了
                {
                    // IOCP会收到这个通知，所以这里不需要处理
                    return true;
                }
                else
                {
                    DWORD errCode = WSAGetLastError();
                    if (errCode == WSA_IO_PENDING)
                    {
                        return true;
                    }

                    // 失败了
                    this->on_fail_operation(&m_recv_op, 0, errCode);
                    return false;
                }

                // 不可达
                //ASSERT(false);
                //return false;
            }
        }
    }
}




#endif //defined(WIN32) || defined(WINDOWS)