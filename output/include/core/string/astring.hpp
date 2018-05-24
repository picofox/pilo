#pragma once

#include "../coredefs.hpp"
#include "string_util.hpp"
#include <string>
#include "core/io/fixed_memory_ostream.hpp"
#include "core/io/fixed_memory_istream.hpp"
#include "core/io/memory_istream_adapter.hpp"
#include "core/string/dynamic_astring.hpp"

namespace pilo
{
    namespace core
    {
        namespace string
        {
            template<size_t max_capacity> 
            class astring
            {
            public:
                astring()
                {
                    if (max_capacity > (size_t)MC_FIXEDSTRING_MAX_ELEMENTS)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                }

                astring(const char* cstr, size_t len)
                {
                    _assign(cstr, len);
                }

                astring(const char* cstr)
                {
                    _assign(cstr, MC_INVALID_SIZE);
                }

                astring(const std::string& stdstr)
                {
                    _assign(stdstr.c_str(), stdstr.size());
                }

                template<size_t capa> astring(const astring<capa>& str)
                {
                    _assign(str.c_str(), str.size());
                }

                astring<max_capacity>& operator=(const astring<max_capacity>& str)
                {
                    if (this != &str)
                    {
                        _assign(str.c_str(), str.size());
                    }
                    return *this;
                }

                astring<max_capacity>& operator=(const std::string& str)
                {                    
                    _assign(str.c_str(), str.size());                   
                    return *this;
                }

                astring<max_capacity>& operator=(const char* cstr)
                {
                    _assign(cstr, MC_INVALID_SIZE);
                    return *this;
                }
                

                ::pilo::error_number_t assign(const char* cstr, size_t len)
                {
                    return _assign(cstr, len);
                }
                
                ::pilo::error_number_t assign(const char* cstr)
                {
                    return _assign(cstr, MC_INVALID_SIZE);
                }

                ::pilo::error_number_t assign(const std::string& stdstr )
                {
                    return _assign(stdstr.c_str(), stdstr.size());
                }

                bool is_dynamic() const
                {
                    return (_m_dyn_data != nullptr);
                }

                const char* c_str() const
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        return _m_dyn_data;
                    }
                    else
                    {
                        return _m_fix_data;
                    }
                }

                size_t size() const
                {
                    return _m_size;
                }

                size_t length() const
                {
                    return _m_size;
                }

                char& at(size_t pos)
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                const char& at(size_t pos) const
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }

