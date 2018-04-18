#include "./initiative_tcp_socket.hpp"
#include "core/datetime/datetime.hpp"
#include "core/string/string_util.hpp"
#include "core/io/format_output.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {

            bool initiative_tcp_socket::is_connected() const
            {
                return (m_sock != MC_INVALID_SOCK_FD && m_stage == eCS_Connected);
            }

            bool initiative_tcp_socket::set_reconnection_interval(pilo::u32_t secs)
            {
                m_reconnection_interval = secs;
                return true;
            }

            bool initiative_tcp_socket::on_closed_state()
            {
                if (m_sock == MC_INVALID_SOCK_FD)
                {
                    if (m_stage != eCS_Attached)
                    {
                        m_stage = eCS_Closed;
                    }
                }

                switch (m_stage)
                {
                    case eCS_Attached:
                    {	// 开始一个异步的连接过程	
                        if (!this->connect_async())
                        {	// 失败了
                            m_next_connnect_tick = PiloGetTickCount64() + m_reconnection_interval * 1000;
                            m_stage = eCS_Closed;
                        }
                    } break;

                    case eCS_Closed:
                    {
                        pilo::tick64_t now = PiloGetTickCount64();

                        // 重连时间间隔为0表示不需要重连
                        if (m_reconnection_interval == 0)
                        {
                            return tcp_socket::on_closed_state();
                        }

                        // 时间没到，继续等待
                        if (now < m_next_connnect_tick)
                        {
                            return true;
                        }

                        // 开始一个异步的连接过程
                        if (!this->connect_async())
                        {	// 失败了
                            m_next_connnect_tick = PiloGetTickCount64() + m_reconnection_interval * 1000;
                            m_stage = eCS_Closed;
                        }
                    } break;
               
                    case eCS_Waiting:
                    {
                        int nRet = this->check_connected();
                        if (nRet < 0)
                        {	// 出错了
                            // 这里不能调用 KSocket::Close()，因为前面没有对应的 OnConnected()
                            // 仅仅 closesocket 就可以了
                            this->_close_nolock();
                            m_next_connnect_tick = PiloGetTickCount64() + m_reconnection_interval * 1000;
                            m_stage = eCS_Closed;
                        }
                        else if (nRet == 0)
                        {	// 连接过程中
                        }
                        else
                        {	// 连接成功了
                            m_stage = eCS_Connected;
                        }
                    } break;

                    case eCS_Connected:
                    {
                                       // 等待维护线程把这个对象移动到Active队列中
                    } break;

                    default:
                        M_ASSERT(false);
                        break;
                }

                return true;
            }

            void initiative_tcp_socket::reset()
            {
                m_next_connnect_tick = 0;
                m_try_connecting_tick = (pilo::tick64_t)-1;
                m_stage = eCS_Attached;
                tcp_socket::reset();
            }

            pilo::core::string::fixed_astring<256> initiative_tcp_socket::to_string() const
            {
                char cTmp[256];

                pilo::core::io::string_format_output(cTmp, sizeof(cTmp), 
                    "Client TCP Socket %u Peer:%s", m_sock, ipv4_address::to_string(m_peer_address).c_str());

                return cTmp;
            }

            void initiative_tcp_socket::on_start_working()
            {
                tcp_socket::on_start_working();
            }

            bool initiative_tcp_socket::connect_async()
            {
                M_ASSERT(m_sock == MC_INVALID_SOCK_FD);
                M_ASSERT(m_stage == eCS_Attached || m_stage == eCS_Closed);

                m_try_connecting_tick = PiloGetTickCount64();

                pilo::os_socket_fd_t sock = basic_socket::create_tcp_socket();
                if (sock == MC_INVALID_SOCK_FD)
                {
                    //DWORD errCode = GetLastError();
                    //TODO log
                    //Log(LOG_ERROR, "error: KSocket::CreateTcpSocket errCode:%u, %s", errCode, strerror_r2(errCode).c_str());
                    return false;
                }

                sockaddr_in peer = ipv4_address::to_sockaddr(m_peer_address);
                int socklen = sizeof(peer);

                int nRet = ::connect(sock, (sockaddr*)&peer, socklen);
                if (nRet == 0)
                {	// 立即完成了
                    m_sock = sock;
                    m_stage = eCS_Connected;
                    return true;
                }

                ::pilo::os_error_number_t errCode = get_last_os_error_number();
                if (this->is_would_block(errCode))
                {	// 为完成，进入等待状态
                    m_sock = sock;
                    m_stage = eCS_Waiting;
                    return true;
                }

                // 连接失败了
                this->on_connect_error(errCode);

                // 出错了
                // TODO
                //Log(LOG_DEBUG, "debug: connect errCode:%u, %s", errCode, strerror_r2(errCode).c_str());

                // 关闭 前面打开的SOcket
                pilo::core::net::close_socket(sock);
                return false;
            }

            pilo::i32_t initiative_tcp_socket::check_connected()
            {
                fd_set wfds; FD_ZERO(&wfds);
                fd_set efds; FD_ZERO(&efds);
                struct timeval timedout = { 0, 0 };

                FD_SET(m_sock, &wfds);
                FD_SET(m_sock, &efds);

                int nRet = select((int)m_sock + 1, NULL, &wfds, &efds, &timedout);
                if (nRet > 0)
                {
                    if (FD_ISSET(m_sock, &efds))
                    {
                        this->on_connect_error(9999);
                        return -1;
                    }
                    if (FD_ISSET(m_sock, &wfds))
                    {
                        // linux下面返回可写也不一定连接成功了，还需要其他的检查
                        int err = 0;
                        socklen_t len = sizeof(err);
                        getsockopt(m_sock, SOL_SOCKET, SO_ERROR, (char*)&err, &len);
                        if (!err)
                        {
                            return 1;
                        }
                        else
                        {
                            this->on_connect_error(9999);
                            return -1;
                        }
                    }

                    // ASSERT(FALSE); // 应该是不可达的
                    //TODO not reachable code
                    //Log(LOG_ERROR, "error: check connected, select return %d, check fail, %s", nRet, this->ToString().c_str());
                    this->on_connect_error(9999);
                    return -1;
                }
                else if (nRet == 0)
                {
                    return 0;
                }
                else
                {
                    return -1;
                }
            }

        }
    }
}