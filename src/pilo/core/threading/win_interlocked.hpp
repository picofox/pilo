#ifdef WINDOWS
# pragma once

#undef PILO_USE_WINDOWS_H

#if (_MSC_VER >= 1600)
#define PILO_USE_WINDOWS_H
#endif

#define INTEL_WIN

#if defined( PILO_USE_WINDOWS_H ) //0
#   include <windows.h>
#   define PILO_WIN_INTERLOCKED_INCREMENT InterlockedIncrement
#   define PILO_WIN_INTERLOCKED_DECREMENT InterlockedDecrement
#   define PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE InterlockedCompareExchange
#   define PILO_WIN_INTERLOCKED_EXCHANGE InterlockedExchange
#   define PILO_WIN_INTERLOCKED_EXCHANGE_ADD InterlockedExchangeAdd
#   define PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE_POINTER InterlockedCompareExchangePointer
#   define PILO_WIN_INTERLOCKED_EXCHANGE_POINTER InterlockedExchangePointer

#elif defined( _MSC_VER ) || defined( INTEL_WIN ) //1
#   if defined( _MSC_VER ) && _MSC_VER >= 1600
#       include <intrin.h>

#   elif defined( __CLRCALL_PURE_OR_CDECL )
        extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedIncrement( long volatile * );
        extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedDecrement( long volatile * );
        extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedCompareExchange( long volatile *, long, long );
        extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedExchange( long volatile *, long );
        extern "C" long __CLRCALL_PURE_OR_CDECL _InterlockedExchangeAdd( long volatile *, long );
#   else
        extern "C" long __cdecl _InterlockedIncrement( long volatile * );
        extern "C" long __cdecl _InterlockedDecrement( long volatile * );
        extern "C" long __cdecl _InterlockedCompareExchange( long volatile *, long, long );
        extern "C" long __cdecl _InterlockedExchange( long volatile *, long );
        extern "C" long __cdecl _InterlockedExchangeAdd( long volatile *, long );
#   endif

#   pragma intrinsic( _InterlockedIncrement )
#   pragma intrinsic( _InterlockedDecrement )
#   pragma intrinsic( _InterlockedCompareExchange )
#   pragma intrinsic( _InterlockedExchange )
#   pragma intrinsic( _InterlockedExchangeAdd )

#   if defined(_M_IA64) || defined(_M_AMD64)
        extern "C" void* __cdecl _InterlockedCompareExchangePointer( void* volatile *, void*, void* );
        extern "C" void* __cdecl _InterlockedExchangePointer( void* volatile *, void* );
#       pragma intrinsic( _InterlockedCompareExchangePointer )
#       pragma intrinsic( _InterlockedExchangePointer )
#       define PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE_POINTER _InterlockedCompareExchangePointer
#       define PILO_WIN_INTERLOCKED_EXCHANGE_POINTER _InterlockedExchangePointer

#   else
#       define PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE_POINTER(dest,exchange,compare) \
            ((void*)PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE((long volatile*)(dest),(long)(exchange),(long)(compare)))
#       define PILO_WIN_INTERLOCKED_EXCHANGE_POINTER(dest,exchange) \
            ((void*)PILO_WIN_INTERLOCKED_EXCHANGE((long volatile*)(dest),(long)(exchange)))

#   endif

#   define PILO_WIN_INTERLOCKED_INCREMENT _InterlockedIncrement
#   define PILO_WIN_INTERLOCKED_DECREMENT _InterlockedDecrement
#   define PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE _InterlockedCompareExchange
#   define PILO_WIN_INTERLOCKED_EXCHANGE _InterlockedExchange
#   define PILO_WIN_INTERLOCKED_EXCHANGE_ADD _InterlockedExchangeAdd

#elif defined( WIN32 ) || defined( _WIN32 ) || defined( __WIN32__ ) || defined( __CYGWIN__ ) //0
#   if defined(__MINGW64__)
#       define PILO_WIN_INTERLOCKED_IMPORT
#   else
#       define PILO_WIN_INTERLOCKED_IMPORT __declspec(dllimport)
#   endif


    namespace pilo
    {
        namespace win
        {
            extern "C" PILO_WIN_INTERLOCKED_IMPORT long __stdcall InterlockedIncrement( long volatile * );
            extern "C" PILO_WIN_INTERLOCKED_IMPORT long __stdcall InterlockedDecrement( long volatile * );
            extern "C" PILO_WIN_INTERLOCKED_IMPORT long __stdcall InterlockedCompareExchange( long volatile *, long, long );
            extern "C" PILO_WIN_INTERLOCKED_IMPORT long __stdcall InterlockedExchange( long volatile *, long );
            extern "C" PILO_WIN_INTERLOCKED_IMPORT long __stdcall InterlockedExchangeAdd( long volatile *, long );

            # if defined(_M_IA64) || defined(_M_AMD64)
            extern "C" PILO_WIN_INTERLOCKED_IMPORT void* __stdcall InterlockedCompareExchangePointer( void* volatile *, void*, void* );
            extern "C" PILO_WIN_INTERLOCKED_IMPORT void* __stdcall InterlockedExchangePointer( void* volatile *, void* );
            # endif

        } // namespace win

    } // namespace pilo

#   define PILO_WIN_INTERLOCKED_INCREMENT ::pilo::win::InterlockedIncrement
#   define PILO_WIN_INTERLOCKED_DECREMENT ::pilo::win::InterlockedDecrement
#   define PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE ::pilo::win::InterlockedCompareExchange
#   define PILO_WIN_INTERLOCKED_EXCHANGE ::pilo::win::InterlockedExchange
#   define PILO_WIN_INTERLOCKED_EXCHANGE_ADD ::pilo::win::InterlockedExchangeAdd

#   if defined(_M_IA64) || defined(_M_AMD64)
#       define PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE_POINTER ::pilo::win::InterlockedCompareExchangePointer
#       define PILO_WIN_INTERLOCKED_EXCHANGE_POINTER ::pilo::win::InterlockedExchangePointer
#   else
#       define PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE_POINTER(dest,exchange,compare) \
            ((void*)PILO_WIN_INTERLOCKED_COMPARE_EXCHANGE((long volatile*)(dest),(long)(exchange),(long)(compare)))
#       define PILO_WIN_INTERLOCKED_EXCHANGE_POINTER(dest,exchange) \
            ((void*)PILO_WIN_INTERLOCKED_EXCHANGE((long volatile*)(dest),(long)(exchange)))
#   endif

#else

# error "Interlocked intrinsics not available"

#endif

#endif //WINDOWS