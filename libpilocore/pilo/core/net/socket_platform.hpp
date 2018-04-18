#pragma once
#include "./net_sys_header.hpp"

#ifdef WINDOWS
#include "./iocp_tcp_listen_socket.hpp"
#include "./iocp_tcp_socket.hpp"
#include "./iocp_udp_socket.hpp"
#else
#include "./epoll_tcp_listen_socket.hpp"
#include "./epoll_tcp_socket.hpp"
#include "./epoll_udp_socket.hpp"
#endif




namespace pilo
{
    namespace core
    {
        namespace net
        {

#if defined(_USE_SELECT_)
            typedef KSelectTcpListenSocket KTcpListenSocket;
            typedef KSelectTcpSocket KTcpSocket;
            typedef KSelectUdpSocket KUdpSocket;
            typedef KSelectSockServer KSockServer;
#else
#   if defined(WIN32) || defined(WINDOWS)
            typedef iocp_tcp_listen_socket tcp_listen_socket;
            typedef iocp_tcp_socket tcp_socket;
            typedef iocp_udp_socket<> udp_socket;
            // typedef KIocpSockServer KSockServer;
#   else
            typedef epoll_tcp_socket tcp_socket;
            // typedef KEpollTcpListenSocket KTcpListenSocket;
            // typedef KEpollTcpSocket KTcpSocket;
            // typedef KEpollUdpSocket KUdpSocket;
            // typedef KEpollSockServer KSockServer;
#   endif
#endif
            
            enum EnumSocketShutdownHow 
            {
                eSSH_NoMoreRecv    = 0,
                eSSH_NoMoreSend    = 1,
                eSSH_Neither       = 2,                            
            };
            ::pilo::EnumErrorCode close_socket(os_socket_fd_t socket);
            ::pilo::EnumErrorCode shutdown_socket(os_socket_fd_t socket, EnumSocketShutdownHow how);

        }
    }
}