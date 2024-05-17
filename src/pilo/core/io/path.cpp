#include "path.hpp"
#include "../i18n/encoding_conversion.hpp"
#include "../memory/util.hpp"
#include "../string/string_operation.hpp"

#ifdef WINDOWS
#include <Shlobj.h>
#include <atlstr.h>
#include <ShellAPI.h>

#define PMI_PATH_PREF_ABS_LEN (4)

#else 
#define PMI_PATH_PREF_ABS_LEN (0)

#endif

namespace pilo
{
    namespace core
    {
        namespace io
        {
            bool path::equals_to(const ::pilo::core::io::path& p) const
            {
                if (this == &p)
                {
                    return true;
                }

                if (this->invalid() && p.invalid())
                {
                    return true;
                }

                if (this->_m_type != p._m_type)
                    return false;
                if (this->_m_ext_filename_len != p._m_ext_filename_len)
                    return false;
                if (this->_m_filename_start_pos != p._m_filename_start_pos)
                    return false;
                if (this->_m_length != p._m_length)
                    return false;
                if (0 != ::pilo::core::string::strict_compare(this->_m_pathstr_ptr, 0, p._m_pathstr_ptr, 0, -1))
                    return false;
                return true;
            }

            ::pilo::err_t path::set(bool is_dir, const char* path, ::pilo::i64_t len, ::pilo::pathlen_t extra)
            {
                if (path == nullptr) return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);

                this->clear();
                ::pilo::char_buffer_t buffer(this->_m_pathstr_ptr, this->_m_capacity, false);

                ::pilo::i8_t fs_type = ::pilo::core::io::path::path_type_na;                
                ::pilo::err_t err = validate_path(&buffer, path, len, extra, fs_type, is_dir);
                if (err != PILO_OK)
                {
                    return err;
                }
                buffer.set_adopt(false);
                

                if (!is_dir)
                {
                    const char* filename_sep = ::pilo::core::string::rfind_char(buffer.begin(), buffer.size(), PMI_PATH_SEP);
                    if (filename_sep == nullptr)
                    {
                        this->_m_filename_start_pos = 0;
                    }
                    else
                    {
                        this->_m_filename_start_pos = (::pilo::pathlen_t)(filename_sep - buffer.begin() + 1) ;
                    }
                }
                else
                {
                    this->_m_filename_start_pos = path::unknow_length;
                }
               

                this->_m_pathstr_ptr = buffer.begin();
                this->_m_capacity = (::pilo::pathlen_t)buffer.capacity();
                this->_m_length = (::pilo::pathlen_t)buffer.size();
                this->_m_type = fs_type;
                
                if (!is_dir)
                {
                    for (::pilo::pathlen_t i = this->_m_length - 1; i > 0; i--)
                    {
                        if (this->_m_pathstr_ptr[i] == PMI_PATH_SEP)
                        {
                            break;
                        }
                        else if (this->_m_pathstr_ptr[i] == '.')
                        {
                            this->_m_ext_filename_len = (::pilo::u8_t)(this->_m_length - i - 1);
                        }
                    }
                }

                return PILO_OK;
            }

            ::pilo::err_t path::validate_path(::pilo::char_buffer_t* buffer, const char* path_str, ::pilo::i64_t len
                , ::pilo::pathlen_t extra, ::pilo::i8_t & fs_type, bool & is_dir)
            {       
                if (buffer == nullptr)
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                fs_type = ::pilo::core::io::path::path_type_na;
                if (path_str == nullptr) return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL); 

                ::pilo::i64_t test_len = ::pilo::core::string::character_count(path_str);
                if (test_len < 1 || len < 1)
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_EMPTY);
                }
                else if (test_len > (len - (::pilo::i64_t)extra - PMI_PATH_PREF_ABS_LEN))
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_TOO_LARGE, 1);
                }
                if (len == path::unknow_length)
                {
                    len = test_len;
                }
                else if (len > (path::length_max - (::pilo::i64_t)extra) - PMI_PATH_PREF_ABS_LEN)
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_TOO_LARGE);
                }
                buffer->check_space((::pilo::i32_t)len + 6 + (::pilo::i32_t)extra);                 

                ::pilo::core::memory::object_array<char, PMI_STCPARAM_PATH_DEFAULT_LENGTH> tmp_path;
                tmp_path.check_space((::pilo::i32_t)len + 1);
                ::pilo::core::string::n_copyz(tmp_path.begin(), tmp_path.capacity(), path_str, len);
                tmp_path.set_size((::pilo::i32_t)len);

