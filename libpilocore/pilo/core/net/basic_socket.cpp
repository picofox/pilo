
#include "basic_socket.hpp"
#include "./socket_platform.hpp"
#include "core/datetime/datetime.hpp"

#if defined(WIN32) || defined(WINDOWS)
#include <WS2tcpip.h>
#endif

namespace pilo
{
    namespace core
    {
        namespace net
        {
            
            basic_socket::basic_socket() : m_sock(MC_INVALID_SOCK_FD)
            {
                basic_socket::reset();
            }

            basic_socket::~basic_socket()
            {
                this->_close_nolock();
            }

            pilo::os_socket_fd_t basic_socket::create_tcp_socket()
            {
#if defined(WIN32) || defined(WINDOWS)
                os_socket_fd_t sock = WSASocketW(AF_INET, SOCK_STREAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
#else
                os_socket_fd_t sock = socket(AF_INET, SOCK_STREAM, 0);
#endif
                if (sock != MC_INVALID_SOCK_FD)
                {
                    basic_socket::set_none_blocking_mode(sock);
                    basic_socket::ignore_sigpipe(sock);
                }
                return sock;
            }

            pilo::os_socket_fd_t basic_socket::create_udp_socket()
            {
#if defined(WIN32) || defined(WINDOWS)
                os_socket_fd_t sock = WSASocketW(AF_INET, SOCK_DGRAM, IPPROTO_IP, NULL, 0, WSA_FLAG_OVERLAPPED);
#else
                os_socket_fd_t sock = socket(AF_INET, SOCK_DGRAM, 0);
#endif
                if (sock != MC_INVALID_SOCK_FD)
                {
                    basic_socket::set_none_blocking_mode(sock);
                    basic_socket::ignore_sigpipe(sock);
                }
                return sock;
            }

            bool basic_socket::set_none_blocking_mode(os_socket_fd_t fd)
            {
#if defined(WIN32) || defined(WINDOWS)
                unsigned long ul = 1;
                int nRet = ioctlsocket(fd, FIONBIO, (unsigned long*)&ul);
#else
                int oldFlags = fcntl(fd, F_GETFL);
                if (oldFlags == -1)
                {
                    os_error_number_t errCode = get_last_os_error_number();
                    M_UNUSED(errCode);
                    //TODO log this
                    //Log(LOG_ERROR, "error: get socket flag, %s", strerror_r2(errCode).c_str());
                    oldFlags = 0;
                }
                int nRet = fcntl(fd, F_SETFL, oldFlags | O_NONBLOCK);
#endif

                if (nRet != 0)
                {
                   // DWORD errCode = GetLastError();
                    //TODO log
                    //Log(LOG_ERROR, "error: set socket %u non block, %s", sock, strerror_r2(errCode).c_str());
                    return false;
                }
                return true;
            }

            bool basic_socket::ignore_sigpipe(os_socket_fd_t fd)
            {        
                M_UNUSED(fd);
#if defined(WIN32) || defined(WINDOWS)
                
                return true;
#else
                //int set = 1;
                //int nRet = setsockopt(fd, SOL_SOCKET, SO_NOSIGPIPE, (void *)&set, sizeof(int));
                //return nRet != -1;
                return true;
#endif
            }

#if defined(WIN32) || defined(WINDOWS)
            pilo::u32_t basic_socket::get_ipv4_by_name(const char* addrName)
            {
                struct in_addr s;
                inet_pton(AF_INET, addrName, (void *)&s);
                unsigned long ip = s.s_addr;
                if (ip != INADDR_NONE) return ntohl(ip);

                struct addrinfo hints;
                struct addrinfo *res=nullptr, *cur=nullptr;
                int ret;
                pilo::u32_t nips[64];
                struct sockaddr_in *addr = nullptr;
                int i = 0;

                memset(&hints, 0, sizeof(struct addrinfo));
                hints.ai_family = AF_INET; /* Allow IPv4 */
                hints.ai_flags = AI_PASSIVE; /* For wildcard IP address */
                hints.ai_protocol = 0; /* Any protocol */
                hints.ai_socktype = SOCK_STREAM;

                ret = ::getaddrinfo(addrName, nullptr,&hints,&res);
                if (ret == -1) return 0xFFFFFFFF;

                for (cur = res; cur != NULL; cur = cur->ai_next) 
                {
                    addr = (struct sockaddr_in *)cur->ai_addr;
                    nips[i++] = addr->sin_addr.s_addr;
                }

                pilo::tick32_t now = PiloGetTickCount32();
                if (now & 0x80000000) now &= 0x7fffffff;
                int idx = now % i;
                pilo::u32_t nip = nips[idx];
                return ntohl(nip);                 
            }
#else


            pilo::u32_t basic_socket::get_ipv4_by_name(const char* addrName)
            {

                struct in_addr s;
                inet_pton(AF_INET, addrName, (void *)&s);
                unsigned long ip = s.s_addr;
                if (ip != INADDR_NONE) return ntohl(ip);

                struct hostent* pHostent = gethostbyname(addrName);
                if (!pHostent)
                {
                    // DWORD errCode = GetLastError();
                    //Log(LOG_WARN, "warn: gethostname %s, %s", addrName, strerror_r2(errCode).c_str());
                    //TODO log
                    return 0;
                }

                if (pHostent->h_addrtype != AF_INET)
                {
                    //TODO log
                    //Log(LOG_WARN, "warn: %s not a AF_INET address", addrName);
                    return 0;
                }

                pilo::u32_t i;
                pilo::u32_t nips[64];
                for (i = 0; i < 64 && pHostent->h_addr_list[i] != NULL; i++)
                {
                    nips[i] = *(pilo::u32_t*)pHostent->h_addr_list[i];
                }

                pilo::tick32_t now = PiloGetTickCount32();
                if (now & 0x80000000) now &= 0x7fffffff;

                int idx = now % i;
                pilo::u32_t nip = nips[idx];
                return ntohl(nip);
            }
#endif

            void basic_socket::set_sockopt_send_buffer_size(socklen_t bytes)
            {
                pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                socklen_t sendbuflen = bytes;
                socklen_t len = sizeof(sendbuflen);
                ::setsockopt(m_sock, SOL_SOCKET, SO_SNDBUF, (const char*)&sendbuflen, len);
            }

            void basic_socket::inc_ref_count()
            {
                pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);
                m_ref_count ++;
            }

