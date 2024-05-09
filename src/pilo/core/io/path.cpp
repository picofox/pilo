#include "path.hpp"



namespace pilo
{
    namespace core
    {
        namespace io
        {


            char* path::get_cwd(char* buffer, ::pilo::i64_t cc_buffer
                , ::pilo::pathlen_t* out_length
                , ::pilo::i32_t endsep_mode, ::pilo::pathlen_t extra_space)
            {
                char* strRet = buffer;
                ::pilo::pathlen_t ret_size = 0;                

#           ifdef  WINDOWS
                ::pilo::pathlen_t szMesured = 0;
                ::pilo::pathlen_t szFinal = 0;

                ::pilo::pathlen_t max_capa;
                if (cc_buffer > path::length_max)
                {
                    max_capa = path::length_max;
                }
                else
                {
                    max_capa = (::pilo::pathlen_t)cc_buffer;
                }
                szMesured = (::pilo::pathlen_t)GetCurrentDirectoryA(0, 0);
                ::pilo::u32_t test_size = szMesured + 5 + extra_space;
                if (test_size >= path::length_max)
                    return nullptr;
                szMesured = (::pilo::pathlen_t)test_size;

                if ((buffer == 0) || (cc_buffer == 0) || (szMesured > cc_buffer))
                {
                    strRet = (char*)PMF_HEAP_MALLOC(szMesured);
                    if (strRet == nullptr)
                    {
                        return nullptr;
                    }
                    max_capa = szMesured;
                }
                ::pilo::core::string::n_copy(strRet, (::pilo::i64_t) max_capa,
                    ::pilo::core::string::constants<char>::root_dir_sep(),
                    (::pilo::i64_t)::pilo::core::string::constants<char>::root_dir_sep_length());

                szFinal = (::pilo::pathlen_t)GetCurrentDirectoryA(
                    (DWORD)(cc_buffer - (DWORD)::pilo::core::string::constants<char>::root_dir_sep_length()),
                    strRet + (DWORD)::pilo::core::string::constants<char>::root_dir_sep_length());
                if (szFinal <= 0)
                {
                    if (strRet != buffer)
                    {
                        PMF_HEAP_FREE(strRet);
                    }
                    return nullptr;
                }
                ret_size = szFinal + (::pilo::pathlen_t) ::pilo::core::string::constants<char>::root_dir_sep_length();

                if (strRet[(DWORD)::pilo::core::string::constants<char>::root_dir_sep_length() + 1] == '\\'
                    && strRet[(DWORD)::pilo::core::string::constants<char>::root_dir_sep_length()] == '\\')
                {
                    ::pilo::core::string::movez(strRet, max_capa, strRet + (DWORD)::pilo::core::string::constants<char>::root_dir_sep_length());
                    ret_size -= (::pilo::pathlen_t)(DWORD)::pilo::core::string::constants<char>::root_dir_sep_length();
                }


#           else
                strRet = getcwd(buffer, cc_buffer - extra_space);
                if (strRet == nullptr && errno != ERANGE)
                {
                    return nullptr;
                }
                if (strRet == nullptr)
                {
                    strRet = getcwd(nullptr, 0);
                    if (strRet == nullptr)
                        return nullptr;
                }
                ret_size = PMF_STRING_LENGTH_A(strRet);
                if (extra_space > 0)
                {
                    char* str_ret_2 = (char*)PMF_HEAP_REALLOC(strRet, ret_size + 1 + extra_space);
                    if (str_ret_2 == nullptr)
                    {
                        PMF_HEAP_FREE(strRet);
                        return nullptr;
                    }
                }

#           endif

                if (endsep_mode == path::sepmode_needsep)
                {
                    ret_size = path::string_append_endsep_inplace(strRet, ret_size);
                }
                else if (endsep_mode == path::sepmode_nosep)
                {
                    ret_size = path::string_remove_endsep_inplace(strRet, ret_size);
                }

                if (out_length != nullptr)
                {
                    *out_length = ret_size;
                }

                return strRet;
            }
        }
    }
}
