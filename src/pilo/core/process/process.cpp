#include "process.hpp"

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
            os_pid_t current_process_id()
            {
                return ::GetCurrentProcessId();
            }

            os_pid_t parent_process_id(os_pid_t pid)
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

            os_pid_t current_parent_process_id()
            {
                return parent_process_id(current_process_id());
            }

#else
            os_pid_t current_process_id()
            {
                return ::getpid();
            }

            os_pid_t parent_process_id(os_pid_t pid)
            {
                return -1;
            }

            os_pid_t current_parent_process_id()
            {
                return ::getppid();
            }

#endif
        }
    }
}
