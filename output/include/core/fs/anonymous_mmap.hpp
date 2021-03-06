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
                anonymous_mmap() : 
                     _m_address(nullptr)
                    , _m_address_required(nullptr)
                    , _m_length(0)
                {
#ifdef WINDOWS
                    _m_win32_map_handle = NULL;
#endif
                }

                anonymous_mmap(size_t length, void* desired_start_address = nullptr) :
                    _m_address(nullptr)
                    , _m_address_required(nullptr)
                    , _m_length(0)
                {
#ifdef WINDOWS
                    _m_win32_map_handle = NULL;
#endif

                    map(length, desired_start_address);
                }

                ~anonymous_mmap()
                {
                    unmap();
                }

                ::pilo::error_number_t map(size_t length, void* desired_start_address = nullptr);
                ::pilo::error_number_t unmap();
                void* mapped_ptr() { return _m_address;  }
                inline size_t length() const { return _m_length;  }

            protected:
                

            protected:
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
