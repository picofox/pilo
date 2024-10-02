#pragma once

#define KILO(x)        ((x)*1000)
#define MEGA(x)        ((x)*1000000)
#define GIGA(x)        ((x)*1000000000)

#define BKILO(x)       ((x)*1024)
#define BMEGA(x)       ((x)*1048576)
#define BGIGA(x)       ((x)*1073741824)


#define PMI_MEMORY_ALLOCATOR_RECORDER_ENABLE_LEVEL		(1)	

#define SP_PMI_TLV_STEP                    (65536)
#define SP_PMI_TASK_STEP                   (65536)
#define SP_PMI_PATH_DEFAULT_LENGTH          (1)
#define SP_PMS_PILO_PREDEF_DIR_CNF          "conf"
#define SP_PMS_PILO_PREDEF_DIR_TMP          "tmp"
#define SP_PMS_PILO_PREDEF_DIR_LOG          "log"
#define SP_PMI_TEXT_FILE_RDBUF_SIZE         (1024)
#define SP_PMI_TEXT_FILE_WRBUF_SIZE         (0)
#define SP_PMI_TEXT_FILE_DFL_LINE_SIZE      (128)
#define SP_PMI_TEXT_FILE_LINE_STEP_SIZE     (64)
#define SP_PMS_TEXT_FILE_DFL_LINE_RSEP      "\n"
#define SP_PMS_TEXT_FILE_DFL_LINE_WSEP      "\n"
#define SP_PMS_LOG_FILE_DFL_FLD_SEP         "\t"
#define SP_PMI_FILE_BUFRD_BUF_SIZE          (4096)
#define SP_PMI_THREAD_POOL_WORKER_COUNT		(4)
#define SP_PMI_THREAD_TASK_EXECUTOR_COUNT	(1)

#define SP_PMI_LBKBUF_NODE_4K_UNIT_SIZE     (BKILO(4))
#define SP_PMI_LBKBUF_NODE_4K_STEP_SIZE     (1024)