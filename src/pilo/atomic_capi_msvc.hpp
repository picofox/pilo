#pragma once

#include "./platform.hpp"
#if (PMI_PFD_COMPILER_MSVC == 1)

#ifdef __cplusplus
extern "C" {
#endif

#if PMI_PFD_TARGET_XBOX_360 // Xbox 360
    void _ReadWriteBarrier();
#pragma intrinsic(_ReadWriteBarrier)
#endif

    //-------------------------------------
    //  Atomic types
    //-------------------------------------
    // In MSVC, correct alignment of each type is already ensured.
    // MSVC doesn't seem subject to out-of-thin-air stores like GCC, so volatile is
    // omitted.
    // (MS volatile implies acquire & release semantics, which may be expensive on
    // ARM or Xbox 360.)
    typedef struct { uint8_t nonatomic; } pilo_atomic8_t;
    typedef struct { uint16_t nonatomic; } pilo_atomic16_t;
    typedef struct { uint32_t nonatomic; } pilo_atomic32_t;
    typedef struct { uint64_t nonatomic; } pilo_atomic64_t;
    typedef struct { void* nonatomic; } pilo_atomic_ptr_t;

    //-------------------------------------
    //  Fences
    //-------------------------------------
#define pilo_signal_fence_consume() (0)
#define pilo_signal_fence_acquire() _ReadWriteBarrier()
#define pilo_signal_fence_release() _ReadWriteBarrier()
#define pilo_signal_fence_seq_cst() _ReadWriteBarrier()

#if PMI_PFD_TARGET_XBOX_360 // Xbox 360
// According to
// http://msdn.microsoft.com/en-us/library/windows/desktop/ee418650.aspx,
// __lwsync() also acts a compiler barrier, unlike MemoryBarrier() on X360.
// Hoping __sync() acts as a compiler barrier too.
#   define pilo_thread_fence_consume() (0)
#   define pilo_thread_fence_acquire() __lwsync()
#   define pilo_thread_fence_release() __lwsync()
#   define pilo_thread_fence_seq_cst() __sync()
#else // Windows
#   define pilo_thread_fence_consume() (0)
#   define pilo_thread_fence_acquire() _ReadWriteBarrier()
#   define pilo_thread_fence_release() _ReadWriteBarrier()
#   define pilo_thread_fence_seq_cst() MemoryBarrier()
#endif

//----------------------------------------------
//  8-bit atomic operations
//----------------------------------------------
    PMC_INLINE uint8_t pilo_atomic_load_8_relaxed(const pilo_atomic8_t* object) {
        // Do a volatile load so that compiler doesn't duplicate loads, which makes
        // them nonatomic. (Happened in testing.)
        return ((volatile pilo_atomic8_t*)object)->nonatomic;
    }
    PMC_INLINE uint8_t pilo_atomic_load_8_relaxed_(const int8_t* object) {
        // Do a volatile load so that compiler doesn't duplicate loads, which makes
        // them nonatomic. (Happened in testing.)
        return (* (volatile char*) object);
    }

    PMC_INLINE void pilo_atomic_store_8_relaxed(pilo_atomic8_t* object, uint8_t value) {
        ((volatile pilo_atomic8_t*)object)->nonatomic = value;
    }
    PMC_INLINE void pilo_atomic_store_8_relaxed_(int8_t* object, int8_t value) {
        *(volatile char*)object = value;
    }

    PMC_INLINE uint8_t pilo_atomic_compare_exchange_8_relaxed(pilo_atomic8_t* object, uint8_t expected, uint8_t desired) {
        return _InterlockedCompareExchange8((char*)object, desired, expected);
    }
    PMC_INLINE int8_t pilo_atomic_compare_exchange_8_relaxed_(int8_t* object, int8_t expected, int8_t desired) {
        return _InterlockedCompareExchange8((char*)object, desired, expected);
    }

    PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_8_relaxed(pilo_atomic8_t* object, uint8_t* expected, uint8_t desired) {
        uint8_t e = *expected;
        uint8_t previous = _InterlockedCompareExchange8((char*)object, desired, e);
        intptr_t matched = (previous == e);
        if (!matched)
            *expected = previous;
        return matched;
    }
    PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_8_relaxed_(int8_t* object, int8_t* expected, int8_t desired) {
        uint8_t e = *expected;
        uint8_t previous = _InterlockedCompareExchange8((char*)object, desired, e);
        intptr_t matched = (previous == e);
        if (!matched)
            *expected = previous;
        return matched;
    }

    PMC_INLINE uint8_t pilo_atomic_exchange_8_relaxed(pilo_atomic8_t* object, uint8_t desired) {
        return _InterlockedExchange8((char*)object, desired);
    }
    PMC_INLINE int8_t pilo_atomic_exchange_8_relaxed_(int8_t* object, int8_t desired) {
        return _InterlockedExchange8((char*)object, desired);
    }

    PMC_INLINE uint8_t pilo_atomic_fetch_add_8_relaxed(pilo_atomic8_t* object, int8_t operand) {
        return _InterlockedExchangeAdd8((char*)object, operand);
    }
    PMC_INLINE int8_t pilo_atomic_fetch_add_8_relaxed_(int8_t* object, int8_t operand) {
        return _InterlockedExchangeAdd8((char*)object, operand);
    }

    PMC_INLINE uint8_t pilo_atomic_fetch_and_8_relaxed(pilo_atomic8_t* object, uint8_t operand) {
        return _InterlockedAnd8((char*)object, operand);
    }
    PMC_INLINE int8_t pilo_atomic_fetch_and_8_relaxed_(int8_t* object, int8_t operand) {
        return _InterlockedAnd8((char*)object, operand);
    }

    PMC_INLINE uint8_t pilo_atomic_fetch_or_8_relaxed(pilo_atomic8_t* object, uint8_t operand) {
        return _InterlockedOr8((char*)object, operand);
    }
    PMC_INLINE int8_t pilo_atomic_fetch_or_8_relaxed_(int8_t* object, int8_t operand) {
        return _InterlockedOr8((char*)object, operand);
    }

    //----------------------------------------------
    //  16-bit atomic operations
    //----------------------------------------------
    PMC_INLINE uint16_t pilo_atomic_load_16_relaxed(const pilo_atomic16_t* object) {
        // Do a volatile load so that compiler doesn't duplicate loads, which makes
        // them nonatomic. (Happened in testing.)
        return ((volatile pilo_atomic16_t*)object)->nonatomic;
    }
    PMC_INLINE int16_t pilo_atomic_load_16_relaxed_(const int16_t* object) {
        // Do a volatile load so that compiler doesn't duplicate loads, which makes
        // them nonatomic. (Happened in testing.)
        return (*(volatile int16_t*)object);
    }

    PMC_INLINE void pilo_atomic_store_16_relaxed(pilo_atomic16_t* object, uint16_t value) {
        ((volatile pilo_atomic16_t*)object)->nonatomic = value;
    }
    PMC_INLINE void pilo_atomic_store_16_relaxed_(int16_t* object, int16_t value) {
        *object = value;
    }

    PMC_INLINE uint16_t pilo_atomic_compare_exchange_16_relaxed(pilo_atomic16_t* object, uint16_t expected, uint16_t desired) {
        return _InterlockedCompareExchange16((short*)object, desired, expected);
    }
    PMC_INLINE int16_t pilo_atomic_compare_exchange_16_relaxed_(int16_t* object, int16_t expected, int16_t desired) {
        return _InterlockedCompareExchange16(object, desired, expected);
    }

    PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_16_relaxed(pilo_atomic16_t* object, uint16_t* expected, uint16_t desired) {
        uint16_t e = *expected;
        uint16_t previous = _InterlockedCompareExchange16((short*)object, desired, e);
        intptr_t matched = (previous == e);
        if (!matched)
            *expected = previous;
        return matched;
    }
    PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_16_relaxed_(int16_t* object, int16_t* expected, int16_t desired) {
        int16_t e = *expected;
        int16_t previous = _InterlockedCompareExchange16(object, desired, e);
        intptr_t matched = (previous == e);
        if (!matched)
            *expected = previous;
        return matched;
    }

    PMC_INLINE uint16_t pilo_atomic_exchange_16_relaxed(pilo_atomic16_t* object, uint16_t desired) {
        return _InterlockedExchange16((short*)object, desired);
    }
    PMC_INLINE int16_t pilo_atomic_exchange_16_relaxed_(int16_t* object, int16_t desired) {
        return _InterlockedExchange16(object, desired);
    }

    PMC_INLINE uint16_t pilo_atomic_fetch_add_16_relaxed(pilo_atomic16_t* object, int16_t operand) {
        return _InterlockedExchangeAdd16((short*)object, operand);
    }
    PMC_INLINE int16_t pilo_atomic_fetch_add_16_relaxed_(int16_t* object, int16_t operand) {
        return _InterlockedExchangeAdd16(object, operand);
    }

    PMC_INLINE uint16_t pilo_atomic_fetch_and_16_relaxed(pilo_atomic16_t* object, uint16_t operand) {
        return _InterlockedAnd16((short*)object, operand);
    }
    PMC_INLINE int16_t pilo_atomic_fetch_and_16_relaxed_(int16_t* object, int16_t operand) {
        return _InterlockedAnd16(object, operand);
    }

    PMC_INLINE uint16_t pilo_atomic_fetch_or_16_relaxed(pilo_atomic16_t* object, uint16_t operand) {
        return _InterlockedOr16((short*)object, operand);
    }
    PMC_INLINE int16_t pilo_atomic_fetch_or_16_relaxed_(int16_t* object, int16_t operand) {
        return _InterlockedOr16(object, operand);
    }

    //----------------------------------------------
    //  32-bit atomic operations
    //----------------------------------------------
    PMC_INLINE uint32_t pilo_atomic_load_32_relaxed(const pilo_atomic32_t* object) {
        // Do a volatile load so that compiler doesn't duplicate loads, which makes
        // them nonatomic. (Happened in testing.)
        return ((volatile pilo_atomic32_t*)object)->nonatomic;
    }
    PMC_INLINE int32_t pilo_atomic_load_32_relaxed_(const int32_t* object) {
        return (*(volatile int32_t*)object);
    }

    PMC_INLINE void pilo_atomic_store_32_relaxed(pilo_atomic32_t* object, uint32_t value) {
        ((volatile pilo_atomic32_t*)object)->nonatomic = value;
    }
    PMC_INLINE void pilo_atomic_store_32_relaxed_(int32_t* object, int32_t value) {
        *(volatile int32_t*)object = value;
    }

    PMC_INLINE uint32_t pilo_atomic_compare_exchange_32_relaxed(pilo_atomic32_t* object, uint32_t expected, uint32_t desired) {
        return _InterlockedCompareExchange((long*)object, desired, expected);
    }
    PMC_INLINE int32_t pilo_atomic_compare_exchange_32_relaxed_(int32_t* object, int32_t expected, int32_t desired) {
        return _InterlockedCompareExchange((long*)object, desired, expected);
    }

    PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_32_relaxed(pilo_atomic32_t* object, uint32_t* expected, uint32_t desired) {
        uint32_t e = *expected;
        uint32_t previous = _InterlockedCompareExchange((long*)object, desired, e);
        intptr_t matched = (previous == e);
        if (!matched)
            *expected = previous;
        return matched;
    }
    PMC_INLINE intptr_t pilo_atomic_compare_exchange_weak_32_relaxed_(int32_t* object, int32_t* expected, int32_t desired) {
        int32_t e = *expected;
        int32_t previous = _InterlockedCompareExchange((long*)object, desired, e);
        intptr_t matched = (previous == e);
        if (!matched)
            *expected = previous;
        return matched;
    }

    PMC_INLINE uint32_t pilo_atomic_exchange_32_relaxed(pilo_atomic32_t* object, uint32_t desired) {
        return _InterlockedExchange((long*)object, desired);
    }
    PMC_INLINE int32_t pilo_atomic_exchange_32_relaxed_(int32_t* object, int32_t desired) {
        return _InterlockedExchange((long*)object, desired);
    }

    PMC_INLINE uint32_t pilo_atomic_fetch_add_32_relaxed(pilo_atomic32_t* object, int32_t operand) {
        return _InterlockedExchangeAdd((long*)object, operand);
    }
    PMC_INLINE int32_t pilo_atomic_fetch_add_32_relaxed_(int32_t* object, int32_t operand) {
        return _InterlockedExchangeAdd((long*)object, operand);
    }

    PMC_INLINE uint32_t pilo_atomic_fetch_and_32_relaxed(pilo_atomic32_t* object, uint32_t operand) {
        return _InterlockedAnd((long*)object, operand);
    }
    PMC_INLINE int32_t pilo_atomic_fetch_and_32_relaxed_(int32_t* object, int32_t operand) {
        return _InterlockedAnd((long*)object, operand);
    }

    PMC_INLINE uint32_t pilo_atomic_fetch_or_32_relaxed(pilo_atomic32_t* object, uint32_t operand) {
        return _InterlockedOr((long*)object, operand);
    }
    PMC_INLINE int32_t pilo_atomic_fetch_or_32_relaxed_(int32_t* object, int32_t operand) {
        return _InterlockedOr((long*)object, operand);
    }

    //----------------------------------------------
    //  64-bit atomic operations
    //----------------------------------------------
    PMC_INLINE uint64_t pilo_atomic_load_64_relaxed(const pilo_atomic64_t* object) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        // Do a volatile load so that compiler doesn't duplicate loads, which makes
        // them nonatomic. (Happened in testing.)
        return ((volatile pilo_atomic64_t*)object)->nonatomic;
#else
        // On 32-bit x86, the most compatible way to get an atomic 64-bit load is with
        // cmpxchg8b.
        // This essentially performs pilo_compare_exchange_64_relaxed(object,
        // _dummyValue, _dummyValue).
        uint64_t result;
        __asm {
            mov esi, object;
            mov ebx, eax;
            mov ecx, edx;
            lock cmpxchg8b[esi];
            mov dword ptr result, eax;
            mov dword ptr result[4], edx;
        }
        return result;
#endif
    }
    PMC_INLINE int64_t pilo_atomic_load_64_relaxed_(const int64_t* object) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        // Do a volatile load so that compiler doesn't duplicate loads, which makes
        // them nonatomic. (Happened in testing.)
        return *((volatile int64_t*)object);
