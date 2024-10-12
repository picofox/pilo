#include "threading.hpp"


#ifdef WINDOWS
	void pilo::core::threading::xpf_sleep(::pilo::i64_t secs)
	{
		::Sleep((DWORD)(secs * 1000));
	}

	void pilo::core::threading::xpf_msleep(::pilo::i64_t msecs)
	{
		::Sleep((DWORD)msecs);
	}

	void pilo::core::threading::xpf_usleep(::pilo::i64_t usecs)
	{
		::Sleep(DWORD(usecs / 1000) + 1);
	}


#else
	void pilo::core::threading::xpf_sleep(::pilo::i64_t secs)
	{
		::sleep(secs);
	}

	void pilo::core::threading::xpf_msleep(::pilo::i64_t msecs)
	{
		::usleep(msecs*1000);
	}

	void pilo::core::threading::xpf_usleep(::pilo::i64_t usecs)
	{
		::usleep(usecs);
	}

#endif