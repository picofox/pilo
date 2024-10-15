#include "./thread_pool_interface.hpp"
#include "../process/context.hpp"
#include "../logging/logger_interface.hpp"
#include "./threading.hpp"
#include "polled_thread_pool_pulsive_worker.hpp"

namespace pilo
{
    namespace core 
    {
        namespace threading
        {
            ::pilo::err_t polled_thread_pool_pulsive_worker::start()
            {
                if (this->_worker_thread != nullptr) {
                    return ::pilo::mk_perr(PERR_EXIST);
                }
                
                this->_shutting = false;                
                this->_worker_thread = new auto_join_thread(
                    [](polled_thread_pool_pulsive_worker* w) {
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

            ::pilo::err_t polled_thread_pool_pulsive_worker::stop()
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



            void polled_thread_pool_pulsive_worker::_on_starting()
            {
                if (this->_on_starting_handler != nullptr) {
                    this->_on_starting_handler(this);
                }                
            }

            void polled_thread_pool_pulsive_worker::_on_running()
            {                
                while (!_shutting) {
                    try
                    {
                        this->_on_running_handler(this);
                    }
                    catch (const std::exception& ex)
                    {
                        PLOG(::pilo::core::logging::level::error, "exception in thread worker %s: %s", _name.c_str(), ex.what());
                    }                                       

                    if (_pulse_delay >= 0)
                        xpf_usleep(_pulse_delay);                     
                }    


                this->_stop = true;
                this->_shutting = false;

            }

            void polled_thread_pool_pulsive_worker::_on_cleaning()
            {                
                if (this->_on_cleaning_handler != nullptr) {
                    this->_on_cleaning_handler(this);
                }                
            }

            void polled_thread_pool_pulsive_worker::set_running_handler(pool_callback_func_type hdl)
            {
                this->_on_running_handler = hdl;
            }

            void polled_thread_pool_pulsive_worker::post_task(::pilo::core::sched::task* )
            {
                PMC_ASSERT(false);
            }

        }
    }
}