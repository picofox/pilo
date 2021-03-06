#include "dynamic_astring.hpp"
#include "core/string/string_util.hpp"

namespace pilo
{
    namespace core
    {
        namespace string
        {
            
            dynamic_astring::dynamic_astring(const char* str) : _m_pdata(nullptr), _m_size(0), _m_capacity(0)
            {
                _assign(str);
            }

            dynamic_astring::dynamic_astring(const char* str, size_t len) : _m_pdata(nullptr), _m_size(0), _m_capacity(0)
            {
                _assign(str, len);
            }

            dynamic_astring::dynamic_astring(const std::string& str) : _m_pdata(nullptr), _m_size(0), _m_capacity(0)
            {
                _assign(str.c_str(), str.length());
            }

            dynamic_astring::dynamic_astring(const dynamic_astring& str) : _m_pdata(nullptr), _m_size(0), _m_capacity(0)
            {
                if (this != &str)
                {
                    _assign(str.c_str(), str.length());
                }
            }

            char& dynamic_astring::operator[](size_t pos)
            {
                if (_m_size <= pos)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }

                return _m_pdata[pos];
            }            

            const char& dynamic_astring::operator[](size_t pos) const
            {
                if (_m_size <= pos)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }

                return _m_pdata[pos];
            }

            char& dynamic_astring::at(size_t pos)
            {
                if (_m_size <= pos)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                return _m_pdata[pos];
            }

            const char& dynamic_astring::at(size_t pos) const
            {
                if (_m_size <= pos)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                return _m_pdata[pos];
            }

