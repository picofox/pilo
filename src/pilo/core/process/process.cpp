#include "process.hpp"
#include "../memory/util.hpp"
#include "../string/string_operation.hpp"

#ifdef WINDOWS
#include <windows.h>
#include <winternl.h>

#else
extern char** environ;
#endif

#include <cstdlib>

namespace pilo
{
    namespace core
    {
        namespace process
        {
#define     PMI_INVALID_PROCESS_ID      ((::pilo::os_pid_t) -1)

#ifdef WINDOWS
            char* xpf_get_proc_name(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen)
            {               
                char tmp_buffer[PMI_PATH_MAX] = { 0 };
                ::pilo::u32_t len = GetModuleFileName(NULL, tmp_buffer, sizeof(tmp_buffer));
                ::pilo::char_buffer_t   cb(buffer, bufsz, 0, false);

                if (len > 0) {
                    cb.check_space(len + 1);
                    const char* lastSlash = ::pilo::core::string::rfind_char(tmp_buffer, len, '\\');
                    if (lastSlash != nullptr) {
                        ::pilo::core::string::copyz(cb.begin(), cb.capacity(), lastSlash + 1);
                        ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)len - (::pilo::i32_t)(lastSlash - tmp_buffer + 1));
                        return cb.begin();
                    }
                    else {
                        ::pilo::core::string::copyz(cb.begin(), cb.capacity(), tmp_buffer);
                        ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)len);
                        return cb.begin();
                    }
                }
                else {
                    return nullptr;
                }

            }

            char* xpf_get_proc_basename(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen, const char* suffix, ::pilo::i32_t suffix_len)
            {
                char tmp_buffer[PMI_PATH_MAX] = { 0 };
                ::pilo::u32_t len = GetModuleFileName(NULL, tmp_buffer, sizeof(tmp_buffer));
                ::pilo::char_buffer_t   cb(buffer, bufsz, 0, false);
                ::pilo::i32_t delta = len;
                if (suffix != nullptr && suffix_len < 0)
                    suffix_len = (::pilo::i32_t) ::pilo::core::string::character_count(suffix);


                if (len > 0) {
                    cb.check_space(len + 1 + suffix_len);
                    const char* lastSlash = ::pilo::core::string::rfind_char(tmp_buffer, len, '\\');
                    if (lastSlash != nullptr) {
                        ::pilo::core::string::copyz(cb.begin(), cb.capacity(), lastSlash + 1);
                        delta = (::pilo::i32_t)len - (::pilo::i32_t)(lastSlash - tmp_buffer + 1);
                        ::pilo::set_if_ptr_is_not_null(rlen, delta);
                    }
                    else {
                        delta = len;
                        ::pilo::core::string::copyz(cb.begin(), cb.capacity(), tmp_buffer);
                        ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)len);
                    }

                    const char* firstdot = ::pilo::core::string::find_char(cb.begin(), delta, '.');
                    if (firstdot != nullptr) {
                        ::pilo::i32_t base_len = (::pilo::i32_t)(firstdot - cb.begin());
                        cb.set_value(base_len, 0);
                        cb.set_size(base_len);
                        ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), suffix, suffix_len);
                        cb.set_value(base_len + suffix_len, 0);
                        cb.set_size(base_len + suffix_len);

                        ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)(base_len + suffix_len));
                    }
                    return cb.begin();

                }
                else {
                    return nullptr;
                }
            }

            char* xpf_get_proc_fullbasename(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen, const char* suffix, ::pilo::i32_t suffix_len)
            {
                char tmp_buffer[PMI_PATH_MAX] = { 0 };
                ::pilo::u32_t len = GetModuleFileName(NULL, tmp_buffer, sizeof(tmp_buffer));
                ::pilo::char_buffer_t   cb(buffer, bufsz, 0, false);
                ::pilo::i32_t delta = len;
                if (suffix != nullptr && suffix_len < 0)
                    suffix_len = (::pilo::i32_t) ::pilo::core::string::character_count(suffix);


                if (len > 0) {
                    cb.check_space(len + 1 + suffix_len);
                    delta = len;
                    ::pilo::core::string::copyz(cb.begin(), cb.capacity(), tmp_buffer);
                    ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)len);                

                    const char* firstdot = ::pilo::core::string::find_char(cb.begin(), delta, '.');
                    if (firstdot != nullptr) {
                        ::pilo::i32_t base_len = (::pilo::i32_t)(firstdot - cb.begin());
                        cb.set_value(base_len, 0);
                        cb.set_size(base_len);
                        ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), suffix, suffix_len);
                        cb.set_value(base_len + suffix_len, 0);
                        cb.set_size(base_len + suffix_len);

                        ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)(base_len + suffix_len));
                    }
                    return cb.begin();

                }
                else {
                    return nullptr;
                }
            }

            ::pilo::err_t xpf_iterate_enviroment_variable(env_iter_func_type func, void* ctx, bool ignore_err)
            {
                char** env = *__p__environ();
                while (env != nullptr) 
                {
                    ::pilo::i64_t tmplen = ::pilo::core::string::character_count(*env);
                    if (tmplen < 0) {
                        if (!ignore_err) {
                            return ::pilo::mk_perr(PERR_NULL_PTR);
                        }
                        continue;
                    }

                    const char* delim = ::pilo::core::string::find_char(*env, tmplen,  '=');
                    if (delim == nullptr) {
                        if (!ignore_err) {
                            return ::pilo::mk_perr(PERR_NULL_PTR);
                        }
                        continue;
                    }
                    ::pilo::i32_t key_len = (::pilo::i32_t)(delim - *env);
                    if (key_len <= 0) {
                        if (!ignore_err) {
                            return ::pilo::mk_perr(PERR_INV_LEN);
                        }
                        continue;
                    }
                    const char* val = delim + 1;
                    ::pilo::err_t err = func(*env, key_len, val, (::pilo::i32_t) (tmplen - key_len - 1), ctx);
                    if (err != PILO_OK) {
                        if (!ignore_err) {
                            return err;
                        }
                        continue;
                    }

                    env++;
                }
                return PILO_OK;
            }

            ::pilo::err_t xpf_set_environment_variable(const char* name, const char* value)
            {
                if (!SetEnvironmentVariable(name, value))
                    return ::pilo::mk_err(PERR_SET_ENV_FAIL);
                return PILO_OK;
            }

            ::pilo::err_t xpf_unset_environment_variable(const char* name)
            {
                if (!SetEnvironmentVariable(name, nullptr))
                    return ::pilo::mk_err(PERR_SET_ENV_FAIL);
                return PILO_OK;
            }

            os_pid_t xpf_current_process_id()
            {
                return ::GetCurrentProcessId();
            }

            os_pid_t xpf_parent_process_id(os_pid_t pid)
            {
                LONG                      status;
                os_pid_t                     dwParentPID = PMI_INVALID_PROCESS_ID;
                HANDLE                    hProcess;
                PROCESS_BASIC_INFORMATION pbi = {0};

                typedef LONG(WINAPI* PROCNTQSIP)(HANDLE, UINT, PVOID, ULONG, PULONG);
                PROCNTQSIP NtQueryInformationProcess;

                NtQueryInformationProcess = (PROCNTQSIP)GetProcAddress(GetModuleHandle("ntdll.dll"), "NtQueryInformationProcess");

                if (!NtQueryInformationProcess)
                    return PMI_INVALID_PROCESS_ID;

                // Get process handle  
                hProcess = OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, pid);
                if (!hProcess)
                    return (DWORD) PMI_INVALID_PROCESS_ID;

                // Retrieve information  
                status = NtQueryInformationProcess(hProcess,
                    ProcessBasicInformation,
                    (PVOID)&pbi,
                    sizeof(PROCESS_BASIC_INFORMATION),
                    NULL
                );

                // Copy parent Id on success  
                if (!status)
                    dwParentPID = (DWORD) ((::pilo::i64_t) pbi.Reserved3);

                CloseHandle(hProcess);

                return dwParentPID;
            }

            os_pid_t xpf_current_parent_process_id()
            {
                return xpf_parent_process_id(xpf_current_process_id());
            }

