﻿#pragma once

#include "../../pilo.hpp"
#include "../memory/dynamic_memory_pool.hpp"
#include "../threading/spin_mutex.hpp"
#include "../pattern/singleton.hpp"
#include "../stat/pool_object_stat_info.hpp"
#include "../io/path.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
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
                context();
                ~context();
                ::pilo::core::stat::pool_object_stat_manager& pool_object_stat_mgr() { return _pool_object_stat_mgr;}
                ::pilo::i32_t initialize();
                void finalize();
                inline const ::pilo::core::io::path& cwd(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::cwd].fill_with_cwd(0);
                    }
                    return _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::cwd];
                }
                inline const ::pilo::core::io::path& exec_path(bool update = false)
                { 
                    if (update)
                    {
                        _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::exe].fill_with_exe(0);
                    }
                    return _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::exe]; 
                }
                inline const ::pilo::core::io::path& bin_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::bin].fill_with_bin(0);
                    }
                    return _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::bin];
                }
                inline const ::pilo::core::io::path& home_path(bool update = false)
                { 
                    if (update)
                    {
                        _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::home].fill_with_home(0);
                    }
                    return _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::home]; 
                }
                inline const ::pilo::core::io::path& cnf_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::cnf].fill_with_cnf(0);
                    }
                    return _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::cnf];
                }
                inline const ::pilo::core::io::path& log_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::log].fill_with_log(0);
                    }
                    return _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::log];
                }
                inline const ::pilo::core::io::path& tmp_path(bool update = false)
                {
                    if (update)
                    {
                        _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::cnf].fill_with_tmp(0);
                    }
                    return _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::tmp];
                }
                inline const ::pilo::core::io::path& proc_path(::pilo::core::io::path::predefined_pilo_dir_enum which) const { return _proc_paths[(int)which];}
                
              
                inline ::pilo::pointer allocate_page_buffer() { return  _page_pool->allocate(); }
                inline void deallocate_page_buffer(::pilo::pointer p) { _page_pool->deallocate(p); }

                std::string startup_info() const;

            private:                
                ::pilo::core::io::path _proc_paths[(int)::pilo::core::io::path::predefined_pilo_dir_enum::count];



                page_allocator::page_allocator_type*   _page_pool;
                ::pilo::core::stat::pool_object_stat_manager _pool_object_stat_mgr;
            };
        }
    }
}