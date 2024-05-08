#ifndef __pilo_platform_hpp_
#define __pilo_platform_hpp_

#define PMI_OS_UNSUPPORT	(0)
#define PMI_OS_WINDOWS		(1)
#define PMI_OS_LINUX		(2)
#define PMI_OS_APPLE		(3)

#define PMS_CODE_PAGE_LINUX_UNICODE_NAME "UCS-4LE"
#define PMS_CODE_PAGE_MACOS_UNICODE_NAME "UCS-4LE"
#define PMS_CODE_PAGE_WINDOWS_UNICODE_NAME "UCS-2LE"

#if defined(__INTEL_COMPILER)
#	define PMI_PFD_COMPILER_INTEL 1

#elif defined(_MSC_VER)
// Win32 API
#	define PMI_PFD_COMPILER_MSVC 1
#	define PMI_PFD_TARGET_WIN32 1
#	define PMI_PFD_LONG_SIZE 4
#	ifndef PMI_PFD_HAS_STDINT
#		if _MSC_VER >= 1600
#			define PMI_PFD_HAS_STDINT 1
#		endif
#	endif

#	if defined(_M_X64)
	// x64
#		define PMI_PFD_CPU_X64 1
#		define PMI_PFD_PTR_SIZE 8
#	elif defined(_M_IX86)
	// x86
#		define PMI_PFD_CPU_X86 1
#		define PMI_PFD_PTR_SIZE 4
#	else
#		error "Unrecognized platform!"
#	endif

#elif defined(__GNUC__)
// GCC compiler family
#	define PMI_PFD_COMPILER_GCC 1
#	ifndef _HAS_STDINT
#		define PMI_PFD_HAS_STDINT 1
#	endif
#	if defined(__APPLE__)
#		define PMI_PFD_TARGET_APPLE 1
#		define PMI_PFD_TARGET_POSIX 1
#		include <TargetConditionals.h>
#		if TARGET_IPHONE_SIMULATOR || TARGET_OS_IPHONE
#			define PMI_PFD_TARGET_IOS 1
#		endif
#	endif
#	if defined(__FreeBSD__)
#		define PMI_PFD_TARGET_POSIX 1
#		define PMI_PFD_KERNEL_FREEBSD 1
#	endif
#	if defined(__linux__)
#		define PMI_PFD_TARGET_POSIX 1
#		define PMI_PFD_KERNEL_LINUX 1
#	endif
#	if defined(__MACH__)
	// Mach kernel, eg. Apple MacOS/iOS
#		define PMI_PFD_KERNEL_MACH 1
#	endif

#	if defined(__MINGW32__) || defined(__MINGW64__)
	// Leave TURF_TARGET_WIN32 undefined because too much API is missing from MinGW's Win32 implementation
#		define PMI_PFD_TARGET_MINGW 1
#		define PMI_PFD_TARGET_POSIX 1
#	endif
#	if defined(__x86_64__)
	// x64
#		define PMI_PFD_CPU_X64 1
#		define PMI_PFD_PTR_SIZE 8
#	elif defined(__i386__)
	// x86
#		define PMI_PFD_CPU_X86 1
#		define PMI_PFD_PTR_SIZE 4
#	elif defined(__arm__)
	// ARM
#		define PMI_PFD_CPU_ARM 1
#		define PMI_PFD_PTR_SIZE 4
#		if defined(__ARM_ARCH_7__) || defined(__ARM_ARCH_7A__) || defined(__ARM_ARCH_7EM__) || defined(__ARM_ARCH_7R__) || defined(__ARM_ARCH_7M__) || defined(__ARM_ARCH_7S__)
	// ARMv7
#			define PMI_PFD_CPU_ARM_VERSION 7
#		elif defined(__ARM_ARCH_6__) || defined(__ARM_ARCH_6J__) || defined(__ARM_ARCH_6K__) || defined(__ARM_ARCH_6T2__) || defined(__ARM_ARCH_6Z__) || defined(__ARM_ARCH_6ZK__)
	// ARMv6
#			define PMI_PFD_CPU_ARM_VERSION 6
#		else
	// Could support earlier ARM versions at some point using compiler barriers and swp instruction
#			error "Unrecognized ARM CPU architecture version!"
#		endif
#		if defined(__thumb__)
	// Thumb instruction set mode
#			define PMI_PFD_CPU_ARM_THUMB 1
#		endif
#	elif defined(__arm64__)
	// ARM64
#		define PMI_PFD_CPU_ARM64 1
#		define PMI_PFD_PTR_SIZE 8

#		if	defined(__ARM64_ARCH_8__)
	// ARMv8
