#pragma once

#include "../coredefs.hpp"
#include "string_util.hpp"
#include <string>
#include "core/io/fixed_memory_ostream.hpp"
#include "core/io/fixed_memory_istream.hpp"
#include "core/io/memory_istream_adapter.hpp"
#include "core/string/dynamic_astring.hpp"
#include <wchar.h>

namespace pilo
{
    namespace core
    {
        namespace string
        {
            //----------------------------------------------------------------------------------------------//
            //--------------------------------------------||------------------------------------------------//
            //------------------------------[auto_string<char, BUFSZ_DFL>]----------------------------------//
            //--------------------------------------------||------------------------------------------------//
            //--------------------------------------------||------------------------------------------------//
            //--------------------------------------------\/------------------------------------------------//


            template <typename CHAR_T, size_t BUFSZ_DFL>
            class auto_string
            {

            };

            template<size_t BUFSZ_DFL>
            class auto_string<char, BUFSZ_DFL>
            {
            public:
                typedef char value_type;
            public:
                auto_string()
                {
                    if (BUFSZ_DFL > (size_t)MC_FIXEDSTRING_MAX_ELEMENTS)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                }
                auto_string(const value_type* cstr, size_t len)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(cstr, len);
                }
                auto_string(const value_type* cstr)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(cstr, MC_INVALID_SIZE);
                }
                auto_string(const std::string& stdstr)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(stdstr.c_str(), stdstr.size());
                }
                auto_string(const auto_string<value_type, BUFSZ_DFL>& astr)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(astr.c_str(), astr.size());
                }
                template<size_t SZ>
                auto_string(const auto_string<value_type, SZ>& astr)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(astr.c_str(), astr.size());
                }

                auto_string& operator=(const value_type* str)
                {
                    _assign(str, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& operator=(const std::string& stdstr)
                {
                    _assign(stdstr.c_str(), stdstr.size());
                    return *this;
                }
                auto_string& operator=(const auto_string<value_type, BUFSZ_DFL>& astr)
                {
                    _assign(astr.c_str(), astr.size());
                    return *this;
                }
                

                bool is_dynamic() const
                {
                    return (_m_dyn_data != nullptr);
                }
                const value_type* c_str() const
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

                value_type* data()
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

                size_t size() const { return _m_size; }
                size_t length() const { return _m_size; }
                size_t capacity() const
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        return _m_dyn_capacity;
                    }
                    else
                    {
                        return BUFSZ_DFL;
                    }
                }

                ::pilo::error_number_t assign(const value_type* cstr, size_t len) { return _assign(cstr, len); }


            protected:
                ::pilo::error_number_t _reserve(size_t sz)
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        if (sz <= _m_dyn_capacity)
                        {
                            return ::pilo::EC_OK;
                        }

                        size_t tmpcapa = M_ALIGN_SIZE((sz + 1), sizeof(void*));
                        value_type * tmpbuff = (value_type*)malloc(tmpcapa);
                        if (tmpbuff == nullptr)
                        {
                            return pilo::EC_INSUFFICIENT_MEMORY;
                        }

                        if (_m_size > 0)
                        {
                            if (_m_size != string_util::copy(tmpbuff, tmpcapa, _m_dyn_data, _m_size))
                            {
                                MP_SAFE_FREE(_m_dyn_data);
                                return pilo::EC_UNDEFINED;
                            }
                        }
                        

                        _m_dyn_capacity = sz;
                        MP_SAFE_FREE(_m_dyn_data);
                        _m_dyn_data = tmpbuff;
                    }
                    else
                    {
                        if (sz <= BUFSZ_DFL)
                        {
                            return ::pilo::EC_OK;
                        }

                        size_t tmpcapa = M_ALIGN_SIZE((sz + 1), sizeof(void*));
                        value_type * _m_dyn_data = (value_type*)malloc(tmpcapa);
                        if (_m_dyn_data == nullptr)
                        {
                            return pilo::EC_INSUFFICIENT_MEMORY;
                        }

                        if (_m_size > 0)
                        {
                            if (_m_size != string_util::copy(_m_dyn_data, tmpcapa, _m_fix_data, _m_size))
                            {
                                MP_SAFE_FREE(_m_dyn_data);
                                return pilo::EC_UNDEFINED;
                            }
                        }
                        
                        _m_dyn_capacity = sz;
                    }

                    return pilo::EC_OK;
                }

                ::pilo::error_number_t _assign(const value_type* cstr, size_t len)
                {
                    if (cstr == nullptr) // null str mean clear data, restore to fixed buffer
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
                    value_type * neo_buffer = nullptr;

                    if (nullptr == _m_dyn_data) //dyn buffer is not in using , 2 cases, one is still user fixed buffer, the other is switch to dyn buffer
                    {
                        if (len > BUFSZ_DFL)
                        {
                            neo_capa = M_ALIGN_SIZE((len + 1), sizeof(void*));
                            neo_buffer = (value_type*)malloc(neo_capa);
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
                            _m_size = string_util::copy(_m_fix_data, BUFSZ_DFL + 1, cstr, len);
                        }
                    }
                    else //using dyn buffer
                    {
                        if (len > _m_dyn_capacity)
                        {
                            neo_capa = M_ALIGN_SIZE((len + 1), sizeof(void*));
                            neo_buffer = (value_type*)malloc(neo_capa);
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
                            _m_size = string_util::copy(_m_dyn_data, _m_dyn_capacity + 1, cstr, len);
                            if (_m_size != len)
                            {
                                return ::pilo::EC_COPY_STRING_FAILED;
                            }
                        }
                    }

                    return ::pilo::EC_OK;
                }

            protected:
                size_t          _m_dyn_capacity;
                value_type*     _m_dyn_data;
                size_t          _m_size;
                value_type      _m_fix_data[BUFSZ_DFL + 1];
            };





            //----------------------------------------------------------------------------------------------//
            //--------------------------------------------||------------------------------------------------//
            //-----------------------------------[auto_string<char, 0>]-------------------------------------//
            //--------------------------------------------||------------------------------------------------//
            //--------------------------------------------||------------------------------------------------//
            //--------------------------------------------\/------------------------------------------------//


            template<>
            class auto_string<char, 0>
            {
            public:
                typedef char value_type;

            public:
                auto_string()
                {
                    _m_pdata = nullptr;
                    _reserve(0);
                    _m_pdata[0] = 0;
                    _m_size = 0;                    
                }
                auto_string(const value_type* cstr, size_t len)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(cstr, len);
                }
                auto_string(const value_type* cstr)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(cstr, MC_INVALID_SIZE);
                }
                auto_string(const std::string& stdstr)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(stdstr.c_str(), stdstr.size());
                }
                auto_string(const auto_string<value_type, 0>& astr)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(astr.c_str(), astr.size());
                }
                template<size_t SZ>
                auto_string(const auto_string<value_type, SZ>& astr)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(astr.c_str(), astr.size());
                }

                auto_string& operator=(const value_type* str)
                {
                    _assign(str, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& operator=(const std::string& stdstr)
                {
                    _assign(stdstr.c_str(), stdstr.size());
                    return *this;
                }
                auto_string& operator=(const auto_string<value_type, 0>& astr)
                {
                    _assign(astr.c_str(), astr.size());
                    return *this;
                }

                bool is_dynamic() const { return true; }
                const value_type* c_str() const { return _m_pdata; }
                value_type* data() { return _m_pdata; }
                size_t size() const { return _m_size; }
                size_t length() const { return _m_size; }
                size_t capacity() const { return _m_capacity; }
                ::pilo::error_number_t assign(const value_type* cstr, size_t len) { return _assign(cstr, len); }

            protected:
                ::pilo::error_number_t _reserve(size_t sz)
                {
                    value_type* tmp_pdata = nullptr;
                    size_t tmp_capa = M_ALIGN_SIZE((sz + 1), sizeof(void*));

                    if (nullptr == _m_pdata)
                    {
                        tmp_pdata = (value_type*)malloc(tmp_capa);
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
                        tmp_pdata = (value_type*)malloc(tmp_capa);
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

                pilo::i32_t _assign(const value_type* str, size_t len)
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
                    value_type* tmp_pdata = nullptr;


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

                    tmp_pdata = (value_type*)malloc(tmp_capa);
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

            protected:
                value_type* _m_pdata;
                size_t      _m_size;
                size_t      _m_capacity;
            };




            //----------------------------------------------------------------------------------------------//
            //--------------------------------------------||------------------------------------------------//
            //----------------------------[auto_string<wchar_t, BUFSZ_DFL>]---------------------------------//
            //--------------------------------------------||------------------------------------------------//
            //--------------------------------------------||------------------------------------------------//
            //--------------------------------------------\/------------------------------------------------//


            template<size_t BUFSZ_DFL>
            class auto_string<wchar_t, BUFSZ_DFL>
            {
            public:
                typedef wchar_t value_type;

            public:
                auto_string()
                {
                    if (BUFSZ_DFL > (size_t)MC_FIXEDSTRING_MAX_ELEMENTS)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                }
                auto_string(const value_type* cstr, size_t len)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(cstr, len);
                }
                auto_string(const value_type* cstr)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(cstr, MC_INVALID_SIZE);
                }
                auto_string(const std::wstring& stdstr)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(stdstr.c_str(), stdstr.size());
                }
                auto_string(const auto_string<value_type, BUFSZ_DFL>& astr)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(astr.c_str(), astr.size());
                }
                template<size_t SZ>
                auto_string(const auto_string<value_type, SZ>& astr)
                {
                    _m_size = 0;
                    *_m_fix_data = 0;
                    _m_dyn_data = nullptr;
                    _m_dyn_capacity = 0;
                    _assign(astr.c_str(), astr.size());
                }

                auto_string& operator=(const value_type* str)
                {
                    _assign(str, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& operator=(const std::wstring& stdstr)
                {
                    _assign(stdstr.c_str(), stdstr.size());
                    return *this;
                }
                auto_string& operator=(const auto_string<value_type, BUFSZ_DFL>& astr)
                {
                    _assign(astr.c_str(), astr.size());
                    return *this;
                }

                const value_type* c_str() const
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

                value_type* data()
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

                bool is_dynamic() const
                {
                    return (_m_dyn_data != nullptr);
                }
                size_t size() const { return _m_size; }
                size_t length() const { return _m_size; }
                size_t capacity() const
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        return _m_dyn_capacity;
                    }
                    else
                    {
                        return BUFSZ_DFL;
                    }
                }

                ::pilo::error_number_t assign(const value_type* cstr, size_t len) { return _assign(cstr, len); }


            protected:
                ::pilo::error_number_t _reserve(size_t sz)
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        if (sz <= _m_dyn_capacity)
                        {
                            return ::pilo::EC_OK;
                        }

                        size_t tmpcapa = M_ALIGN_SIZE((sz + 1), sizeof(void*));
                        value_type * tmpbuff = (value_type*)malloc(tmpcapa*sizeof(wchar_t));
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
                        if (sz <= BUFSZ_DFL)
                        {
                            return ::pilo::EC_OK;
                        }

                        size_t tmpcapa = M_ALIGN_SIZE((sz + 1), sizeof(void*));
                        value_type * _m_dyn_data = (value_type*)malloc(tmpcapa*sizeof(value_type));
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

                ::pilo::error_number_t _assign(const value_type* cstr, size_t len)
                {
                    if (cstr == nullptr) // null str mean clear data, restore to fixed buffer
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
                    value_type * neo_buffer = nullptr;

                    if (nullptr == _m_dyn_data) //dyn buffer is not in using , 2 cases, one is still user fixed buffer, the other is switch to dyn buffer
                    {
                        if (len > BUFSZ_DFL)
                        {
                            neo_capa = M_ALIGN_SIZE((len + 1), sizeof(void*));
                            neo_buffer = (value_type*)malloc(neo_capa*sizeof(value_type));
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
                            _m_size = string_util::copy(_m_fix_data, BUFSZ_DFL + 1, cstr, len);
                        }
                    }
                    else //using dyn buffer
                    {
                        if (len > _m_dyn_capacity)
                        {
                            neo_capa = M_ALIGN_SIZE((len + 1), sizeof(void*));
                            neo_buffer = (value_type*)malloc(neo_capa*sizeof(value_type));
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
                            _m_size = string_util::copy(_m_dyn_data, _m_dyn_capacity + 1, cstr, len);
                            if (_m_size != len)
                            {
                                return ::pilo::EC_COPY_STRING_FAILED;
                            }
                        }
                    }

                    return ::pilo::EC_OK;
                }

            protected:
                size_t          _m_dyn_capacity;
                value_type*     _m_dyn_data;
                size_t          _m_size;
                value_type      _m_fix_data[BUFSZ_DFL + 1];
            };







