#pragma once

#include "core/threading/dummy_mutex.hpp"
#include "core/container/singly_linked_selflist.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/memory/fixed_overlapped_memory_piece.hpp"

#ifdef _DEBUG_MEM_OBJ_POOL
#include <string>
#endif

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  size_t _UnitSize, 
                        size_t _Step, 
                        typename _Lock = pilo::core::threading::dummy_mutex>
            class overlapped_memory_pool
            {
            public:
                typedef _Lock                                                       lock_type;
                typedef fixed_overlapped_memory_piece_node<_UnitSize, _Step>        piece_type;
                typedef typename piece_type::unit_node                              unit_node;
                typedef pilo::core::container::singly_linked_selflist<piece_type>   piece_list;
                typedef pilo::core::container::singly_linked_selflist<unit_node>    unit_list;
                typedef size_t                                                      size_type;
                typedef void*                                                       pointer;
                typedef const void*                                                 const_pointer;

            public:
                mutable lock_type   m_lock;
                piece_list          m_full_piece_list;
                piece_list          m_available_piece_list;
                unit_list           m_free_unit_list;

            public:
                overlapped_memory_pool()
                {

                }
                ~overlapped_memory_pool()
                {
                    this->reset();
                }
                void reset()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    m_free_unit_list.clear();
                    piece_type* piece = nullptr;
                    while (piece = m_available_piece_list.pop_front(), piece != nullptr)
                        delete piece;
                    while (piece = m_full_piece_list.pop_front(), piece != nullptr)
                        delete piece;
                }
                void clear()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    m_free_unit_list.clear();
                    piece_type* piece = m_available_piece_list.begin();
                    for (; piece != nullptr; piece = m_available_piece_list.next(piece)) 
                    { 
                        piece->clear();  //all storage in this piece is cleared, now this piece is empty.
                    }
                    while (piece = m_full_piece_list.pop_front(), piece != nullptr) 
                    { 
                        piece->clear();  //all storage in this piece is cleared, now this piece is empty.
                        m_available_piece_list.push_back(piece); //return the piece back to available list
                    }
                }

                inline pointer allocate()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    unit_node* ptr = m_free_unit_list.pop_front();
                    if (ptr != nullptr)
                    {
                        ptr->m_flag = 0;
                        return ptr;
                    }

                    piece_type* piece = m_available_piece_list.begin();
                    if (nullptr == piece)
                    {
                        piece = new piece_type();
                        m_available_piece_list.push_back(piece);
                    }

                    ptr = (unit_node*)piece->alloc_unit();
                    if (! piece->available())
                    {
                        m_available_piece_list.pop_front();
                        m_full_piece_list.push_back(piece);
                    }
                    ptr->m_flag = 0;
                    return ptr;
                }

                inline void deallocate(pointer ptr)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    unit_node* unode = (unit_node*)ptr;
                    M_ASSERT(unode->m_flag != MC_MEM_POOL_UNIT_FREED_FLAG);
                    unode->m_flag = MC_MEM_POOL_UNIT_FREED_FLAG;
                    m_free_unit_list.push_back(unode);
                }
 
                size_type piece_count() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return m_full_piece_list.size() + m_available_piece_list.size();
                } 

#ifdef _DEBUG_MEM_OBJ_POOL
            public:
                std::string make_report(::pilo::u32_t flag = 0, size_t level = 0)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    std::string str;
                    str.clear();
                    ::pilo::core::string::fixed_astring<128> tmp_str;
                    std::string tab_str;
                    tab_str.clear();
                    for (int i = 0; i < level; i++)
                    {
                        tab_str += "    ";
                    }

                    tmp_str.format("%sOverlapped_memory_pool @ 0x%p:\n", tab_str.c_str(), this);
                    str += tmp_str.c_str();

                    size_t fl_count = m_free_unit_list.size();
                    tmp_str.format("%s    Total Free Count:%u\n", tab_str.c_str(), fl_count);
                    str += tmp_str.c_str();

                    size_t i = 0;
                    if (flag & MB_DEBUG_POOL_SHOW_FREELIST_DETAIL)
                    {
                        unit_node* node = m_free_unit_list.begin();
                        for (; node != m_free_unit_list.end(); node = m_free_unit_list.next(node))
                        {
                            tmp_str.format("%s        Free object_%u\t: @ 0x%p\n", tab_str.c_str(), i++, node);
                            str += tmp_str.c_str();
                        }
                    }

                    size_t fu_count = m_full_piece_list.size();
                    tmp_str.format("%s    Full list Count:%u\n", tab_str.c_str(), fu_count);
                    str += tmp_str.c_str();
                    i = 0;



                    size_t av_count = m_available_piece_list.size();
                    tmp_str.format("%s    Aval list Count:%u\n", tab_str.c_str(), av_count);
                    str += tmp_str.c_str();

                    if (flag & MB_DEBUG_POOL_SHOW_AVAILIST_DETAIL)
                    {
                        piece_type* piece = m_available_piece_list.begin();
                        for (; piece != m_available_piece_list.end(); piece = m_available_piece_list.next(piece))
                        {
                            std::string stdstr2 = piece->make_report(flag, level + 2);
                            str += stdstr2;
                        }
                    }  

                    return str;

                }
#endif
            };
        }
    }
}
