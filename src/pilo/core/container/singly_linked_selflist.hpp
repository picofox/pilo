#pragma once

#include "../../pilo.hpp"
#include "../threading/dummy_mutex.hpp"
#include <mutex>


namespace pilo
{
    namespace core
    {
        namespace container
        {                                                    
            template <typename T> struct singly_linked_list_node
            {
                typedef T value_type;
                value_type                  m_value;
                singly_linked_list_node*    m_next;
            };

            template<typename _Value, typename _Lock = pilo::core::threading::dummy_mutex> 
            class singly_linked_selflist
            {
            public:
                typedef _Value value_type;
                typedef _Lock  lock_type;
                

            public:
                singly_linked_selflist() : m_size(0), m_head(nullptr), m_tail(nullptr)
                {
                }

                inline void clear()
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    m_size = 0;
                    m_head = nullptr;
                    m_tail = nullptr;
                }

                inline bool empty() const
                {
                    return m_head == nullptr;
                }

                inline ::pilo::i64_t size() const
                {
                    return m_size;
                }

                //element access
                inline const value_type* front() const
                {
                    return (const value_type*) m_head;
                }
                inline value_type* front()
                {
                    return (value_type*)m_head;
                }
                inline const value_type* back() const
                {
                    return (const value_type*) m_tail;
                }
                inline value_type* back()
                {
                    return (value_type*)m_tail;
                }
                
                //op
                inline value_type* pop_front()
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    if (m_head == nullptr) return nullptr;

                    value_type* node = (value_type*)m_head;
                    m_head = (volatile value_type*)node->m_next;
                    if (nullptr == m_head)
                    {
                        m_tail = nullptr;
                    }
                    node->m_next = nullptr;

                    m_size--;

                    return node;
                }
                inline void push_back(value_type* node)
                {
                    std::lock_guard<lock_type> locker(m_mutex);

                    node->m_next = nullptr;
                    if (m_head != nullptr)
                    {
                        m_tail->m_next = node;
                    }
                    else
                    {
                        m_head = node;
                    }
                    m_tail = node;
                    m_size++;
                }
                inline void push_front(value_type* node)
                {
                    std::lock_guard<lock_type> locker(m_mutex);

                    if (m_head == nullptr)
                    {
                        node->m_next = nullptr;
                        m_tail = node;
                    }
                    else
                    {
                        node->m_next = (value_type*)m_head;
                    }

                    m_head = node;
                    m_size += 1;
                }

                inline bool erase_after(value_type* prev, value_type** deleted_node = nullptr)
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    if (deleted_node != nullptr)
                    {
                        *deleted_node = nullptr;
                    }

                    if (prev == nullptr)
                    {
                        if (empty())
                        {
                            return false;
                        }
                        if (deleted_node != nullptr)
                        {
                            *deleted_node = front();
                        }
                        this->pop_front();
                        return true;
                    }
                    value_type* node_to_del = prev->m_next;
                    if (nullptr == node_to_del)
                    {
                        return false;
                    }
                    value_type* pNextNext = node_to_del->m_next;
                    node_to_del->m_next = nullptr;
                    prev->m_next = pNextNext;
                    if (nullptr == pNextNext)
                    {
                        m_tail = prev;
                    }
                    PMC_ASSERT(m_size > 0);
                    m_size--;
                    if (deleted_node != nullptr)
                    {
                        *deleted_node = node_to_del;
                    }
                    return true;
                }

                inline bool erase(value_type* node)
                {
                    std::lock_guard<lock_type> locker(m_mutex);

                    if (nullptr == m_head) return false;
                    if (nullptr == node) return false;

                    value_type* pPrev = nullptr;
                    value_type* p = (value_type*)m_head;
                    do
                    {
                        if (p == node) //find it
                        {
                            if (pPrev) //not first element
                            {
                                pPrev->m_next = p->m_next; //relink
                                if (node->m_next == nullptr) //last node to delete, must update tail ptr
                                {
                                    m_tail = pPrev;
                                }
                            }
                            else //first element, update head
                            {
                                m_head = p->m_next;
                                if (nullptr == m_head) //empty 
                                {
                                    m_tail = nullptr;
                                }
                            }
                            m_size--;
                            return true;
                        }
                        pPrev = p;
                    } while (p = p->m_next, p);

                    return false;
                }

                inline bool find(const value_type* node, value_type** saved_prev_node) const
                {
                    std::lock_guard<lock_type> locker(m_mutex);

                    if (saved_prev_node != nullptr)
                    {
                        *saved_prev_node = nullptr;
                    }

                    if (m_head == nullptr)
                    {
                        return false;
                    }

                    value_type* p = (value_type*)m_head;
                    do
                    {
                        if (p == node)
                        {
                            return true;
                        }

                        if (saved_prev_node != nullptr)
                        {
                            *saved_prev_node = p;
                        }
                    } while (p = p->m_next, p);

                    return false;
                }

                //mutex
                inline void lock() const
                {
                    m_mutex.lock();
                }

                inline void unlock() const
                {
                    m_mutex.unlock();
                }

                //iterator
                inline value_type* begin() 
                {
                    return (value_type*)m_head;
                }

                inline const value_type* cbegin() const
                {
                    return (const value_type*)m_head;
                }

                inline value_type* end() 
                {
                    return (value_type*) nullptr;
                }

                inline const value_type* cend() const
                {
                    return (const value_type*) nullptr;
                }

                inline value_type* rbegin() 
                {
                    return (value_type*)m_tail;
                }

                inline value_type* next(value_type* pNode) 
                {
                    return pNode->m_next;
                }
    
                inline const value_type* next(const value_type* pNode) const
                {
                    return (const value_type*) pNode->m_next;
                }

            protected:
                ::pilo::i64_t           m_size;
                volatile value_type*    m_head;
                value_type*             m_tail;
                lock_type               m_mutex;
            private:
                PMC_DISABLE_COPY(singly_linked_selflist);
            };       


        }
    }
}