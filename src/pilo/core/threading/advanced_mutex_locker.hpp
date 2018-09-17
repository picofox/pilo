#pragma once

#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            // LOCK PROPERTIES
            struct adopt_lock_t
            {	// indicates adopt lock
            };

            struct defer_lock_t
            {	// indicates defer lock
            };

            struct try_to_lock_t
            {	// indicates try to lock
            };

            const adopt_lock_t adopt_lock;
            const defer_lock_t defer_lock;
            const try_to_lock_t try_to_lock;

            template<class _Mutex>
            class advanced_mutex_locker
            {
            public:
                typedef _Mutex mutex_type;
                //ctors and dtors
                advanced_mutex_locker() M_NOEXCEPT
                    : _mutex(0), _is_owner(false)
                {
                }

                explicit advanced_mutex_locker(_Mutex& _Mtx)
                    : _mutex(&_Mtx), _is_owner(false)
                {
                    _mutex->lock();
                    _is_owner = true;
                }

                advanced_mutex_locker(_Mutex& _Mtx, adopt_lock_t)
                    : _mutex(&_Mtx), _is_owner(true)
                {	// construct and assume already locked
                }

                advanced_mutex_locker(_Mutex& _Mtx, defer_lock_t) M_NOEXCEPT
                    : _mutex(&_Mtx), _is_owner(false)
                {	// construct but don't lock
                }

                advanced_mutex_locker(_Mutex& _Mtx, try_to_lock_t)
                    : _mutex(&_Mtx), _is_owner(_mutex->try_lock())
                {	// construct and try to lock
                }

                advanced_mutex_locker(_Mutex& mtx, pilo::uint_r millisecs) 
                    : _mutex(mtx), _is_owner(_mutex->try_lock_for(millisecs))
                {

                }

                ~advanced_mutex_locker() M_NOEXCEPT
                {	// clean up
                    if (_is_owner)
                    _mutex->unlock();
                }

#if          (M_CXX_FEATURE_RVALUE_REF == 1)
                
                advanced_mutex_locker(advanced_mutex_locker&& _Other) M_NOEXCEPT
                    : _mutex(_Other._mutex), _is_owner(_Other._is_owner)
                {	// move
                    _Other._mutex = 0;
                    _Other._is_owner = false;
                }

                advanced_mutex_locker& operator=(advanced_mutex_locker&& _Other) M_NOEXCEPT
                {	// move
                    if (this != &_Other)
                    {
                        if (_is_owner)
                            _mutex->unlock(); //unlock old mutex, or you will lose control of it
                        _mutex = _Other._mutex;
                        _is_owner = _Other._is_owner;
                        _Other._mutex = 0;
                        _Other._is_owner = false;
                    }
                    return (*this);
                }
#endif

                // lock and unlock operations
                void lock()
                {	// lock the mutex
                    _mutex->lock();
                    _is_owner = true;
                }

                bool try_lock() M_NOEXCEPT
                {	// try to lock the mutex
                    _is_owner = _mutex->try_lock();
                    return (_is_owner);
                }

                bool try_lock_for(pilo::uint_r millisecs)
                {
                    _is_owner = _mutex->try_lock_for(millisecs);
                    return (_is_owner);
                }

                void unlock()
                {	// unlock the mutex
                    _mutex->unlock();
                    _is_owner = false;
                }

                //property
                bool owns_lock() const M_NOEXCEPT
                {	// return true if this object owns the lock
                    return (_is_owner);
                }

                operator bool() const M_NOEXCEPT
                {	// return true if this object owns the lock
                    return (_is_owner);
                }

                _Mutex *mutex() const M_NOEXCEPT
                {	// return internal data
                    return (_mutex);
                }

            private:
                M_DISABLE_COPY(advanced_mutex_locker)

                _Mutex *_mutex;
                bool _is_owner;

            };
        }
    }
}