#include "core/coredefs.hpp"

#if !defined(WIN32) && !defined(WINDOWS)

#include "epoll_tcp_listen_socket.hpp"
#include "core/io/format_output.hpp"
#include "core/datetime/datetime.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            
            epoll_tcp_listen_socket::epoll_tcp_listen_socket() :m_listen_ip(0),m_listen_port(0)
            {

            }

            epoll_tcp_listen_socket::~epoll_tcp_listen_socket()
            {

            }

            bool epoll_tcp_listen_socket::listen(pilo::u16_t port)
            {
                return this->listen(INADDR_ANY, port);
            }

            bool epoll_tcp_listen_socket::listen(pilo::u32_t ip, pilo::u16_t port)
            {
                M_ASSERT(m_sock == MC_INVALID_SOCK_FD);

                ::pilo::core::net::ipv4_address sockAddr = {ip, port};
                sockaddr_in localAddr = ipv4_address::to_sockaddr(sockAddr);

                os_socket_fd_t sock = basic_socket::create_tcp_socket();
                if(sock == MC_INVALID_SOCK_FD) return false;

                if(::bind(sock, (sockaddr*)&localAddr, sizeof(localAddr)) != 0)
                {
                    //TODO log error
//                     DWORD errCode = GetLastError();
//                     Log(LOG_ERROR, "error: bind to %s, %s",
//                         KSocketAddress::ToString(sockAddr).c_str(),
//                         strerror_r2(errCode).c_str());
                    ::close(sock);
                    return false;
                }

                struct linger lgr;
                lgr.l_onoff = 1;
                lgr.l_linger = 0;
                ::setsockopt(sock, SOL_SOCKET, SO_LINGER, (char*)&lgr, sizeof(lgr));

                if (::listen(sock, 10) != 0)
                {
                    ::close(sock);
                    return false;
                }

                m_sock = sock;
                m_listen_ip = ip;
                m_listen_port = port;

                return true;
            }

            bool epoll_tcp_listen_socket::listen(const char* address_name, pilo::u16_t port)
            {
                ::pilo::u32_t ip  = basic_socket::get_ipv4_by_name(address_name);
                if(ip == 0) return false;
                return this->listen(ip, port);
            }

            pilo::core::net::basic_socket::EnumSocketType epoll_tcp_listen_socket::socket_type() const
            {
                return basic_socket::eST_ListenTCP;
            }

            void epoll_tcp_listen_socket::reset()
            {
                epoll_socket::reset();
                m_listen_ip = 0;
                m_listen_port = 0;
            }

            size_t epoll_tcp_listen_socket::send(const void* packet, size_t len)
            {
                M_UNUSED(packet); M_UNUSED(len);
                M_ASSERT(false);
                return 0;
            }

            size_t epoll_tcp_listen_socket::send_to(const void* packet, size_t len, const pilo::core::net::ipv4_address& address)
            {
                M_UNUSED(packet); M_UNUSED(len); M_UNUSED(address);
                M_ASSERT(false);
                return 0;
            }

            pilo::core::string::fixed_astring<256> epoll_tcp_listen_socket::to_string() const
            {
                char cTmp[256];
                ::pilo::core::net::ipv4_address addr = {m_listen_ip, m_listen_port};
                ::pilo::core::io::string_format_output(cTmp, sizeof(cTmp), "TCP Listen Socket %u listen: %s",
                                                       m_sock != MC_INVALID_SOCK_FD ? m_sock : m_sock_dup,
                                                        ::pilo::core::net::ipv4_address::to_string(addr).c_str());
                return cTmp;
            }

            void epoll_tcp_listen_socket::read()
            {
                sockaddr_in peerAddr;
                socklen_t addrLen = sizeof(peerAddr);

                while(true)
                {
                    ::pilo::os_socket_fd_t sock = ::accept(m_sock, (sockaddr*)&peerAddr, &addrLen);
                    if(sock == MC_INVALID_SOCK_FD) break;

                    if(m_socket_server->can_accept())
                    {
                        basic_socket::set_none_blocking_mode(sock);
                        basic_socket::ignore_sigpipe(sock);
                        this->on_accepted(sock);
                    }
                    else
                    {
                        ::close(sock);
                    }

                    m_bytes_received += 1;
                    m_latest_recv_tick = PiloGetTickCount64();
                }

                ::pilo::os_error_number_t errCode = ::pilo::get_last_os_error_number();

                // 读完了
                if (errCode == EAGAIN) return;

                switch (errCode)
                {
                    case EMFILE://system file limit
                        M_ASSERT(false); break;
                    case ENFILE:// user file limit
                        M_ASSERT(false); break;
                    case ENOBUFS:
                        M_ASSERT(false); break;
                    case ENOMEM:
                        M_ASSERT(false); break;
                    case EPERM: //firewall settings
                        M_ASSERT(false); break;
                };

                // 出错了
                this->on_socket_error(errCode);

                return;
            }

            void epoll_tcp_listen_socket::write()
            {
                return;
            }

            void epoll_tcp_listen_socket::on_start_working()
            {
                epoll_socket::on_start_working();
            }

        }
    }
}




#endif