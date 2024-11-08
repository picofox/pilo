#ifndef _pilo_core_sched_cron_manager_hpp_
#define _pilo_core_sched_cron_manager_hpp_

#include    "./cron.hpp"
#include    "../threading/native_mutex.hpp"
#include    <map>
#include    <mutex>


namespace pilo
{
    namespace core
    {
        namespace sched
        {
            class cron_manager
            {
            public:
                friend class ::pilo::core::sched::cron;
                typedef std::mutex  lock_type;

            public:
                cron_manager() : _timer_id(-1)
                {

                }
                ~cron_manager()
                {
                    finanlize();
                }
                static void s_cron_timer_func(::pilo::core::sched::task* t);

            public:
                void start();
                void finanlize();
                void pulse();
                ::pilo::i64_t add_job(::pilo::i8_t tz, const std::string& spec, task_func_type f_func, void* obj, void* param, task_destructor_func_type dtor);
                ::pilo::err_t delete_job(::pilo::i64_t);
            private:
                ::pilo::i64_t                                   _timer_id;
                mutable lock_type                               _mutex;
                std::map<::pilo::i8_t, cron*>                   _crons;
            };
        }
    }
}


#endif