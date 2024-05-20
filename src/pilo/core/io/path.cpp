#include "path.hpp"
#include "../i18n/encoding_conversion.hpp"
#include "../memory/util.hpp"
#include "../string/string_operation.hpp"
#include <vector>

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
                if (this->_m_ext_name_len != p._m_ext_name_len)
                    return false;
                if (this->_m_lastpart_start_pos != p._m_lastpart_start_pos)
                    return false;
                if (this->_m_length != p._m_length)
                    return false;
                if (0 != ::pilo::core::string::strict_compare(this->_m_pathstr_ptr, 0, p._m_pathstr_ptr, 0, -1))
                    return false;
                return true;
            }

            ::pilo::err_t path::set(const char* p, ::pilo::i64_t len, ::pilo::pathlen_t extra, predefined_pilo_dir_enum rel_to_abs_basis)
            {
                if (p == nullptr) return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                bool isabs = false;
                this->clear();
                ::pilo::char_buffer_t buffer(this->_m_pathstr_ptr, this->_m_capacity, 0,  true);

                ::pilo::i8_t fs_type = ::pilo::core::io::path::path_type_na;                
                ::pilo::err_t err = validate_path(&buffer, p, len, extra, fs_type, isabs, rel_to_abs_basis);
                buffer.set_adopt(false);
                if (err != PILO_OK)
                {
                    return err;
                }

                const char* filename_sep = ::pilo::core::string::rfind_char(buffer.begin(), buffer.size(), PMI_PATH_SEP);
                if (filename_sep == nullptr)
                {
                    this->_m_lastpart_start_pos = 0;
                }
                else
                {
                    this->_m_lastpart_start_pos = (::pilo::pathlen_t)(filename_sep - buffer.begin() + 1);

                    if (isabs)
                    {
#ifdef  WINDOWS
                        if (buffer.size() == 6)
                        {
                            this->_m_lastpart_start_pos = 0;
                        }
#else
                        if (buffer.size() == 0)
                        {
                            this->_m_lastpart_start_pos = 0;
                        }
#endif //  WINDOWS
                    }

                }

                this->_m_pathstr_ptr = buffer.begin();
                this->_m_capacity = (::pilo::pathlen_t)buffer.capacity();
                this->_m_length = (::pilo::pathlen_t)buffer.size();
                this->_m_type = fs_type;
                
                for (::pilo::pathlen_t i = this->_m_length - 1; i > 0; i--)
                {
                    if (this->_m_pathstr_ptr[i] == PMI_PATH_SEP)
                    {
                        break;
                    }
                    else if (this->_m_pathstr_ptr[i] == '.')
                    {
                        if (i > 1 && this->_m_pathstr_ptr[i - 1] != PMI_PATH_SEP)
                        {
                            this->_m_ext_name_len = (::pilo::u8_t)(this->_m_length - i - 1);
                        }
                    }
                }

                return PILO_OK;
            }

            ::pilo::err_t path::fill_with_cwd(::pilo::pathlen_t extra)
            {
                char cbf[PMI_PATH_MAX] = { 0 };
                ::pilo::char_buffer_t buffer(cbf, sizeof(cbf), 0, false);
                ::pilo::err_t err = ::pilo::core::io::path::get_cwd(buffer, path::sepmode_nosep, extra);
                if (err != PILO_OK)
                    return err;

                return this->set(buffer.begin(), buffer.size(), extra, path::predefined_pilo_dir_enum::cwd);
            }

            ::pilo::err_t path::fill_with_exe(::pilo::pathlen_t extra)
            {
                return path::get_executable_path(this, extra);
            }

            ::pilo::err_t path::fill_with_home(::pilo::pathlen_t extra)
            {
                path p;
                ::pilo::err_t err = p.fill_with_bin(extra);
                if (err != PILO_OK)
                    return err;
                ::pilo::pathlen_t rlen = 0;
                const char* par = p.parentpath(rlen);
                return this->set(par, rlen, extra);
            }

            ::pilo::err_t path::fill_with_bin(::pilo::pathlen_t extra)
            {
                path p;
                ::pilo::err_t err = p.fill_with_exe(extra);
                if (err != PILO_OK)
                    return err;
                ::pilo::pathlen_t rlen = 0;
                const char* par = p.parentpath(rlen);
                return this->set(par, rlen, extra);
            }

            ::pilo::err_t path::fill_with_cnf(::pilo::pathlen_t extra)
            {
                ::pilo::err_t err = fill_with_home(extra);
                if (err != PILO_OK)
                    return err;
                return append(PMS_STCPARAM_PILO_PREDEF_DIR_CNF, path::unknow_length, extra);          
            }

            ::pilo::err_t path::fill_with_log(::pilo::pathlen_t extra)
            {
                ::pilo::err_t err = fill_with_home(extra);
                if (err != PILO_OK)
                    return err;
                return append(PMS_STCPARAM_PILO_PREDEF_DIR_LOG, path::unknow_length, extra);
            }

            ::pilo::err_t path::fill_with_tmp(::pilo::pathlen_t extra)
            {
                ::pilo::err_t err = fill_with_home(extra);
                if (err != PILO_OK)
                    return err;
                return append(PMS_STCPARAM_PILO_PREDEF_DIR_TMP, path::unknow_length, extra);
            }

            ::pilo::err_t path::append(const char* p, ::pilo::i64_t len, ::pilo::pathlen_t extra, predefined_pilo_dir_enum rel_to_abs_basis)
            {
                if (p == nullptr) return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                if (len == path::unknow_length)
                    len = ::pilo::core::string::character_count(p);
                if (len < 1)
                {
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_EMPTY);
                }
                bool isabs = false;
                ::pilo::i8_t fs_type = ::pilo::core::io::path::path_type_na;
                ::pilo::core::memory::object_array<char, PMI_PATH_MAX> tbuf;
                ::pilo::char_buffer_t buffer(this->_m_pathstr_ptr, this->_m_capacity, this->_m_length, true);                
                
                if (p[0] != PMI_PATH_SEP)
                {
                    buffer.check_space((::pilo::i32_t)this->length() + (::pilo::i32_t)len + 2 + (::pilo::i32_t)extra);
                    buffer.set(buffer.size(), PMI_PATH_SEP);
                    buffer.add_size(1);
                    buffer.set(buffer.size(), 0);
                }
                else
                {
                    buffer.check_space((::pilo::i32_t)this->length() + (::pilo::i32_t)len + 1 + (::pilo::i32_t)extra);                    
                }
                tbuf.check_space(buffer.capacity());
                ::pilo::core::string::n_copyz(tbuf.begin(), tbuf.capacity(), buffer.begin() ,buffer.size());
                ::pilo::core::string::n_concatenate_inplace(tbuf.ptr(), tbuf.space_available(), p, len);
                tbuf.set_size(buffer.size() + (::pilo::i32_t) len);
                ::pilo::err_t err = validate_path(&buffer, tbuf.begin(), tbuf.size(), extra, fs_type, isabs, rel_to_abs_basis);
                buffer.set_adopt(false);
                this->_m_pathstr_ptr = buffer.begin();
                if (err != PILO_OK)
                {
                    return err;
                }                

                const char* filename_sep = ::pilo::core::string::rfind_char(buffer.begin(), buffer.size(), PMI_PATH_SEP);
                if (filename_sep == nullptr)
                {
                    this->_m_lastpart_start_pos = 0;
                }
                else
                {
                    this->_m_lastpart_start_pos = (::pilo::pathlen_t)(filename_sep - buffer.begin() + 1);
                    if (isabs)
                    {
#ifdef  WINDOWS
                        if (buffer.size() == 6)
                        {
                            this->_m_lastpart_start_pos = 0;
                        }
#else
                        if (buffer.size() == 0)
                        {
                            this->_m_lastpart_start_pos = 0;
                        }
#endif //  WINDOWS
                    }
                }

                this->_m_pathstr_ptr = buffer.begin();
                this->_m_capacity = (::pilo::pathlen_t)buffer.capacity();
                this->_m_length = (::pilo::pathlen_t)buffer.size();
                this->_m_type = fs_type;

                for (::pilo::pathlen_t i = this->_m_length - 1; i > 0; i--)
                {
                    if (this->_m_pathstr_ptr[i] == PMI_PATH_SEP)
                    {
                        break;
                    }
                    else if (this->_m_pathstr_ptr[i] == '.')
                    {
                        if (i > 1 && this->_m_pathstr_ptr[i - 1] != PMI_PATH_SEP)
                        {
                            this->_m_ext_name_len = (::pilo::u8_t)(this->_m_length - i - 1);
                        }
                    }
                }

                return PILO_OK;
            }

            ::pilo::err_t path::validate_path(::pilo::char_buffer_t* buffer, const char* path_str, ::pilo::i64_t path_str_len , ::pilo::pathlen_t extra, ::pilo::i8_t & fs_type, bool& isabs, predefined_pilo_dir_enum rel_to_abs_basis)
            {    
                PMC_UNUSED(rel_to_abs_basis);
                if (buffer == nullptr)
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                fs_type = ::pilo::core::io::path::path_type_na;
                if (path_str == nullptr) return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL); 

                ::pilo::i64_t test_len = ::pilo::core::string::character_count(path_str);
                if (test_len < 1 || path_str_len < 1)
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_EMPTY);
                }
                
                if (path_str_len == path::unknow_length)
                {
                    path_str_len = test_len;
                }
                else if (path_str_len > (path::length_max - (::pilo::i64_t)extra) - PMI_PATH_PREF_ABS_LEN)
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_TOO_LARGE);
                }
                //buffer->check_space((::pilo::i32_t)path_str_len + 6 + (::pilo::i32_t)extra);

                ::pilo::core::memory::object_array<char, 64> tmp_path;
                tmp_path.check_space((::pilo::i32_t)path_str_len + 6);
                if (::std::isalpha(path_str[0]) && path_str[1] == ':')
                {
                    ::pilo::core::string::n_copyz(tmp_path.begin(), tmp_path.capacity(), ::pilo::core::string::constants<char>::root_dir_sep(), ::pilo::core::string::constants<char>::root_dir_sep_length());
                    tmp_path.set_size((::pilo::i32_t) ::pilo::core::string::constants<char>::root_dir_sep_length());
                }

                ::pilo::core::string::n_copyz(tmp_path.ptr(), tmp_path.space_available(), path_str, path_str_len);
                tmp_path.add_size((::pilo::i32_t)path_str_len);                

                ::pilo::pathlen_t endidx = 1;
