#pragma once
#include "core/coredefs.hpp"
#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/fs/fs_util.hpp"

namespace pilo
{
    namespace core
    {
        namespace fs
        {
            template<size_t MAX_PATH_SZ>
            class path
            {
            public:
                path() 
                { 
                    reset(); 
                }
                path(const char* str)
                {
                    reset();
                    set(str);
                }

                bool valid() const { return !_m_str_path.empty(); }

                const char* c_str() const { return _m_str_path.c_str(); }
                size_t length() const { return _m_str_path.size(); }
                bool is_absolute() const
                {
                    return ::pilo::core::fs::fs_util::is_absolute_path(_m_str_path.c_str());
                }

                void reset()
                {
                    _m_str_path.clear();
                }                

                bool set(const char* cstr_path)
                {
                    if (cstr_path == nullptr) return false;
                    if (*cstr_path == 0) return false;

                    size_t len = strlen(cstr_path);
                    if (_m_str_path.capacity() <= len)
                    {
                        return false;
                    }

                    if (::pilo::EC_OK != validate(cstr_path))
                    {
                        return false;
                    }

                    return true;
                }

                ::pilo::error_number_t to_absolute(bool bAppendSep, bool bCompact)
                {
                    if (!valid())
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    if (! is_absolute())
                    {
                        char buffer_max[MAX_PATH_SZ] = { 0 };
                        bool bUseHeap = false;
                        char* p = ::pilo::core::fs::fs_util::get_current_working_directory(buffer_max, sizeof(buffer_max), &bUseHeap, nullptr, true);
                        if (p == nullptr)
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                        size_t cwd_len = ::pilo::core::string::string_util::length(p);

                        if (MAX_PATH_SZ <= cwd_len + _m_str_path.size())
                        {
                            return ::pilo::EC_BUFFER_TOO_SMALL;
                        }

                        bool bInsertRet = _m_str_path.insert(0, p, cwd_len);
                        if (bUseHeap)
                        {
                            free(p);
                        }
                        if (!bInsertRet)
                        {
                            return ::pilo::EC_COPY_STRING_FAILED;
                        }                        
                    } // end of is_absolute

                    if (_m_str_path.back() == M_PATH_SEP_C)
                    {
                        _m_str_path.pop_back();
                    }

                    if (bCompact)
                    {
                        char buffer_max_tmp[MAX_PATH_SZ] = { 0 };
                        ::pilo::core::string::string_util::copy(buffer_max_tmp, sizeof(buffer_max_tmp), _m_str_path.c_str(), _m_str_path.size());
                        ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::compact_path(buffer_max_tmp, _m_str_path.size());
                        if (ret != ::pilo::EC_OK)
                        {
                            return ::pilo::EC_INVALID_PATH;
                        }
                        _m_str_path.assign(buffer_max_tmp);
                    }

                    if (bAppendSep)
                    {
                        int apd_ret = append_directory_seperator();
                        if ( apd_ret != ::pilo::EC_OK)
                        {
                            return apd_ret;
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
                    
                    if (_m_str_path.back() != M_PATH_SEP_C)
                    {
                        if (_m_str_path.available_capacity() <= 0)
                        {
                            return ::pilo::EC_BUFFER_TOO_SMALL;
                        }
                        _m_str_path.push_back(M_PATH_SEP_C);
                    }

                    return ::pilo::EC_OK;
                }

            protected:
                ::pilo::error_number_t validate(const char* cstr_path)
                {
                    char buffer[MAX_PATH_SZ] = { 0 };
                    ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::validate_and_parse_path_string(buffer, cstr_path);
                    if (ret != ::pilo::EC_OK)
                    {
                        return ret;
                    }
                    _m_str_path = buffer;
                    return ::pilo::EC_OK;
                }

            protected:
                ::pilo::core::string::fixed_astring<MAX_PATH_SZ> _m_str_path;
            };

            template<>
            class path<0>
            {
            public:
                path()
                {
                    reset();
                }
                path(const char* str)
                {
                    reset();
                    set(str);
                }

                bool valid() const { return !_m_str_path.empty(); }
                const char* c_str() const { return _m_str_path.c_str(); }
                size_t length() const { return _m_str_path.size(); }
                bool is_absolute() const
                {
                    return ::pilo::core::fs::fs_util::is_absolute_path(_m_str_path.c_str());
                }

                void reset()
                {
                    _m_str_path.clear();
                }                

                bool set(const char* cstr_path)
                {
                    if (cstr_path == nullptr) return false;
                    if (*cstr_path == 0) return false;

                    if (::pilo::EC_OK != validate(cstr_path))
                    {
                        return false;
                    }

                    return true;
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

                    if (_m_str_path.back() == M_PATH_SEP_C)
                    {
                        _m_str_path.pop_back();
                    }

                    if (bCompact)
                    {
                        char* path_ptr = (char*) malloc(_m_str_path.size() + 1);
                        if (path_ptr == nullptr)
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                        ::pilo::core::string::string_util::copy(path_ptr, _m_str_path.size()+1, _m_str_path.c_str(), _m_str_path.size());
                        ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::compact_path(path_ptr, _m_str_path.size());
                        if (ret != ::pilo::EC_OK)
                        {
                            free(path_ptr);
                            return ::pilo::EC_INVALID_PATH;
                        }

                        _m_str_path.assign(path_ptr);
                        free(path_ptr);
                        
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

                ::pilo::error_number_t append_directory_seperator()
                {
                    if (!valid())
                    {
                        return ::pilo::EC_INVALID_PATH;
                    }

                    if (_m_str_path.back() != M_PATH_SEP_C)
                    {
                        _m_str_path += M_PATH_SEP_S;
                    }

                    return ::pilo::EC_OK;
                }

            protected:
                ::pilo::error_number_t validate(const char* cstr_path)
                {
                    char * pDynBuffer = nullptr;
                    size_t dstlen = ::strlen(cstr_path) + 4;
                    pDynBuffer = (char*)malloc(dstlen);
                    if (pDynBuffer == nullptr)
                    {
                        return ::pilo::EC_INSUFFICIENT_MEMORY;
                    }

                    ::pilo::error_number_t ret = ::pilo::core::fs::fs_util::validate_and_parse_path_string(pDynBuffer, dstlen, cstr_path);
                    if (ret != ::pilo::EC_OK)
                    {
                        return ret;
                    }

                    _m_str_path = pDynBuffer;
                    free(pDynBuffer);

                    return ::pilo::EC_OK;
                }

            protected:
                std::string     _m_str_path;
            };
        }
    }
}