#ifdef WINDOWS
                if (tmp_path.at(0) != '\\' || tmp_path.at(1) != '\\')
                    ::pilo::core::string::replace_char(tmp_path.begin(), tmp_path.size(), '/', '\\');

#else

#endif

                bool isabs = false;
                ::pilo::pathlen_t endidx = 1;
#ifdef WINDOWS
                if (std::isalpha(tmp_path.begin()[0]) && tmp_path.begin()[1] == ':')
                {
                    isabs = true;
                    endidx = 2;
                }
                else if (buffer->size() > 3 && ::pilo::core::string::strict_compare(buffer->begin(), 0
                    , ::pilo::core::string::constants<char>::root_dir_sep(), 0
                    , ::pilo::core::string::constants<char>::root_dir_sep_length() != 0))
                {
                    isabs = true;
                    endidx = 4;
                }
#else
                if (buffer->at(0) == PMI_PATH_SEP)
                {
                    isabs = true;
                }

#endif

                if (!is_dir)
                {
                    if (tmp_path.begin()[len - 1] == PMI_PATH_SEP)
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);                    
                }
                else
                {
                    
#ifdef WINDOWS
                    
                                    
#else
                    if (len == 2)
                    {
                        if (src_path_str[0] == PMI_PATH_SEP && src_path_str[1] == PMI_PATH_SEP)
                        {
                            len--;
                        }
                    }
#endif
                    for (::pilo::pathlen_t i = (::pilo::pathlen_t) len - 1; i >= endidx; i--)
                    {
                        if ((tmp_path.begin()[i] == PMI_PATH_SEP))
                        {
                            len--;
                        }
                        else
                            break;
                    }  
                }

                //all cases
#ifdef WINDOWS
                if (len == 1)
                {
                    if (tmp_path.begin()[0] == '\\' || tmp_path.begin()[0] == '/')
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL, 1);                    
                    else if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars(), (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[0]))
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);                    
                    fs_type = ::pilo::core::io::path::local_fs_path;
                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), 1);
                    buffer->set_size(1);
                    return PILO_OK;
                }
                else if (len == 2)
                {
                    if ((tmp_path.begin()[0]) == '\\')
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL, 0);
                    
                    else if (::std::isalpha(tmp_path.begin()[0]) && tmp_path.begin()[1] == ':')
                    {                  
                        ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), ::pilo::core::string::constants<char>::root_dir_sep(), ::pilo::core::string::constants<char>::root_dir_sep_length());
                        ::pilo::core::string::n_copyz(buffer->ptr(::pilo::core::string::constants<char>::root_dir_sep_length()), buffer->space_available(), tmp_path.begin(), 2);
                        buffer->set_size(6);
                        is_dir = true;
                        fs_type = ::pilo::core::io::path::local_fs_path;
                        return PILO_OK;
                    }

                    for (::pilo::pathlen_t i = 0; i < len; i++)
                    {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[i]))
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        
                    }         
                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(),2);
                    buffer->set_size(2);
                    return PILO_OK;
                }
                else if (len == 3)
                {
                    if ((tmp_path.begin()[0]) == '\\' && (tmp_path.begin()[1]) == '\\')
                    {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[2]) != nullptr
                            || tmp_path.begin()[2] == '\\')
                        {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        }
                        ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), 3);
                        buffer->set_size(3);
                        fs_type = path::smb_fs_path;
                        return PILO_OK;
                    }
                    else if ((tmp_path.begin()[0]) == '\\')
                    {
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                    }
                    else if (::std::isalpha(tmp_path.begin()[0]) && tmp_path.begin()[1] == ':')
                    {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[2]) != nullptr)
                        {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        }
                        ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), ::pilo::core::string::constants<char>::root_dir_sep(), ::pilo::core::string::constants<char>::root_dir_sep_length());
                        ::pilo::core::string::n_copyz(buffer->ptr(::pilo::core::string::constants<char>::root_dir_sep_length()), buffer->space_available(), tmp_path.begin(), 2);
                        buffer->set(6, PMI_PATH_SEP);
                        buffer->set(7, tmp_path.begin()[2]);
                        buffer->set(8, 0);                        
                        buffer->set_size(8);
                        if (tmp_path.begin()[2] == PMI_PATH_SEP)
                            is_dir = true;
                        fs_type = ::pilo::core::io::path::local_fs_path;
                        return PILO_OK;
                    }                    

                    for (::pilo::pathlen_t i = 0; i < len; i++)
                    {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[i]))
                        {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        }
                    }

                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), len);
                    buffer->set_size((::pilo::i32_t)len);
                    fs_type = ::pilo::core::io::path::local_fs_path;
                    return PILO_OK;
                }

                
