#pragma once
#include "./types.hpp"



namespace pilo
{
	enum EnumErrorCode
	{
		EC_OK = 0,
		EC_PARTIAL,
		EC_UNDEFINED,
		EC_TIMEOUT,
		EC_OUT_OF_RANGE,
		EC_REACH_LOWER_LIMIT,
		EC_REACH_UPPER_LIMIT,
		EC_USER_CANCELED,
		EC_INVALID_CALLER,
		EC_INSUFFICIENT_MEMORY,
		EC_BUSY,
		EC_THREAD_NOT_RUNNING,
		EC_GET_THREAD_INFO_FAILED,
		EC_SET_THREAD_INFO_FAILED,
		EC_KILL_THREAD_FAILED,
		EC_SYNC_THREAD_FAILED,
		EC_DETACH_THREAD_FAILED,
		EC_START_THREAD_FAILED,
		EC_THREAD_NOT_EXIST,
		EC_THREAD_ABORTED,
		EC_NONSENSE_OPERATION,
		EC_THREAD_STATE_ERROR,
		EC_ALREADY_DONE,
        EC_INITIALIZE_FAILED,
        EC_FINALIZE_FAILED,
        EC_DATA_TYPE_MISMATCH,
        EC_BUFFER_TOO_SMALL,
        EC_OBJECT_NOT_FOUND,
        EC_INVALID_OBJECT,
        EC_SHUTDOWN_SOCKET_ERROR,
        EC_CLOSE_SOCKET_ERROR,
        EC_UNSUPPORT,
        EC_COPY_STRING_FAILED,
        EC_OPEN_FILE_FAILED,
        EC_LOCK_FILE_ERROR,
        EC_NULL_PTR,
        EC_INVALID_PATH,
        EC_UNDEFINED_FILE_TYPE,
        EC_DIR_ALREAD_EXIST,
        EC_FILE_ALREADY_EXIST,
        EC_INFINITE_LOOP,
        EC_INVALID_PARAM,
        EC_NULL_PARAM,
        EC_NOT_OWNER,
        EC_OP_NOT_ALLOWED,
        EC_COPY_REPLACE_FAILED,
        EC_COPY_CONCAT_FAILED,        
        EC_CLOSE_FILE_ERROR,
        EC_DELETE_FILE_ERROR,
        EC_DIR_NOT_EMPTY,
        EC_DELETE_DIR_ERROR,
        EC_DIR_NOT_EXIST,
        EC_OPEN_DIR_ERROR,
        EC_CLOSE_DIR_ERROR,
        EC_ITER_DIR_ERROR,
        EC_LOAD_DLL_ERROR,
        EC_FIND_FUNCTION_ERROR,
        EC_OPEN_PROCESS_ERROR,
        EC_GET_PROCESS_TIME_ERROR,
        EC_UNLOAD_DLL_ERROR,
        EC_CREATE_DIR_ERROR,
        EC_SET_PROCESS_PRIVILEGE_ERROR,
        EC_POST_COMPPORT_ERROR,
        EC_READ_DIR_CHANGE_ERROR,
        EC_GET_COMPPORT_STATE_ERROR,
        EC_WAIT_MONITOR_DIR_TASK_START_ERROR,
        EC_WAIT_MONITOR_DIR_TASK_STOP_ERROR,
        EC_USER_ABORTED,
        EC_GET_FILE_TIME_ERROR,
        EC_INVALID_FILE_DESCRIPTOR,
        EC_SYNC_FILE_FAILED,
        EC_READ_FILE_ERROR,
        EC_WRITE_FILE_ERROR,
        EC_SEEK_FILE_ERROR,
        EC_UNINITIALIZED,
        EC_DEV_ALREADY_OPENED, 
        EC_DEV_NOT_OPENED,          
        EC_NOT_ABS_PATH,
        EC_GET_FILE_SZ_FAILED,
        EC_CREATE_FILE_MAP_ERROR,
        EC_UNMAP_FAILED,
        EC_MAP_FAILED,
        EC_CONTAINER_FULL,
        EC_DEV_SEEK_ERROR,
        EC_DEV_TRUNK_ERROR,
        EC_CREATE_FILE_ERROR,


        EC_PILO_ERROR_CODE_COUNT, 
	};

}

extern const char* __glb_pilo_error_code_to_string_list[pilo::EC_PILO_ERROR_CODE_COUNT];

#define MF_WITH_OS_ERROR_INFO  0x80000000

#define MAKE_SYSERR(errcode)    ((errcode) | MF_WITH_OS_ERROR_INFO)
#define IS_SYSERR(errcode)  (errcode & MF_WITH_OS_ERROR_INFO)
#define ERRCODE(errcode)    (errcode & ~MF_WITH_OS_ERROR_INFO)

namespace pilo
{
    inline const char* get_pilo_error_string_by_errno(pilo::i32_t error_number)
    {
        if (error_number >= pilo::EC_PILO_ERROR_CODE_COUNT)
        {
            return "Invalid PILO error number";
        }
        return __glb_pilo_error_code_to_string_list[error_number];
    }

    os_error_number_t get_last_os_error_number();

    const char* get_os_error_string_by_errno(char* buffer, size_t buffer_size, os_error_number_t err_number);

    const char* get_last_os_error_string(char* buffer, size_t buffer_size);
}
