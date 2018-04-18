#include "core/coredefs.hpp"

#if defined(WIN32) || defined(WINDOWS)

#include "iocp_socket.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {

            iocp_socket::iocp_socket() : m_socket_server(nullptr)
            {

            }

            iocp_socket::~iocp_socket()
            {

            }

            basic_socket_server* iocp_socket::socket_server()
            {
                return (basic_socket_server*) m_socket_server;
            }

            void iocp_socket::reset()
            {
                m_socket_server = nullptr;
                basic_socket::reset();
            }

            void iocp_socket::on_start_working()
            {
                m_ready_for_io = true;
                basic_socket::on_start_working();
            }

        }
    }
}

#endif //defined(WIN32) || defined(WINDOWS)