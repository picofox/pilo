#pragma once

#include "../compat.hpp"
#include "../error.hpp"
#include "../types.hpp"

#include <stddef.h>


#define MC_PATH_MAX                                 (260)
#define MC_FIXEDSTRING_MAX_ELEMENTS                 (-1)
#define MC_FORMAT_OUTPUT_BUFFER_DEFAULT_SIZE        (40960)
#define MC_THREAD_MAX_STACK_SIZE                    (10*1024*1024)
#define MC_READ_DIR_CHANGE_BUFFER_SIZE              (4096)


#define M_DEBUG_NEW_INTERNAL_BUCKET_SIZE            (256)
#define M_DEBUG_NEW_REQUIRE_THREAD_SAFE
#define M_DEBUG_NEW_USE_REF_FILENAME
#define M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_NEW
//#define M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_DELETE
#define M_DEBUG_NEW_INFO_REQUIRE_TIMESTAMP_ON_REPORT
//#define M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_NEW
#define M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_DELETE
//#define M_DEBUG_NEW_INFO_REQUIRE_THREADID_ON_REPORT
//#define M_DEBUG_NEW_INFO_REQUIRE_BT_ON_NEW 
//#define M_DEBUG_NEW_INFO_REQUIRE_BT_ON_REPORT

#define M_DEBUG_NEW_BT_INFO_SIZE    (2048)
#define M_DEBUG_NEW_DEFAULT_REPORT_FILE_PATH    "./debug_heap_alloc_report.log"
