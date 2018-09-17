#pragma once

#include "core/coredefs.hpp"
#include "net_sys_header.hpp"
#include "core/string/fixed_astring.hpp"
#include "ipv4_address.hpp"
#include "core/threading/recursive_mutex.hpp"
#include "core/threading/mutex_locker.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {
            class basic_socket
            {
            public:
                enum EnumSocketType
                {
                    eST_ListenTCP = 0,
                    eST_InitiativeTCP = 1, //mostly server socket by accept
                    eST_PassiveTCP = 2, //mostly client socket by connect                
                    eST_UDP = 3,

                    eST_MAX
                };

            public:
                basic_socket();
                virtual ~basic_socket();

            public:
                static os_socket_fd_t  create_tcp_socket();
                static os_socket_fd_t  create_udp_socket();
                static bool         set_none_blocking_mode(os_socket_fd_t fd);
                static bool         ignore_sigpipe(os_socket_fd_t fd);
                static pilo::u32_t  get_ipv4_by_name(const char* name);

            public:                
//                 size_t get_active_timeout() const;
//                 void set_active_timeout(size_t secs); 

                void set_sockopt_send_buffer_size(socklen_t bytes);
                inline void set_user_data(pilo::u32_t data) { m_userdata = data; }
                inline pilo::u32_t user_data() const { return m_userdata; }
                void inc_ref_count();
                void dec_ref_count();
                size_t ref_count();
                ipv4_address local_address() const;
                pilo::tick64_t get_idle_ticks() const;       // Socket Close后，经过的时间
                pilo::tick64_t get_latest_send_tick() const;  // 最近一次发送成功的时间，GetTickCount
                pilo::tick64_t get_latest_recv_tick() const;  // 最近一次成功接收的时间，GetTickCount
                size_t get_send_speed() const;       // 工作期间的发送速度 Bytes/Sec
                size_t get_recv_speed() const;       // 工作期间的接收速度 Bytes/Sec

                
                bool close(); //关闭并且调用on_close
                bool close_delayed(size_t secs); // 延迟关闭，在维护线程中检查时间，到了就执行正真的关闭动作
                void pause_send(pilo::tick64_t ticks); // 线程不安全的，只能在发送线程中调用
                

                virtual EnumSocketType socket_type() const = 0;
                virtual size_t send(const void* packet, size_t len) = 0; // Tcp使用的发送函数
                virtual size_t send_to(const void* packet, size_t len, const pilo::core::net::ipv4_address& address) = 0; // UDP使用的发送函数
                virtual size_t send_ndelay(const void* packet, size_t len) = 0;
                virtual size_t send_to_ndelay(const void* packet, size_t len, const pilo::core::net::ipv4_address& address) = 0;

                //不能调用的
                virtual bool post_packet(int cmd, const void* pData, int len) { M_UNUSED(cmd); M_UNUSED(pData); M_UNUSED(len); M_ASSERT(false); return false; }

                virtual void reset(); // 重置Socket对象的状态
                virtual void destroy(); // SockServer销毁Socket对象是使用的函数  缺省实现时Delete，如果使用了对象池技术的话，则需要重写这个函数
                virtual bool on_active_state(); // SOCKET在Active状态时，每隔一定时间调用一次  Socket对象可以再这里检查对象的活跃时间，收发是否正常，或者其他的检查
                virtual void on_breathe(pilo::tick64_t ticks_interval);         
                virtual bool on_closed_state(); // SOCKET在关闭状态下，每隔一定时间调用一次  主动连接的SOCKET可以在这个函数里面执行重连的动作 SockServer会根据这个函数的返回值，决定是否销毁这个对象(TRUE保留，FALSE销毁)
                virtual bool on_attached(); // Socket对象刚关联到SockServer上时的通知                
                virtual bool on_detached(); // Socket对象从SockServer卸下的通知                
                virtual bool on_invalid_packet(const pilo::core::net::ipv4_address* pFrom, EnumErrorPacketReason reason); // 收到非法包的通知
                virtual bool on_sending_data(const void* pData, size_t len, const pilo::core::net::ipv4_address* pTarget);                
                virtual size_t on_receiving_data(const void* pData, size_t len, const pilo::core::net::ipv4_address* pFrom); // 返回值表示在函数中消耗了多少个字节，小于 0 表示错误
                virtual bool on_socket_error(pilo::error_number_t errCode); // Socket的IO操作发生了错误
                virtual void on_would_block();                
                virtual bool on_accepted(os_socket_fd_t sock); // Tcp Listen Scket 接受了一个连接请求
                virtual bool on_connected(); // TCP/ClientTcp socket 连接建立时的通知 对于 UDP Socket，这是可以开始 IO 的时机通知                 
                virtual bool on_closed(); // SOCKET被关闭时的通知
                virtual pilo::core::string::fixed_astring<256> to_string() const = 0; // Socket对象的字符串表示                
                virtual void on_start_working(); // 开始IO工作前的通知，在这里把一些Buffer和滋生的一些状态重置一下                
                   
            public: //attribute
                inline os_socket_fd_t socket_file_descriptor() const { return m_sock; }
                inline pilo::tick64_t delay_close_ticks() const { return m_delay_close_ticks; }
                inline size_t socket_index() const { return m_socket_index; }

            protected:
                void _close_nolock(); // 真正执行CloseSocket，所有sockserver中的SOCKET的close操作都会走这个函数  
                void _destroy_by_delete(); // 缺省的销毁对象的方式，直接Delete

            public:
                mutable pilo::core::threading::recursive_mutex  m_lock; // 保护Socket对象内部成员的互斥量
    
                pilo::u32_t                                     m_userdata;                
                
                os_socket_fd_t                                     m_sock;
                os_socket_fd_t                                     m_sock_dup;			// 关闭SOCKET时，会把SOCKET的值拷贝到这里，从EPOLL中删除需要使用
                volatile bool                                   m_ready_for_io; // 是否准备好了，可以进行IO操作了 Send操作是从外部发起的，通过这个值可以把一些过早的Send挡住
                size_t                                          m_socket_index;          // Socket在SockServer队列中的索引号
                volatile size_t                                 m_ref_count;  // 这个Socket的引用计数，只有在0的时候才能被销毁

                pilo::u64_t                                     m_bytes_sent;        // 开始工作以来发送的字节数
                pilo::u64_t                                     m_bytes_received;        // 开始工作以来接收的字节数
                pilo::tick64_t                                  m_start_working_tick;        // SOCKET开始工作的时刻，GetTickCount
                pilo::tick64_t                                  m_latest_send_tick;  // 最近一次发送成功的时刻，GetTickCount
                pilo::tick64_t                                  m_latest_recv_tick;  // 最近一次接收成功的时刻，GetTickCount
                pilo::tick64_t                                  m_closed_tick;        // the time ticks of close socket. this will be use in OnClosedState to check whether Destroy
                pilo::tick64_t                                  m_delay_close_ticks;		// 延迟关闭的时间，0表示没有设定延迟关闭
                volatile bool                                   m_debug_state;
                volatile bool                                   m_send_paused;
                volatile pilo::tick64_t                         m_send_pause_ticks;
                volatile bool                                   m_notified_conncted; // OnConnected是否已通知


            };
        }
    }    
}