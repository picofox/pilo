#pragma once

#include <memory>
#include "../memory/dynamic_memory_pool.hpp"
#include "../stat/pool_object_stat_info.hpp"
#include "../io/path.hpp"
#include "../config/core_config.hpp"
#include "../logging/logger_manager.hpp"
#include "../rtti/wired_type_factory.hpp"
#include "../threading/spin_mutex.hpp"
#include "./cmdline_args.hpp"
#include "./environment_variable_manager.hpp"
#include "../sched/task.hpp"
#include "../threading/thread_pool_interface.hpp"
#include "../service/service_manager.hpp"
#include "../sched/timer.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template<::pilo::i64_t TA_UNIT_SIZE>
            class linked_buffer_node;
        }

        namespace process
        {
            class context
            {
            public:
                typedef ::pilo::core::memory::compactable_autoreset_object_pool<::pilo::tlv, SP_PMI_TLV_STEP, ::pilo::core::threading::native_mutex>  tlv_pool_type;
                typedef ::pilo::core::memory::compactable_object_pool<::pilo::core::memory::linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE>, SP_PMI_LBKBUF_NODE_4K_STEP_SIZE, ::pilo::core::threading::spin_mutex> linked_buffer_node_4k_pool_type;
                typedef ::pilo::core::memory::compactable_autoreset_object_pool<::pilo::core::sched::task, SP_PMI_TASK_STEP, ::pilo::core::threading::native_mutex>  task_pool_type;
                typedef ::pilo::core::memory::compactable_autoreset_object_pool<::pilo::core::sched::timer, SP_PMI_TIMER_STEP, ::pilo::core::threading::native_mutex>  timer_pool_type;

            public:
                const static ::pilo::u32_t s_pilo_version = PMF_MAKE_U32_BY_BYTES_BE(1,0,35,0);
                context();
                ~context();
                ::pilo::core::stat::pool_object_stat_manager& pool_object_stat_mgr() { return _pool_object_stat_mgr;}
                ::pilo::i32_t initialize(int argc, char* argv[]);
                void finalize();
                inline ::pilo::u32_t version() const
                {
                    return s_pilo_version;
                }
                inline ::pilo::u8_t major_version() const
                {
                    return PMF_EXTRACT_U8(s_pilo_version, 3);
                }
                inline ::pilo::u8_t minor_version() const
                {
                    return PMF_EXTRACT_U8(s_pilo_version, 2);
                }
                inline ::pilo::u8_t revision() const
                {
                    return PMF_EXTRACT_U8(s_pilo_version, 1);
                }
                inline const char* stage_cstr() const
                {
                    const static char* cst_parr[] = {"base", "alpha", "beta", "RC", "release"};
                    ::pilo::u8_t i = PMF_EXTRACT_U8(s_pilo_version, 0);
                    return cst_parr[i];
                }
                inline const std::string& process_name() const
                {
                    return this->_proc_name;
                }

                inline const std::string& process_basename() const
                {
                    return this->_proc_basename;
                }

                inline const ::pilo::core::io::path& cwd(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_path::cwd].fill_with_cwd(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_path::cwd];
                }
                inline const ::pilo::core::io::path& exec_path(bool update = false)
                { 
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_path::exe].fill_with_exe(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_path::exe]; 
                }
                inline const ::pilo::core::io::path& bin_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_path::bin].fill_with_bin(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_path::bin];
                }
                inline const ::pilo::core::io::path& home_path(bool update = false)
                { 
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_path::home].fill_with_home(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_path::home]; 
                }
                inline const ::pilo::core::io::path& cnf_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_path::cnf].fill_with_cnf(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_path::cnf];
                }
                inline const ::pilo::core::io::path& log_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_path::log].fill_with_log(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_path::log];
                }
                inline const ::pilo::core::io::path& tmp_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_path::tmp].fill_with_tmp(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_path::tmp];
                }
                inline const ::pilo::core::io::path& core_cfg_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_path::tmp].fill_with_core_cfg(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_path::core_cfg];
                }

                inline const ::pilo::core::io::path& proc_path(::pilo::predefined_pilo_path which) const { return _proc_paths[(int)which];}
                inline const ::pilo::core::stat::system_information* system_information() const
                {
                    return _system_information;
                }
                inline ::pilo::core::rtti::wired_type_factory* wired_type_factory()
                {
                    return this->_wired_type_facotry;
                }

                inline ::pilo::os_pid_t process_id() const { return _pid; }
                inline ::pilo::os_pid_t xpf_parent_process_id() const { return _ppid; }

                inline ::pilo::err_t load_core_config()
                {
                    ::pilo::core::config::core_config *cfg = new ::pilo::core::config::core_config;
                    ::pilo::err_t err = cfg->load();
                    if (err != PILO_OK)
                        return err;
                    _core_config.reset(cfg);
                    return PILO_OK;
                }

                inline ::std::shared_ptr<::pilo::core::config::core_config> core_config() const {
                    return _core_config;
                }

                inline ::pilo::core::logging::logger_interface* logger(::pilo::i32_t idx)
                {
                    return _logger_manager.at(idx);
                }

                inline ::pilo::i64_t timestamp() const
                {
                    return _timestamp;
                }

                inline void update_timestamp()
                {
                    _timestamp = ::pilo::core::datetime::timestamp_micro_steady();
                }

                ::pilo::tlv* allocate_tlv();
                void deallocate_tlv(::pilo::tlv* tlvp);                
                tlv_pool_type* tlv_pool() { return &_tlv_pool;  }
                task_pool_type* task_pool() { return &_task_pool;  }
                ::pilo::core::sched::task* allocate_task();
                ::pilo::core::sched::task* allocate_task(::pilo::core::sched::task_func_type f_func, void* obj, void* param, void * context, ::pilo::core::sched::task_destructor_func_type dtor);
                void deallocate_task(::pilo::core::sched::task* task);
                ::pilo::core::sched::timer* allocate_timer();
                void deallocate_timer(::pilo::core::sched::timer * ptimer);


                ::pilo::core::memory::linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE>* allocate_linked_buffer_node_4k();
                void deallocate_linked_buffer_node_4k(::pilo::core::memory::linked_buffer_node<SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE> * node_ptr);
                linked_buffer_node_4k_pool_type* linked_buffer_node_4k_pool() { return _linked_buffer_node_pool; }
 
                std::string startup_info() const;

                inline const cmdline_arg& cmdline_args() const { return _cmdline_arg;  }
                inline ::pilo::core::service::service_manager* service_manger() { return _service_manager;  }
                inline ::pilo::core::threading::thread_pool_interface* thread_pool() { return _thread_pool;  }

                ::pilo::err_t start();

                inline void post_task(::pilo::core::sched::task* task_ptr)
                {
                    this->_thread_pool->post_task(task_ptr);
                }

                ::pilo::i64_t add_rel_milli_timer(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura
                    , ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor);
                ::pilo::i64_t add_rel_sec_timer(::pilo::u32_t duration, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura
                    , ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor);
                ::pilo::i64_t add_abs_milli_timer(::pilo::i64_t epoch, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura
                    , ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor);
                ::pilo::i64_t add_abs_sec_timer(::pilo::i64_t epoch, ::pilo::u32_t rep_cnt, ::pilo::u32_t rep_dura
                    , ::pilo::core::sched::task_func_type f_func, void* obj, void* param, ::pilo::core::sched::task_destructor_func_type dtor);

                

            private:
                ::pilo::core::io::path _proc_paths[(int)::pilo::predefined_pilo_path::count];
                bool                _initialized;
                ::pilo::os_pid_t    _pid;
                ::pilo::os_pid_t    _ppid;
                volatile ::pilo::i64_t       _timestamp;
                std::string         _proc_name;
                std::string         _proc_basename;


                ::pilo::core::stat::system_information* _system_information;
                ::pilo::core::rtti::wired_type_factory* _wired_type_facotry;


                tlv_pool_type _tlv_pool;
                task_pool_type _task_pool;
                timer_pool_type _timer_pool;
                linked_buffer_node_4k_pool_type *_linked_buffer_node_pool;
                

                ::pilo::core::stat::pool_object_stat_manager _pool_object_stat_mgr;

                ::std::shared_ptr<::pilo::core::config::core_config> _core_config;
                cmdline_arg                                          _cmdline_arg;
                environment_variable_manager                         _environment_variable_manager;
                

                ::pilo::core::service::service_manager *        _service_manager;
                ::pilo::core::threading::thread_pool_interface* _thread_pool;


                ::pilo::core::logging::logger_manager _logger_manager;


                
                
            };

            ::pilo::err_t pilo_startup_initialize(int argc, char*argv[]);
            core::process::context* pilo_context();
        }
    }
}

#define PLOG_RELEASE(lv, fmt, ...)   do {::pilo::core::process::pilo_context()->logger(0)->log(lv, fmt, ##__VA_ARGS__);} while(0)
#define PLOG_DEBUG(lv, fmt, ...)   do {::pilo::core::process::pilo_context()->logger(0)->log_debug(__FILE__,__LINE__,lv, fmt, ##__VA_ARGS__);} while(0)

#define PLOG    PLOG_DEBUG