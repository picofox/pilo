#ifndef _file_lock_h_
#define _file_lock_h_

#include "../../pilo.hpp"

#ifdef WINDOWS
#   define PMI_LOCK_ENTIRE_FILE_LENGTH MAXDWORD
#else
#   define PMI_LOCK_ENTIRE_FILE_LENGTH (0)
#endif

namespace pilo
{
    namespace core
    {
        namespace process
        {
            class file_lock
            {
            public:
                file_lock(::pilo::os_file_handle_t fd);
                file_lock();
                ~file_lock();

                ::pilo::err_t initialize(::pilo::os_file_handle_t fd);
                ::pilo::err_t finalize();
                ::pilo::err_t lock_shared(::pilo::i64_t offset, ::pilo::i64_t length);
                ::pilo::err_t lock(::pilo::i64_t offset, ::pilo::i64_t length);
                ::pilo::err_t try_lock(::pilo::i64_t offset, ::pilo::i64_t length);
                ::pilo::err_t try_lock_shared(::pilo::i64_t offset, ::pilo::i64_t length);
                ::pilo::err_t unlock(::pilo::i64_t offset, ::pilo::i64_t length);
                ::pilo::err_t unlock();

            protected:
                ::pilo::os_file_handle_t    _m_fd;
                bool                        _m_lock;

#ifdef WINDOWS
                DWORD _m_info_len_low;
                DWORD _m_info_len_high;
                OVERLAPPED _m_info_overlapped;                
#else

                struct flock _m_info;
#endif

                
#ifdef WINDOWS
                ::pilo::err_t _lock(::pilo::i64_t offset, ::pilo::i64_t length, DWORD flag);
#else

                ::pilo::err_t _lock(::pilo::i64_t offset, ::pilo::i64_t length, int cmd, int type);
#endif

                PMC_DISABLE_COPY(file_lock)
            };

        }
    }
}




#endif // !_file_lock_h_
