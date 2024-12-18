﻿#include "./context.hpp"
#include "../stat/system_information.hpp"
#include "../../tlv.hpp"
#include "../memory/linked_byte_buffer.hpp"
#include "../string/fixed_width_line_formater.hpp"
#include "process.hpp"
#include "../memory/linked_buffer_node.hpp"
#include <cstdlib>
#include "../threading/efficient_thread_pool.hpp"
#include "../threading/performance_thread_pool.hpp"
#include "../logging/logger_interface.hpp"
#include "../service/service_manager.hpp"
#include "../service/builtin/timer_service.hpp"
#include "../datetime/zoned_clock.hpp"
#include "../logging/logger_interface.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            static ::pilo::core::process::context* _s_pilo_context_instance = nullptr;

#ifdef WINDOWS
            static int s_on_exit(void)
            {
                
                printf("pilo on exit, finanlizing.....\n");
                PILO_CONTEXT->finalize();
                delete _s_pilo_context_instance;
                return PILO_OK;
            }
#else
            static void s_on_exit(void)
            {
                printf("pilo on exit, finanlizing.....\n");
                PILO_CONTEXT->finalize();
                delete _s_pilo_context_instance;
            }
#endif
            
            static void _s_service_manager_pulse_func(::pilo::core::threading::thread_pool_worker_interface* w)
            {
                PMC_UNUSED(w);
                _s_pilo_context_instance->service_manger()->pulse();
            }

            static void _s_dummy_thread_func(::pilo::core::threading::thread_pool_worker_interface* w)
            {
                PMC_UNUSED(w);
            }


            context::context()   
                : _initialized(false), _pid(0), _ppid(0), _proc_name(""), _proc_basename("")
                , _system_information(nullptr), _wired_type_facotry(nullptr)
                , _linked_buffer_node_pool(nullptr)
                , _service_manager(nullptr), _thread_pool(nullptr)
            {
#ifdef WINDOWS
                _tzset();
#else
                tzset();
#endif


                char buffer[PMI_PATH_MAX] = { 0 };
                ::pilo::i32_t rlen = 0;

                _initialized = false;
                _pid = ::pilo::core::process::xpf_current_process_id();
                _ppid = ::pilo::core::process::xpf_current_parent_process_id();
                _timestamp = -1;
                char* cret = xpf_get_proc_name(buffer, sizeof(buffer), &rlen);
                if (cret != nullptr) {
                    _proc_name.assign(cret, rlen);
                    size_t pos = _proc_name.rfind('.');
                    if (pos != std::string::npos) {
                        _proc_basename = _proc_name.substr(0, pos);
                    }
                    if (cret != nullptr && cret != buffer) {
                        PMF_HEAP_FREE(cret);
                    }

                } else {
                    _proc_name.assign("unknow_proc", 11);
                    _proc_basename = _proc_name;
                }                                    
                _core_config = ::std::make_shared<::pilo::core::config::core_config>();     
                this->_system_information = new ::pilo::core::stat::system_information();
                this->_wired_type_facotry = new ::pilo::core::rtti::wired_type_factory();

                this->_linked_buffer_node_pool = new linked_buffer_node_4k_pool_type();
                
                this->_service_manager = nullptr;
                this->_thread_pool = nullptr;

            }

            context::~context()
            {
                
            }

            ::pilo::tlv* context::allocate_tlv()
            {
                return _tlv_pool.allocate();
            }

            void context::deallocate_tlv(::pilo::tlv* tlvp)
            {
                _tlv_pool.deallocate(tlvp);
            }

            ::pilo::core::sched::task* context::allocate_task()
            {
                return _task_pool.allocate();
            }

            ::pilo::core::sched::task* context::allocate_task(::pilo::core::sched::task_func_type f_func, void* obj, void* param, void* ctx, ::pilo::core::sched::task_destructor_func_type d_func)
            {
                ::pilo::core::sched::task* task = _task_pool.allocate();
                task->set(f_func, obj, param, ctx, d_func);
                return task;
            }

            void context::deallocate_task(::pilo::core::sched::task* task)
            {
                _task_pool.deallocate(task);
            }

            ::pilo::core::sched::timer* context::allocate_timer()
            {
                return _timer_pool.allocate();
            }

            void context::deallocate_timer(::pilo::core::sched::timer* t)
            {
                _timer_pool.deallocate(t);
            }

            ::pilo::core::memory::linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE> * context::allocate_linked_buffer_node_4k()
            {
                return this->_linked_buffer_node_pool->allocate();
            }

            void context::deallocate_linked_buffer_node_4k(::pilo::core::memory::linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE>* node_ptr)
            {
                this->_linked_buffer_node_pool->deallocate(node_ptr);
            }

            std::string context::startup_info() const
            {
                char buffer[64] = {0};
                std::stringstream ss;
                ::pilo::core::string::fixed_width_line_formater formater;

                formater.add_meta_field(16, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Item ");
                formater.add_meta_field(56, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Info");
                formater.format_header(ss);

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Process:");
                formater.format_field(ss, 1, this->process_basename().c_str());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t) 0, (const char*) "PILO Version:");
                ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%u.%u.%u.%s"
                                                         , major_version(), minor_version(), revision(), stage_cstr());
                formater.format_field(ss, 1, (const char *)buffer);
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t) 0, (const char*) "Executable");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_path::exe).fullpath());
                ss << std::endl;

                ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%d (%d)", _pid, _ppid);
                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"IDS");
                formater.format_field(ss, 1, (const char*) buffer);
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"CWD");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_path::cwd).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Home");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_path::home).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Bin");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_path::bin).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Cnf");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_path::cnf).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Log");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_path::log).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Tmp");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_path::tmp).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Core-CFG");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_path::core_cfg).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Cmd-Args");
                formater.format_field(ss, 1, this->_cmdline_arg.args_to_string().c_str());
                ss << std::endl;
                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Cmd-Targets");
                formater.format_field(ss, 1, this->_cmdline_arg.targets_to_string().c_str());
                ss << std::endl;

                return ss.str();
            }

            ::pilo::i32_t context::initialize(int argc, char* argv[])
            {
                if (_initialized)
                    return PILO_OK;

                ::pilo::err_t err = PILO_OK;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_path::cwd].fill_with_cwd(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_path::exe].fill_with_exe(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_path::bin].fill_with_bin(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_path::home].fill_with_home(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_path::cnf].fill_with_cnf(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_path::log].fill_with_log(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_path::tmp].fill_with_tmp(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_path::core_cfg].fill_with_core_cfg(0);
                if (err != PILO_OK)
                    return err;

                err = _core_config->load_or_save_default();
                if (err != PILO_OK) {
                    ::pilo::core::io::file_formatted_output(stderr, "load_or_save_default core config failed. (%s)\n", ::pilo::str_err(err, nullptr, true).c_str());
                    return err;
                }

                std::string errmsg;
                if (!_core_config->cwd().empty()) {
                    ::pilo::core::io::path tmp_cwd(_core_config->cwd().c_str(), (::pilo::pathlen_t) _core_config->cwd().size());
                    if (tmp_cwd.invalid()) {
                        errmsg = "cwd -> ";
                        errmsg += _core_config->cwd();
                        errmsg += " is invalid.";
                        return ::pilo::mk_perr(PERR_INVALID_PATH);
                    }                                          
                    err = xpf_change_current_directory(_core_config->cwd().c_str(), (::pilo::pathlen_t) _core_config->cwd().size());
                    if (err != PILO_OK) {
                        errmsg = "change cwd -> ";
                        errmsg += _core_config->cwd();
                        errmsg += " Failed.";
                        return err;
                    }
                    err = this->_proc_paths[(int) ::pilo::predefined_pilo_path::cwd].fill_with_cwd(0);
                    if (err != PILO_OK) {
                        errmsg = "refill cwd -> ";
                        errmsg += _core_config->cwd();
                        errmsg += " Failed.";
                        return err;
                    }
                }

                err = _cmdline_arg.parse(argc, argv, errmsg);
                if (err != PILO_OK) {
                    ::pilo::core::io::file_formatted_output(stderr, "Parse cmdline arguments failed. (%s)\n", errmsg.c_str());
                    return err;
                }

                _pool_object_stat_mgr.register_item(::pilo::core::stat::pool_object_stat_manager::pool_object_key_code::key_tlv
                    , sizeof(::pilo::tlv), [](::pilo::core::stat::pool_object_stat_manager::pool_object_key_code 
                        , ::pilo::core::stat::pool_object_stat_manager::stat_item* si) -> ::pilo::err_t { return ::pilo::tlv::update_pool_object_stat(si);}
                    , "pilo_tlv"
                );
                _pool_object_stat_mgr.register_item(::pilo::core::stat::pool_object_stat_manager::pool_object_key_code::linked_buffer_node_4k
                    , sizeof(::pilo::core::memory::linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE>), [](::pilo::core::stat::pool_object_stat_manager::pool_object_key_code
                        , ::pilo::core::stat::pool_object_stat_manager::stat_item* si) -> ::pilo::err_t { return ::pilo::core::memory::linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE>::update_pool_object_stat(si); }
                    , "local_bn"
                );

                _environment_variable_manager.initialize();
                err = _logger_manager.initialize(_core_config->loggers());
                if (err != PILO_OK)
                    return err;
                if (this->_core_config->overrided_timezone() == PMI_USE_SYSTEM_TIMEZONE) {
                    ::pilo::i8_t tt = PILO_CONTEXT->system_information()->system_timezone();
                    this->_core_config->set_overrided_timezone(tt);
                }

