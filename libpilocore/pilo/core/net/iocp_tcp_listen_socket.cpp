#include "core/coredefs.hpp"


#if defined(WIN32) || defined(WINDOWS)

#include "socket_platform.hpp"
#include "iocp_tcp_listen_socket.hpp"
#include "core/datetime/datetime.hpp"
#include "iocp_socket_server.hpp"


namespace pilo
{
    namespace core
    {
        namespace net
        {
            iocp_tcp_listen_socket::iocp_tcp_listen_socket()
            {
                this->reset();
            }

            iocp_tcp_listen_socket::~iocp_tcp_listen_socket()
            {

            }

            bool iocp_tcp_listen_socket::listen(pilo::u16_t port)
            {
                return this->listen((pilo::u32_t)INADDR_ANY, port);
            }

            bool iocp_tcp_listen_socket::listen(pilo::u32_t ip, pilo::u16_t port)
            {
                M_ASSERT(m_sock == MC_INVALID_SOCK_FD);

                ipv4_address sockAddr = { ip, port };
                sockaddr_in localAddr = ipv4_address::to_sockaddr(sockAddr);

                os_socket_fd_t sock = socket(AF_INET, SOCK_STREAM, 0);
                if (sock == MC_INVALID_SOCK_FD)
                {
                    return false;
                }

                if (bind(sock, (sockaddr*)&localAddr, sizeof(localAddr)) != 0)
                {
                    closesocket(sock);
                    return false;
                }

                struct linger lgr;
                lgr.l_onoff = 1;
                lgr.l_linger = 0;
                setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&lgr, sizeof(lgr));

                if (::listen(sock, 10) != 0)
                {
                    closesocket(sock);
                    return false;
                }

                m_sock = sock;
                m_listen_ip = ip;
                m_listen_port = port;

                return true;
            }

            bool iocp_tcp_listen_socket::listen(const char* address_name, pilo::u16_t port)
            {
                pilo::u32_t listen_ip = basic_socket::get_ipv4_by_name(address_name);
                if (listen_ip == 0)
                {
                    return false;
                }
                return this->listen(listen_ip, port);
            }

            basic_socket::EnumSocketType iocp_tcp_listen_socket::socket_type() const
            {
                return basic_socket::eST_ListenTCP;
            }

            size_t iocp_tcp_listen_socket::send(const void* packet, size_t len)
            {
                M_UNUSED(packet);
                M_UNUSED(len);
                M_ASSERT(false);
                return 0;
            }

            size_t iocp_tcp_listen_socket::send_to(const void* packet, size_t len, const pilo::core::net::ipv4_address& address)
            {
                M_UNUSED(packet);
                M_UNUSED(len);
                M_UNUSED(address);
                M_ASSERT(false);
                return 0;
            }


            void iocp_tcp_listen_socket::reset()
            {
                m_listen_ip = 0;
                m_listen_port = 0;
                memset(&m_accept_operation, 0, sizeof(m_accept_operation));
                basic_socket::reset(); //TODO make sure why not call reset of ancestors not base
            }

            bool iocp_tcp_listen_socket::on_closed_state()
            {
                if (iocp_socket::on_closed_state())
                {
                    return true;
                }

                M_ASSERT(m_sock == MC_INVALID_SOCK_FD);

                if (m_accept_operation.m_operation_type == iocp_scoket_operation::eISOT_InvalidOperation)
                {
                    return false;
                }

                return true;
            }

            pilo::core::string::fixed_astring<256> iocp_tcp_listen_socket::to_string() const
            {
                char cTmp[256];
                ipv4_address addr = { m_listen_ip, m_listen_port };
                sprintf_s(cTmp, "TCP Listen Socket %u listen: %s",
                    m_sock != MC_INVALID_SOCK_FD ? m_sock : m_sock_dup,
                    ipv4_address::to_string(addr).c_str());
                return cTmp;
            }

            bool iocp_tcp_listen_socket::initiate_send_operation()
            {
                M_ASSERT(false);
                return false;
            }

            bool iocp_tcp_listen_socket::initiate_receive_operation()
            {
                M_ASSERT(false);
                return false;
            }

