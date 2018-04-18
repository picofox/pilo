#pragma once
#include "core/coredefs.hpp"
#include "net_sys_header.hpp"



namespace pilo
{
    namespace core
    {
        namespace net
        {
            struct ipv4_mask
            {
                pilo::u32_t m_mask;		// язбК
                pilo::u32_t m_target;	// ip & mask == target

                bool is_valid() const;
                bool match(pilo::u32_t ip) const;

                static ipv4_mask build(const char* ipAddress);
            };

        }
    }    
}