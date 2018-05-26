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

                inline void push_back(char ch) { _push_back(ch); }
                inline void pop_back() { _pop_back(); }
                

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

                char* data()                 {
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

                astring& append(const char* suffix_str, size_t pos, size_t len)
                {
                    _append(suffix_str, pos, len);
                    return *this;
                }

                astring& append(const std::string& stdstr )
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }

                astring& append(const char* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }

                astring& append(const astring& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
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


                ::pilo::i32_t compare(const char* str, size_t len_to_compare = MC_INVALID_SIZE) const
                {
                    return pilo::core::string::string_util::binary_compare(this->c_str(), str, len_to_compare);
                }

                ::pilo::i32_t compare(const std::string& str) const
                {
                    return pilo::core::string::string_util::binary_compare(this->c_str(), str.c_str(), MC_INVALID_SIZE);
                }

                astring insert(size_t pos, const char* str, size_t len)
                {
                    _insert(pos, str, len );
                    return *this;
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

                char* _data() 
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

                        _m_dyn_capacity = sz;
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
                        _m_dyn_capacity = sz;
                    }                    

                    return pilo::EC_OK;
                }

                ::pilo::error_number_t _assign(const char* cstr, size_t len)
                {
                    if (cstr == nullptr) // null str mean clear data, restore to fixxed buffer
                    {
                        _m_dyn_capacity = 0;
                        MP_SAFE_FREE(_m_dyn_data);
                        _m_size = 0;
                        *_m_fix_data = 0;                        
                        return ::pilo::EC_OK;
                    }

                    if (len == MC_INVALID_SIZE) //auto calculate src str length
                    {
                        len = ::pilo::core::string::string_util::length(cstr);
                    }

                    if (0 == *cstr || len == 0) //clear data
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
                    
                    size_t neo_capa = 0;
                    size_t neo_size = 0;
                    char * neo_buffer = nullptr;

                    if (nullptr == _m_dyn_data) //dyn buffer is not in using , 2 cases, one is still user fixed buffer, the other is switch to dyn buffer
                    {
                        if (len > max_capacity)
                        {                            
                            neo_capa = M_ALIGN_SIZE((len + 1), sizeof(void*));
                            neo_buffer = (char*) malloc (neo_capa);
                            if (neo_buffer == nullptr)
                            {
                                return ::pilo::EC_INSUFFICIENT_MEMORY;
                            }

                            neo_size = string_util::copy(neo_buffer, neo_capa, cstr, len);
                            if (neo_size == MC_INVALID_SIZE)
                            {
                                return ::pilo::EC_COPY_STRING_FAILED;
                            }

                            _m_dyn_data = neo_buffer;
                            _m_size = neo_size;
                            _m_dyn_capacity = len;

                        }
                        else
                        {
                            _m_size = string_util::copy(_m_fix_data, max_capacity + 1, cstr, len);
                        }
                    }
                    else //using dyn buffer
                    {
                        if (len > _m_dyn_capacity)
                        {
                            neo_capa = M_ALIGN_SIZE((len + 1), sizeof(void*));
                            neo_buffer = (char*)malloc(neo_capa);
                            if (neo_buffer == nullptr)
                            {
                                return ::pilo::EC_INSUFFICIENT_MEMORY;
                            } 

                            if (string_util::copy(neo_buffer, neo_capa, cstr, len) != len)
                            {
                                return ::pilo::EC_COPY_STRING_FAILED;
                            }

                            _m_size = len;
                            _m_dyn_capacity = len;
                            _m_dyn_data = neo_buffer;
                        }
                        else
                        {
                            _m_size = string_util::copy(_m_dyn_data, _m_dyn_capacity+1, cstr, len);
                            if (_m_size != len)
                            {
                                return ::pilo::EC_COPY_STRING_FAILED;
                            }
                        }
                    }                   

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _push_back(char ch)
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

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _pop_back()
                {
                    if (_m_size <= 0)
                    {
                        return ::pilo::EC_REACH_LOWER_LIMIT;
                    }

                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        _m_dyn_data[--_m_size] = 0;
                    }
                    else
                    {
                        _m_fix_data[--_m_size] = 0;
                    }

                    return ::pilo::EC_OK;
                }

                ::pilo::i32_t _append(const char* suffix_str, size_t pos, size_t len)
                {
                    if (suffix_str == nullptr)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = ::strlen(suffix_str);
                    }

                    if (len == 0)
                    {
                        return ::pilo::EC_OK;
                    }

                    if (pos >= len)
                    {
                        return ::pilo::EC_INVALID_PARAM;
                    }

                    if (capacity() < len + length())
                    {
                        if (::pilo::EC_OK != _resize(length() + len))
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                    }                    

                    if (nullptr == ::pilo::core::string::string_util::concatenate_string(_data() + _m_size, available_capacity()+1, suffix_str + pos, len))
                    {
                        return ::pilo::EC_COPY_STRING_FAILED;
                    }

                    return ::pilo::EC_OK;
                }

                ::pilo::i32_t _insert(size_t pos, const char* str, size_t len)
                {
                    if (str == 0 || *str == 0)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    if (pos >= size())
                    {
                        return _append(str, 0,  len);
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = strlen(str);
                    }

                    if (capacity() < len + length())
                    {
                        if (::pilo::EC_OK != _resize(length() + len))
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                    }

                    ::memmove(data() + pos + len, data() + pos, size() - pos);
                    ::memmove(data() + pos, str, len);
                    _m_size += len;
                    data()[_m_size] = 0;

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
                astring() : dynamic_astring()
                {

                }

                astring(const char* cstr) : dynamic_astring(cstr)
                {

                }

                astring(const char* cstr, size_t len) : dynamic_astring(cstr, len)
                {
                    
                }

                astring(const std::string& stdstr) : dynamic_astring(stdstr)
                {

                }

                astring(const astring<0>& astr) : dynamic_astring(astr.c_str(), astr.length())
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