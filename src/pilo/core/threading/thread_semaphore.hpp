#pragma once
#include "../../pilo.hpp"
// Portable single-producer, single-consumer semaphore below:

#ifdef PMI_PFD_COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable: 4365)		// Disable erroneous 'conversion from long to unsigned int, signed/unsigned mismatch' error when using `assert`
#ifdef __cplusplus_cli
#pragma managed(push, off)
#endif
#endif

#if defined(WINDOWS)
// Avoid including windows.h in a header; we only need a handful of
// items, so we'll redeclare them here (this is relatively safe since
// the API generally has to remain stable between Windows versions).
// I know this is an ugly hack but it still beats polluting the global
// namespace with thousands of generic names or adding a .cpp for nothing.
extern "C" {
    struct _SECURITY_ATTRIBUTES;
    __declspec(dllimport) void* __stdcall CreateSemaphoreW(_SECURITY_ATTRIBUTES* lpSemaphoreAttributes, long lInitialCount, long lMaximumCount, const wchar_t* lpName);
    __declspec(dllimport) int __stdcall CloseHandle(void* hObject);
    __declspec(dllimport) unsigned long __stdcall WaitForSingleObject(void* hHandle, unsigned long dwMilliseconds);
    __declspec(dllimport) int __stdcall ReleaseSemaphore(void* hSemaphore, long lReleaseCount, long* lpPreviousCount);
}
#elif defined(__MACH__)
#include <mach/mach.h>
#elif defined(FREERTOS)
#include <FreeRTOS.h>
#include <semphr.h>
#include <task.h>
#else
#include <semaphore.h>
#endif


namespace pilo
{
    namespace core
    {
        namespace threading
        {
#if defined(WINDOWS)
            class thread_semaphore
            {
            private:
                void* m_hSema;

                thread_semaphore(const thread_semaphore& other);
                thread_semaphore& operator=(const thread_semaphore& other);

            public:
                PMC_NO_TSAN thread_semaphore(int initialCount = 0) : m_hSema()
                {
                    PMC_ASSERT(initialCount >= 0);
                    const long maxLong = 0x7fffffff;
                    m_hSema = CreateSemaphoreW(nullptr, initialCount, maxLong, nullptr);
                    PMC_ASSERT(m_hSema);
                }

                PMC_NO_TSAN ~thread_semaphore()
                {
                    CloseHandle(m_hSema);
                }

                bool wait() PMC_NO_TSAN
                {
                    const unsigned long infinite = 0xffffffff;
                    return WaitForSingleObject(m_hSema, infinite) == 0;
                }

                bool try_wait() PMC_NO_TSAN
                {
                    return WaitForSingleObject(m_hSema, 0) == 0;
                }

                bool timed_wait(::pilo::u64_t ms) PMC_NO_TSAN
                {
                    DWORD to = INFINITE;
                    if (ms >= 0)
                        to = (DWORD)ms;
                    return WaitForSingleObject(m_hSema, (unsigned long)(ms)) == 0;
                }

                void notify() PMC_NO_TSAN
                {
                    while (!ReleaseSemaphore(m_hSema, 1, nullptr));
                }

                void notify(int count) PMC_NO_TSAN
                {
                    while (!ReleaseSemaphore(m_hSema, count, nullptr));
                }
            };
#elif defined(__MACH__)
            //---------------------------------------------------------
            // Semaphore (Apple iOS and OSX)
            // Can't use POSIX semaphores due to http://lists.apple.com/archives/darwin-kernel/2009/Apr/msg00010.html
            //---------------------------------------------------------
            class thread_semaphore
            {
            private:
                semaphore_t m_sema;

                thread_semaphore(const thread_semaphore& other);
                thread_semaphore& operator=(const thread_semaphore& other);

            public:
                PMC_NO_TSAN thread_semaphore(int initialCount = 0) : m_sema()
                {
                    assert(initialCount >= 0);
                    kern_return_t rc = semaphore_create(mach_task_self(), &m_sema, SYNC_POLICY_FIFO, initialCount);
                    assert(rc == KERN_SUCCESS);
                    M_UNUSED(rc);
                }

                PMC_NO_TSAN ~thread_semaphore()
                {
                    semaphore_destroy(mach_task_self(), m_sema);
                }

                bool wait() PMC_NO_TSAN
                {
                    return semaphore_wait(m_sema) == KERN_SUCCESS;
                }

                bool try_wait() PMC_NO_TSAN
                {
                    return timed_wait(0);
                }

                bool timed_wait(::pilo::i64_t ms) PMC_NO_TSAN
                {
                    mach_timespec_t ts;
                    ts.tv_sec = static_cast<unsigned int>(ms / 1000);
                    ts.tv_nsec = static_cast<int>((ms % 1000) * 1000000);

                    // added in OSX 10.10: https://developer.apple.com/library/prerelease/mac/documentation/General/Reference/APIDiffsMacOSX10_10SeedDiff/modules/Darwin.html
                    kern_return_t rc = semaphore_timedwait(m_sema, ts);
                    return rc == KERN_SUCCESS;
                }

