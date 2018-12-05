#include "anonymous_mmap.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {

            ::pilo::error_number_t anonymous_mmap::initialize(size_t length, PiloGenericPrivillegeEnumeration ePriv)
            {
                if (ePriv == PiloGenericPrivillegeEnumeration::eGPE_None)
                {
                    return  ::pilo::EC_NONSENSE_OPERATION;
                }

                DeviceRWModeEnumeration e_file_acc_prev = eDRWM_ReadWrite;
                _m_priv = ePriv;

#ifdef WINDOWS
                DWORD win32_protect = PAGE_READONLY;
#endif
                if (pilo_test_flag_bit_by_value(ePriv, eGPE_Write))
                {

                    e_file_acc_prev = eDRWM_ReadWrite;
                    win32_protect = PAGE_READWRITE;
                }

                if (length == 0)
                {
                    return ::pilo::EC_NONSENSE_OPERATION;
                }

                _m_priv = ePriv;

#ifdef WINDOWS
                DWORD hi = 0;
                DWORD lo = 0;
                if (length >= 0)
                {
#ifdef __x86_64__
                    hi = M_HI32BIT(length);
#else
                    hi = 0;
#endif // __x86_64__

                    
                    lo = M_LO32BIT(length);
                }

                _m_win32_map_handle = ::CreateFileMapping(INVALID_HANDLE_VALUE, NULL, win32_protect, hi, lo, NULL);
                if (NULL == _m_win32_map_handle)
                {
                    return ::pilo::EC_CREATE_FILE_MAP_ERROR;
                }
#else
               
#endif	

                return ::pilo::EC_OK;
            }

            ::pilo::error_number_t anonymous_mmap::finalize_nolock()
            {
                if (_m_address != nullptr)
                {
                    unmap();
                }

#ifdef WINDOWS
                if (_m_win32_map_handle != NULL)
                {
                    ::CloseHandle(_m_win32_map_handle);
                    _m_win32_map_handle = NULL;
                }
#endif

                _m_priv = PiloGenericPrivillegeEnumeration::eGPE_None;
                _m_address = nullptr;
                _m_address_required = nullptr;
                _m_length = 0;

                return ::pilo::EC_OK;
            }

            ::pilo::error_number_t anonymous_mmap::map(void ** pptr, PiloGenericPrivillegeEnumeration ePriv, size_t length, void* desired_start_address)
            {
                if (pptr == nullptr)
                {
                    return ::pilo::EC_NULL_PARAM;
                }

                if (length == 0)
                {
                    length = ::pilo::EC_INVALID_PARAM;
                }
#ifdef WINDOWS
                DWORD priv = 0;
                if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Exec))
                {
                    priv |= FILE_MAP_EXECUTE;
                }
                if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Read))
                {
                    priv |= FILE_MAP_READ;
                }
                if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Write))
                {
                    priv |= FILE_MAP_WRITE;
                }

                _m_address = MapViewOfFileEx(_m_win32_map_handle, priv, 0, 0, length, desired_start_address);

#else
                int priv = PROT_NONE;
                if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Exec))
                {
                    priv |= PROT_EXEC;
                }
                if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Read))
                {
                    priv |= PROT_READ;
                }
                if (pilo_test_flag_bit_by_value(ePriv, PiloGenericPrivillegeEnumeration::eGPE_Write))
                {
                    priv |= PROT_WRITE;
                }


                _m_address = mmap(desired_start_address, length, priv, MAP_SHARED | MAP_ANONYMOUS, -1, mmap_offset);



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
#ifdef WINDOWS
                BOOL unmap_ret = UnmapViewOfFile(_m_address);
                if (!unmap_ret) return ::pilo::EC_UNMAP_FAILED;
#else
                int unmap_ret = munmap(_m_address, _m_length);
                if (unmap_ret != 0) return ::pilo::EC_UNMAP_FAILED;
#endif
                return ::pilo::EC_OK;
            }

        }
    }
}