#include "passive_tcp_socket.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            bool passive_tcp_socket::attach(os_socket_fd_t sock)
            {
                this->reset();

                sockaddr_in remoteAddr;
                socklen_t socklen = sizeof(remoteAddr);
                int nRet = getpeername(sock, (sockaddr*)&remoteAddr, &socklen);
                if (nRet != 0)
                {
                    //TODO log this error
                    // DWORD errCode = GetLastError();
                    // Log(LOG_WARN, "warn: getpeername(%u) %s", sock, strerror_r2(errCode).c_str());
                    return false;
                }

                m_sock = sock;
                m_peer_address = ipv4_address::from_sockaddr(remoteAddr);

                return true;
            }

            pilo::core::string::fixed_astring<256> passive_tcp_socket::to_string() const
            {
                char cTmp[256] = { 0 };
                pilo::core::io::string_format_output(cTmp, sizeof(cTmp), "Server TCP Socket %u remote: %s",
                    m_sock != MC_INVALID_SOCK_FD ? m_sock : m_sock_dup,
                    ipv4_address::to_string(m_peer_address).c_str());
                return cTmp;
            }

        }
    }
}