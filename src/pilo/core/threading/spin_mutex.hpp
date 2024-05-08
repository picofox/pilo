#pragma once
#include "../../pilo.hpp"

#define PMC_DEAD_LOCK_DETECT

#ifdef PMC_DEAD_LOCK_DETECT
#include <atomic>
#endif

namespace pilo
{
	namespace core
	{
		namespace threading
		{
			class spin_mutex 
			{				
			public:
				spin_mutex()
				{
#ifdef WINDOWS
					(void) InitializeCriticalSectionAndSpinCount(&_mutex, (DWORD)(0xFFFFFFFF)); //higher bit is set, gurentee prealloc internal event
#else
					pthread_spin_init(&_mutex, PTHREAD_PROCESS_PRIVATE);
#endif
#ifdef PMC_DEAD_LOCK_DETECT
					_op.store(0);
#endif
				}
				~spin_mutex()
				{
#ifdef WINDOWS
					DeleteCriticalSection(&_mutex);
#else
					pthread_spin_destroy(&_mutex);
#endif
				}
				spin_mutex(const spin_mutex&) = delete;
				spin_mutex& operator= (const spin_mutex&) = delete;
				spin_mutex(spin_mutex&&) = delete;
				spin_mutex& operator= (spin_mutex&&) = delete;

				void lock()
				{
					if (_op.load() == 1)
						PMC_ASSERT("REEN LOCK");
#ifdef WINDOWS
					EnterCriticalSection(&_mutex);
#else
					pthread_spin_lock(&_mutex);
#endif
					_op.store(1);
						
				}
				void unlock()
				{
					if (_op.load() == 0)
						PMC_ASSERT("REEN LOCK");
					_op.store(0);
#ifdef WINDOWS
					LeaveCriticalSection(&_mutex);
#else
					pthread_spin_unlock(&_mutex);
#endif
				}

				bool try_lock()
				{

#ifdef WINDOWS
					return TryEnterCriticalSection(&_mutex) == TRUE;
#else
					return  pthread_spin_trylock(&_mutex) == 0;
#endif
				}

			private:
#ifdef WINDOWS
				CRITICAL_SECTION		_mutex;
#else
				pthread_spinlock_t		_mutex;
#endif

#ifdef PMC_DEAD_LOCK_DETECT
				std::atomic<int>		_op;
#endif
				
			};
		}
	}
}