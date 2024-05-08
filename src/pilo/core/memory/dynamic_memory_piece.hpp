#pragma once

#include "../../pilo.hpp" //[c+1]
#include "../io/formatted_io.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            struct dynamic_memory_piece_statistics
            {
                dynamic_memory_piece_statistics()
                    : m_unit_size(0)
                    , m_total_units(0)
                    , m_total_size(0)
                    , m_available_units(0)
                    , m_available_size(0)
                {
                }
                ::pilo::i64_t m_unit_size;
                ::pilo::i64_t m_total_units;
                ::pilo::i64_t m_total_size;
                ::pilo::i64_t m_available_units;
                ::pilo::i64_t m_available_size;

                const char* to_cstr(char* buffer, ::pilo::i64_t sz) const
                {
                    ::pilo::core::io::string_formated_output(buffer, sz, "Pcs_%llu -> Avail BS=%.4lf%% (%llu/%llu) Avail Cnt=%.4lf%% (%llu/%llu)"
                        , m_unit_size
                        , (double)m_available_size / (double)m_total_size * 100.f
                        , m_available_size 
                        , m_total_size
                        , (double)m_available_units / (double)m_total_units * 100.f
                        , m_available_units
                        , m_total_units
                    );
                    return buffer;
                }

                std::string to_str() const
                {
                    char buffer[256] = { 0 };
                    to_cstr(buffer, 256);
                    return std::string(buffer);
                }
            };

            struct dynamic_memory_piece
            {
                struct unit_node
                {
                    uint_r      m_flag;
                    unit_node*  m_next;
                };

                ::pilo::int_r		m_available;        //can use directly                
                ::pilo::int_r	    m_unit_count;
                unit_node*          m_free_unit_list;   //has been used once at least
                ::pilo::int_r       m_free_unit_num;    //size of the list
                char*               m_data;		        //data storage
                ::pilo::i64_t        m_unit_size;
                ::pilo::i64_t        m_capa;


                ///
                dynamic_memory_piece()
                    : m_available(0)
                    , m_unit_count(0)
                    , m_free_unit_list(nullptr)
                    , m_free_unit_num(0)
                    , m_data(nullptr)
                    , m_unit_size(0)
                    , m_capa(0)
                {

                }

                dynamic_memory_piece(::pilo::i64_t n_unit_size, ::pilo::int_r n)
                {                    
                    m_data = nullptr;
                    initialize(n_unit_size, n);
                }

                ~dynamic_memory_piece()
                {
                    if (m_data != nullptr)
                    {                        
                        PMF_HEAP_FREE(m_data);
                        m_data = nullptr;
                    }
                }

                inline void get_statistics(dynamic_memory_piece_statistics& st)
                {
                    st.m_unit_size = this->m_unit_size;
                    st.m_total_units = this->m_unit_count;
                    st.m_total_size = this->m_capa;
                    st.m_available_units = this->m_available + this->m_free_unit_num;
                    st.m_available_size = st.m_available_units * st.m_unit_size;
                }

                inline bool initialize(::pilo::i64_t n_unit_size, ::pilo::int_r n)
                {
                    PMC_ASSERT(m_data == nullptr)
                    m_data = (char*)PMF_HEAP_CALLOC(n, n_unit_size);
                    if (m_data == nullptr)
                        return false;
                    m_unit_count = n;
                    m_available = m_unit_count;
                    m_free_unit_list = nullptr;
                    m_free_unit_num = 0;
                    m_unit_size = n_unit_size;
                    m_capa = m_unit_size * m_unit_count;
                    return true;
                }


                inline void clear()
                {
                    m_available = m_unit_count;
                    m_free_unit_list = nullptr;
                    m_free_unit_num = 0;
                }

                inline bool has(void* ptr)
                {
                    char* u = (char*)ptr;
                    return ((u >= m_data) && (u < m_data + m_capa));
                }

                inline ::pilo::pointer alloc_unit()
                {
                    if (m_free_unit_list != nullptr)
                    {
                        unit_node* unode = m_free_unit_list;
                        m_free_unit_list = unode->m_next;
                        m_free_unit_num--;
                        return unode;
                    }
                    if (0 == m_available) return nullptr;
                    return &m_data[--m_available * m_unit_size];
                }

                inline bool dealloc_unit(::pilo::pointer ptr)
                {
                    if (!this->has(ptr)) return false; //this ptr is not my boy.

                    unit_node* unode = (unit_node*)ptr; //force to unit_node to save space
                    unode->m_next = m_free_unit_list;
                    m_free_unit_list = unode;
                    m_free_unit_num++;

                    if (m_free_unit_num + m_available >= m_unit_count)
                    {
                        m_available = m_unit_count;
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
                    return ((m_free_unit_num + m_available) >= m_unit_count);
                }

				inline ::pilo::int_r available_count() const
				{
					return m_free_unit_num + m_available;
				}
 
            };


            struct dynamic_overlapped_memory_piece_node
                : public dynamic_memory_piece
            {
                dynamic_overlapped_memory_piece_node(::pilo::i64_t n_unit_size, ::pilo::int_r n) : dynamic_memory_piece(n_unit_size, n)
                {
                    m_next = nullptr;
                }

                typedef dynamic_memory_piece                                    base_type;
                typedef typename base_type::unit_node                           unit_node;

                dynamic_overlapped_memory_piece_node* m_next;
            };
        }
    }
}