#else

#endif



                return PILO_OK;
            }


            pilo::err_t path::get_cwd(::pilo::char_buffer_t& buffer, ::pilo::i32_t endsep_mode, ::pilo::pathlen_t extra_space)
            { 
                ::pilo::err_t err = PILO_OK;
#           ifdef  WINDOWS
                ::pilo::pathlen_t szMesured = 0;
                ::pilo::pathlen_t szFinal = 0;
                ::pilo::core::memory::object_array<wchar_t, MAX_PATH> tmp_wbuff;                
                szMesured = (::pilo::pathlen_t)GetCurrentDirectoryW(0, 0);
                ::pilo::u32_t test_size = szMesured + 5 + extra_space;
                if (test_size > path::length_max)
                {
                    return ::pilo::make_core_error(PES_BUFFER, PEP_INSUFF);
                }
                tmp_wbuff.check_space(test_size);                    

                ::pilo::core::string::n_copy(tmp_wbuff.begin(), (::pilo::i64_t)tmp_wbuff.capacity(),
                    ::pilo::core::string::constants<wchar_t>::root_dir_sep(),
                    (::pilo::i64_t)::pilo::core::string::constants<wchar_t>::root_dir_sep_length());
                tmp_wbuff.set_size((::pilo::i32_t)::pilo::core::string::constants<wchar_t>::root_dir_sep_length());
                szFinal = (::pilo::pathlen_t)GetCurrentDirectoryW((DWORD) tmp_wbuff.space_available(), tmp_wbuff.ptr());
                if (szFinal <= 0)
                {
                    return ::pilo::make_core_error(PES_GETCWD, PEP_RDFAIL);
                }
                tmp_wbuff.ref_size() += szFinal;

                if ((tmp_wbuff.at(::pilo::core::string::constants<wchar_t>::root_dir_sep_length()) == (wchar_t)'\\')
                    || (tmp_wbuff.at(::pilo::core::string::constants<wchar_t>::root_dir_sep_length() + 1) == (wchar_t)'\\'))
                {
                    ::pilo::core::string::movez(tmp_wbuff.begin(), tmp_wbuff.capacity(), tmp_wbuff.ptr((::pilo::i32_t)::pilo::core::string::constants<char>::root_dir_sep_length()));
                    tmp_wbuff.ref_size() -= (::pilo::pathlen_t)(DWORD)::pilo::core::string::constants<char>::root_dir_sep_length();
                }

                err = ::pilo::core::i18n::os_unicode_to_utf8(buffer, tmp_wbuff.begin(), tmp_wbuff.size(), (::pilo::i32_t)extra_space);
                if (err != PILO_OK)
                {
                    return err;
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
                    err = path::string_append_endsep_inplace(buffer);
                }
                else if (endsep_mode == path::sepmode_nosep)
                {
                    err = path::string_remove_endsep_inplace(buffer);
                }

                return PILO_OK;
            }

