#ifndef _pilo_core_threading_thread_pool_task_executor_hpp_
#define _pilo_core_threading_thread_pool_task_executor_hpp_

#include "../../task.hpp"
#include "./thread_pool_interface.hpp"
#include "./auto_join_thread.hpp"
#include "../container/concurrent_queue.hpp"
#include <vector>

namespace pilo 
{
    namespace core 
    {
        namespace threading 
        {           
            
            class thread_pool_task_executor : public thread_pool_worker_interface
            {
            public:
                thread_pool_task_executor(thread_pool_interface* pool, pool_callback_func_type on_start_cb, pool_callback_func_type on_clean_cb, const char* name_stub);
                ~thread_pool_task_executor();
                

                void set_pulse_delay(::pilo::i64_t t)
                {
                    _pulse_delay = t;
                }     

                         

            public:
                // Inherited via thread_pool_worker_interface
                pool_worker_type type() const override;
                bool has_task_queue() const { return true; }
                ::pilo::err_t start() override;
                ::pilo::err_t stop() override;
                const std::string& name() const { return _name; }
                void post_task(::pilo::task* task) override;

            private:
                void _on_running();
                void _on_starting();
                void _on_cleaning();
                void _process_task();

            private:                           
                thread_pool_interface*  _pool;                
                pool_callback_func_type _on_starting_handler;
                pool_callback_func_type _on_cleaning_handler;
                ::pilo::i64_t           _pulse_delay;
                const ::pilo::i64_t     _task_dequeue_block_msec;
                const bool              _task_queue_owner;
                volatile bool           _shutting;
                volatile bool           _stop;
                auto_join_thread*       _worker_thread;
                pool_task_blocking_queue_type*   _task_queue;
                std::string             _name;


                
            };

        }
    }
}


#endif // !_pilo_core_threading_thread_pool_task_executor_hpp_    
