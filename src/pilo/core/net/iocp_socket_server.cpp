#include "core/coredefs.hpp"
#if defined(WIN32) || defined(WINDOWS)
#include "./iocp_socket_server.hpp"
#include "./initiative_tcp_socket.hpp"
#include "./passive_tcp_socket.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {

            socket_io_thread::socket_io_thread() : m_socket_server_ptr(nullptr)
            {

            }

            socket_io_thread::~socket_io_thread()
            {
                this->stop_sync(true);
            }

            pilo::i32_t socket_io_thread::on_run()
            {
                DWORD numberOfBytes;
                iocp_socket* pSocket;
                iocp_scoket_operation* pSockOperation;
                HANDLE hcp = m_socket_server_ptr->m_iocp_handle;

                HANDLE hThread = GetCurrentThread();
                SetThreadPriority(hThread, THREAD_PRIORITY_HIGHEST);

                while (! termination_is_requested())
                {
                    numberOfBytes = 0;
                    pSocket = NULL;
                    pSockOperation = NULL;

                    const DWORD waitTimeout = 300;

                    BOOL bRet = GetQueuedCompletionStatus(hcp, &numberOfBytes, (PULONG_PTR)&pSocket, (OVERLAPPED**)&pSockOperation, waitTimeout);
                    EnumGetQueuedCompletionStatusResult result = this->GetQueuedCompletionStatusResult(bRet, pSockOperation, numberOfBytes);
                    switch (result)
                    {
                        case eSuccessResult:
                        {
                            pSocket->on_success_operation(pSockOperation, numberOfBytes);
                        }                        
                        break;

                        case eTimeoutResult:
                        break;

                        case eFailIOResult:
                        {
                            pSocket->on_fail_operation(pSockOperation, numberOfBytes, GetLastError());
                        }                        
                        break;

                        case eSocketClosedResult:
                        {
                            pSocket->on_close_operation(pSockOperation);
                        }                        
                        break;
                        case eErrorResult:
                        {
                            //DWORD errCode = GetLastError();
                            //TODO log error
                            //Log(LOG_DEBUG, "debug: GetQueuedCompletionStatus errCode:%u, %s", errCode, strerror_r2(errCode).c_str());
                        }
                        break;
                    }
                } //while

                return 0;
            }


            socket_io_thread::EnumGetQueuedCompletionStatusResult socket_io_thread::GetQueuedCompletionStatusResult(BOOL bRet, iocp_scoket_operation* pOperation, DWORD numberOfBytes)
            {
                M_UNUSED(pOperation);

                if (bRet)
                {
                    return eSuccessResult;
                }

                DWORD errCode = GetLastError();
                if (m_socket_server_ptr != nullptr)
                {
                    if (errCode == ERROR_SUCCESS && numberOfBytes == 0)
                    {
                        return eSocketClosedResult;
                    }
                    else
                    {
                        return eFailIOResult;
                    }
                }
                else
                {
                    if (errCode == WAIT_TIMEOUT)
                    {
                        return eTimeoutResult;
                    }
                    else
                    {
                        return eErrorResult;
                    }
                }
            }

            iocp_maintenance_thread::iocp_maintenance_thread() : m_socket_server_ptr(nullptr), m_last_breathe_tick(0)
            {
                
            }

            iocp_maintenance_thread::~iocp_maintenance_thread()
            {
                this->stop_sync(true);
            }

            pilo::i32_t iocp_maintenance_thread::on_run()
            {
                return 0;
            }

            void iocp_maintenance_thread::process_active_queue(pilo::u64_t intervalTicks)
            {
                ULONGLONG now = GetTickCount64();
                size_t sockCapacity = m_socket_server_ptr->m_socket_capacity;
                iocp_socket** ppActiveSock = m_socket_server_ptr->m_active_sockets;
                for (size_t i = 0; i < sockCapacity; i++)
                {
                    iocp_socket* pSock = ppActiveSock[i];
                    if (pSock == nullptr)
                    {
                        continue;
                    }

                    // 如果对象为Closed状态，移到Closed队列中去
                    if (pSock->socket_file_descriptor() == INVALID_SOCKET)
                    {
                        m_socket_server_ptr->move_socket_to_close_list(i);
                        continue;
                    }

                    pSock->on_breathe(intervalTicks);

                    // 执行OnActiveState函数，对SOCKET的活动状态作定时检查
                    pSock->on_active_state();

                    // SOCKET 在 OnActiveState 中被关闭了
                    if (pSock->m_sock == INVALID_SOCKET) continue;

                    // 延迟关闭的时间到了
                    if (pSock->delay_close_ticks() && now >= pSock->delay_close_ticks())
                    {
                        pSock->close();
                        continue;
                    }

                    if (pSock->socket_type() == basic_socket::eST_ListenTCP)
                    {
                        continue;
                    }

                    // 这时SOCKET都是活跃状态的，发起发送操作
                    // 这个调用仅是以防万一，某种情况下，Send操作中断了，在这里又可以从新开始Send
                    // 每次Send函数调用的时候都会向IOCP发起Send操作
                    pSock->initiate_send_operation();
                }
            }

            void iocp_maintenance_thread::process_closed_queue(pilo::u64_t intervalTicks)
            {
                size_t sockCapacity = m_socket_server_ptr->m_socket_capacity;
                iocp_socket** ppClosedSock = m_socket_server_ptr->m_closed_sockets;
                for (size_t i = 0; i < sockCapacity; i++)
                {
                    iocp_socket* pSock = ppClosedSock[i];
                    if (pSock == nullptr)
                    {
                        continue;
                    }

                    pSock->on_breathe(intervalTicks);

                    // 根据OnClosedState的返回值决定是否销毁Socket对象
                    // TRUE 不销毁
                    // FALSE 销毁
                    if (!pSock->on_closed_state())
                    {
                        // 在JgSockServer::LockSockets的时候，可能会已经把这个Socket取走了
                        // 所以这里再加锁检查一次
                        {
                            pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(pSock->m_lock);
                            if (pSock->on_closed_state())
                            {
                                continue;// 在引用中，等下次吧
                            }

                            // 把这个Slot清空了，别的线程再也无法引用到这个Socket
                            ppClosedSock[i] = nullptr;
                            m_socket_server_ptr->m_managed_socket_count -= 1;
                        }

                        //TODO log
                        //Log(LOG_WARN, "warn: destroy %s", pSock->ToString().c_str());
                        pSock->on_detached();
                        pSock->destroy();
                        continue;
                    }

                    // ClientTcpSocket重连成功需要返回到Active队列中去
                    if (pSock->socket_type() == basic_socket::eST_InitiativeTCP)
                    {
                        initiative_tcp_socket* pClientSock = (initiative_tcp_socket*)pSock;
                        if (pClientSock->is_connected())
                        {
                            // 移到Active队列中
                            pilo::core::threading::mutex_locker<pilo::core::threading::recursive_mutex> guard(pSock->m_lock);
                            m_socket_server_ptr->move_socket_to_active_list(pSock->socket_index());
                            pSock->on_connected(); // pSock->m_cnnNotified = TRUE;
                        }
                    }
                }
            }


            //iocp_socket_server mothods:
            iocp_socket_server::iocp_socket_server()
            {
                m_io_thread_count = 10;
                m_socket_capacity = 100;
                m_managed_socket_count = 0;
                m_active_sockets = nullptr;
                m_closed_sockets = nullptr;
                m_iocp_handle = NULL;
            }

            iocp_socket_server::~iocp_socket_server()
            {
                this->finalize();
            }

            bool iocp_socket_server::initialize(size_t socketCapacity, size_t ioThreadNum)
            {
                //ASSERT(pDecl);
                M_ASSERT(m_iocp_handle == NULL);

                WSADATA wsaData;
                WSAStartup(MAKEWORD(2, 2), &wsaData);

                m_io_thread_count = ioThreadNum;
                m_socket_capacity = socketCapacity;

                m_iocp_handle = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, NULL, (DWORD) m_io_thread_count);
                if (NULL ==  m_iocp_handle)
                {
                    //TODO log
                    //Log(LOG_ERROR, "error: fail to create complete port");
                    return false;
                }

                m_io_threads = new socket_io_thread*[m_io_thread_count];
                M_ASSERT(m_io_threads);
                for (size_t i = 0; i < m_io_thread_count; i++)
                {
                    socket_io_thread*& pIoThread = m_io_threads[i];
                    pIoThread = new socket_io_thread(); M_ASSERT(nullptr != pIoThread);
                    pIoThread->m_socket_server_ptr = this;
                    pIoThread->execute();
                }

                m_active_sockets = new iocp_socket*[m_socket_capacity]; M_ASSERT(nullptr != m_active_sockets);
                memset(m_active_sockets, 0, sizeof(iocp_socket*)*m_socket_capacity);

                m_closed_sockets = new iocp_socket*[m_socket_capacity]; M_ASSERT(nullptr != m_closed_sockets);
                memset(m_closed_sockets, 0, sizeof(iocp_socket*)*m_socket_capacity);

                m_maintenance_thread.m_socket_server_ptr = this;
                m_maintenance_thread.execute();

                return true;
            }

            void iocp_socket_server::finalize()
            {
                m_maintenance_thread.stop_sync();

                if (nullptr != m_io_threads)
                {
                    for (size_t i = 0; i < m_io_thread_count; i++)
                    {
                        socket_io_thread* pThread = m_io_threads[i];
                        pThread->stop_sync();
                        delete pThread;
                    }
                    delete[] m_io_threads;
                    m_io_threads = nullptr;
                }

                if (m_closed_sockets != nullptr)
                {
                    for (size_t i = 0; i < m_socket_capacity; i++)
                    {
                        basic_socket* pSock = m_closed_sockets[i];
                        if (pSock == nullptr) 
                        {
                            continue;
                        }
                        pSock->close();
                        pSock->destroy();
                        m_closed_sockets[i] = nullptr;
                    }
                    delete[] m_closed_sockets;
                    m_closed_sockets = nullptr;
                }

                if (m_active_sockets != nullptr)
                {
                    for (size_t i = 0; i < m_socket_capacity; i++)
                    {
                        basic_socket* pSock = m_active_sockets[i];
                        if (pSock == nullptr)
                        {
                            continue;
                        }
                        pSock->close();
                        pSock->destroy();
                        m_active_sockets[i] = nullptr;
                    }
                    delete[] m_active_sockets;
                    m_active_sockets = nullptr;
                }

                if (m_iocp_handle != NULL)
                {
                    ::CloseHandle(m_iocp_handle);
                    m_iocp_handle = NULL;
                }

                m_managed_socket_count = 0;
                m_socket_capacity = 0;
                m_io_thread_count = 0;
                //this->SetPacketDeclare(NULL);

                WSACleanup();
            }

            bool iocp_socket_server::attach(basic_socket* socket_ptr)
            {
                iocp_socket* pIocpSock = (iocp_socket*)socket_ptr;
                pIocpSock->m_socket_server = this;

                bool bRet = false;
                {
                    pilo::core::threading::advanced_mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);
                    socket_ptr->inc_ref_count();

                    switch (socket_ptr->socket_type())
                    {
                        case basic_socket::eST_ListenTCP:
                            bRet = this->attach_tcp_listen_socket(pIocpSock);
                            break;
                        case basic_socket::eST_PassiveTCP:
                            bRet = this->attach_tcp_passive_socket(pIocpSock);
                            break;
                        case basic_socket::eST_UDP:
                            bRet = this->attach_udp_socket(pIocpSock);
                            break;
                        case basic_socket::eST_InitiativeTCP  :
                            bRet = this->attach_tcp_initiative_socket(pIocpSock);
                            break;
                        default:
                            M_ASSERT(false);
                            break;
                    }
                    socket_ptr->dec_ref_count();
                }
                if (bRet && !this->can_accept())
                {
                    //TODO LOG this error
                    //Log(LOG_WARN, "warn: sockserver full, can not accept more socket");
                }

                return bRet;
            }

            void iocp_socket_server::move_socket_to_active_list(size_t socket_index)
            {
                pilo::core::threading::advanced_mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                M_ASSERT(nullptr == m_active_sockets[socket_index]);
                M_ASSERT(nullptr != m_closed_sockets[socket_index]);

                iocp_socket* pSock = m_closed_sockets[socket_index];
                M_ASSERT(pSock->socket_type() == basic_socket::eST_InitiativeTCP);

                HANDLE hcp = CreateIoCompletionPort((HANDLE)pSock->m_sock, m_iocp_handle, (ULONG_PTR)pSock, (DWORD) m_io_thread_count);
                if (NULL == hcp)
                {
                    //TODO log
                    //DWORD errCode = GetLastError();
                    //Log(LOG_WARN, "warn: attach %s to iocp, %s", pSock->ToString().c_str(), strerror_r2(errCode).c_str());
                    guard.unlock();
                    pSock->close();
                    return;
                }

                m_active_sockets[socket_index] = m_closed_sockets[socket_index];
                m_closed_sockets[socket_index] = nullptr;

                guard.unlock();

                pSock->on_start_working();

                pSock->initiate_receive_operation();
                pSock->initiate_send_operation();
            }

            void iocp_socket_server::move_socket_to_close_list(size_t socket_index)
            {
                pilo::core::threading::advanced_mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);
                
                M_ASSERT(nullptr != m_active_sockets[socket_index]);
                M_ASSERT(nullptr == m_closed_sockets[socket_index]);

                iocp_socket* pSock = m_active_sockets[socket_index];

                if (pSock->socket_type() == basic_socket::eST_InitiativeTCP)
                {
                    initiative_tcp_socket* pClientSock = (initiative_tcp_socket*)pSock;
                    pClientSock->m_next_connnect_tick = GetTickCount64() + pClientSock->m_reconnection_interval * 1000;
                }

                m_closed_sockets[socket_index] = pSock;
                m_active_sockets[socket_index] = nullptr;
                
            }

            bool iocp_socket_server::attach_tcp_listen_socket(iocp_socket* psocket)
            {
                size_t i;
                pilo::core::threading::advanced_mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                for (i = 0; i < m_socket_capacity; i++)
                {
                    if (nullptr == m_active_sockets[i] && nullptr == m_closed_sockets[i])
                    {
                        break;
                    }
                }

                if (i >= m_socket_capacity)
                {// ????
                    //TODO log error
                    //Log(LOG_WARN, "warn: socket count reach %d, can not add more tcp listen socket", m_socket_capacity);
                    return false;
                }

                HANDLE hcp = CreateIoCompletionPort((HANDLE)psocket->m_sock, m_iocp_handle, (ULONG_PTR)psocket, (DWORD) m_io_thread_count);
                if (!hcp)
                {
                    guard.unlock();
                    //TODO log error
                    //DWORD errCode = GetLastError();
                    //Log(LOG_WARN, "warn: attach %s to iocp, %s", pSock->ToString().c_str(), strerror_r2(errCode).c_str());

                    psocket->close();
                    return false;
                }

                psocket->m_socket_index = i;
                m_active_sockets[i] = psocket;
                m_managed_socket_count += 1;

                guard.unlock();

                psocket->on_start_working();

                // the socket client maybe initiate operation in this method
                // so we call OnStartWork first.
                psocket->on_attached();

                psocket->initiate_accept_operation();

                return true;
            }

            bool iocp_socket_server::attach_tcp_initiative_socket(iocp_socket* psocket)
            {
                initiative_tcp_socket* pClientSocket = (initiative_tcp_socket*)psocket;
                pClientSocket->m_stage = initiative_tcp_socket::eCS_Attached;

                size_t i;

                pilo::core::threading::advanced_mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                for (i = 0; i < m_socket_capacity; i++)
                {
                    if (nullptr == m_active_sockets[i] && nullptr == m_closed_sockets[i]) 
                    {
                        break;
                    }
                }

                if (i >= m_socket_capacity)
                {
                    //TODO log error                    
                    //Log(LOG_WARN, "warn: socket count reach %d, can not add more client tcp socket", m_socketCapacity);
                    return false;
                }

                tcp_socket* pTcpSocket = (tcp_socket*)psocket;
                pTcpSocket->m_outgoing_io_buffer.set_pool(m_buffer_pool);

                psocket->m_socket_index = i;
                m_active_sockets[i] = psocket;
                m_managed_socket_count += 1;

                guard.unlock();

                psocket->on_attached();
                return true;
            }

            bool iocp_socket_server::attach_tcp_passive_socket(iocp_socket* psocket)
            {
                size_t i;
                pilo::core::threading::advanced_mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                for (i = 0; i < m_socket_capacity; i++)
                {
                    if (nullptr == m_active_sockets[i] && nullptr == m_closed_sockets[i])
                    {
                        break;
                    }
                }

                if (i >= m_socket_capacity)
                {
                    //TODO log error                    
                    //Log(LOG_WARN, "warn: socket count reach %d, can not add more server tcp socket", m_socketCapacity);
                    return false;
                }

                HANDLE hcp = CreateIoCompletionPort((HANDLE)psocket->m_sock, m_iocp_handle, (ULONG_PTR)psocket, (DWORD) m_io_thread_count);
                if (!hcp)
                {
                    guard.unlock();
                    //TODO log error 
                    //DWORD errCode = GetLastError();
                    //Log(LOG_WARN, "warn: attach %s to iocp, %s", pSock->ToString().c_str(), strerror_r2(errCode).c_str());

                    psocket->close();
                    return false;
                }

                tcp_socket* pTcpSocket = (tcp_socket*)psocket;
                pTcpSocket->m_outgoing_io_buffer.set_pool(m_buffer_pool);

                psocket->m_socket_index = i;
                m_active_sockets[i] = psocket;
                m_managed_socket_count += 1;

                guard.unlock();

                psocket->on_start_working();
                psocket->on_attached();
                psocket->on_connected(); // pSock->m_cnnNotified = TRUE;

                passive_tcp_socket* pServerTcpSock = (passive_tcp_socket*) psocket;
                pServerTcpSock->initiate_receive_operation();
                pServerTcpSock->initiate_send_operation();

                return true;
            }

            bool iocp_socket_server::attach_udp_socket(iocp_socket* psocket)
            {
                size_t i;

                // 这个SOCKET尚未绑定到端口，自动找一个
                if (psocket->m_sock == INVALID_SOCKET)
                {
                    udp_socket* pUdpSock = (udp_socket*)psocket;
                    pUdpSock->auto_listening_from_port();
                }

                {
                    pilo::core::threading::advanced_mutex_locker<pilo::core::threading::recursive_mutex> guard(m_lock);

                    for (i = 0; i < m_socket_capacity; i++)
                    {
                        if (nullptr == m_active_sockets[i] && nullptr == m_closed_sockets[i]) 
                        {
                            break;
                        }
                    }
                    if (i >= m_socket_capacity)
                    {
                        //TODO log error
                        //Log(LOG_WARN, "warn: socket count reach %d, can not add more udp socket", m_socketCapacity);
                        return false;
                    }

                    HANDLE hcp = CreateIoCompletionPort((HANDLE)psocket->m_sock, m_iocp_handle, (ULONG_PTR)psocket, (DWORD) m_io_thread_count);
                    if (0 == hcp)
                    {
                        //TODO log error  
                        /*DWORD errCode = GetLastError();
                        Log(LOG_WARN, "warn: attach %s to iocp, %s", pSock->ToString().c_str(), strerror_r2(errCode).c_str());*/

                        psocket->close();
                        return false;
                    }

                    udp_socket* pUSocket = (udp_socket*)psocket;
                    pUSocket->m_outgoing_io_buffer.set_pool(m_buffer_pool);

                    pUSocket->m_socket_index = i;
                    m_active_sockets[i] = psocket;
                    m_managed_socket_count += 1;
                }

                psocket->on_attached();
                psocket->on_start_working();

                

                udp_socket* pUSocket = (udp_socket*)psocket;
                pUSocket->initiate_receive_operation();
                pUSocket->initiate_send_operation();

                // UDP 本来没有 Connect 事件，这只是通知 UDP socket 可以开始 IO 的时机
                pUSocket->on_connected(); //pSock->m_cnnNotified = TRUE;

                return true;
            }

        }
    }
}



#endif //defined(WIN32) || defined(WINDOWS)