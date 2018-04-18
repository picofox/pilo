#include "memory.hpp"

#ifdef _DEBUG_MEM_OBJ_POOL
#include <string>
#endif

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <size_t _PieceSize, size_t _Capacity>
            struct fixed_memory_piece
            {
                typedef void*                                                       pointer;
                typedef const void*                                                 const_pointer;

                struct unit_type //every single piece
                {
                    char m_data[M_MM_POOL_ALIGN_USIZE(_PieceSize, 2)];
                };

                struct unit_node : public unit_type
                {
                    unit_node*  m_next;
                };

                size_t              m_available;        //can use directly
                size_t              m_free_unit_num;    //size of the list
                unit_node*          m_free_node_list;   //has been used once at least                
                unit_node           m_units[_Capacity]; //data storage

                ///
                fixed_memory_piece()
                    : m_available(_Capacity)
                    , m_free_unit_num(0)
                    , m_free_node_list(nullptr)                    
                {

                }

                inline void clear()
                {
                    m_available = _Capacity;
                    m_free_node_list = nullptr;
                    m_free_unit_num = 0;
                }

                inline bool has(pointer ptr) const //if the address belong to my section
                {
                    return ((ptr >= (pointer)&m_units[0]) && (ptr < (pointer)&m_units[_Capacity]));
                }

                inline pointer alloc_unit()
                {
                    if (nullptr != m_free_node_list) //check free list first
                    {
                        unit_node* unode = m_free_node_list;
                        m_free_node_list = unode->m_next;
                        m_free_unit_num--;

                        return unode;

                    }
                    if (0 == m_available) return nullptr;
                    return (pointer) (&m_units[--m_available]); //still has new unit which has not been allocated
                }

                inline bool dealloc_unit(pointer ptr)
                {   
                    if (!this->has(ptr)) 
                    {
                        return false; //this ptr is not my boy.
                    }

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
            };

            template <size_t _UnitSize, size_t _Capacity>
            struct fixed_memory_piece_node
                : public fixed_memory_piece<_UnitSize, _Capacity>
            {
                typedef fixed_memory_piece<_UnitSize, _Capacity>     base_type;
                typedef typename base_type::unit_node                unit_node;
                typedef typename base_type::unit_type                unit_type;
                
                fixed_memory_piece_node* m_next;
            };

        }
    }
}