#pragma once

#include "core/coredefs.hpp"
#include "core/fs/io_device.hpp"
#include "core/string/string_util.hpp"
#include "core/memory/fixed_buffer.hpp"
#include "core/threading/dummy_read_write_lock.hpp"
#include "core/memory/dynamic_buffer.hpp"
#include "core/fs/fs_util.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {   
            template<                
                size_t _ReadBufferSize,
                size_t _WriteBufferSize,
                bool _UseDynamicBuffer = false, 
                typename _PATH_T = ::pilo::core::string::fixed_astring<MC_PATH_MAX>,                    
                typename _LOCK_TYPE = ::pilo::core::threading::dummy_read_write_lock>
            class buffered_file : public ::pilo::core::fs::io_device
            {
            public:
                typedef _LOCK_TYPE lock_type;
                typedef ::pilo::core::memory::dynamic_buffer<_ReadBufferSize>   dynamic_read_buffer_type;
                typedef ::pilo::core::memory::fixed_buffer<_ReadBufferSize>     fixed_read_buffer_type;
                typedef ::pilo::core::memory::dynamic_buffer<_WriteBufferSize>  dynamic_write_buffer_type;                
                typedef ::pilo::core::memory::fixed_buffer<_WriteBufferSize>    fixed_write_buffer_type;
                typedef  typename _type_switch_< _UseDynamicBuffer, 
                                                dynamic_read_buffer_type,
                                                fixed_read_buffer_type >::type       read_buffer_type;
                typedef  typename _type_switch_< _UseDynamicBuffer,
                                                dynamic_write_buffer_type,
                                                fixed_write_buffer_type >::type       write_buffer_type;

            public:
                buffered_file()
                {
                    _m_os_file_descriptor = MC_INVALID_FILE_DESCRIPTOR;
                    _m_read_buffer.reset();
                    _m_write_buffer.reset();
                    _m_state = eIODS_Uninitialized;
                    _m_context = nullptr;
                    _m_init_flags = 0;

                }
                virtual ~buffered_file()
                {
                    if (! _m_path.empty())
                    {
                        this->_finalize_nolock();
                    }                   
                }

            public: //inherit from base
                virtual ::pilo::error_number_t initialize(const char* path, ::pilo::u32_t flag, void* context)
                {                    
                    if (! _m_path.empty()) //has been initialized
                    {
                        if (this->_finalize_nolock() != ::pilo::EC_OK) //finalize it first
                        {
                            return ::pilo::EC_FINALIZE_FAILED;
                        }                        
                    }
                    return this->_initialize_nolock(path, flag, context); //re-initializing
                }
                virtual ::pilo::error_number_t finalize()
                {
                    if (_m_path.empty()) //has NOT been initialized
                    {
                        return ::pilo::EC_NONSENSE_OPERATION;
                    }

                    return this->_finalize_nolock();

                }

                virtual ::pilo::error_number_t open(DeviceRWModeEnumeration dev_acc_mode, DeviceRWModeEnumeration rw_mode, ::pilo::u32_t flag)
                {
                    M_UNUSED(dev_acc_mode); M_UNUSED(rw_mode); M_UNUSED(flag);
                    return 0;
                }

            virtual ::pilo::error_number_t close()
                {
                    return ::pilo::EC_OK;
                }
            virtual ::pilo::error_number_t read(void* buffer, size_t len, size_t* read_len)
                {
                    M_UNUSED(buffer);
                    M_UNUSED(len);
                    M_UNUSED(read_len);
                    return ::pilo::EC_OK;
                }
            virtual ::pilo::error_number_t write(const void* buffer, size_t len, size_t* written_len)
                {
                    M_UNUSED(buffer);
                    M_UNUSED(len);
                    M_UNUSED(written_len);
                    return ::pilo::EC_OK;
                }
            virtual ::pilo::error_number_t flush(::pilo::i32_t mode)
                {
                    M_UNUSED(mode);
                    return ::pilo::EC_OK;
                }

            virtual ::pilo::error_number_t seek(::pilo::i64_t offset, DeviceSeekWhenceEnumeration eWhence, ::pilo::i64_t* r_offset)
            {
                M_UNUSED(offset); M_UNUSED(eWhence); M_UNUSED(r_offset);
                return ::pilo::EC_OK;
            }

            protected:
                os_file_descriptor_t                                _m_os_file_descriptor; //internal file data structure handle
                _PATH_T                                             _m_path;
                lock_type                                           _m_lock;
                 read_buffer_type                                    _m_read_buffer;
                 write_buffer_type                                   _m_write_buffer;

//                 ::pilo::core::memory::fixed_buffer<_ReadBufferSize> _m_read_buffer;
//                 ::pilo::core::memory::fixed_buffer<_ReadBufferSize> _m_write_buffer;
        
            private:
                virtual ::pilo::error_number_t _initialize_nolock(const char* path, ::pilo::u32_t flag, void* context)
                {
                    if (path == nullptr)
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    _m_init_flags = flag;
                    _m_context = context;
                    ::pilo::error_number_t err = ::pilo::EC_UNDEFINED;

                    char path_buffer[MC_PATH_MAX];

                    if (::pilo::EC_OK != ::pilo::core::fs::fs_util::get_absolute_path(path_buffer, path, sizeof(path_buffer)))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    _m_path = path_buffer;

                    if (flag & MB_IO_DEV_INIT_FLAG_AUTO_CREATE)
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
                            if (flag & MB_IO_DEV_INIT_FLAG_FORCE_DELETE_DIR)
                            {
                                err = ::pilo::core::fs::fs_util::delete_directory(path, true);
                                if (err != ::pilo::EC_OK)
                                {
                                    return err;
                                }
                            }
                            else
                            {
                                return ::pilo::EC_DIR_ALREAD_EXIST;
                            }
                        }
                        else if (enode_type == ::pilo::core::fs::fs_util::eFSNT_RegularFile)
                        {
                            if (flag & MB_IO_DEV_INIT_FLAG_FORCE_DELETE_FILE)
                            {
                                err = ::pilo::core::fs::fs_util::delete_regular_file(path);
                                if (err != ::pilo::EC_OK)
                                {
                                    return err;
                                }
                            }
                            else
                            {
                                return ::pilo::EC_FILE_ALREADY_EXIST;
                            }
                        }
                        err = ::pilo::core::fs::fs_util::create_regular_file(path, true);
                        if (err != ::pilo::EC_OK)
                        {
                            return err;
                        }
                    }     

                    return ::pilo::EC_OK;
                }

                virtual ::pilo::error_number_t _finalize_nolock()
                {
                    if (_m_init_flags & MB_IO_DEV_INIT_FLAG_AUTO_DELETE)
                    {
                        return ::pilo::core::fs::fs_util::delete_regular_file(_m_path.c_str());
                    }

                    _m_read_buffer.reset();
                    _m_write_buffer.reset();
                    _m_state = eIODS_Uninitialized;
                    _m_context = nullptr;
                    _m_init_flags = 0;
                    _m_os_file_descriptor = MC_INVALID_FILE_DESCRIPTOR;
                    
                    return ::pilo::EC_OK;
                }
                
            };
        } // fs
    } //core
}//pilo