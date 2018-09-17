#include "memory.hpp"

#ifdef _DEBUG_MEM_OBJ_POOL
#include <string>
#include "core/string/string_util.hpp"
#endif

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <size_t _UnitSize, size_t _Capacity> 
            struct fixed_overlapped_memory_piece
            {
                typedef void*                                                       pointer;
                typedef const void*                                                 const_pointer;

                struct unit_type //every single piece
                { 
                    char m_data[M_MM_POOL_ALIGN_USIZE(_UnitSize,2)]; 
                };
                struct unit_node 
                { 
                    uint_r      m_flag; 
                    unit_node*  m_next; 
                };

                size_t              m_available;        //can use directly
                unit_node*          m_free_node_list;   //has been used once at least
                size_t              m_free_unit_num;    //size of the list
                unit_type           m_units[_Capacity]; //data storage

                ///
                fixed_overlapped_memory_piece()
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

                inline bool has(pointer ptr) const //if the address belong to my section
                {
                    unit_type* u = (unit_type*)ptr;
                    return ((u >= &m_units[0]) && (u < &m_units[_Capacity]));
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
                    return &m_units[--m_available]; //still has new unit which has not been allocated
                }

                inline bool dealloc_unit(pointer ptr)
                {
                    if (!this->has(ptr)) return false; //this ptr is not my boy.

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

#ifdef _DEBUG_MEM_OBJ_POOL
            public:
                void make_summary_report(unsigned int ind, char* buffer, size_t sz)
                {
                    ::pilo::core::io::string_format_output(buffer, sz, "    #%u FOMP@%p: FreeNode=%u AvailNode=%u\n", ind,
                        this, (unsigned int) m_free_unit_num, (unsigned int) m_available );
                    return;
                }
#endif
            };

            template <size_t _UnitSize, size_t _Capacity>
            struct fixed_overlapped_memory_piece_node
                : public fixed_overlapped_memory_piece<_UnitSize, _Capacity>
            {
                typedef fixed_overlapped_memory_piece<_UnitSize, _Capacity>     base_type;
                typedef typename base_type::unit_node                           unit_node;

                fixed_overlapped_memory_piece_node* m_next;
            };
        }
    }
}