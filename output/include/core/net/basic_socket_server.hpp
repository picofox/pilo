#pragma once

#include "core/net/basic_socket.hpp"
#include "core/memory/io_buffer.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            class basic_socket_server
            {
            public:
                basic_socket_server() {}
                virtual ~basic_socket_server() {}

            public:
                virtual bool initialize(size_t socketCapacity, size_t ioThreadNum) = 0;
                virtual void finalize() = 0;
                // 外部线程调用，可能是多个线程并发的
                virtual bool attach(basic_socket* socket_ptr) = 0;

            public:
                pilo::core::memory::iobuffer_2kb_pool   m_buffer_pool;
                   
            };
        }
    }
}