#include "./cron_scheduler.hpp"
#include    "../process/context.hpp"
#include    "./cron_resolving.hpp"
#include    "../datetime/zoned_clock.hpp"
#include    "../algorithm/uint_sequence_generator.hpp"
#include    "../logging/logger_interface.hpp"
#include    <thread>
#include "cron_job.hpp"


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

            

            cron_scheduler::~cron_scheduler()
            {
                std::map<::pilo::i8_t, std::map<::pilo::i64_t, ::pilo::core::sched::cron_job*>>::iterator outter_it = _jobs.begin();
                for (; outter_it != _jobs.end(); outter_it++) {                    
                    std::map<::pilo::i64_t, ::pilo::core::sched::cron_job*>::iterator inner_it = outter_it->second.begin();
                    for (; inner_it != outter_it->second.end(); inner_it++) {
                        delete inner_it->second;
                    }
                    outter_it->second.clear();
                }
                _jobs.clear();
            }

            ::pilo::i64_t cron_scheduler::start_neo_job(::pilo::i8_t tz, const std::string& spec, task_func_type f_func, void* param, task_destructor_func_type dtor)
            {
                std::lock_guard<lock_type>	guard(this->_mutex);
                cron_job* job = this->_add_job(tz, spec, f_func, param, dtor);
                if (job == nullptr)
                    return -1;
                ::pilo::err_t err = job->schedule(cron_scheduler::timestamp());
                if (err != PILO_OK) {
                    PLOG(::pilo::core::logging::level::error, "[cron] cron_job (%s) can not schedule.", job->to_string().c_str());
                    return -1;
                }
                return job->id();
            }

            ::pilo::core::sched::cron_job* cron_scheduler::_find_job(::pilo::i64_t job_id)
            {
                ::pilo::i8_t tz = (::pilo::i8_t)((job_id >> 48) & 0xFFFF);
                std::map<::pilo::i8_t, std::map<::pilo::i64_t, ::pilo::core::sched::cron_job*>>::iterator outter_it = _jobs.find(tz);
                if (outter_it == _jobs.cend()) {
                    return nullptr;
                }
                if (outter_it->second.count(job_id) < 1) {
                    return nullptr;
                }

                return outter_it->second.find(job_id)->second;
            }

            ::pilo::err_t cron_scheduler::delete_job(::pilo::i64_t job_id)
            {
                std::lock_guard<lock_type>	guard(this->_mutex);
                ::pilo::i8_t tz = (::pilo::i8_t)((job_id >> 48) & 0xFFFF);
                std::map<::pilo::i8_t, std::map<::pilo::i64_t, ::pilo::core::sched::cron_job*>>::iterator outter_it = _jobs.find(tz);
                if (outter_it == _jobs.cend()) {
                    return ::pilo::mk_perr(PERR_NON_EXIST);
                }
                if (outter_it->second.count(job_id) < 1) {
                    return ::pilo::mk_perr(PERR_NON_EXIST);
                }
                outter_it->second.erase(job_id);
                return PILO_OK;
            }

            ::pilo::err_t cron_scheduler::stop_job(::pilo::i64_t job_id)
            {
                std::lock_guard<lock_type>	guard(this->_mutex);
                ::pilo::core::sched::cron_job* job = _find_job(job_id);
                if (job == nullptr) {
                    return ::pilo::mk_perr(PERR_NON_EXIST);
                }
                job->stop();
                return PILO_OK;
            }


            ::pilo::core::sched::cron_job* cron_scheduler::_add_job(::pilo::i8_t tz, const std::string& spec, task_func_type f_func, void* param, task_destructor_func_type dtor)
            {
                std::map<::pilo::i8_t, std::map<::pilo::i64_t, ::pilo::core::sched::cron_job*>>::iterator outter_it = _jobs.find(tz);
                if (outter_it == _jobs.end()) {
                    _jobs.insert(std::pair<::pilo::i8_t, std::map<::pilo::i64_t, ::pilo::core::sched::cron_job*>>(tz, std::map<::pilo::i64_t, ::pilo::core::sched::cron_job*>()));
                    outter_it = _jobs.find(tz);
                    if (outter_it == _jobs.end()) {
                        PLOG(::pilo::core::logging::level::error, "[cron] create and find sub job map failed.");
                        return nullptr;
                    }
                }

                ::pilo::core::sched::cron_job* job = new ::pilo::core::sched::cron_job;                
                ::pilo::err_t err = job->initialize(this, tz, spec, f_func, param, dtor);
                if (err != PILO_OK) {
                    PLOG(::pilo::core::logging::level::error, "[cron] init cron_job failed %lld", err);        
                    delete job;
                    return nullptr;
                }

                std::map<::pilo::i64_t, ::pilo::core::sched::cron_job*>::iterator inner_it = outter_it->second.find(job->id());
                if (inner_it != outter_it->second.end()) {
                    PLOG(::pilo::core::logging::level::error, "[cron] cron_job (%s) exist.", job->to_string().c_str());
                    delete job;
                    return nullptr;
                }

                outter_it->second.insert(std::pair<::pilo::i64_t, ::pilo::core::sched::cron_job*>(job->id(), job));
                return job;
            }

            

        }
    }
}