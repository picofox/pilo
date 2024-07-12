#pragma once
#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            template<class _Mutex>
            class file_lock_guard
            {	// class with destructor that unlocks mutex
                public:
                typedef _Mutex mutex_type;

                explicit file_lock_guard(_Mutex& _Mtx)
                    : _MyMutex(_Mtx)
                {	// construct and lock
                    _MyMutex.lock();
                }

                ~file_lock_guard() noexcept
                {	// unlock
                    _MyMutex.unlock();
                }

                private:
                M_DISABLE_COPY(file_lock_guard)
                _Mutex& _MyMutex;
            };
        }
    }
}