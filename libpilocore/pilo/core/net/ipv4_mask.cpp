#include "ipv4_mask.hpp"
#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"

namespace pilo
{
    namespace core
    {
        namespace net
        {

            bool ipv4_mask::is_valid() const
            {
                static ipv4_mask invalidMask = { 0xffffffff, 0xffffffff };
                return memcmp(this, &invalidMask, sizeof(ipv4_mask)) != 0;
            }

            bool ipv4_mask::match(pilo::u32_t ip) const
            {
                return (m_mask & ip) == m_target;
            }

            pilo::core::net::ipv4_mask ipv4_mask::build(const char* ipAddress)
            {
                char buf[32];
                pilo::core::string::string_util::copy(buf, sizeof(buf), ipAddress, MC_INVALID_SIZE);

                ipv4_mask ipMask = { 0xffffffff, 0xffffffff };

                char* abcd[4];
                size_t n = pilo::core::string::string_util::split(buf, '.', abcd, 4);
                if (n != 4)
                {
                    return ipMask;
                }

                pilo::core::string::fixed_astring<16> star("*");
                pilo::core::string::fixed_astring<16> sa = abcd[0];
                pilo::core::string::fixed_astring<16> sb = abcd[1];
                pilo::core::string::fixed_astring<16> sc = abcd[2];
                pilo::core::string::fixed_astring<16> sd = abcd[3];

                pilo::u32_t a = sa == star ? 0 : pilo::core::string::string_util::cstr_to_int32(sa.c_str());
                pilo::u32_t b = sb == star ? 0 : pilo::core::string::string_util::cstr_to_int32(sb.c_str());
                pilo::u32_t c = sc == star ? 0 : pilo::core::string::string_util::cstr_to_int32(sc.c_str());
                pilo::u32_t d = sd == star ? 0 : pilo::core::string::string_util::cstr_to_int32(sd.c_str());

                unsigned int ma = sa == star ? 0 : 255;
                unsigned int mb = sb == star ? 0 : 255;
                unsigned int mc = sc == star ? 0 : 255;
                unsigned int md = sd == star ? 0 : 255;

                ipMask.m_mask = MF_MAKE_IPV4_ADDRESS(ma, mb, mc, md);
                ipMask.m_target = MF_MAKE_IPV4_ADDRESS(a, b, c, d);

                return ipMask;
            }

        }
    }    
}