                void notify() PMC_NO_TSAN
                {
                    while (semaphore_signal(m_sema) != KERN_SUCCESS);
                }

                void notify(int count) PMC_NO_TSAN
                {
                    while (count-- > 0)
                    {
                        while (semaphore_signal(m_sema) != KERN_SUCCESS);
                    }
                }
            };

#elif defined(FREERTOS)
            //---------------------------------------------------------
            // Semaphore (FreeRTOS)
            //---------------------------------------------------------
            class thread_semaphore
            {
            private:
                SemaphoreHandle_t m_sema;

                thread_semaphore(const thread_semaphore& other);
                thread_semaphore& operator=(const thread_semaphore& other);

            public:
                PMC_NO_TSAN thread_semaphore(int initialCount = 0) : m_sema()
                {
                    assert(initialCount >= 0);
                    m_sema = xSemaphoreCreateCounting(static_cast<UBaseType_t>(~0ull), static_cast<UBaseType_t>(initialCount));
                    assert(m_sema);
                }

                PMC_NO_TSAN ~thread_semaphore()
                {
                    vSemaphoreDelete(m_sema);
                }

                bool wait() PMC_NO_TSAN
                {
                    return xSemaphoreTake(m_sema, portMAX_DELAY) == pdTRUE;
                }

                bool try_wait() PMC_NO_TSAN
                {
                    return xSemaphoreTake(m_sema, 0) == pdTRUE;
                }

                bool timed_wait(s::pilo::i64_t msecs) PMC_NO_TSAN
                {
                   // std::uint64_t msecs = usecs / 1000;
                    TickType_t ticks = static_cast<TickType_t>(msecs / portTICK_PERIOD_MS);
                    return xSemaphoreTake(m_sema, ticks) == pdTRUE;
                }

                void notify() PMC_NO_TSAN
                {
                    BaseType_t rc = xSemaphoreGive(m_sema);
                    assert(rc == pdTRUE);
                    M_UNUSED(rc);
                }

                void notify(int count) PMC_NO_TSAN
                {
                    while (count-- > 0)
                        notify();
                }
            };
#else
            //---------------------------------------------------------
            // Semaphore (POSIX, Linux)
            //---------------------------------------------------------
            class thread_semaphore
            {
            private:
                sem_t m_sema;

                thread_semaphore(const thread_semaphore& other);
                thread_semaphore& operator=(const thread_semaphore& other);

            public:
                PMC_NO_TSAN thread_semaphore(int initialCount = 0) : m_sema()
                {
                    PMC_ASSERT(initialCount >= 0);
                    int rc = sem_init(&m_sema, 0, static_cast<unsigned int>(initialCount));
                    PMC_ASSERT(rc == 0);
                    PMC_UNUSED(rc);
                }

                PMC_NO_TSAN ~thread_semaphore()
                {
                    sem_destroy(&m_sema);
                }

                bool wait() PMC_NO_TSAN
                {
                    // http://stackoverflow.com/questions/2013181/gdb-causes-sem-wait-to-fail-with-eintr-error
                    int rc;
                    do
                    {
                        rc = sem_wait(&m_sema);
                    } while (rc == -1 && errno == EINTR);
                    return rc == 0;
                }

                bool try_wait() PMC_NO_TSAN
                {
                    int rc;
                    do {
                        rc = sem_trywait(&m_sema);
                    } while (rc == -1 && errno == EINTR);
                    return rc == 0;
                }

                bool timed_wait(::pilo::i64_t ms) PMC_NO_TSAN
                {
                    struct timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    ts.tv_sec += static_cast<time_t>(ms / 1000);
                    ts.tv_nsec += static_cast<long>(ms % 1000) * 1000000;
                    // sem_timedwait bombs if you have more than 1e9 in tv_nsec
                    // so we have to clean things up before passing it in
                    if (ts.tv_nsec >= 1000000000) {
                        ts.tv_nsec -= 1000000000;
                        ++ts.tv_sec;
                    }

                    int rc;
                    do {
                        rc = sem_timedwait(&m_sema, &ts);
                    } while (rc == -1 && errno == EINTR);
                    return rc == 0;
                }

                void notify() PMC_NO_TSAN
                {
                    while (sem_post(&m_sema) == -1);
                }

                void notify(int count) PMC_NO_TSAN
                {
                    while (count-- > 0)
                    {
                        while (sem_post(&m_sema) == -1);
                    }
                }
            };
#endif            
        }
    }
}

#if defined(PMI_PFD_COMPILER_MSVC) && (_MSC_VER < 1700 || defined(__cplusplus_cli))
#pragma warning(pop)
#ifdef __cplusplus_cli
#pragma managed(pop)
#endif
#endif