                char& back()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }
                const char& back() const
                {

                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }

                char& front()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }

                const char& front() const
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }

                size_t available_capacity() const
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        return _m_dyn_capacity - _m_size;
                    }
                    else
                    {
                        return max_capacity - _m_size;
                    }
                }

                void clear()
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        *_m_dyn_data = 0;
                    }
                    else
                    {
                        *_m_fix_data = 0;
                    }

                    _m_size = 0;
                }

                size_t capacity() const
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        return _m_dyn_capacity;
                    }
                    else
                    {
                        return max_capacity;
                    }
                }

                ::pilo::error_number_t push_back(char ch)
                {
                    ::pilo::error_number_t ret = ::pilo::EC_OK;
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        if (available_capacity() <= 0)
                        {
                            ret = _resize(_m_dyn_capacity + 1);
                            if (ret != ::pilo::EC_OK)
                            {
                                return ret;
                            }                            
                        }

                        _m_dyn_data[_m_size++] = ch;
                        _m_dyn_data[_m_size] = 0;
                    }
                    else
                    {
                        if (available_capacity() <= 0)
                        {
                            ret = _resize(max_capacity + 1);
                            if (ret != ::pilo::EC_OK)
                            {
                                return ret;
                            }
                            _m_dyn_data[_m_size++] = ch;
                            _m_dyn_data[_m_size] = 0;
                        }
                        else
                        {
                            _m_fix_data[_m_size++] = ch;
                            _m_fix_data[_m_size] = 0;
                        }
                    }
                    
                }

                void pop_back()
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        _m_dyn_data[--_m_size] = 0;
                    }
                    else
                    {
                        _m_fix_data[--_m_size] = 0;
                    }                    
                }

                astring<max_capacity> insert(size_t pos, const char* str, size_t len)
                {
                    if (insert_or_append(pos, str, len) != ::pilo::EC_OK)
                    {
                        throw ::pilo::EC_INSUFFICIENT_MEMORY;
                    }
                    return *this;
                }

                ::pilo::error_number_t insert_or_append(size_t pos, const char* str, size_t len)
                {
                    if (str == 0 || *str == 0)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = strlen(str);
                    }

                    if (available_capacity() < len)
                    {
                        ::pilo::error_number_t resz_ret = _resize(_m_size + len);
                        if (resz_ret != ::pilo::EC_OK)
                        {
                            return resz_ret;
                        }
                    }

                    if (capacity() <= len + size())
                    {
                        return false;
                    }

                    char * p = nullptr;
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        p = _m_dyn_capacity;
                    }
                    else
                    {
                        p = max_capacity;
                    }

                    if (pos >= _m_size)
                    {
                        string_util::copy(p + _m_size, MC_INVALID_SIZE, str, len);
                    }
                    else
                    {
                        ::memmove(p + pos + len, p + pos, _m_size - pos);
                        ::memmove(p + pos, str, len);
                        _m_size += len;
                        p[_m_size] = 0;
                    }

                    return ::pilo::EC_OK;
                }

            protected:
                char& _at(size_t pos)
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        return _m_dyn_data[pos];
                    }
                    else
                    {
                        return _m_fix_data[pos];
                    }
                }

                

                ::pilo::error_number_t _resize(size_t sz)
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        if (sz <= _m_dyn_capacity)
                        {
                            return ::pilo::EC_OK;
                        }

                        size_t tmpcapa = M_ALIGN_SIZE((sz+1), sizeof(void*));
                        char * tmpbuff = (char*)malloc(tmpcapa);
                        if (tmpbuff == nullptr)
                        {
                            return pilo::EC_INSUFFICIENT_MEMORY;
                        }

                        if (_m_size != string_util::copy(tmpbuff, tmpcapa, _m_dyn_data, _m_size))
                        {
                            MP_SAFE_FREE(_m_dyn_data);
                            return pilo::EC_UNDEFINED;
                        }

                        _m_dyn_capacity = tmpcapa;
                        MP_SAFE_FREE(_m_dyn_data);
                        _m_dyn_data = tmpbuff;
                    }
                    else
                    {
                        if (sz <= max_capacity)
                        {
                            return ::pilo::EC_OK;
                        }

                        size_t tmpcapa = M_ALIGN_SIZE((sz+1), sizeof(void*));
                        char * _m_dyn_data = (char*)malloc(tmpcapa);
                        if (_m_dyn_data == nullptr)
                        {
                            return pilo::EC_INSUFFICIENT_MEMORY;
                        }

                        if (_m_size != string_util::copy(_m_dyn_data, tmpcapa, _m_fix_data, _m_size))
                        {
                            MP_SAFE_FREE(_m_dyn_data);
                            return pilo::EC_UNDEFINED;
                        }
                        _m_dyn_capacity = tmpcapa;
                    }                    

                    return pilo::EC_OK;
                }

                ::pilo::error_number_t _assign(const char* cstr, size_t len)
                {
                    if (cstr == nullptr)
                    {
                        _m_dyn_capacity = 0;
                        MP_SAFE_FREE(_m_dyn_data);
                        _m_size = 0;
                        *_m_fix_data = 0;                        
                        return ::pilo::EC_OK;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = ::pilo::core::string::string_util::length(cstr);
                    }

                    if (0 == *cstr || len == 0)
                    {
                        _m_size = 0;
                        if (nullptr != _m_dyn_data)
                        {
                            *_m_dyn_data = 0;
                            return ::pilo::EC_OK;
                        }
                        else
                        {
                            *_m_fix_data = 0;                            
                            return ::pilo::EC_OK;
                        }
                    }
                   
                    if (nullptr == _m_dyn_data)
                    {
                        if (len >= max_capacity)
                        {
                            ::pilo::error_number_t ret = _resize(len + 1);
                            if (ret != ::pilo::EC_OK)
                            {
                                return ret;
                            }
                            _m_size = string_util::copy(_m_dyn_data, len + 1, cstr, len);
                        }
                        else
                        {
                            _m_size = string_util::copy(_m_fix_data, max_capacity + 1, cstr, len);
                        }
                    }
                    else
                    {
                        if (len >= _m_dyn_capacity)
                        {
                            ::pilo::error_number_t ret = _resize(len + 1);
                            if (ret != ::pilo::EC_OK)
                            {
                                return ret;
                            }
                            _m_size = string_util::copy(_m_dyn_data, len + 1, cstr, len);
                        }
                        else
                        {
                            _m_size = string_util::copy(_m_dyn_data, _m_dyn_capacity+1, cstr, len);
                        }
                    }                   

                    return ::pilo::EC_OK;
                }


            protected:
                size_t          _m_dyn_capacity;
                char*           _m_dyn_data;
                size_t          _m_size;
                char            _m_fix_data[max_capacity + 1];
                
            };

            template<>
            class astring<0> : public dynamic_astring
            {
            public:
            astring(const char* cstr) : dynamic_astring(cstr)
            {

            }
    
                bool is_dynamic() const
                {
                    return true;
                }

            };
        }
    }
}