#include "io.hpp"

::pilo::os_file_handle_t pilo::core::io::xpf_open_file(const char* path_str, creation_mode cm, access_permission perm, dev_open_flags f)
{
    ::pilo::os_file_handle_t fd = PMI_INVALID_FILE_HANDLE;

#ifdef WINDOWS
    DWORD dwShareMode = FILE_SHARE_READ;
    DWORD dwDesiredAccess = 0;
    if (((::pilo::u8_t)perm & (::pilo::u8_t)access_permission::read) != 0) {
        dwShareMode |= FILE_SHARE_READ;
        dwDesiredAccess |= GENERIC_READ;
    }
    if (((::pilo::u8_t)perm & (::pilo::u8_t)access_permission::write) != 0) {
        dwDesiredAccess |= GENERIC_WRITE;
        dwShareMode |= FILE_SHARE_WRITE;
    }
    if (((::pilo::u8_t)perm & (::pilo::u8_t)access_permission::exec) != 0) {
        dwDesiredAccess |= GENERIC_EXECUTE;
    }
    DWORD dwCreationDisposition = 0;
    if (cm == creation_mode::create_always) {
        dwCreationDisposition = CREATE_ALWAYS;
    }
    else if (cm == creation_mode::open_existing) {
        dwCreationDisposition = OPEN_EXISTING;
    }
    else if (cm == creation_mode::create_neo) {
        dwCreationDisposition = CREATE_NEW;
    }
    else if (cm == creation_mode::open_always) {
        dwCreationDisposition = OPEN_ALWAYS;
    }
    else if (cm == creation_mode::open_trunc) {
        dwCreationDisposition = TRUNCATE_EXISTING;
    }
    DWORD dwFlagsAndAttributes = FILE_ATTRIBUTE_NORMAL;
    if ((((::pilo::u8_t)f & ((::pilo::u8_t) ::pilo::core::io::dev_open_flags::nonblocking)) != 0))
        dwFlagsAndAttributes |= FILE_FLAG_OVERLAPPED;
    if ((((::pilo::u8_t)f & ((::pilo::u8_t) ::pilo::core::io::dev_open_flags::temp)) != 0))
        dwFlagsAndAttributes |= FILE_ATTRIBUTE_TEMPORARY;
    if ((((::pilo::u8_t)f & ((::pilo::u8_t) ::pilo::core::io::dev_open_flags::write_through)) != 0))
        dwFlagsAndAttributes |= FILE_FLAG_WRITE_THROUGH;

    fd = CreateFile(
        path_str,
        dwDesiredAccess,
        dwShareMode,
        NULL,
        dwCreationDisposition,
        dwFlagsAndAttributes,
        NULL
    );

    if ((((::pilo::u8_t)f & ((::pilo::u8_t) ::pilo::core::io::dev_open_flags::append)) != 0)) {
        if (xpf_seek_file(fd, seek_whence::end, 0) != PILO_OK)
            return PMI_INVALID_FILE_HANDLE;
    }

#else
    int oflag = 0;
    if (perm == access_permission::all
        || perm == access_permission::read_write) {
        oflag = O_RDWR;
    }
    else if (perm == access_permission::exec
        || perm == access_permission::read_exec
        || perm == access_permission::read) {
        oflag = O_RDONLY;
    }
    else if (perm == access_permission::write_exec || perm == access_permission::write) {
        oflag = O_WRONLY;
    }

    mode_t m = S_IRWXU;
    if (((::pilo::u8_t)perm & (::pilo::u8_t)access_permission::read) != 0) {
        m |= S_IRGRP;
        m |= S_IROTH;
    }
    if (((::pilo::u8_t)perm & (::pilo::u8_t)access_permission::write) != 0) {
        m |= S_IWGRP;
        m |= S_IWOTH;
    }
    if (((::pilo::u8_t)perm & (::pilo::u8_t)access_permission::exec) != 0) {
        m |= S_IXGRP;
        m |= S_IXOTH;
    }


    if ((((::pilo::u8_t)f & ((::pilo::u8_t) ::pilo::core::io::dev_open_flags::nonblocking)) != 0))
        oflag |= O_NONBLOCK;
    if ((((::pilo::u8_t)f & ((::pilo::u8_t) ::pilo::core::io::dev_open_flags::temp)) != 0))
        oflag |= O_TMPFILE;
    if ((((::pilo::u8_t)f & ((::pilo::u8_t) ::pilo::core::io::dev_open_flags::write_through)) != 0))
        oflag |= O_SYNC;
    if ((((::pilo::u8_t)f & ((::pilo::u8_t) ::pilo::core::io::dev_open_flags::append)) != 0))
        oflag |= O_APPEND;

    if (cm == creation_mode::create_always) {
        oflag |= (O_CREAT | O_TRUNC);
    }
    else if (cm == creation_mode::open_existing) {

    }
    else if (cm == creation_mode::create_neo) {
        oflag |= O_EXCL;
        oflag |= O_CREAT;
    }
    else if (cm == creation_mode::open_always) {
        oflag |= O_CREAT;
    }
    else if (cm == creation_mode::open_trunc) {
        oflag |= O_CREAT;
        oflag |= O_TRUNC;
    }

    fd = ::open(path_str, oflag, m);
#endif


    return fd;
}

