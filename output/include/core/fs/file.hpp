#pragma once

#include "core/coredefs.hpp"
#include "core/fs/io_device.hpp"
#include "core/string/string_util.hpp"
#include "core/memory/fixed_buffer.hpp"
#include "core/threading/dummy_read_write_lock.hpp"
#include "core/string/auto_string.hpp"
#include "core/memory/dynamic_buffer.hpp"
#include "core/fs/fs_util.hpp"
#include "core/threading/rw_mutex_r_locker.hpp"
#include "core/threading/rw_mutex_w_locker.hpp"
#include "core/fs/path.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {
            template<
                size_t  PATH_BUFFER_SZ = 0,
                typename _LOCK_TYPE = ::pilo::core::threading::dummy_read_write_lock>
            class file : public ::pilo::core::fs::io_device
            {
            public:
                typedef _LOCK_TYPE lock_type;

            public:
                file()
                {
                    _m_os_file_descriptor = MC_INVALID_FILE_DESCRIPTOR;
                    _m_state = eIODS_Uninitialized;
                    _m_context = nullptr;
                    _m_init_flags = 0;
                    _m_path.reset();
                }
                ~file()
                {
                    if (_m_path.valid())
                    {
                        this->_finalize_nolock();
                    }
                }

            public:
                virtual ::pilo::error_number_t initialize(const char* path, ::pilo::u32_t flag, void* context)
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);

                    if (_m_path.valid()) //has been initialized
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
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);

                    if (!_m_path.valid()) //has NOT been initialized
                    {
                        return ::pilo::EC_NONSENSE_OPERATION;
                    }

                    return this->_finalize_nolock();
                }

                virtual ::pilo::error_number_t open(DeviceAccessModeEnumeration dev_acc_mode, DeviceRWModeEnumeration rw_mode, ::pilo::u32_t flag)
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);

                    if (! _m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }                    

                    if (_m_os_file_descriptor != MC_INVALID_FILE_DESCRIPTOR)
                    {
                        if (!(flag & MC_IO_DEV_OP_REOPEN))
                        {
                            return ::pilo::EC_DEV_ALREADY_OPENED;
                        }
                        _close_nolock();
                    }

                    return _open_nolock(dev_acc_mode, rw_mode, flag);
                }

                virtual ::pilo::error_number_t close()
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);

                    if (!_m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_NONSENSE_OPERATION;
                    }

                    return _close_nolock();
                }

                virtual ::pilo::error_number_t read(void* buffer, size_t len, size_t* read_len)
                {
                    ::pilo::core::threading::rw_mutex_r_locker<lock_type> locker(_m_lock);

                    if (! _m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_DEV_NOT_OPENED;
                    }

                    return _read_nolock(buffer, len, read_len);
                }

                ::pilo::error_number_t write(const void* buffer, size_t len, size_t* written_len)
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);

                    if (! _m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_DEV_NOT_OPENED;
                    }

                    return _write_nolock(buffer, len, written_len);
                }

                virtual ::pilo::error_number_t flush(::pilo::i32_t mode)
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);
                    if (!_m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_DEV_NOT_OPENED;
                    }
                    return _flush_nolock(mode);
                }

                virtual ::pilo::error_number_t seek(::pilo::i64_t offset, DeviceSeekWhenceEnumeration eWhence, ::pilo::i64_t* r_offset)
                {
                    ::pilo::core::threading::rw_mutex_w_locker<lock_type> locker(_m_lock);

                    if (!_m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_DEV_NOT_OPENED;
                    }

                    return _seek_nolock(offset, eWhence, r_offset);
                }

                ::pilo::error_number_t flock_shared(size_t start_pos, size_t partial_size)
                {
                    if (!_m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_DEV_NOT_OPENED;
                    }
                    return ::pilo::core::fs::fs_util::lock_file(_m_os_file_descriptor, false, start_pos,  partial_size);
                }               

                ::pilo::error_number_t flock_exclusive(size_t start_pos, size_t partial_size)
                {
                    if (!_m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_DEV_NOT_OPENED;
                    }
                    return ::pilo::core::fs::fs_util::lock_file(_m_os_file_descriptor, true, start_pos,  partial_size);
                }

                ::pilo::error_number_t try_flock_shared(size_t start_pos, size_t partial_size) //MC_INVALID_SIZE means hole fiel
                {
                    if (!_m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_DEV_NOT_OPENED;
                    }
                    return ::pilo::core::fs::fs_util::try_lock_file(_m_os_file_descriptor, false, start_pos,  partial_size);
                }

                ::pilo::error_number_t try_flock_exclusive(size_t start_pos, size_t partial_size)
                {
                    if (!_m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_DEV_NOT_OPENED;
                    }
                    return ::pilo::core::fs::fs_util::try_lock_file(_m_os_file_descriptor, true, start_pos,  partial_size);
                }

                ::pilo::error_number_t funlock(size_t start_pos, size_t partial_size)
                {
                    if (!_m_path.valid())
                    {
                        return ::pilo::EC_UNINITIALIZED;
                    }

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_DEV_NOT_OPENED;
                    }
                    return ::pilo::core::fs::fs_util::unlock_file(_m_os_file_descriptor,  start_pos,  partial_size);
                }

            protected:
                ::pilo::error_number_t _initialize_nolock(const char* path, ::pilo::u32_t flag, void* context)
                {
                    if (path == nullptr)
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    _m_init_flags = flag;
                    _m_context = context;
                    ::pilo::error_number_t err = ::pilo::EC_UNDEFINED;

                    if (!_m_path.assign_path_string(path))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }
                    if (::pilo::EC_OK != _m_path.to_absolute(false, true))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

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
                            if (flag & MC_IO_DEV_FLAG_FORCE_DELETE_FILE_ON_INITIALIZ)
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

                ::pilo::error_number_t _finalize_nolock()
                {
                    if (_m_os_file_descriptor != MC_INVALID_FILE_DESCRIPTOR)
                    {
                        _close_nolock();
                    }

                    if (_m_init_flags & MC_IO_DEV_FLAG_AUTO_DELETE_ON_FINALIZE)
                    {
                        return ::pilo::core::fs::fs_util::delete_regular_file(_m_path.c_str());
                    }

                    _m_state = eIODS_Uninitialized;
                    _m_context = nullptr;
                    _m_init_flags = 0;
                    _m_os_file_descriptor = MC_INVALID_FILE_DESCRIPTOR;

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _open_nolock(DeviceAccessModeEnumeration dev_acc_mode, DeviceRWModeEnumeration rw_mode, ::pilo::u32_t flag)
                {
                    ::pilo::error_number_t ret =  ::pilo::core::fs::fs_util::open_file(_m_os_file_descriptor, 
                                                                _m_path.c_str(),
                                                                dev_acc_mode,
                                                                rw_mode,
                                                                flag);
                    if (ret == ::pilo::EC_OK)
                    {
                        _m_access_mode = dev_acc_mode;
                        _m_rw_mode = rw_mode;
                        _m_open_flag = false;
                    }

                    return ret;
                }

                ::pilo::error_number_t _close_nolock()
                {
                    _flush_nolock(0);
                    
#ifdef          WINDOWS
                    ::CloseHandle(_m_os_file_descriptor);
#else
                    ::close(_m_os_file_descriptor);
#endif

                    _m_open_flag = 0;
                    _m_rw_mode = ::pilo::core::fs::eDRWM_None;
                    _m_access_mode = ::pilo::core::fs::eDAM_OpenExisting;
                    _m_os_file_descriptor = MC_INVALID_FILE_DESCRIPTOR;

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _read_nolock(void* buffer, size_t len, size_t* read_len)
                {
                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_INVALID_FILE_DESCRIPTOR;
                    }
#ifdef          WINDOWS
                    if (! ReadFile(_m_os_file_descriptor,
                        buffer,
                        (DWORD)len,
                        (DWORD*)read_len,
                        NULL))
                    {
                        return ::pilo::EC_READ_FILE_ERROR;
                    }
#else
                    ssize_t nRet = ::read(_m_os_file_descriptor, buffer, len);
                    if (read_len != nullptr) 
                    {
                        *read_len = (size_t) nRet;
                    }
                    if ( -1 == nRet)
                    {
                        return ::pilo::EC_READ_FILE_ERROR;
                    }                                        

#endif
                    return ::pilo::EC_OK;
                    
                }

                ::pilo::error_number_t _write_nolock(const void* buffer, size_t len, size_t* written_len)
                {
                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_INVALID_FILE_DESCRIPTOR;
                    }
#ifdef          WINDOWS
                    if (!WriteFile(_m_os_file_descriptor,
                        buffer,
                        (DWORD) len,
                        (DWORD*)written_len,
                        NULL))
                    {
                        return ::pilo::EC_WRITE_FILE_ERROR;
                    }
#else
                    ssize_t nRet = ::write(_m_os_file_descriptor, buffer, len);
                    if (written_len != nullptr)
                    {
                        *written_len = (size_t)nRet;
                    }
                    if (-1 == nRet)
                    {
                        return ::pilo::EC_WRITE_FILE_ERROR;
                    }
#endif
                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _flush_nolock(::pilo::i32_t mode)
                {
                    M_UNUSED(mode);

                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_INVALID_FILE_DESCRIPTOR;
                    }
#ifdef          WINDOWS
                    if (! ::FlushFileBuffers(_m_os_file_descriptor))
                    {
                        return ::pilo::EC_SYNC_FILE_FAILED;
                    }
#else
                    if (::fsync(_m_os_file_descriptor) != 0)
                    {
                        return ::pilo::EC_SYNC_FILE_FAILED;
                    }
#endif
                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _seek_nolock(::pilo::i64_t offset, DeviceSeekWhenceEnumeration eWhence, ::pilo::i64_t* r_offset)
                {
                    if (_m_os_file_descriptor == MC_INVALID_FILE_DESCRIPTOR)
                    {
                        return ::pilo::EC_INVALID_FILE_DESCRIPTOR;
                    }
#ifdef          WINDOWS
                    LARGE_INTEGER liOff;
                    liOff.QuadPart = offset;
                    LARGE_INTEGER liRet;
                    if (!::SetFilePointerEx(_m_os_file_descriptor, liOff, &liRet, eWhence))
                    {
                        return ::pilo::EC_SEEK_FILE_ERROR;
                    }
                    if (r_offset)
                    {
                        *r_offset = liRet.QuadPart;
                    }                
#else
                    off64_t nRet = lseek64(_m_os_file_descriptor, offset, eWhence);
                    if (r_offset != nullptr) *r_offset = nRet;
                    if (nRet == -1)
                    {
                        return ::pilo::EC_SEEK_FILE_ERROR;
                    }
#endif               
                    return ::pilo::EC_OK;
                }
                

            protected:
                os_file_descriptor_t                        _m_os_file_descriptor; //internal file data structure handle
                ::pilo::core::fs::path_string<PATH_BUFFER_SZ>      _m_path;
                lock_type                                   _m_lock;

            };
        }
    }
}