#pragma once
#include "../../pilo.hpp"
#include <cstddef> // For std::size_t
#include <atomic>
#include <type_traits> // For std::make_signed<T>
#include "thread_lw_semaphore.hpp"


namespace pilo
{
    namespace core
    {
        namespace threading
        {

            //---------------------------------------------------------
            // LightweightSemaphore
            //---------------------------------------------------------
            class thread_lw_semaphore_n
            {
            public:
                typedef std::make_signed<std::size_t>::type ssize_t;

            private:
                std::atomic<ssize_t> m_count;
                ::pilo::core::threading::thread_semaphore m_sema;
                int m_maxSpins;

                bool _wait_with_spin(std::int64_t timeout_usecs = -1)
                {
                    ssize_t oldCount;
                    int spin = m_maxSpins;
                    while (--spin >= 0)
                    {
                        oldCount = m_count.load(std::memory_order_relaxed);
                        if ((oldCount > 0) && m_count.compare_exchange_strong(oldCount, oldCount - 1, std::memory_order_acquire, std::memory_order_relaxed))
                            return true;
                        std::atomic_signal_fence(std::memory_order_acquire);	 // Prevent the compiler from collapsing the loop.
                    }
                    oldCount = m_count.fetch_sub(1, std::memory_order_acquire);
                    if (oldCount > 0)
                        return true;
                    if (timeout_usecs < 0)
                    {
                        if (m_sema.wait())
                            return true;
                    }
                    if (timeout_usecs > 0 && m_sema.timed_wait((std::uint64_t)timeout_usecs))
                        return true;
                    // At this point, we've timed out waiting for the semaphore, but the
                    // count is still decremented indicating we may still be waiting on
                    // it. So we have to re-adjust the count, but only if the semaphore
                    // wasn't signaled enough times for us too since then. If it was, we
                    // need to release the semaphore too.
                    while (true)
                    {
                        oldCount = m_count.load(std::memory_order_acquire);
                        if (oldCount >= 0 && m_sema.try_wait())
                            return true;
                        if (oldCount < 0 && m_count.compare_exchange_strong(oldCount, oldCount + 1, std::memory_order_relaxed, std::memory_order_relaxed))
                            return false;
                    }
                }

                ssize_t _wait_n_with_spin(ssize_t max, std::int64_t timeout_usecs = -1)
                {
                    PMC_ASSERT(max > 0);
                    ssize_t oldCount;
                    int spin = m_maxSpins;
                    while (--spin >= 0)
                    {
                        oldCount = m_count.load(std::memory_order_relaxed);
                        if (oldCount > 0)
                        {
                            ssize_t newCount = oldCount > max ? oldCount - max : 0;
                            if (m_count.compare_exchange_strong(oldCount, newCount, std::memory_order_acquire, std::memory_order_relaxed))
                                return oldCount - newCount;
                        }
                        std::atomic_signal_fence(std::memory_order_acquire);
                    }
                    oldCount = m_count.fetch_sub(1, std::memory_order_acquire);
                    if (oldCount <= 0)
                    {
                        if ((timeout_usecs == 0) || (timeout_usecs < 0 && !m_sema.wait()) || (timeout_usecs > 0 && !m_sema.timed_wait((std::uint64_t)timeout_usecs)))
                        {
                            while (true)
                            {
                                oldCount = m_count.load(std::memory_order_acquire);
                                if (oldCount >= 0 && m_sema.try_wait())
                                    break;
                                if (oldCount < 0 && m_count.compare_exchange_strong(oldCount, oldCount + 1, std::memory_order_relaxed, std::memory_order_relaxed))
                                    return 0;
                            }
                        }
                    }
                    if (max > 1)
                        return 1 + try_wait_n(max - 1);
                    return 1;
                }

            public:
                thread_lw_semaphore_n(ssize_t initialCount = 0, int maxSpins = 10000) : m_count(initialCount), m_maxSpins(maxSpins)
                {
                    PMC_ASSERT(initialCount >= 0);
                    PMC_ASSERT(maxSpins >= 0);
                }

                bool try_wait()
                {
                    ssize_t oldCount = m_count.load(std::memory_order_relaxed);
                    while (oldCount > 0)
                    {
                        if (m_count.compare_exchange_weak(oldCount, oldCount - 1, std::memory_order_acquire, std::memory_order_relaxed))
                            return true;
                    }
                    return false;
                }

                bool wait()
                {
                    return try_wait() || _wait_with_spin();
                }

                bool wait(std::int64_t timeout_milli)
                {
                    return try_wait() || _wait_with_spin(timeout_milli);
                }

                // Acquires between 0 and (greedily) max, inclusive
                ssize_t try_wait_n(ssize_t max)
                {
                    PMC_ASSERT(max >= 0);
                    ssize_t oldCount = m_count.load(std::memory_order_relaxed);
                    while (oldCount > 0)
                    {
                        ssize_t newCount = oldCount > max ? oldCount - max : 0;
                        if (m_count.compare_exchange_weak(oldCount, newCount, std::memory_order_acquire, std::memory_order_relaxed))
                            return oldCount - newCount;
                    }
                    return 0;
                }

                // Acquires at least one, and (greedily) at most max
                ssize_t timed_wait_n(ssize_t max, std::int64_t timeout_usecs)
                {
                    PMC_ASSERT(max >= 0);
                    ssize_t result = try_wait_n(max);
                    if (result == 0 && max > 0)
                        result = _wait_n_with_spin(max, timeout_usecs);
                    return result;
                }

                ssize_t wait_n(ssize_t max)
                {
                    ssize_t result = timed_wait_n(max, -1);
                    PMC_ASSERT(result > 0);
                    return result;
                }

                void notify(ssize_t count = 1)
                {
                    PMC_ASSERT(count >= 0);
                    ssize_t oldCount = m_count.fetch_add(count, std::memory_order_release);
                    ssize_t toRelease = -oldCount < count ? -oldCount : count;
                    if (toRelease > 0)
                    {
                        m_sema.notify((int)toRelease);
                    }
                }

                std::size_t available_estimate() const
                {
                    ssize_t count = m_count.load(std::memory_order_relaxed);
                    return count > 0 ? static_cast<std::size_t>(count) : 0;
                }
            };
        }
    }
}


