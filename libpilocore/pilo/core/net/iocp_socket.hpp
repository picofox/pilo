#pragma once
#include "core/coredefs.hpp"
#if defined(WIN32) || defined(WINDOWS)
#include "basic_socket.hpp"
#include "basic_socket_server.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            struct iocp_scoket_operation
            {
                enum EnumIocpSocketOperationType
                {
                    eISOT_InvalidOperation,
                    eISOT_SendOperation,
                    eISOT_ReceiveOperation,
                    eISOT_AcceptOperation
                };

                OVERLAPPED m_overlapped;
                volatile EnumIocpSocketOperationType m_operation_type;
                pilo::tick64_t m_initiataion_tick;

                static const char* operation_name(EnumIocpSocketOperationType opType)
                {
                    switch (opType)
                    {
                        case eISOT_InvalidOperation:
                        {    
                            return "eISOT_InvalidOperation";
                            break;
                        }
                        case eISOT_SendOperation:
                        {
                            return "eISOT_SendOperation";
                            break;
                        }                            
                        case eISOT_ReceiveOperation:
                        {
                            return "eISOT_ReceiveOperation";
                            break;
                        }
                        case eISOT_AcceptOperation:
                        {
                            return "eISOT_AcceptOperation";
                            break;
                        }
                        default:
                        {
                            M_ASSERT(FALSE);
                        }
                    }
                    return "[Err_get_op]";;
                }
            };

            struct iocp_tcp_scoket_send_operation : public iocp_scoket_operation
            {
                WSABUF m_wsa_buffer;
            };

            struct iocp_tcp_scoket_receive_operation : public iocp_scoket_operation
            {
                WSABUF m_wsa_buffer;
            };

            struct iocp_tcp_scoket_accept_operation : public iocp_scoket_operation
            {
                char*           m_receive_buffer;
                size_t          m_length;
                size_t          m_bytes_received;
                os_socket_fd_t     m_accept_socket;
                sockaddr_in*    m_local_socket_address;
                sockaddr_in*    m_peer_socket_address;
            };

            struct iocp_udp_socket_send_operation : public iocp_scoket_operation
            {
                WSABUF m_wsa_buffer;
                sockaddr_in m_remote_socket_address;
            };

            struct iocp_udp_socket_receive_operation : public iocp_scoket_operation
            {
                WSABUF      m_wsa_buffer;
                sockaddr_in m_originated_socket_address;
                pilo::i32_t m_address_length;
            };

            class iocp_socket_server;
            class iocp_socket : public basic_socket
            {
            public:
                iocp_socket();
                virtual ~iocp_socket();

            public:
                basic_socket_server* socket_server();
                void reset();

            public:
                virtual void on_start_working();

                // 发起各种类型的IO操作
                // 由程序逻辑保证实际的操作不会并发
                virtual bool initiate_send_operation() = 0;
                virtual bool initiate_receive_operation() = 0;
                virtual bool initiate_accept_operation() = 0;

                // 收到了IOCP的通知
                virtual bool on_success_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes) = 0;
                virtual bool on_fail_operation(iocp_scoket_operation* pSockOperation, size_t numberOfBytes, size_t errCode) = 0;
                virtual bool on_close_operation(iocp_scoket_operation* pSockOperation) = 0;

            public:
                iocp_socket_server* m_socket_server;
            };
        }
    }
}


#endif //defined(WIN32) || defined(WINDOWS)