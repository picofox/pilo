#pragma once

#include "../../pilo.hpp"
#include "../memory/dynamic_memory_pool.hpp"
#include "../threading/spin_mutex.hpp"
#include "../pattern/singleton.hpp"
#include "../stat/pool_object_stat_info.hpp"
#include "../io/path.hpp"
#include "./process.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            char* xpf_get_proc_name(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen);


            class context
            {
            public:
                class page_allocator
                {
                public:
                    typedef ::pilo::core::memory::dynamic_memory_pool<::pilo::core::threading::spin_mutex> page_allocator_type;
                    inline static ::pilo::pointer allocate() {return ::pilo::core::pattern::singleton<page_allocator_type>::instance()->allocate(); }
                    inline static void deallocate(::pilo::pointer p) { ::pilo::core::pattern::singleton<page_allocator_type>::instance()->deallocate(p); }

                };                

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
                inline const char* process_name() const
                {
                    return this->_proc_name.c_str();
                }

                inline const char* process_basename() const
                {
                    return this->_proc_basename.c_str();
                }

                inline const ::pilo::core::io::path& cwd(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir_enum::cwd].fill_with_cwd(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir_enum::cwd];
                }
                inline const ::pilo::core::io::path& exec_path(bool update = false)
                { 
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir_enum::exe].fill_with_exe(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir_enum::exe]; 
                }
                inline const ::pilo::core::io::path& bin_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir_enum::bin].fill_with_bin(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir_enum::bin];
                }
                inline const ::pilo::core::io::path& home_path(bool update = false)
                { 
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir_enum::home].fill_with_home(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir_enum::home]; 
                }
                inline const ::pilo::core::io::path& cnf_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir_enum::cnf].fill_with_cnf(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir_enum::cnf];
                }
                inline const ::pilo::core::io::path& log_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir_enum::log].fill_with_log(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir_enum::log];
                }
                inline const ::pilo::core::io::path& tmp_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::predefined_pilo_dir_enum::cnf].fill_with_tmp(0);
                    }
                    return _proc_paths[(int)::pilo::predefined_pilo_dir_enum::tmp];
                }
                inline const ::pilo::core::io::path& proc_path(::pilo::predefined_pilo_dir_enum which) const { return _proc_paths[(int)which];}
                
              
                inline ::pilo::pointer allocate_page_buffer() { return  _page_pool->allocate(); }
                inline void deallocate_page_buffer(::pilo::pointer p) { _page_pool->deallocate(p); }

                inline ::pilo::os_pid_t process_id() const { return _pid; }
                inline ::pilo::os_pid_t parent_process_id() const { return _ppid; }

                std::string startup_info() const;

            private:
                ::pilo::core::io::path _proc_paths[(int)::pilo::predefined_pilo_dir_enum::count];
                ::pilo::os_pid_t    _pid;
                ::pilo::os_pid_t    _ppid;
                std::string         _proc_name;
                std::string         _proc_basename;

                page_allocator::page_allocator_type*   _page_pool;
                ::pilo::core::stat::pool_object_stat_manager _pool_object_stat_mgr;
            };
        }
    }
}