#ifdef WINDOWS
                if (tmp_path.size() > 3 && ::pilo::core::string::strict_compare(tmp_path.begin(), 0
                    , ::pilo::core::string::constants<char>::root_dir_sep(), 0
                    , ::pilo::core::string::constants<char>::root_dir_sep_length()) == 0)
                {

                    ::pilo::core::string::replace_char(tmp_path.begin()+4, tmp_path.size()-4, '/', '\\');

                    if (tmp_path.size() >= 4 && tmp_path.size() < 6)
                    {
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                    }
                    isabs = true;
                    endidx = 4;                    
                }
                else
                {
                    ::pilo::core::string::replace_char(tmp_path.begin(), tmp_path.size(), '/', '\\');
                }
                ::pilo::i64_t rscnt = 0;
                ::pilo::i64_t rscnt2 = 0;
                ::pilo::core::string::rescanable_replace_inplace(tmp_path.begin() + endidx, tmp_path.size() - endidx, tmp_path.capacity() - endidx, "\\\\", "\\", &rscnt);
                ::pilo::core::string::rescanable_replace_inplace(tmp_path.begin() + endidx, tmp_path.size() - endidx, tmp_path.capacity() - endidx, "\\.\\", "\\", &rscnt2);
                if (rscnt > 0 || rscnt2 > 0)
                    tmp_path.set_size((::pilo::i32_t) ::pilo::core::string::character_count(tmp_path.begin()));


