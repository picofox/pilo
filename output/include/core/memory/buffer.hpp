#pragma once
#include "core/coredefs.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/threading//dummy_mutex.hpp"
#include "core/memory/fixed_buffer.hpp"
#include "core/memory/dynamic_buffer.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template<size_t _Capacity, typename _Lock = pilo::core::threading::dummy_mutex>
            class buffer : public ::pilo::core::memory::fixed_buffer<_Capacity, _Lock>
            {

            };

            template<typename _Lock = pilo::core::threading::dummy_mutex>
            class buffer<0> : public ::pilo::core::memory::dynamic_buffer<, _Lock>
            {

            };
        }
    }
}