            void basic_socket::dec_ref_count()
            {
                pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);
                m_ref_count--;
            }

            size_t basic_socket::ref_count()
            {
                pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);
                return m_ref_count;
            }

            pilo::core::net::ipv4_address basic_socket::local_address() const
            {
                pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                pilo::core::net::ipv4_address sockAddr = { 0, 0 };
                if (m_sock == MC_INVALID_SOCK_FD) 
                {
                    return sockAddr;
                }

                sockaddr_in localAddr;
                socklen_t addrLen = sizeof(localAddr);
                int nRet = getsockname(m_sock, (sockaddr*)&localAddr, &addrLen);
                if (nRet == -1) 
                {
                    return sockAddr;
                }

                return ipv4_address::from_sockaddr(localAddr);
            }

            pilo::tick64_t basic_socket::get_idle_ticks() const
            {
                pilo::tick64_t now = PiloGetTickCount64();
                pilo::tick64_t sendIdleTicks = now - m_latest_send_tick;
                pilo::tick64_t recvIdleTicks = now - m_latest_recv_tick;

                if (sendIdleTicks > recvIdleTicks)
                    return sendIdleTicks;
                return recvIdleTicks;
            }

            pilo::tick64_t basic_socket::get_latest_send_tick() const
            {
                //pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                return m_latest_send_tick;
            }

            pilo::tick64_t basic_socket::get_latest_recv_tick() const
            {
                //pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                return m_latest_recv_tick;
            }

            size_t basic_socket::get_send_speed() const
            {
                //pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                pilo::tick64_t now = PiloGetTickCount64();
                pilo::tick64_t workTicks = now - m_start_working_tick;
                return (size_t)(m_bytes_sent / workTicks);
            }

            size_t basic_socket::get_recv_speed() const
            {
                //pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                pilo::tick64_t now = PiloGetTickCount64();
                pilo::tick64_t workTicks = now - m_start_working_tick;
                return (size_t)(m_bytes_received / workTicks);
            }

            bool basic_socket::close()
            {
                bool done = false;
                if (m_sock != MC_INVALID_SOCK_FD)
                {
                    pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);
                    if (m_sock == MC_INVALID_SOCK_FD)
                    {
                        return false;
                    }

                    done = true;
                    m_sock_dup = m_sock;
                    m_closed_tick = PiloGetTickCount64();

                    m_ref_count ++; //inc ref count to ensure this will not be destroy in on_close event handle

                    m_ready_for_io = false; //now any further io is NOT allowed

                    this->_close_nolock(); //actually closesocket now
                }

                if (done)
                {
                    this->on_closed();
                    this->dec_ref_count();
                }

