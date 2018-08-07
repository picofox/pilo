#pragma once
#include "core/coredefs.hpp"
#include "core/string/string_util.hpp"
#include "core/string/auto_string.hpp"

#define MC_PILO_PATH_FLAG_VALID (1)

namespace pilo
{
    namespace core
    {
        namespace fs
        {
            class fs_util;

            template<size_t BUFFSZ_DEFALT>
            class path_string 
            {
            public:
                path_string() 
                {
                    _set_validity(false);
                }

                path_string(const char* str) : _m_string(str)
                {
                    if (_assign(str, MC_INVALID_SIZE) == ::pilo::EC_OK)
                    {
                        _set_validity(true);
                    }
                    else
                    {
                        _set_validity(false);
                    }
                }

                path_string(const char* str, size_t len) : _m_string(str, len)
                {
                    if (_assign(str, len) == ::pilo::EC_OK)
                    {
                        _set_validity(true);
                    }
                    else
                    {
                        _set_validity(false);
                    }
                }

                path_string(const std::string& str) : _m_string(str)
                {
                    if (_assign(str.c_str(), str.size()) == ::pilo::EC_OK)
                    {
                        _set_validity(true);
                    }
                    else
                    {
                        _set_validity(false);
                    }
                }

                template<size_t SZ>
                path_string(const path_string<SZ>& astr) : _m_string(astr)
                {
                    if (_assign(str.c_str(), str.size()) == ::pilo::EC_OK)
                    {
                        _set_validity(true);
                    }
                    else
                    {
                        _set_validity(false);
                    }
                }

                ::pilo::error_number_t assign(const char* cstr_path, size_t len = MC_INVALID_SIZE)
                {
                    return _assign(cstr_path, len);
                }

                const char* c_str() const
                {
                    if (!valid()) return nullptr;
                    return _m_string.c_str();
                }

                const char* last_part() const
                {
                    if (!valid()) return nullptr;
                    
                    const char* ret = ::strrchr(_m_string.c_str(), M_PATH_SEP_C);
                    if (ret == nullptr) return c_str();

                    return ret+1;
                }

                size_t last_part_length() const
                {
                    if (!valid()) return MC_INVALID_SIZE;

                    return ::strlen(last_part());
                }

                size_t base_length(bool inc_sep) const
                {
                    if (!valid()) return MC_INVALID_SIZE;

                    if (inc_sep)
                    {
                        return length() - last_part_length();
                    }
                    else
                    {
                        return length() - last_part_length() - 1;
                    }
                }

                size_t length() const
                {
                    if (!valid()) return MC_INVALID_SIZE;
                    return _m_string.size();
                }

                bool is_absolute() const
                {
                    if (!valid()) return false;
                    return ::pilo::core::fs::fs_util::is_absolute_path(_m_string.c_str());
                }

                void reset()
                {
                    _m_string.clear();
                    _m_flags = 0;
                }

                bool valid() const
                {
                    return ((_m_flags & MC_PILO_PATH_FLAG_VALID) != 0);
                }

                ::pilo::error_number_t  erase_last_part(bool need_sep, bool force_remove_root)
                {
                    if (!valid())
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }
                    const char* p = ::pilo::core::string::string_util::find_reversely(_m_string.c_str(), M_PATH_SEP_S, 1);
                    if (p == nullptr)
                    {
                        if (force_remove_root)
                        {
                            clear();
                            _set_validity(false);
                            return ::pilo::EC_OK;
                        }
                        return ::pilo::EC_OBJECT_NOT_FOUND;
                    }

                    size_t len = 0;
                    if (need_sep)
                    {
                        len = p - _m_string.c_str() + 1;
                    }
                    else
                    {
                        len = p - _m_string.c_str();
                    }

