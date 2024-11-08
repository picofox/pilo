#ifndef _pilo_core_container_sorted_vector_hpp_
#define _pilo_core_container_sorted_vector_hpp_

#include	<deque>
#include	"../../pilo.hpp"

namespace pilo
{
	namespace core
	{
		namespace container
		{
			template <typename T, typename C = ::pilo::default_comparator<T, ::pilo::default_less_than_comparator<T>>>
			class sorted_deque
			{
			public:
				typedef T	value_type;
				typedef C   comparator_type;

			public:
				::pilo::i64_t bsearch(const value_type& val, bool& found) const
				{
					found = false;
					if (_m_vec.size() < 1) return 0;

					::pilo::i64_t lo = 0;
					::pilo::i64_t hi = _m_vec.size() - 1;
					register ::pilo::i64_t  num = _m_vec.size();
					::pilo::i64_t half = 0;
					::pilo::i64_t mid_idx = 0;
					int result = 0;
					while (lo <= hi) {						
						if ((half = num / 2) != 0)	{	// num >= 2
							mid_idx = lo + (num & 1 ? half : (half - 1));
							result = _m_cmp(val, _m_vec[mid_idx]);
							if (result < 0) {
								// val < mid
								hi = mid_idx - 1;
								num = num & 1 ? half : half - 1;
								if (lo > hi) {
									return mid_idx;
								}
							} else if (result > 0) {
								// val > mid
								lo = mid_idx + 1;
								num = half;
								if (lo > hi)
								{
									return mid_idx + 1;
								}
							}
							else {
								found = true;
								return mid_idx;
							}
						}
						else if (num)
						{	//only one
							result = _m_cmp(val, _m_vec[lo]);
							if (result < 0) {
								// val < mid
								return lo;
							}
							else if (result > 0) {
								// val > mid
								return lo + 1;
							}
							else {
								found = true;
								return lo;
							}
						}
						else
						{	//no
							return 0;
						}
					}

					return -1;
				}
				
				value_type& at(::pilo::i64_t idx)
				{
					return _m_vec.at(idx);
				}
				const value_type& at(::pilo::i64_t idx) const
				{
					return _m_vec.at(idx);
				}

				value_type& operator [] (::pilo::i64_t idx)
				{
					return _m_vec.at(idx);
				}
				const value_type& operator [] (::pilo::i64_t idx) const
				{
					return _m_vec.at(idx);
				}

				::pilo::i64_t insert(const value_type& val)
				{
					bool found;
					::pilo::i64_t pos = this->bsearch(val, found);
					_m_vec.insert(_m_vec.begin() + pos, val);
					return pos;
				}

				::pilo::i64_t insert_unique(const value_type& val)
				{
					bool found;
					int pos = this->bsearch(val, found);
					if (found) return -1;
					_m_vec.insert(pos, val);
					return pos;
				}

				::pilo::i64_t size() const
				{
					return _m_vec.size();
				}

				bool empty() const
				{
					return _m_vec.empty();
				}

				void pop_front()
				{
					_m_vec.pop_front();
				}

				const value_type& front() const {
					return _m_vec.front();
				}

				void pop_back()
				{
					_m_vec.pop_back();
				}

				const value_type& back() const {
					return _m_vec.back();
				}

				void clear()
				{
					_m_vec.clear();
				}

				::pilo::err_t earse(::pilo::i64_t idx)
				{
					if ((::pilo::i64_t)_m_vec.size() > idx) {
						_m_vec.erase(_m_vec.begin() + idx);
						return PILO_OK;
					}
					return ::pilo::mk_perr(PERR_NON_EXIST);
					
				}

			protected:
				std::deque<T>	_m_vec;
				comparator_type _m_cmp;
			};
		}
	}
}

#endif