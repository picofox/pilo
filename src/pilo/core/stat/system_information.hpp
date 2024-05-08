#pragma once

#include <string>
#include "../../pilo.hpp"
#include "../pattern/singleton.hpp"

namespace pilo
{
    namespace core
    {
        namespace stat
        {   

            class system_information
            {
            public:
                system_information();

                inline ::pilo::i64_t freq() const { return _freq; }
                inline ::pilo::i32_t num_cpus() const { return _num_cpus; }
                inline ::pilo::i32_t num_cores() const { return _num_cores;  }
                inline ::pilo::i32_t num_logical_cores() const { return _num_logical_processors;  }
                inline ::pilo::i32_t page_size() const { return _page_size; }

                std::string to_string() const;

            private:
                ::pilo::i32_t _num_cpus;
                ::pilo::i32_t _num_cores;
                ::pilo::i32_t _num_logical_processors;
                ::pilo::i32_t _page_size;
                ::pilo::i64_t   _freq;
                  
            };
        }
    }

    //inline ::pilo::core::stat::system_information* pilo_get_system_information()
    //{
    //    return ::pilo::core::pattern::singleton<::pilo::core::stat::system_information>::instance();
    //}
}

#define PMSO_SYSTEM_INFORMATION (::pilo::core::pattern::singleton<::pilo::core::stat::system_information>::instance())