//----------------------------------------------------------------------------------------------//
//--------------------------------------------||------------------------------------------------//
//---------------------------------[auto_string<wchar_t, 0>]------------------------------------//
//--------------------------------------------||------------------------------------------------//
//--------------------------------------------||------------------------------------------------//
//--------------------------------------------\/------------------------------------------------//


            template<>
            class auto_string<wchar_t, 0>
            {
            public:
                typedef wchar_t value_type;

            public:
                auto_string()
                {
                    _m_pdata = nullptr;
                    _reserve(0);
                    _m_pdata[0] = 0;
                    _m_size = 0;
                }
                auto_string(const value_type* cstr, size_t len)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(cstr, len);
                }
                auto_string(const value_type* cstr)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(cstr, MC_INVALID_SIZE);
                }
                auto_string(const std::wstring& stdstr)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(stdstr.c_str(), stdstr.size());
                }
                auto_string(const auto_string<value_type, 0>& astr)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(astr.c_str(), astr.size());
                }
                template<size_t SZ>
                auto_string(const auto_string<value_type, SZ>& astr)
                {
                    _m_pdata = nullptr;
                    _m_size = 0;
                    _assign(astr.c_str(), astr.size());
                }

                auto_string& operator=(const value_type* str)
                {
                    _assign(str, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& operator=(const std::wstring& stdstr)
                {
                    _assign(stdstr.c_str(), stdstr.size());
                    return *this;
                }
                auto_string& operator=(const auto_string<value_type, 0>& astr)
                {
                    _assign(astr.c_str(), astr.size());
                    return *this;
                }


                bool is_dynamic() const { return true; }
                const value_type* c_str() const { return _m_pdata; }
                value_type* data() { return _m_pdata; }
                size_t size() const { return _m_size; }
                size_t length() const { return _m_size; }
                size_t capacity() const { return _m_capacity; }
                ::pilo::error_number_t assign(const value_type* cstr, size_t len) { return _assign(cstr, len); }


            protected:
                ::pilo::error_number_t _reserve(size_t sz)
                {
                    value_type* tmp_pdata = nullptr;
                    size_t tmp_capa = M_ALIGN_SIZE((sz + 1), sizeof(void*));

                    if (nullptr == _m_pdata)
                    {
                        tmp_pdata = (value_type*)malloc(tmp_capa*sizeof(value_type));
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
                        tmp_pdata = (value_type*)malloc(tmp_capa*sizeof(value_type));
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

                pilo::i32_t _assign(const value_type* str, size_t len)
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
                    value_type* tmp_pdata = nullptr;


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

                    tmp_pdata = (value_type*)malloc(tmp_capa*sizeof(value_type));
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

            protected:
                value_type*       _m_pdata;
                size_t            _m_size;
                size_t            _m_capacity;
            };
        }
    }
}