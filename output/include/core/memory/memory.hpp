#pragma once

#include "core/coredefs.hpp"

#if (__BITS__ == 64)
#   define MC_MEM_POOL_UNIT_FREED_FLAG (0xFFFFEEEEFFFFEEEE)
#else
#   define MC_MEM_POOL_UNIT_FREED_FLAG (0xFFFFEEEE)
#endif

#define M_MM_POOL_ALIGN_USIZE(usize,cnt) M_MAX_SIZE((usize+sizeof(void*)-1)&(~(sizeof(void*)-1)),sizeof(void*)*(cnt))

#define MB_DEBUG_POOL_SHOW_FREELIST_DETAIL  (0x1)
#define MB_DEBUG_POOL_SHOW_AVAILIST_DETAIL  (0x2)
#define MB_DEBUG_POOL_SHOW_FULLLIST_DETAIL  (0x3)
#define MB_DEBUG_POOL_SHOW_UNIT_IMAGE  (0x4)