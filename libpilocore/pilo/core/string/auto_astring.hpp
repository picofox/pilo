#include "fixed_astring.hpp"
#include "dynamic_astring.hpp"

namespace pilo
{
    namespace core
    {
        namespace string
        {
            enum EnumAAStringTypeInUsing
            {
                eASTIU_None = 0,
                eASTIU_Fixed = 1,
                eASTIU_Dynamic = 2,
            };

            template<size_t _CAPACITY>
            class auto_astring
            {
            public:
                

            protected:
                fixed_astring<_CAPACITY>    _m_fastring;
                dynamic_astring             _m_dastring;


            protected:
                

                pilo::i32_t _assign(const char* str, size_t len)
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed || t == eASTIU_None)
                    {
                        if (len < _CAPACITY)
                        {
                            _m_fastring.assign(str, len);
                            _m_dastring.reset();
                            return pilo::EC_OK;
                        }                        
                    }

                    _m_dastring.assign(str, len);
                    _m_fastring.clear();
                    return pilo::EC_OK;
                }

                pilo::i32_t _assign(const char* str)
                {
                    size_t len = ::pilo::core::string::string_util::length(str);
                    return  this->_assign(str, len);
                }


            public:
                auto_astring()
                {

                }

                auto_astring(const char* str)
                {
                    _assign(str);
                }

                auto_astring(const char* str, size_t len)
                {
                    _assign(str, len);
                }

                template<size_t capa> 
                auto_astring(const auto_astring<capa>& str)
                {
                    _assign(str.c_str(), str.length());
                }

                auto_astring<_CAPACITY>& operator=(const auto_astring<_CAPACITY>& str)
                {
                    if (this != &str)
                    {
                        _assign(str.c_str(), str.length());
                    }
                    return *this;
                }
                template<size_t capa>
                auto_astring<_CAPACITY>& operator=(const auto_astring<capa>& str)
                {
                    if (this != &str)
                    {
                        _assign(str.c_str(), str.length());
                    }
                    
                    return *this;
                }
                auto_astring<_CAPACITY>& operator=(const char* str)
                {
                    _assign(str);
                    return *this;
                }


            public:
                void clear()
                {
                    _m_fastring.clear();
                    _m_dastring.clear();
                }

                void reset()
                {
                    _m_fastring.clear();
                    _m_dastring.reset();
                }

            public:
                operator const char* () const
                {
                    return this->c_str();
                }
                const char* c_str() const
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_fastring.c_str();
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring.c_str();
                    }
                    return nullptr;
                }
                size_t length() const
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_fastring.length();
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring.length();
                    }
                    return 0;
                }

                char& operator[] (size_t pos)
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_fastring[pos];
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring[pos];
                    }
                    M_ASSERT(false);    
                    throw pilo::EC_DATA_TYPE_MISMATCH;
                }

                const char& operator[] (size_t pos) const
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_fastring[pos];
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring[pos];
                    }
                    M_ASSERT(false);
                    throw pilo::EC_DATA_TYPE_MISMATCH;
                }

                char& at(size_t pos)
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_fastring[pos];
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring[pos];
                    }
                    M_ASSERT(false);
                    throw pilo::EC_DATA_TYPE_MISMATCH;
                }
                const char& at(size_t pos) const
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_fastring[pos];
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring[pos];
                    }
                    M_ASSERT(false);
                    throw pilo::EC_DATA_TYPE_MISMATCH;
                }
                char& back()
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_dastring.back();
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring.back();
                    }
                    M_ASSERT(false);
                    throw pilo::EC_DATA_TYPE_MISMATCH;

                }
                const char& back() const
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_dastring.back();
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring.back();
                    }
                    M_ASSERT(false);
                    throw pilo::EC_DATA_TYPE_MISMATCH;
                }
                char& front()
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_dastring.front();
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring.front();
                    }
                    M_ASSERT(false);
                    throw pilo::EC_DATA_TYPE_MISMATCH;
                }
                const char& front() const
                {
                    EnumAAStringTypeInUsing t = which_in_using();
                    if (t == eASTIU_Fixed)
                    {
                        return _m_dastring.front();
                    }
                    else if (t == eASTIU_Dynamic)
                    {
                        return _m_dastring.front();
                    }
                    M_ASSERT(false);
                    throw pilo::EC_DATA_TYPE_MISMATCH;
                }

                EnumAAStringTypeInUsing which_in_using() const
                {
                    if (_m_dastring.capacity() > 0)
                    {
                        return eASTIU_Dynamic;
                    }
                    if (_m_fastring.capacity() > 0)
                    {
                        return eASTIU_Fixed;
                    }
                    return eASTIU_None;
                }

            }; //end of class auto_+string
        } // end of string
    } //end of core
} //end of pilo