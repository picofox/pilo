#pragma once

#include "../coredefs.hpp"
#include "string_util.hpp"
#include <string>
#include "core/io/fixed_memory_ostream.hpp"
#include "core/io/fixed_memory_istream.hpp"
#include "core/io/memory_istream_adapter.hpp"
#include "core/io/memory_ostream_adapter.hpp"

namespace pilo
{
    namespace core
    {
        namespace string
        {
            class dynamic_astring
            {
                static const size_t npos = MC_INVALID_SIZE;
            protected:
                char*       _m_pdata;
                size_t      _m_size;
                size_t      _m_capacity;

            protected:
                

            public:
                //constructors
                dynamic_astring(): _m_pdata(nullptr), _m_size(0), _m_capacity(0)
                {

                }
                dynamic_astring(const char* str);
                dynamic_astring(const char* str, size_t len);
                dynamic_astring(const std::string&  str);
                dynamic_astring(const dynamic_astring& str);

                ~dynamic_astring()
                {
                    MP_SAFE_FREE(_m_pdata);
                    _m_size = 0;
                    _m_capacity = 0;
                }

                dynamic_astring& operator=(const dynamic_astring& str);
                dynamic_astring& operator=(const char* str);
                dynamic_astring& operator=(const std::string&  str);


            public:
                //element access
                char& operator[] (size_t pos);
                const char& operator[] (size_t pos) const;
                char& at(size_t pos);
                const char& at(size_t pos) const;
                char& back();
                const char& back() const;
                char& front();
                const char& front() const;

                size_t length() const { return _m_size; }
                size_t capacity() const { return _m_capacity; }
                const char* c_str() const {return _m_pdata; }
                void clear()
                {
                    _m_size = 0;
                }
                void reset()
                {
                    _m_size = 0;
                    _m_capacity = 0;
                    MP_SAFE_FREE(_m_pdata);
                }
                bool empty() const { return (_m_size == 0); } 

                bool format(const char * fmt, ...);
                pilo::i32_t reserve(size_t sz);

                dynamic_astring& assign(const char* str, size_t len);
                dynamic_astring& assign(const char* str);
                

            protected:
                ::pilo::error_number_t _assign(const char* str);
                ::pilo::error_number_t _assign(const char* str, size_t len);
                ::pilo::error_number_t _resize(size_t sz);

            };
           

        } //end of string
    } // end of core
} // end of pilo