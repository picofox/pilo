#pragma once
#include "core/coredefs.hpp"

#define MC_IO_DEV_OP_FLAG_APPEND    1
#define MC_IO_DEV_OP_FLAG_TRUNC     2
#define MC_IO_DEV_OP_FLAG_CREATE    4
#define MC_IO_DEV_OP_FLAG_EXCL      8

#define MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE        (1<<0)
#define MC_IO_DEV_FLAG_FORCE_DELETE_DIR_ON_INITIALIZE   (1<<1)
#define MC_IO_DEV_FLAG_AUTO_DELETE_ON_CLOSE             (1<<2)
#define MC_IO_DEV_FLAG_AUTO_DELETE_ON_FINALIZE          (1<<3)
#define MC_IO_DEV_FLAG_REQUIRE_FILELOCK                 (1<<4)
#define MC_IO_DEV_FLAG_FORCE_DELETE_FILE_ON_INITIALIZ   (1<<5)






namespace pilo
{
    namespace core
    {
        namespace fs
        {
            typedef enum
            {
#               ifdef  WINDOWS
                eODM_CreateAlways = CREATE_ALWAYS,/**< Creates a new file, always. */
                eODM_CreateNew = CREATE_NEW,/**< Creates a new file, only if it does not already exist. */
                eODM_OpenAlways = OPEN_ALWAYS, /**<  Opens a file, always.*/
                eODM_OpenExisting = OPEN_EXISTING, /**< Opens a file or device, only if it exists. */
                eODM_TruncateExisting = TRUNCATE_EXISTING, /**< Opens a file and truncates it so that its size is zero bytes, only if it exists.*/
#               else
                eODM_CreateAlways = (O_CREAT | O_TRUNC),/**< Creates a new file, always. */
                eODM_CreateNew = (O_EXCL | O_CREAT),/**< Creates a new file, only if it does not already exist. */
                eODM_OpenAlways = O_CREAT, /**<  Opens a file, always.*/
                eODM_OpenExisting = 0, /**< Opens a file or device, only if it exists. */
                eODM_TruncateExisting = O_TRUNC, /**< Opens a file and truncates it so that its size is zero bytes, only if it exists.*/
#               endif
            } OpenDeviceModeEnumeration;

            typedef enum
            {
#               ifdef  WINDOWS
                eDAM_Read = GENERIC_READ,
                eDAM_Write = GENERIC_WRITE,
                eDAM_ReadWrite = (GENERIC_READ | GENERIC_WRITE),
#               else
                eDAM_Read = O_RDONLY,
                eDAM_Write = O_WRONLY,
                eDAM_ReadWrite = O_RDWR,

#               endif
            } DeviceAccessModeEnumeration;

            enum EnumDevFlushMode
            {
                eDFM_Sync = 0,
                eDFM_Async = 0,
            };

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
                }

                virtual ~io_device()
                {
                    
                }

                virtual ::pilo::error_number_t initialize(const char* path, ::pilo::u32_t flag, void* context) = 0;
                virtual ::pilo::error_number_t finalize() = 0;
                virtual ::pilo::error_number_t open(DeviceAccessModeEnumeration dev_acc_mode, ::pilo::u32_t op_flag) = 0;
                virtual ::pilo::error_number_t close() = 0;
                virtual ::pilo::error_number_t read(void* buffer, size_t len, size_t* read_len) = 0;
                virtual ::pilo::error_number_t write(const void* buffer, size_t len, size_t* written_len) = 0;
                virtual ::pilo::error_number_t flush(::pilo::i32_t mode) = 0;

                inline void set_context(void* context)
                {
                    m_context = context;
                }
            protected:
                void* m_context;
                volatile EnumIODeviceState m_state;
                ::pilo::u32_t m_init_flags;
            };
        }
    }    
}  