#pragma once

#include <mutex>
#include "../../pilo.hpp"
#include "../threading/dummy_mutex.hpp"
#include "../container/singly_linked_selflist.hpp"
#include "../memory/fixed_memory_piece.hpp"
#include "memory_pool_allocator_interface.hpp"


#include <sstream>
#include "../io/formatted_io.hpp"


namespace pilo
{
    namespace core
    {
        namespace memory
		{
            template <  ::pilo::i64_t _UnitSize, 
                        ::pilo::i64_t _Step, 
                        typename _Lock = pilo::core::threading::dummy_mutex>
            class memory_pool : public memory_pool_allocator_interface
            {
            public:
                typedef _Lock                                                       lock_type;
                typedef fixed_overlapped_memory_piece_node<_UnitSize, _Step>        piece_type;
                typedef typename piece_type::unit_node                              unit_node;
                typedef pilo::core::container::singly_linked_selflist<piece_type>   piece_list;
                typedef pilo::core::container::singly_linked_selflist<unit_node>    unit_list;
				typedef ::pilo::i64_t                                                size_type;


            public:
                mutable lock_type   m_mutex;
                piece_list          m_full_piece_list;
                piece_list          m_available_piece_list;
                unit_list           m_free_unit_list;
                

            public:
                memory_pool()
                {

                }
                ~memory_pool()
                {
                    this->reset();
                }
                void reset()
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    m_free_unit_list.clear();
                    piece_type* piece = nullptr;
                    while (piece = m_available_piece_list.pop_front(), piece != nullptr)
                        delete piece;
                    while (piece = m_full_piece_list.pop_front(), piece != nullptr)
                        delete piece;
                }
                void clear()
                {
                    std::lock_guard<lock_type> locker(m_mutex);
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

                ::pilo::i64_t unit_size() const 
                {
                    return _UnitSize;
                }

                inline ::pilo::pointer allocate()
                {
                    std::lock_guard<lock_type> locker(m_mutex);
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

                inline void deallocate(::pilo::pointer ptr)
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    unit_node* unode = (unit_node*)ptr;
                    PMC_ASSERT(unode->m_flag != PMB_MEM_POOL_UNIT_FREED_FLAG);
                    unode->m_flag = PMB_MEM_POOL_UNIT_FREED_FLAG;
                    m_free_unit_list.push_back(unode);
                }
 
                ::pilo::int_r piece_count() const
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    return m_full_piece_list.size() + m_available_piece_list.size();
                } 

                void calc_available_units_nolock(::pilo::i64_t& avial, ::pilo::i64_t& total, ::pilo::i64_t count_in_freelist) const
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    _calc_available_units_nolock(avial, total, count_in_freelist);
                }

                void _calc_available_units_nolock(::pilo::i64_t& avial, ::pilo::i64_t & total, ::pilo::i64_t count_in_freelist) const
                {
                    const piece_type* piece = m_available_piece_list.cbegin();
                    for (; piece != m_available_piece_list.cend(); piece = m_available_piece_list.next(piece))
                    {
                        avial += piece->m_available;
                        total += _Step;
                    }

                    if (count_in_freelist > 0)
                    {
                        avial += count_in_freelist;
                    }
                    else
                    {
                        avial += this->m_free_unit_list.size();
                    }
                    

                    piece = m_full_piece_list.cbegin();
                    for (; piece != m_full_piece_list.cend(); piece = m_full_piece_list.next(piece))
                    {
                        total += _Step;
                    }

                }

                std::string to_string() const
                {
                    std::lock_guard<lock_type> locker(m_mutex);
                    ::pilo::i64_t avail = 0;
                    ::pilo::i64_t total = 0;
                    _calc_available_units_nolock(avail, total);
                    std::stringstream ss;
                    ss << "MemPool:" << "us:" << avail << "/" << total;
                    return ss.str();
                }
            };
        }
    }
}
