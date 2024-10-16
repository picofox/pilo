#ifndef _pilo_core_config_thread_pool_config_h_
#define _pilo_core_config_thread_pool_config_h_

#include "../logging/logger_def.hpp"
#include "../io/path.hpp"
#include "../ml/json_tlv_driver.hpp"
#include "./configuation_interface.hpp"

namespace pilo 
{
    namespace core 
    {
        namespace config 
        {
            class thread_pool_config : public configuation_interface
            {
            public:
                thread_pool_config() : _performance_mode(false)    
                    , _global_task_queue(false)
                    , _workers_count(0)
                    , _task_executor_count(0 )
                    , _pulse_delay_usec(0)
                    , _task_dequeue_block_usec(1000)
                    , _name("")
                {

                }

                ::pilo::i64_t pulse_delay_usec() const {return _pulse_delay_usec;}
                ::pilo::i32_t workers_count() const;
                ::pilo::i32_t task_executor_count() const;
                inline const std::string& name() const { return _name;  }
                inline bool performance_mode() const { return _performance_mode;  }
                inline bool global_task_queue() const { return _global_task_queue; }
                inline ::pilo::i64_t task_dequeue_block_usec() const { return _task_dequeue_block_usec; }

                void set(bool performance_mode, bool global_task_queue
                    , ::pilo::i32_t workers_count, ::pilo::i32_t task_executor_count
                    , ::pilo::i64_t  pulse_delay_usec, ::pilo::i64_t task_dequeue_block_usec
                    , const char *  name)
                {
                    _performance_mode = performance_mode;
                    _global_task_queue = global_task_queue;
                    _workers_count = workers_count;
                    _task_executor_count = task_executor_count;
                    _pulse_delay_usec = pulse_delay_usec;
                    _task_dequeue_block_usec = task_dequeue_block_usec;
                    _name = name;
                }

            public:
                // Inherited via configuation_interface
                ::pilo::err_t load() override;

                ::pilo::err_t save() override;

                ::pilo::err_t set_default() override;

                void clear(bool purge) override;

                ::pilo::err_t save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver) override;

                ::pilo::err_t load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver) override;

                const::pilo::core::io::path* file_path() const override;

                bool invalid() const override;

            private:
                bool            _performance_mode;
                bool            _global_task_queue;                
                ::pilo::i32_t   _workers_count;
                ::pilo::i32_t   _task_executor_count;
                ::pilo::i64_t   _pulse_delay_usec;
                ::pilo::i64_t   _task_dequeue_block_usec;
                std::string     _name;
                

                

            };

        }
    }
}


#endif