#pragma once

#include "socket_platform.hpp"
#include "core/io/format_output.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            class passive_tcp_socket : public tcp_socket
            {
            public:
                passive_tcp_socket() {}
                virtual ~passive_tcp_socket() {};

            public:
                bool attach(os_socket_fd_t sock);                

            public:
                virtual EnumSocketType socket_type() const { return basic_socket::eST_PassiveTCP; }

                virtual pilo::core::string::fixed_astring<256> to_string() const;
                
            };
        }
    }
}