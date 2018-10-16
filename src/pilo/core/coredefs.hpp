#pragma once
#include "../compat.hpp"
#include "../types.hpp"
#include "../error.hpp"
#include "parameter.hpp"
/*
* MB  - Macros which act as a bitwise flag.
* MP  - Macros which act as a Procedure,no value is returned
* MF - Macros which act as a Function
* MC  - Macros which defined as constant value
* MS  - Macros which defined as a control switch
*/

#  ifdef PILO_NO_INCLASS_MEMBER_INITIALIZATION
#       define PILO_STATIC_CONSTANT(type, assignment) enum { assignment }
#  else
#     define PILO_STATIC_CONSTANT(type, assignment) static const type assignment
#  endif

#define MF_ALIGN_SIZE(n) ((n+sizeof(void*)-1) & (~(sizeof(void*)-1)))
#define MP_SAFE_FREE(a)            do {if ((a)!= (0)) {free(a); (a)=0;}} while(0)
#define MP_SAFE_DELETE(a)          do {if ((a)!= (0)) {delete(a); (a)=0;}} while(0)
#define MP_SAFE_DELETE_CAST(t,a)          do {if ((a)!= ((void*)0)) {delete((t*) (a)); (a)=(void*)0;}} while(0)
#define MP_SAFE_DELETE_ARRAY(a)    do {if ((a)!= ((void*)0)) {delete[] (a); (a)=(void*)0;}} while(0)
#define MP_SAFE_DELETE_ARRAY_CAST(t,a)  do {if ((a)!= ((void*)0)) {delete[] (t*)(a); (a)=(void*)0;}} while(0)

#define MP_CHECK_EMPTY_CSTR_RET(s, e)    do { if (((s) == nullptr) || ((*s) == 0)) { return e; }} while (0)
#define MP_CHECK_EMPTY_CSTR_RET_VOID(s)    do { if (((s) == nullptr) || ((*s) == 0)) { return ; }} while (0)
#define MP_CHECK_EQUAL_VALUE_RET(v, c, e)    do { if ((v) == (c)) { return e; }} while (0)
#define MP_CHECK_NULL_PTR_RET(p, e)  do { if ((p) == nullptr) { return e; }} while (0)
#define MP_CHECK_NULL_PTR_RET_VOID(p)  do { if ((p) == nullptr) { return; }} while (0)

#if defined(WIN32) || defined(WINDOWS)
#   define THREAD_ROUTINE_CALL     __stdcall

#if !defined(MC_INVALID_FILE_DESCRIPTOR)
#   define MC_INVALID_FILE_DESCRIPTOR   INVALID_HANDLE_VALUE
#endif

#else
#   define THREAD_ROUTINE_CALL

#if !defined(MC_INVALID_FILE_DESCRIPTOR)
#   define MC_INVALID_FILE_DESCRIPTOR (-1)
#endif

#endif

/* MC_INVALID_SIZE */
#if !defined(MC_INVALID_SIZE)
#   define MC_INVALID_SIZE ((size_t) (~0))
#endif

#if !defined(MC_INVALID_SIZE_32)
#   define MC_INVALID_SIZE_32 ((unsigned int) (~0))
#endif

#if !defined(MC_INVALID_PTR)
#   define MC_INVALID_PTR (void*)(~0)
#endif

#if !defined(MC_INVALID_SOCK_FD)
#   define MC_INVALID_SOCK_FD (os_socket_fd_t)(~0)
#endif

#if !defined(MC_INVALID_PROCESS_ID)
#   define MC_INVALID_PROCESS_ID ((os_process_id_t)~0)
#endif

#if !defined(MC_INVALID_DLL_MODULE_HANDLE)
#   define MC_INVALID_DLL_MODULE_HANDLE ((os_dll_module_handle_t) NULL)
#endif


#define M_DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);

#define MP_CHECK_RANGE_OO_RETURN(v,l,u,ret)   \
    {\
        if (((v) <= (l)) || ((v) >= (u))) return ret; \
    }
#define MP_CHECK_RANGE_OC_RETURN(v,l,u,ret)   \
    {\
        if (((v) <= (l)) || ((v) > (u))) return ret; \
    }
