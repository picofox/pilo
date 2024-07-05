#pragma once

#include <mutex>
#include "../../pilo.hpp"
#include "../threading/dummy_mutex.hpp" //[c+1]
#include "../container/singly_linked_selflist.hpp"
#include "dynamic_memory_piece.hpp"
#include "memory_pool_allocator_interface.hpp"

#ifdef _DEBUG_MEM_OBJ_POOL
#include "../io/formatted_io.hpp"
#endif

namespace pilo
{
    namespace core
    {
        namespace memory
        {
			struct dynamic_memory_pool_statistics
			{
                dynamic_memory_pool_statistics()
					: m_unit_size(0)
					, m_total_units(0)
					, m_available_units(0)

                    , m_total_pcs(0)
				{
				}
				::pilo::i64_t m_unit_size;
				::pilo::i64_t m_total_units;
				::pilo::i64_t m_available_units;

                ::pilo::i64_t m_total_pcs;

				const char* to_cstr(char* buffer, ::pilo::i64_t sz) const
				{
                    ::pilo::core::io::string_formated_output(buffer, sz, "pool_%04llu: { total:{bs:%llu, cnt:%llu, pcs:%llu}, avail:{bs:%llu, cnt:%llu}, ap:%.2lf%% }"
                        , m_unit_size
                        , m_unit_size * m_total_units
                        , m_total_units
                        , m_total_pcs
                        , m_available_units * m_unit_size
                        , m_available_units
                        , m_total_units == 0 ? 0.f : (double)m_available_units / (double)m_total_units * 100.f
                    );

					return buffer;
				}

				std::string to_str() const
				{
					char buffer[256] = { 0 };
					to_cstr(buffer, 256);
					return std::string(buffer);
				}

				void append_str(std::string& str) const
				{
					char buffer[256] = { 0 };
					to_cstr(buffer, 256);

					str += buffer;
					str.push_back('\n');
				}
			};

            template <typename _Lock = pilo::core::threading::dummy_mutex>
                class dynamic_memory_pool : public memory_pool_allocator_interface
            {
            public:
                typedef _Lock                                                       lock_type;
                typedef dynamic_overlapped_memory_piece_node                        piece_type;
                typedef typename piece_type::unit_node                              unit_node;
                typedef pilo::core::container::singly_linked_selflist<piece_type>   piece_list;
                typedef pilo::core::container::singly_linked_selflist<unit_node>    unit_list;
                typedef ::pilo::i64_t                                                size_type;

            public:                
                piece_list          m_full_piece_list;
                piece_list          m_available_piece_list;
                unit_list           m_free_unit_list;
                const size_type     m_unit_size;
                const size_type     m_steps;
                mutable lock_type   m_mutex;


            public:
                dynamic_memory_pool() : m_unit_size(0), m_steps(0)
                {
                }

                dynamic_memory_pool(size_type unit_size, size_type n_steps) : m_unit_size(unit_size), m_steps(n_steps)
                {
                    //printf("   innner usize= %lld, n_steps=%lld\n", unit_size, n_steps);
                }

                virtual ~dynamic_memory_pool()
                {
                    this->reset();
                }

				inline const dynamic_memory_pool_statistics& get_statistics(dynamic_memory_pool_statistics& st)
				{
                    std::lock_guard<lock_type> locker(m_mutex);

					st.m_unit_size = this->m_unit_size;
                    st.m_total_units = (m_full_piece_list.size() + m_available_piece_list.size()) * m_steps;


                    st.m_available_units = m_free_unit_list.size();
                    piece_type* piece = m_available_piece_list.begin();
					for (; piece != nullptr; piece = m_available_piece_list.next(piece))
					{
                        st.m_available_units += piece->available_count();
					}

                    st.m_total_pcs = m_available_piece_list.size() + m_full_piece_list.size();
                    return st;
				}

                ::pilo::i32_t init(size_type unit_size, size_type n_steps)
                {
                    if (m_unit_size == 0 && m_steps == 0)
                    {
                        *( (size_type*) (&(this->m_unit_size)) ) = unit_size;
                        *( (size_type*) (&(this->m_steps)) ) = n_steps;
                        return PILO_OK;
                    }
                    else
                    {
                        return ::pilo::mk_perr(PERR_NOOP);
                    }
                }                

                virtual ::pilo::i64_t unit_size()
                {
                    return m_unit_size;
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
                    return this->m_unit_size;
                }

                virtual ::pilo::pointer allocate()
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
                        piece = new piece_type(m_unit_size, m_steps);
                        m_available_piece_list.push_back(piece);
                    }

                    ptr = (unit_node*)piece->alloc_unit();
                    if (!piece->available())
                    {
                        m_available_piece_list.pop_front();
                        m_full_piece_list.push_back(piece);
                    }

                    ptr->m_flag = 0;
                    return ptr;
                }

                virtual void deallocate(::pilo::pointer ptr)
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
                    std::lock_guard<lock_type> locker(m_mutex);
                    unsigned int ind = 0;
                    char buffer[256];
                    ::pilo::core::io::string_formated_output(buffer, sizeof(buffer),
                        "(OMP@%p) FreeNode=%lld FullList=%lld AvalList=%lld\n", this,
                        (::pilo::i64_t) m_free_unit_list.size(),
                        (::pilo::i64_t) m_full_piece_list.size(),
                        (::pilo::i64_t)m_available_piece_list.size());
                    str += buffer;

                    str += "  FullPieceList:\n";
                    piece_type* piece = m_full_piece_list.begin();
                    for (; piece != m_full_piece_list.end(); piece = m_full_piece_list.next(piece))
                    {
                        piece->make_summary_report(ind++, buffer, 256);
                        str += buffer;
                    }

                    ind = 0;
                    str += "  AvalPieceList:\n";
                    piece = m_available_piece_list.begin();
                    for (; piece != m_available_piece_list.end(); piece = m_available_piece_list.next(piece))
                    {
                        piece->make_summary_report(ind++, buffer, 256);
                        str += buffer;
                    }

                    return str;
                }
#endif
            };
        }
    }
}
