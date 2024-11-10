#ifndef _pilo_core_sched_cron_job_hpp_
#define _pilo_core_sched_cron_job_hpp_

#include    "./cron_resolving.hpp"
#include    "./task.hpp"
#include    "../container/concurrent_queue.hpp"
#include    "../container/sorted_deque.hpp"
#include    "./timer.hpp"
#include    <memory>


namespace pilo
{
    namespace core
    {
        namespace sched
        {
            class cron_scheduler;
            const static ::pilo::i64_t max_cron_id = 0x0000FFFFFFFFFFFFLL;

            class cron_job
            {
            public:
                cron_job() : _id(-1)
                    , _prev_ts(0)
                    , _cronexpr(nullptr)
                    , _scheduler(nullptr)
                    , _func(nullptr)
                    , _param(nullptr)
                    , _dtor(nullptr)
                    , _stop(0)
                    , _expr("")

                {

                }

                ~cron_job() {
                    reset(); 
                }
                ::pilo::i64_t id() const { return _id;  }
                cronexpr* cron_expression() { return _cronexpr;  }
                bool is_stop() const { return _stop; }
                void stop() { _stop = true; }
                const std::string& expr() { return _expr; };
                ::pilo::i8_t timezone() const { return (::pilo::i8_t)((_id >> 48) & 0xFFFF); }
                ::pilo::err_t schedule(::pilo::i64_t now);
                ::pilo::err_t initialize(cron_scheduler* scheduler, ::pilo::i8_t tz, const std::string& spec, task_func_type f_func, void* param, task_destructor_func_type dtor);
                void reset(); 
                std::string to_string() const;
                const char* to_cstring(char* buffer, ::pilo::i64_t sz) const;
                ::pilo::err_t delete_job();
                
            private:
                ::pilo::i64_t _id;  
                ::pilo::i64_t _prev_ts;
                cronexpr* _cronexpr;
                cron_scheduler* _scheduler;
                task_func_type _func;
                void* _param;
                task_destructor_func_type _dtor;
                volatile bool _stop;
                std::string _expr;    
                
            };



            

        }
    }
}


#endif