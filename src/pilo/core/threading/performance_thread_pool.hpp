#ifndef _pilo_core_threading_polled_performance_thread_pool_hpp_
#define _pilo_core_threading_polled_performance_thread_pool_hpp_

#include "../../task.hpp"
#include "./thread_pool_interface.hpp"
#include "./auto_join_thread.hpp"
#include "../container/concurrent_queue.hpp"
#include "../config/thread_pool_config.hpp"
#include <vector>

namespace pilo 
{
    namespace core 
    {
        namespace threading 
        {
            class performance_thread_pool : public thread_pool_interface
            {
            public:
                performance_thread_pool(::pilo::core::config::thread_pool_config* cfg
                    , pool_callback_func_type on_start_cb
                    , pool_callback_func_type on_run_cb
                    , pool_callback_func_type on_clean_cb
                    ) : _config(cfg)
                                             , _task_executor_count(0)
                                             , _on_starting_handler(on_start_cb)
                                             , _on_running_handler(on_run_cb)
                                             , _on_cleaning_handler(on_clean_cb)
                                             , _task_queue(nullptr)

                {
                    if (cfg->global_task_queue()) {
                        _task_queue = new pool_task_queue_type;
                    }
                    else {
                        _task_queue = nullptr;
                    }
                }

                ~performance_thread_pool()
                {
                    this->stop();
                    if (this->_task_queue != nullptr)
                        delete this->_task_queue;
                }

            public:
                ::pilo::err_t initialize();     
                bool has_task_queue() const override;
                bool get_task(::pilo::task*& task) override;
                ::pilo::err_t set_worker_count(::pilo::i32_t total, ::pilo::i32_t n_executor) override;
                ::pilo::err_t start() override;
                ::pilo::err_t stop() override;
                const::pilo::core::config::thread_pool_config* config() const override;
                ::pilo::i32_t task_executor_count() const override;
                void post_task(::pilo::task* task) override;

            private:
                ::pilo::core::config::thread_pool_config*       _config;
                ::pilo::i32_t                                   _task_executor_count;
                pool_callback_func_type                         _on_starting_handler;
                pool_callback_func_type                         _on_running_handler;
                pool_callback_func_type                         _on_cleaning_handler; 
                pool_task_queue_type*                           _task_queue;
                std::vector<thread_pool_worker_interface*>      _workers;



                



                

            };
        }
    }
}


#endif // !_pilo_core_threading_polled_thread_pool_interface_hpp_    
