#ifndef _pilo_core_threading_thread_pool_interface_hpp_
#define _pilo_core_threading_thread_pool_interface_hpp_

#include "../../task.hpp"
#include "../config/thread_pool_config.hpp"
#include <vector>
#include "../container/concurrent_queue.hpp"
#include "../container/blocking_concurrent_queue.hpp"


namespace pilo 
{
    namespace core 
    {
        namespace threading 
        {
            enum class pool_worker_type
            {
                hybrid = 0,
                pulsive = 1,
                blocking = 2,
            };

            class thread_pool_worker_interface
            {
            public:
                virtual ~thread_pool_worker_interface() {};
                virtual pool_worker_type type() const = 0;
                virtual ::pilo::err_t start() = 0;
                virtual ::pilo::err_t stop() = 0;
                virtual const std::string& name() const = 0;    
                virtual void post_task(::pilo::task* task) = 0;
            };

            typedef ::pilo::core::container::concurrent_queue<::pilo::task*> pool_task_queue_type;
            typedef ::pilo::core::container::blocking_concurrent_queue<::pilo::task*> pool_task_blocking_queue_type;

            typedef void (*pool_callback_func_type)(thread_pool_worker_interface*);

            class thread_pool_interface
            {
            public:
                virtual ~thread_pool_interface() {};
            public:
                virtual ::pilo::err_t start() = 0;
                virtual ::pilo::err_t stop() = 0;
                virtual void post_task(::pilo::task* task) = 0;
                virtual bool get_task(::pilo::task*& task) = 0;
                virtual bool has_task_queue() const = 0;
                virtual ::pilo::err_t set_worker_count(::pilo::i32_t total, ::pilo::i32_t n_executor) = 0;
                virtual const ::pilo::core::config::thread_pool_config* config() const = 0;
                virtual ::pilo::i32_t task_executor_count() const = 0;

            };
        }
    }
}


#endif // !_pilo_core_threading_thread_pool_interface_hpp_    
