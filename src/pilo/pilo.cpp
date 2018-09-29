#include "pilo.hpp"
#include "core/info/system_info.hpp"

//return -1: sysinfo init failed

int pilo_initialize()
{
	if (! ::pilo::core::info::system_info::good())
	{
		return -1;
	}

	return 0;
}
