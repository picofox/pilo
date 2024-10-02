#pragma once

#include "../../pilo.hpp"
#include "atomic.hpp"
#include <algorithm>
#include "thread_semaphore.hpp"

// Portable single-producer, single-consumer semaphore below:

#ifdef PMI_PFD_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4365)		// Disable erroneous 'conversion from long to unsigned int, signed/unsigned mismatch' error when using `assert`
#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif
#endif

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class thread_lw_semaphore
            {
            public:
                typedef std::make_signed<std::size_t>::type ssize_t;

            private:
                ::pilo::core::threading::weak_atomic<ssize_t>       m_count;
                ::pilo::core::threading::thread_semaphore           m_sema;

                bool _wait_with_spin(std::int64_t timeout_usecs = -1) PMC_NO_TSAN
                {
                    ssize_t oldCount;
                    // Is there a better way to set the initial spin count?
                    // If we lower it to 1000, testBenaphore becomes 15x slower on my Core i7-5930K Windows PC,
                    // as threads start hitting the kernel semaphore.
                    int spin = 1024;
                    while (--spin >= 0)
                    {
                        if (m_count.load() > 0)
                        {
                            m_count.fetch_add_acquire(-1);
                            return true;
                        }

                        pilo_thread_fence_acquire(); // Prevent the compiler from collapsing the loop.
                    }
                    oldCount = m_count.fetch_add_acquire(-1);
                    if (oldCount > 0)
                        return true;
                    if (timeout_usecs < 0)
                    {
                        if (m_sema.wait())
                            return true;
                    }
                    if (timeout_usecs > 0 && m_sema.timed_wait(static_cast<::pilo::i64_t>(timeout_usecs)))
                        return true;
                    // At this point, we've timed out waiting for the semaphore, but the
                    // count is still decremented indicating we may still be waiting on
                    // it. So we have to re-adjust the count, but only if the semaphore
                    // wasn't signaled enough times for us too since then. If it was, we
                    // need to release the semaphore too.
                    while (true)
                    {
                        oldCount = m_count.fetch_add_release(1);
                        if (oldCount < 0)
                            return false;    // successfully restored things to the way they were
                        // Oh, the producer thread just signaled the semaphore after all. Try again:
                        oldCount = m_count.fetch_add_acquire(-1);
                        if (oldCount > 0 && m_sema.try_wait())
                            return true;
                    }
                }

            public:
                PMC_NO_TSAN thread_lw_semaphore(ssize_t initialCount = 0) : m_count(initialCount), m_sema()
                {
                    PMC_ASSERT(initialCount >= 0);
                }

                bool try_wait() PMC_NO_TSAN
                {
                    if (m_count.load() > 0)
                    {
                        m_count.fetch_add_acquire(-1);
                        return true;
                    }
                    return false;
                }

                bool wait() PMC_NO_TSAN
                {
                    return try_wait() || _wait_with_spin();
                }

                bool timed_wait(::pilo::i64_t timeout_usecs) PMC_NO_TSAN
                {
                    return try_wait() || _wait_with_spin(timeout_usecs);
                }

                void notify(ssize_t count = 1) PMC_NO_TSAN
                {
                    PMC_ASSERT(count >= 0);
                    ssize_t oldCount = m_count.fetch_add_release(count);
                    PMC_ASSERT(oldCount >= -1);
                    if (oldCount < 0)
                    {
                        m_sema.notify(1);
                    }
                }

                ::pilo::u64_t available_estimate() const PMC_NO_TSAN
                {
                    ssize_t count = m_count.load();
                    return count > 0 ? static_cast<std::size_t>(count) : 0;
                }
            };
        }
    }
}


#if defined(PMI_PFD_COMPILER_MSVC) && (_MSC_VER < 1700 || defined(__cplusplus_cli))
#pragma warning(pop)
#ifdef __cplusplus_cli
#pragma managed(pop)
#endif
#endif
