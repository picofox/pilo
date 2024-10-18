#include "./thread_pool_interface.hpp"
#include "../process/context.hpp"
#include "../logging/logger_interface.hpp"
#include "./threading.hpp"
#include "polled_thread_pool_hybrid_worker.hpp"

namespace pilo
{
    namespace core 
    {
        namespace threading
        {
            ::pilo::err_t polled_thread_pool_hybrid_worker::start()
            {
                if (this->_worker_thread != nullptr) {
                    if (this->is_running())
                        return ::pilo::mk_perr(PERR_EXIST);
                    else { //TODO 
                        delete this->_worker_thread;
                        this->_worker_thread = nullptr;
                    }
                        
                }

                this->_shutting = false;                
                this->_worker_thread = new auto_join_thread(
                    [](polled_thread_pool_hybrid_worker* w) {
                        ::pilo::flag_guard<volatile bool, bool> guard(w->_running, true, false);
                        w->_on_starting();
                        try
                        {
                            w->_on_running();
                        }
                        catch (const std::exception&)
                        {
                            ;
                        }
                        w->_on_cleaning();
                        return;
                    },
                    this
                );
                return PILO_OK;
            }

            ::pilo::err_t polled_thread_pool_hybrid_worker::stop()
            {
                if (this->_worker_thread == nullptr || this->_shutting) {
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



            void polled_thread_pool_hybrid_worker::_on_starting()
            {
                if (this->_on_starting_handler != nullptr) {
                    this->_on_starting_handler(this);
                }
                
            }

            void polled_thread_pool_hybrid_worker::_on_running()
            {   
                while (!_shutting) {
                    try
                    {
                        this->_on_running_handler(this);
                        this->_process_task();
                    }
                    catch (const std::exception& ex)
                    {
                        PLOG(::pilo::core::logging::level::error, "exception in thread worker %s: %s", _name.c_str(), ex.what());
                    }                                       

                    if (_pulse_delay >= 0)
                        xpf_usleep(_pulse_delay);                     
                }    

                this->_process_task();
                this->_shutting = false;
            }

            void polled_thread_pool_hybrid_worker::_on_cleaning()
            {                
                if (this->_on_cleaning_handler != nullptr) {
                    this->_on_cleaning_handler(this);
                }                
            }

            void polled_thread_pool_hybrid_worker::_process_task()
            {
                ::pilo::core::sched::task* task_ptr = nullptr;
                bool has_task = false;

                do {
                    if (this->_task_queue_owner) {
                        has_task = this->_task_queue->try_dequeue(task_ptr);
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

            void polled_thread_pool_hybrid_worker::set_running_handler(pool_callback_func_type hdl)
            {
                this->_on_running_handler = hdl;
            }

            void polled_thread_pool_hybrid_worker::post_task(::pilo::core::sched::task* task)
            {
                PMC_ASSERT(_task_queue != nullptr);
                _task_queue->enqueue(task);
            }

            pool_worker_type polled_thread_pool_hybrid_worker::type() const
            {
                return pool_worker_type::hybrid;
            }

        }
    }
}