#ifdef WINDOWS
                _onexit(s_on_exit);
#else
                atexit(s_on_exit);
#endif

                if (!core_config()->thread_pool().name().empty()) {
                    if (!core_config()->core_services().empty()) {
                        if (core_config()->thread_pool().performance_mode()) {
                            _thread_pool = new ::pilo::core::threading::performance_thread_pool(&core_config()->thread_pool(), nullptr, _s_service_manager_pulse_func, nullptr);
                        }
                        else {
                            _thread_pool = new ::pilo::core::threading::efficient_thread_pool(&core_config()->thread_pool(), nullptr, _s_service_manager_pulse_func, nullptr);
                        }
                        if (_thread_pool == nullptr) {
                            errmsg = "create thread pool failed.";
                            return ::pilo::mk_perr(PERR_CREATE_OBJ_FAIL);
                        }

                        _service_manager = new ::pilo::core::service::service_manager();
                        err = _service_manager->initialize(&core_config()->core_services());
                    }
                    else {
                        if (core_config()->thread_pool().performance_mode()) {
                            _thread_pool = new ::pilo::core::threading::performance_thread_pool(&core_config()->thread_pool(), nullptr, _s_dummy_thread_func, nullptr);
                        }
                        else {
                            _thread_pool = new ::pilo::core::threading::efficient_thread_pool(&core_config()->thread_pool(), nullptr, _s_dummy_thread_func, nullptr);
                        }
                        if (_thread_pool == nullptr) {
                            errmsg = "create thread pool failed.";
                            return ::pilo::mk_perr(PERR_CREATE_OBJ_FAIL);
                        }
                        _service_manager = nullptr;
                    }
                }
                else {
                    _thread_pool = nullptr;
                }

                if (_thread_pool != nullptr) {
                    _thread_pool->start();
                }

                _initialized = true;
                
                return PILO_OK;
            }

            ::pilo::err_t context::start()
            {
                ::pilo::err_t err = PILO_OK;
                if (_service_manager != nullptr)
                    err = _service_manager->start();
                if (err != PILO_OK)
                    return err;

                return PILO_OK;                    
            }

            ::pilo::i64_t context::add_rel_milli_timer(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor)
            {
                return _service_manager->timer_service_cache()->add_rel_milli_timer(duration, rep_cnt, rep_dura, f_func, obj, param, dtor);
            }

            ::pilo::i64_t context::add_rel_sec_timer(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor)
            {
                return _service_manager->timer_service_cache()->add_rel_sec_timer(duration, rep_cnt, rep_dura, f_func, obj, param, dtor);
            }

            ::pilo::i64_t context::add_abs_milli_timer(::pilo::i64_t epoch, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor)
            {
                return _service_manager->timer_service_cache()->add_abs_milli_timer(epoch, rep_cnt, rep_dura, f_func, obj, param, dtor);
            }

            ::pilo::i64_t context::add_abs_sec_timer(::pilo::i64_t epoch, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura, ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor)
            {
                return _service_manager->timer_service_cache()->add_abs_sec_timer(epoch, rep_cnt, rep_dura, f_func, obj, param, dtor);
            }

            ::pilo::i64_t context::start_neo_cron_job(::pilo::i8_t tz, const std::string& spec, ::pilo::core::sched::task_func_type f_func, void* param, ::pilo::core::sched::task_destructor_func_type dtor)
            {
                ::pilo::i64_t id = _cron_scheduler.start_neo_job(tz, spec, f_func, param, dtor);
                if (id > 0) {
                    PLOG(::pilo::core::logging::level::info, "[cron] job (%d-%lld:%s) is added", ::pilo::core::sched::cron_scheduler::timezone_of_id(id), ::pilo::core::sched::cron_scheduler::seq_of_id(id), spec.c_str());
                }
                else {
                    PLOG(::pilo::core::logging::level::error, "[cron] job (%d-%lld:%s) add failed.", ::pilo::core::sched::cron_scheduler::timezone_of_id(id), ::pilo::core::sched::cron_scheduler::seq_of_id(id), spec.c_str());
                }
                return id;
            }

            ::pilo::err_t context::stop_cron_job(::pilo::i64_t job_id)
            {
                ::pilo::err_t err = this->_cron_scheduler.stop_job(job_id);
                if (err != PILO_OK) {
                    PLOG(::pilo::core::logging::level::error, "[cron] stop cron job (%d-%lld) failed."
                        , ::pilo::core::sched::cron_scheduler::timezone_of_id(job_id)
                        , ::pilo::core::sched::cron_scheduler::seq_of_id(job_id));
                }
                else {
                    PLOG(::pilo::core::logging::level::info, "[cron] cron job (%d-%lld) marked stopped."
                        , ::pilo::core::sched::cron_scheduler::timezone_of_id(job_id)
                        , ::pilo::core::sched::cron_scheduler::seq_of_id(job_id));
                }
                return err;
            }

            ::pilo::err_t context::on_cron_job_check(void* entry)
            {
                ::pilo::core::sched::cron_job* job = (::pilo::core::sched::cron_job*)entry;
                if (job == nullptr) {
                    return ::pilo::mk_perr(PERR_NULL_PTR);
                }

                if (job->is_stop()) {
                    char buffer[64] = { 0 };
                    job->to_cstring(buffer, sizeof(buffer));
                    ::pilo::err_t err = job->delete_job();
                    if (err != PILO_OK) {
                        PLOG(::pilo::core::logging::level::error, "[cron] delete cron job (%s) failed.", buffer);
                        return ::pilo::mk_perr(PERR_NON_EXIST);
                    }
                    PLOG(::pilo::core::logging::level::info, "[cron] cron job (%s) deleted.", buffer);
                    return ::pilo::mk_perr(PERR_NOOP);
                }
                return PILO_OK;
            }

            ::pilo::err_t context::on_cron_job_continue(void* entry)
            {
                ::pilo::core::sched::cron_job* job = (::pilo::core::sched::cron_job*)entry;
                if (job == nullptr) {
                    return ::pilo::mk_perr(PERR_NULL_PTR);
                }
                
                return job->schedule(PMI_INVALID_TIMESTAMP);
            }

            

            void context::delete_timer(::pilo::i64_t timer_id)
            {
                return _service_manager->timer_service_cache()->delete_timer(timer_id);
            }


            void context::finalize()
            {
                std::string str = this->_pool_object_stat_mgr.to_string();
                std::cout << str << std::endl;
                PLOG(::pilo::core::logging::level::info, "pilo finalizing");
                PLOG(::pilo::core::logging::level::info, "stopping service manager.");
                _service_manager->stop();
                PLOG(::pilo::core::logging::level::info, "stopping thread pool.");
                _thread_pool->stop();   
            }

            ::pilo::err_t pilo_startup_initialize(int argc, char* argv[])
            {
                if (_s_pilo_context_instance != nullptr)
                    return PILO_OK;

                _s_pilo_context_instance = new ::pilo::core::process::context();
                if (_s_pilo_context_instance == nullptr)
                    return ::pilo::mk_perr(PERR_INSUF_HEAP);
                

                ::pilo::err_t err = _s_pilo_context_instance->initialize(argc, argv);

                if (err != PILO_OK) {
                    fprintf(stderr, "PILO Initilization Failed. (%s)", ::pilo::str_err(err, nullptr, true).c_str());
                    return err;
                }
                

                std::string si = _s_pilo_context_instance->startup_info();
                _s_pilo_context_instance->logger(0)->log_raw(si.c_str(), (::pilo::i64_t) si.size());


                return PILO_OK;
            }

            ::pilo::core::process::context* pilo_context()
            {
                return _s_pilo_context_instance;
            }
           
        }
    }
}