﻿#include    "../process/context.hpp"
#include    "./cron_resolving.hpp"
#include    "../datetime/zoned_clock.hpp"
#include    "../algorithm/uint_sequence_generator.hpp"
#include    "../logging/logger_interface.hpp"
#include    <thread>
#include "cron_job.hpp"
#include "../process/context.hpp"


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

            ::pilo::err_t core::sched::cron_job::schedule(::pilo::i64_t now)
            {
                if (now < 0) {
                    now = _prev_ts;
                }
                 ::pilo::i64_t next_ts = ::pilo::core::sched::cron_next(*_cronexpr, now, this->timezone());
                if (next_ts == PMI_INVALID_TIMESTAMP) {
                    return -1;
                }
                ::pilo::i64_t tid = PILO_CONTEXT->add_abs_sec_timer((::pilo::u32_t)next_ts, 0, 0, this->_func, this, this->_param, this->_dtor);
                if (tid < 0) {
                    return ::pilo::mk_perr(PERR_CREATE_OBJ_FAIL);
                }

                _prev_ts = next_ts;
                return PILO_OK;
            }

            ::pilo::err_t cron_job::initialize(cron_scheduler* scheduler, ::pilo::i8_t tz, const std::string& spec, task_func_type f_func,  void* param, task_destructor_func_type dtor)
            {
                if (_cronexpr != nullptr || f_func == nullptr) {
                    return ::pilo::mk_perr(PERR_EXIST);
                }
                _cronexpr = ::pilo::core::sched::make_cron(spec);
                if (_cronexpr == nullptr) {
                    return ::pilo::mk_perr(PERR_CREATE_OBJ_FAIL);
                }
                _id = s_cron_job_id_generator.next() | (((::pilo::i64_t) tz) << 48);
                _func = f_func;
                _param = param;
                _dtor = dtor;
                _expr = spec;
                _scheduler = scheduler;
                return PILO_OK;
            }

            void cron_job::reset()
            {
                if (_cronexpr != nullptr) {
                    delete _cronexpr;
                    _cronexpr = nullptr;
                }
                
                _scheduler = nullptr;
            }

            std::string cron_job::to_string() const
            {
                char buffer[128] = { 0 };
                to_cstring(buffer, sizeof(buffer));
                return std::string(buffer);
            }

            const char* cron_job::to_cstring(char* buffer, ::pilo::i64_t sz) const
            {
                ::pilo::i64_t seq = _id & 0xFFFFFFFFFFFFLL;
                ::pilo::core::io::string_formated_output(buffer, sz, "%d-%lld:%s", this->timezone(), seq, _expr.c_str());
                return buffer;
            }

            ::pilo::err_t cron_job::delete_job()
            {
                return this->_scheduler->delete_job(this->_id);
            }

        }
    }
}