#           ifdef  WINDOWS
            static ::pilo::err_t GetLnkFileName(OUT wchar_buffer_t & dst, IN wchar_buffer_t & src)
            {
                CoInitialize(0);
                IShellLinkW* shlink = 0;
                IPersistFile* persist = 0;
                WIN32_FIND_DATAW wfd = { 0 };

                dst.check_space(MAX_PATH);

                HRESULT hr = CoCreateInstance(CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, IID_IShellLinkW, (void**)&shlink);
                if (SUCCEEDED(hr)) {
                    hr = shlink->QueryInterface(IID_IPersistFile, (void**)&persist);
                    if (SUCCEEDED(hr)) {
                        hr = persist->Load(src.begin(), STGM_READ);
                        if (SUCCEEDED(hr)) {
                            // shlink->Resolve(0, SLR_ANY_MATCH | SLR_NO_UI);           
                            hr = shlink->GetPath(dst.begin(), MAX_PATH, &wfd, SLGP_RAWPATH);
                        }
                        persist->Release();
                    }
                    shlink->Release();
                }
                CoUninitialize();
                return PILO_OK;
            }

            ::pilo::err_t path::get_path_node_type(const char* path, ::pilo::pathlen_t path_len, ::pilo::i8_t path_type_hint
                , ::pilo::i8_t& node_type, ::pilo::i8_t* target_node_type, ::pilo::char_buffer_t* buffer)
            {                
                if (path == nullptr || *path == 0)
                {
                    node_type = path::node_type_na;
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);
                }
                if (path_len < 0)
                {
                    ::pilo::i64_t tmplen = ::pilo::core::string::character_count(path);
                    if (tmplen >= path::length_max)
                    {
                        return ::pilo::make_core_error(PES_PARAM, PEP_ARR_IDX_OOB);
                    }
                    path_len = (::pilo::pathlen_t)tmplen;
                }

                ::pilo::err_t err = PILO_OK;
                if (path_type_hint == path::local_fs_path)
                {
                    wchar_t warr[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = { 0 };
                    ::pilo::wchar_buffer_t wbuf(warr, PMI_STCPARAM_PATH_DEFAULT_LENGTH);
                    err = ::pilo::core::i18n::utf8_to_os_unicode(wbuf, path, path_len, -1);
                    if (err != PILO_OK)
                    {
                        node_type = path::node_type_na;
                        return err;
                    }

                    DWORD attributes = GetFileAttributesW(wbuf.begin());
                    if (attributes == INVALID_FILE_ATTRIBUTES) {
                        node_type = path::node_type_na;
                        return ::pilo::make_core_error(PES_FILE_ATTR, PEP_RDFAIL);
                    }

                    if (attributes & FILE_ATTRIBUTE_DIRECTORY) 
                    {
                        node_type = path::fs_node_type_dir;
                    }
                    else
                    {
                        SHFILEINFOW shFileInfo = { 0 };
                        SHGetFileInfoW(wbuf.begin(), 0, &shFileInfo, sizeof(SHFILEINFOW),
                            SHGFI_DISPLAYNAME | SHGFI_ICON | SHGFI_SMALLICON | SHGFI_TYPENAME | SHGFI_ATTRIBUTES);
                        if (shFileInfo.dwAttributes & SFGAO_LINK)
                        {
                            node_type = path::fs_node_type_lnk;
                            if (buffer != nullptr)
                            {
                                wchar_t wdst_arr[MAX_PATH + 1] = { 0 };
                                ::pilo::wchar_buffer_t wdst(wdst_arr, MAX_PATH+1);

                                err = GetLnkFileName(wdst, wbuf); //XXXXXXX
                                if (err != PILO_OK)
                                {
                                    node_type = path::node_type_na;
                                    return err;
                                }

                                if (target_node_type != nullptr)
                                {
                                    attributes = GetFileAttributesW(wdst.begin());
                                    if (attributes == INVALID_FILE_ATTRIBUTES) {
                                        ::pilo::set_if_ptr_is_not_null<::pilo::i8_t>(target_node_type, (::pilo::i8_t)path::node_type_na);
                                        return ::pilo::make_core_error(PES_FILE_ATTR, PEP_RDFAIL);
                                    }

                                    if (attributes & FILE_ATTRIBUTE_DIRECTORY) {
                                        ::pilo::set_if_ptr_is_not_null<::pilo::i8_t>(target_node_type, (::pilo::i8_t)path::fs_node_type_dir);
                                    }
                                    else
                                    {
                                        ::pilo::set_if_ptr_is_not_null<::pilo::i8_t>(target_node_type, (::pilo::i8_t)path::fs_node_type_file);
                                    }
                                }                                
                            }
                        }
                        else
                        {
                            node_type = path::fs_node_type_file;
                        }
                    }
                }
                else
                {
                    err = ::pilo::make_core_error(PES_OP, PEP_UNSUPPORT);
                }

                return err;
            }




            ::pilo::err_t path::make_dir(const char* dirpath, ::pilo::pathlen_t path_len)
            {
                if (dirpath == nullptr || *dirpath == 0 || path_len == 0)
                {
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                }
                wchar_t wpath_buffer[PMI_STCPARAM_PATH_DEFAULT_LENGTH] = {0};
                ::pilo::wchar_buffer_t wpath(wpath_buffer, PMF_COUNT_OF(wpath_buffer));
                if (path_len == path::unknow_length)
                {
                    path_len = (::pilo::pathlen_t) ::pilo::core::string::character_count(dirpath);
                }

                ::pilo::err_t err = ::pilo::core::i18n::utf8_to_os_unicode(wpath, dirpath, (::pilo::i32_t) path_len, 0);
                if (err != PILO_OK)
                    return err;

             //   ::pilo::i8_t node_type = path::node_type_na;
                
                




                return PILO_OK;
            }
            

        }
    }
#           else


#endif
}
