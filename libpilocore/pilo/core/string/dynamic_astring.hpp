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
                    _reserve(0);
                    *_m_pdata = 0;
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

                size_t size() const { return _m_size; }
                size_t length() const { return _m_size; }
                size_t capacity() const { return _m_capacity; }
                const char* c_str() const {return _m_pdata; }
                char* data() { return _m_pdata; }
                void clear()
                {
                    _m_size = 0;
                    *_m_pdata = 0;
                }

                bool empty() const { return (_m_size == 0); } 

                bool format(const char * fmt, ...);
                inline pilo::error_number_t reserve(size_t sz) { return _reserve(sz); }
                inline pilo::error_number_t grow(size_t sz) { return _reserve(capacity() + sz); }
                void recalculate_size();

                dynamic_astring& assign(const char* str, size_t len);
                dynamic_astring& assign(const char* str);

                inline size_t available_capacity() const
                {
                    return _m_capacity - _m_size;
                }

                ::pilo::i32_t compare(const char* str, size_t len_to_compare = MC_INVALID_SIZE) const;
                ::pilo::i32_t compare(const std::string& str) const;
                
                inline void push_back(char ch) { _push_back(ch); }
                inline void pop_back() { _pop_back(); }

                dynamic_astring& append(const char* suffix_str, size_t pos, size_t len)
                {
                    _append(suffix_str, pos, len);
                    return *this;
                }

                dynamic_astring& append(const std::string& stdstr)
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }

                dynamic_astring& append(const char* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }

                dynamic_astring& append(const dynamic_astring& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
                }

                dynamic_astring insert(size_t pos, const char* str, size_t len)
                {
                    _insert(pos, str, len);
                    return *this;
                }
                


            protected:
                ::pilo::error_number_t _assign(const char* str);
                ::pilo::error_number_t _assign(const char* str, size_t len);
                ::pilo::error_number_t _reserve(size_t sz);
                ::pilo::i32_t _push_back(char ch);
                ::pilo::i32_t _pop_back();
                ::pilo::i32_t _append(const char* suffix_str, size_t pos, size_t len);
                ::pilo::i32_t _insert(size_t pos, const char* str, size_t len);
            };
           

        } //end of string
    } // end of core
} // end of pilo