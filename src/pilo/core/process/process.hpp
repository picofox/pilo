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


            typedef ::pilo::err_t(*env_iter_func_type) (const char* key, ::pilo::i32_t key_len, const char* val, ::pilo::i32_t val_len, void* ctx);
            ::pilo::err_t xpf_iterate_enviroment_variable(env_iter_func_type func, void* ctx, bool ignore_err);

        }
    }
}


#endif