#pragma once

#include <memory>
#include "../memory/dynamic_memory_pool.hpp"
#include "../stat/pool_object_stat_info.hpp"
#include "../io/path.hpp"
#include "../config/core_config.hpp"
#include "../logging/logger_manager.hpp"
#include "../rtti/wired_type_factory.hpp"
#include "../memory/linked_byte_buffer.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            char* xpf_get_proc_name(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen);
            char* xpf_get_proc_basename(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen, const char* suf, ::pilo::i32_t len);

            class context
            {
            public:
                typedef ::pilo::core::memory::compactable_autoreset_object_pool<::pilo::tlv, SP_PMI__TLV_STEP, ::pilo::core::threading::native_mutex>  tlv_pool_type;

            public:
                const static ::pilo::u32_t s_pilo_version = PMF_MAKE_U32_BY_BYTES_BE(1,0,34,0);
                context();
                ~context();
                ::pilo::core::stat::pool_object_stat_manager& pool_object_stat_mgr() { return _pool_object_stat_mgr;}
                ::pilo::i32_t initialize();
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
                        _proc_paths[(int)::pilo::predefined_pilo_dir::cwd].fill_with_cwd(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir::cwd];
                }
                inline const ::pilo::core::io::path& exec_path(bool update = false)
                { 
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir::exe].fill_with_exe(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir::exe]; 
                }
                inline const ::pilo::core::io::path& bin_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir::bin].fill_with_bin(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir::bin];
                }
                inline const ::pilo::core::io::path& home_path(bool update = false)
                { 
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir::home].fill_with_home(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir::home]; 
                }
                inline const ::pilo::core::io::path& cnf_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir::cnf].fill_with_cnf(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir::cnf];
                }
                inline const ::pilo::core::io::path& log_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir::log].fill_with_log(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir::log];
                }
                inline const ::pilo::core::io::path& tmp_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir::cnf].fill_with_tmp(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir::tmp];
                }
                inline const ::pilo::core::io::path& proc_path(::pilo::predefined_pilo_dir which) const { return _proc_paths[(int)which];}
                inline const ::pilo::core::stat::system_information* system_information() const
                {
                    return _system_information;
                }
                inline ::pilo::core::rtti::wired_type_factory* wired_type_factory()
                {
                    return this->_wired_type_facotry;
                }

                inline ::pilo::os_pid_t process_id() const { return _pid; }
                inline ::pilo::os_pid_t parent_process_id() const { return _ppid; }

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


                ::pilo::tlv* allocate_tlv();
                void deallocate_tlv(::pilo::tlv* tlvp);                
                tlv_pool_type* tlv_pool() { return &_tlv_pool;  }

                std::string startup_info() const;

            private:
                ::pilo::core::io::path _proc_paths[(int)::pilo::predefined_pilo_dir::count];
                bool                _initialized;
                ::pilo::os_pid_t    _pid;
                ::pilo::os_pid_t    _ppid;
                std::string         _proc_name;
                std::string         _proc_basename;

                ::pilo::core::stat::system_information* _system_information;
                ::pilo::core::rtti::wired_type_factory* _wired_type_facotry;


                tlv_pool_type _tlv_pool;
                

                ::pilo::core::stat::pool_object_stat_manager _pool_object_stat_mgr;


                ::std::shared_ptr<::pilo::core::config::core_config> _core_config;
                ::pilo::core::logging::logger_manager _logger_manager;
                
            };

            ::pilo::err_t startup_initialize();
            core::process::context* pilo_context();
        }
    }
}