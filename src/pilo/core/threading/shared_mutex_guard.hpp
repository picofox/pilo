#pragma once
#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            template<class _Mutex>
            class shared_mutex_guard
            {	// class with destructor that unlocks mutex
            public:
                typedef _Mutex mutex_type;

                explicit shared_mutex_guard(_Mutex& _Mtx)
                    : _MyMutex(_Mtx)
                {	// construct and lock
                    _MyMutex.lock_shared();
                }

                ~shared_mutex_guard() noexcept
                {	// unlock
                    _MyMutex.unlock_shared();
                }

            private:
                M_DISABLE_COPY(shared_mutex_guard)
                _Mutex& _MyMutex;
            };
        }
    }
}