#include "system_info.hpp"

void pilo::core::info::system_info::_get_mmap_granuity()
{
#ifdef WINDOWS
	SYSTEM_INFO tagSystemInf;
	GetSystemInfo(&tagSystemInf);
	_mmap_granuity = tagSystemInf.dwAllocationGranularity;

#else
	_mmap_granuity = getpagesize();
#endif

	if (_mmap_granuity <= 0)
	{
		pilo_set_flag_bit_by_index<::pilo::u64_t>(_flags, MB_SYSINFO_FLAG_MMAP_GRAN, true);
	}
}

size_t pilo::core::info::system_info::_mmap_granuity = 0;
size_t pilo::core::info::system_info::_flags = 0;

