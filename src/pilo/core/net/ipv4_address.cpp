#include "ipv4_address.hpp"
#include "core/string//string_util.hpp"
#include "core/io/format_output.hpp"

#ifdef WINDOWS
#include <WS2tcpip.h>
#endif

namespace pilo
{
    namespace core
    {
        namespace net
        {
            bool ipv4_address::operator==(const ipv4_address& o) const
            {
                return m_ip == o.m_ip && m_port == o.m_port;
            }

            sockaddr_in ipv4_address::to_sockaddr() const
            {
                sockaddr_in sockAddr;
                sockAddr.sin_family = AF_INET;
                sockAddr.sin_addr.s_addr = htonl(m_ip);
                sockAddr.sin_port = htons(m_port);
                return sockAddr;
            }

            sockaddr_in ipv4_address::to_sockaddr(const ipv4_address& addr)
            {
                sockaddr_in sockAddr;
                sockAddr.sin_family = AF_INET;
                sockAddr.sin_addr.s_addr = htonl(addr.m_ip);
                sockAddr.sin_port = htons(addr.m_port);
                return sockAddr;
            }

            pilo::core::net::ipv4_address ipv4_address::from_string(const char* addrStr)
            {
                ipv4_address sockAddr = { 0, 0 };

                if (!pilo::core::string::string_util::ipv4_address_cstr_to_int(sockAddr.m_ip, sockAddr.m_port, addrStr))
                {
                   ;
                }

                return sockAddr;
            }

            pilo::core::string::fixed_astring<64> ipv4_address::to_string() const
            {
                char cTmp[48];
                sockaddr_in sockAddr = this->to_sockaddr();
                char strip[16];
                inet_ntop(AF_INET, (void *)&sockAddr.sin_addr, strip, sizeof(strip));
                pilo::core::io::string_format_output(cTmp, sizeof(cTmp), "%s:%u", strip, m_port);
                return cTmp;
            }

            pilo::core::string::fixed_astring<64> ipv4_address::to_string(const ipv4_address& addr)
            {
                char cTmp[64];
                sockaddr_in sockAddr = ipv4_address::to_sockaddr(addr);
                char strip[16];
                inet_ntop(AF_INET, (void *)&sockAddr.sin_addr, strip, sizeof(strip));
                pilo::core::io::string_format_output(cTmp, sizeof(cTmp), "%s:%u", strip, addr.m_port);
                return cTmp;
            }

            pilo::core::string::fixed_astring<64> ipv4_address::to_string(pilo::u32_t ip)
            {
                pilo::u32_t nip = htonl(ip);
                char strip[16];
                inet_ntop(AF_INET, (void *)&nip, strip, sizeof(strip));
                pilo::core::string::fixed_astring<64> ret(strip);
                return ret;
            }

            pilo::core::net::ipv4_address ipv4_address::from_sockaddr(const sockaddr_in& sockAddr)
            {
                pilo::core::net::ipv4_address addr;
                addr.m_ip = ntohl(sockAddr.sin_addr.s_addr);
                addr.m_port = ntohs(sockAddr.sin_port);
                return addr;
            }

        }
    }    
}