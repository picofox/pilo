#ifndef _pilo_core_sched_cron_scheduler_hpp_
#define _pilo_core_sched_cron_scheduler_hpp_

#include    "./cron_resolving.hpp"
#include    "../container/concurrent_queue.hpp"
#include    "../container/sorted_deque.hpp"
#include    "./timer.hpp"
#include    "./cron_job.hpp"
#include    <mutex>
#include    <map>


namespace pilo
{
    namespace core
    {
        namespace sched
        {
            class cron_scheduler
            {
            public:
                static ::pilo::i8_t timezone_of_id(::pilo::i64_t id)
                {
                    return (::pilo::i8_t)((id >> 48) & 0xFFFF);
                }

                static ::pilo::i64_t seq_of_id(::pilo::i64_t id)
                {
                    return id & 0xFFFFFFFFFFFFLL;
                }

                static ::pilo::i64_t timestamp()
                {
                    return (::pilo::i64_t) std::time(0);
                }

            public:
                friend class ::pilo::core::sched::cron_job;
                typedef std::mutex  lock_type;

            public:
                ~cron_scheduler();
                ::pilo::i64_t start_neo_job(::pilo::i8_t tz, const std::string& spec, task_func_type f_func, void* param, task_destructor_func_type dtor);
                ::pilo::err_t delete_job(::pilo::i64_t job_id);
                ::pilo::err_t stop_job(::pilo::i64_t job_id);

            private:
                ::pilo::core::sched::cron_job* _add_job(::pilo::i8_t tz, const std::string& spec, task_func_type f_func, void* param, task_destructor_func_type dtor);
                ::pilo::core::sched::cron_job* _find_job(::pilo::i64_t job_id);

            private:
                mutable lock_type                                                                       _mutex;
                std::map<::pilo::i8_t, std::map<::pilo::i64_t, ::pilo::core::sched::cron_job*>>         _jobs;
                
            };



            

        }
    }
}


#endif