                return done;
                
            }

            bool basic_socket::close_delayed(size_t delaySecs)
            {
                pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                if (m_sock == MC_INVALID_SOCK_FD)
                {
                    return false;
                }

                if (delaySecs > 0)
                {
                    m_delay_close_ticks = PiloGetTickCount64() + delaySecs * 1000;
                }
                else
                {	// shutdown read and write

                    if (!delaySecs) 
                    {
                        ::pilo::core::net::shutdown_socket(m_sock, ::pilo::core::net::eSSH_Neither);
                    }
                    else 
                    {
                        //TOCHECK no more what exactly?
                        ::pilo::core::net::shutdown_socket(m_sock, ::pilo::core::net::eSSH_NoMoreSend);
                    }
                }

                return true;
            }

            void basic_socket::pause_send(pilo::tick64_t ticks)
            {
                pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                m_send_pause_ticks += ticks;
                m_send_paused = true;
            }            

            void basic_socket::reset()
            {
                this->close();

                //m_userData = 0; //user is responsible for reset userdata

                pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);
                m_ready_for_io = false;

                m_socket_index = MC_INVALID_SIZE;
                m_ref_count = 0;

                m_bytes_sent = 0;
                m_bytes_received = 0;
                m_start_working_tick = 0;
                m_latest_send_tick = 0;
                m_latest_recv_tick = 0;
                m_closed_tick = 0;

                m_delay_close_ticks = 0;
                m_debug_state = false;

                m_send_paused = false;
                m_send_pause_ticks = 0;
                m_notified_conncted = false;
            }

            void basic_socket::destroy()
            {
                this->_destroy_by_delete();
            }

            bool basic_socket::on_active_state()
            {
                return true;
            }

            void basic_socket::on_breathe(pilo::tick64_t ticks_interval)
            {
                if (m_send_paused)
                {
                    if (m_send_pause_ticks <= ticks_interval)
                    {
                        m_send_pause_ticks = 0;
                        m_send_paused = false;
                    }
                    else
                    {
                        m_send_pause_ticks -= ticks_interval;
                    }
                }
            }
                
            bool basic_socket::on_connected()
            {
                return true;
            }

            bool basic_socket::on_closed()
            {
                return true;
            }

            void basic_socket::on_start_working()
            {
                M_ASSERT(m_sock != MC_INVALID_SOCK_FD);
                //m_readyForIO = TRUE;
                m_bytes_sent = 0;
                m_bytes_received = 0;
                m_start_working_tick = PiloGetTickCount64();
                m_latest_send_tick = 0;
                m_latest_recv_tick = 0;
                m_closed_tick = 0;
                m_delay_close_ticks = 0;
            }

            void basic_socket::_close_nolock()
            {
                if (m_sock != MC_INVALID_SOCK_FD)
                {
                    if (m_debug_state)
                    {
                        //TODO LOG
                        //Log(LOG_ERROR, "sockDebug: %s close", this->ToString().c_str());                        
                    }

                    pilo::core::net::close_socket(m_sock);
                    m_sock = MC_INVALID_SOCK_FD;
                }
            }

            void basic_socket::_destroy_by_delete()
            {
                delete this;
            }

            bool basic_socket::on_closed_state()
            {
                pilo::tick64_t nowTick = PiloGetTickCount64();
                if ((nowTick - m_closed_tick) < 5000) return true;
                return m_ref_count > 0;
            }

            bool basic_socket::on_attached()
            {
                return true;
            }

            bool basic_socket::on_detached()
            {
                return true;
            }

            bool basic_socket::on_invalid_packet(const pilo::core::net::ipv4_address* pFrom, EnumErrorPacketReason reason)
            {
                M_UNUSED(pFrom);
                M_UNUSED(reason);
                //TODO
                //Log(LOG_WARN, "warn: %s invalid packet from %s, reason:%u, close it",      this->ToString().c_str(),           KSocketAddress::ToString(*pFrom).c_str(),      reason);
                this->close();
                return true;
            }

            bool basic_socket::on_sending_data(const void* data, size_t len, const pilo::core::net::ipv4_address* target)
            {
                M_UNUSED(data);
                M_UNUSED(len);
                M_UNUSED(target)
                return true;
            }

            size_t basic_socket::on_receiving_data(const void* data, size_t len, const pilo::core::net::ipv4_address* from)
            {
                M_UNUSED(data);
                M_UNUSED(len);
                M_UNUSED(from)
                return len;
            }

            bool basic_socket::on_socket_error(pilo::error_number_t errCode)
            {
                M_UNUSED(errCode);
                //TODO
               // Log(LOG_DEBUG, "debug: %s socket error, close it, %s", this->ToString().c_str(), strerror_r2(errCode).c_str());
                this->close();
                return true;
            }

            void basic_socket::on_would_block()
            {
                return ;
            }

            bool basic_socket::on_accepted(os_socket_fd_t sock)
            {
                M_UNUSED(sock);
                M_ASSERT(this->socket_type() == basic_socket::eST_ListenTCP);
                //TODO log this info
                //Log(LOG_INFO, "info: %s accept a new socket %u, you should create new socket object here",    this->ToString().c_str(), sock);
                return true;
            }

            

        }
    }
}