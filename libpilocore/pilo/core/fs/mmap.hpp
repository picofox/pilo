#pragma once
#include "core/fs/file.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {

#define MB_MMAP_PARAFLAG_ANONYMOUS  (1<<0)
                 

            struct mmap_parameter
            {
                mmap_parameter()
                {
                    m_offset = 0;
                    m_length = 0;
                    m_flags = 0;
                    m_priv = PiloGenericPrivillegeEnumeration::eGPE_None;
                }

                size_t                              m_offset;
                size_t                              m_length;
                ::pilo::u32_t                       m_flags; 
                PiloGenericPrivillegeEnumeration    m_priv;
            };

            template<typename _LOCK_TYPE = ::pilo::core::threading::dummy_read_write_lock>
            class mmap
            {
            public:
                typedef _LOCK_TYPE lock_type;


            public:
                mmap()
                {
                    memset(&_m_map_parameter, 0x00, sizeof(_m_map_parameter));
                    _m_file_size = MC_INVALID_SIZE;
                    _m_mapped_size = MC_INVALID_SIZE;
                    _m_mapped_ptr = nullptr;   
#ifdef WINDOWS
                    _m_win32_map_handle = nullptr;
#endif
                }

                ~mmap()
                {

                }

            protected:
                ::pilo::error_number_t _initialize_nolock(const char* path, DeviceAccessModeEnumeration eMode, PiloGenericPrivillegeEnumeration ePriv)
                {
                    if (path == nullptr || *path == 0)
                    {
                        _m_map_parameter.m_priv = ePriv;
                        pilo_set_flag_bit_by_value(_m_map_parameter.m_flags, MB_MMAP_PARAFLAG_ANONYMOUS);
                        return ::pilo::EC_OK;
                    }

                    ::pilo::error_number_t ret = _m_file.initialize(path, 0, nullptr);
                    if (ret != ::pilo::EC_OK)
                    {
                        return ::pilo::EC_INITIALIZE_FAILED;
                    }

                    if (ePriv == PiloGenericPrivillegeEnumeration::eGPE_None)
                    {
                        return  ::pilo::EC_NONSENSE_OPERATION;
                    }

                    DeviceRWModeEnumeration e_file_acc_prev = eDRWM_ReadWrite;
                    _m_map_parameter.m_priv = ePriv;

                    if (pilo_test_flag_bit_by_value(ePriv, eGPE_Write))
                    {
                        e_file_acc_prev = eDRWM_ReadWrite;
                    }
                    else
                    {
                        e_file_acc_prev = eDRWM_Read;
                    } 

                    if (_m_file.open(eMode, e_file_acc_prev, 0))
                    {
                        return ::pilo::EC_OPEN_FILE_FAILED;
                    }

#ifdef WINDOWS
                    _m_file_size = _m_file.get_file_size();
                    if (_m_file_size == MC_INVALID_SIZE)
                    {
                        return ::pilo::EC_GET_FILE_SZ_FAILED;
                    }
                    DWORD hi = 0;
                    DWORD lo = 0;
                    hi = M_HI32BIT(_m_file_size);
                    lo = M_LO32BIT(_m_file_size);
                    _m_win32_map_handle = ::CreateFileMapping(_m_file.file_descriptor(), NULL, flag, M_HI32BIT, M_LO32BIT, NULL);
                    if (!_m_win32_map_handle)
                    {
                        return ::pilo::EC_CREATE_FILE_MAP_ERROR;
                    }


#else


#endif

                    return ret;
                }




            protected:
                mmap_parameter                      _m_map_parameter;

                size_t                              _m_file_size;
                size_t                              _m_mapped_size;
                void *                              _m_mapped_ptr;
                
                lock_type                           _m_lock;
                ::pilo::core::fs::file<MC_PATH_MAX, ::pilo::core::threading::dummy_read_write_lock> _m_file;

#ifdef WINDOWS
                HANDLE                              _m_win32_map_handle;
#endif



            private:
                M_DISABLE_COPY
            };

        }
    }
}