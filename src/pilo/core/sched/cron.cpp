#include "./cron.hpp"

#include    "../process/context.hpp"
#include    "./cron_resolving.hpp"
#include    "../datetime/zoned_clock.hpp"
#include    "../algorithm/uint_sequence_generator.hpp"
#include    "../logging/logger_interface.hpp"
#include    <thread>

/*
5 * * * * ?  every 5 secs
0 * / 1 * **? 每隔1分钟执行一次
0 0 5 - 15 * *? 每天5 - 15点整点触发
0 0 / 3 * **? 每三分钟触发一次
0 0 - 5 14 * *? 在每天下午2点到下午2 : 05期间的每1分钟触发
0 0 / 5 14 * *? 在每天下午2点到下午2 : 55期间的每5分钟触发
0 0 / 5 14, 18 * *? 在每天下午2点到2 : 55期间和下午6点到6 : 55期间的每5分钟触发
0 0 / 30 9 - 17 * *? 朝九晚五工作时间内每半小时
0 0 10, 14, 16 * *? 每天上午10点，下午2点，4点

0 0 12 ? *WED 表示每个星期三中午12点
0 0 17 ? *TUES, THUR, SAT 每周二、四、六下午五点
0 10, 44 14 ? 3 WED 每年三月的星期三的下午2 : 10和2 : 44触发
0 15 10 ? *MON - FRI 周一至周五的上午10 : 15触发
0 0 23 L * ? 每月最后一天23点执行一次
0 15 10 L * ? 每月最后一日的上午10 : 15触发
0 15 10 ? *6L 每月的最后一个星期五上午10 : 15触发
0 15 10 * *? 2005 2005年的每天上午10 : 15触发
0 15 10 ? *6L 2002 - 2005 2002年至2005年的每月的最后一个星期五上午10 : 15触发
0 15 10 ? *6#3 每月的第三个星期五上午10:15触发


"30 * * * * ?" 每半分钟触发任务
"30 10 * * * ?" 每小时的10分30秒触发任务
"30 10 1 * * ?" 每天1点10分30秒触发任务
"30 10 1 20 * ?" 每月20号1点10分30秒触发任务
"30 10 1 20 10 ? *" 每年10月20号1点10分30秒触发任务
"30 10 1 20 10 ? 2011" 2011年10月20号1点10分30秒触发任务
"30 10 1 ? 10 * 2011" 2011年10月每天1点10分30秒触发任务
"30 10 1 ? 10 SUN 2011" 2011年10月每周日1点10分30秒触发任务
"15,30,45 * * * * ?" 每15秒，30秒，45秒时触发任务
"15-45 * * * * ?" 15到45秒内，每秒都触发任务
"15/5 * * * * ?" 每分钟的每15秒开始触发，每隔5秒触发一次
"15-30/5 * * * * ?" 每分钟的15秒到30秒之间开始触发，每隔5秒触发一次
"0 0/3 * * * ?" 每小时的第0分0秒开始，每三分钟触发一次
"0 15 10 ? * MON-FRI" 星期一到星期五的10点15分0秒触发任务
"0 15 10 L * ?" 每个月最后一天的10点15分0秒触发任务
"0 15 10 LW * ?" 每个月最后一个工作日的10点15分0秒触发任务
"0 15 10 ? * 5L" 每个月最后一个星期四的10点15分0秒触发任务
"0 15 10 ? * 5#3" 每个月第三周的星期四的10点15分0秒触发任务

*/

namespace pilo
{
    namespace core
    {
        namespace sched
        {

            static ::pilo::core::algorithm::uint_sequence_generator<::pilo::i64_t>	s_cron_job_id_generator(0, max_cron_id);

            ::pilo::err_t cron_entry::schedule(::pilo::i64_t now)
            {
                //::pilo::core::datetime::zoned_clock zc(now*1000000, 8);
                //printf("now -> %s ", zc.to_string().c_str());

                _next = ::pilo::core::sched::cron_next(*_cronexpr, now, _cron->timezone());
                if (_next == PMI_INVALID_TIMESTAMP)
                    return ::pilo::mk_perr(PERR_INC_DATA);
                _cron->entries().insert(this);                

                //zc.set(_next * 1000000);
                //printf("next -> %s ", zc.to_string().c_str());
                return PILO_OK;
            }
            ::pilo::err_t cron_entry::initialize(const std::string& spec, task_func_type f_func, void* obj, void* param, task_destructor_func_type dtor, cron * c)
            {
                if (_cronexpr != nullptr || f_func == nullptr) {
                    return ::pilo::mk_perr(PERR_EXIST);
                }
                _cronexpr = ::pilo::core::sched::make_cron(spec);
                if (_cronexpr == nullptr) {
                    return ::pilo::mk_perr(PERR_CREATE_OBJ_FAIL);
                }
                _id = s_cron_job_id_generator.next() | (((::pilo::i64_t)c->timezone()) << 48);
                _task = PILO_CONTEXT->allocate_task();
                if (_task == nullptr)
                    return ::pilo::mk_perr(PERR_CREATE_OBJ_FAIL);
                _task->set(f_func, obj, param, this, dtor);
                _cron = c;
                _expr = spec;
                return PILO_OK;
            }

