#pragma once
#include "core/coredefs.hpp"


namespace pilo
{
    namespace core
    {
        namespace infomation
        {
            void pilo_get_cpuidex(unsigned int CPUInfo[4], unsigned int InfoType, unsigned int ECXValue);
            void pilo_get_cpuid(unsigned int CPUInfo[4], unsigned int InfoType);
            int pilo_get_cpubrand(char* pbrand);
            int pilo_get_cpu_vendor(char* pvendor);            
        }
    }
}