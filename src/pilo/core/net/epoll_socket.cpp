#include "core/coredefs.hpp"
#if !defined(WIN32) && !defined(WINDOWS)

#include "core/datetime/datetime.hpp"
#include "epoll_socket.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {

            epoll_socket::epoll_socket()
            {
                m_in_epoll = false;
                m_socket_server = nullptr;
                m_writable = false;
                m_in_writing = false;
                //m_careOut = FALSE;
                //m_lastBlockTick = 0;
                this->reset();
            }

            epoll_socket::~epoll_socket()
            {
                
            }

            pilo::core::net::basic_socket_server* epoll_socket::socket_server()
            {
                return m_socket_server;
            }

            void epoll_socket::reset()
            {
                basic_socket::reset();

                m_in_epoll = false;
                m_socket_server = nullptr;
                m_writable = false;
                m_in_writing = false;                
            }

            void epoll_socket::on_start_working()
            {
                m_in_epoll = true;
                //m_writable = FALSE;
                m_in_writing = false;
                //m_careOut = FALSE;
                //m_lastBlockTick = 0;
                
                basic_socket::on_start_working();

            }

            bool epoll_socket::on_closed_state()
            {
                if(basic_socket::on_closed_state())
                {
                    return true;
                }

                if(m_in_epoll)
                {
                    ::pilo::tick64_t now = PiloGetTickCount64();
                    if(now - m_closed_tick > 5000)
                    {
                        // 5 seconds exceeds, I think that epoll does not recv an error notification
                        // so destroy it
                        return false;
                    }
                    else
                    {
                        return true;
                    }
                }
                return false;
            }

            void epoll_socket::enable_writable()
            {
                m_writable = true;
            }

            void epoll_socket::disable_writable()
            {
                m_writable = false;
            }

        }
    }
}

#endif