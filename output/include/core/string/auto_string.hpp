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

                char* data()
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
                        if (sz <= BUFSZ_DFL)
                        {
                            return ::pilo::EC_OK;
                        }

                        size_t tmpcapa = M_ALIGN_SIZE((sz + 1), sizeof(void*));
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

            protected:
                size_t          _m_dyn_capacity;
                char*           _m_dyn_data;
                size_t          _m_size;
                char            _m_fix_data[BUFSZ_DFL + 1];
            };

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

                bool is_dynamic() const { return true; }
                const char* c_str() const { return _m_pdata; }
                char* data() { return _m_pdata; }
                size_t size() const { return _m_size; }
                size_t length() const { return _m_size; }
                size_t capacity() const { return _m_capacity; }

            protected:
                ::pilo::error_number_t _reserve(size_t sz)
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

            protected:
                char*       _m_pdata;
                size_t      _m_size;
                size_t      _m_capacity;
            };

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

                const wchar_t* c_str() const
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

                wchar_t* data()
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
                        wchar_t * tmpbuff = (wchar_t*)malloc(tmpcapa*sizeof(wchar_t));
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
                        wchar_t * _m_dyn_data = (wchar_t*)malloc(tmpcapa*sizeof(wchar_t));
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

            protected:
                size_t          _m_dyn_capacity;
                wchar_t*        _m_dyn_data;
                size_t          _m_size;
                wchar_t         _m_fix_data[BUFSZ_DFL + 1];
            };

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

                bool is_dynamic() const { return true; }
                const wchar_t* c_str() const { return _m_pdata; }
                wchar_t* data() { return _m_pdata; }
                size_t size() const { return _m_size; }
                size_t length() const { return _m_size; }
                size_t capacity() const { return _m_capacity; }

            protected:
                ::pilo::error_number_t _reserve(size_t sz)
                {
                    wchar_t* tmp_pdata = nullptr;
                    size_t tmp_capa = M_ALIGN_SIZE((sz + 1), sizeof(void*));

                    if (nullptr == _m_pdata)
                    {
                        tmp_pdata = (wchar_t*)malloc(tmp_capa*sizeof(wchar_t));
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
                        tmp_pdata = (wchar_t*)malloc(tmp_capa*sizeof(wchar_t));
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
            protected:
                wchar_t*       _m_pdata;
                size_t         _m_size;
                size_t        _m_capacity;
            };
        }
    }
}