void pilo::core::io::xpf_close_file(::pilo::os_file_handle_t* fd)
{
    if (*fd != PMI_INVALID_FILE_HANDLE) {
#ifdef WINDOWS
        ::CloseHandle(*fd);
#else
        ::close(*fd);
#endif
        *fd = PMI_INVALID_FILE_HANDLE;

    }
}

::pilo::err_t pilo::core::io::xpf_seek_file(::pilo::os_file_handle_t fd, seek_whence whence, ::pilo::i64_t off)
{
#ifdef WINDOWS
    LARGE_INTEGER neo_off = { 0 };
    LARGE_INTEGER old_off = { 0 };
    neo_off.QuadPart = off;
    old_off.QuadPart = 0;
    DWORD w = FILE_BEGIN;
    if (whence == seek_whence::end)
        w = FILE_END;
    else if (whence == seek_whence::current)
        w = FILE_CURRENT;
    BOOL ret = SetFilePointerEx(fd, neo_off, &old_off, w);
    if (!ret)
        return ::pilo::mk_err(PERR_IO_SEEK_FAIL);

#else
    int w = SEEK_SET;
    if (whence == seek_whence::end)
        w = SEEK_END;
    else if (whence == seek_whence::current)
        w = SEEK_CUR;

    off_t ret = lseek(fd, (off_t)off, w);
    if (ret == (off_t)-1)
        return ::pilo::mk_err(PERR_IO_SEEK_FAIL);

#endif // WINDOWS

    return PILO_OK;
}

::pilo::err_t pilo::core::io::xpf_tell_file(::pilo::os_file_handle_t fd, ::pilo::i64_t& off)
{
#ifdef WINDOWS
    LARGE_INTEGER ret = { 0 };
    LARGE_INTEGER pos = { 0 };
    pos.QuadPart = 0;
    BOOL bret = SetFilePointerEx(fd, pos, &ret, FILE_CURRENT);
    if (!bret) {
        off = -1;
        return ::pilo::mk_err(PERR_IO_SEEK_FAIL);
    }
    off = ret.QuadPart;
#else
    off_t offset = lseek(fd, 0, SEEK_CUR);
    if (offset == (off_t)-1) {
        off = -1;
        return ::pilo::mk_err(PERR_IO_SEEK_FAIL);
    }
    off = (::pilo::i64_t)offset;
#endif
    return PILO_OK;
}

::pilo::err_t pilo::core::io::xpf_flush_file(::pilo::os_file_handle_t fd, flush_level lv)
{
#ifdef WINDOWS
    PMC_UNUSED(lv);
    if (! ::FlushFileBuffers(fd)) {
        return ::pilo::mk_err(PERR_IO_SYNC_FAIL);
    }
#else
    int ret = 0;
    if (lv == flush_level::partial) {
        ret = ::fdatasync(fd);
    } else {
        ret = ::fsync(fd);
    }
    if (ret != 0) {
        return ::pilo::mk_err(PERR_IO_SYNC_FAIL);
    }
#endif

    return PILO_OK;
}