            char& dynamic_astring::back()
            {
                if (0 == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                return _m_pdata[_m_size - 1];
            }

            const char& dynamic_astring::back() const
            {
                if (0 == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                return _m_pdata[_m_size - 1];
            }

            char& dynamic_astring::front()
            {
                if (0 == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_LOWER_LIMIT;
                }
                return _m_pdata[0];
            }

            const char& dynamic_astring::front() const
            {
                if (0 == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_LOWER_LIMIT;
                }
                return _m_pdata[0];
            }

            bool dynamic_astring::format(const char * fmt, ...)
            {
                va_list args;
                pilo::i32_t ret;

                va_start(args, fmt);
#               if defined(WINDOWS)
                ret = _vsnprintf_s(_m_pdata, capacity(), _TRUNCATE, fmt, args);
#               else
                ret = vsnprintf(_m_pdata, capacity(), fmt, args);
#               endif
                va_end(args);

                if (ret < 0)
                {
                    return false;
                }

                _m_size = static_cast<size_t>(ret);
                return true;
            }

            void dynamic_astring::recalculate_size()
            {
                if (_m_pdata == nullptr)
                {
                    _m_size = 0;
                }
                else
                {
                    _m_size = strlen(_m_pdata);
                }
            }

            pilo::core::string::dynamic_astring& dynamic_astring::assign(const char* str, size_t len)
            {
                if (pilo::EC_OK != _assign(str, len))
                {
                    this->clear();
                }
                return *this;
            }

            pilo::core::string::dynamic_astring& dynamic_astring::assign(const char* str)
            {
                if (pilo::EC_OK != _assign(str))
                {
                    this->clear();
                }
                return *this;
            }

            ::pilo::i32_t dynamic_astring::compare(const char* str, size_t len_to_compare /*= MC_INVALID_SIZE*/) const
            {
                return pilo::core::string::string_util::binary_compare(this->_m_pdata, str, len_to_compare);
            }            

            ::pilo::i32_t dynamic_astring::compare(const std::string& str) const
            {
                return pilo::core::string::string_util::binary_compare(this->_m_pdata, str.c_str(), MC_INVALID_SIZE);
            }

            

            pilo::core::string::dynamic_astring& dynamic_astring::operator=(const std::string& str)
            {
                _assign(str.c_str(), str.length());
                return *this;
            }

            pilo::core::string::dynamic_astring& dynamic_astring::operator=(const char* str)
            {
                _assign(str);
                return *this;
            }

            pilo::core::string::dynamic_astring& dynamic_astring::operator=(const dynamic_astring& str)
            {
                if (this != &str)
                {
                    _assign(str.c_str(), str.length());
                }
                
                return *this;                
            }

            pilo::i32_t dynamic_astring::_assign(const char* str)
            {                
                return _assign(str, MC_INVALID_SIZE);
            }

            pilo::i32_t dynamic_astring::_assign(const char* str, size_t len)
            {
                if (str == nullptr)
                {
                    if (_m_pdata == nullptr)
                    {
                        _reserve(0);
                    }

                    *_m_pdata = 0;
                    _m_size = 0;
                    return pilo::EC_OK;
                }

                size_t tmp_size = 0;
                size_t tmp_capa = 0;
                char* tmp_pdata = nullptr;


                if (len == MC_INVALID_SIZE)
                {
                    len = ::pilo::core::string::string_util::length(str);
                }

                if (*str == 0 || len == 0)
                {
                    tmp_size = 0;
                }
                else
                {
                    tmp_size = len;
                }

                tmp_capa = M_ALIGN_SIZE((tmp_size + 1), sizeof(void*));

                if (nullptr != _m_pdata)
                {
                    if (len <= _m_capacity) //use existing buffer;
                    {
                        if (tmp_size == 0) //set with empty string
                        {
                            *_m_pdata = 0;
                            _m_size = 0;
                        }
                        else
                        {
                            if (tmp_size != string_util::copy(_m_pdata, tmp_capa, str, tmp_size))
                            {
                                return pilo::EC_UNDEFINED;
                            }

                            _m_size = len;
                        }

                        return ::pilo::EC_OK;
                    }
                }

                tmp_pdata = (char*)malloc(tmp_capa);
                if (tmp_pdata == nullptr)
                {
                    return pilo::EC_INSUFFICIENT_MEMORY;
                }

                if (0 == tmp_size)
                {
                    *tmp_pdata = 0;
                }
                else
                {
                    if (tmp_size != string_util::copy(tmp_pdata, tmp_capa, str, tmp_size))
                    {
                        MP_SAFE_FREE(tmp_pdata);
                        return pilo::EC_UNDEFINED;
                    }
                }

                MP_SAFE_FREE(_m_pdata);
                _m_capacity = tmp_size;
                _m_pdata = tmp_pdata;
                _m_size = tmp_size;

                return ::pilo::EC_OK;
               
            }

            ::pilo::i32_t dynamic_astring::_push_back(char ch)
            {
                if (available_capacity() <= 0)
                {
                    if (::pilo::EC_OK != _reserve(_m_capacity + 1))
                    {
                        return ::pilo::EC_INSUFFICIENT_MEMORY;
                    }
                }

                _m_pdata[_m_size++] = ch;
                _m_pdata[_m_size] = 0;
                return ::pilo::EC_OK;
            }

            ::pilo::i32_t dynamic_astring::_pop_back()
            {
                if (_m_size <= 0)
                {
                    return ::pilo::EC_REACH_LOWER_LIMIT;
                }
                _m_pdata[--_m_size] = 0;
                return ::pilo::EC_OK;
            }

            ::pilo::i32_t dynamic_astring::_append(const char* suffix_str, size_t pos, size_t len)
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

                if (::pilo::EC_OK != _reserve(_m_capacity + len))
                {
                    return ::pilo::EC_INSUFFICIENT_MEMORY;
                }

                if (nullptr == ::pilo::core::string::string_util::concatenate_string(_m_pdata + _m_size, available_capacity()+1, suffix_str + pos, len))
                {
                    return ::pilo::EC_COPY_STRING_FAILED;
                }

                _m_size += len;

                return ::pilo::EC_OK;
            }

            ::pilo::i32_t dynamic_astring::_insert(size_t pos, const char* str, size_t len)
            {
                if (str == 0 || *str == 0)
                {
                    return ::pilo::EC_NULL_PARAM;
                }

                if (pos >= size())
                {
                    return _append(str, 0, len);
                }

                if (len == MC_INVALID_SIZE)
                {
                    len = strlen(str);
                }

                if (capacity() < len + length())
                {
                    if (::pilo::EC_OK != _reserve(length() + len))
                    {
                        return ::pilo::EC_INSUFFICIENT_MEMORY;
                    }
                }

                ::memmove(_m_pdata + pos + len, _m_pdata + pos, size() - pos);
                ::memmove(_m_pdata + pos, str, len);
                _m_size += len;
                _m_pdata[_m_size] = 0;

                return ::pilo::EC_OK;
            }

            ::pilo::error_number_t dynamic_astring::_reserve(size_t sz)
            {
                char* tmp_pdata = nullptr;
                size_t tmp_capa = M_ALIGN_SIZE((sz + 1), sizeof(void*));

                if (nullptr == _m_pdata)
                {
                    tmp_pdata = (char*)malloc(tmp_capa);
                    if (tmp_pdata == nullptr)
                    {
                        return pilo::EC_INSUFFICIENT_MEMORY;
                    }
                    _m_pdata = tmp_pdata;
                    _m_size = 0;
                    _m_capacity = sz;

                    return ::pilo::EC_OK;
                }
                else 
                {
                    if (sz < _m_capacity)
                    {
                        return ::pilo::EC_OK;
                    }

                    tmp_capa = M_ALIGN_SIZE((sz + 1), sizeof(void*));
                    tmp_pdata = (char*)malloc(tmp_capa);
                    if (tmp_pdata == nullptr)
                    {
                        return pilo::EC_INSUFFICIENT_MEMORY;
                    }

                    if (_m_size > 0)
                    {
                        if (_m_size != string_util::copy(tmp_pdata, tmp_capa, _m_pdata, _m_size))
                        {
                            MP_SAFE_FREE(tmp_pdata);
                            return pilo::EC_UNDEFINED;
                        }
                    }
                    

                    MP_SAFE_FREE(_m_pdata);
                    _m_capacity = sz;
                    _m_pdata = tmp_pdata;
                    
                }
                
                return pilo::EC_OK;
            }

        }
    }
}