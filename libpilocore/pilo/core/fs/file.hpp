#pragma once

#include "core/coredefs.hpp"
#include "core/fs/io_device.hpp"
#include "core/string/string_util.hpp"
#include "core/memory/fixed_buffer.hpp"
#include "core/threading/dummy_mutex.hpp"
#include "core/string/auto_astring.hpp"
#include "core/memory/dynamic_buffer.hpp"
#include "core/fs/fs_util.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {   
            template<   
                size_t _PathCapacity,
                size_t _ReadBufferSize,
                size_t _WriteBufferSize,
                bool _UseDynamicBuffer = false,                        
                typename _LOCK_TYPE = ::pilo::core::threading::dummy_mutex>
            class file : public ::pilo::core::fs::io_device
            {
            public:
                typedef _LOCK_TYPE lock_type;
                typedef ::pilo::core::memory::dynamic_buffer<_ReadBufferSize>   dynamic_read_buffer_type;
                typedef ::pilo::core::memory::fixed_buffer<_ReadBufferSize>     fixed_read_buffer_type;
                typedef ::pilo::core::memory::dynamic_buffer<_WriteBufferSize>  dynamic_write_buffer_type;                
                typedef ::pilo::core::memory::fixed_buffer<_WriteBufferSize>    fixed_write_buffer_type;
                typedef  _type_switch_< _UseDynamicBuffer, 
                                                dynamic_read_buffer_type,
                                                fixed_read_buffer_type >        read_buffer_type;
                typedef  _type_switch_< _UseDynamicBuffer,
                                                dynamic_write_buffer_type,
                                                fixed_write_buffer_type >       write_buffer_type;

            public: //inherit from base
                virtual ::pilo::i32_t initialize(const char* path, ::pilo::u32_t flag, void* context)
                {
                    if (path == nullptr)
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    m_init_flags = flag;
                    m_context = context;

                    char path_buffer[MC_PATH_MAX];
                    if (!::pilo::core::fs::fs_util::get_absolute_path(path_buffer, path, sizeof(path_buffer)))
                    {
                        return ::pilo::EC_INVALID_PATH; 
                    }

                    _m_path = path_buffer;

                    if (flag & MC_IO_DEV_FLAG_AUTO_CREATE_ON_INITIALIZE)
                    {
                        ::pilo::core::fs::fs_util::EnumFSNodeType enode_type = ::pilo::core::fs::fs_util::calculate_type(_m_path.c_str());
                        if (enode_type == ::pilo::core::fs::fs_util::eFSNT_Error)
                        {
                            return ::pilo::EC_OPEN_FILE_FAILED;
                        }
                        else if (enode_type == ::pilo::core::fs::fs_util::eFSNT_InvalidPath)
                        {
                            return ::pilo::EC_INVALID_PATH;
                        }
                        else if (enode_type == ::pilo::core::fs::fs_util::eFSNT_TypeUndefined)
                        {
                            return ::pilo::EC_NONSENSE_OPERATION;
                        }
                        else if (enode_type == ::pilo::core::fs::fs_util::eFSNT_NonExist)
                        {
                            
                        }
                        else if (enode_type == ::pilo::core::fs::fs_util::eFSNT_Directory)
                        {
                            if (flag & MC_IO_DEV_FLAG_FORCE_DELETE_DIR_ON_INITIALIZE)
                            {

                            }
                        }
                    }
                     


                    return ::pilo::EC_OK;
                }
                virtual ::pilo::i32_t finalize()
                {
                    return ::pilo::EC_OK;
                }
                virtual ::pilo::i32_t open(DeviceAccessModeEnumeration rw_mode, ::pilo::u32_t op_flag)
                {
                    M_UNUSED(rw_mode);
                    M_UNUSED(op_flag);
                    return ::pilo::EC_OK;
                }
                virtual ::pilo::i32_t close()
                {
                    return ::pilo::EC_OK;
                }
                virtual ::pilo::i32_t read(void* buffer, size_t len, size_t* read_len)
                {
                    M_UNUSED(buffer);
                    M_UNUSED(len);
                    M_UNUSED(read_len);
                    return ::pilo::EC_OK;
                }
                virtual ::pilo::i32_t write(const void* buffer, size_t len, size_t* written_len)
                {
                    M_UNUSED(buffer);
                    M_UNUSED(len);
                    M_UNUSED(written_len);
                    return ::pilo::EC_OK;
                }
                virtual ::pilo::i32_t flush(::pilo::i32_t mode)
                {
                    M_UNUSED(mode);
                    return ::pilo::EC_OK;
                }

            protected:
                os_file_descriptor_t                                _m_os_file_descriptor; //internal file data structure handle
                ::pilo::core::string::auto_astring<_PathCapacity>   _m_path;
                lock_type                                           _m_lock;
                read_buffer_type                                    _m_read_buffer;
                write_buffer_type                                   _m_write_buffer;
            };
        } // fs
    } //core
}//pilo