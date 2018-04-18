#pragma once

#include "core/coredefs.hpp"
#include "fixed_array.hpp"

namespace pilo
{
    namespace core
    {
        namespace container
        {
            template< class _ELEM, size_t _CAPA, \
                typename _C = ref_gel_compare_greater<_ELEM>, \
                typename _E = ref_gel_compare_greater<_ELEM> >
            class fixed_bin_heap
            {
            public:
                typedef _ELEM   value_type;
                typedef _C      cmp_type;
                typedef _E      equal_type;

            public:
                fixed_bin_heap() {}
                fixed_bin_heap( _ELEM array, size_t num)
                {
                    for (size_t i = 0; i < num; i++)
                    {
                        insert(array[i]);
                    }
                }
                ~fixed_bin_heap() {}

            protected:
                fixed_array<value_type, _CAPA>  m_array;
                value_type                      m_swap;
                cmp_type                        m_comparator;
                equal_type                      m_eq_comparator;

            protected:               

                inline size_t parent_index(size_t idx)
                {
                    if (idx == 0)
                    {
                        return MC_INVALID_SIZE;
                    }
                    return (idx + 1) / 2 - 1;
                }

                inline size_t lchild_index(size_t idx)
                {
                    size_t nResult = 2 * (idx + 1) - 1;
                    if (nResult < m_array.size())
                        return nResult;
                    else return MC_INVALID_SIZE;
                }

                inline size_t rchild_index(size_t idx)
                {
                    size_t nResult = 2 * (idx + 1);
                    if (nResult < m_array.size())
                        return nResult;
                    else return MC_INVALID_SIZE;
                }

                inline void swap(size_t nIndex1, size_t nIndex2)
                {
                    M_ASSERT(nIndex1 != MC_INVALID_SIZE);
                    M_ASSERT(nIndex2 != MC_INVALID_SIZE);

                    m_swap = m_array[nIndex1];
                    m_array[nIndex1] = m_array[nIndex2];
                    m_array[nIndex2] = m_swap;
                }

                inline size_t min_child_index(size_t nIndex)
                {
                    size_t nResult = lchild_index(nIndex);
                    if (MC_INVALID_SIZE == nResult)
                    {
                        return MC_INVALID_SIZE;
                    }
                    else
                    {
                        size_t nRightIndex = rchild_index(nIndex);
                        if (MC_INVALID_SIZE == nRightIndex)
                        {
                            return nResult;
                        }
                        else if (1 == m_comparator(m_array[nResult], m_array[nRightIndex]))
                        {
                            return nResult;
                        }                            
                        else
                        {
                            return nRightIndex;
                        }   
                    }
                }

                void push_down(size_t nIndex)
                {
                    size_t nMinChildIndex = MC_INVALID_SIZE;
                    if (MC_INVALID_SIZE != (nMinChildIndex = min_child_index(nIndex)))
                    {
                        if (-1 == m_comparator(m_array[nIndex], m_array[nMinChildIndex]))
                        {
                            swap(nIndex, nMinChildIndex);
                            push_down(nMinChildIndex);
                        }
                    }

                }

                void push_up(size_t nIndex)
                {
                    size_t nParentIndex = MC_INVALID_SIZE;
                    if (MC_INVALID_SIZE != (nParentIndex = parent_index(nIndex)))
                    {
                        if (1 == m_comparator(m_array[nIndex], m_array[nParentIndex]))
                        {
                            swap(nIndex, nParentIndex);
                            push_up(nParentIndex);
                        }
                    }
                }

                public:

                    inline size_t size() const
                    {
                        return m_array.size();
                    }

                    inline bool empty() const
                    {
                        return m_array.empty();
                    }

                    inline bool full() const
                    {
                        return m_array.full();
                    }

                    void clear()
                    {
                        m_array.clear();
                    }

                    template<typename _EC>
                    void value_changed(const value_type& v, _EC equal_cmp)
                    {
                         size_t index = m_array.find(v, equal_cmp);
                        if (index == MC_INVALID_SIZE)
                        {
                            return;
                        }

                        push_up(index);                                             
                    }

                    void push(const value_type& val)
                    {
                        if (m_array.full())
                        {
                            M_ASSERT(false);
                        }
                        m_array.push_back(val);
                        push_up(m_array.size() - 1);
                    }

                    bool pop()
                    {
                        if (m_array.empty())
                        {
                            return false;
                        }
                        size_t nSize = m_array.size();
                        m_array[0] = m_array[nSize - 1];
                        m_array.erase(nSize - 1);
                        push_down(0);
                        return true;
                    }

                    value_type* top()
                    {
                        if (m_array.empty())
                        {
                            return nullptr;
                        }                            
                        else
                        {
                            return &(m_array[0]);
                        }                            
                    }

                    bool erase(const value_type& val)
                    {
                        size_t nSize = m_array.size();
                        size_t nPos = 0;
                        for (; nPos < nSize; nPos++)
                        {
                            if (0 == m_eq_comparator(m_array[nPos], val))
                            {
                                break;
                            }
                        }
                        if (nPos == nSize)
                        {
                            return false;
                        }

                        m_array[nPos] = m_array[nSize - 1];
                        m_array.erase(nSize - 1);
                        push_up(nPos);
                        push_down(nPos);
                        return true;
                    }
            };
        }
    }
}