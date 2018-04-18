#include "./process_util.hpp"
#include "core/dll/dynamic_link_library.hpp"

#ifdef WINDOWS
static const char* __stc_process_priv_name_list[] = { SE_BACKUP_NAME, SE_RESTORE_NAME, SE_CHANGE_NOTIFY_NAME, nullptr };
#endif

namespace pilo
{
    namespace core
    {
        namespace process
        {

            ::pilo::i32_t process_util::get_parent_process_id(::pilo::os_process_id_t& ppid)
            {
#ifdef  WINDOWS
                LONG                            status;
                PROCESS_BASIC_INFORMATION       pbi;
                HANDLE                          hProcess = 0;
                FILETIME                        tagCreationTimeChild, tagCreationTimeParent,tagExitTime, tagKerTime, tagUserTime;
                ::pilo::i32_t                   ret = ::pilo::EC_OK;

                ::pilo::core::dll::dynamic_link_library dll;
                ret = dll.load("ntdll.dll", 0); //load ntdll.dll
                if (ret != ::pilo::EC_OK)
                {
                    return MAKE_SYSERR(::pilo::EC_LOAD_DLL_ERROR);
                }

                pfnNtQueryInformationProcess pfqip;
                ret = dll.find_function<pfnNtQueryInformationProcess>(&pfqip, "NtQueryInformationProcess"); //find the func we need
                if (ret != ::pilo::EC_OK)
                {
                    return MAKE_SYSERR(::pilo::EC_FIND_FUNCTION_ERROR);
                }
                
                hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ::GetCurrentProcessId()); //should close
                if (!hProcess)
                {
                    return MAKE_SYSERR(::pilo::EC_OPEN_PROCESS_ERROR);
                }                    

                if (! ::GetProcessTimes(hProcess, &tagCreationTimeChild, &tagExitTime, &tagKerTime, &tagUserTime))
                {
                    ::CloseHandle(hProcess);
                    return MAKE_SYSERR(::pilo::EC_GET_PROCESS_TIME_ERROR);
                }

                status = pfqip(hProcess, ProcessBasicInformation, (PVOID)&pbi, sizeof(PROCESS_BASIC_INFORMATION), NULL);

                ::CloseHandle(hProcess); //close process handle first

                if (status >= 0)
                {
                    ppid = (::pilo::os_process_id_t) pbi.Reserved3; 

                    HANDLE   hProcessParent;
                    hProcessParent = ::OpenProcess(PROCESS_QUERY_INFORMATION, FALSE, ppid);
                    if (!hProcessParent)
                    {                    
                        return MAKE_SYSERR(::pilo::EC_OPEN_PROCESS_ERROR);
                    }                        

                    if (!::GetProcessTimes(hProcessParent, &tagCreationTimeParent, &tagExitTime, &tagKerTime, &tagUserTime))
                    {
                        ::CloseHandle(hProcessParent);
                        return MAKE_SYSERR(::pilo::EC_GET_PROCESS_TIME_ERROR);
                    }     
                    ::CloseHandle(hProcessParent);
            
                    LONG which_earlier = ::CompareFileTime(&tagCreationTimeParent, &tagCreationTimeChild);                    
                    if (which_earlier > 0) //parent > child
                    {
                        ppid = MC_INVALID_PROCESS_ID;
                    }
                }

#else
                ppid = getppid();       

#endif
                return ::pilo::EC_OK;
            }

            ::pilo::os_process_id_t process_util::current_process_id()
            {
#ifdef  WINDOWS
                return ::GetCurrentProcessId();
#else
                return getpid();
#endif
            }



            ::pilo::error_number_t process_util::set_current_process_privilege(EnumProcessPrivilegeName eWhich, bool enable)
            {
#ifdef WINDOWS
                BOOL fOk = FALSE;
                // Assume function fails    
                HANDLE hToken;
                // Try to open this process's access token    
                if (::OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken))
                {
                    // privilege        
                    TOKEN_PRIVILEGES tp = { 1 };

                    if (LookupPrivilegeValue(NULL, __stc_process_priv_name_list[(int)eWhich], &tp.Privileges[0].Luid))
                    {
                        tp.Privileges[0].Attributes = enable ? SE_PRIVILEGE_ENABLED : 0;

                        AdjustTokenPrivileges(hToken, FALSE, &tp,
                            sizeof(tp), NULL, NULL);

                        fOk = (GetLastError() == ERROR_SUCCESS);
                    }

                    CloseHandle(hToken);
                }



                if (!fOk) return MAKE_SYSERR(::pilo::EC_SET_PROCESS_PRIVILEGE_ERROR);
                
                return ::pilo::EC_OK;
#else
                return ::pilo::EC_OK;
#endif
                
            }

        }
    }
}