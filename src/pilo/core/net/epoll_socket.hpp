#pragma once

#include "core/coredefs.hpp"


#if !defined(WIN32) && !defined(WINDOWS)

#include "core/net/basic_socket.hpp"
#include "core/net/epoll_socket_server.hpp"
#include <sys/epoll.h>

namespace pilo
{
    namespace core
    {
        namespace net
        {
            class basic_socket_server;
            class epoll_socket_server;

            class epoll_socket : public basic_socket
            {
            public:
                epoll_socket();
                virtual ~epoll_socket();

            public:
                basic_socket_server* socket_server();
                virtual void reset();

            public:
                // recv
                virtual void read() = 0;

                // send
                virtual void write() = 0;

                virtual void on_start_working();
                virtual bool on_closed_state();

                void enable_writable();
                void disable_writable();

            public:
                // 是否加入到EPOLL
                volatile bool m_in_epoll;
                epoll_socket_server* m_socket_server;
                volatile bool m_writable;
                volatile bool m_in_writing;
                //volatile BOOL m_careOut;
                //volatile DWORD m_lastBlockTick;
            };
        }
    }
}

#endif 