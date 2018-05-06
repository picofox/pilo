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
            template<size_t max_capacity> class fixed_astring
            {
                static const size_t npos = MC_INVALID_SIZE;
            public:
                //constructors
                fixed_astring();
                fixed_astring(const char* str);
                fixed_astring(const char* str, size_t len);
                fixed_astring(const std::string&  str);
                template<size_t capa> fixed_astring(const fixed_astring<capa>& str)
                {
                    if (max_capacity > (size_t)MC_FIXEDSTRING_MAX_ELEMENTS)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }               

                    _assign(str);
                }

                fixed_astring(pilo::i32_t i);
                fixed_astring(pilo::i64_t i);
                fixed_astring(pilo::u32_t i);
                fixed_astring(pilo::u64_t i);
                fixed_astring(float r);
                fixed_astring(double r);

                //copy constructor
                fixed_astring(const fixed_astring<max_capacity>& str)
                {
                    if (this != &str)
                    {
                        _assign(str);
                    }
                }

                fixed_astring<max_capacity>& assign(size_t count, char ch);
                fixed_astring<max_capacity>& assign(const std::string& str);
                fixed_astring<max_capacity>& assign(const char* str, size_t len)
                {
                    _assign(str, len);
                    return *this;
                }
                fixed_astring<max_capacity>& assign(const char* str);
                template<size_t capa>
                fixed_astring<max_capacity>& assign(const fixed_astring<capa>& str)
                {
                    _assign(str);
                    return *this;
                }                

                template<typename NUMBER_T>
                fixed_astring<max_capacity>& assign(NUMBER_T num)
                {
                    _assign(num);
                    return *this;
                }

                fixed_astring<max_capacity>& operator=(const fixed_astring<max_capacity>& str)
                {
                    if (this != &str)
                    {
                        _assign(str);
                    }
                    return *this;
                }

                template<size_t capa>
                fixed_astring<max_capacity>& operator=(const fixed_astring<capa>& str)
                {

                    _assign(str);     
            
                    return *this;
                }
                fixed_astring<max_capacity>& operator=(const std::string& str)
                {
                    _assign(str);
                    return *this;
                }
                fixed_astring<max_capacity>& operator=(const char* str)
                {
                    _assign(str);
                    return *this;
                }
                fixed_astring<max_capacity>& operator=(char ch)
                {
                    _assign(ch);
                    return *this;
                }
                template<typename NUMBER_T>
                fixed_astring<max_capacity>& operator=(NUMBER_T num)
                {
                    _assign(num);
                    return *this;
                }

                fixed_astring<max_capacity>& operator+=(const char* str)
                {
                    concatenate(str);
                    return *this;
                }

                operator const char* ()
                {
                    return _m_data;
                }

                //attribute
                size_t size() const;
                size_t length() const;
                size_t max_size() const;
                size_t capacity() const;
                void clear();
                bool empty() const;
                size_t element_count(pilo::core::string::EnumCharsetEncodeType e);
                size_t available_capacity() const;
                bool has_space_for(const char * str);

                //element access
                const char* c_str() const;
                char& operator[] (size_t pos);
                const char& operator[] (size_t pos) const;
                char& at(size_t pos);
                const char& at(size_t pos) const;
                char& back();
                const char& back() const;
                char& front();
                const char& front() const;

                //operator
                friend bool operator==(const fixed_astring& l, const fixed_astring& r)
                {
                    return (l.compare(r) == 0);
                }
                friend bool operator!=(const fixed_astring& l, const fixed_astring& r)
                {
                    return (l.compare(r) != 0);
                }
                
                template<size_t _Stream_Capacity>
                friend pilo::core::io::fixed_memory_ostream<_Stream_Capacity>& operator<< (pilo::core::io::fixed_memory_ostream<_Stream_Capacity>& output, const fixed_astring& str)
                {
                    if (!output.write_string32(str.c_str(), str.size(), true))
                    {
                        M_ASSERT(false);
                    }

                    return output;
                }
                template<size_t _Stream_Capacity>
                friend pilo::core::io::fixed_memory_istream<_Stream_Capacity>& operator>> (pilo::core::io::fixed_memory_istream<_Stream_Capacity>& is, fixed_astring& str)
                {
                    str.clear();
                    size_t  sz = is.read_string32((char*)str._m_data, str.capacity());
                    if (sz == MC_INVALID_SIZE)
                    {
                        M_ASSERT(false);
                    }
                    str._m_size = sz;
                    return is;
                }
                template<size_t _Stream_Capacity>
                friend pilo::core::io::memory_ostream_adapter& operator<< (pilo::core::io::memory_ostream_adapter& output, const fixed_astring& str)
                {
                    if (str.size() != output.write_string32(str.c_str(), str.size()))
                    {
                        M_ASSERT(false);
                    }

                    return output;
                }
                friend pilo::core::io::memory_istream_adapter& operator>> (pilo::core::io::memory_istream_adapter& is, fixed_astring& str)
                {
                    str.clear();
                    size_t  sz = is.read_string32((char*)str._m_data, str.capacity());
                    if (sz == MC_INVALID_SIZE)
                    {
                        M_ASSERT(false);
                    }
                    str._m_size = sz;
                    return is;
                }

                //compare
                template<typename _CompraeClass>
                pilo::i32_t compare(const _CompraeClass& o) const
                {
                    size_t rlen = ((this->size() < o.size()) ? this->size() : o.size());
                    pilo::i32_t iret = pilo::core::string::string_util::binary_compare(this->_m_data, o.c_str(), rlen);
                    if (0 == iret)
                    {
                        if (this->size() < o.size())
                        {
                            return -1;
                        }
                        else if (this->size() > o.size())
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }

                    return iret;
                }

                template<size_t capa>
                pilo::i32_t compare(const fixed_astring<capa>& str) const
                {
                    size_t rlen = ((this->size() < str.size()) ? this->size() : str.size());
                    pilo::i32_t iret = pilo::core::string::string_util::binary_compare(this->_m_data, str.c_str(), rlen);
                    if (0 == iret)
                    {
                        if (this->size() < str.size())
                        {
                            return -1;
                        }
                        else if (this->size() > str.size())
                        {
                            return 1;
                        }
                        else
                        {
                            return 0;
                        }
                    }

                    return iret;
                }
                pilo::i32_t compare(const std::string& str) const;
                pilo::i32_t compare(const char* str) const;

               

                //operation
                size_t copy(char* s, size_t len, size_t pos = 0) const;
                size_t copy(char* dst_str, size_t dest_len, size_t dst_pos, size_t copy_len, size_t copy_start_pos) const;

                //format input
                bool format(const char * fmt, ...)
                {
                    va_list args;
                    pilo::i32_t ret;

                    va_start(args, fmt);
#               if defined(WINDOWS)
                    ret = _vsnprintf_s(_m_data, max_capacity, _TRUNCATE, fmt, args);
#               else
                    ret = vsnprintf(_m_data, max_capacity, fmt, args);
#               endif
                    va_end(args);

                    if (ret < 0)
                    {
                        return false;
                    }

                    _m_size = static_cast<size_t>(ret);
                    return true;
                }

                
                fixed_astring<max_capacity>& concatenate(const char * str)
                {          
                    
                    if (str != nullptr && *str != 0)
                    {
                        size_t av = available_capacity();
                        size_t len = ::pilo::core::string::string_util::length(str);
                        if (av <= len)
                        {
                            return *this;
                        }

                        ::pilo::core::string::string_util::concatenate_string(_m_data, available_capacity(), str, len);                        
                    }

                    return *this;
                }

                bool insert(size_t pos, const char* str, size_t len)
                {
                    if (str == 0 || *str == 0)
                    {
                        return false;
                    }
                    
                    if (pos >= size())
                    {
                        return false;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = strlen(str);
                    }

                    if (capacity() <= len + size())
                    {
                        return false;
                    }

                    ::memmove(_m_data+pos+len, _m_data+pos, size() - pos);
                    ::memmove(_m_data+pos    , str,          len);
                    _m_size += len;
                    _m_data[_m_size] = 0;

                    return true;
                }

                //debug


            protected:
                template<size_t capa> 
                void _assign(const fixed_astring<capa>& str)
                {
                    if (str.size() > max_capacity)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }
                    _m_size = string_util::m_copy<char>(_m_data, max_capacity, str.c_str(), str.size());
                    _m_data[_m_size] = 0;
                }
                void _assign(const std::string& str)
                {
                    if (str.size() > max_capacity)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }
                    _m_size = string_util::m_copy<char>(_m_data, max_capacity, str.c_str(), str.size());
                    _m_data[_m_size] = 0;
                }
                void _assign(const char* cstr, size_t len)
                {
                    if (cstr == nullptr || *cstr == 0)
                    {
                        _m_size = 0;
                        *_m_data = 0;
                        return;
                    }
                    _m_size = string_util::copy(_m_data, max_capacity + 1, cstr, len);
                }
                void _assign(const char* cstr)
                {
                    if (cstr == nullptr || *cstr == 0)
                    {
                        _m_size = 0;
                        *_m_data = 0;
                        return ;
                    }
                    _m_size = string_util::copy(_m_data, max_capacity + 1, cstr, MC_INVALID_SIZE);
                }
                void _assign(char ch)
                {
                    if (max_capacity == 0)
                    {
                        M_ASSERT(false);
                        throw EC_BUFFER_TOO_SMALL;
                    }
                    _m_data[0] = ch;
                    _m_data[1] = 0; 
                    _m_size = 1;
                }
                void _assign(pilo::i32_t i)
                {
                    _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, i);
                    if (MC_INVALID_SIZE == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_BUFFER_TOO_SMALL;
                    }
                }

                void _assign(pilo::i64_t i)
                {
                    _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, i);
                    if (MC_INVALID_SIZE == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_BUFFER_TOO_SMALL;
                    }
                }
                void _assign(pilo::u32_t i)
                {
                    _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, i);
                    if (MC_INVALID_SIZE == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_BUFFER_TOO_SMALL;
                    }
                }
                void _assign(pilo::u64_t i)
                {
                    _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, i);
                    if (MC_INVALID_SIZE == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_BUFFER_TOO_SMALL;
                    }
                }
                void _assign(float f)
                {
                    _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, f);
                    if (MC_INVALID_SIZE == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_BUFFER_TOO_SMALL;
                    }
                }
                void _assign(double df)
                {
                    _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, df);
                    if (MC_INVALID_SIZE == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_BUFFER_TOO_SMALL;
                    }
                }
                void _assign(char ch, size_t count)
                {
                    if (count > max_capacity)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_BUFFER_TOO_SMALL;
                    }
                    pilo::core::string::string_util::set(_m_data, ch, count);
                    _m_size = count;
                }

            protected:
                char        _m_data[max_capacity+1];
                size_t      _m_size;

            public:
                typedef fixed_astring<max_capacity> fixed_astring_type;
                typedef char char_type;

            };                       

            template<size_t max_capacity>
            bool pilo::core::string::fixed_astring<max_capacity>::has_space_for(const char * str)
            {
                if (str == nullptr || *str == 0)
                {
                    return true;
                }

                size_t len = ::pilo::core::string::string_util::length(str);
                if (len >= available_capacity())
                {
                    return false;
                }
                return true;
            }

            template<size_t max_capacity>
            size_t pilo::core::string::fixed_astring<max_capacity>::available_capacity() const
            {
                return max_capacity - _m_size - 1;                
            }

            template<size_t max_capacity>
            pilo::i32_t pilo::core::string::fixed_astring<max_capacity>::compare(const std::string& str) const
            {
                size_t rlen = std::min(this->size(), str.size());
                pilo::i32_t iret = pilo::core::string::string_util::binary_compare(this->_m_data, str.c_str(), rlen);
                if (0 == iret)
                {
                    if (this->size() < str.size())
                    {
                        return -1;
                    }
                    else if (this->size() > str.size())
                    {
                        return 1;
                    }
                    else
                    {
                        return 0;
                    }
                }

                return iret;
            }

            template<size_t max_capacity>
            pilo::i32_t pilo::core::string::fixed_astring<max_capacity>::compare(const char* str) const
            {                
                return pilo::core::string::string_util::binary_compare(this->_m_data, str);
            }




            //implementation
            //constructors
            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring()
            {
                if (max_capacity > (size_t)MC_FIXEDSTRING_MAX_ELEMENTS)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                _m_size = 0;
                *_m_data = 0;
            }

            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring(const char* str)
            {
                if (max_capacity > (size_t) MC_FIXEDSTRING_MAX_ELEMENTS)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                _assign(str);
            }


            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring(const char* str, size_t len)
            {
                if (max_capacity > (size_t)MC_FIXEDSTRING_MAX_ELEMENTS)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }

                if (len > max_capacity)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }

                if (str == 0 || *str == 0)
                {
                    _m_size = 0;
                    *_m_data = 0;
                    return;
                }

                _m_size = string_util::m_copy(_m_data, max_capacity, str, len);
                _m_data[_m_size] = 0;
            }

            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring(const std::string& str)
            {
                if (str.length() == 0)
                {
                    clear();
                    return;
                }

                if (max_capacity > (size_t)MC_FIXEDSTRING_MAX_ELEMENTS)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }

                _assign(str);
            }

            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring(pilo::i32_t i)
            {
                _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, i);
                if (MC_INVALID_SIZE == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_BUFFER_TOO_SMALL;
                }
            }

            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring(pilo::i64_t i)
            {
                _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, i);
                if (MC_INVALID_SIZE == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_BUFFER_TOO_SMALL;
                }
            }

            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring(pilo::u32_t i)
            {
                _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, i);
                if (MC_INVALID_SIZE == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_BUFFER_TOO_SMALL;
                }
            }

            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring(pilo::u64_t i)
            {
                _m_size = pilo::core::string::string_util::number_to_string(_m_data, max_capacity, i);
                if (MC_INVALID_SIZE == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_BUFFER_TOO_SMALL;
                }
            }

            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring(float r)
            {
                if (max_capacity > (size_t)MC_FIXEDSTRING_MAX_ELEMENTS)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                _assign(r);
            }

            template<size_t max_capacity>
            pilo::core::string::fixed_astring<max_capacity>::fixed_astring(double r)
            {
                if (max_capacity > (size_t)MC_FIXEDSTRING_MAX_ELEMENTS)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                _assign(r);
            }

            // implementation of attribute
            template< size_t max_capacity>
            size_t pilo::core::string::fixed_astring<max_capacity>::size() const
            {
                return _m_size;
            }

            template< size_t max_capacity>
            size_t pilo::core::string::fixed_astring<max_capacity>::length() const
            {
                return _m_size;
            }

            template< size_t max_capacity>
            size_t pilo::core::string::fixed_astring<max_capacity>::max_size() const
            {
                return (size_t) MC_FIXEDSTRING_MAX_ELEMENTS;
            }

            template< size_t max_capacity>
            size_t pilo::core::string::fixed_astring<max_capacity>::capacity() const
            {
                return max_capacity;
            }

            template< size_t max_capacity>
            void pilo::core::string::fixed_astring<max_capacity>::clear()
            {
                /*memset(_m_data, 0x00, sizeof(_m_data));*/
                *_m_data = 0;
                _m_size = 0;
            }

            template< size_t max_capacity>
            bool pilo::core::string::fixed_astring<max_capacity>::empty() const
            {
                return (_m_size == 0 ? true : false);
            }

            template< size_t max_capacity>
            const char* pilo::core::string::fixed_astring<max_capacity>::c_str() const
            {
                return _m_data;
            }

            template< size_t max_capacity>
            size_t pilo::core::string::fixed_astring<max_capacity>::element_count(pilo::core::string::EnumCharsetEncodeType e)
            {
                return string_util::element_count(_m_data, e);
            }

            //element access
            template< size_t max_capacity>
            char& pilo::core::string::fixed_astring<max_capacity>::operator[](size_t pos)
            {
                if (max_capacity < pos)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }

                if (pos > _m_size)
                {
                    _m_data[pos] = 0;
                }

                return _m_data[pos];
            }

            template< size_t max_capacity>
            const char& pilo::core::string::fixed_astring<max_capacity>::operator[](size_t pos) const
            {
                if (pos > _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }

                return _m_data[pos];
            }

            template< size_t max_capacity>
            char& pilo::core::string::fixed_astring<max_capacity>::at(size_t pos)
            {
                if (max_capacity < pos)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }

                if (pos > _m_size)
                {
                    _m_data[pos] = 0;
                }

                return _m_data[pos];
            }

            template< size_t max_capacity>
            const char& pilo::core::string::fixed_astring<max_capacity>::at(size_t pos) const
            {
                if (pos > _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }

                return _m_data[pos];
            }

            template< size_t max_capacity>
            char& pilo::core::string::fixed_astring<max_capacity>::front()
            {
                if (0 == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_LOWER_LIMIT;
                }
                return _m_data[0];
            }

            template< size_t max_capacity>
            const char& pilo::core::string::fixed_astring<max_capacity>::front() const
            {
                if (0 == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_LOWER_LIMIT;
                }
                return _m_data[0];
            }

            template< size_t max_capacity>
            char& pilo::core::string::fixed_astring<max_capacity>::back()
            {
                if (0 == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                return _m_data[_m_size - 1];
            }

            template< size_t max_capacity>
            const char& pilo::core::string::fixed_astring<max_capacity>::back() const
            {
                if (0 == _m_size)
                {
                    M_ASSERT(false);
                    throw pilo::EC_REACH_UPPER_LIMIT;
                }
                return _m_data[_m_size - 1];
            }

            template<size_t max_capacity>
            fixed_astring<max_capacity>& pilo::core::string::fixed_astring<max_capacity>::assign(const std::string& str)
            {
                _assign(str);
                return *this;
            }

            template<size_t max_capacity>
            fixed_astring<max_capacity>& pilo::core::string::fixed_astring<max_capacity>::assign(size_t count, char ch)
            {
                _assign(ch, count);
                return *this;
            }

            template<size_t max_capacity>
            fixed_astring<max_capacity>& pilo::core::string::fixed_astring<max_capacity>::assign(const char* str)
            {
                _assign(str);
                return *this;
            }

            template< size_t max_capacity>
            size_t pilo::core::string::fixed_astring<max_capacity>::copy(char* dst_str, size_t src_count, size_t src_pos /*= 0*/) const
            {
                size_t ret = string_util::m_copy(dst_str, 0, MC_INVALID_SIZE, _m_data, src_pos, src_count);
                return ret;
            }

            template< size_t max_capacity>
            size_t pilo::core::string::fixed_astring<max_capacity>::copy(char* dst_str, size_t dst_pos, size_t dst_capacity, size_t src_pos, size_t src_count) const
            {
                size_t ret = string_util::m_copy(dst_str, dst_pos, dst_capacity, _m_data, src_pos, src_count);
                return ret;
            }            
            

        } //end of string
    } // end of core
} // end of pilo