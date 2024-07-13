﻿#pragma once
#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            template<class _Mutex>
            class shared_file_lock_guard
            {	// class with destructor that unlocks mutex
            public:
                typedef _Mutex mutex_type;

                explicit shared_file_lock_guard(_Mutex& _Mtx)
                    : _MyMutex(_Mtx)
                {	// construct and lock
                    _MyMutex.lock_shared();
                }

                ~shared_file_lock_guard() noexcept
                {	// unlock
                    _MyMutex.unlock();
                }

            private:
                M_DISABLE_COPY(shared_file_lock_guard)
                _Mutex& _MyMutex;
            };
        }
    }
}