#else
        // On 32-bit x86, the most compatible way to get an atomic 64-bit load is with
        // cmpxchg8b.
        // This essentially performs pilo_compare_exchange_64_relaxed(object,
        // _dummyValue, _dummyValue).
        uint64_t result;
        __asm {
            mov esi, object;
            mov ebx, eax;
            mov ecx, edx;
            lock cmpxchg8b[esi];
            mov dword ptr result, eax;
            mov dword ptr result[4], edx;
        }
        return result;
#endif
    }

    PMC_INLINE void pilo_atomic_store_64_relaxed(pilo_atomic64_t* object, uint64_t value) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        ((volatile pilo_atomic64_t*)object)->nonatomic = value;
#else
        // On 32-bit x86, the most compatible way to get an atomic 64-bit store is
        // with cmpxchg8b.
        // Essentially, we perform pilo_compapre_exchange_64_relasex(object,
        // object->nonatomic, desired)
        // in a loop until it returns the previous value.
        // According to the Linux kernel (atomic64_cx8_32.S), we don't need the
        // "lock;" prefix
        // on cmpxchg8b since aligned 64-bit writes are already atomic on 586 and
        // newer.
        __asm {
            mov esi, object;
            mov ebx, dword ptr value;
            mov ecx, dword ptr value[4];
        retry:
            cmpxchg8b[esi];
            jne retry;
        }
