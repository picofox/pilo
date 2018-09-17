#pragma once
#include "core/coredefs.hpp"


#ifdef WINDOWS
#include <wtypes.h>
#include <Winternl.h>


typedef NTSTATUS(NTAPI *pfnNtQueryInformationProcess)(
    IN  HANDLE ProcessHandle,
    IN  PROCESSINFOCLASS ProcessInformationClass,
    OUT PVOID ProcessInformation,
    IN  ULONG ProcessInformationLength,
    OUT PULONG ReturnLength    OPTIONAL
    );

#endif

namespace pilo
{
    namespace core
    {
        namespace process
        {

            
        }
    }
}