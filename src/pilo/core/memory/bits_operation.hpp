#pragma once

#include "../../pilo.hpp"


namespace pilo
{
	namespace core
	{
		namespace memory
		{
			class bits_operation
			{
			public:
				template<typename INT_T>
				static INT_T fetch_nbits(INT_T v, unsigned int start, unsigned int len)
				{
					PMC_REGULATE_BETWEEN(start, 0, 0, sizeof(INT_T) * 8 - 1, sizeof(INT_T) * 8 - 1);
					PMC_REGULATE_BETWEEN(len, 1, 1, sizeof(INT_T) * 8 - start, sizeof(INT_T) * 8 - start);
					v >>= sizeof(INT_T) * 8 - ((INT_T)start + (INT_T)len);
					INT_T mask = 0;
					bits_operation::int_mask(mask, len);
					return (v & mask);
				}

				template<typename INT_T>
				static bool set_nbits(INT_T& v, unsigned int start, unsigned int len, INT_T v2)
				{
					PMC_NOT_IN_RANGE_RET(start, 0, sizeof(INT_T) * 8 - 1, false);
					PMC_NOT_IN_RANGE_RET(len, 1, sizeof(INT_T) * 8 - start, false);
					INT_T mask = 0;
					bits_operation::int_mask(mask, len);
					PMC_NOT_IN_RANGE_RET(v2, 0, mask, false);

					int pos = start + len;
					INT_T left = fetch_nbits(v, 0, start) << (sizeof(INT_T) * 8 - start);
					INT_T middle = (v2 & mask) << ((sizeof(INT_T) * 8) - pos);
					INT_T right = fetch_nbits(v, pos, sizeof(INT_T) * 8 - pos);

					v = left | middle | right;

					return true;
				}

				template<typename INT_T >
				static void int_mask(INT_T& ret, int n)
				{
					if (n == 0)
						ret = 0;
					else if (n >= (int)sizeof(INT_T) * 8)
						ret = (INT_T)~0;
					else
						ret = ((1LL << n) - 1);
				}

				static void switch_endian(unsigned short& value, bool to_big);
				static void switch_endian(unsigned int& value, bool to_big);
				static void switch_endian(unsigned long long& value, bool to_big);
				static void switch_endian(short& value, bool to_big);
				static void switch_endian(int& value, bool to_big);
				static void switch_endian(long long& value, bool to_big);
			};
		}
	}
}