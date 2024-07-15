#include "file_lock.hpp"
#include <cstring>



pilo::core::process::file_lock::file_lock()
{
    _m_fd = PMI_INVALID_FILE_HANDLE;
    _m_lock = false;
    _m_owner = false;
#ifdef WINDOWS
    _m_info_len_low = MAXDWORD;
    _m_info_len_high = MAXDWORD;
    memset(&_m_info_overlapped, 0x00, sizeof(_m_info_overlapped));
#else
    ::memset(&_m_info, 0x00, sizeof(_m_info));
#endif
}

pilo::core::process::file_lock::~file_lock()
{
    this->finalize();
}

::pilo::err_t pilo::core::process::file_lock::lock(::pilo::i64_t offset, ::pilo::i64_t length)
{
#ifdef WINDOWS
    return _lock(offset, length, LOCKFILE_EXCLUSIVE_LOCK);
#else
    return _lock(offset, length, F_SETLKW, F_WRLCK);
#endif
}

::pilo::err_t  pilo::core::process::file_lock::try_lock(::pilo::i64_t offset, ::pilo::i64_t length)
{
#ifdef WINDOWS
     return _lock(offset, length, LOCKFILE_EXCLUSIVE_LOCK | LOCKFILE_FAIL_IMMEDIATELY);
#else
     return _lock(offset, length, F_SETLK, F_WRLCK);
#endif

}

::pilo::err_t pilo::core::process::file_lock::initialize(::pilo::os_file_handle_t fd, const char* path_str, ::pilo::core::io::creation_mode cm, ::pilo::core::io::access_permission perm)
{

    if (this->_m_fd != PMI_INVALID_FILE_HANDLE) {
        return ::pilo::mk_perr(PERR_DUP_INIT);
    }

    if (fd != PMI_INVALID_FILE_HANDLE && path_str != nullptr) {
        return ::pilo::mk_perr(PERR_INVALID_PARAM);
    }
    else if (fd != PMI_INVALID_FILE_HANDLE && path_str != nullptr) {
        return ::pilo::mk_perr(PERR_NULL_PARAM);
    } else if (fd != PMI_INVALID_FILE_HANDLE) {
        _m_fd = fd;
    } else {
        _m_fd = ::pilo::core::io::xpf_open_file(path_str, cm, perm, ::pilo::core::io::dev_open_flags::none);
    }

    if (this->_m_fd != PMI_INVALID_FILE_HANDLE) {
        return ::pilo::mk_perr(PERR_IO_OPEN_FAIL);
    }

    return PILO_OK;
}

::pilo::err_t pilo::core::process::file_lock::finalize()
{
    if (_m_lock && _m_fd != PMI_INVALID_FILE_HANDLE) {
        this->unlock();
    }

    if (_m_owner && _m_fd != PMI_INVALID_FILE_HANDLE) {
        ::pilo::core::io::xpf_close_file(&(this->_m_fd));
    }

    _m_fd = PMI_INVALID_FILE_HANDLE;
    _m_lock = false;
#ifdef WINDOWS
    _m_info_len_low = MAXDWORD;
    _m_info_len_high = MAXDWORD;
    memset(&_m_info_overlapped, 0x00, sizeof(_m_info_overlapped));
#else
    ::memset(&_m_info, 0x00, sizeof(_m_info));
#endif
    return PILO_OK;
}

::pilo::err_t pilo::core::process::file_lock::lock_shared(::pilo::i64_t offset, ::pilo::i64_t length)
{
#ifdef WINDOWS
    return _lock(offset, length,  0);
#else
    return _lock(offset, length, F_SETLK, F_RDLCK);
#endif
}

::pilo::err_t  pilo::core::process::file_lock::try_lock_shared(::pilo::i64_t offset, ::pilo::i64_t length)
{
    ::pilo::err_t ret = PILO_OK;
#ifdef WINDOWS
    ret = _lock(offset, length, LOCKFILE_FAIL_IMMEDIATELY);
#else
    ret = _lock(offset, length, F_SETLK, F_WRLCK);
#endif
    if (ret == PERR_RETRY)
        return false;
    return true;
}

