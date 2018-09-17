#pragma once

#if defined(_WIN32) || defined(_WIN64)
#	ifndef WINDOWS
#		define WINDOWS
#	endif
#endif

#ifdef WINDOWS
#   if defined(_WIN64)
#	    ifndef __x86_64__
#           define __x86_64__
#	    endif
#   define __BITS__    64

#   else
#       undef __x86_64__
#       ifndef __x86__
#           define __x86__
#       endif
#   define __BITS__    32
#   endif 

#else
#   ifdef _LINUX
#       if defined(__x86_64__)
#           define __BITS__ 64

#       else
#           define __BITS__ 32

#       endif //__x86_64__
#   endif // _LINUX 
#endif // WINDOWS



#if defined(WIN32) || defined(WINDOWS)
#	ifndef WIN32_LEAN_AND_MEAN
#		define WIN32_LEAN_AND_MEAN
#	endif

#endif

#include <cstdio>
#include <cstdlib>

#ifdef WINDOWS
#if defined(WIN32)
#   include <intrin.h>
#else
#endif

#   include <windows.h>
#   include <limits.h>
#   include <wchar.h>
#   include <tchar.h>
#   include <mbstring.h>
#   include <new.h>
#   include <malloc.h>
#   include <Strsafe.h>
#   include <process.h>

#else

#   include <unistd.h>
#   include <limits.h>
#   include <ctype.h>
#   include <wctype.h>
#   include <stdarg.h>
#   include <wchar.h>
#   include <new>
#   include <errno.h>
#   include <pthread.h>
#   include <fcntl.h>

#endif
