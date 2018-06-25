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
                ::pilo::i64_t calculate_spare_available_units_nolock()
                {
                    ::pilo::i64_t ret = 0;
                    piece_type* piece = m_available_piece_list.begin();
                    for (; piece != m_available_piece_list.end(); piece = m_available_piece_list.next(piece))
                    {
                        ret += piece->m_available;
                    }
                    return ret;
                }

                std::string& make_summary_report(std::string& str)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    unsigned int ind = 0;
                    char buffer[256];
                    ::pilo::core::io::string_format_output(buffer, sizeof(buffer), 
                        "(OMP@%p) FreeNode=%u FullList=%u AvalList=%u\n", this, 
                        (unsigned int) m_free_unit_list.size(),
                        (unsigned int) m_full_piece_list.size(),
                        (unsigned int) m_available_piece_list.size());
                    str += buffer;

                    str += "  FullPieceList:\n";
                    piece_type* piece = m_full_piece_list.begin();
                    for (; piece != m_full_piece_list.end(); piece = m_full_piece_list.next(piece))
                    {
                        piece->make_summary_report(ind++, buffer, 256);
                        str+=buffer;
                    }

                    ind = 0;
                    str += "  AvalPieceList:\n";
                    piece = m_available_piece_list.begin();
                    for (; piece != m_available_piece_list.end(); piece = m_available_piece_list.next(piece))
                    {
                        piece->make_summary_report(ind++, buffer, 256);
                        str+=buffer;
                    }

                    return str;
                }
#endif
            };
        }
    }
}