            void cron_entry::reset()
            {
                if (_cronexpr != nullptr) {
                    delete _cronexpr;
                    _cronexpr = nullptr;
                }
                if (_task != nullptr) {
                    PILO_CONTEXT->deallocate_task(_task);
                    _task = nullptr;
                }
                _cron = nullptr;
            }

            std::string cron_entry::to_string() const
            {
                char buffer[128] = { 0 };
                to_cstring(buffer, sizeof(buffer));
                return std::string(buffer);
            }

            const char* cron_entry::to_cstring(char* buffer, ::pilo::i64_t sz) const
            {
                ::pilo::i8_t tzkey = (::pilo::i8_t)((_id >> 48) & 0xFFFF);
                ::pilo::i64_t seq = _id & 0xFFFFFFFFFFFFLL;
                ::pilo::core::io::string_formated_output(buffer, sz, "%d-%lld:%s @ <0x%016p>", tzkey, seq, _expr.c_str(), _task);
                return buffer;
            }
            

            void cron::pulse()
            {       
                ::pilo::i64_t now = cron::timestamp();
                cron_entry* ent = nullptr;
                for (;;) {
                    if (_entries.empty() || _entries.at(0)->next() == PMI_INVALID_TIMESTAMP ) {
                        return;
                    }
                    ent = this->_entries.front();
                    if (ent->next() != PMI_INVALID_TIMESTAMP && ent->next() <= now) {
                        this->_entries.pop_front();
                        PLOG(::pilo::core::logging::level::info, "[cron] Post job task @<%016p>", ent->task());
                        PILO_CONTEXT->post_task(ent->task());                        
                        ent->schedule(now);
                    }
                    else {
                        return;
                    }
                    
                }
            }

            void cron::finalize()
            {
                for (auto i = 0; i < _entries.size(); i++) {
                    delete _entries[i];
                }
                _entries.clear();
            }
                
            ::pilo::i64_t cron::timestamp()
            {
                return std::time(0);
            }

            ::pilo::err_t cron::check_and_delete_job(void* ctx)
            {
                cron_entry* ent = (cron_entry*)ctx;
                if (ent == nullptr)
                    return ::pilo::mk_perr(PERR_NULL_PARAM);

                char buffer[64] = { 0 };
                  ent->to_cstring(buffer, sizeof(buffer));
                if (ent->is_shutdown()) {
                    ::pilo::err_t err = ent->crontab()->delete_job(ent->id());
                    if (err == PILO_OK)
                        PLOG(::pilo::core::logging::level::info, "[cron] job (%s) is deleted. no further use it.", buffer);
                    else
                        PLOG(::pilo::core::logging::level::info, "[cron] job (%s) is being deleted. but not found.", buffer);
                    return ::pilo::mk_perr(PERR_NON_EXIST);
                }
                    

                return PILO_OK;
            }

            ::pilo::i8_t cron::timezone_of_id(::pilo::i64_t id)
            {
                return (::pilo::i8_t)((id >> 48) & 0xFFFF);
            }

            ::pilo::i64_t cron::seq_of_id(::pilo::i64_t id)
            {
                return id & 0xFFFFFFFFFFFFLL;
            }

            cron::~cron()
            {
                for (auto i = 0; i < _entries.size(); i++) {
                    if (_entries[i] != nullptr)
                        delete _entries[i];
                    _entries[i] = nullptr;
                }
                _entries.clear();
            }

            ::pilo::i64_t cron::add_job(const std::string& spec, task_func_type f_func, void* obj, void* param, task_destructor_func_type dtor)
            {
                ::pilo::core::sched::cron_entry* entry = new ::pilo::core::sched::cron_entry;
                ::pilo::err_t err = entry->initialize(spec, f_func, obj, param, dtor, this);
                if (err != PILO_OK)
                    return -1;                
                err = entry->schedule(cron::timestamp());
                if (err != PILO_OK)
                    return -1;                
                return entry->id();
            }

            ::pilo::err_t cron::delete_job_async(::pilo::i64_t id)
            {            
                for (auto i = 0; i < _entries.size(); i++) {
                    if (_entries[i]->id() == id) {
                        _entries[i]->shutdown();
                        return PILO_OK;
                    }
                }
                return ::pilo::mk_perr(PERR_NON_EXIST);
            }

            ::pilo::err_t cron::delete_job(::pilo::i64_t id)
            {
                std::lock_guard<cron_manager::lock_type>	guard(this->manager()->_mutex);
                for (auto i = 0; i < _entries.size(); i++) {
                    if (_entries[i]->id() == id) {
                        cron_entry* ent = _entries[i];
                        _entries.earse(i);
                        delete ent;
                        return PILO_OK;
                    }
                }
                return ::pilo::mk_perr(PERR_NON_EXIST);
            }

        }
    }
}