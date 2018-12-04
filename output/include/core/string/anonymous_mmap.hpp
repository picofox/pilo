#pragma once
#include "core/info/system_info.hpp"
#include "core/fs/file.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {            
            class anonymous_mmap
            {
            public:
                anonymous_mmap() : _m_priv(PiloGenericPrivillegeEnumeration::eGPE_None)
                    , _m_address(nullptr)
                    , _m_address_required(nullptr)
                    , _m_length(0)
                {
#ifdef WINDOWS
                    _m_win32_map_handle = NULL;
#endif
                }

                ~anonymous_mmap()
                {
                    finalize_nolock();
                }

                ::pilo::error_number_t initialize(
                    size_t length,
                    PiloGenericPrivillegeEnumeration ePriv);
                ::pilo::error_number_t finalize_nolock();
                ::pilo::error_number_t map(void ** pptr, PiloGenericPrivillegeEnumeration ePriv, size_t length, void* desired_start_address);
                ::pilo::error_number_t unmap();

            protected:
                

            protected:
                PiloGenericPrivillegeEnumeration    _m_priv;
                void*								_m_address;
                void*							    _m_address_required;
                size_t								_m_length;
#ifdef WINDOWS
                HANDLE                              _m_win32_map_handle;
#endif
            };
        }
    }
}
