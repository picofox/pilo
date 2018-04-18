#pragma once
#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace random
        {
            class simple_random
            {
            public:
                simple_random();
                simple_random(::pilo::u32_t seed);
                ~simple_random();

                ::pilo::u32_t generate_u32();
                unsigned int generate_u32(::pilo::u32_t beg, ::pilo::u32_t end);
                float generate_float(::pilo::u32_t lessthan);
                double generate_double(::pilo::u32_t lessthan);

            private:
                ::pilo::u32_t m_seed;
            };
        }
    }
}