#			define PMI_PFD_CPU_ARM_VERSION 8
#		else
#			error "Unrecognized ARM64 CPU architecture version!"
#		endif

#	elif defined(__powerpc__) || defined(__POWERPC__) || defined(__PPC__)
#		define PMI_PFD_CPU_POWERPC 1
#		if defined(__powerpc64__)
#			define PMI_PFD_PTR_SIZE 8
#		else
#			define PMI_PFD_PTR_SIZE 4 // 32-bit architecture
#		endif
#	else
#		error "Unrecognized target CPU!"
#	endif

#	define PMI_PFD_LONG_SIZE PMI_PFD_PTR_SIZE

#else
#error "Unrecognized compiler!"
#endif


#include <stddef.h>
#if PMI_PFD_HAS_STDINT
#   include <stdint.h>
#else
typedef signed char int8_t;
typedef unsigned char uint8_t;
#define INT8_MIN (-0x7f - 1)
#define INT8_MAX 0x7f
#define UINT8_MAX 0xff

typedef short int16_t;
typedef unsigned short uint16_t;
#define INT16_MIN (-0x7fff - 1)
#define INT16_MAX 0x7fff
#define UINT16_MAX 0xffff

typedef int int32_t;
typedef unsigned int uint32_t;
#define INT32_MIN (-0x7fffffff - 1)
#define INT32_MAX 0x7fffffff
#define UINT32_MAX 0xffffffff

typedef __int64 int64_t;
typedef unsigned __int64 uint64_t;
#define INT64_MIN (-0x7fffffffffffffff - 1)
#define INT64_MAX 0x7fffffffffffffff
#define UINT64_MAX 0xffffffffffffffffu
#endif

#if defined(_WIN32) || defined(_WIN64)
#	ifndef WINDOWS
#		define WINDOWS
#	endif
#	define	PMI_OS_TYPE	PMI_OS_WINDOWS
#   define  PMS_OS_UNICODE_NAME PMS_CODE_PAGE_WINDOWS_UNICODE_NAME
#	define  PMS_PFD_ATOMIC_API_H	"atomic_capi_msvc.h"

#elif __linux__
#	define	PMI_OS_TYPE	PMI_OS_LINUX
#   define  PMS_OS_UNICODE_NAME PMS_CODE_PAGE_LINUX_UNICODE_NAME
#	define  PMS_PFD_ATOMIC_API_H	"atomic_capi_gcc_x86-64.h"

#elif __APPLE__
#	define	PMI_OS_TYPE	PMI_OS_APPLE
#   define  PMS_OS_UNICODE_NAME PMS_CODE_PAGE_MACOS_UNICODE_NAME
#	define  PMS_PFD_ATOMIC_API_H	"atomic_capi_gcc_x86-64.h"

#else
#	define	PMI_OS_TYPE	PMI_OS_UNSUPPORT
#	error "Unknown compiler"

#endif


#if defined(WIN32) || defined(WINDOWS)
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif
#endif

#if defined(_M_IA64) || defined(__ia64__)
#	define PMD_ARCH_IA64
#	define PMS_ARCH			"ia64"
#	define PMI_BITS			(64)

#elif defined(_WIN64) || defined(__amd64__) || defined(_M_X64) || defined(__x86_64__)
#	define PMD_ARCH_X64
#	define PMS_ARCH			"x86_64"
#	define PMI_BITS			(64)

#elif defined(_M_IX86) || defined(__i386__)
#	define PMD_ARCH_X86
#	define PMS_ARCH			"i386"
#	define PMI_BITS			(32)


#elif defined(_M_PPC) || defined(__powerpc__)
#	define PMD_ARCH_PPC
#	define PMS_ARCH			"ppc"
#	define PMI_BITS			(64)

#elif defined(_M_ARM64)
#	define PMD_ARCH_ARM64
#	define PMS_ARCH			"arm64"
#	define PMI_BITS			(64)

#else
#	    error "Unknown Arch"
#endif


#ifndef BYTE_ORDER
#	if (BSD >= 199103)
#		include <machine/endian.h>
#	else
#		if defined(linux) || defined(__linux__)
#			include <endian.h>
#		else
#			define LITTLE_ENDIAN   1234    /* least-significant byte first (vax, pc) */
#			define BIG_ENDIAN  4321    /* most-significant byte first (IBM, net) */
#			define PDP_ENDIAN  3412    /* LSB first in word, MSW first in long (pdp)*/

#			if defined(__i386__) || defined(__x86_64__) || defined(__amd64__) || \
				defined(vax) || defined(ns32000) || defined(sun386) || \
				defined(MIPSEL) || defined(_MIPSEL) || defined(BIT_ZERO_ON_RIGHT) || \
				defined(__alpha__) || defined(__alpha)
