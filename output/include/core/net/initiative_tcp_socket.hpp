#pragma once

#include "./socket_platform.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/net/epoll_tcp_socket.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            class initiative_tcp_socket : public tcp_socket
            {
            public:
                friend class iocp_socket_server;

                enum EnumConnectionStage
                {
                    eCS_Attached,
                    eCS_Closed,
                    eCS_Waiting,
                    eCS_Connected,
                };

            public:
                initiative_tcp_socket();
                ~initiative_tcp_socket();

            public:
                virtual bool on_connect_error(pilo::u32_t errcode);


            public:
                void set_peer_address(const char* addrName, pilo::u16_t port);
                void set_peer_address(const ipv4_address& addr);

                bool is_connected() const;
                bool set_reconnection_interval(pilo::u32_t secs);// 把重连时间设置为0，这样这个Socket就不会尝试重连了                     

                inline virtual EnumSocketType socket_type() const { return basic_socket::eST_InitiativeTCP; }

                virtual bool on_closed_state();

                virtual void reset();

                virtual pilo::core::string::fixed_astring<256> to_string() const;

            public:
                void on_start_working();

                inline bool is_would_block(pilo::os_error_number_t errcode)
                {
                #if defined(WIN32) || defined(WINDOWS)
                    return errcode == WSAEWOULDBLOCK;
                #else
                    return errcode == EINPROGRESS;
                #endif
                }

                // 主动连接，异步
                bool connect_async();

                // 使用 select 确定连接是否建立
                pilo::i32_t check_connected();

            public:
                pilo::u32_t  m_reconnection_interval;     // seconds
                pilo::tick64_t m_next_connnect_tick;
                pilo::tick64_t m_try_connecting_tick;
                EnumConnectionStage m_stage;

            };

        }
    }
}