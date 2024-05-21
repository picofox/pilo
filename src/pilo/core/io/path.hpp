﻿#ifndef __pilo_io_path_hpp_
#define __pilo_io_path_hpp_

#include <string>
#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include "./formatted_io.hpp"
#include "../threading/native_mutex.hpp"
#include "../memory/compactable_autoreset_object_pool.hpp"
#include "../memory/util.hpp"


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
                typedef ::pilo::err_t(*iter_func_t)(const ::pilo::core::io::path* p, ::pilo::pathlen_t len, ::pilo::i32_t idx, bool is_last, void* ctx);

                static ::pilo::err_t s_dir_create_handler(const ::pilo::core::io::path* p, ::pilo::pathlen_t len, ::pilo::i32_t , bool , void* null_means_no_force )
                {
                    bool is_force = null_means_no_force == nullptr ? false : true;
                    return path::make_dir(p->fullpath(), len, is_force);
                }

                const static ::pilo::u8_t invalid_ext_length = 0xFF;
                const static ::pilo::u16_t unknow_length = 0xFFFF;
                const static ::pilo::u16_t length_max = 65534;
                const static ::pilo::i32_t sepmode_current = 0;
                const static ::pilo::i32_t sepmode_nosep = 1;
                const static ::pilo::i32_t sepmode_needsep = 2;

                const static ::pilo::i8_t path_type_na = -1;
                const static ::pilo::i8_t local_fs_path = 0;
                const static ::pilo::i8_t smb_fs_path = 1;

                const static ::pilo::i8_t node_type_na = -1;
                const static ::pilo::i8_t fs_node_type_file = 0;
                const static ::pilo::i8_t fs_node_type_dir = 1;
                const static ::pilo::i8_t fs_node_type_lnk = 2;
                const static ::pilo::i8_t fs_node_type_other = 3;

                const static ::pilo::i8_t other_absolute = -1;
                const static ::pilo::i8_t relative = 0;
                const static ::pilo::i8_t absolute = 1;


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
                static ::pilo::err_t string_remove_endsep_inplace(::pilo::core::memory::object_array_adapter<CHAR_T> & src_buffer)
                {
                    int i = 0;
                    if (src_buffer.invalid())
                    {
                        return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                    }

                    for (i = src_buffer.size() - 1; i >= 0; i--)
                    {
                        if (src_buffer.at(i) == PMI_PATH_SEP)
                        {
                            src_buffer.set(i, 0);
                            src_buffer.add_size(-1);
                        }
                        else
                        {
                            break;
                        }
                    }

                    return PILO_OK;
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
                static ::pilo::err_t string_append_endsep_inplace(::pilo::core::memory::object_array_adapter<CHAR_T>& src_buffer)
                {
                    if (src_buffer.invalid())
                    {
                        return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                    }

                    ::pilo::err_t err = path::string_remove_endsep_inplace(src_buffer);
                    if (err != PILO_OK)
                    {
                        return err;
                    }

                    src_buffer.check_space(src_buffer.size() + 2);
                    src_buffer.set(src_buffer.size(), PMI_PATH_SEP);
                    src_buffer.add_size(1);
                    src_buffer.set(src_buffer.size(), 0);
                    return PILO_OK;
                }


                template<typename TA_CHAR>
                static ::pilo::err_t split_paths(const TA_CHAR* pathstr, ::pilo::pathlen_t path_len
                    , ::pilo::core::memory::object_array_adapter<::pilo::pathlen_t> & indices)
                {
                    bool end_with_sep = false;
                    ::pilo::i8_t abs_type = path::absolute_type(pathstr, &end_with_sep, path_len);
                    if (abs_type == path::path_type_na)
                    {
                        return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);
                    }

                    if (path_len == path::unknow_length)
                    {
                        path_len = (::pilo::pathlen_t)  ::pilo::core::string::character_count(path_cstr);
                    }

                    ::pilo::i32_t cnt = (::pilo::i32_t) ::pilo::core::string::cstring_ch_count(pathstr, 0, path_len, PMI_PATH_SEP);
                    cnt++;
                    parts.check_space(cnt);

                    TA_CHAR* beg_ptr = ::pilo::core::string::find_char(pathstr, (::pilo::i64_t)path_len, PMI_PATH_SEP);
                    if (beg_ptr == nullptr)
                    {
                        indices.set(0, path_len);
                        return PILO_OK;
                    }

                    if (abs_type == path::absolute)
                    {
#ifdef WINDOWS
                        beg_ptr = pathstr + 7;
#else
                        beg_ptr = path + 1;
#endif
                    }


                    return PILO_OK;
                }

                static ::pilo::err_t validate_path(::pilo::char_buffer_t* buffer, const char* path, ::pilo::i64_t len, ::pilo::pathlen_t extra, ::pilo::i8_t & fs_type, bool & isabs, predefined_pilo_dir_enum rel_to_abs_basis);
                static ::pilo::err_t get_cwd(::pilo::char_buffer_t& buffer,  ::pilo::i32_t endsep_mode, ::pilo::pathlen_t extra_space = 0);
                static ::pilo::err_t get_path_node_type(const char* path, ::pilo::pathlen_t path_len,  ::pilo::i8_t path_type_hint, ::pilo::i8_t& node_type, ::pilo::i8_t* target_node_type, ::pilo::char_buffer_t* buffer);
                static ::pilo::err_t make_dir(const char* dirpath, ::pilo::pathlen_t path_len, bool is_force);
                static ::pilo::err_t get_executable_path(::pilo::core::io::path * path, ::pilo::pathlen_t extra);

                template<typename TA_CHAR>
                static ::pilo::i8_t absolute_type(const TA_CHAR* path_cstr, bool* end_with_sep, ::pilo::pathlen_t length = path::unknow_length)
                {
                    if (path_cstr == nullptr)
                    {
                        return path::path_type_na;
                    }

                    if (length == path::unknow_length)
                    {
                        length = (::pilo::pathlen_t)  ::pilo::core::string::character_count(path_cstr);
                    }

                    if (*path_cstr == 0)
                    {
                        return path::path_type_na;
                    }

                    if (path_cstr[length - 1] == PMI_PATH_SEP)
                    {
                        ::pilo::set_if_ptr_is_not_null(end_with_sep, true);
                    }
                    else
                    {
                        ::pilo::set_if_ptr_is_not_null(end_with_sep, false);
                    }

                    

                    if (0 == ::pilo::core::string::strict_compare(path_cstr, 0, ::pilo::core::string::constants<TA_CHAR>::root_dir_sep(), 0, ::pilo::core::string::constants<TA_CHAR>::root_dir_sep_length()))
                    {
#ifdef WINDOWS
                        if (::std::isalpha(path_cstr[4]) && path_cstr[5] == ':')
                        {
                            return path::absolute;
                        }
                        else
                        {
                            return path::path_type_na;
                        }
#else 
                        return path::absolute;
#endif // WINDOWS
                        
                        
                    }
                    else if (0 == ::pilo::core::string::strict_compare(path_cstr, 0, ::pilo::core::string::constants<TA_CHAR>::smb_root(), 0, ::pilo::core::string::constants<TA_CHAR>::smb_root_length()))
                    {
                        return path::other_absolute;
                    }
                    else
                    {
#ifdef WINDOWS
                        if (::std::isalpha(path_cstr[0]) && path_cstr[1] == ':')
                        {
                            return path::absolute;
                        }
#endif
                    }

                    return path::relative;

                }


                public:
                    path(const char* p, ::pilo::i64_t len, ::pilo::pathlen_t extra, predefined_pilo_dir_enum rel_to_abs_basis = predefined_pilo_dir_enum::count)
                    {
                        set(p, len, extra, rel_to_abs_basis);
                    }

                    path(const char* p, ::pilo::i64_t len, predefined_pilo_dir_enum rel_to_abs_basis = predefined_pilo_dir_enum::count)
                    {
                        set(p, len, 0, rel_to_abs_basis);
                    }

                    path(const char* p, predefined_pilo_dir_enum rel_to_abs_basis = predefined_pilo_dir_enum::count)
                    {
                        set(p, path::unknow_length, 0, rel_to_abs_basis);
                    }

                    path() : _m_pathstr_ptr(nullptr), _m_length(0), _m_capacity(0), _m_lastpart_start_pos(path::unknow_length), _m_ext_name_len(path::invalid_ext_length),_m_type(path::path_type_na)
                    {
                    }       
                    ::pilo::err_t append(const char* p, ::pilo::i64_t len, ::pilo::pathlen_t extra, predefined_pilo_dir_enum rel_to_abs_basis = predefined_pilo_dir_enum::count);
                    ::pilo::err_t append(const char* p, ::pilo::i64_t len, predefined_pilo_dir_enum rel_to_abs_basis = predefined_pilo_dir_enum::count)
                    {
                        return append(p, len, 0, rel_to_abs_basis);
                    }
                    ::pilo::err_t append(const char* p,  predefined_pilo_dir_enum rel_to_abs_basis = predefined_pilo_dir_enum::count)
                    {
                        return append(p, path::unknow_length, 0, rel_to_abs_basis);
                    }

                    ::pilo::err_t set(const char* p, ::pilo::i64_t len, ::pilo::pathlen_t extra, predefined_pilo_dir_enum rel_to_abs_basis = predefined_pilo_dir_enum::count);
                    ::pilo::err_t set(const char* p, predefined_pilo_dir_enum rel_to_abs_basis = predefined_pilo_dir_enum::count)
                    {
                        return set(p, path::unknow_length, 0, rel_to_abs_basis);
                    }

                    ::pilo::err_t forward_iterate(iter_func_t func, void* ctx, bool ignore_err);
                    ::pilo::err_t create(::pilo::i8_t fs_node_type, bool is_force);

                    ::pilo::err_t fill_with_cwd(::pilo::pathlen_t extra);
                    ::pilo::err_t fill_with_exe(::pilo::pathlen_t extra);
                    ::pilo::err_t fill_with_home(::pilo::pathlen_t extra);
                    ::pilo::err_t fill_with_bin(::pilo::pathlen_t extra);
                    ::pilo::err_t fill_with_cnf(::pilo::pathlen_t extra);
                    ::pilo::err_t fill_with_log(::pilo::pathlen_t extra);
                    ::pilo::err_t fill_with_tmp(::pilo::pathlen_t extra);
                    inline ::pilo::err_t fill_with_predef_path(predefined_pilo_dir_enum rel_to_abs_basis, ::pilo::pathlen_t extra)
                    {
                        if (rel_to_abs_basis == predefined_pilo_dir_enum::cwd) return fill_with_cwd(extra);
                        else if (rel_to_abs_basis == predefined_pilo_dir_enum::exe) return fill_with_exe(extra);
                        else if (rel_to_abs_basis == predefined_pilo_dir_enum::home) return fill_with_home(extra);
                        else if (rel_to_abs_basis == predefined_pilo_dir_enum::bin) return fill_with_bin(extra);
                        else if (rel_to_abs_basis == predefined_pilo_dir_enum::cnf) return fill_with_cnf(extra);
                        else if (rel_to_abs_basis == predefined_pilo_dir_enum::log) return fill_with_log(extra);
                        else return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);                        
                    }


                    inline bool invalid() const
                    {
                        if (_m_pathstr_ptr == nullptr)
                        {
                            return true;
                        }
                        return false;
                    }

                    bool equals_to(const ::pilo::core::io::path& p) const;

                    friend bool operator==(const path& f1, const path& f2)
                    {
                        return f1.equals_to(f2);
                    }
                    friend bool operator!=(const path& f1, const path& f2)
                    {
                        return !f1.equals_to(f2);
                    }

                    ::pilo::i8_t absolute_type() const;

                    inline const char* fullpath() const
                    {
                        return _m_pathstr_ptr;
                    }

                    inline ::pilo::pathlen_t capacity() const
                    {
                        return _m_capacity;
                    }

                    inline ::pilo::pathlen_t length() const
                    {
                        return _m_length;
                    }

                    inline ::pilo::pathlen_t lastpart_start_pos() const
                    {
                        return _m_lastpart_start_pos;
                    }

                    inline ::pilo::pathlen_t lastpart_len() const
                    {
                        return _m_length - _m_lastpart_start_pos;
                    }

                    inline ::pilo::u8_t extname_len() const
                    {
                        return _m_ext_name_len;
                    }

                    inline ::pilo::i8_t type() const
                    {
                        return _m_type;
                    }                    

                    inline const char* lastpart() const
                    {
                        if (this->_m_lastpart_start_pos >= this->_m_length
                            || this->_m_lastpart_start_pos == path::unknow_length)
                        {
                            return nullptr;
                        }
                        return this->_m_pathstr_ptr + _m_lastpart_start_pos;
                    }

                    inline const char* extname() const
                    {
                        if ((::pilo::pathlen_t)this->_m_ext_name_len >= this->_m_length
                            || this->_m_ext_name_len == path::invalid_ext_length)
                        {
                            return nullptr;
                        }
                        return this->_m_pathstr_ptr + this->_m_length - _m_ext_name_len;
                    }

                    inline const char* basename(::pilo::pathlen_t & rlen) const
                    {
                        if (_m_ext_name_len == path::invalid_ext_length)
                        {
                            rlen = this->_m_ext_name_len;
                            return lastpart();
                        }
                        else if (_m_ext_name_len == 0)
                        {
                            rlen = this->_m_ext_name_len - 1;
                            return lastpart();
                        }

                        rlen = _m_length - this->_m_lastpart_start_pos - 1 - _m_ext_name_len;
                        return lastpart();
                    }

                    inline ::pilo::err_t get_basename(::pilo::char_buffer_t& ret) const
                    {
                        ::pilo::pathlen_t rlen = 0;
                        const char* p = basename(rlen);
                        if (p == nullptr)
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                        ret.check_space(rlen + 1);
                        ::pilo::core::string::n_copyz(ret.ptr(), ret.space_available(), p, rlen);
                        ret.set_size(rlen);
                        return PILO_OK;
                    }

                    inline const char* parentpath(::pilo::pathlen_t& rlen) const
                    {
                        if (_m_lastpart_start_pos == 0)
                        {
                            return nullptr;
                        }
                        if (_m_lastpart_start_pos == path::unknow_length)
                        {
                            return nullptr;
                        }
                        else
                        {
                            rlen = _m_lastpart_start_pos - 1;
                        }                        
                        return this->_m_pathstr_ptr;
                    }

                    inline std::string parentpath() const
                    {
                        ::pilo::pathlen_t rlen = 0;
                        const char* ptr = parentpath(rlen);
                        if (ptr == nullptr)
                            return "";
                        return std::string(ptr, (size_t)rlen);
                    }

                    inline ::pilo::err_t get_parentpath(::pilo::char_buffer_t& ret) const
                    {
                        ::pilo::pathlen_t rlen = 0;
                        const char* p = parentpath(rlen);
                        if (p == nullptr)
                            return ::pilo::make_core_error(PES_PATH_STR, PEP_FMT_FATAL);
                        ret.check_space(rlen + 1);
                        ::pilo::core::string::n_copyz(ret.ptr(), ret.space_available(), p, rlen);
                        ret.set_size(rlen);
                        return PILO_OK;
                    }

                    inline void reset()
                    {
                        if (_m_pathstr_ptr != nullptr)
                        {
                            PMF_HEAP_FREE(_m_pathstr_ptr);
                        }
                        _m_pathstr_ptr = nullptr;
                        _m_length = 0;
                        _m_capacity = 0;
                        _m_lastpart_start_pos = path::unknow_length;
                        _m_ext_name_len = path::invalid_ext_length;
                        _m_type = path::path_type_na;
                    }

                    inline void clear()
                    {
                        _m_length = 0;
                        _m_lastpart_start_pos = path::unknow_length;
                        _m_ext_name_len = path::invalid_ext_length;
                        _m_type = path::path_type_na;
                    }

                    inline bool is_root()
                    {
#ifdef WINDOWS
                        if (_m_type == path::local_fs_path && _m_pathstr_ptr != nullptr && _m_length == 6)
                        {
                            if (::pilo::core::string::strict_compare(_m_pathstr_ptr, 0, ::pilo::core::string::constants<char>::root_dir_sep(), 0, 4) == 0)
                            {
                                if (std::isalpha(_m_pathstr_ptr[4]) && _m_pathstr_ptr[5] == ':')
                                {
                                    return true;
                                }
                            }
                        }
#else
                        if (_m_type == path::local_fs_path && _m_pathstr_ptr != nullptr && _m_length == 0)
                        {
                            return true;
                        }
#endif // WINDOWS

                        return false;
                        
                    }
                    

                protected:
                    char*    _m_pathstr_ptr;
                    ::pilo::pathlen_t       _m_capacity;
                    ::pilo::pathlen_t       _m_length;
                    ::pilo::pathlen_t       _m_lastpart_start_pos;
                    ::pilo::u8_t            _m_ext_name_len;
                    ::pilo::i8_t            _m_type;


            }; //end of path


        }
    }
}



#endif //__pilo_io_path_hpp_