#pragma once

#include "lang_feature.hpp"


#if defined(WINDOWS)
template <typename T>
inline void piloUnused(T &x) { (void)x; }
#   define M_UNUSED(x) piloUnused(x);
#   define MF_COUNT_OF _countof

#else
#   define M_UNUSED(x) (void)x;
#   define MF_COUNT_OF(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#endif


#ifdef WINDOWS
#   pragma warning(disable:4127)
#endif

#define M_DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);


#if defined(WIN32) || defined(WINDOWS)
typedef int	socklen_t;

#define M_PATH_SEP_S  "\\"
#define M_PATH_SEP_C '\\'
#else

#define M_PATH_SEP_S "/"
#define M_PATH_SEP_C '/'
#endif