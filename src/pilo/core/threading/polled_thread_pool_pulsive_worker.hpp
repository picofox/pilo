#ifndef _pilo_core_threading_polled_thread_pool_pulsive_worker_hpp_
#define _pilo_core_threading_polled_thread_pool_pulsive_worker_hpp_

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
            
            class polled_thread_pool_pulsive_worker : public thread_pool_worker_interface
            {
            public:
                polled_thread_pool_pulsive_worker(thread_pool_interface* pool
                    , pool_callback_func_type on_start_cb
                    , pool_callback_func_type on_run_cb
                    , pool_callback_func_type on_clean_cb
                    , const char* name_stub) :  _pool(pool)
                                                , _on_starting_handler(on_start_cb)
                                                , _on_running_handler(on_run_cb)
                                                , _on_cleaning_handler(on_clean_cb)
                                                , _pulse_delay(pool->config()->pulse_delay_usec())
                                                , _shutting(false)
                                                , _stop(true)
                                                , _worker_thread(false)
                {                    

                    if (name_stub == nullptr)
                        name_stub = "pulsive_worker";
                    char buffer[64] = { 0 };
                    static int worker_id = 0;
                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%s-%u", name_stub, worker_id++);
                    _name = buffer;
                }

                ~polled_thread_pool_pulsive_worker()
                {
                    stop();
                }

                void set_pulse_delay(::pilo::i64_t t)
                {
                    _pulse_delay = t;
                }
                

            public:
                // Inherited via thread_pool_worker_interface
                pool_worker_type type() const override { return pool_worker_type::pulsive; }
                bool has_task_queue() const { return true; }
                ::pilo::err_t start() override;
                ::pilo::err_t stop() override;
                const std::string& name() const { return _name; }
                void post_task(::pilo::task* task) override;
                void set_running_handler(pool_callback_func_type hdl) override;

            private:
                void _on_running();
                void _on_starting();
                void _on_cleaning();


            private:                           
                thread_pool_interface*  _pool;                
                pool_callback_func_type _on_starting_handler;
                pool_callback_func_type _on_running_handler;
                pool_callback_func_type _on_cleaning_handler;
                ::pilo::i64_t           _pulse_delay;
                volatile bool           _shutting;
                volatile bool           _stop;
                auto_join_thread*       _worker_thread;           
                std::string             _name;                                

            };

        }
    }
}


#endif // !_pilo_core_threading_polled_thread_pool_pulsive_worker_hpp_    
