#ifndef _pilo_core_sched_cron_hpp_
#define _pilo_core_sched_cron_hpp_

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
            class cron;
            class cron_manager;
            const static ::pilo::i64_t max_cron_id = 0x0000FFFFFFFFFFFFLL;

            class cron_entry
            {
            public:
                cron_entry() : _id(-1), _cronexpr(nullptr), _task(nullptr), _next(0),  _cron(nullptr),_shutdown(false), _expr("")
                {

                }

                ~cron_entry() { 
                    reset(); 
                }
                ::pilo::i64_t id() const { return _id;  }
                cronexpr* cron_expression() { return _cronexpr;  }
                bool is_shutdown() const { return _shutdown; }
                void shutdown() { _shutdown = true;  }
                ::pilo::err_t schedule(::pilo::i64_t now);
                cron* crontab() { return _cron; }
                ::pilo::i64_t next() const { return _next;  } 
                void set_next(::pilo::i64_t n) { _next = n;  }
                ::pilo::core::sched::task* task() const { return _task;  }
                ::pilo::err_t initialize(const std::string& spec, task_func_type f_func, void* obj, void* param, task_destructor_func_type dtor, cron* c);
                void reset();        
                const std::string&  expr() { return _expr; };

                std::string to_string() const;
                const char* to_cstring(char* buffer, ::pilo::i64_t sz) const;
                
            private:
                ::pilo::i64_t _id;                
                cronexpr* _cronexpr;
                ::pilo::core::sched::task* _task;
                ::pilo::i64_t _next;
                cron* _cron;         
                volatile bool _shutdown;
                std::string _expr;
                
            };



            class cron_entry_less_than_comparator
            {
            public:
                bool operator () (const cron_entry* a, const cron_entry* b) const
                {
                    if (a->next() == PMI_INVALID_TIMESTAMP) 
                        return false;
                    if (b->next() == PMI_INVALID_TIMESTAMP) 
                        return true;
                    return (a->next()) < (b->next());
                }
            }; 
            template <typename L = cron_entry_less_than_comparator>
            class cron_entry_comparator
            {
            public:
                L m_less;
                int operator () (const cron_entry* a, const cron_entry* b) const
                {
                    if (m_less(a, b)) return -1;
                    else if (m_less(b, a)) return 1;
                    else return 0;
                }
            };

            class sp_cron_entry_less_than_comparator
            {
            public:
                bool operator () (const std::shared_ptr<cron_entry> a, const std::shared_ptr<cron_entry> b) const
                {
                    return (a->next()) < (b->next());
                }
            };
            template <typename L = sp_cron_entry_less_than_comparator>
            class sp_cron_entry_comparator
            {
            public:
                L m_less;
                int operator () (const std::shared_ptr<cron_entry> a, const std::shared_ptr<cron_entry> b) const
                {
                    if (m_less(a, b)) return -1;
                    else if (m_less(b, a)) return 1;
                    else return 0;
                }
            };


            class cron
            {
            public:
                static ::pilo::i64_t timestamp();    
                static ::pilo::err_t check_and_delete_job(void* ctx);
                static ::pilo::i8_t timezone_of_id(::pilo::i64_t id);
                static ::pilo::i64_t seq_of_id(::pilo::i64_t id);

            public:
                cron(::pilo::i8_t tz, cron_manager * mgr) : _tz(tz), _manager(mgr)
                {                   
                }
                ~cron();
                ::pilo::i64_t add_job(const std::string& spec, task_func_type f_func, void* obj, void* param, task_destructor_func_type dtor);
                ::pilo::err_t delete_job_async(::pilo::i64_t id);
                ::pilo::err_t delete_job(::pilo::i64_t id);
                void pulse();
                ::pilo::i8_t timezone() const { return _tz; }
                ::pilo::core::container::sorted_deque<::pilo::core::sched::cron_entry*, ::pilo::core::sched::cron_entry_comparator<::pilo::core::sched::cron_entry_less_than_comparator>>& entries() { return _entries; }
                void finalize();
                cron_manager* manager() { return _manager; }
            private:
                ::pilo::core::container::sorted_deque<::pilo::core::sched::cron_entry*, ::pilo::core::sched::cron_entry_comparator<::pilo::core::sched::cron_entry_less_than_comparator>>  _entries;
                ::pilo::i8_t                                            _tz;   
                cron_manager* _manager;

            };

            

        }
    }
}


#endif