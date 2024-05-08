#pragma once
#include "platform.hpp"

#ifdef WINDOWS
#if defined(WIN32)
#   include <intrin.h>
#else
#endif

#   include <windows.h>
#   include <wchar.h>
#   include <tchar.h>
#   include <mbstring.h>
#   include <new.h>
#   include <malloc.h>
#   include <Strsafe.h>
#   include <process.h>
#   include <WinSock2.h>


#else

#   include <unistd.h>
#   include <ctype.h>
#   include <wctype.h>
#   include <stdarg.h>
#   include <wchar.h>
#   include <errno.h>
#   include <pthread.h>
#   include <fcntl.h>
#   include <limits.h>
#   include <arpa/inet.h>

#if defined __linux__
#   include <sys/syscall.h>
#   endif

#endif

#include <float.h>