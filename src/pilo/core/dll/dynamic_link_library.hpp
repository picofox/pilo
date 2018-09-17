#pragma once
#include "core/coredefs.hpp"

#if defined(WIN32) || defined(WINDOWS)

#else
#   include <dlfcn.h>
#endif

#define MB_LOAD_DLL_EXECUTABLE      (0x1)

namespace pilo
{
    namespace core
    {
        namespace dll
        {
            class dynamic_link_library
            {
             public:
                dynamic_link_library() : _m_dll_module_handle(NULL),_m_ref_count(0) {}
                ~dynamic_link_library();

                ::pilo::i32_t load(const char* name, ::pilo::u32_t flags);
                ::pilo::i32_t unload();

                inline void inc_ref_count() { _m_ref_count ++ ; }
                inline void dec_ref_count() { _m_ref_count -- ; }
                inline unsigned int ref_count() const { return _m_ref_count; }

                template<typename T>
                ::pilo::i32_t find_function(T* pfunc, const char*fname)
                {
                    if (fname == nullptr || *fname == 0 || pfunc == nullptr) return ::pilo::EC_NULL_PARAM;

                    if (_m_dll_module_handle == nullptr)
                    {
                        return ::pilo::EC_NULL_PTR;
                    }

#if defined(WIN32) || defined(WINDOWS)
                    *pfunc = (T) ::GetProcAddress(_m_dll_module_handle, fname);
                    if ((*pfunc) == NULL)
                    {
                        return MAKE_SYSERR(::pilo::EC_FIND_FUNCTION_ERROR);
                    }
#else
                    /*
                    The function dlsym() takes a "handle" of a dynamic library returned by dlopen() and the null-terminated symbol
                    name, returning the address where that symbol is loaded into memory. If the symbol is not found, in the specified
                    library or any of the libraries that were automatically loaded by dlopen() when that library was loaded, dlsym()
                    returns NULL. (The search performed by dlsym() is breadth first through the dependency tree of these libraries.)
                    Since the value of the symbol could actually be NULL (so that a NULL return from dlsym() need not indicate an error),
                    the correct way to test for an error is to call dlerror() to clear any old error conditions, then call dlsym(),
                    and then call dlerror() again, saving its return value into a variable, and check whether this saved value is not NULL.
                    */
                    ::dlerror();
                    *pfunc = (T) ::dlsym(_m_dll_module_handle, fname);
                    if (::dlerror() != NULL)
                    {
                        return MAKE_SYSERR(::pilo::EC_FIND_FUNCTION_ERROR);
                    }
#endif

                    return ::pilo::EC_OK;

                }

            private:
                M_DISABLE_COPY(dynamic_link_library);

            protected:
                os_dll_module_handle_t  _m_dll_module_handle;
                unsigned int            _m_ref_count;

            };
        }        
    }
}