#else
            os_pid_t xpf_current_process_id()
            {
                return ::getpid();
            }

            os_pid_t xpf_parent_process_id(os_pid_t pid)
            {
                return -1;
            }

            os_pid_t xpf_current_parent_process_id()
            {
                return ::getppid();
            }

            char* xpf_get_proc_name(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen)
            {
                char filename[PMI_PATH_MAX] = { 0 };
                ::pilo::char_buffer_t   cb(buffer, bufsz, 0, false);
                ssize_t len = readlink("/proc/self/exe", filename, sizeof(filename) - 1);
                if (len < 0 || (size_t) len >= sizeof(filename)) {
                    return nullptr;
                }
                filename[len] = '\0';
                const char* p0 = ::pilo::core::string::rfind_char(filename, len, '/');
                const char* p1 = ::pilo::core::string::rfind_char(filename, len, '\\');
                if (p0 == nullptr && p1 == nullptr) {
                    p0 = filename;
                } else if (p0 == nullptr) {
                    p0 = p1 + 1;
                } else if (p1 == nullptr) {
                    p0 ++;
                } else {
                    if (p0 < p1) {
                        p0 = p1 + 1;
                    } else {
                        p0 ++;
                    }
                }
                len = len - (p0 - filename);
                cb.check_space((int) (len + 1));
                ::pilo::core::string::copyz(cb.begin(), cb.capacity(), p0);
                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)len);
                return cb.begin();
            }

            char* xpf_get_proc_basename(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen, const char* suffix, ::pilo::i32_t suffix_len)
            {
                char filename[PMI_PATH_MAX] = { 0 };
                ::pilo::char_buffer_t   cb(buffer, bufsz, 0, false);
                ssize_t len = readlink("/proc/self/exe", filename, sizeof(filename) - 1);
                if (len < 0 || (size_t) len >= sizeof(filename)) {
                    return nullptr;
                }
                filename[len] = '\0';
                const char* p0 = ::pilo::core::string::rfind_char(filename, len, '/');
                const char* p1 = ::pilo::core::string::rfind_char(filename, len, '\\');
                if (p0 == nullptr && p1 == nullptr) {
                    p0 = filename;
                } else if (p0 == nullptr) {
                    p0 = p1 + 1;
                } else if (p1 == nullptr) {
                    p0 ++;
                } else {
                    if (p0 < p1) {
                        p0 = p1 + 1;
                    } else {
                        p0 ++;
                    }
                }
                len = len - (p0 - filename);
                if (suffix == nullptr)
                    suffix_len = 0;
                else if (suffix_len < 0)
                    suffix_len = (int) ::pilo::core::string::character_count(suffix);
                cb.check_space((int) (len + suffix_len + 1));
                ::pilo::core::string::copyz(cb.begin(), cb.capacity(), p0);
                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)len);

                const char * firstdot = ::pilo::core::string::find_char(cb.begin(), len, '.');
                if (firstdot != nullptr) {
                    auto base_len = (::pilo::i32_t)(firstdot - cb.begin());
                    cb.set_value(base_len, 0);
                    cb.set_size(base_len);
                    ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), suffix, suffix_len);
                    cb.set_value(base_len + suffix_len, 0);
                    cb.set_size(base_len + suffix_len);
                    ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)(base_len + suffix_len));
                } else {
                    cb.set_value((int)len, 0);
                    cb.set_size((int)len);
                    ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), suffix, suffix_len);
                    cb.set_value((int)len + suffix_len, 0);
                    cb.set_size((int)len + suffix_len);
                    ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)(len + suffix_len));
                }
                return cb.begin();
            }

            char* xpf_get_proc_fullbasename(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen, const char* suffix, ::pilo::i32_t suffix_len)
            {
                char filename[PMI_PATH_MAX] = { 0 };
                ::pilo::char_buffer_t   cb(buffer, bufsz, 0, false);
                ssize_t len = readlink("/proc/self/exe", filename, sizeof(filename) - 1);
                if (len < 0 || (size_t)len >= sizeof(filename)) {
                    return nullptr;
                }
                filename[len] = '\0';
                if (suffix == nullptr)
                    suffix_len = 0;
                else if (suffix_len < 0)
                    suffix_len = (int) ::pilo::core::string::character_count(suffix);
                cb.check_space((int)(len + suffix_len + 1));
                ::pilo::core::string::copyz(cb.begin(), cb.capacity(), filename);
                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)len);

                const char* firstdot = ::pilo::core::string::find_char(cb.begin(), len, '.');
                if (firstdot != nullptr) {
                    auto base_len = (::pilo::i32_t)(firstdot - cb.begin());
                    cb.set_value(base_len, 0);
                    cb.set_size(base_len);
                    ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), suffix, suffix_len);
                    cb.set_value(base_len + suffix_len, 0);
                    cb.set_size(base_len + suffix_len);
                    ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)(base_len + suffix_len));
                }
                else {
                    cb.set_value((int)len, 0);
                    cb.set_size((int)len);
                    ::pilo::core::string::n_copyz(cb.ptr(), cb.space_available(), suffix, suffix_len);
                    cb.set_value((int)len + suffix_len, 0);
                    cb.set_size((int)len + suffix_len);
                    ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)(len + suffix_len));
                }
                return cb.begin();
            }

            ::pilo::err_t xpf_iterate_enviroment_variable(env_iter_func_type func, void* ctx, bool ignore_err)
            {
                char** env = environ;
                while (env != nullptr)
                {
                    ::pilo::i64_t tmplen = ::pilo::core::string::character_count(*env);
                    if (tmplen < 0) {
                        if (!ignore_err) {
                            return ::pilo::mk_perr(PERR_NULL_PTR);
                        }
                        continue;
                    }

                    const char* delim = ::pilo::core::string::find_char(*env, tmplen, '=');
                    if (delim == nullptr) {
                        if (!ignore_err) {
                            return ::pilo::mk_perr(PERR_NULL_PTR);
                        }
                        continue;
                    }
                    ::pilo::i32_t key_len = (::pilo::i32_t)(delim - *env);
                    if (key_len <= 0) {
                        if (!ignore_err) {
                            return ::pilo::mk_perr(PERR_INV_LEN);
                        }
                        continue;
                    }
                    const char* val = delim + 1;
                    ::pilo::err_t err = func(*env, key_len, val, (::pilo::i32_t)(tmplen - key_len - 1), ctx);
                    if (err != PILO_OK) {
                        if (!ignore_err) {
                            return err;
                        }
                        continue;
                    }

                    env++;
                }
                return PILO_OK;
            }

            ::pilo::err_t xpf_set_environment_variable(const char* name, const char* value)
            {
                if (0 != setenv(name, value, 1))
                    return ::pilo::mk_err(PERR_SET_ENV_FAIL);
                return PILO_OK;
            }

            ::pilo::err_t xpf_unset_environment_variable(const char* name)
            {
                if (0 != unsetenv(name))
                    return ::pilo::mk_err(PERR_SET_ENV_FAIL);
                return PILO_OK;
            }

#endif
        }
    }
}
