#include "process.hpp"
#include "../memory/util.hpp"
#include "../string/string_operation.hpp"

#ifdef WINDOWS
#include <windows.h>
#include <winternl.h>
#endif

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

            os_pid_t xpf_current_process_id()
            {
                return ::GetCurrentProcessId();
            }

            os_pid_t xpf_parent_process_id(os_pid_t pid)
            {
                LONG                      status;
                os_pid_t                     dwParentPID = PMI_INVALID_PROCESS_ID;
                HANDLE                    hProcess;
                PROCESS_BASIC_INFORMATION pbi;

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
                int fd = -1;
                fd = open("/proc/self/comm", O_RDONLY);
                ssize_t n = ::read(fd, filename, sizeof(filename));
                if (n < 0) {
                    ::close(fd);
                    return nullptr;
                }
                int len = (int) ::pilo::core::string::character_count(filename);
                cb.check_space(len + 1);
                ::pilo::core::string::copyz(cb.begin(), cb.capacity(), filename);
                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)len);
                return cb.begin();
            }

            char* xpf_get_proc_basename(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen, const char* suffix, ::pilo::i32_t suffix_len)
            {
                char filename[PMI_PATH_MAX] = { 0 };
                ::pilo::char_buffer_t   cb(buffer, bufsz, 0, false);
                int fd = -1;
                fd = open("/proc/self/comm", O_RDONLY);
                ssize_t n = ::read(fd, filename, sizeof(filename));
                if (n < 0) {
                    ::close(fd);
                    return nullptr;
                }
                int len = (int) ::pilo::core::string::character_count(filename);
                cb.check_space(len + 1);
                ::pilo::core::string::copyz(cb.begin(), cb.capacity(), filename);
                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t)len);

                const char* firstdot = ::pilo::core::string::find_char(cb.begin(), len, '.');
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

#endif
        }
    }
}
