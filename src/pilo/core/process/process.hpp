#ifndef _process_hpp_
#define _process_hpp_

#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            os_pid_t xpf_current_process_id();
            os_pid_t xpf_parent_process_id(os_pid_t);
            os_pid_t xpf_current_parent_process_id();
            char* xpf_get_proc_name(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen);
            char* xpf_get_proc_basename(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen, const char* suf, ::pilo::i32_t len);
        }
    }
}


#endif