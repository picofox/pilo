#ifndef _pilo_core_io_file_hpp_
#define _pilo_core_io_file_hpp_

#include "../../pilo.hpp"
#include "../../tlv.hpp"
#include "./device_interface.hpp"
#include "path.hpp"
#include "../threading/dummy_read_write_lock.hpp"
#include "../process/dummy_file_lock.hpp"
#include "../process/shared_file_lock_guard.hpp"
#include "../process/file_lock_guard.hpp"
#include "../process/file_lock.hpp"
#include "../threading/shared_mutex_guard.hpp"
#include "../threading/exclusive_mutex_guard.hpp"
#include "../io/io.hpp"

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
                static ::pilo::err_t  s_buffer_write_func(
                    ::pilo::core::memory::byte_buffer_interface*
                    , char* buffer
                    , ::pilo::i64_t b_len, void* ctx)
                {                    
                    ::pilo::i64_t rlen = 0;
                    file* filp = (file*)ctx;
                    return filp->write(buffer, b_len, &rlen);
                }

                static void s_free_all_buffer(char** p)
                {
                    if (p != nullptr && *p != nullptr) {
                        PMF_HEAP_FREE(*p);
                        *p = nullptr;
                    }
                }

            public:
                file()
                {
                    _m_fd = PMI_INVALID_FILE_HANDLE;
                    this->set_state(state_code::uninitialized);
#ifdef WINDOWS
                    memset(&_m_overlapped, 0x00, sizeof(_m_overlapped));
#endif
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

                ::pilo::err_t set_path(const ::pilo::core::io::path* path)
                {
                    if (this->state() > state_code::initialized) {
                        return ::pilo::mk_perr(PERR_EXIST);
                    }
                    this->_m_path = *path;
                    return PILO_OK;
                }

                ::pilo::err_t set_size(::pilo::i64_t length)
                {
                    return ::pilo::core::io::xpf_set_size(_m_fd, length);
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

                virtual ::pilo::err_t open(creation_mode cm, access_permission perm, dev_open_flags f)
                {
                    if (this->_m_path.invalid())
                        return ::pilo::mk_err(PERR_INVALID_PATH);
                    ::pilo::err_t err = _pre_open();
                    if (err != PILO_OK)
                        return err;

                    if ((err = _m_path.ensure_parent_path_exist()) != PILO_OK) {
                        return err;
                    }

                    return this->_open(cm, perm, f);                    
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

                    err = this->_m_path.ensure_parent_path_exist();
                    if (err != PILO_OK) {
                        return ::pilo::mk_perr(PERR_INVALID_PATH);
                    }

                    return this->_open(cm, perm, f);
                }

                virtual ::pilo::err_t open(const ::pilo::core::io::path* p, creation_mode cm, access_permission perm, dev_open_flags f)
                {
                    ::pilo::err_t err = _pre_open();
                    if (err != PILO_OK)
                        return err;

                    if ((err = p->ensure_parent_path_exist()) != PILO_OK) {
                        return err;
                    }

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

                virtual ::pilo::err_t read(char* buffer, ::pilo::i64_t rbs, ::pilo::i64_t* n_read)
                {
                    ::pilo::err_t err = _pre_read();
                    if (err != PILO_OK)
                        return err;

                    return _read(buffer, rbs, n_read);
                }

                virtual ::pilo::err_t read(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t rbs, ::pilo::i64_t* n_read)
                {
                    ::pilo::err_t err = _pre_read();
                    if (err != PILO_OK)
                        return err;


                    return _read(buf, rbs, n_read);
                }

                virtual char* read_all(char* buffer, ::pilo::i64_t capa, ::pilo::i64_t* n_read, ::pilo::err_t * rerr)
                {
                    ::pilo::err_t err = _pre_read();
                    if (err != PILO_OK) {
                        ::pilo::set_if_ptr_is_not_null(rerr, err);
                        return nullptr;
                    }


                    ::pilo::i64_t size_to_read = 0;
                    err = xpf_get_file_size(this->_m_fd, size_to_read);
                    if (err != PILO_OK) {
                        ::pilo::set_if_ptr_is_not_null(rerr, err);
                        return nullptr;
                    }

                    char* ptr = buffer;
                    if (capa < size_to_read + 1 || buffer == nullptr) {
                        ptr = (char*)PMF_HEAP_MALLOC(size_to_read + 1);
                        if (ptr == nullptr) {
                            ::pilo::set_if_ptr_is_not_null(rerr, ::pilo::mk_err(PERR_INSUF_HEAP));
                            return nullptr;
                        }
                        ptr[size_to_read] = 0;
                    }

                    err = _read(ptr, size_to_read, n_read);
                    if (err != PILO_OK) {
                        ::pilo::set_if_ptr_is_not_null(rerr, err);
                        return nullptr;
                    }

                    return ptr;
                }


                virtual ::pilo::err_t write(const char* buffer, ::pilo::i64_t wbs, ::pilo::i64_t* n_written)
                {
                    ::pilo::err_t err = _pre_write();
                    if (err != PILO_OK)
                        return err;

                    return _write(buffer, wbs, n_written);
                }


                virtual ::pilo::err_t write(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t wbs, ::pilo::i64_t* n_written)
                {
                    ::pilo::err_t err = _pre_write();
                    if (err != PILO_OK)
                        return err;

                    return _write(buf, wbs, n_written);
                }

                virtual ::pilo::err_t seek(seek_whence whence, ::pilo::i64_t off)
                {
                    if (error())
                        return ::pilo::mk_perr(PERR_HAS_PREV_ERR);
                    if (state() != state_code::opened)
                        return ::pilo::mk_perr(PERR_INV_IO_STATE);
                    return ::pilo::core::io::xpf_seek_file(_m_fd, whence, off);
                }

                virtual ::pilo::err_t tell(::pilo::i64_t & off)
                {
                    return pilo::core::io::xpf_tell_file(_m_fd, off);
                }

                virtual ::pilo::err_t control(::pilo::i32_t cmd, ::pilo::tlv* param)
                {
                    PMC_UNUSED(cmd);
                    PMC_UNUSED(param);
                    return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
                }

                virtual ::pilo::err_t flush(flush_level lv)
                {
                    return xpf_flush_file(_m_fd, lv);
                }

                virtual ::pilo::err_t close()
                {
                    ::pilo::core::io::xpf_close_file(&_m_fd);
                    this->set_state(state_code::closed);
                    return PILO_OK;
                }

                virtual ::pilo::err_t finalize()
                {
                    if (this->_m_fd != PMI_INVALID_FILE_HANDLE) {
                        ::pilo::core::io::xpf_close_file(&_m_fd);
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
                    return this->_m_thread_lock.lock_shared();
                }

                inline void lock() 
                {
                    return this->_m_thread_lock.lock();
                }

                inline bool try_lock() 
                { 
                    return this->_m_thread_lock.try_lock();
                }

                inline bool try_lock_shared() 
                { 
                    return this->_m_thread_lock.try_lock_shared();
                }

                inline void unlock() 
                {
                    return this->_m_thread_lock.unlock();
                }

                inline void unlock_shared() 
                {
                    return this->_m_thread_lock.unlock_shared();
                }


            protected:
                ::pilo::err_t _read(char* buffer, ::pilo::i64_t rbs, ::pilo::i64_t* n_read)
                {
#ifdef WINDOWS
                    DWORD r_dwnread = 0;
                    LPOVERLAPPED lpOverlapped = NULL;
                    if (this->nonblocking())
                        lpOverlapped = &this->_m_overlapped;
                    BOOL bret = ReadFile(_m_fd, (LPVOID)buffer, (DWORD)rbs, &r_dwnread, lpOverlapped);
                    ::pilo::set_if_ptr_is_not_null(n_read, (::pilo::i64_t)r_dwnread);
                    if (!bret) {                        
                        set_error();
                        return ::pilo::mk_err(PERR_IO_READ_FAIL);
                    }                    
                    
                    if (r_dwnread < rbs) {
                        return ::pilo::mk_perr(PERR_EOF);
                    }
#else
                    ssize_t r_read = ::read(_m_fd, buffer, rbs);
                    if (r_read < 0) {
                        ::pilo::set_if_ptr_is_not_null(n_read, (::pilo::i64_t) 0);
                    } else {
                        ::pilo::set_if_ptr_is_not_null(n_read, (::pilo::i64_t)r_read);
                    }
                    
                    if (r_read < 0) {
                        set_error();
                        return ::pilo::mk_err(PERR_IO_READ_FAIL);
                    } else if (r_read == 0) {
                        return ::pilo::mk_perr(PERR_EOF);
                    } else if (r_read < rbs) {
                        return ::pilo::mk_perr(PERR_EOF);
                    }     
#endif // WINDOWS

                    return PILO_OK;
                }

                ::pilo::err_t _read(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t rbs, ::pilo::i64_t* n_read)
                {
                    char tmp_buff[SP_PMI_4K_BUFFER_NODE_SIZE] = { 0 };
                    ::pilo::i64_t tmp_space = SP_PMI_4K_BUFFER_NODE_SIZE;
                    ::pilo::err_t err = PILO_OK;
                    ::pilo::i64_t rb = 0;
                    ::pilo::i64_t rb_total = 0;
                    while (rbs > 0)
                    {
                        if (rbs > SP_PMI_4K_BUFFER_NODE_SIZE)
                            tmp_space = SP_PMI_4K_BUFFER_NODE_SIZE;
                        else
                            tmp_space = rbs;
                        rbs -= tmp_space;

                        err = _read(tmp_buff, tmp_space, &rb);
                        if (err == PILO_OK) {
                            buf->write_raw_bytes(tmp_buff, 0, rb);
                            rb_total += rb;     

                        } else if ( err == PERR_EOF) {
                            buf->write_raw_bytes(tmp_buff, 0, rb);
                            rb_total += rb;
                            break;

                        } else {
                            return err;
                        }
                
                    }
                    ::pilo::set_if_ptr_is_not_null(n_read, rb_total);
                    return PILO_OK;
                }

                ::pilo::err_t _write(const char* buffer, ::pilo::i64_t wbs, ::pilo::i64_t* n_written)
                {
#ifdef WINDOWS
                    DWORD dw_written = 0;
                    LPOVERLAPPED pol = NULL;
                    if (this->nonblocking()) {
                        pol = &_m_overlapped;
                    }
                    BOOL bret = WriteFile(this->_m_fd, buffer, (DWORD)wbs, &dw_written, pol);
                    ::pilo::set_if_ptr_is_not_null(n_written, (::pilo::i64_t)dw_written);
                    if (!bret) {
                        return pilo::mk_err(PERR_IO_WRITE_FAIL);
                    }

                    if ((::pilo::i64_t)dw_written < wbs)
                        return pilo::mk_err(PERR_WR_PARTIAL_DATA);
                    
#else

                    ssize_t ret = ::write(this->_m_fd, buffer, (size_t)wbs);
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

                ::pilo::err_t _write(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t wbs, ::pilo::i64_t* n_written)
                {
                    ::pilo::i64_t written_bs = 0;
                    ::pilo::err_t err = buf->iterate(file::s_buffer_write_func, this, wbs, &written_bs, false);
                    ::pilo::set_if_ptr_is_not_null(n_written, written_bs);
                    return err;
                }

                ::pilo::err_t _open(creation_mode cm, access_permission perm, dev_open_flags f)
                {
                    _m_fd = ::pilo::core::io::xpf_open_file(_m_path.fullpath(), cm, perm, f);
                    if (_m_fd == PMI_INVALID_FILE_HANDLE) {
                        this->set_error();
                        return ::pilo::mk_err(PERR_IO_OPEN_FAIL);
                    }

                    this->_m_flags = f;
                    this->_m_creation_mode = cm;
                    this->_m_access_perm = perm;
                    this->set_state(state_code::opened);

                    return this->_m_proc_lock.initialize(this->_m_fd, nullptr);
                }

                ::pilo::err_t _pre_open() const
                {
                    if (error())
                        return ::pilo::mk_perr(PERR_HAS_PREV_ERR);
                    if (state() == state_code::opened)
                        return ::pilo::mk_perr(PERR_INV_IO_STATE);


                    return PILO_OK;
                }

                ::pilo::err_t _pre_read() const
                {
                    if (error())
                        return ::pilo::mk_perr(PERR_HAS_PREV_ERR);
                    if (state() != state_code::opened)
                        return ::pilo::mk_perr(PERR_INV_IO_STATE);
                    return PILO_OK;
                }

                ::pilo::err_t _pre_write()
                {
                    if (error())
                        return ::pilo::mk_perr(PERR_HAS_PREV_ERR);
                    if (state() != state_code::opened)
                        return ::pilo::mk_perr(PERR_INV_IO_STATE);

#ifdef WINDOWS
                    if (this->append()) {
                        if (this->seek(seek_whence::end, 0) != PILO_OK)
                            return ::pilo::mk_err(PERR_IO_SEEK_FAIL);
                    }                    
                    
#endif

                    return PILO_OK;
                }

                ::pilo::err_t _seek(seek_whence whence, ::pilo::i64_t off)
                {
                    return ::pilo::core::io::xpf_seek_file(this->_m_fd, whence, off);
                }


            protected:
                ::pilo::core::io::path      _m_path;
                ::pilo::os_file_handle_t    _m_fd;
                process_lock_type           _m_proc_lock;
                thread_lock_type            _m_thread_lock;

#ifdef WINDOWS
                OVERLAPPED                  _m_overlapped;
#else
#endif

                PMC_DISABLE_COPY(file)    
                    
            };
        }
    }
}

#endif