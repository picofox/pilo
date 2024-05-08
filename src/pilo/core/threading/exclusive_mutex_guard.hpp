#pragma once
#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            template<class _Mutex>
            class exclusive_mutex_guard
            {	// class with destructor that unlocks mutex
                public:
                typedef _Mutex mutex_type;

                explicit exclusive_mutex_guard(_Mutex& _Mtx)
                    : _MyMutex(_Mtx)
                {	// construct and lock
                    _MyMutex.lock();
                }

                ~exclusive_mutex_guard() noexcept
                {	// unlock
                    _MyMutex.unlock();
                }

                private:
                M_DISABLE_COPY(exclusive_mutex_guard)
                _Mutex& _MyMutex;
            };
        }
    }
}