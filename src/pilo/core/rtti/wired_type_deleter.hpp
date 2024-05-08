#pragma once

#include "../../pilo.hpp"

namespace pilo
{
    class tlv;
	namespace core
	{
		namespace rtti
		{

			void delete_tlv(::pilo::tlv* t);
			

			template <typename T>
			inline void wired_type_deleter(T)
			{
				return;
			}

			template <>
			inline void wired_type_deleter(char* s)
			{
				PMF_HEAP_FREE((void*)s);
			}



			template <>
			inline void wired_type_deleter(::pilo::tlv * t)
			{
				delete_tlv(t);
			}

		}
	}
}