#pragma once

#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace container
        {
            template <typename _ELEM, size_t _CAPA>
            class fixed_array
            {
            public:
                typedef _ELEM   value_type;

            protected:
                size_t          m_size;
                value_type      m_data[_CAPA];

            public:
                fixed_array() : m_size(0)
                {
                    M_ASSERT(_is_simple_type_<_ELEM>::value);
                }

                fixed_array(const value_type* src, size_t len, bool allow_trunc = false)
                {
                    M_ASSERT(_is_simple_type_<_ELEM>::value);
                    if (len > _CAPA)
                    {
                        if (allow_trunc)
                        {
                            m_size = _CAPA;                        
                        }
                        else
                        {
                            M_ASSERT(false);
                        }
                    }
                    else
                    {
                        m_size = len;
                        
                    }  

                    ::memcpy(m_data, src, m_size*sizeof(value_type));
                }

                fixed_array(const fixed_array& o)
                {
                    M_ASSERT(_CAPA >= o.m_size);

                    m_size = o.m_size;
                    ::memcpy(m_data, o.m_data, m_size*sizeof(value_type));
                }

                fixed_array& operator= (const fixed_array& o)
                {
                    M_ASSERT(_CAPA >= o.m_size);

                    if (this != &o)
                    {
                        m_size = o.m_size;
                        ::memcpy(m_data, o.m_data, m_size*sizeof(value_type));
                    }                     
                    
                    return *this;
                }

                size_t full() const
                {
                    return (m_size >= _CAPA);
                }

                size_t capacity() const
                {
                    return _CAPA;
                }

                size_t size() const
                {
                    return m_size;
                }

                void clear()
                {
                    m_size = 0;
                }

                bool empty() const
                {
                    return (m_size == 0 ? true : false);
                }

                template<typename _E>
                size_t find(const value_type& val, _E equal_cmp) const
                {
                    for (size_t i = 0; i < m_size; i++)
                    {
                        if (equal_cmp(m_data[i], val))
                        {
                            return i;
                        }
                    }

                    return MC_INVALID_SIZE;
                }

                size_t find(const value_type& val)
                {
                    for (size_t i = 0; i < m_size; i++)
                    {
                        if (m_data[i] == val)
                        {
                            return i;
                        }
                    }

                    return MC_INVALID_SIZE;
                }

                value_type& at(size_t idx)
                {
                    M_ASSERT(idx < _CAPA);
                    return m_data[idx];
                }

                const value_type& at(size_t idx) const
                {
                    M_ASSERT(idx < m_size);
                    return m_data[idx];
                }

                value_type& operator[](size_t idx)
                {
                    M_ASSERT(idx < _CAPA);
                    return m_data[idx];
                }

                const value_type& operator[](size_t idx) const
                {
                    M_ASSERT(idx < m_size);
                    return m_data[idx];
                }

                value_type& get(size_t idx)
                {
                    M_ASSERT(idx < m_size);
                    return m_data[idx];
                }

                fixed_array& push_back(const value_type& val)
                {
                    this->at(m_size++) = val;
                    return *this;
                }

                value_type pop_back()
                {
                    M_ASSERT(m_size);
                    m_size -= 1;
                    return m_data[m_size];
                }

                value_type& back()
                {
                    M_ASSERT(m_size);
                    return m_data[m_size - 1];
                }

                const value_type& back() const
                {
                    M_ASSERT(m_size);
                    return m_data[m_size - 1];
                }

                value_type& front()
                {
                    M_ASSERT(m_size);
                    return m_data[0];
                }

                const value_type& front() const
                {
                    M_ASSERT(m_size);
                    return m_data[0];
                }


                fixed_array& erase(size_t idx)
                {
                    M_ASSERT(idx < m_size);
                    if (idx >= m_size) return *this;

                    size_t len = (m_size - idx - 1) * sizeof(value_type);
                    if (len > 0)
                    {
                        void* pSrc = &m_data[idx + 1];
                        void* pDst = &m_data[idx];
                        ::memmove(pDst, pSrc, len);
                    }
                    m_size -= 1;
                    return *this;
                }

                fixed_array& insert(size_t idx, const value_type& val)
                {
                    M_ASSERT(idx <= m_size);
                    if (idx == m_size)
                    {
                        return push_back(val);
                    }

                    size_t sz = m_size + 1;
                    M_ASSERT(sz <= _CAPA);

                    

                    size_t len = (m_size - idx)*sizeof(value_type);
                    if (len > 0)
                    {
                        void* pSrc = &m_data[idx];
                        void* pDst = &m_data[idx + 1];
                        memmove(pDst, pSrc, len);
                    }
                    m_data[idx] = val;
                    m_size += 1;
                    return *this;
                }

				value_type* data()
				{
					return m_data;
				}

            };


        }
    }
}