::pilo::err_t pilo::core::io::xpf_write_file(::pilo::os_file_handle_t fd, const char* buffer, ::pilo::i64_t wbs, ::pilo::i64_t* n_written)
{
#ifdef WINDOWS
    DWORD dw_written = 0;
    BOOL bret = WriteFile(fd, buffer, (DWORD)wbs, &dw_written, nullptr);
    ::pilo::set_if_ptr_is_not_null(n_written, (::pilo::i64_t)dw_written);
    if (!bret) {
        return pilo::mk_err(PERR_IO_WRITE_FAIL);
    }
    if ((::pilo::i64_t)dw_written < wbs)
        return pilo::mk_err(PERR_WR_PARTIAL_DATA);
#else
    ssize_t ret = ::write(fd, buffer, (size_t)wbs);
    if (ret < 0) {
        ::pilo::set_if_ptr_is_not_null(n_written, (::pilo::i64_t)0);
        return pilo::mk_err(PERR_IO_WRITE_FAIL);
    }
    ::pilo::set_if_ptr_is_not_null(n_written, (::pilo::i64_t)ret);
    if ((::pilo::i64_t)ret < wbs)
        return pilo::mk_err(PERR_WR_PARTIAL_DATA);

#endif // 
    return PILO_OK;
}

::pilo::err_t pilo::core::io::xpf_read_file(::pilo::os_file_handle_t fd, char* buffer, ::pilo::i64_t rbs, ::pilo::i64_t* n_read)
{
#ifdef WINDOWS
    DWORD r_dwnread = 0;
    BOOL bret = ReadFile(fd, (LPVOID)buffer, (DWORD)rbs, &r_dwnread, nullptr);
    ::pilo::set_if_ptr_is_not_null(n_read, (::pilo::i64_t)r_dwnread);
    if (!bret) {
        return ::pilo::mk_err(PERR_IO_READ_FAIL);
    }

    if (r_dwnread < rbs) {
        return ::pilo::mk_perr(PERR_EOF);
    }
#else
    ssize_t r_read = ::read(fd, buffer, rbs);
    if (r_read < 0) {
        ::pilo::set_if_ptr_is_not_null(n_read, (::pilo::i64_t)0);
    }
    else {
        ::pilo::set_if_ptr_is_not_null(n_read, (::pilo::i64_t)r_read);
    }

    if (r_read < 0) {
        return ::pilo::mk_err(PERR_IO_READ_FAIL);
    }
    else if (r_read == 0) {
        return ::pilo::mk_perr(PERR_EOF);
    }
    else if (r_read < rbs) {
        return ::pilo::mk_perr(PERR_EOF);
    }
#endif // WINDOWS
    return PILO_OK;
}

::pilo::err_t pilo::core::io::xpf_set_size(::pilo::os_file_handle_t fd, ::pilo::i64_t sz)
{
#ifdef WINDOWS
    LARGE_INTEGER liDistanceToMove;
    liDistanceToMove.QuadPart = sz;
    BOOL bret = SetFilePointerEx(fd, liDistanceToMove, NULL, FILE_BEGIN);
    if (!bret) {
        return ::pilo::mk_err(PERR_IO_SEEK_FAIL);
    }
    bret = SetEndOfFile(fd);
    if (!bret) {
        return ::pilo::mk_err(PERR_FILE_SETSIZE_FAIL);
    }

#else
    int r = ftruncate(fd, (off_t) sz);
    if (r == -1)
        return ::pilo::mk_err(PERR_FILE_SETSIZE_FAIL);

#endif
    return PILO_OK;
}

::pilo::err_t pilo::core::io::xpf_get_file_size(::pilo::os_file_handle_t fd, ::pilo::i64_t& sz)
{
#ifdef WINDOWS
    LARGE_INTEGER size = {0};
    BOOL bret = ::GetFileSizeEx(fd, &size);
    if (!bret)
        return ::pilo::mk_err(PERR_IO_READ_FAIL);
    sz = size.QuadPart;
#else
    struct stat file_inf = {0};
    int ret = fstat(fd, &file_inf);
    if (0! = ret)
        return ::pilo::mk_err(PERR_IO_READ_FAIL);
    sz = (::pilo::i64_t)file_inf.st_size;

#endif
    return PILO_OK;
}