#endif
    }
    PMC_INLINE void pilo_atomic_store_64_relaxed_(int64_t* object, int64_t value) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        (*(volatile int64_t*)object) = value;
#else
        // On 32-bit x86, the most compatible way to get an atomic 64-bit store is
        // with cmpxchg8b.
        // Essentially, we perform pilo_compapre_exchange_64_relasex(object,
        // object->nonatomic, desired)
        // in a loop until it returns the previous value.
        // According to the Linux kernel (atomic64_cx8_32.S), we don't need the
        // "lock;" prefix
        // on cmpxchg8b since aligned 64-bit writes are already atomic on 586 and
        // newer.
        __asm {
            mov esi, object;
            mov ebx, dword ptr value;
            mov ecx, dword ptr value[4];
        retry:
            cmpxchg8b[esi];
            jne retry;
        }
#endif
    }

    PMC_INLINE uint64_t pilo_atomic_compare_exchange_64_relaxed(pilo_atomic64_t* object, uint64_t expected, uint64_t desired) {
        // Or make 2 versions of function? (Only if there's an advantage for GCC.)
        return _InterlockedCompareExchange64((LONGLONG*)object, desired, expected);
    }
    PMC_INLINE int64_t pilo_atomic_compare_exchange_64_relaxed_(int64_t* object, int64_t expected, int64_t desired) {
        // Or make 2 versions of function? (Only if there's an advantage for GCC.)
        return _InterlockedCompareExchange64(object, desired, expected);
    }

    PMC_INLINE intptr_t pilo_compare_exchange_weak_64_relaxed(pilo_atomic64_t* object, uint64_t* expected, uint64_t desired) {
        uint64_t e = *expected;
        uint64_t previous = _InterlockedCompareExchange64((LONGLONG*)object, desired, e);
        intptr_t matched = (previous == e);
        if (!matched)
            *expected = previous;
        return matched;
    }
    PMC_INLINE intptr_t pilo_compare_exchange_weak_64_relaxed_(int64_t* object, int64_t* expected, int64_t desired) {
        int64_t e = *expected;
        int64_t previous = _InterlockedCompareExchange64(object, desired, e);
        intptr_t matched = (previous == e);
        if (!matched)
            *expected = previous;
        return matched;
    }

    PMC_INLINE uint64_t pilo_atomic_exchange_64_relaxed(pilo_atomic64_t* object, uint64_t desired) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        return _InterlockedExchange64((LONGLONG*)object, desired);
