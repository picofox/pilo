#include "../../pilo.hpp"
#include "../../tlv.hpp"
#include "./device_interface.hpp"
#include "path.hpp"
#include "../threading/dummy_read_write_lock.hpp"
#include "../process/dummy_file_lock.hpp"
#include "../process/file_lock.hpp"


namespace pilo
{
    namespace core
    {
        namespace io
        {
            template <typename PLOCK = ::pilo::core::process::dummy_file_lock
                ,typename TLOCK = ::pilo::core::threading::dummy_read_write_lock>
            class file : public device_interface
            {
            public:
                typedef PLOCK process_lock_type;
                typedef TLOCK thread_lock_type;

            public:
                file()
                {
                    _m_fd = PMI_INVALID_FILE_HANDLE;
                    this->set_state(state_code::uninitialized);
                }

                virtual ~file()
                {
                    this->finalize();
                }

                file(file&& other) noexcept
                {
                    _m_path = std::move(other._m_path);
                    _m_fd = other._m_fd;
                    other._m_fd = PMI_INVALID_FILE_HANDLE;
                }

                file& operator=(file&& other) noexcept
                {
                    if (this != &other)
                    {
                        device_interface::operator=(std::move(other));
                        _m_path = std::move(other._m_path);
                        _m_fd = other._m_fd;
                        other._m_fd = PMI_INVALID_FILE_HANDLE;
                    }
                    return *this;
                }

            public:
                // Inherited via device_interface
                virtual ::pilo::err_t initialize(::pilo::tlv* param)
                {
                    PMC_UNUSED(param);
                    if (this->state() > state_code::uninitialized)
                    {
                        return PILO_OK;
                    }
                    this->set_state(state_code::initialized);
                    return PILO_OK;
                }

                virtual ::pilo::err_t open(const char* path_str, creation_mode cm, access_permission perm, predefined_pilo_dir_enum prefix, dev_open_flags f)
                {
                    ::pilo::err_t err = _pre_open();
                    if (err != PILO_OK)
                        return err;

                    this->_m_path.set(path_str, prefix);
                    if (this->_m_path.invalid()) {
                        return ::pilo::mk_perr(PERR_INVALID_PATH);
                    }
                    return this->_open(cm, perm, f);
                }

                virtual ::pilo::err_t open(const ::pilo::core::io::path* p, creation_mode cm, access_permission perm, dev_open_flags f)
                {
                    ::pilo::err_t err = _pre_open();
                    if (err != PILO_OK)
                        return err;

                    this->_m_path = *p;
                    if (this->_m_path.invalid()) {
                        return ::pilo::mk_perr(PERR_INVALID_PATH);
                    }
                    return this->_open(cm, perm, f);
                }


                virtual ::pilo::err_t open(const char* path_str, creation_mode cm, access_permission perm, dev_open_flags f)
                {
                    ::pilo::err_t err = _pre_open();
                    if (err != PILO_OK)
                        return err;

                    this->_m_path.set(path_str, predefined_pilo_dir_enum::count);
                    if (this->_m_path.invalid()) {
                        return ::pilo::mk_perr(PERR_INVALID_PATH);
                    }
                    return this->_open(cm, perm, f);
                }

                virtual ::pilo::err_t read(char* buffer, ::pilo::i64_t capacity, ::pilo::i64_t rbs, ::pilo::i64_t* n_read)
                {
                    PMC_UNUSED(buffer);
                    PMC_UNUSED(capacity);
                    PMC_UNUSED(rbs);
                    PMC_UNUSED(n_read);
                    return ::pilo::err_t();
                }

                virtual ::pilo::err_t read(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t rbs, ::pilo::i64_t* n_read)
                {
                    PMC_UNUSED(buf);
                    PMC_UNUSED(rbs);
                    PMC_UNUSED(n_read);
                    return ::pilo::err_t();
                }


                virtual ::pilo::err_t write(const char* buffer, ::pilo::i64_t wbs, ::pilo::i64_t* n_written)
                {
                    PMC_UNUSED(buffer);
                    PMC_UNUSED(wbs);
                    PMC_UNUSED(n_written);
                    return ::pilo::err_t();
                }

                virtual ::pilo::err_t write(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t wbs, ::pilo::i64_t* n_written)
                {
                    PMC_UNUSED(buf);
                    PMC_UNUSED(wbs);
                    PMC_UNUSED(n_written);
                    return ::pilo::err_t();
                }

                virtual ::pilo::err_t seek(seek_whence whence, ::pilo::i64_t off)
                {
                    if (error() != 0)
                        return ::pilo::mk_perr(PERR_HAS_PREV_ERR);

                    if (state() != state_code::opened)
                        return ::pilo::mk_perr(PERR_INV_IO_STATE);

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
                    BOOL ret = SetFilePointerEx(this->_m_fd, neo_off, &old_off, w);
                    if (!ret)
                        return ::pilo::mk_err(PERR_IO_SEEK_FAIL);

#else
                    int w = SEEK_SET;
                    if (whence == seek_whence::end)
                        w = SEEK_END;
                    else if (whence == seek_whence::current)
                        w = SEEK_CUR;


                    off_t ret = lseek(this->_m_fd, (off_t)off, w);
                    if (ret == (off_t)-1)
                        return ::pilo::mk_err(PERR_IO_SEEK_FAIL);

#endif // WINDOWS

                    return PILO_OK;
                }

                virtual ::pilo::err_t control(::pilo::i32_t cmd, ::pilo::tlv* param)
                {
                    PMC_UNUSED(cmd);
                    PMC_UNUSED(param);
                    return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
                }

