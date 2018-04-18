#pragma once
#include "core/coredefs.hpp"
#if defined(WIN32) || defined(WINDOWS)
#include "./iocp_socket.hpp"
#include "./tcp_socket_buffer.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            class iocp_tcp_socket : public iocp_socket, public tcp_socket_buffer<>
            {
            public:
                iocp_tcp_socket();
                virtual ~iocp_tcp_socket();

            public:
                inline basic_socket* get_socket() { return this; }

                virtual size_t send(const void* packet, size_t len); // Tcp使用的发送函数
                virtual size_t send_to(const void* packet, size_t len, const pilo::core::net::ipv4_address& address); // UDP使用的发送函数
                virtual size_t send_ndelay(const void* packet, size_t len);
                virtual size_t send_to_ndelay(const void* packet, size_t len, const pilo::core::net::ipv4_address& address);

                virtual void reset();
                virtual bool on_closed_state();

            public:
                // 发起各种类型的IO操作
                // 由程序逻辑保证实际的操作不会并发
                virtual bool initiate_send_operation();
                virtual bool initiate_receive_operation();
                virtual bool initiate_accept_operation();

                // 收到了IOCP的通知
                virtual bool on_success_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes);
                virtual bool on_fail_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes, pilo::error_number_t errCode);
                virtual bool on_close_operation(iocp_scoket_operation* pSockOperation);

                // handle start work notification
                virtual void on_start_working();

            public:
                bool _initiate_send_operation();
                bool _initiate_receive_operation();

            public:
                iocp_tcp_scoket_send_operation m_send_op;
                iocp_tcp_scoket_receive_operation m_recv_op;
            };

        }
    }
}











#endif