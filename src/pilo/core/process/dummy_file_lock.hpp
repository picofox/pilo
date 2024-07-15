#ifndef _dummy_dummy_file_lock_h_
#define _dummy_dummy_file_lock_h_

#include "../io/io.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            class dummy_file_lock
            {
            public:
                dummy_file_lock() {}
                dummy_file_lock(::pilo::os_file_handle_t fd) { PMC_UNUSED(fd);}

                inline ::pilo::err_t initialize(::pilo::os_file_handle_t fd, const char* path_str
                    , ::pilo::core::io::creation_mode cm = ::pilo::core::io::creation_mode::open_existing
                    , ::pilo::core::io::access_permission perm = ::pilo::core::io::access_permission::none)
                {
                    PMC_UNUSED(fd);
                    PMC_UNUSED(path_str);
                    PMC_UNUSED(cm);
                    PMC_UNUSED(perm);
                    return PILO_OK;
                }

                inline ::pilo::err_t finalize() { return PILO_OK;  }

                inline ::pilo::err_t lock_shared(::pilo::i64_t offset = 0, ::pilo::i64_t length = -1)
                { 
                    PMC_UNUSED(offset);
                    PMC_UNUSED(length);
                    return PILO_OK; 
                }

                inline ::pilo::err_t lock(::pilo::i64_t offset = 0, ::pilo::i64_t length = -1)
                { 
                    PMC_UNUSED(offset);
                    PMC_UNUSED(length);
                    return PILO_OK; 
                }

                inline ::pilo::err_t try_lock(::pilo::i64_t offset = 0, ::pilo::i64_t length = -1)
                { 
                    PMC_UNUSED(offset);
                    PMC_UNUSED(length);
                    return true; 
                }
                inline ::pilo::err_t try_lock_shared(::pilo::i64_t offset = 0, ::pilo::i64_t length = -1)
                { 
                    PMC_UNUSED(offset);
                    PMC_UNUSED(length);
                    return true;
                }

                inline ::pilo::err_t unlock(::pilo::i64_t offset, ::pilo::i64_t length)
                { 
                    PMC_UNUSED(offset);
                    PMC_UNUSED(length);
                    return PILO_OK; 
                }

                inline ::pilo::err_t unlock() { return PILO_OK; }

                PMC_DISABLE_COPY(dummy_file_lock)
            };

        }
    }
}


#endif