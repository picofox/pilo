#pragma once
#include "../../pilo.hpp"

namespace pilo {
    namespace core {
        namespace algorithm {

            template<typename T>
            const T* find_reversely_in_array_by_value(const T* c, ::pilo::i64_t length, const T& v)
            {
                if (c == nullptr) return nullptr;

                for (::pilo::i64_t i = (::pilo::i64_t) length - 1; i >= 0; i--)
                {
                    if (c[i] == v)
                    {
                        return c+i;
                    }
                }

                return nullptr;
            }

            template<typename T>
            const T* find_in_array_by_value(const T* c, ::pilo::i64_t length, const T& v)
            {
                if (c == nullptr) return nullptr;

                for (::pilo::i64_t i = 0; i <length; i++)
                {
                    if (c[i] == v)
                    {
                        return c + i;
                    }
                }

                return nullptr;
            }

			template<typename T>
            ::pilo::i64_t replace_in_array_by_value(T* c, ::pilo::i64_t length, const T& v, const T& replacement, ::pilo::i64_t max_count)
			{
                ::pilo::i64_t rep_count = 0;
				if (c == nullptr) return -1;
                if (max_count == 0) return 0;
				for (::pilo::i64_t i = 0; i < length; i++)
				{
					if (c[i] == v)
					{
                        c[i] = replacement;
                        rep_count++;
                        if (rep_count >= max_count)
						    break;
					}
				}

				return rep_count;
			}

        }
    }
}