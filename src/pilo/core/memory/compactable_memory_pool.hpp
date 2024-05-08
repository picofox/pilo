#pragma once

#include <mutex>
#include "../threading/dummy_mutex.hpp"
#include "../container/singly_linked_selflist.hpp"
#include "../memory/memory_pool.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <  ::pilo::i64_t _UnitSize,
                        ::pilo::i64_t _Step,
                        typename _Lock = pilo::core::threading::dummy_mutex>
            class compactable_memory_pool
                : public memory_pool<_UnitSize, _Step, _Lock>
            {
            public:
                typedef _Lock                                              lock_type;
                typedef memory_pool<_UnitSize, _Step, _Lock>    base_type;
                typedef typename base_type::piece_type          piece_type;
                typedef typename base_type::unit_node           unit_node;
                typedef typename base_type::piece_list          piece_list;
                typedef typename base_type::unit_list           unit_list;
                typedef void*                                   pointer;
                typedef const void*                             const_pointer;
				typedef ::pilo::i64_t                            size_type;

            protected:
                bool _need_compact_nolock() const
                {
                    ::pilo::int_r piece_count = this->piece_count_nolock();
                    ::pilo::int_r free_obj_count = base_type::m_free_unit_list.size();
                    return (piece_count > 3) && (free_obj_count / (::pilo::int_r)_Step) >= (piece_count / 2);
                }

                bool _has_nolock(void* ptr) const
                {
                    const piece_type* piece = base_type::m_full_piece_list.begin();

                    for (; piece != nullptr; piece = base_type::m_full_piece_list.next(piece))
                    {
                        if (piece->has(ptr))
                        {
                            return true;
                        }
                    }
                    piece = base_type::m_available_piece_list.begin();
                    for (; piece != nullptr; piece = base_type::m_available_piece_list.next(piece))
                    {
                        if (piece->has(ptr))
                        {
                            return true;
                        }
                    }

                    return false;
                }

                bool _compact_piece_nolock(piece_type* piece)
                {
                    unit_node* u;
                    unit_list tmp_list;

                    while (u = base_type::m_free_unit_list.pop_front(), u != nullptr)
                    {
                        if (piece->dealloc_unit(u))
                        {
                            if (piece->empty())
                            {
                                piece->clear(); //reset this piece, set it's internal list to null
                                while (u = tmp_list.pop_front(), u != nullptr) //restore all node that not belong to this list
                                {
                                    base_type::m_free_unit_list.push_back(u);
                                }

                                return true;  //piece emptied
                            }
                        }
                        else //not belong to this list, save it for now.
                        {
                            tmp_list.push_back(u);
                        }
                    }

                    //restore those node that not belong this piece to free list.
                    while (u = tmp_list.pop_front(), u != nullptr)
                    {
                        base_type::m_free_unit_list.push_back(u);
                    }

                    return false;
                }

                void _compact_nolock()
                {
                    piece_type*     piece;
                    piece_list      tmp_list, empty_list;
                    ::pilo::int_r       piece_count = piece_count_nolock();

                    // full list
                    while (piece = base_type::m_full_piece_list.pop_front(), piece != nullptr)
                    {
                        if (this->_compact_piece_nolock(piece))
                        {
                            empty_list.push_back(piece);
                        }
                        else if (piece->available())
                        {
                            base_type::m_available_piece_list.push_back(piece);
                        }
                        else
                        {
                            tmp_list.push_back(piece);
                        }

                        if (base_type::m_free_unit_list.empty())
                        {
                            break;
                        }
                    }
                    while (piece = tmp_list.pop_front(), piece != nullptr)
                    {
                        base_type::m_full_piece_list.push_back(piece);
                    }

                    // avail list
                    while (piece = base_type::m_available_piece_list.pop_front(), piece != nullptr)
                    {
                        if (this->_compact_piece_nolock(piece))
                        {
                            empty_list.push_back(piece);
                        }
                        else
                        {
                            tmp_list.push_back(piece);
                        }
                        if (base_type::m_free_unit_list.empty())
                        {
                            break;
                        }
                    }
                    while (piece = tmp_list.pop_front(), piece)
                    {
                        base_type::m_available_piece_list.push_back(piece);
                    }

					PMC_ASSERT(base_type::m_free_unit_list.empty());

                    // empty list
                    while (piece = empty_list.pop_front(), piece != nullptr)
                    {
                        if (piece_count > 3)
                        {
                            delete piece;
                            piece_count--;
                        }
                        else
                        {
                            base_type::m_available_piece_list.push_back(piece);
                        }
                    }
                }

            public:                           
                compactable_memory_pool()
                {
                }                
 
                ::pilo::int_r piece_count_nolock() const
                {
                    return base_type::m_full_piece_list.size() + base_type::m_available_piece_list.size();
                }

                inline void compact()
                {
                    std::lock_guard<lock_type> locker(base_type::m_mutex);
                    _compact_nolock();
                }

                inline void deallocate(pointer ptr)
                {
                    std::lock_guard<lock_type> locker(base_type::m_mutex);

                    unit_node* unode = (unit_node*)ptr;

					PMC_ASSERT(unode->m_flag != PMB_MEM_POOL_UNIT_FREED_FLAG);
                    unode->m_flag = PMB_MEM_POOL_UNIT_FREED_FLAG;

                    base_type::m_free_unit_list.push_back(unode);

                    if (_need_compact_nolock())
                    {
                        _compact_nolock();
                    }
                }

                
            };
        }
    }
}