                virtual ::pilo::err_t sync(::pilo::i32_t mode)
                {
                    PMC_UNUSED(mode);
                    return ::pilo::err_t();
                }

                virtual ::pilo::err_t close()
                {
                    if (this->_m_fd != PMI_INVALID_FILE_HANDLE) {
#ifdef WINDOWS
                        CloseHandle(this->_m_fd);
#else
                        ::close(this->_m_fd);
#endif
                        this->_m_fd = PMI_INVALID_FILE_HANDLE;

                    }
                    this->set_state(state_code::closed);
                    return PILO_OK;
                }

                virtual ::pilo::err_t finalize()
                {
                    if (this->_m_fd != PMI_INVALID_FILE_HANDLE) {
#ifdef WINDOWS
                        CloseHandle(this->_m_fd);
#else
                        ::close(this->_m_fd);
#endif
                        this->_m_fd = PMI_INVALID_FILE_HANDLE;

                    }                    
                    this->set_state(state_code::uninitialized);
                    this->_m_proc_lock.finalize();
                    return PILO_OK;
                }                

                virtual ::pilo::err_t exist() const
                {
                    if (this->_m_path.invalid())
                        return ::pilo::mk_perr(PERR_FSNODE_NON_EXIST);
                    ::pilo::i8_t target_node_type = 0;
                    if (path::path_type_na == this->_m_path.get_fs_info(&target_node_type, nullptr)) {
                        return ::pilo::mk_perr(PERR_FSNODE_NON_EXIST);
                    }
                    if (target_node_type == path::fs_node_type_file) {
                        return ::pilo::mk_perr(PERR_FSNODE_EXIST);
                    }
                    return ::pilo::mk_perr(PERR_NOT_A_REG_FILE);
                }

                ::pilo::err_t remove()
                {
                    if (exist() == PERR_FSNODE_EXIST) {
                        return this->_m_path.remove(false, false);
                    }
                    return ::pilo::mk_perr(PERR_FSNODE_EXIST);
                }


                inline ::pilo::err_t process_lock_shared(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_proc_lock.lock_shared(offset, length);
                }

                inline ::pilo::err_t process_lock(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_proc_lock.lock(offset, length);
                }

                inline ::pilo::err_t process_try_lock(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_proc_lock.try_lock(offset, length);
                }

                inline ::pilo::err_t process_try_lock_shared(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_proc_lock.try_lock_shared(offset, length);
                }

                inline ::pilo::err_t process_unlock(::pilo::i64_t offset, ::pilo::i64_t length)
                {
                    return this->_m_proc_lock.unlock(offset, length);
                }

                inline ::pilo::err_t process_unlock()
                {
                    return this->_m_proc_lock.unlock();
                }

                inline void thread_lock_shared() 
                {
                    return this->_m_thread_lock->lock_shared();
                }

                inline void lock() 
                {
                    return this->_m_thread_lock->lock();
                }

                inline bool try_lock() 
                { 
                    return this->_m_thread_lock->try_lock();
                }

                inline bool try_lock_shared() 
                { 
                    return this->_m_thread_lock->try_lock_shared();
                }

                inline void unlock() 
                {
                    return this->_m_thread_lock->unlock();
                }

                inline void unlock_shared() 
                {
                    return this->_m_thread_lock->unlock_shared();
                }


            protected:
                ::pilo::err_t _open(creation_mode cm, access_permission perm, dev_open_flags f)
                {

#ifdef WINDOWS
                    DWORD dwShareMode = FILE_SHARE_READ;
                    DWORD dwDesiredAccess = 0;
                    if (((::pilo::u8_t)perm & (::pilo::u8_t)access_permission::read) != 0) {
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

                    _m_fd = CreateFile(
                        _m_path.fullpath(),
                        dwDesiredAccess,
                        dwShareMode,
                        NULL,
                        dwCreationDisposition,
                        dwFlagsAndAttributes,
                        NULL
                    );

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

                    if (cm == creation_mode::create_always) {
                        oflag |= (O_CREAT | O_WRONLY | O_TRUNC);
                    }
                    else if (cm == creation_mode::open_existing) {

                    }
                    else if (cm == creation_mode::create_neo) {
                        oflag |= O_EXCL;
                    }
                    else if (cm == creation_mode::open_always) {
                        oflag |= O_CREAT;
                    }
                    else if (cm == creation_mode::open_trunc) {
                        oflag |= O_CREAT;
                        oflag |= O_TRUNC;
                    }

                    _m_fd = ::open(_m_path.fullpath(), oflag, m);

#endif
                    if (_m_fd == PMI_INVALID_FILE_HANDLE) {
                        this->set_error();
                        return ::pilo::mk_err(PERR_IO_OPEN_FAIL);
                    }

                    this->_m_flags = f;
                    this->_m_creation_mode = cm;
                    this->_m_access_perm = perm;
                    this->set_state(state_code::opened);

                    this->_m_proc_lock.initialize(this->_m_fd);
                    return PILO_OK;
                }

                ::pilo::err_t _pre_open() const
                {
                    if (error() != 0)
                        return ::pilo::mk_perr(PERR_HAS_PREV_ERR);
                    if (state() == state_code::opened)
                        return ::pilo::mk_perr(PERR_INV_IO_STATE);
                    return PILO_OK;
                }


            private:
                ::pilo::core::io::path      _m_path;
                ::pilo::os_file_handle_t    _m_fd;
                process_lock_type           _m_proc_lock;
                thread_lock_type            _m_thread_lock;

                PMC_DISABLE_COPY(file)    
                    
            };
        }
    }
}