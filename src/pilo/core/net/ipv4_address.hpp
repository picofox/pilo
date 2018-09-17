#pragma once

#include "core/coredefs.hpp"
#include "net_sys_header.hpp"
#include "core/string/fixed_astring.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
#pragma pack(push, 1)

            struct ipv4_address
            {
                pilo::u32_t m_ip;
                pilo::u16_t m_port;

                ///
                bool operator== (const ipv4_address& o) const;
                sockaddr_in to_sockaddr() const;
                pilo::core::string::fixed_astring<64> to_string() const;

                ///
                static ipv4_address from_sockaddr(const sockaddr_in& sockAddr);
                static sockaddr_in to_sockaddr(const ipv4_address& addr);
                static ipv4_address from_string(const char* addrStr); // 192.168.10.46:9998
                static pilo::core::string::fixed_astring<64> to_string(const ipv4_address& addr);
                static pilo::core::string::fixed_astring<64> to_string(pilo::u32_t ip);
            };

            struct udp_packet_header
            {
                ipv4_address m_remote_socket_address;
                pilo::u16_t  m_length;
            };

#pragma pack(pop)
        }
    }    
}