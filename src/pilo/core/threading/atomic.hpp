﻿#pragma once
#include "../../pilo.hpp"

#if (PMI_PFD_COMPILER_MSVC == 0) || (_MSC_VER >= 1700 && !defined(__cplusplus_cli))
#   define PMD_USE_STD_ATOMIC_FOR_WEAK_ATOMIC
#endif

#ifdef PMD_USE_STD_ATOMIC_FOR_WEAK_ATOMIC
#   include <atomic>
#endif
#include <utility>


#ifdef __cplusplus 
extern "C" {
#endif



#   ifdef WINDOWS
    // Since VS2005 volatile read/writes always release
    inline long pilo_atomic_read_acquire_32(long volatile* x) PML_NOEXCEPT
    {
        long const res = *x;
        return res;
    }
    inline void* pilo_atomic_read_acquire_ptr(void* volatile* x) PML_NOEXCEPT
    {
        void* const res = *x;
        return res;
    }
    inline void pilo_atomic_write_release_32(long volatile* x, long value) PML_NOEXCEPT
    {
        *x = value;
    }
    inline void pilo_atomic_write_release_ptr(void* volatile* x, void* value) PML_NOEXCEPT
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
#       define  PMF_ATOMIC_FETCH_ADD_32(pa,d)        InterlockedExchangeAdd((LONG *)(pa),(LONG)(d))
#       define  PMF_ATOMIC_FETCH_ADD_64(pa,d)        InterlockedExchangeAdd64((LONGLONG *)(pa),(LONGLONG)(d))

        /***
        * Sets a 32-bit variable to the specified value as an atomic operation.
        * @param pv[in,out] A pointer to the value to be exchanged. The function sets this variable to Value, and returns its prior
        * @param nv[in]      The value to be exchanged with the value pointed to by Target.
        * @return:          The function returns the initial value of the Target parameter.
        * @note:            The variables for these functions must be aligned on a 32-bit / 64-bit boundary
        */
#       define  PMF_ATOMIC_SET_32(pv, nv)                 InterlockedExchange((LONG*)(pv),(LONG)(nv))
#       define  PMF_ATOMIC_SET_64(pv,nv)                  InterlockedExchange64((LONGLONG*)(pv),(LONGLONG)(nv))

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
#       define  PILO_ATOMIC_COMPARE_SET_32(pv,cv,nv)       InterlockedCompareExchange((LONG*)(pv),(LONG)(nv),(LONG)(cv))
#       define  PILO_ATOMIC_COMPARE_SET_64(pv,cv,nv)       InterlockedCompareExchange64((LONGLONG*) pv,(LONGLONG)nv,  (LONGLONG)cv)
        
#       if defined(_WIN64)

            /***
            * Performs an atomic AND operation on the specified char/short/long/longlong values.
            * @param d[in,out]  A pointer to the first operand. This value will be replaced with the result of the operation.
            * @param v[in]      The second operand.
            * @return:          The function returns the original value of the Destination parameter.
            * @note:            The variables for these functions must be aligned on a 32-bit / 64-bit boundary
            */
#           define  PILO_ATOMIC_FETCH_BITAND_8(d,v)            InterlockedAnd8((char*)(d),(char)(v))
#           define  PILO_ATOMIC_FETCH_BITAND_16(d,v)           InterlockedAnd16((SHORT*)(d),(SHORT)(v))
#           define  PILO_ATOMIC_FETCH_BITAND_32(d,v)           InterlockedAnd((LONG*)(d),(LONG)(v))
#           define  PILO_ATOMIC_FETCH_BITAND_64(d,v)           InterlockedAnd64((LONGLONG*)(d),(LONGLONG)(v))

            /***
            * Performs an atomic XOR operation on the specified char/short/long/longlong values.
            * @param d[in,out]  A pointer to the first operand. This value will be replaced with the result of the operation.
            * @param v[in]      The second operand.
            * @return:          The function returns the original value of the Destination parameter.
            * @note:            The variables for these functions must be aligned on a 32-bit / 64-bit boundary
            */
#           define  PILO_ATOMIC_FETCH_BITXOR_8(d,v)            InterlockedXor8((char*)(d),(char)(v))
#           define  PILO_ATOMIC_FETCH_BITXOR_16(d,v)           InterlockedXor16((SHORT*)(d),(SHORT)(v))
#           define  PILO_ATOMIC_FETCH_BITXOR_32(d,v)           InterlockedXor((LONG*)(d),(LONG)(v))
#           define  PILO_ATOMIC_FETCH_BITXOR_64(d,v)           InterlockedXor64((LONGLONG*)(d),(LONGLONG)(v))

            /***
            * Performs an atomic OR operation on the specified char/short/long/longlong values.
            * @param d[in,out]  A pointer to the first operand. This value will be replaced with the result of the operation.
            * @param v[in]      The second operand.
            * @return:          The function returns the original value of the Destination parameter.
            * @note:            The variables for these functions must be aligned on a 32-bit / 64-bit boundary
            */
#           define  PILO_ATOMIC_FETCH_BITOR_8(d,v)             InterlockedOr8((char*)(d),(char)(v))
#           define  PILO_ATOMIC_FETCH_BITOR_16(d,v)            InterlockedOr16((SHORT*)(d),(SHORT)(v))     
#           define  PILO_ATOMIC_FETCH_BITOR_32(d,v)            InterlockedOr((LONG*)(d),(LONG)(v))
#           define  PILO_ATOMIC_FETCH_BITOR_64(d,v)            InterlockedOr64((LONGLONG*)(d),(LONGLONG)(v))

#       else
#           define  PILO_ATOMIC_FETCH_BITAND_8(d,v)            _InterlockedAnd8((char*)(d),(char)(v))
#           define  PILO_ATOMIC_FETCH_BITAND_16(d,v)           _InterlockedAnd16((SHORT*)(d),(SHORT)(v))
#           define  PILO_ATOMIC_FETCH_BITAND_32(d,v)           _InterlockedAnd((LONG*)(d),(LONG)(v))
#           define  PILO_ATOMIC_FETCH_BITAND_64(d,v)           _InterlockedXor64((char*)(d),(char)(v))

#           define  PILO_ATOMIC_FETCH_BITXOR_8(d,v)            _InterlockedXor8((char*)(d),(char)(v))
#           define  PILO_ATOMIC_FETCH_BITXOR_16(d,v)           _InterlockedXor16((SHORT*)(d),(SHORT)(v))
#           define  PILO_ATOMIC_FETCH_BITXOR_32(d,v)           _InterlockedXor((LONG*)(d),(LONG)(v))
#           define  PILO_ATOMIC_FETCH_BITXOR_64(d,v)           _InterlockedXor64((LONGLONG*)(d),(LONGLONG)(v))

#           define  PILO_ATOMIC_FETCH_BITOR_8(d,v)             _InterlockedOr8((char*)(d),(char)(v))
#           define  PILO_ATOMIC_FETCH_BITOR_16(d,v)             _InterlockedOr16((SHORT*)(d),(SHORT)(v))     
#           define  PILO_ATOMIC_FETCH_BITOR_32(d,v)             _InterlockedOr((LONG*)(d),(LONG)(v))
#           define  PILO_ATOMIC_FETCH_BITOR_64(d,v)             _InterlockedOr64((LONGLONG*)(d),(LONGLONG)(v))
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
#       define  PILO_ATOMIC_PTR_SET(t,v)                       PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE_POINTER(t,v)



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
#       define  PILO_ATOMIC_PTR_COMPARE_SET(d,c,n)        InterlockedCompareExchangePointer((PVOID*)(d),(PVOID)(n),(PVOID)(c))

#   else
#       define PMF_ATOMIC_FETCH_ADD_32                      __sync_fetch_and_add
#       define PMF_ATOMIC_FETCH_ADD_64                      __sync_fetch_and_add
#       define PMF_ATOMIC_SET_32                            __sync_lock_test_and_set
#       define PMF_ATOMIC_SET_64                            __sync_lock_test_and_set
#       define PILO_ATOMIC_COMPARE_SET_32                   __sync_val_compare_and_swap
#       define PILO_ATOMIC_COMPARE_SET_64                   __sync_val_compare_and_swap
#       define PILO_ATOMIC_FETCH_BITAND_8                   __sync_fetch_and_and
#       define PILO_ATOMIC_FETCH_BITAND_16                  __sync_fetch_and_and
#       define PILO_ATOMIC_FETCH_BITAND_32                  __sync_fetch_and_and
#       define PILO_ATOMIC_FETCH_BITAND_64                  __sync_fetch_and_and
#       define PILO_ATOMIC_FETCH_BITXOR_8                   __sync_fetch_and_xor
#       define PILO_ATOMIC_FETCH_BITXOR_16                  __sync_fetch_and_xor
#       define PILO_ATOMIC_FETCH_BITXOR_32                  __sync_fetch_and_xor
#       define PILO_ATOMIC_FETCH_BITXOR_64                  __sync_fetch_and_xor
#       define PILO_ATOMIC_FETCH_BITOR_8                    __sync_fetch_and_or
#       define PILO_ATOMIC_FETCH_BITOR_16                   __sync_fetch_and_or    
#       define PILO_ATOMIC_FETCH_BITOR_32                   __sync_fetch_and_or
#       define PILO_ATOMIC_FETCH_BITOR_64                   __sync_fetch_and_or

#       define  PILO_ATOMIC_PTR_SET(d,n)                    __sync_lock_test_and_set((PLONG)(d),(LONG)(n))
#       define  PILO_ATOMIC_PTR_COMPARE_SET(d,c,n)          __sync_val_compare_and_swap((PVOID*)(d),(PVOID)(c),(PVOID)(n))


#   endif


	

#ifdef __cplusplus 
    }
#endif