#				define BYTE_ORDER    LITTLE_ENDIAN
#			endif

#			if defined(sel) || defined(pyr) || defined(mc68000) || defined(sparc) || \
			defined(is68k) || defined(tahoe) || defined(ibm032) || defined(ibm370) || \
			defined(MIPSEB) || defined(_MIPSEB) || defined(_IBMR2) || defined(DGUX) ||\
			defined(apollo) || defined(__convex__) || defined(_CRAY) || \
			defined(__hppa) || defined(__hp9000) || \
			defined(__hp9000s300) || defined(__hp9000s700) || \
			defined (BIT_ZERO_ON_LEFT) || defined(m68k) || defined(__sparc)
#			define BYTE_ORDER  BIG_ENDIAN
#			endif
#		endif /* linux */
#	endif /* BSD */
#endif /* BYTE_ORDER */

#ifndef BYTE_ORDER
#	ifdef __BYTE_ORDER
#		if defined(__LITTLE_ENDIAN) && defined(__BIG_ENDIAN)
#			ifndef LITTLE_ENDIAN
#				define LITTLE_ENDIAN __LITTLE_ENDIAN
#			endif
#			ifndef BIG_ENDIAN
#				define BIG_ENDIAN __BIG_ENDIAN
#			endif
#			if (__BYTE_ORDER == __LITTLE_ENDIAN)
#				define BYTE_ORDER LITTLE_ENDIAN
#			else
#				define BYTE_ORDER BIG_ENDIAN
#			endif
#		endif
#	endif
#endif



enum endianness_enum : uint32_t {
	eLittleEndian = 0x00000001,
	eBigEndian = 0x01000000,
	ePDPEndian = 0x00010000,
	eUnkownEndian = 0xFFFFFFFF
};
/**
 * A simple function that can be used to help determine a program's endianness
 * at compile-time.
 */
constexpr endianness_enum getEndianOrder()
{
	return
		((1 & 0xFFFFFFFF) == eLittleEndian)
		? eLittleEndian
		: ((1 & 0xFFFFFFFF) == eBigEndian)
		? eBigEndian
		: ((1 & 0xFFFFFFFF) == ePDPEndian)
		? ePDPEndian
		: eUnkownEndian;
}

#define PMC_ENDIANNESS getEndianOrder()

#ifndef PMI_PFD_CACHE_LINE_SIZE
#	define PMI_PFD_CACHE_LINE_SIZE 64
#endif

#ifndef PMD_PFD_EXCEPTIONS_ENABLED
#   if (defined(_MSC_VER) && defined(_CPPUNWIND)) || (defined(__GNUC__) && defined(__EXCEPTIONS)) || (!defined(_MSC_VER) && !defined(__GNUC__))
#       define PMD_PFD_EXCEPTIONS_ENABLED
#   endif
#endif

#ifndef PMI_PFD_HAS_EMPLACE
#	if !defined(_MSC_VER) || _MSC_VER >= 1800 // variadic templates: either a non-MS compiler or VS >= 2013
#		define PMI_PFD_HAS_EMPLACE    1
#	endif
#endif

#ifndef PMD_PFD_MAYBE_ALIGN_TO_CACHELINE
#	if defined (__APPLE__) && defined (__MACH__) && __cplusplus >= 201703L
	// This is required to find out what deployment target we are using
#		include <CoreFoundation/CoreFoundation.h>
#		if !defined(MAC_OS_X_VERSION_MIN_REQUIRED) || MAC_OS_X_VERSION_MIN_REQUIRED < MAC_OS_X_VERSION_10_14
			// C++17 new(size_t, align_val_t) is not backwards-compatible with older versions of macOS, so we can't support over-alignment in this case
#			define PMD_PFD_MAYBE_ALIGN_TO_CACHELINE
#		endif
#	endif
#endif

#if (PMI_PFD_COMPILER_MSVC == 1) || (PMI_PFD_COMPILER_INTEL == 1)
#	define PMF_ALIGN(x) __declspec(align(x))
#elif PMI_PFD_COMPILER_GCC == 1
#	define PMF_ALIGN(x) __attribute__((aligned(x)))
#else
// Assume GCC compliant syntax...
#	define PMF_ALIGN(x) __attribute__((aligned(x)))
#endif

#ifndef PMD_PFD_MAYBE_ALIGN_TO_CACHELINE
#define PMD_PFD_MAYBE_ALIGN_TO_CACHELINE PMF_ALIGN(PMI_PFD_CACHE_LINE_SIZE)
#endif


#ifdef __cplusplus
#define PMC_INLINE inline
#else
#define PMC_INLINE __inline
#endif

#endif //__pilo_platform_hpp_