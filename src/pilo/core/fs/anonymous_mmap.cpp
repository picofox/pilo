#include "anonymous_mmap.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {
            ::pilo::error_number_t anonymous_mmap::map(size_t length, void* desired_start_address)
            {
                if (length == 0)
                {
                    return ::pilo::EC_NONSENSE_OPERATION;
                }

#ifdef WINDOWS
                DWORD hi = 0;
                DWORD lo = 0;
                if (length >= 0)
                {
                    hi = M_HI32BIT(length);
                    lo = M_LO32BIT(length);
                }

                _m_win32_map_handle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, hi, lo, NULL);
                if (NULL == _m_win32_map_handle)
                {
                    return ::pilo::EC_CREATE_FILE_MAP_ERROR;
                }

                _m_address = MapViewOfFileEx(_m_win32_map_handle, FILE_MAP_READ | FILE_MAP_WRITE, 0, 0, 0, desired_start_address);
                ::CloseHandle(_m_win32_map_handle);
                _m_win32_map_handle = NULL;

#else
                _m_address = mmap(desired_start_address, length, PROT_EXEC|PROT_READ|PROT_EXEC, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
#endif

                if (_m_address == MC_MAP_FAILED_INDICATOR)
                {
                    return ::pilo::EC_MAP_FAILED;
                }

                _m_address_required = desired_start_address;
                _m_length = length;

                return ::pilo::EC_OK;
            }

            ::pilo::error_number_t anonymous_mmap::unmap()
            {
                if (_m_address != MC_MAP_FAILED_INDICATOR)
                {
#ifdef WINDOWS
                    BOOL unmap_ret = UnmapViewOfFile(_m_address);
                    if (!unmap_ret) return ::pilo::EC_UNMAP_FAILED;
#else
                    int unmap_ret = munmap(_m_address, _m_length);
                    if (unmap_ret != 0) return ::pilo::EC_UNMAP_FAILED;
#endif
                } 

                _m_address_required = nullptr;
                _m_length = 0;
                _m_address = nullptr;

                return ::pilo::EC_OK;
            }

        }
    }
}