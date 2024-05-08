#pragma once

#include "../../pilo.hpp"


#include <string>
#include <sstream>
#include "../io/formatted_io.hpp"


namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <::pilo::i64_t _UnitSize, ::pilo::i64_t _Capacity> 
            struct fixed_memory_piece
            {
                struct unit_type //every single piece
                { 
                    char m_data[PMF_MPOOL_ALIGN_SIZE(_UnitSize,2)];
                };
                struct unit_node 
                { 
                    uint_r      m_flag; 
                    unit_node*  m_next; 
                };

                ::pilo::int_r				m_available;        //can use directly
                unit_node*					m_free_node_list;   //has been used once at least
				::pilo::int_r				m_free_unit_num;    //size of the list
                unit_type					m_units[_Capacity]; //data storage

                ///
                fixed_memory_piece()
                    : m_available(_Capacity)
                    , m_free_node_list(nullptr)
                    , m_free_unit_num(0)
                {

                }

                inline void clear()
                {
                    m_available = _Capacity;
                    m_free_node_list = nullptr;
                    m_free_unit_num = 0;
                }

                inline bool has(::pilo::pointer ptr) const //if the address belong to my section
                {
                    unit_type* u = (unit_type*)ptr;
                    return ((u >= &m_units[0]) && (u < &m_units[_Capacity]));
                }

                inline ::pilo::pointer alloc_unit()
                {
                    if (nullptr != m_free_node_list) //check free list first
                    {
                        unit_node* unode = m_free_node_list; //get unit node from free list if list is not null
                        m_free_node_list = unode->m_next; //detach the node which user want.
                        m_free_unit_num--;
                        return unode;
                    }
                    if (0 == m_available) return nullptr; //all memory out :(
                    return &m_units[--m_available]; //still has new unit which has not been allocated, find space in one time cache
                }

                inline bool dealloc_unit(::pilo::pointer ptr)
                {
                    if (!this->has(ptr)) return false; //this unit is not my boy.

                    unit_node* unode = (unit_node*)ptr; //force to unit_node to save space
                    unode->m_next = m_free_node_list;
                    m_free_node_list = unode;
                    m_free_unit_num++;
                    return true;
                }

                inline bool available() const
                {
                    return ((m_free_node_list != nullptr) || (m_available > 0));
                }

                inline bool empty() const
                {
                    return ((m_free_unit_num + m_available) >= _Capacity);
                }

				inline ::pilo::int_r available_count() const
				{
					return m_free_unit_num + m_available;
				}


            public:
                std::string to_string() const
                {
                    std::stringstream ss;
                    ss << "f:" << m_free_unit_num << ", a:" << m_available << ", us:" << available_count() << "/" << _Capacity;
                    return ss.str();
                } 
            };

            template <::pilo::i64_t _UnitSize, ::pilo::i64_t _Capacity>
            struct fixed_overlapped_memory_piece_node
                : public fixed_memory_piece<_UnitSize, _Capacity>
            {
                typedef fixed_memory_piece<_UnitSize, _Capacity>     base_type;
                typedef typename base_type::unit_node                           unit_node;

                fixed_overlapped_memory_piece_node() : m_next(nullptr) {}

                fixed_overlapped_memory_piece_node* m_next;

                std::string to_string() const
                {
                    std::stringstream ss;
                    char pb[24] = { 0 };
                    char pbn[24] = { 0 };
                    ::pilo::core::io::string_formated_output(pb, 24, "%p", this);
                    ::pilo::core::io::string_formated_output(pbn, 24, "%p", this->m_next);
                    ss << "[" << pb << "->" << pbn << "]:";
                    ss << "f:" << this->m_free_unit_num << ", a:" << this->m_available << ", us:" << this->available_count() << "/" << _Capacity;
                    return ss.str();
                }

            };
        }
    }
}