#else
        // It would be cool to check the zero flag, which is set by lock cmpxchg8b, to
        // know whether the CAS succeeded,
        // but that would require an __asm block, which forces us to move the result
        // to a stack variable.
        // Let's just re-compare the result with the previous instead.
        uint64_t expected = object->nonatomic;
        for (;;) {
            uint64_t previous = _InterlockedCompareExchange64((LONGLONG*)object, desired, expected);
            if (previous == expected)
                return previous;
            expected = previous;
        }
#endif
    }
    PMC_INLINE int64_t pilo_atomic_exchange_64_relaxed_(int64_t* object, int64_t desired) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        return _InterlockedExchange64(object, desired);
#else
        int64_t expected = object->nonatomic;
        for (;;) {
            int64_t previous = _InterlockedCompareExchange64(object, desired, expected);
            if (previous == expected)
                return previous;
            expected = previous;
    }
#endif
    }

    PMC_INLINE uint64_t pilo_atomic_fetch_add_64_relaxed(pilo_atomic64_t* object, int64_t operand) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        return _InterlockedExchangeAdd64((LONGLONG*)object, operand);
#else
        uint64_t expected = object->nonatomic;
        for (;;) {
            uint64_t previous = _InterlockedCompareExchange64((LONGLONG*)object, expected + operand, expected);
            if (previous == expected)
                return previous;
            expected = previous;
        }
