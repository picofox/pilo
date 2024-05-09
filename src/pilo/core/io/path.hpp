#ifndef __pilo_io_path_hpp_
#define __pilo_io_path_hpp_

#include <string>
#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include "./formatted_io.hpp"
#include "../threading/native_mutex.hpp"
#include "../memory/compactable_autoreset_object_pool.hpp"

#define PMI_STC_PARAM_PATH_STEP_SIZE    (510)
#define PMI_PATH_MAX_SIZE (65535)

namespace pilo
{
    namespace core
    {
        namespace io
        {
            class path
                : public ::pilo::core::memory::portable_compactable_autoreset_object_pool<path, PMI_STC_PARAM_PATH_STEP_SIZE, ::pilo::core::threading::native_mutex>
            {

            public:
                const static ::pilo::u16_t invalid_length = 0xFFFF;
                const static ::pilo::u16_t length_max = 65534;
                const static ::pilo::i32_t sepmode_current = 0;
                const static ::pilo::i32_t sepmode_nosep = 1;
                const static ::pilo::i32_t sepmode_needsep = 2;

            public: //static methods
                /**
            * @brief                    ensure a string path is Not end path separator
            * @param path_cstr [in/out] the string of path to process
            * @param length [in]        the length of the string, pass PMI_INVALID_PATH_LEN to auto calculate.
            * @return                   the size of the string after process
            * @see                      path_string_append_endsep_inplace path_string_has_endsep
            * @attention                [NPC] [RC] [MTS]
            */
                template<typename CHAR_T>
                static ::pilo::pathlen_t string_remove_endsep_inplace(CHAR_T* path_str, ::pilo::pathlen_t len)
                {
                    int i = 0;
                    if (path_str == nullptr)
                    {
                        return 0;
                    }

                    if (len == path::invalid_length)
                    {
                        len = (::pilo::pathlen_t) ::pilo::core::string::character_count(path_str);
                    }

                    for (i = (int)len - 1; i >= 0; i--)
                    {
                        if (path_str[i] == PMI_PATH_SEP)
                        {
                            path_str[i] = 0;
                        }
                        else
                        {
                            return (::pilo::pathlen_t)(i + 1);
                        }
                    }

                    return len;
                }
                /**
            * @brief                    ensure a string path is end path separators
            * @param path_cstr [in]     the string of path to process
            * @param length [in]        the length of the string, pass PMI_INVALID_PATH_LEN to auto calculate.
            * @return                   the size of the string after process
            * @see                      path_string_remove_endsep_inplace path_string_has_endsep
            * @attention                [NPC] [RC] [MTS]
            */
                template<typename CHAR_T>
                static ::pilo::pathlen_t string_append_endsep_inplace(CHAR_T* path_str, ::pilo::pathlen_t len)
                {
                    if (path_str == nullptr)
                    {
                        return 0;
                    }

                    if (len == path::invalid_length)
                    {
                        len = (::pilo::pathlen_t) ::pilo::core::string::character_count(path_str);
                    }

                    if (len > 0)
                    {
                        if (PMI_PATH_SEP == path_str[len - 1])
                        {
                            len = path::string_remove_endsep_inplace(path_str, len);
                        }
                    }

                    path_str[len] = PMI_PATH_SEP;
                    len++;
                    path_str[len] = 0;
                    return len;
                }

                static char* get_cwd(char* buffer, ::pilo::i64_t cc_buffer, ::pilo::pathlen_t* out_length, ::pilo::i32_t endsep_mode, ::pilo::pathlen_t extra_space = 0); 

            };
        }
    }
}



#endif //__pilo_io_path_hpp_