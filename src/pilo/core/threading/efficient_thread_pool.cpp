#include "./efficient_thread_pool.hpp"
#include "../process/context.hpp"
#include "../logging/logger_interface.hpp"
#include "./threading.hpp"
#include "./thread_pool_task_executor.hpp"
#include "./polled_thread_pool_pulsive_worker.hpp"
#include "./thread_pool_task_executor.hpp"
#include "../container/concurrent_queue.hpp"
#include "../threading/polled_thread_pool_pulsive_tick_worker.hpp"

namespace pilo
{
    namespace core 
    {
        namespace threading
        {
            ::pilo::err_t efficient_thread_pool::start()
            {
                PLOG(::pilo::core::logging::level::info, SP_PMS_LOGMOD_ETP" Starting...");
                if (_workers.size() > 0) {
                    return ::pilo::mk_perr(PERR_EXIST);
                }

                ::pilo::err_t err = PILO_OK;
                ::pilo::i32_t gw_cnt = _config->workers_count();
                ::pilo::i32_t te_cnt = _config->task_executor_count();
                thread_pool_worker_interface* worker = nullptr;

                for (::pilo::i32_t i = 0; i < te_cnt; i++)
                {
                    worker = new thread_pool_task_executor(this, _on_starting_handler, _on_cleaning_handler, name().c_str());
                    if (worker == nullptr) {
                        return ::pilo::mk_err(PERR_CREATE_OBJ_FAIL);
                    }
                    _workers.push_back(worker);
                    err = worker->start();
                    if (err != PILO_OK)
                        return err;
                }

                _task_executor_count = te_cnt;
                for (::pilo::i32_t i = te_cnt; i < gw_cnt; i++)
                {
                    if (i == te_cnt) {
                        worker = new polled_thread_pool_pulsive_tick_worker(this, _on_starting_handler, _on_running_handler, _on_cleaning_handler, name().c_str());
                    }
                    else {
                        worker = new polled_thread_pool_pulsive_worker(this, _on_starting_handler, _on_running_handler, _on_cleaning_handler, name().c_str());
                    }
                    
                    if (worker == nullptr) {
                        return ::pilo::mk_err(PERR_CREATE_OBJ_FAIL);
                    }
                    _workers.push_back(worker);
                    err = worker->start();
                    if (err != PILO_OK)
                        return err;
                }

                PLOG(::pilo::core::logging::level::info, SP_PMS_LOGMOD_ETP" Waiting started.");

                bool all_running = false;
                while (! all_running) {
                    all_running = true;
                    for (::pilo::i32_t i = 0; i < gw_cnt; i++) {
                        if (!_workers[i]->is_running()) {
                            all_running = false;
                            ::pilo::core::threading::xpf_msleep(100);
                        }
                    }
                }
                
                PLOG(::pilo::core::logging::level::info, SP_PMS_LOGMOD_ETP" Started.");

                return PILO_OK;
            }
            ::pilo::err_t efficient_thread_pool::stop()
            {
                return set_worker_count(0, 0);
            }

            void efficient_thread_pool::post_task(::pilo::core::sched::task* t)
            {      
                if (_task_queue != nullptr) {                    
                    if (!_task_queue->enqueue(t)) {
                        PLOG(::pilo::core::logging::level::error, SP_PMS_LOGMOD_ETP" Post task to g-b-queue failed %x", &t);
                    }
                }
                else {
                    ::pilo::i32_t idx = ::rand() % this->task_executor_count();
                    _workers[idx]->post_task(t);
                }                    
            }
            bool efficient_thread_pool::get_task(::pilo::core::sched::task*& task)
            {
                PMC_ASSERT(_task_queue != nullptr);
                return _task_queue->wait_dequeue_timed(task, this->_config->task_dequeue_block_usec());
            }
            bool efficient_thread_pool::has_task_queue() const
            {
                return (_task_queue != nullptr);
            }
            ::pilo::err_t efficient_thread_pool::set_worker_count(::pilo::i32_t total, ::pilo::i32_t n_executor)
            {
                ::pilo::i32_t old_n_exec = this->_task_executor_count;
                ::pilo::i32_t old_total = (::pilo::i32_t) this->_workers.size();
                ::pilo::i32_t old_pulsive_cnt = old_total - old_n_exec;
                ::pilo::i32_t pulsive_cnt = total - n_executor;

                thread_pool_worker_interface* worker = nullptr;
                if (old_n_exec < n_executor) {
                    for (::pilo::i32_t i = 0; i < (n_executor - old_n_exec); i++) {
                        worker = new thread_pool_task_executor(this, _on_starting_handler, _on_cleaning_handler, name().c_str());
                        if (worker == nullptr) {
                            return ::pilo::mk_err(PERR_CREATE_OBJ_FAIL);
                        }
                        worker->start();
                        this->_workers.insert(this->_workers.begin(), worker);
                    }
                }
                else if (old_n_exec > n_executor) {
                    for (::pilo::i32_t i = old_n_exec - 1; i >= n_executor; i--) {
                        worker = this->_workers[i];
                        this->_workers.erase(this->_workers.begin() + i);
                        worker->stop();
                        delete worker;
                    }
                }

                if (old_pulsive_cnt < pulsive_cnt) {
                    for (::pilo::i32_t i = 0; i < (pulsive_cnt - old_pulsive_cnt); i++) {
                        worker = new polled_thread_pool_pulsive_worker(this, _on_starting_handler, _on_running_handler, _on_cleaning_handler, name().c_str());
                        if (worker == nullptr) {
                            return ::pilo::mk_err(PERR_CREATE_OBJ_FAIL);
                        }
                        worker->start();
                        this->_workers.insert(this->_workers.begin(), worker);
                    }
                }
                else if (old_pulsive_cnt > pulsive_cnt) {
                    ::pilo::i32_t to_del = old_pulsive_cnt - pulsive_cnt;
                    while (to_del > 0) {
                        worker = this->_workers.back();
                        this->_workers.pop_back();
                        worker->stop();
                        delete worker;
                        to_del--;
                    }
                }


                return PILO_OK;
            }

            const::pilo::core::config::thread_pool_config* efficient_thread_pool::config() const
            {
                return this->_config;
            }

            ::pilo::i32_t efficient_thread_pool::task_executor_count() const
            {
                return this->_task_executor_count;
            }

            void efficient_thread_pool::set_running_handler(pool_callback_func_type hdl)
            {
                this->_on_running_handler = hdl;
                for (::pilo::i32_t i = this->_task_executor_count; i < this->_workers.size(); i++) {
                    this->_workers[i]->set_running_handler(hdl);
                }
            }
           
        }
    }
}