                    _m_string[len] = 0;
                    _m_string.recalculate_size();


                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t erase_last_seperator()
                {
                    if (!valid())
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

#ifdef WINDOWS
                    if (_m_string.back() == M_PATH_SEP_C)
#else
                    if (! _m_string.empty() && _m_string.back() == M_PATH_SEP_C)
#endif

                    {
                        _m_string._pop_back();
                    }

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t append_directory_seperator()
                {
                    if (!valid())
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }
#ifdef WINDOWS
                    if (_m_string.back() != M_PATH_SEP_C)
#else
                    if (_m_string.empty() || _m_string.back() != M_PATH_SEP_C)
#endif
                    {
                        _m_string.push_back(M_PATH_SEP_C);
                    }

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t append_part(const char* str, size_t len, bool need_sep)
                {

                    if (::pilo::EC_OK != append_part_without_validation(str, len, need_sep))
                    {
                        return ::pilo::EC_COPY_CONCAT_FAILED;
                    }

                    _set_validity(true);

                }

                ::pilo::error_number_t append_part_without_validation(const char* str, size_t len, bool need_sep)
                {
                    if (!valid())
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    if (str == nullptr)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    if (append_directory_seperator() != ::pilo::EC_OK)
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = ::pilo::core::string::string_util::length(str);
                    }

                    if (len == 0)
                    {
                        return ::pilo::EC_OK;
                    }

                    while (str[len - 1] == M_PATH_SEP_C)
                    {
                        len--;
                        if (len <= 0)
                        {
                            return ::pilo::EC_INVALID_PATH;
                        }
                    }

                    ::pilo::error_number_t ret = _m_string.append_string(str, 0, len);
                    if (ret != ::pilo::EC_OK)
                    {
                        return ret;
                    }

                    if (need_sep)
                    {
                        if (append_directory_seperator() != ::pilo::EC_OK)
                        {
                            return ::pilo::EC_INVALID_PATH;
                        }
                    }
                    else
                    {
                        
                    }

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t to_absolute(bool bAppendSep, bool bCompact)
                {
                    if (!valid())
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    if (!is_absolute())
                    {
                        char buffer_max[MC_PATH_MAX] = { 0 };
                        bool bUseHeap = false;
                        char* p = ::pilo::core::fs::fs_util::get_current_working_directory(buffer_max, sizeof(buffer_max), &bUseHeap, nullptr, true);
                        if (p == nullptr)
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                        size_t cwd_len = ::pilo::core::string::string_util::length(p);

                        _m_string.insert(0, p, cwd_len);
                        if (bUseHeap)
                        {
                            free(p);
                        }
                    } // end of is_absolute

                    if ((!_m_string.empty()) && (_m_string.back() == M_PATH_SEP_C))
                    {
                        _m_string.pop_back();
                    }

                    if (bCompact)
                    {
                        ::pilo::core::string::auto_string<char, BUFFSZ_DEFALT + 8> tmp_auto_string(_m_string.c_str(), _m_string.size());
                        ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::compact_path(tmp_auto_string.data(), tmp_auto_string.capacity());
                        if (ret != ::pilo::EC_OK)
                        {
                            return ::pilo::EC_INVALID_PATH;
                        }
                        _m_string.assign(tmp_auto_string.c_str(), tmp_auto_string.size());
                    }

                    if (bAppendSep)
                    {
                        int apd_ret = append_directory_seperator();
                        if (apd_ret != ::pilo::EC_OK)
                        {
                            return apd_ret;
                        }
                    }

                    return ::pilo::EC_OK;
                }


            protected:
                void _set_validity(bool is_valid)
                {
                    if (is_valid)
                    {
                        _m_flags |= MC_PILO_PATH_FLAG_VALID;
                    }
                    else
                    {
                        _m_flags &= (~MC_PILO_PATH_FLAG_VALID);
                    }
                }


                ::pilo::error_number_t _validate(const char* cstr_path, size_t len)
                {
                    ::pilo::core::string::auto_string<char, BUFFSZ_DEFALT> tmp_str_path;
                    if (::pilo::EC_OK != tmp_str_path.reserve(len + 3))
                    {
                        return ::pilo::EC_INSUFFICIENT_MEMORY;
                    }

                    ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::validate_and_parse_path_string( tmp_str_path.data(), 
                                                                                                            tmp_str_path.capacity(), 
                                                                                                            cstr_path, len);
                    if (ret != ::pilo::EC_OK)
                    {
                        _set_validity(false);
                        return ret;
                    }
                    tmp_str_path.recalculate_size();

                    _set_validity(true);

                    _m_string.assign(tmp_str_path.c_str(), tmp_str_path.size());

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _assign(const char* cstr_path, size_t len)
                {
                    if (cstr_path == nullptr)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    if (*cstr_path == 0 || len == 0)
                    {
                        reset();
                        return ::pilo::EC_OK;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = ::pilo::core::string::string_util::length(cstr_path);
                    }

                    if (::pilo::EC_OK != _validate(cstr_path, len))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    return ::pilo::EC_OK;
                }

            protected:           
                ::pilo::u32_t                                                   _m_flags;
                ::pilo::core::string::auto_string<char, BUFFSZ_DEFALT>          _m_string;                
            };           
        }
    }
}


