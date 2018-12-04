#pragma once

#include "lang_feature.hpp"


#if defined(WINDOWS)
#   define M_UNUSED(x) do { (void)(x); } while (0)
#   define MF_COUNT_OF _countof
#   define MC_MAP_FAILED_INDICATOR  (NULL)

#else
#   define M_UNUSED(x) (void)x
#   define MF_COUNT_OF(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#   define MC_MAP_FAILED_INDICATOR  (MAP_FAILED)
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