::pilo::err_t pilo::core::process::file_lock::unlock(::pilo::i64_t offset, ::pilo::i64_t length)
{
    if (_m_fd == PMI_INVALID_FILE_HANDLE) {
        return ::pilo::mk_perr(PERR_NOT_INIT);
    }
#ifdef WINDOWS
    ULARGE_INTEGER off = { 0 };
    ULARGE_INTEGER len = { 0 };
    off.QuadPart = (ULONGLONG)offset;
    len.QuadPart = (ULONGLONG)length;
    _m_info_overlapped.OffsetHigh = off.HighPart;
    _m_info_overlapped.Offset = off.LowPart;
    BOOL fSuccess = UnlockFileEx(_m_fd, 0, len.LowPart, len.HighPart, &_m_info_overlapped);
    if (!fSuccess) {
        return ::pilo::mk_err(PERR_UNLOCK_FILE_FAIL);
    }
    _m_info_overlapped.OffsetHigh = off.HighPart;
    _m_info_overlapped.Offset = off.LowPart;
    _m_lock = false;
    return PILO_OK;

#else
    if (length == -1)
        length = 0;
    _m_info.l_type = F_UNLCK;
    _m_info.l_whence = SEEK_SET;
    _m_info.l_start = offset;
    _m_info.l_len = length;
    int ret = fcntl(_m_fd, F_SETLK, &_m_info);
    if (ret != 0) {
        return ::pilo::mk_err(PERR_UNLOCK_FILE_FAIL);
    }
    _m_lock = false;
    return PILO_OK;
#endif
}

::pilo::err_t pilo::core::process::file_lock::unlock()
{
    if (_m_fd == PMI_INVALID_FILE_HANDLE) {
        return ::pilo::mk_perr(PERR_NOT_INIT);
    }
#ifdef WINDOWS
    BOOL fSuccess = UnlockFileEx(_m_fd, 0, _m_info_len_low, _m_info_len_high, &_m_info_overlapped);
    if (!fSuccess) {
        return ::pilo::mk_err(PERR_UNLOCK_FILE_FAIL);
    }
    _m_lock = false;
    return PILO_OK;

#else
    _m_info.l_type = F_UNLCK;
    _m_info.l_whence = SEEK_SET;
    int ret = fcntl(_m_fd, F_SETLK, &_m_info);
    if (ret != 0) {
        return ::pilo::mk_err(PERR_UNLOCK_FILE_FAIL);
    }
    _m_lock = false;
    return PILO_OK;
#endif
}

#ifdef WINDOWS
::pilo::err_t pilo::core::process::file_lock::_lock(::pilo::i64_t offset, ::pilo::i64_t length, DWORD flags)
{
    if (_m_fd == PMI_INVALID_FILE_HANDLE) {
        return ::pilo::mk_perr(PERR_NOT_INIT);
    }

    ULARGE_INTEGER off = { 0 };
    off.QuadPart = (ULONGLONG)offset;   
    
    ULARGE_INTEGER len = { 0 };   
    len.QuadPart = (ULONGLONG)length;
    _m_info_overlapped.OffsetHigh = off.HighPart;
    _m_info_overlapped.Offset = off.LowPart;
    _m_info_len_low = len.LowPart;
    _m_info_len_high = len.HighPart;
    
    BOOL fSuccess = LockFileEx(_m_fd, flags, 0, _m_info_len_low, _m_info_len_high, &_m_info_overlapped);
    if (!fSuccess) {
        if (GetLastError() == ERROR_IO_PENDING) {
            return ::pilo::mk_err(PERR_RETRY);
        }
        return ::pilo::mk_err(PERR_LOCK_FILE_FAIL);
    }   

    _m_lock = true;
    return PILO_OK;

}
#else
::pilo::err_t pilo::core::process::file_lock::_lock(::pilo::i64_t offset, ::pilo::i64_t length, int cmd, int type)
{
    if (_m_fd == PMI_INVALID_FILE_HANDLE) {
        return ::pilo::mk_perr(PERR_NOT_INIT);
    }

    if (length == -1)
        length = 0;
    _m_info.l_type = type;
    _m_info.l_whence = SEEK_SET;
    _m_info.l_start = offset;
    _m_info.l_len = length;
    int ret = fcntl(_m_fd, cmd, &_m_info);
    if (ret != 0) {
        if (errno == EACCES || errno == EAGAIN) {
            return ::pilo::mk_err(PERR_RETRY);
        }
        return ::pilo::mk_err(PERR_LOCK_FILE_FAIL);
    }
    _m_lock = true;
    return PILO_OK;
}
#endif