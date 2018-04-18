#pragma once
#include "core/coredefs.hpp"
#if defined(WIN32) || defined(WINDOWS)
#include "iocp_socket.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {

            class iocp_tcp_listen_socket : public iocp_socket
            {
            public:
                iocp_tcp_listen_socket();
                virtual ~iocp_tcp_listen_socket();

            public:
                bool listen(pilo::u16_t port);							// 监听所有Interface的指定端口
                bool listen(pilo::u32_t ip, pilo::u16_t port);				// 在指定IP地址上监听端口
                bool listen(const char* address_name, pilo::u16_t port);	// 监听制定域名上的端口

            public:
                virtual EnumSocketType socket_type() const;

                virtual size_t send(const void* packet, size_t len);
                virtual size_t send_to(const void* packet, size_t len, const pilo::core::net::ipv4_address& address); // UDP使用的发送函数
                virtual size_t send_ndelay(const void* packet, size_t len) {  M_UNUSED(packet); M_UNUSED(len); return 0;}
                virtual size_t send_to_ndelay(const void* packet, size_t len, const pilo::core::net::ipv4_address& address) { M_UNUSED(packet); M_UNUSED(len); M_UNUSED(address); return 0; }
                void reset();
                virtual bool on_closed_state();
                virtual pilo::core::string::fixed_astring<256> to_string() const;

            public:

                virtual bool initiate_send_operation();
                virtual bool initiate_receive_operation();
                virtual bool initiate_accept_operation();

                // 收到了IOCP的通知
                virtual bool on_success_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes);
                virtual bool on_fail_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes, size_t errCode);
                virtual bool on_close_operation(iocp_scoket_operation* pSockOperation);

                // handle start work notification
                virtual void on_start_working();

                protected:
                bool _initiate_accept_operation();

            public:
                ::pilo::u32_t                     m_listen_ip;
                ::pilo::u16_t                     m_listen_port;
                char                              m_receive_buffer[256];
                iocp_tcp_scoket_accept_operation m_accept_operation;
            };


        }
    }
}


























#endif //defined(WIN32) || defined(WINDOWS)