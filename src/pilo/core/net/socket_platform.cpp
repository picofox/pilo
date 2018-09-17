#include "./socket_platform.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
#       if defined(WIN32) || defined(WINDOWS)
            ::pilo::EnumErrorCode close_socket(os_socket_fd_t socket)
            {
                if (SOCKET_ERROR == ::closesocket(socket))
                {
                    return pilo::EC_CLOSE_SOCKET_ERROR;
                }
                return pilo::EC_OK;
            }

            ::pilo::EnumErrorCode shutdown_socket(os_socket_fd_t socket, EnumSocketShutdownHow how)
            {
                int local_how = 0;
                if (how == ::pilo::core::net::eSSH_Neither)
                {
                    local_how = SD_BOTH;
                }
                else if (how == ::pilo::core::net::eSSH_NoMoreSend)
                {
                    local_how = SD_SEND;
                }
                else if (how == ::pilo::core::net::eSSH_NoMoreRecv)
                {
                    local_how = SD_RECEIVE;
                }
                int ret = ::shutdown(socket, local_how);
                if (ret == SOCKET_ERROR)
                {
                    return pilo::EC_SHUTDOWN_SOCKET_ERROR;
                }
                return pilo::EC_OK;
            }
#       else
            ::pilo::EnumErrorCode close_socket(os_socket_fd_t socket)
            {
                if (-1 == ::close(socket))
                {
                    return pilo::EC_CLOSE_SOCKET_ERROR;
                }
                return pilo::EC_OK;
            }

            ::pilo::EnumErrorCode shutdown_socket(os_socket_fd_t socket, EnumSocketShutdownHow how)
            {
                int local_how = 0;
                if (how == ::pilo::core::net::eSSH_Neither)
                {
                    local_how = SHUT_RDWR;
                }
                else if (how == ::pilo::core::net::eSSH_NoMoreSend)
                {
                    local_how = SHUT_WR;
                }
                else if (how == ::pilo::core::net::eSSH_NoMoreRecv)
                {
                    local_how = SHUT_RD;
                }
                int ret = ::shutdown(socket, local_how);
                if (ret == -1)
                {
                    return pilo::EC_SHUTDOWN_SOCKET_ERROR;
                }
                return pilo::EC_OK;
            }

#       endif //defined(WIN32) || defined(WINDOWS)

            

        }
    }
}