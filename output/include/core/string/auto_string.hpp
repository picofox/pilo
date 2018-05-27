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
                auto_string() : _m_pdata(nullptr), _m_size(0), _m_capacity(0)
                {
                
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
            protected:
                size_t          _m_dyn_capacity;
                wchar_t*           _m_dyn_data;
                size_t          _m_size;
                wchar_t            _m_fix_data[BUFSZ_DFL + 1];
            };

            template<>
            class auto_string<wchar_t, 0>
            {
            public:
                auto_string() : _m_pdata(nullptr), _m_size(0), _m_capacity(0)
                {

                }
            protected:
                wchar_t*       _m_pdata;
                size_t         _m_size;
                size_t        _m_capacity;
            };
        }
    }
}