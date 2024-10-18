#ifndef _pilo_core_threading_polled_thread_pool_hybrid_worker_hpp_
#define _pilo_core_threading_polled_thread_pool_hybrid_worker_hpp_

#include "../sched/task.hpp"
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
            
            class polled_thread_pool_hybrid_worker : public thread_pool_worker_interface
            {
            public:
                polled_thread_pool_hybrid_worker(thread_pool_interface* pool
                    , pool_callback_func_type on_start_cb
                    , pool_callback_func_type on_run_cb
                    , pool_callback_func_type on_clean_cb
                    , const char* name_stub) :  _pool(pool)
                                                , _on_starting_handler(on_start_cb)
                                                , _on_running_handler(on_run_cb)
                                                , _on_cleaning_handler(on_clean_cb)
                                                , _pulse_delay(pool->config()->pulse_delay_usec())
                                                , _task_queue_owner(pool->has_task_queue()? false : true)
                                                , _shutting(false)
                                                , _running(false)
                                                , _worker_thread(nullptr)
                {                    

                    if (name_stub == nullptr)
                        name_stub = "hybrid_worker";
                    char buffer[64] = { 0 };
                    static int worker_id = 0;
                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%s-%u", name_stub, worker_id++);
                    if (_task_queue_owner) {
                        _task_queue = new pool_task_queue_type;
                    }
                    else {
                        _task_queue = nullptr;
                    }                    
                    _name = buffer;
                }

                ~polled_thread_pool_hybrid_worker()
                {
                    stop();
                    if (_task_queue_owner) {
                        delete _task_queue;
                        _task_queue = nullptr;
                    }
                }

                void set_pulse_delay(::pilo::i64_t t)
                {
                    _pulse_delay = t;
                }

            public:
                // Inherited via thread_pool_worker_interface
                pool_worker_type type() const override;
                ::pilo::err_t start() override;
                ::pilo::err_t stop() override;
                const std::string& name() const override { return _name;  };
                void post_task(::pilo::core::sched::task* task) override;
                void set_running_handler(pool_callback_func_type hdl) override;
                bool is_running() const override  { return _running; }
            private:
                void _on_running();
                void _on_starting();
                void _on_cleaning();
                void _process_task();



            private:                           
                thread_pool_interface*  _pool;                
                pool_callback_func_type _on_starting_handler;
                pool_callback_func_type _on_running_handler;
                pool_callback_func_type _on_cleaning_handler;
                ::pilo::i64_t           _pulse_delay;
                const bool              _task_queue_owner;
                volatile bool           _shutting;
                volatile bool           _running;
                auto_join_thread*       _worker_thread;
                pool_task_queue_type*   _task_queue;               
                std::string             _name;

            };

        }
    }
}


#endif // !_pilo_core_threading_polled_thread_pool_general_worker_hpp_    
