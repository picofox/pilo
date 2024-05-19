#pragma once

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

                enum class path_types
                {
                    cwd = 0,
                    exe = 1,
                    parent = 2,
                    bin = 3,                    
                    log = 4,
                    tmp = 5,
                    count = 6,
                };

            public:
                context();
                ~context();
                ::pilo::core::stat::pool_object_stat_manager& pool_object_stat_mgr() { return _pool_object_stat_mgr;}
                ::pilo::i32_t initialize();
                void finalize();
                inline const ::pilo::core::io::path& proc_path(int which) const { return _proc_paths[which];}
                inline const ::pilo::core::io::path& exec_path() const { return _proc_paths[(int)path_types::exe]; }
                inline const ::pilo::core::io::path& parent_path() const { return _proc_paths[(int)path_types::parent]; }
                inline const ::pilo::core::io::path& bin_path() const { return _proc_paths[(int)path_types::bin]; }

                inline ::pilo::pointer allocate_page_buffer() { return  _page_pool->allocate(); }
                inline void deallocate_page_buffer(::pilo::pointer p) { _page_pool->deallocate(p); }

            private:                
                ::pilo::core::io::path _proc_paths[(int)path_types::count];

                page_allocator::page_allocator_type*   _page_pool;
                ::pilo::core::stat::pool_object_stat_manager _pool_object_stat_mgr;
            };
        }
    }
}