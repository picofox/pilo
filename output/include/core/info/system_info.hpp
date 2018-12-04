#include "core/coredefs.hpp"

#define MB_SYSINFO_FLAG_MMAP_GRAN	(0)

namespace pilo
{
	namespace core
	{
		namespace info
		{
			class system_info
			{
			public:
				system_info()
				{
					_flags = 0xFFFFFFFFFFFFFFFF;					
				}

				inline static size_t mmap_granuity()
				{
					return _mmap_granuity;
				}

				inline static bool good() 
                { 
                    if (_flags == 0)
                    {
                        return true;
                    }

                    _flags = 0;
                    _calculate_mmap_granuity();
                    return (_flags == 0);
                }

			private:
				static void _calculate_mmap_granuity();

			private:
				static ::pilo::u64_t _flags;
				static size_t _mmap_granuity;
			};
		}
	}
} 