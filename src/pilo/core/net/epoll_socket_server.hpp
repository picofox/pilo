#pragma once


#if !defined(WIN32) && !defined(WINDOWS)
#   include "core/coredefs.hpp"
#   include "core/threading/recursive_mutex.hpp"
#   include "core/threading/mutex_locker.hpp"
#   include "core/threading/basic_thread.hpp"
#   include "core/random/simple_random.hpp"
#   include "basic_socket_server.hpp"
#   include "basic_socket.hpp"


namespace pilo
{
    namespace core
    {
        namespace net
        {
            class epoll_socket_server;
            class epoll_socket;

            class epoll_io_thread : public pilo::core::threading::basic_thread
            {
            public:
                epoll_io_thread();
                ~epoll_io_thread();

            public:
                bool initialize(epoll_socket_server* pSockServer, size_t startSockIdx, size_t sockCount);
                void finalize();

            public:
                virtual i32_t on_run();
                virtual void stop_sync(bool auto_reset = true);

           private:
                void process_active_queue(pilo::u64_t intervalTicks);
                void process_closed_queue(pilo::u64_t intervalTicks);

            public:
                volatile bool m_need_stop;
                epoll_socket_server* m_socket_server_ptr;

                size_t  m_start_socket_index;
                size_t  m_socket_count;

                int m_epoll_descriptor;
                pilo::tick64_t m_last_breathe_ts;
            };

            class epoll_socket_server : public basic_socket_server
            {
            public:
                epoll_socket_server();
                virtual ~epoll_socket_server();

            public:
                virtual bool initialize(size_t socketCapacity, size_t ioThreadNum);
                virtual void finalize();

                virtual bool attach(basic_socket* socket_ptr);
                inline size_t socket_capacity() const { return m_socket_capacity; }
                size_t current_socket_count() const;

            public:
                // 是否还能接受更多的Socket
                bool can_accept();

                bool attach_tcp_listen_socket(epoll_socket* psocket);
                bool attach_tcp_initiative_socket(epoll_socket* psocket);
                bool attach_tcp_passive_socket(epoll_socket* psocket);
                bool attach_udp_socket(epoll_socket* psocket);

                void move_socket_to_active(size_t socket_index);
                void move_socket_to_closed(size_t socket_index);

                bool add_to_epoll(epoll_socket* pSock);
                //BOOL AddEpollWriteEvent(KEpollSocket* pSock);
                //BOOL DelEpollWriteEvent(KEpollSocket* pSock);
                bool del_from_epoll(epoll_socket* pSock);

                public:
                int _get_available_socket_index();
                epoll_io_thread& get_responsibility_thread(int sockIndex);

                public:
                size_t  m_io_thread_count;
                size_t  m_socket_capacity;
                size_t  m_socket_count;

                ::pilo::core::random::simple_random m_random;
                epoll_io_thread** m_ppIoThread;

                ::pilo::core::threading::recursive_mutex    m_socket_mutex;
                epoll_socket** m_active_sockets;
                epoll_socket** m_closed_sockets;

                struct epoll_event* m_events;

                ::pilo::i32_t   m_sigpipe_tls;
            };


        }
    }
}

#endif