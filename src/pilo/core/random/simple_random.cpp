#include "simple_random.hpp"
#include "core/datetime/datetime.hpp"

namespace pilo
{
    namespace core
    {
        namespace random
        {


            simple_random::simple_random() : m_seed((unsigned int)(time(NULL) * PiloGetTickCount32()))
            {

            }

            simple_random::simple_random(::pilo::u32_t seed) : m_seed(seed)
            {

            }

            simple_random::~simple_random()
            {

            }

            ::pilo::u32_t simple_random::generate_u32()
            {
                return (m_seed = m_seed * 214013L + 2531011L);
            }

            unsigned int simple_random::generate_u32(::pilo::u32_t beg, ::pilo::u32_t end)
            {
                if (beg == end) return beg;
                unsigned int n = this->generate_u32() & 0x7fffffff;
                return n % (end - beg) + beg;
            }

            float simple_random::generate_float(::pilo::u32_t lessthan)
            {
                return generate_u32() / (float)(UINT_MAX / (0 == lessthan ? 1.0f : lessthan));
            }

            double simple_random::generate_double(::pilo::u32_t lessthan)
            {
                return generate_u32() / (double)(UINT_MAX / (0 == lessthan ? 1.0f : lessthan));
            }

        }
    }
}