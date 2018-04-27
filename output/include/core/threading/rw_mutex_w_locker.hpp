#pragma once
#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            template<class _Mutex>
            class rw_mutex_w_locker
            {	// class with destructor that unlocks mutex
                public:
                typedef _Mutex mutex_type;

                explicit mutex_locker(_Mutex& _Mtx)
                    : _MyMutex(_Mtx)
                {	// construct and lock
                    _MyMutex.lock_write();
                }

                ~mutex_locker() M_NOEXCEPT
                {	// unlock
                    _MyMutex.unlock_write();
                }

                private:
                M_DISABLE_COPY(rw_mutex_w_locker)
                    _Mutex& _MyMutex;
            };
        }
    }
}