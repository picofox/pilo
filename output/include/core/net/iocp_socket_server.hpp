#pragma once
#include "core/coredefs.hpp"

#if defined(WIN32) || defined(WINDOWS)
#include "core/threading/basic_thread.hpp"
#include "core/net/iocp_socket.hpp"



namespace pilo
{
    namespace core
    {
        namespace net
        {
            class iocp_socket_server;

            /**
            * Socket IO 线程
            * 如果是 IOCP, 则执行 GetQueuedCompletionStatus
            * 如果是 epoll, 待定
            *
            */
            class socket_io_thread : public pilo::core::threading::basic_thread
            {
            public:
                enum EnumGetQueuedCompletionStatusResult
                {
                    eSuccessResult,
                    eTimeoutResult,
                    eFailIOResult,
                    eSocketClosedResult,
                    eErrorResult,
                };

            public:
                socket_io_thread();
                ~socket_io_thread();

            public:
                virtual i32_t on_run();

            protected:
                EnumGetQueuedCompletionStatusResult GetQueuedCompletionStatusResult(BOOL bRet, iocp_scoket_operation* pOperation, DWORD numberOfBytes);

            public:
                iocp_socket_server* m_socket_server_ptr;
            };

            /**
            * Socket维护线程
            *
            * ActiveSocket队列
            *     所有的Socket在Attach后都是先放到Active队列中，由维护线程
            *     根据Socket的当前状态把它移到Close队列中
            *     如果是ClientTcpSocket，为连接，转移到CloseSocket队列中
            *     不在发送操作中，但是有发送包的，发起一个发送操作
            *
            * ClosedSocket队列
            *     执行Socket的OnClosedState, 如果没有未完成的操作，释放Socket对象
            *
            */
            class iocp_maintenance_thread : public pilo::core::threading::basic_thread
            {
            public:
                iocp_maintenance_thread();
                ~iocp_maintenance_thread();

            public:
                virtual i32_t on_run();

            private:
                void process_active_queue(pilo::u64_t intervalTicks);
                void process_closed_queue(pilo::u64_t intervalTicks);

            public:
                iocp_socket_server* m_socket_server_ptr;
                ULONGLONG m_last_breathe_tick;
            };

            class iocp_socket_server : public basic_socket_server
            {
            public:
                iocp_socket_server();
                virtual ~iocp_socket_server();

            public:
                virtual bool initialize(size_t socketCapacity, size_t ioThreadNum);
                virtual void finalize();

                // 外部线程调用，可能是多个线程并发的
                virtual bool attach(basic_socket* socket_ptr) ;

                inline size_t socket_capacity() const { return m_socket_capacity; }
                inline size_t managed_socket_count() const {return m_managed_socket_count; }
                inline bool can_accept() const { return (m_managed_socket_count < m_socket_capacity); }

            public:
                void move_socket_to_active_list(size_t socket_index);
                void move_socket_to_close_list(size_t socket_index);

                bool attach_tcp_listen_socket(iocp_socket* psocket);
                bool attach_tcp_initiative_socket(iocp_socket* psocket);
                bool attach_tcp_passive_socket(iocp_socket* psocket);
                bool attach_udp_socket(iocp_socket* psocket);

            public:
                size_t  m_io_thread_count;
                size_t  m_socket_capacity;

                // 当前管理了多少Socket
                size_t m_managed_socket_count;

                socket_io_thread** m_io_threads;
                iocp_maintenance_thread m_maintenance_thread;

            public:
                pilo::core::threading::recursive_mutex   m_lock;                
                iocp_socket** m_active_sockets;
                iocp_socket** m_closed_sockets;

            public:
                HANDLE m_iocp_handle;
            };

        }
    }
}







#endif