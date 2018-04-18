#include "memory.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template <size_t _UnitSize>
            struct dynamic_overlapped_memory_piece
            {
                struct unit_type //every single piece
                {
                    char m_data[M_MM_POOL_ALIGN_USIZE(_PieceSize,2)];
                };
                struct unit_node
                {
                    uint_r      m_flag;
                    unit_node*  m_next;
                };

                size_t              m_available;        //can use directly
                size_t              m_capacity;
                unit_node*          m_free_unit_list;   //has been used once at least
                size_t              m_free_unit_num;    //size of the list
                unit_type*          m_units_ptr; //data storage

                ///
                dynamic_overlapped_memory_piece()
                    : m_available(0)
                    , m_capacity(0)
                    , m_units_ptr(nullptr)
                    , m_free_unit_list(nullptr)
                    , m_free_unit_num(0)
                {

                }

                dynamic_overlapped_memory_piece(size_t n)
                {
                    init(n);
                }

                ~dynamic_overlapped_memory_piece()
                {
                    if (m_units_ptr != nullptr)
                    {
                        free(m_units_ptr);
                        m_units_ptr = nullptr;
                    }
                }

                inline bool init(size_t n)
                {
                    m_units_ptr = (unit_type*)malloc(n);
                    if (m_units_ptr == nullptr)
                    {
                        return false;
                    }
                    m_capacity = n;
                    m_available = m_capacity;
                    m_free_unit_list = nullptr;
                    m_free_unit_num = 0;
                    return false;
                }

                inline void clear()
                {
                    m_available = m_capacity;
                    m_free_unit_list = nullptr;
                    m_free_unit_num = 0;
                }

                inline has(void* ptr)
                {
                    unit_type* u = (unit_type*)ptr;
                    return ((u >= m_units_ptr) && (u < m_units_ptr + m_capacity));
                }

                inline void* alloc_unit()
                {
                    if (m_free_unit_list != nullptr)
                    {
                        unit_node* unode = m_free_unit_list;
                        m_free_unit_list = unode->m_next;
                        m_free_unit_num--;
                        return unode;
                    }
                    if (0 == m_available) return nullptr;
                    return &m_units_ptr[--avail];
                }

                inline bool dealloc_unit(void* ptr)
                {
                    if (!this->has(ptr)) return false; //this ptr is not my boy.

                    unit_node* unode = (unit_node*)ptr; //force to unit_node to save space
                    unode->m_next = m_free_unit_list;
                    m_free_unit_list = unode;
                    m_free_unit_num++;

                    if (m_free_unit_num + m_available >= m_capacity)
                    {
                        m_available = m_capacity;
                        m_free_unit_list = nullptr;
                        m_free_unit_num = 0;
                    }
                    return true;
                }

                inline bool available() const
                {
                    return ((m_free_unit_list != nullptr) || (m_available > 0));
                }

                inline bool empty() const
                {
                    return ((m_free_unit_num + m_available) >= _Capacity);
                }
 
            };

            template <size_t _UnitSize, size_t _Capacity>
            struct dynamic_memory_piece_node
                : public dynamic_overlapped_memory_piece<_UnitSize>
            {
                dynamic_memory_piece_node()
                {
                    this->init(_Capacity);
                }
                typedef dynamic_overlapped_memory_piece<_UnitSize>             base_type;
                typedef typename base_type::unit_node               unit_node;

                dynamic_memory_piece_node*                          m_next;
            };
        }
    }
}