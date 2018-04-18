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
            template <size_t _PieceSize, size_t _Capacity> 
            struct fixed_overlapped_memory_piece
            {
                typedef void*                                                       pointer;
                typedef const void*                                                 const_pointer;

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
                std::string make_report(::pilo::u32_t flag = 0, size_t level = 0)
                {
                    std::string str;
                    str.clear();
                    ::pilo::core::string::fixed_astring<128> tmp_str;
                    std::string tab_str;
                    tab_str.clear();
                    for (int i = 0; i < level; i++)
                    {
                        tab_str += "    ";
                    }

                    tmp_str.format("%sFxOLMem Piece (US=%u CA=%u) @ 0x%p:\n", tab_str.c_str(),
                        (unsigned int)_PieceSize, (unsigned int)_Capacity, this);
                    str += tmp_str.c_str();
                    tmp_str.format("%s    Free Unit Count:%u\n", tab_str.c_str(), (unsigned int) m_free_unit_num);
                    str += tmp_str.c_str();
                    size_t i = 0;
                    if (flag & MB_DEBUG_POOL_SHOW_FREELIST_DETAIL)
                    {
                        unit_node* node = m_free_node_list;
                        for (; node != nullptr; node = node->m_next)
                        {
                            tmp_str.format("%s        Free unit_%u\t: @ 0x%p\n", tab_str.c_str(), (unsigned int)i++, node);
                            str += tmp_str.c_str();
                        }
                    }

                    tmp_str.format("%s    Aval Unit Count:%u\n", tab_str.c_str(), (unsigned int) m_available);
                    str += tmp_str.c_str();
                    if (m_available > 0)
                    {
                        tmp_str.format("%s        Avail 0x%p - 0x%p\n", tab_str.c_str(), &(m_units[0]), &(m_units[m_available - 1]));                        
                    }
                    else
                    {
                        tmp_str.format("%s        No Available Unit\n", tab_str.c_str());
                    }
                    str += tmp_str.c_str();

                    if (empty())
                    {
                        tmp_str.format("%s        No In Used Unit\n", tab_str.c_str());
                    }
                    else
                    {
                        tmp_str.format("%s        In Used 0x%p - 0x%p\n", tab_str.c_str(), &(m_units[m_available]), &(m_units[_Capacity - 1]));
                    }
                    str += tmp_str.c_str();

                    if (flag & MB_DEBUG_POOL_SHOW_UNIT_IMAGE)
                    {
                        for (size_t ind = m_available; ind < _Capacity; ind++)
                        {
                            size_t  tmp_len = _PieceSize * 3 + 1;
                            char* buffer = (char*)calloc(tmp_len, 1);
                            tmp_str.format("%s        Unit Array_%u\t: @ 0x%p\n", tab_str.c_str(), (unsigned int)ind, &(m_units[ind]));
                            str += tmp_str.c_str();
                            ::pilo::core::string::string_util::binary_to_cstr(buffer, tmp_len, &m_units[ind], _PieceSize, 24);
                            str += buffer;
                            str += "\n";
                        }
                    }


                    return str;

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