#else
                ::pilo::i64_t rscnt = 0;
                ::pilo::i64_t rscnt2 = 0;
                ::pilo::core::string::rescanable_replace_inplace(tmp_path.begin() + endidx, tmp_path.size() - endidx, tmp_path.capacity() - endidx, "//", "/", &rscnt);
                ::pilo::core::string::rescanable_replace_inplace(tmp_path.begin() + endidx, tmp_path.size() - endidx, tmp_path.capacity() - endidx, "/./", "/", &rscnt2);
                if (rscnt > 0 || rscnt2 > 0)
                    tmp_path.set_size((::pilo::i32_t) ::pilo::core::string::character_count(tmp_path.begin()));

#endif
                ::pilo::i32_t len_to_dec = 0;
                for (::pilo::i32_t i = (::pilo::pathlen_t)tmp_path.size() - 1; i >= endidx; i--)
                {
                    if ((tmp_path.begin()[i] == PMI_PATH_SEP))
                    {
                        tmp_path.begin()[i] = 0;
                        len_to_dec--;
                        
                    }
                    else
                        break;
                }
                tmp_path.add_size(len_to_dec);

                //all cases
#ifdef WINDOWS
                if (tmp_path.size() == 1)
                {
                    if (tmp_path.begin()[0] == '\\' || tmp_path.begin()[0] == '/')
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL, 1);                    
                    else if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars(), (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[0]))
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);                    
                    fs_type = ::pilo::core::io::path::local_fs_path;
                    buffer->check_space(2);
                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), 1);
                    buffer->set_size(1);
                    return PILO_OK;
                }
                else if (tmp_path.size() == 2)
                {                    
                    if ((tmp_path.begin()[0]) == '\\')
                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL, 0);
                    else if (::std::isalpha(tmp_path.begin()[0]) && tmp_path.begin()[1] == ':')
                    {         
                        buffer->check_space(7);
                        ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), ::pilo::core::string::constants<char>::root_dir_sep(), ::pilo::core::string::constants<char>::root_dir_sep_length());
                        ::pilo::core::string::n_copyz(buffer->ptr(::pilo::core::string::constants<char>::root_dir_sep_length()), buffer->space_available(), tmp_path.begin(), 2);
                        buffer->set_size(6);
                        fs_type = ::pilo::core::io::path::local_fs_path;
                        return PILO_OK;
                    }
                    buffer->check_space(3);
                    for (::pilo::pathlen_t i = 0; i < tmp_path.size(); i++)
                    {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[i]))
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        
                    }         
                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(),2);
                    buffer->set_size(2);
                    return PILO_OK;
                }
                else if (tmp_path.size() == 3)
                {
                    buffer->check_space(4);
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
                        buffer->check_space(10);
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
                        fs_type = ::pilo::core::io::path::local_fs_path;
                        return PILO_OK;
                    }                    

                    for (::pilo::i32_t i = 0; i < tmp_path.size(); i++)
                    {
                        if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                            , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.begin()[i]))
                        {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                        }
                    }

                    ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), tmp_path.size());
                    buffer->set_size((::pilo::i32_t)tmp_path.size());
                    fs_type = ::pilo::core::io::path::local_fs_path;
                    return PILO_OK;
                }
                else
                {
                    fs_type = ::pilo::core::io::path::local_fs_path;
                    if (isabs)
                    {
                        if (tmp_path.size() == 6)
                        {
                            buffer->check_space(7);
                            ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), tmp_path.size());
                            buffer->set_size((::pilo::i32_t)tmp_path.size());
                            fs_type = ::pilo::core::io::path::local_fs_path;
                            return PILO_OK;                            
                        }
                        else if (tmp_path.size() < 6)
                        {
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                        }
                        else
                        {
                            if (tmp_path.at(6) != PMI_PATH_SEP)
                            {
                                ::pilo::core::string::n_movez(tmp_path.begin()+7, tmp_path.capacity() - 7,  tmp_path.begin() + 6, tmp_path.size() - 6);
                                tmp_path.begin()[6]= PMI_PATH_SEP;
                                tmp_path.add_size(1);
                            }

                            for (::pilo::pathlen_t i = 6; i < tmp_path.size(); i++)
                            {
                                if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                                    , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.at(i)) != nullptr)
                                {
                                    return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                                }
                            }
                            
                            ::pilo::i32_t pcnt = (::pilo::i32_t) ::pilo::core::string::cstring_ch_count(tmp_path.begin(), 5, tmp_path.size() - 5, PMI_PATH_SEP);
                            if (pcnt > 0 && ::pilo::core::string::find_substring(tmp_path.begin(), "..", tmp_path.size()))
                            {
                                ::pilo::core::memory::object_array<::pilo::cstr_ref<char>, 1> parts;
                                parts.check_space(pcnt + 1);
                                ::pilo::i64_t nparts = ::pilo::core::string::split_fixed(tmp_path.begin()+5, tmp_path.size()-5
                                   ,"\\", 1, parts.begin(), parts.capacity(), false, true, false, false);
                                if (nparts < 2)
                                {
                                    PMC_ASSERT(false);
                                    return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                                }

                                parts.at(0).ptr = tmp_path.begin();
                                parts.at(0).length += 5;    
                                std::vector<::pilo::cstr_ref<char>*> vec;
                                vec.push_back(&parts.at((int)0));
                                for (::pilo::i64_t j = 1; j < nparts; j++)
                                {
                                    //printf(" -> [%s]\n", parts.at((int)j).to_string().c_str());
                                    if (parts.at((int)j).length == 2 && parts.at((int)j).ptr[0] == '.' && parts.at((int)j).ptr[1] == '.')
                                    {
                                        if (vec.size() > 1)
                                            vec.pop_back();
                                        else
                                        {
                                            return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                                        }
                                    }
                                    else
                                    {
                                        vec.push_back(&parts.at((int)j));
                                    }                    

                                }
                                ::pilo::core::string::concatenate_rac(buffer, vec, "\\", 1);
                            }
                            else
                            {        
                                buffer->check_space(tmp_path.size() + 1);
                                ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), tmp_path.size());
                                buffer->set_size(tmp_path.size());
                            }
                            
                            return PILO_OK;
                        }                        
                    }
                    else //relative p
                    {
                        for (::pilo::pathlen_t i = 0; i < tmp_path.size(); i++)
                        {
                            if (::pilo::core::string::find_char(::pilo::core::string::constants<char>::illegal_path_chars()
                                , (::pilo::i64_t) ::pilo::core::string::constants<char>::illegal_path_chars_length(), tmp_path.at(i)) != nullptr)
                            {
                                return ::pilo::make_core_error(PES_PATH_STR, PEP_INV_CHARS);
                            }
                        }

                        ::pilo::i32_t pcnt = (::pilo::i32_t) ::pilo::core::string::cstring_ch_count(tmp_path.begin(), 0, tmp_path.size(), PMI_PATH_SEP);
                        if (pcnt > 0 && ::pilo::core::string::find_substring(tmp_path.begin(), "..", tmp_path.size()))
                        {
                            ::pilo::core::memory::object_array<::pilo::cstr_ref<char>, 1> parts;
                            parts.check_space(pcnt + 1);
                            ::pilo::i64_t nparts = ::pilo::core::string::split_fixed(tmp_path.begin(), tmp_path.size()
                                , "\\", 1, parts.begin(), parts.capacity(), false, true, false, false);
                            if (nparts < 2)
                            {
                                PMC_ASSERT(false);
                                return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                            }


                            std::vector<::pilo::cstr_ref<char>*> vec;
                            for (::pilo::i64_t j = 0; j < nparts; j++)
                            {
                                //printf(" -> [%s]\n", parts.at((int)j).to_string().c_str());
                                if (parts.at((int)j).length == 2 && parts.at((int)j).ptr[0] == '.' && parts.at((int)j).ptr[1] == '.')
                                {
                                    if (vec.size() > 1)
                                        vec.pop_back();
                                    else
                                    {
                                        return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                                    }
                                }
                                else
                                {
                                    vec.push_back(&parts.at((int)j));
                                }

                            }
                            ::pilo::core::string::concatenate_rac(buffer, vec, "\\", 1);
                        }
                        else
                        {
                            buffer->check_space(tmp_path.size() + 1);
                            ::pilo::core::string::n_copyz(buffer->begin(), buffer->capacity(), tmp_path.begin(), tmp_path.size());
                            buffer->set_size(tmp_path.size());
                        }
                    }
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

             //   ::pilo::i8_t node_type = p::node_type_na;              
                




                return PILO_OK;
            }

            ::pilo::err_t path::get_executable_path(::pilo::core::io::path* p, ::pilo::pathlen_t extra)
            {
                
#ifdef WINDOWS
                WCHAR tp[PMI_PATH_MAX] = { 0 };
                DWORD need_capa = GetModuleFileNameW(NULL, tp, sizeof(tp));
                if (need_capa == 0)
                {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_NULL);
                }

                char buf[PMI_PATH_MAX] = { 0 };
                ::pilo::char_buffer_t buffer(buf,sizeof(buf), 0, false);                
                ::pilo::core::i18n::os_unicode_to_utf8(buffer, tp, need_capa, 0);
                return p->set(buf, path::unknow_length, extra);

#else
                char tp[PMI_PATH_MAX] = { 0 };
                ssize_t len = readlink("/proc/self/exe", tp, sizeof(tp) - 1);
                if (len < 0 || (size_t)len >= sizeof(tp)) {
                    return ::pilo::make_core_error(PES_PATH_STR, PEP_IS_NULL);
                }
                tp[len] = '\0';
                return p->set(tp, path::unknow_length, extra);
#endif

            }
            

        }
    }
#           else


#endif
}
