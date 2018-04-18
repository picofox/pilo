#include "./dynamic_link_library.hpp"

namespace pilo
{
    namespace core
    {
        namespace dll
        {
            dynamic_link_library::~dynamic_link_library()
            {
                if (_m_dll_module_handle)
                {
                    unload();
                }
            }

            ::pilo::i32_t dynamic_link_library::load(const char* name, ::pilo::u32_t flags)
            {
                os_dll_module_handle_t dll_module_handle = MC_INVALID_DLL_MODULE_HANDLE;

#if defined(WIN32) || defined(WINDOWS)
                M_UNUSED(flags);
                if (name == nullptr || *name == 0)
                {
                    return ::pilo::EC_NULL_PARAM;
                }
                dll_module_handle = ::LoadLibrary(_T(name));
#else
                ::dlerror();
                if (flags & MB_LOAD_DLL_EXECUTABLE)
                {
                    dll_module_handle = ::dlopen(NULL, RTLD_NOW);
                }
                else
                {
                    if (name == nullptr || *name == 0)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }
                    dll_module_handle = ::dlopen(name, RTLD_NOW);
                }
                if (::dlerror() != NULL)
                {
                    return MAKE_SYSERR(::pilo::EC_LOAD_DLL_ERROR);
                }

#endif 
                if (dll_module_handle == NULL)
                {
                    return MAKE_SYSERR(::pilo::EC_LOAD_DLL_ERROR);
                }

                if (_m_dll_module_handle != nullptr)
                {
                    unload();
                }

                _m_dll_module_handle = dll_module_handle;

                return ::pilo::EC_OK;
            }

            ::pilo::i32_t dynamic_link_library::unload()
            {
                if (_m_dll_module_handle == nullptr)
                {
                    return ::pilo::EC_NONSENSE_OPERATION;
                }
#if defined(WIN32) || defined(WINDOWS)
                if (!::FreeLibrary(_m_dll_module_handle))
                {
                    return MAKE_SYSERR(::pilo::EC_UNLOAD_DLL_ERROR);
                }
#else
                if (0 != ::dlclose(_m_dll_module_handle))
                {
                    return MAKE_SYSERR(::pilo::EC_UNLOAD_DLL_ERROR);
                }
#endif
                _m_dll_module_handle = nullptr;
                return ::pilo::EC_OK;
            }
        }      
    }
}