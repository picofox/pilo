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

            public:
//                 template<typename CHAR_T, size_t LSZ, size_t RSZ>
//                 friend bool operator==(const auto_string<CHAR_T, LSZ>& left, const auto_string<CHAR_T, RSZ>& right);

            };            

            template<typename CHAR_T, size_t LSZ, size_t RSZ>
            bool operator==(const auto_string<CHAR_T, LSZ>& left, const auto_string<CHAR_T, RSZ>& right)
            {
                return (left.compare(right) == 0);
            }
            template<typename CHAR_T, size_t LSZ>
            bool operator==(const auto_string<CHAR_T, LSZ>& left, const CHAR_T* right)
            {
                return (left.compare(right, MC_INVALID_SIZE) == 0);
            }
            template<typename CHAR_T, size_t RSZ>
            bool operator==(const CHAR_T* left, const auto_string<CHAR_T, RSZ>& right)
            {
                return (right.compare(left, MC_INVALID_SIZE) == 0);
            }
            template<size_t LSZ>
            bool operator==(const auto_string<char, LSZ>& left, const std::string& right)
            {
                return (left.compare(right) == 0);
            }
            template<size_t RSZ>
            bool operator==(const std::string& left, const auto_string<char, RSZ>& right)
            {
                return (right.compare(left) == 0);
            }
            template<size_t LSZ>
            bool operator==(const auto_string<wchar_t, LSZ>& left, const std::wstring& right)
            {
                return (left.compare(right) == 0);
            }
            template<size_t RSZ>
            bool operator==(const std::wstring& left, const auto_string<wchar_t, RSZ>& right)
            {
                return (right.compare(left) == 0);
            }

            template<typename CHAR_T, size_t LSZ, size_t RSZ>
            bool operator!=(const auto_string<CHAR_T, LSZ>& left, const auto_string<CHAR_T, RSZ>& right)
            {
                return (left.compare(right) != 0);
            }
            template<typename CHAR_T, size_t LSZ>
            bool operator!=(const auto_string<CHAR_T, LSZ>& left, const CHAR_T* right)
            {
                return (left.compare(right, MC_INVALID_SIZE) != 0);
            }
            template<typename CHAR_T, size_t RSZ>
            bool operator!=(const CHAR_T* left, const auto_string<CHAR_T, RSZ>& right)
            {
                return (right.compare(left, MC_INVALID_SIZE) != 0);
            }
            template<size_t LSZ>
            bool operator!=(const auto_string<char, LSZ>& left, const std::string& right)
            {
                return (left.compare(right) != 0);
            }
            template<size_t RSZ>
            bool operator!=(const std::string& left, const auto_string<char, RSZ>& right)
            {
                return (right.compare(left) != 0);
            }
            template<size_t LSZ>
            bool operator!=(const auto_string<wchar_t, LSZ>& left, const std::wstring& right)
            {
                return (left.compare(right) != 0);
            }
            template<size_t RSZ>
            bool operator!=(const std::wstring& left, const auto_string<wchar_t, RSZ>& right)
            {
                return (right.compare(left) != 0);
            }


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

                auto_string& operator+=(const value_type* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& operator+=(const std::string stdstr)
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }
                template<size_t SZ>
                auto_string& operator+=(const auto_string<value_type, SZ>& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
                }

                ::pilo::i32_t compare(const value_type* str, size_t len_to_compare = MC_INVALID_SIZE) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), str, len_to_compare);
                }
                ::pilo::i32_t compare(const std::string& str) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), str.c_str(), MC_INVALID_SIZE);
                }
                template <size_t SZ>
                ::pilo::i32_t compare(const auto_string<value_type, SZ>& astr) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), astr.c_str(), MC_INVALID_SIZE);
                }

                void recalculate_size()
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        if (_m_dyn_data == nullptr)
                        {
                            _m_size = 0;
                        }
                        else
                        {
                            _m_size = ::strlen(_m_dyn_data);
                        }
                    }
                    else
                    {
                        _m_size = ::strlen(_m_fix_data);
                    }                    
                }

                inline void clear()
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

                ::pilo::error_number_t append_string(const value_type* suffix_str, size_t pos, size_t len)
                {
                    return _append(suffix_str, pos, len);
                }

                auto_string& append(const value_type* suffix_str, size_t pos, size_t len)
                {
                    _append(suffix_str, pos, len);
                    return *this;
                }

                auto_string& append(const std::string& stdstr)
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }

                auto_string& append(const value_type* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }

                template<size_t SZ>
                auto_string& append(const auto_string<value_type, SZ>& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
                }

                auto_string& insert(size_t pos, const value_type* str, size_t len)
                {
                    _insert(pos, str, len);
                    return *this;
                }
                auto_string& insert(size_t pos, const value_type* str)
                {
                    _insert(pos, str, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& insert(size_t pos, const std::string & str)
                {
                    _insert(pos, str.c_str(), str.size());
                    return *this;
                }
                template<size_t SZ>
                auto_string& insert(size_t pos, const auto_string<value_type, SZ>& astr)
                {
                    _insert(pos, astr.c_str(), astr.size());
                    return *this;
                }

                inline pilo::error_number_t reserve(size_t sz) { return _reserve(sz); }
                inline pilo::error_number_t grow_from_size(size_t sz) { return _reserve(_m_size + sz); }

                size_t available_capacity() const
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        return _m_dyn_capacity - _m_size;
                    }
                    else
                    {
                        return BUFSZ_DFL - _m_size;
                    }
                }
                
                value_type& at(size_t pos)
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                const value_type& at(size_t pos) const
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }

                value_type& back()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }
                const value_type& back() const
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }

                value_type& operator[] (size_t pos)
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                const value_type& operator[] (size_t pos) const
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }

                value_type& front()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(0);
                }

                const value_type& front() const
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(0);
                }
                
                inline bool empty() const { return (_m_size == 0); }

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


                inline void push_back(value_type ch) { _push_back(ch); }
                inline void pop_back() { _pop_back(); }

                size_t find_first_of ( char c, size_t pos = 0 ) const
                {
                    if (pos >= _m_size) return MC_INVALID_SIZE;

                    const char * p = nullptr;
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        const value_type* p = ::pilo::core::string::string_util::find(_m_dyn_data, c, pos, MC_INVALID_SIZE);                        
                    }
                    else
                    {
                        const value_type* p = ::pilo::core::string::string_util::find(_m_fix_data, c, pos, MC_INVALID_SIZE);
                    }

                    if (p == nullptr)
                    {
                        return MC_INVALID_SIZE;
                    }
                    return (p - _m_pdata) / sizeof(value_type); 
                }

                void erase_front_until(char ch, bool inc)
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        ::pilo::core::string::string_util::erase_front_until(_m_dyn_data, ch, _m_size, inc);               
                    }
                    else
                    {
                        ::pilo::core::string::string_util::erase_front_until(_m_fix_data, ch, _m_size, inc);   
                    }
                }

            protected:
                value_type& _at(size_t pos)
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
                        _m_dyn_data = (value_type*)malloc(tmpcapa);
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

                ::pilo::error_number_t _push_back(value_type ch)
                {
                    ::pilo::error_number_t ret = ::pilo::EC_OK;
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        if (available_capacity() <= 0)
                        {
                            ret = _reserve(_m_dyn_capacity + 1);
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
                            ret = _reserve(BUFSZ_DFL + 1);
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


                ::pilo::error_number_t _append(const value_type* suffix_str, size_t pos, size_t len)
                {
                    if (suffix_str == nullptr)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    if (pos == MC_INVALID_SIZE)
                    {
                        return ::pilo::EC_INVALID_PARAM;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = ::pilo::core::string::string_util::length(suffix_str + pos);
                    }

                    if (len == 0)
                    {
                        return ::pilo::EC_OK;
                    }

                    if (available_capacity() < len)
                    {
                        if (::pilo::EC_OK != grow_from_size(len))
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                    }

                    if (MC_INVALID_SIZE == ::pilo::core::string::string_util::copy(data() + _m_size, available_capacity() + 1, suffix_str + pos, len))
                    {
                        return ::pilo::EC_COPY_STRING_FAILED;
                    }

                    _m_size += len;

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _insert(size_t pos, const value_type* str, size_t len)
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
                        len = ::pilo::core::string::string_util::length(str);
                    }

                    if (available_capacity() < len)
                    {
                        if (::pilo::EC_OK != grow_from_size(len))
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

                auto_string& operator+=(const value_type* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& operator+=(const std::string stdstr)
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }
                template<size_t SZ>
                auto_string& operator+=(const auto_string<value_type, SZ>& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
                }

                ::pilo::i32_t compare(const value_type* str, size_t len_to_compare = MC_INVALID_SIZE) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), str, len_to_compare);
                }
                ::pilo::i32_t compare(const std::string& str) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), str.c_str(), MC_INVALID_SIZE);
                }
                template <size_t SZ>
                ::pilo::i32_t compare(const auto_string<value_type, SZ>& astr) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), astr.c_str(), MC_INVALID_SIZE);
                }

                void recalculate_size()
                {
                    if (_m_pdata == nullptr)
                    {
                        _m_size = 0;
                    }
                    else
                    {
                        _m_size = ::strlen(_m_pdata);
                    }
                }

                void clear()
                {
                    if (_m_pdata != nullptr)
                    {
                        *_m_pdata = 0;
                    }
                    _m_size = 0;
                }

                ::pilo::error_number_t append_string(const value_type* suffix_str, size_t pos, size_t len)
                {
                    return _append(suffix_str, pos, len);
                }

                auto_string& append(const value_type* suffix_str, size_t pos, size_t len)
                {
                    _append(suffix_str, pos, len);
                    return *this;
                }
                auto_string& append(const std::string& stdstr)
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }
                auto_string& append(const value_type* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }
                template<size_t SZ>
                auto_string& append(const auto_string<value_type, SZ>& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
                }

                auto_string& insert(size_t pos, const value_type* str, size_t len)
                {
                    _insert(pos, str, len);
                    return *this;
                }
                auto_string& insert(size_t pos, const value_type* str)
                {
                    _insert(pos, str, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& insert(size_t pos, const std::string & str)
                {
                    _insert(pos, str.c_str(), str.size());
                    return *this;
                }
                template<size_t SZ>
                auto_string& insert(size_t pos, const auto_string<value_type, SZ>& astr)
                {
                    _insert(pos, astr.c_str(), astr.size());
                    return *this;
                }

                inline pilo::error_number_t reserve(size_t sz) { return _reserve(sz); }
                inline pilo::error_number_t grow_from_size(size_t sz) { return _reserve(_m_size + sz); }

                inline size_t available_capacity() const
                {
                    return _m_capacity - _m_size;
                }

                value_type& at(size_t pos)
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                const value_type& at(size_t pos) const
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                value_type& operator[] (size_t pos)
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                const value_type& operator[] (size_t pos) const
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }

                value_type& back()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }
                const value_type& back() const
                {

                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }

                value_type& front()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(0);
                }

                const value_type& front() const
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(0);
                }

                inline bool empty() const { return (_m_size == 0); }
                bool is_dynamic() const { return true; }
                const value_type* c_str() const { return _m_pdata; }
                value_type* data() { return _m_pdata; }
                size_t size() const { return _m_size; }
                size_t length() const { return _m_size; }
                size_t capacity() const { return _m_capacity; }
                ::pilo::error_number_t assign(const value_type* cstr, size_t len) { return _assign(cstr, len); }
                inline void push_back(value_type ch) { _push_back(ch); }
                inline void pop_back() { _pop_back(); }
                size_t find_first_of ( value_type c, size_t pos = 0 ) const
                {
                    if (pos >= _m_size) return MC_INVALID_SIZE; 
                    const value_type* p = ::pilo::core::string::string_util::find(_m_pdata, c, pos, MC_INVALID_SIZE);
                    if (p == nullptr)
                    {
                        return MC_INVALID_SIZE;
                    }
                    return (p - _m_pdata) / sizeof(value_type);             
                }

                void erase_front_until(char ch, bool inc)
                {                    
                    ::pilo::core::string::string_util::erase_front_until(_m_pdata, ch, _m_size, inc);     
                }

            protected:
                value_type& _at(size_t pos)
                {                    
                    return _m_pdata[pos];
                }
                const value_type& _at(size_t pos) const
                {
                    return _m_pdata[pos];
                }
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

                ::pilo::error_number_t _push_back(value_type ch)
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

                ::pilo::error_number_t _pop_back()
                {
                    if (_m_size <= 0)
                    {
                        return ::pilo::EC_REACH_LOWER_LIMIT;
                    }
                    _m_pdata[--_m_size] = 0;
                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _append(const value_type* suffix_str, size_t pos, size_t len)
                {
                    if (suffix_str == nullptr)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    if (pos == MC_INVALID_SIZE)
                    {
                        return ::pilo::EC_INVALID_PARAM;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = ::pilo::core::string::string_util::length(suffix_str+pos);
                    }

                    if (len == 0)
                    {
                        return ::pilo::EC_OK;
                    }

                    if (available_capacity() < len)
                    {
                        if (::pilo::EC_OK != _reserve(_m_capacity + len))
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                    }                    

                    if (MC_INVALID_SIZE == ::pilo::core::string::string_util::copy(_m_pdata + _m_size, available_capacity() + 1, suffix_str + pos, len))
                    {
                        return ::pilo::EC_COPY_STRING_FAILED;
                    }

                    _m_size += len;

                    return ::pilo::EC_OK;
                }

                ::pilo::i32_t _insert(size_t pos, const value_type* str, size_t len)
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
                        len = ::pilo::core::string::string_util::length(str);
                    }

                    if (available_capacity() < len)
                    {
                        if (::pilo::EC_OK != grow_from_size(len))
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

                auto_string& operator+=(const value_type* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& operator+=(const std::wstring stdstr)
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }
                template<size_t SZ>
                auto_string& operator+=(const auto_string<value_type, SZ>& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
                }

                ::pilo::i32_t compare(const value_type* str, size_t len_to_compare = MC_INVALID_SIZE) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), str, len_to_compare);
                }
                ::pilo::i32_t compare(const std::wstring& str) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), str.c_str(), MC_INVALID_SIZE);
                }
                template <size_t SZ>
                ::pilo::i32_t compare(const auto_string<value_type, SZ>& astr) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), astr.c_str(), MC_INVALID_SIZE);
                }

                void recalculate_size()
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        if (_m_dyn_data == nullptr)
                        {
                            _m_size = 0;
                        }
                        else
                        {
                            _m_size = ::pilo::core::string::string_util::length(_m_dyn_data);
                        }
                    }
                    else
                    {
                        _m_size = ::pilo::core::string::string_util::length(_m_fix_data);
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

                ::pilo::error_number_t append_string(const value_type* suffix_str, size_t pos, size_t len)
                {
                    return _append(suffix_str, pos, len);
                }

                auto_string& append(const value_type* suffix_str, size_t pos, size_t len)
                {
                    _append(suffix_str, pos, len);
                    return *this;
                }
                auto_string& append(const std::string& stdstr)
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }
                auto_string& append(const value_type* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }
                template<size_t SZ>
                auto_string& append(const auto_string<value_type, SZ>& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
                }

                auto_string& insert(size_t pos, const value_type* str, size_t len)
                {
                    _insert(pos, str, len);
                    return *this;
                }
                auto_string& insert(size_t pos, const value_type* str)
                {
                    _insert(pos, str, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& insert(size_t pos, const std::wstring & str)
                {
                    _insert(pos, str.c_str(), str.size());
                    return *this;
                }
                template<size_t SZ>
                auto_string& insert(size_t pos, const auto_string<value_type, SZ>& astr)
                {
                    _insert(pos, astr.c_str(), astr.size());
                    return *this;
                }

                inline pilo::error_number_t reserve(size_t sz) { return _reserve(sz); }
                inline pilo::error_number_t grow_from_size(size_t sz) { return _reserve(_m_size + sz); }

                size_t available_capacity() const
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        return _m_dyn_capacity - _m_size;
                    }
                    else
                    {
                        return BUFSZ_DFL - _m_size;
                    }
                }

                value_type& at(size_t pos)
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                const value_type& at(size_t pos) const
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }

                value_type& operator[] (size_t pos)
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                const value_type& operator[] (size_t pos) const
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }

                value_type& back()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }
                const value_type& back() const
                {

                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }

                value_type& front()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(0);
                }

                const value_type& front() const
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(0);
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

                inline bool empty() const { return (_m_size == 0); }
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
                inline void push_back(value_type ch) { _push_back(ch); }
                inline void pop_back() { _pop_back(); }

                size_t find_first_of ( char c, size_t pos = 0 ) const
                {
                    if (pos >= _m_size) return MC_INVALID_SIZE;

                    const char * p = nullptr;
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        const value_type* p = ::pilo::core::string::string_util::find(_m_dyn_data, c, pos, MC_INVALID_SIZE);                        
                    }
                    else
                    {
                        const value_type* p = ::pilo::core::string::string_util::find(_m_fix_data, c, pos, MC_INVALID_SIZE);
                    }

                    if (p == nullptr)
                    {
                        return MC_INVALID_SIZE;
                    }
                    return (p - _m_pdata) / sizeof(value_type); 
                }

                void erase_front_until(char ch, bool inc)
                {
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        ::pilo::core::string::string_util::erase_front_until(_m_dyn_data, ch, _m_size, inc);               
                    }
                    else
                    {
                        ::pilo::core::string::string_util::erase_front_until(_m_fix_data, ch, _m_size, inc);   
                    }
                }

            protected:
                value_type& _at(size_t pos)
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
                        _m_dyn_data = (value_type*)malloc(tmpcapa*sizeof(value_type));
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

                ::pilo::error_number_t _push_back(value_type ch)
                {
                    ::pilo::error_number_t ret = ::pilo::EC_OK;
                    if (nullptr != _m_dyn_data) //use dynamic buffer
                    {
                        if (available_capacity() <= 0)
                        {
                            ret = _reserve(_m_dyn_capacity + 1);
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
                            ret = _reserve(BUFSZ_DFL + 1);
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

                ::pilo::error_number_t _append(const value_type* suffix_str, size_t pos, size_t len)
                {
                    if (suffix_str == nullptr)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    if (pos == MC_INVALID_SIZE)
                    {
                        return ::pilo::EC_INVALID_PARAM;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = ::pilo::core::string::string_util::length(suffix_str + pos);
                    }

                    if (len == 0)
                    {
                        return ::pilo::EC_OK;
                    }

                    if (available_capacity() < len)
                    {
                        if (::pilo::EC_OK != grow_from_size(len))
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                    }

                    if (MC_INVALID_SIZE == ::pilo::core::string::string_util::copy(data() + _m_size, available_capacity() + 1, suffix_str + pos, len))
                    {
                        return ::pilo::EC_COPY_STRING_FAILED;
                    }

                    _m_size += len;

                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _insert(size_t pos, const value_type* str, size_t len)
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
                        len = ::pilo::core::string::string_util::length(str);
                    }

                    if (available_capacity() < len)
                    {
                        if (::pilo::EC_OK != grow_from_size(len))
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                    }

                    ::memmove(data() + pos + len, data() + pos, (size() - pos) * sizeof(value_type) );
                    ::memmove(data() + pos, str, len * sizeof(value_type));
                    _m_size += len;
                    data()[_m_size] = 0;

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

                auto_string& operator+=(const value_type* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& operator+=(const std::wstring stdstr)
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }
                template<size_t SZ>
                auto_string& operator+=(const auto_string<value_type, SZ>& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
                }

                ::pilo::i32_t compare(const value_type* str, size_t len_to_compare = MC_INVALID_SIZE) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), str, len_to_compare);
                }
                ::pilo::i32_t compare(const std::wstring& str) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), str.c_str(), MC_INVALID_SIZE);
                }
                template <size_t SZ>
                ::pilo::i32_t compare(const auto_string<value_type, SZ>& astr) const
                {
                    return ::pilo::core::string::string_util::binary_compare(this->c_str(), astr.c_str(), MC_INVALID_SIZE);
                }

                void recalculate_size()
                {
                    if (_m_pdata == nullptr)
                    {
                        _m_size = 0;
                    }
                    else
                    {
                        _m_size = ::pilo::core::string::string_util::length(_m_pdata);
                    }
                }

                void clear()
                {
                    if (nullptr != _m_pdata) //use dynamic buffer
                    {
                        *_m_pdata = 0;
                    }

                    _m_size = 0;
                }

                ::pilo::error_number_t append_string(const value_type* suffix_str, size_t pos, size_t len)
                {
                    return _append(suffix_str, pos, len);
                }

                auto_string& append(const value_type* suffix_str, size_t pos, size_t len)
                {
                    _append(suffix_str, pos, len);
                    return *this;
                }
                auto_string& append(const std::wstring& stdstr)
                {
                    _append(stdstr.c_str(), 0, stdstr.size());
                    return *this;
                }

                auto_string& append(const value_type* cstr)
                {
                    _append(cstr, 0, MC_INVALID_SIZE);
                    return *this;
                }
                template<size_t SZ>
                auto_string& append(const auto_string<value_type, SZ>& astr)
                {
                    _append(astr.c_str(), 0, astr.size());
                    return *this;
                }

                auto_string& insert(size_t pos, const value_type* str, size_t len)
                {
                    _insert(pos, str, len);
                    return *this;
                }
                auto_string& insert(size_t pos, const value_type* str)
                {
                    _insert(pos, str, MC_INVALID_SIZE);
                    return *this;
                }
                auto_string& insert(size_t pos, const std::wstring & str)
                {
                    _insert(pos, str.c_str(), str.size());
                    return *this;
                }
                template<size_t SZ>
                auto_string& insert(size_t pos, const auto_string<value_type, SZ>& astr)
                {
                    _insert(pos, astr.c_str(), astr.size());
                    return *this;
                }

                inline pilo::error_number_t reserve(size_t sz) { return _reserve(sz); }
                inline pilo::error_number_t grow_from_size(size_t sz) { return _reserve(_m_size + sz); }

                inline size_t available_capacity() const
                {
                    return _m_capacity - _m_size;
                }

                value_type& at(size_t pos)
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                const value_type& at(size_t pos) const
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }

                value_type& operator[] (size_t pos)
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }
                const value_type& operator[] (size_t pos) const
                {
                    if (pos > _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(pos);
                }

                value_type& back()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }
                const value_type& back() const
                {

                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_UPPER_LIMIT;
                    }

                    return _at(_m_size - 1);
                }

                value_type& front()
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(0);
                }

                const value_type& front() const
                {
                    if (0 == _m_size)
                    {
                        M_ASSERT(false);
                        throw pilo::EC_REACH_LOWER_LIMIT;
                    }

                    return _at(0);
                }


                inline bool empty() const { return (_m_size == 0); }
                bool is_dynamic() const { return true; }
                const value_type* c_str() const { return _m_pdata; }
                value_type* data() { return _m_pdata; }
                size_t size() const { return _m_size; }
                size_t length() const { return _m_size; }
                size_t capacity() const { return _m_capacity; }
                ::pilo::error_number_t assign(const value_type* cstr, size_t len) { return _assign(cstr, len); }
                inline void push_back(value_type ch) { _push_back(ch); }
                inline void pop_back() { _pop_back(); }

                size_t find_first_of ( value_type c, size_t pos = 0 ) const
                {
                    if (pos >= _m_size) return MC_INVALID_SIZE; 
                    const value_type* p = ::pilo::core::string::string_util::find(_m_pdata, c, pos, MC_INVALID_SIZE);
                    if (p == nullptr)
                    {
                        return MC_INVALID_SIZE;
                    }
                    return (p - _m_pdata) / sizeof(value_type);             
                }

                void erase_front_until(char ch, bool inc)
                {                    
                    ::pilo::core::string::string_util::erase_front_until(_m_pdata, ch,_m_size, inc);     
                }

            protected:
                value_type& _at(size_t pos)
                {
                    return _m_pdata[pos];
                }
                const value_type& _at(size_t pos) const
                {
                    return _m_pdata[pos];
                }
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

                ::pilo::error_number_t _push_back(value_type ch)
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

                ::pilo::error_number_t _pop_back()
                {
                    if (_m_size <= 0)
                    {
                        return ::pilo::EC_REACH_LOWER_LIMIT;
                    }
                    _m_pdata[--_m_size] = 0;
                    return ::pilo::EC_OK;
                }

                ::pilo::error_number_t _append(const value_type* suffix_str, size_t pos, size_t len)
                {
                    if (suffix_str == nullptr)
                    {
                        return ::pilo::EC_NULL_PARAM;
                    }

                    if (pos == MC_INVALID_SIZE)
                    {
                        return ::pilo::EC_INVALID_PARAM;
                    }

                    if (len == MC_INVALID_SIZE)
                    {
                        len = ::pilo::core::string::string_util::length(suffix_str + pos);
                    }

                    if (len == 0)
                    {
                        return ::pilo::EC_OK;
                    }

                    if (available_capacity() < len)
                    {
                        if (::pilo::EC_OK != _reserve(_m_capacity + len))
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                    }

                    if (MC_INVALID_SIZE == ::pilo::core::string::string_util::copy(_m_pdata + _m_size, available_capacity() + 1, suffix_str + pos, len))
                    {
                        return ::pilo::EC_COPY_STRING_FAILED;
                    }

                    _m_size += len;

                    return ::pilo::EC_OK;
                }

                ::pilo::i32_t _insert(size_t pos, const value_type* str, size_t len)
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
                        len = ::pilo::core::string::string_util::length(str);
                    }

                    if (available_capacity() < len)
                    {
                        if (::pilo::EC_OK != grow_from_size(len))
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                    }

                    ::memmove(_m_pdata + pos + len, _m_pdata + pos, (size() - pos) * sizeof(value_type));
                    ::memmove(_m_pdata + pos, str, len* sizeof(value_type));
                    _m_size += len;
                    _m_pdata[_m_size] = 0;

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