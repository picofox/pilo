#pragma once

#include "../../pilo.hpp"
#include "../memory/dynamic_memory_pool.hpp"
#include "../threading/spin_mutex.hpp"
#include "../pattern/singleton.hpp"
#include "../stat/pool_object_stat_info.hpp"

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

                

                inline ::pilo::pointer allocate_page_buffer() { return  _page_pool->allocate(); }
                inline void deallocate_page_buffer(::pilo::pointer p) { _page_pool->deallocate(p); }

            private:
                
                page_allocator::page_allocator_type*   _page_pool;
                ::pilo::core::stat::pool_object_stat_manager _pool_object_stat_mgr;
            };
        }
    }
}