            bool iocp_tcp_listen_socket::initiate_accept_operation()
            {
                if (m_sock == MC_INVALID_SOCK_FD)
                {
                    return false;
                }

                if (m_accept_operation.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                {
                    return true;
                }

                {	// ?????????????????????
                    pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> locker(m_lock);
                    if (m_accept_operation.m_operation_type != iocp_scoket_operation::eISOT_InvalidOperation)
                    {
                        return true;
                    }
                    m_accept_operation.m_operation_type = iocp_scoket_operation::eISOT_AcceptOperation;
                }

                // ????????????????
                return this->_initiate_accept_operation();
            }

            bool iocp_tcp_listen_socket::on_success_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes)
            {
                M_UNUSED(numberOfBytes);
                M_ASSERT(&m_accept_operation == pSockOperation);                             m_bytes_received ++;                m_latest_recv_tick = PiloGetTickCount64();                os_socket_fd_t new_socket = m_accept_operation.m_accept_socket;                if (!m_socket_server->can_accept())
                {
                    pilo::core::net::close_socket(new_socket);
                    // 已经在Accept状态中，直接使用内部发起操作的函数
                    this->_initiate_accept_operation();
                    return true;
                }                int nret = ::setsockopt(new_socket, SOL_SOCKET, SO_UPDATE_ACCEPT_CONTEXT, (char*)&m_sock, sizeof(m_sock));                if (nret)
                {   //TODO log error
                    // 出错了
                    //DWORD errCode = GetLastError();
                    //Log(LOG_ERROR, "error: %s setsockopt, %s", this->ToString().c_str(), strerror_r2(errCode).c_str());
                    pilo::core::net::close_socket(new_socket);
                }                else
                {
                    this->on_accepted(m_accept_operation.m_accept_socket);
                }                // 已经在Accept状态中，直接使用内部发起操作的函数
                this->_initiate_accept_operation();                return true;            }

            bool iocp_tcp_listen_socket::on_fail_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes, size_t errCode)
            {
                M_UNUSED(numberOfBytes);
                M_UNUSED(errCode);
                M_ASSERT(&m_accept_operation == pSockOperation);

               // this->on_socket_error(errCode);

                // ????????Socket??????????????????????close
                pilo::core::net::close_socket(m_accept_operation.m_accept_socket);
                m_accept_operation.m_accept_socket = MC_INVALID_SOCK_FD;
                m_accept_operation.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;

                return true;
            }

            bool iocp_tcp_listen_socket::on_close_operation(iocp_scoket_operation* pSockOperation)
            {
                M_ASSERT(&m_accept_operation == pSockOperation);

                // ????????Socket??????????????????????close
                pilo::core::net::close_socket(m_accept_operation.m_accept_socket);
                m_accept_operation.m_accept_socket = MC_INVALID_SOCK_FD;
                m_accept_operation.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;

                return true;
            }

            void iocp_tcp_listen_socket::on_start_working()
            {
                memset(&m_accept_operation, 0x00, sizeof(m_accept_operation));
                iocp_socket::on_start_working();
            }

            bool iocp_tcp_listen_socket::_initiate_accept_operation()
            {
                pilo::os_socket_fd_t sock = basic_socket::create_tcp_socket();
                if (sock == MC_INVALID_SOCK_FD)
                {
                   // DWORD errCode = GetLastError();
                    //TODO log
                    //Log(LOG_WARN, "warn: WSASocket errCode:%u, %s", errCode, strerror_r2(errCode).c_str());

                    m_accept_operation.m_operation_type = iocp_scoket_operation::eISOT_InvalidOperation;
                    return false;
                }

                m_accept_operation.m_initiataion_tick      = PiloGetTickCount64();
                m_accept_operation.m_receive_buffer        = &m_receive_buffer[0];
                m_accept_operation.m_length                = sizeof(m_receive_buffer);
                m_accept_operation.m_accept_socket         = sock;
                m_accept_operation.m_local_socket_address  = (sockaddr_in*)&m_receive_buffer[0];
                m_accept_operation.m_peer_socket_address = (sockaddr_in*)&m_receive_buffer[sizeof(sockaddr_in)+16];

                BOOL bRet = AcceptEx(m_sock,
                    m_accept_operation.m_accept_socket,
                    m_accept_operation.m_receive_buffer,
                    0,
                    sizeof(sockaddr_in)+16,
                    sizeof(sockaddr_in)+16,
                    (LPDWORD) &m_accept_operation.m_bytes_received,
                    (OVERLAPPED*)&m_accept_operation);

                if (bRet)
                {	// 立刻成功
                    // IOCP 会收到这个通知，此处不许处理
                    //this->OnSuccessOperation(&m_acceptOperation, m_acceptOperation.m_byteRecved);
                }
                else
                {
                    DWORD errCode = WSAGetLastError();
                    if (errCode == ERROR_IO_PENDING) 
                    {
                        return true;
                    }
                    this->on_fail_operation(&m_accept_operation, 0, errCode);
                    return false;
                }

                return true;
            }

        }
    }
}

#endif //defined(WIN32) || defined(WINDOWS)

