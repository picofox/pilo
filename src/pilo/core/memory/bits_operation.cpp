#include "../../platform.hpp"
#include "bits_operation.hpp"

namespace pilo
{
	namespace core
	{
		namespace memory
		{
			void bits_operation::switch_endian(unsigned short& value, bool to_big)
			{
				if (PMC_ENDIANNESS == eBigEndian)
				{
					if (!to_big)
					{
						value = ntohs(value);
					}
				}
				else
				{
					if (to_big)
					{
						value = htons(value);
					}
				}
			}

			void bits_operation::switch_endian(unsigned int& value, bool to_big)
			{
				if (PMC_ENDIANNESS == eBigEndian)
				{
					if (!to_big)
					{
						value = ntohl(value);
					}
				}
				else
				{
					if (to_big)
					{
						value = htonl(value);
					}
				}
			}

			void bits_operation::switch_endian(unsigned long long& value, bool to_big)
			{
				if (PMC_ENDIANNESS == eBigEndian)
				{
					if (!to_big)
					{
						value = ntohll(value);
					}
				}
				else
				{
					if (to_big)
					{
						value = htonll(value);
					}
				}
			}

			void bits_operation::switch_endian(short& value, bool to_big)
			{
				if (PMC_ENDIANNESS == eBigEndian)
				{
					if (!to_big)
					{
						value = ntohs(value);
					}
				}
				else
				{
					if (to_big)
					{
						value = htons(value);
					}
				}
			}

			void bits_operation::switch_endian(int& value, bool to_big)
			{
				if (PMC_ENDIANNESS == eBigEndian)
				{
					if (!to_big)
					{
						value = ntohl(value);
					}
				}
				else
				{
					if (to_big)
					{
						value = htonl(value);
					}
				}
			}

			void bits_operation::switch_endian(long long& value, bool to_big)
			{
				if (PMC_ENDIANNESS == eBigEndian)
				{
					if (!to_big)
					{
						value = ntohll(value);
					}
				}
				else
				{
					if (to_big)
					{
						value = htonll(value);
					}
				}
			}


		}
	}
}