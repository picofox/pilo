#pragma once
#include "core/coredefs.hpp"

#if !defined(WIN32) && !defined(WINDOWS)
#include "epoll_socket.hpp"
#include "tcp_socket_buffer.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            class epoll_tcp_socket : public epoll_socket, public tcp_socket_buffer<>
            {
            public:
                epoll_tcp_socket();
                virtual ~epoll_tcp_socket();

            public:
                inline basic_socket* get_socket() { return this; }

            public:
                virtual void reset();
                virtual size_t send(const void* packet, size_t len);
                virtual size_t send_to(const void* packet, size_t len, const pilo::core::net::ipv4_address& address); // UDP使用的发送函数
                virtual size_t send_ndelay(const void* packet, size_t len);
                virtual size_t send_to_ndelay(const void* packet, size_t len, const pilo::core::net::ipv4_address& address) { M_UNUSED(packet); M_UNUSED(len); M_UNUSED(address); return 0; }



            public:
                virtual void read();
                virtual void write();
                virtual void on_start_working();

                public:
                //
            };
        }
    }
}



#endif