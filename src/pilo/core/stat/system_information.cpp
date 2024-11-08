#include <iomanip>
#include <sstream>
#include "../../pilo.hpp"
#include "system_information.hpp"
#include "../datetime/datetime.hpp"


#ifdef WINDOWS
#include <sysinfoapi.h>

DWORD CountSetBits(ULONG_PTR bitMask)
{
    DWORD LSHIFT = sizeof(ULONG_PTR) * 8 - 1;
    DWORD bitSetCount = 0;
    ULONG_PTR bitTest = (ULONG_PTR)1 << LSHIFT;
    DWORD i;

    for (i = 0; i <= LSHIFT; ++i)
    {
        bitSetCount += ((bitMask & bitTest) ? 1 : 0);
        bitTest /= 2;
    }

    return bitSetCount;
}

int win32_get_cpu_info(::pilo::i32_t& ncpus, ::pilo::i32_t& ncores, ::pilo::i32_t & nlcores)
{
    BOOL done = FALSE;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = NULL;
    PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr = NULL;
    DWORD returnLength = 0;
    DWORD logicalProcessorCount = 0;
    DWORD numaNodeCount = 0;
    DWORD processorCoreCount = 0;
    DWORD processorL1CacheCount = 0;
    DWORD processorL2CacheCount = 0;
    DWORD processorL3CacheCount = 0;
    DWORD processorPackageCount = 0;
    DWORD byteOffset = 0;
    PCACHE_DESCRIPTOR Cache;

    while (!done)
    {
        DWORD rc = GetLogicalProcessorInformation(buffer, &returnLength);

        if (FALSE == rc)
        {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
            {
                if (buffer)
                    free(buffer);

                buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(
                    returnLength);

                if (NULL == buffer)
                {
                    _tprintf(TEXT("\nError: Allocation failure\n"));
                    return (2);
                }
            }
            else
            {
                _tprintf(TEXT("\nError %d\n"), GetLastError());
                return (3);
            }
        }
        else
        {
            done = TRUE;
        }
    }

    ptr = buffer;
    if (ptr == NULL) 
    {
        return ::pilo::mk_perr(PERR_NULL_PTR);
    }
    while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
    {
        switch (ptr->Relationship)
        {
        case RelationNumaNode:
            // Non-NUMA systems report a single record of this type.
            numaNodeCount++;
            break;

        case RelationProcessorCore:
            processorCoreCount++;

            // A hyperthreaded core supplies more than one logical processor.
            logicalProcessorCount += CountSetBits(ptr->ProcessorMask);
            break;

        case RelationCache:
            // Cache data is in ptr->Cache, one CACHE_DESCRIPTOR structure for each cache. 
            Cache = &ptr->Cache;
            if (Cache->Level == 1)
            {
                processorL1CacheCount++;
            }
            else if (Cache->Level == 2)
            {
                processorL2CacheCount++;
            }
            else if (Cache->Level == 3)
            {
                processorL3CacheCount++;
            }
            break;

        case RelationProcessorPackage:
            // Logical processors share a physical package.
            processorPackageCount++;
            break;

        default:
            _tprintf(TEXT("\nError: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.\n"));
            break;
        }
        byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
        ptr++;
    }

    ncpus = (::pilo::i32_t)processorPackageCount;
    ncores = (::pilo::i32_t)processorCoreCount;
    nlcores = (::pilo::i32_t)logicalProcessorCount;   


    free(buffer);
    return 0;
}

#elif defined(LINUX)
#include <iostream>
#include <fstream>
#include <string>
#include <unordered_set>
#include "../datetime/timestamp.hpp"
#include "../string/string_operation.hpp"
#include <thread>


int linux_get_cpu_info(::pilo::i32_t& ncpus, ::pilo::i32_t& ncores, ::pilo::i32_t& nlcores, ::pilo::i64_t & freq)
{
    int threadCount = 0; // 超线程数
    std::unordered_set<int> sss;
    std::ifstream file("/proc/cpuinfo");
    std::string line;
    while (getline(file, line)) {
        if (line.find("physical id") != std::string::npos) {
            size_t pos = line.find(':');
            int cpuid = atoi(&line.c_str()[pos + 2]);
            sss.insert(cpuid);
        }
        else if (line.find("cpu MHz") != std::string::npos)
        {
            ::pilo::i64_t pos_freq = line.find(':') + 2;
            ::pilo::f64_t ff = 0.0f;
            ::pilo::core::string::string_to_number(ff, line.c_str() + pos_freq, -1);
            freq = (::pilo::i64_t) ff * 1000;
        }
        else if (line.find("siblings") != std::string::npos) {
            size_t pos = line.find(':');
            if (pos != std::string::npos && isdigit(line[pos + 2])) {
                threadCount = atoi(&line.c_str()[pos + 2]);
            }
        }
    }

    ncpus = sss.size();
    ncores = sysconf(_SC_NPROCESSORS_ONLN);
    nlcores = threadCount;


    return 0;

}

#endif

namespace pilo
{
    namespace core
    {
        namespace stat
        {
            core::stat::system_information::system_information() 
                : _num_cpus(0)
                , _num_cores(0)
                , _num_logical_processors(0)
                , _page_size(0)
                , _freq(0)

            {
#ifdef WINDOWS
                win32_get_cpu_info(this->_num_cpus, this->_num_cores, this->_num_logical_processors);

                LARGE_INTEGER li;
                QueryPerformanceFrequency(&li);
                this->_freq = li.QuadPart;
                SYSTEM_INFO systemInfo;
                GetSystemInfo(&systemInfo);
                this->_page_size = systemInfo.dwPageSize;

#elif defined(LINUX)

                linux_get_cpu_info(this->_num_cpus, this->_num_cores, this->_num_logical_processors, this->_freq);
                this->_page_size = sysconf(_SC_PAGESIZE);




#else
                error("unsupport OS");

#endif
                this->_system_timezone = (::pilo::i8_t) (0 - (::pilo::core::datetime::xpf_timezone_offset_seconds() / 3600LL));
            }
            std::string system_information::to_string() const
            {
                ::std::stringstream ss;
                ss << "system information:" << std::endl;
                ss << "\tNumber Of CPUs: " << this->_num_cpus << std::endl;
                ss << "\tNumber Of Cores: " << this->_num_cores << std::endl;
                ss << "\tNumber Of LP: " << this->_num_logical_processors << std::endl;
                ss << "\tPage Size: " << this->page_size() << std::endl;
                ss << "Virtual Frequency:" << this->_freq << std::endl;

                return ss.str();;
            }
        }
    }
}