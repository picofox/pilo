#pragma once
#include "core/coredefs.hpp"
#include "core/string/string_util.hpp"
#include "core/string/astring.hpp"
#include "core/fs/fs_util.hpp"

#define MC_PILO_PATH_FLAG_VALID (1)

namespace pilo
{
    namespace core
    {
        namespace fs
        {
//             template<typename T, size_t BUFFSZ_DEFALT>
//             class path : private ::pilo::core::string
//             {
// 
//             };

            template<size_t MAX_PATH_SZ>
            class strpath
            {
                public:
                strpath()
                {
                    reset();
                }
                strpath(const char* str)
                {
                    reset();
                    assign(str);
                }
                strpath(const char* str, size_t len)
                {
                    reset();
                    assign(str, len);
                }

                const char* c_str() const
                {
                    if (!valid()) return nullptr;

                    return _m_str_path.c_str();
                }

                void reset()
                {
                    _m_str_path.clear();
                    _m_flags = 0;
                }

                size_t length() const
                {
                    if (!valid()) return MC_INVALID_SIZE;
                    return _m_str_path.size();
                }

                bool is_absolute() const
                {
                    if (!valid()) return false;
                    return ::pilo::core::fs::fs_util::is_absolute_path(_m_str_path.c_str());
                }                

                bool assign(const char* cstr_path, size_t len = MC_INVALID_SIZE)
                {
                    return (_assign(cstr_path, len) == ::pilo::EC_OK);                    
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

                        _m_str_path.insert(0, p, cwd_len);
                        if (bUseHeap)
                        {
                            free(p);
                        }
                    } // end of is_absolute

                    if ((!_m_str_path.empty()) && _m_str_path.back() == M_PATH_SEP_C)
                    {
                        _m_str_path.pop_back();
                    }


                    printf("path intermedia is (%s)\n", _m_str_path.c_str());

                    if (bCompact)
                    {
                        char buffer_max_tmp[MC_PATH_MAX] = { 0 };
                        ::pilo::core::string::string_util::copy(buffer_max_tmp, sizeof(buffer_max_tmp), _m_str_path.c_str(), _m_str_path.size());
                        ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::compact_path(buffer_max_tmp, _m_str_path.size());
                        if (ret != ::pilo::EC_OK)
                        {
                            printf("ret is  %d\n", ret);
                            return ::pilo::EC_INVALID_PATH;
                        }
                        _m_str_path.assign(buffer_max_tmp);
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

                ::pilo::error_number_t  erase_last_part(bool need_sep)
                {                
                    if (!valid())
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }
                    const char* p = ::pilo::core::string::string_util::find_reversely(_m_str_path.c_str(), M_PATH_SEP_S, 1);
                    if (p == nullptr)
                    {
                        return ::pilo::EC_OBJECT_NOT_FOUND;
                    }

                    size_t len = 0;
                    if (need_sep)
                    {
                        len  = p - _m_str_path.c_str() + 1;
                    }
                    else
                    {
                        len = p - _m_str_path.c_str();                       
                    }
                    
                    _m_str_path[len] = 0;
                    _m_str_path.recalculate_size();
                    

                    return ::pilo::EC_OK;
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

                    if (append_directory_seperator() != ::pilo::EC_OK )
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = strlen(str);
                    }

                    while (str[len - 1] == M_PATH_SEP_C)
                    {
                        str[len - 1] = 0;
                        len--;
                        if (len == 1)
                        {
                            return ::pilo::EC_INVALID_PATH;
                        }
                    }                    

                    ::pilo::error_number_t ret = _m_str_path.append(str, 0, len);
                    if ( ret  != ::pilo::EC_OK)
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

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t append_directory_seperator()
                {
                    if (!valid())
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }
#ifdef WINDOWS
                    if (_m_str_path.back() != M_PATH_SEP_C)
#else
                    if (_m_str_path.empty() || _m_str_path.back() != M_PATH_SEP_C)
#endif
                    {
                        _m_str_path.push_back(M_PATH_SEP_C);
                    }

                    return ::pilo::EC_OK;
                }

                bool valid() const
                {
#ifdef WINDOWS
                    if (_m_str_path.empty()) return false;
#endif // WINDOWS
                    return ((_m_flags & MC_PILO_PATH_FLAG_VALID) != 0);
                }

            protected:
                ::pilo::error_number_t _validate(const char* cstr_path, size_t len)
                {
                    ::pilo::core::string::astring<MAX_PATH_SZ> tmp_str_path;
                    if (::pilo::EC_OK != tmp_str_path.reserve(len + 3))
                    {
                        return ::pilo::EC_INSUFFICIENT_MEMORY;
                    }

                    ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::validate_and_parse_path_string(tmp_str_path.data(), tmp_str_path.capacity(), cstr_path, len);
                    if (ret != ::pilo::EC_OK)
                    {                        
                        _set_validity(false);
                        return ret;
                    }
                    tmp_str_path.recalculate_size();

                    _set_validity(true);
                    _m_str_path = tmp_str_path;
                    return ::pilo::EC_OK;
                }

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
                        len = strlen(cstr_path);
                    }                    

                    if (::pilo::EC_OK != _validate(cstr_path, len))
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    return ::pilo::EC_OK;
                }

            protected:
                ::pilo::core::string::astring<MAX_PATH_SZ> _m_str_path;
                ::pilo::u32_t                              _m_flags;
            };
        }
    }
}


