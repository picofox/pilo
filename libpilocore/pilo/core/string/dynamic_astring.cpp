#include "dynamic_astring.hpp"

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

            pilo::i32_t dynamic_astring::reserve(size_t sz)
            {
                if (_m_capacity <= sz)
                {
                    size_t tmpcapa = M_ALIGN_SIZE(sz + 1, sizeof(void*));
                    char * tmpbuff = (char*)malloc(tmpcapa);
                    if (tmpbuff == nullptr)
                    {
                        return pilo::EC_INSUFFICIENT_MEMORY;
                    }
                    if (_m_size != string_util::copy(tmpbuff, tmpcapa, _m_pdata, _m_size))
                    {
                        return pilo::EC_UNDEFINED;
                    }                    

                    _m_capacity = tmpcapa;   
                    MP_SAFE_FREE(_m_pdata);                   
                    _m_pdata = tmpbuff;                   
                }
                return pilo::EC_OK;
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
                return _assign(str, ::pilo::core::string::string_util::length(str));
            }

            pilo::i32_t dynamic_astring::_assign(const char* str, size_t len)
            {
                if (str == nullptr)
                {
                    _m_size = 0;
                    MP_SAFE_FREE(_m_pdata);
                    _m_capacity = 0;
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

                if (nullptr == _m_pdata)
                {
                    tmp_pdata = (char*)malloc(tmp_capa);
                    if (tmp_pdata == nullptr)
                    {
                        return pilo::EC_INSUFFICIENT_MEMORY;
                    }
                }
                else
                {
                    if (len >= _m_capacity)
                    {
                        tmp_pdata = (char*)malloc(tmp_capa);
                        if (tmp_pdata == nullptr)
                        {
                            return pilo::EC_INSUFFICIENT_MEMORY;
                        }
                    }
                }

                if (tmp_size == 0)
                {
                    *tmp_pdata = 0;
                }
                else
                {
                    if (tmp_size != string_util::copy(tmp_pdata, tmp_capa, str, len))
                    {
                        MP_SAFE_FREE(tmp_pdata);
                        return pilo::EC_UNDEFINED;
                    }
                }
                

                MP_SAFE_FREE(_m_pdata);
                _m_capacity = tmp_capa;
                _m_pdata = tmp_pdata;
                _m_size = tmp_size;


                return pilo::EC_OK;                
            }

            ::pilo::error_number_t dynamic_astring::_resize(size_t sz)
            {
                char* tmp_pdata = nullptr;
                size_t tmp_capa = M_ALIGN_SIZE((sz + 1), sizeof(void*));

                if (nullptr != _m_pdata)
                {
                    tmp_pdata = (char*)malloc(tmp_capa);
                    if (tmp_pdata == nullptr)
                    {
                        return pilo::EC_INSUFFICIENT_MEMORY;
                    }
                    _m_pdata = tmp_pdata;
                    _m_size = 0;
                    _m_capacity = tmp_capa;

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
                    _m_capacity = tmp_capa;                    
                    _m_pdata = tmp_pdata;
                    
                }
                
                return pilo::EC_OK;
            }

        }
    }
}