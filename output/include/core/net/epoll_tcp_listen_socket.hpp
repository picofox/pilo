#pragma once

#include "core/coredefs.hpp"


#if !defined(WIN32) && !defined(WINDOWS)

#include "epoll_socket.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            class epoll_tcp_listen_socket : public epoll_socket
            {
            public:
                epoll_tcp_listen_socket();
                virtual ~epoll_tcp_listen_socket();

            public:
                bool listen(pilo::u16_t port);							// 监听所有Interface的指定端口
                bool listen(pilo::u32_t ip, pilo::u16_t port);				// 在指定IP地址上监听端口
                bool listen(const char* address_name, pilo::u16_t port);	// 监听制定域名上的端口

            public:
                virtual EnumSocketType socket_type() const;
                virtual void reset();
                virtual size_t send(const void* packet, size_t len);
                virtual size_t send_to(const void* packet, size_t len, const pilo::core::net::ipv4_address& address); // UDP使用的发送函数
                virtual size_t send_ndelay(const void* packet, size_t len) {  M_UNUSED(packet); M_UNUSED(len); return 0;}
                virtual size_t send_to_ndelay(const void* packet, size_t len, const pilo::core::net::ipv4_address& address) { M_UNUSED(packet); M_UNUSED(len); M_UNUSED(address); return 0; }

                virtual pilo::core::string::fixed_astring<256> to_string() const;

            public:
                virtual void read();
                virtual void write();
                virtual void on_start_working();

            public:
                ::pilo::u32_t                     m_listen_ip;
                ::pilo::u16_t                     m_listen_port;
            };
        }
    }
}


#endif