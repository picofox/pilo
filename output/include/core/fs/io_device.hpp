#pragma once
#include "core/coredefs.hpp"

#define MC_IO_DEV_OP_FLAG_APPEND    (1<<0)
#define MC_IO_DEV_OP_NO_OS_CACHE    (1<<1)
#define MC_IO_DEV_OP_REOPEN         (1<<2)


#define MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE        (1<<0)
#define MC_IO_DEV_FLAG_FORCE_DELETE_DIR_ON_INITIALIZE   (1<<1)
#define MC_IO_DEV_FLAG_FORCE_DELETE_FILE_ON_INITIALIZ   (1<<2)
#define MC_IO_DEV_FLAG_AUTO_DELETE_ON_CLOSE             (1<<3)
#define MC_IO_DEV_FLAG_AUTO_DELETE_ON_FINALIZE          (1<<4)


namespace pilo
{
    namespace core
    {
        namespace fs
        {
            typedef enum
            {
#               ifdef  WINDOWS
                eDAM_CreateAlways = CREATE_ALWAYS,/**< Creates a new file, always. */
                eDAM_CreateNew = CREATE_NEW,/**< Creates a new file, only if it does not already exist. */
                eDAM_OpenAlways = OPEN_ALWAYS, /**<  Opens a file, always.*/
                eDAM_OpenExisting = OPEN_EXISTING, /**< Opens a file or device, only if it exists. */
                eDAM_TruncateExisting = TRUNCATE_EXISTING, /**< Opens a file and truncates it so that its size is zero bytes, only if it exists.*/
#               else
                eDAM_CreateAlways = (O_CREAT | O_TRUNC),/**< Creates a new file, always. */
                eDAM_CreateNew = (O_EXCL | O_CREAT),/**< Creates a new file, only if it does not already exist. */
                eDAM_OpenAlways = O_CREAT, /**<  Opens a file, always.*/
                eDAM_OpenExisting = 0, /**< Opens a file or device, only if it exists. */
                eDAM_TruncateExisting = O_TRUNC, /**< Opens a file and truncates it so that its size is zero bytes, only if it exists.*/
#               endif
            } DeviceAccessModeEnumeration;

            typedef enum
            {
#               ifdef  WINDOWS
                eDRWM_None = 0,
                eDRWM_Read = GENERIC_READ,
                eDRWM_Write = GENERIC_WRITE,
                eDRWM_ReadWrite = (GENERIC_READ | GENERIC_WRITE),
#               else
                eDRWM_None = 0,
                eDRWM_Read = O_RDONLY,
                eDRWM_Write = O_WRONLY,
                eDRWM_ReadWrite = O_RDWR,

#               endif
            } DeviceRWModeEnumeration;            

            typedef enum 
            {
#               ifdef  WINDOWS
                eDSW_Begin = FILE_BEGIN,
                eDSW_Current = FILE_CURRENT,
                eDSW_END = FILE_CURRENT,

#               else
                eDSW_Begin = SEEK_SET,
                eDSW_Current = SEEK_CUR,
                eDSW_END = SEEK_END,

#               endif

            } DeviceSeekWhenceEnumeration;

            class io_device
            {
            public:           
                enum EnumIODeviceState
                {
                    eIODS_Uninitialized = 0,
                    eIODS_Initialized   = 1,
                    eIODS_Opend         = 2,
                };

                io_device()
                {
                    m_context = nullptr;
                    m_state = eIODS_Uninitialized;
                    m_init_flags = 0;
                    m_access_mode = eDAM_OpenExisting;
                    m_rw_mode = eDRWM_None;
                    m_open_flag = 0;
                }

                virtual ~io_device()
                {
                    
                }

                virtual ::pilo::error_number_t initialize(const char* path, ::pilo::u32_t flag, void* context) = 0;
                virtual ::pilo::error_number_t finalize() = 0;
                virtual ::pilo::error_number_t open(DeviceAccessModeEnumeration dev_acc_mode, DeviceRWModeEnumeration rw_mode, ::pilo::u32_t flag) = 0;
                virtual ::pilo::error_number_t close() = 0;
                virtual ::pilo::error_number_t read(void* buffer, size_t len, size_t* read_len) = 0;
                virtual ::pilo::error_number_t write(const void* buffer, size_t len, size_t* written_len) = 0;
                virtual ::pilo::error_number_t flush(::pilo::i32_t mode) = 0;
                virtual ::pilo::error_number_t seek(::pilo::i64_t offset, DeviceSeekWhenceEnumeration eWhence, ::pilo::i64_t* r_offset) = 0;

                inline void set_context(void* context)
                {
                    m_context = context;
                }
            protected:
                void* m_context;
                volatile EnumIODeviceState m_state;
                ::pilo::u32_t m_init_flags;
                DeviceAccessModeEnumeration m_access_mode;
                DeviceRWModeEnumeration m_rw_mode;
                ::pilo::u32_t m_open_flag;
            };
        }
    }    
}  