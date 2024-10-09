#include "./thread_pool_interface.hpp"
#include "../process/context.hpp"
#include "../logging/logger_interface.hpp"
#include "./threading.hpp"
#include "thread_pool_task_executor.hpp"

namespace pilo
{
    namespace core 
    {
        namespace threading
        {
            ::pilo::err_t thread_pool_task_executor::start()
            {
                if (this->_worker_thread != nullptr) {
                    return ::pilo::mk_perr(PERR_EXIST);
                }

                this->_shutting = false;                
                this->_worker_thread = new auto_join_thread(
                    [](thread_pool_task_executor* w) {
                        w->_on_starting();
                        try
                        {
                            w->_on_running();
                        }
                        catch (const std::exception&)
                        {
                            w->_on_cleaning();
                            return;
                        }
                        w->_on_cleaning();
                        return;
                    },
                    this
                );
                this->_stop = false;
                return PILO_OK;
            }

            ::pilo::err_t thread_pool_task_executor::stop()
            {
                if (this->_worker_thread == nullptr || this->_shutting || this->_stop) {
                    return ::pilo::mk_perr(PERR_NOOP);
                }
                this->_shutting = true;
                if (this->_worker_thread->id() != std::this_thread::get_id())
                {
                    try
                    {
                        this->_worker_thread->join();                        
                    }
                    catch (const std::exception&)
                    {
                    }

                    this->_stop = true;
                    delete this->_worker_thread;
                    this->_worker_thread = nullptr;
                    this->_shutting = false;
                }
                else
                {
                    return ::pilo::mk_perr(PERR_OK_WITH_INFO);
                }
                

                return PILO_OK;
            }



            void thread_pool_task_executor::_on_starting()
            {
                if (this->_on_starting_handler != nullptr) {
                    this->_on_starting_handler(this);
                }
                
            }

            void thread_pool_task_executor::_on_running()
            {                
                while (!_shutting) {
                    try
                    {
                        _process_task();
                    }
                    catch (const std::exception& ex)
                    {
                        PLOG(::pilo::core::logging::level::error, "exception in thread worker %s: %s", _name.c_str(), ex.what());
                    }  
                }    

                _process_task();

                this->_stop = true;
                this->_shutting = false;

            }

            void thread_pool_task_executor::_on_cleaning()
            {                
                if (this->_on_cleaning_handler != nullptr) {
                    this->_on_cleaning_handler(this);
                }                
            }

            void thread_pool_task_executor::_process_task()
            {
                ::pilo::task* task_ptr = nullptr;
                bool has_task = false;
                do {
                    if (this->_task_queue_owner) {
                        has_task = this->_task_queue->wait_dequeue_timed(task_ptr, _task_dequeue_block_msec);
                    }
                    else {
                        has_task = this->_pool->get_task(task_ptr);
                    }

                    if (!has_task) {
                        return;
                    }
                    else {
                        (*task_ptr)();                      
                    }
                } while (has_task);
                return;
            }

            void thread_pool_task_executor::set_running_handler(pool_callback_func_type)
            {
                return;
            }

            void thread_pool_task_executor::post_task(::pilo::task* task)
            {
                PMC_ASSERT(_task_queue != nullptr);
                _task_queue->enqueue(task);
            }


            thread_pool_task_executor::thread_pool_task_executor(thread_pool_interface* pool, pool_callback_func_type on_start_cb, pool_callback_func_type on_clean_cb, const char* name_stub)
                : _pool(pool)
                , _on_starting_handler(on_start_cb)
                , _on_cleaning_handler(on_clean_cb)
                , _pulse_delay(pool->config()->pulse_delay_usec())
                , _task_dequeue_block_msec(pool->config()->task_dequeue_block_msec())
                , _task_queue_owner(pool->has_task_queue() ? false : true)
                , _shutting(false)
                , _stop(true)
                , _worker_thread(false)
            {
                if (name_stub == nullptr)
                    name_stub = "task_executor";
                char buffer[64] = { 0 };
                static int worker_id = 0;
                ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%s-%u", name_stub, worker_id++);
                if (_task_queue_owner) {
                    _task_queue = new pool_task_blocking_queue_type;
                }
                else {
                    _task_queue = nullptr;
                }
                _name = buffer;
            }

            thread_pool_task_executor::~thread_pool_task_executor()
            {
                stop();
                if (_task_queue_owner) {
                    delete _task_queue;
                    _task_queue = nullptr;
                }
            }
          

            pool_worker_type thread_pool_task_executor::type() const
            {
                return pool_worker_type::blocking;
            }

        }
    }
}