#endif
    }
    PMC_INLINE int64_t pilo_atomic_fetch_add_64_relaxed_(int64_t* object, int64_t operand) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        return _InterlockedExchangeAdd64(object, operand);
#else
        int64_t expected = object->nonatomic;
        for (;;) {
            int64_t previous = _InterlockedCompareExchange64(object, expected + operand, expected);
            if (previous == expected)
                return previous;
            expected = previous;
        }
#endif
    }

    PMC_INLINE uint64_t pilo_atomic_fetch_and_64_relaxed(pilo_atomic64_t* object, uint64_t operand) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        return _InterlockedAnd64((LONGLONG*)object, operand);
#else
        uint64_t expected = object->nonatomic;
        for (;;) {
            uint64_t previous = _InterlockedCompareExchange64((LONGLONG*)object, expected & operand, expected);
            if (previous == expected)
                return previous;
            expected = previous;
        }
#endif
    }
    PMC_INLINE int64_t pilo_atomic_fetch_and_64_relaxed_(int64_t* object, int64_t operand) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        return _InterlockedAnd64(object, operand);
#else
        int64_t expected = object->nonatomic;
        for (;;) {
            int64_t previous = _InterlockedCompareExchange64(object, expected & operand, expected);
            if (previous == expected)
                return previous;
            expected = previous;
        }
#endif
    }

    PMC_INLINE uint64_t pilo_atomic_fetch_or_64_relaxed(pilo_atomic64_t* object, uint64_t operand) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        return _InterlockedOr64((LONGLONG*)object, operand);
#else
        uint64_t expected = object->nonatomic;
        for (;;) {
            uint64_t previous = _InterlockedCompareExchange64((LONGLONG*)object, expected | operand, expected);
            if (previous == expected)
                return previous;
            expected = previous;
        }
#endif
    }
    PMC_INLINE int64_t pilo_atomic_fetch_or_64_relaxed_(int64_t* object, int64_t operand) {
#if (PMI_PFD_PTR_SIZE == 8) || PMI_PFD_TARGET_XBOX_360
        return _InterlockedOr64(object, operand);
#else
        int64_t expected = object->nonatomic;
        for (;;) {
            int64_t previous = _InterlockedCompareExchange64(object, expected | operand, expected);
            if (previous == expected)
                return previous;
            expected = previous;
        }
#endif
    }

#ifdef __cplusplus
} // extern "C"
#endif


#endif
