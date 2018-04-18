#pragma once

#include "core/threading/dummy_mutex.hpp"
#include "core/container/singly_linked_selflist.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/memory/fixed_memory_piece.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  size_t _UnitSize, 
                        size_t _Step, 
                        typename _Lock = pilo::core::threading::dummy_mutex>
            class memory_pool
            {
            public:
                typedef _Lock                                                       lock_type;
                typedef fixed_memory_piece_node<_UnitSize, _Step>                   piece_type;
                typedef typename piece_type::unit_node                              unit_node;
                //typedef typename piece_type::unit_type                              unit_type;
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
                memory_pool()
                {

                }
                ~memory_pool()
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

                pointer allocate()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);

                    unit_node* ptr = m_free_unit_list.pop_front();
                    if (ptr != nullptr) //there is one unit available in freelist
                    {
                        ptr->m_next = (unit_node*) MC_INVALID_PTR;
                        return ptr;
                    }

                    //check if available piece list has available list
                    piece_type* piece = m_available_piece_list.begin();
                    if (nullptr == piece)
                    {
                        piece = new piece_type(); //no, make it now
                        m_available_piece_list.push_back(piece);
                    }

                    //now available list has unit can use
                    ptr = (unit_node*) piece->alloc_unit();
                    M_ASSERT(nullptr != ptr); //must be a valid unit
                    ptr->m_next = (unit_node*) MC_INVALID_PTR;

                    //after alloc if piece is full, shift it to full list
                    if (!piece->available())
                    {
                        m_available_piece_list.pop_front();
                        m_full_piece_list.push_back(piece);
                    }

                    return ptr;
                }

                void deallocate(pointer ptr)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    
                    unit_node* u = (unit_node*) ptr;
                    M_ASSERT(u->m_next == (unit_node*) MC_INVALID_PTR);
                    m_free_unit_list.push_back(u);
                }

                size_type piece_count() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(m_lock);
                    return m_full_piece_list.size() + m_available_piece_list.size();
                }                
            };            
        }
    }
}