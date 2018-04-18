#pragma once
#include "../../core/coredefs.hpp"

#ifdef WINDOWS
#include "win_interlocked.hpp"

inline long interlocked_read_acquire(long volatile* x)
{
    long const res = *x;
    return res;
}
inline void* interlocked_read_acquire(void* volatile* x)
{
    void* const res = *x;
    return res;
}

#endif


#ifdef __cplusplus 
extern "C" {
#endif

#   ifdef WINDOWS
    // Since VS2005 volatile read/writes always release
    inline long pilo_atomic_read_acquire_32(long volatile* x) M_NOEXCEPT
    {
        long const res = *x;
        return res;
    }
    inline void* pilo_atomic_read_acquire_ptr(void* volatile* x) M_NOEXCEPT
    {
        void* const res = *x;
        return res;
    }
    inline void pilo_atomic_write_release_32(long volatile* x, long value) M_NOEXCEPT
    {
        *x = value;
    }
    inline void pilo_atomic_write_release_ptr(void* volatile* x, void* value) M_NOEXCEPT
    {
        *x = value;
    }


        /***
        * Performs an atomic addition of two 32-bit / 64-bit values.
        * @param pa[in,out] A pointer to a variable. The value of this variable will be replaced with the result of the operation.
        * @param d[in]      The value to be added to the variable pointed to by the Addend parameter.
        * @return:          The value to be added to the variable pointed to by the Addend parameter.
        * @note:            The variables for these functions must be aligned on a 32-bit / 64-bit boundary
        */
#       define  pilo_atomic_fetch_then_add32(pa,d)        InterlockedExchangeAdd((LONG *)(pa),(LONG)(d))
#       define  pilo_atomic_fetch_then_add64(pa,d)        InterlockedExchangeAdd64((LONGLONG *)(pa),(LONGLONG)(d))

        /***
        * Sets a 32-bit variable to the specified value as an atomic operation.
        * @param pv[in,out] A pointer to the value to be exchanged. The function sets this variable to Value, and returns its prior
        * @param nv[in]      The value to be exchanged with the value pointed to by Target.
        * @return:          The function returns the initial value of the Target parameter.
        * @note:            The variables for these functions must be aligned on a 32-bit / 64-bit boundary
        */
#       define  pilo_atomic_set32(pv, nv)                 InterlockedExchange((LONG*)(pv),(LONG)(nv))
#       define  pilo_atomic_set64(pv,nv)                  InterlockedExchange64((LONGLONG*)(pv),(LONGLONG)(nv))

        /***
        * Performs an atomic compare-and-exchange operation on the specified values.
        * The function compares two specified 32-bit / 64-bit values and
        * exchanges with another 32-bit / 64-bit value based on the outcome of the comparison.
        * @param pv[in,out] A pointer to the destination value.
        * @param cv[in]     The value to compare to Destination.
        * @param nv[in]     The exchange value.
        * @return:          The function returns the initial value of the Destination parameter.
        * @note:            The parameters for this function must be aligned on a 32-bit boundary
        */
#       define  pilo_atomic_compare_then_set32(pv,cv,nv)       InterlockedCompareExchange((LONG*)(pv),(LONG)(nv),(LONG)(cv))
#       define  pilo_atomic_compare_then_set64(pv,cv,nv)       InterlockedCompareExchange64((LONGLONG*) pv,(LONGLONG)nv,  (LONGLONG)cv)
        
#       if defined(_WIN64)

            /***
            * Performs an atomic AND operation on the specified char/short/long/longlong values.
            * @param d[in,out]  A pointer to the first operand. This value will be replaced with the result of the operation.
            * @param v[in]      The second operand.
            * @return:          The function returns the original value of the Destination parameter.
            * @note:            The variables for these functions must be aligned on a 32-bit / 64-bit boundary
            */
#           define  pilo_atomic_fetch_then_bitand8(d,v)            InterlockedAnd8((char*)(d),(char)(v))
#           define  pilo_atomic_fetch_then_bitand16(d,v)           InterlockedAnd16((SHORT*)(d),(SHORT)(v))
#           define  pilo_atomic_fetch_then_bitand32(d,v)           InterlockedAnd((LONG*)(d),(LONG)(v))
#           define  pilo_atomic_fetch_then_bitand64(d,v)            InterlockedAnd64((LONGLONG*)(d),(LONGLONG)(v))

            /***
            * Performs an atomic XOR operation on the specified char/short/long/longlong values.
            * @param d[in,out]  A pointer to the first operand. This value will be replaced with the result of the operation.
            * @param v[in]      The second operand.
            * @return:          The function returns the original value of the Destination parameter.
            * @note:            The variables for these functions must be aligned on a 32-bit / 64-bit boundary
            */
#           define  pilo_atomic_fetch_then_bitxor8(d,v)            InterlockedXor8((char*)(d),(char)(v))
#           define  pilo_atomic_fetch_then_bitxor16(d,v)           InterlockedXor16((SHORT*)(d),(SHORT)(v))
#           define  pilo_atomic_fetch_then_bitxor32(d,v)           InterlockedXor((LONG*)(d),(LONG)(v))
#           define  pilo_atomic_fetch_then_bitxor64(d,v)           InterlockedXor64((LONGLONG*)(d),(LONGLONG)(v))

            /***
            * Performs an atomic OR operation on the specified char/short/long/longlong values.
            * @param d[in,out]  A pointer to the first operand. This value will be replaced with the result of the operation.
            * @param v[in]      The second operand.
            * @return:          The function returns the original value of the Destination parameter.
            * @note:            The variables for these functions must be aligned on a 32-bit / 64-bit boundary
            */
#           define  pilo_atomic_fetch_then_bitor8(d,v)             InterlockedOr8((char*)(d),(char)(v))
#           define  pilo_atomic_fetch_then_bitor16(d,v)            InterlockedOr16((SHORT*)(d),(SHORT)(v))     
#           define  pilo_atomic_fetch_then_bitor32(d,v)            InterlockedOr((LONG*)(d),(LONG)(v))
#           define  pilo_atomic_fetch_then_bitor64(d,v)            InterlockedOr64((LONGLONG*)(d),(LONGLONG)(v))

#       else
#           define  pilo_atomic_fetch_then_bitand8(d,v)            _InterlockedAnd8((char*)(d),(char)(v))
#           define  pilo_atomic_fetch_then_bitand16(d,v)           _InterlockedAnd16((SHORT*)(d),(SHORT)(v))
#           define  pilo_atomic_fetch_then_bitand32(d,v)           _InterlockedAnd((LONG*)(d),(LONG)(v))
#           define  pilo_atomic_fetch_then_bitand64(d,v)           _InterlockedXor64((char*)(d),(char)(v))

#           define  pilo_atomic_fetch_then_bitxor8(d,v)            _InterlockedXor8((char*)(d),(char)(v))
#           define  pilo_atomic_fetch_then_bitxor16(d,v)           _InterlockedXor16((SHORT*)(d),(SHORT)(v))
#           define  pilo_atomic_fetch_then_bitxor32(d,v)           _InterlockedXor((LONG*)(d),(LONG)(v))
#           define  pilo_atomic_fetch_then_bitxor64(d,v)           _InterlockedXor64((LONGLONG*)(d),(LONGLONG)(v))

#           define  pilo_atomic_fetch_then_bitor8(d,v)             _InterlockedOr8((char*)(d),(char)(v))
#           define  pilo_atomic_fetch_then_bitor16(d,v)             _InterlockedOr16((SHORT*)(d),(SHORT)(v))     
#           define  pilo_atomic_fetch_then_bitor32(d,v)             _InterlockedOr((LONG*)(d),(LONG)(v))
#           define  pilo_atomic_fetch_then_bitor64(d,v)             _InterlockedOr64((LONGLONG*)(d),(LONGLONG)(v))
#       endif

        /***
        * Atomically exchanges a pair of addresses.
        * @param t[in,out]      A pointer to the address to exchange. 
        *                       The function sets the address pointed to by the t parameter 
        *                       (*Target) to the address that is the value of the Value parameter, 
        *                       and returns the prior value of the Target parameter. 
        * @param v[in,out]         The address to be exchanged with the address pointed to by the t parameter (*t).
        * @return:              The function returns the initial address pointed to by the Target parameter.
        * @note:                On a 64-bit system, the parameters are 64 bits and the Target parameter must be 
        *                       aligned on 64-bit boundaries; otherwise, the function will behave unpredictably. 
        *                       On a 32-bit system, the parameters are 32 bits and the Target parameter must be 
        *                       aligned on 32-bit boundaries.
        */
#       define  pilo_atomic_ptr_set(t,v)                       PILO_INTERLOCKED_COMPARE_EXCHANGE_POINTER(t,v)



        /***
        * Performs an atomic compare-and-exchange operation on the specified values. 
        * The function compares two specified pointer values and exchanges 
        * with another pointer value based on the outcome of the comparison.
        * @param pv[in,out] A pointer to a pointer to the destination value.
        * @param cv[in]     The value to compare to Destination.
        * @param nv[in]     The exchange value.
        * @return:          The function returns the initial value of the pv parameter.
        * @note:            On a 64-bit system, the parameters are 64 bits and must be
        *                   aligned on 64-bit boundaries; otherwise, the function will 
        *                   behave unpredictably. On a 32-bit system, the parameters are
        *                   32 bits and must be aligned on 32-bit boundaries.
        */
#       define  pilo_atomic_ptr_compare_then_set(d,c,n)        InterlockedCompareExchangePointer((PVOID*)(d),(PVOID)(n),(PVOID)(c))

#   else
#       define pilo_atomic_fetch_then_add32                     __sync_fetch_and_add
#       define pilo_atomic_fetch_then_add32                     __sync_fetch_and_add
#       define pilo_atomic_set32                                __sync_lock_test_and_set
#       define pilo_atomic_set64                                __sync_lock_test_and_set
#       define pilo_atomic_compare_then_set32                   __sync_val_compare_and_swap
#       define pilo_atomic_compare_then_set64                   __sync_val_compare_and_swap
#       define pilo_atomic_fetch_then_bitand8                   __sync_fetch_and_and
#       define pilo_atomic_fetch_then_bitand16                  __sync_fetch_and_and
#       define pilo_atomic_fetch_then_bitand32                  __sync_fetch_and_and
#       define pilo_atomic_fetch_then_bitand64                  __sync_fetch_and_and
#       define pilo_atomic_fetch_then_bitxor8                   __sync_fetch_and_xor
#       define pilo_atomic_fetch_then_bitxor16                  __sync_fetch_and_xor
#       define pilo_atomic_fetch_then_bitxor32                  __sync_fetch_and_xor
#       define pilo_atomic_fetch_then_bitxor64                  __sync_fetch_and_xor
#       define pilo_atomic_fetch_then_bitor8                    __sync_fetch_and_or
#       define pilo_atomic_fetch_then_bitor16                   __sync_fetch_and_or    
#       define pilo_atomic_fetch_then_bitor32                   __sync_fetch_and_or
#       define pilo_atomic_fetch_then_bitor64                   __sync_fetch_and_or

#       define  pilo_atomic_ptr_set(d,n)                         __sync_lock_test_and_set((PLONG)(d),(LONG)(n))
#       define  pilo_atomic_ptr_compare_then_set(d,c,n)         __sync_val_compare_and_swap((PVOID*)(d),(PVOID)(c),(PVOID)(n))


#   endif


#ifdef __cplusplus 
    }
#endif