#define MP_CHECK_RANGE_CO_RETURN(v,l,u,ret)   \
    {\
        if (((v) < (l)) || ((v) >= (u))) return ret; \
    }
#define MP_CHECK_RANGE_CC_RETURN(v,l,u,ret)   \
    {\
        if (((v)<(l)) || ((v)>(u))) return ret; \
    }

#define MP_CHECK_RANGE_OO_VOID(v,l,u)   \
    {\
        if (((v) <= (l)) || ((v) >= (u))) return; \
    }
#define MP_CHECK_RANGE_OC_VOID(v,l,u)   \
    {\
        if (((v) <= (l)) || ((v) > (u))) return; \
    }
#define MP_CHECK_RANGE_CO_VOID(v,l,u)   \
    {\
        if (((v) < (l)) || ((v) >= (u))) return; \
    }
#define MP_CHECK_RANGE_CC_VOID(v,l,u)   \
    {\
        if (((v)<(l)) || ((v)>(u))) return; \
    }


#if defined(WIN32) || defined(WINDOWS)
#   define _USER_BREAK_	__debugbreak() //{ do { __asm { int 3 }; } while (0); }

#else
#   define _USER_BREAK_ { do { asm ( "int $3;" ); } while(0); }

#endif

#if defined(NOASSERT)
#   define ASSERT_I(cond) \
        if (!(cond)) \
        { \
            _log_(LOG_FATAL, "ASSERT: %s:%u <%s>", __FILE__, __LINE__, #cond); \
        }

#   define ASSERT_I_2(cond,rst) \
        if ((cond)) \
        { \
            rst = TRUE; \
        } \
        else \
        { \
            rst = FALSE; \
            _log_(LOG_FATAL, "ASSERT: %s:%u <%s>", __FILE__, __LINE__, #cond); \
        }

#else
#   define ASSERT_I(cond) \
        { \
            if (!(cond)) { _USER_BREAK_ } \
        }

#   define ASSERT_I_2(cond,rst) \
        { \
            if ((cond)) \
            { \
                rst = TRUE; \
            } \
            else \
            { \
            rst = FALSE; \
            _USER_BREAK_ \
            } \
        }
#endif

#   define ASSERT_RETURN(cond, ret) \
        { \
            BOOL _assert_b_; \
            ASSERT_I_2(cond, _assert_b_); \
            if (!_assert_b_) return ret; \
        }

#   define ASSERT_RETURN_VOID(cond) \
        { \
            BOOL _assert_b_; \
            ASSERT_I_2(cond, _assert_b_); \
            if (!_assert_b_) return; \
        }

#   define DEBUG_BREAK		_USER_BREAK_
#   define RELEASE_BREAK	_USER_BREAK_

#define M_ASSERT(expression) \
            { do { if (!(expression)) { RELEASE_BREAK; } } while (0); }

#define Assert ASSERT
#define aSsErT ASSERT

#define M_MAX_SIZE(a,b) ((a)>(b)?(a):(b))



#define M_STRUCT_ADDRESS_BY_MEMBER(ptr, TYPE, member) \
(TYPE*)((unsigned char*)(ptr)-(size_t)&(((TYPE*)0)->member))

template <class T> const T& pilo_min(const T& t1, const T& t2) { return t1 < t2 ? t1 : t2; }
template <class T> const T& pilo_max(const T& t1, const T& t2) { return t1 < t2 ? t2 : t1; }


#define M_ALIGN_SIZE(size, align) ((size + align - 1) & (~(align - 1)))

#define M_LO16BIT(_dw)           ((::pilo::u16_t)(((::pilo::u32_t)(_dw)) & 0xffff))
#define M_HI16BIT(_dw)           ((::pilo::u16_t)((((::pilo::u32_t)(_dw)) >> 16) & 0xffff))
#define M_LO32BIT(_qw)           ((::pilo::u32_t)(_qw))
#define M_HI32BIT(_qw)           ((::pilo::u32_t)(((_qw) >> 32) & 0xffffffff))
