#pragma once
#include "pilo/core/coredefs.hpp"
#include "dynamic_array.hpp"

namespace pilo
{
    namespace core
    {
        namespace container
        {


            template <
                typename T, 
                typename C = ref_gel_compare_less<T>, 
                size_t initCapacity = 32, 
                size_t inflateStep = 16>
            class sorted_dynamic_array : private dynamic_array<T, initCapacity, inflateStep>
            {
            public:
                typedef T value_type;
                typedef dynamic_array<T, initCapacity, inflateStep> base_type;
                typedef C cmp_type;

            public:
                cmp_type m_cmp;

            public:
                sorted_dynamic_array()
                {

                }
                sorted_dynamic_array(value_type* vals, size_t capacity) :base_type(vals, capacity, 0)
                {

                }
                sorted_dynamic_array(const sorted_dynamic_array& o) :base_type(o)
                {

                }
                ~sorted_dynamic_array()
                {

                }
                sorted_dynamic_array& operator = (const sorted_dynamic_array& o)
                {
                    if (this == &o) return *this;
                    base_type::operator = (o);
                    return *this;
                }
                sorted_dynamic_array& attach(value_type* vals, size_t capacity)
                {
                    base_type::attach(vals, capacity, 0);
                    return *this;
                }
                // 调用者必须保证已有的数据是排好序的
                sorted_dynamic_array& attach(value_type* vals, size_t capacity, size_t size)
                {
                    base_type::attach(vals, capacity, size);
                    return *this;
                }
                sorted_dynamic_array& detach()
                {
                    base_type::detach();
                    return *this;
                }
                size_t size() const
                {
                    return base_type::_m_size;
                }
                bool empty() const
                {
                    return base_type::_m_size < 1;
                }
                void clear()
                {
                    base_type::clear();
                }
                value_type& at(size_t idx)
                {
                    return base_type::at(idx);
                }
                const value_type& at(size_t idx) const
                {
                    return base_type::at(idx);
                }
                value_type& operator [] (size_t idx)
                {
                    return base_type::at(idx);
                }
                const value_type& operator [] (size_t idx) const
                {
                    return base_type::at(idx);
                }
                value_type& get(size_t idx)
                {
                    return base_type::get(idx);
                }
                const value_type& get(size_t idx) const
                {
                    return base_type::get(idx);
                }
                size_t find(const value_type& val) const
                {
                    bool found;
                    size_t pos = this->bsearch(val, found);
                    if (!found) return MC_INVALID_SIZE;
                    return pos;
                }
                size_t insert(const value_type& val)
                {
                    bool found;
                    size_t pos = this->bsearch(val, found);
                    base_type::insert(pos, val);
                    return pos;
                }
                size_t insert_unique(const value_type& val)
                {
                    bool found;
                    size_t pos = this->bsearch(val, found);
                    if (found) return -1;
                    base_type::insert(pos, val);
                    return pos;
                }
                value_type pop_back()
                {
                    return base_type::pop_back();
                }
                value_type& back()
                {
                    return base_type::back();
                }
                const value_type& back() const
                {
                    return base_type::back();
                }
                value_type& front()
                {
                    return base_type::front();
                }
                const value_type& front() const
                {
                    return base_type::front();
                }
                sorted_dynamic_array& erase(size_t idx)
                {
                    base_type::erase(idx);
                    return *this;
                }
                sorted_dynamic_array& erase_by_index(size_t idx)
                {
                    base_type::erase(idx);
                    return *this;
                }
                bool erase(const value_type& val)
                {
                    size_t pos = this->find(val);
                    if (pos < 0) return false;
                    this->erase(pos);
                    return true;
                }
                bool erase_by_value(const value_type& val)
                {
                    size_t pos = this->find(val);
                    if (pos < 0) return false;
                    this->erase_by_index(pos);
                    return true;
                }

                /**
                * 实现对半查找，返回值是元素应当插入的位置
                */
                size_t bsearch(const value_type& val, bool& found) const
                {
                    found = false;
                    if (base_type::_m_size < 1) return 0;

                    char* orig = (char*)base_type::_m_vals;
                    register char* lo = orig;
                    register char* hi = lo + (base_type::_m_size - 1) * sizeof(value_type);

                    register size_t num = base_type::_m_size;
                    register size_t unitlen = sizeof(value_type);

                    char* mid;
                    size_t half;
                    ::pilo::i32_t result;

                    while (lo <= hi)
                    {
                        if ((half = num / 2) != 0)
                        {	// num >= 2
                            mid = lo + (num & 1 ? half : (half - 1)) * unitlen;
                            if (0 == (result = m_cmp(val, *(const value_type*)mid)))
                            {
                                found = true;
                                return (size_t)((mid - orig) / unitlen);
                            }
                            else if (result < 0)
                            {	// val < mid
                                hi = mid - unitlen;
                                num = num & 1 ? half : half - 1;

                                if (lo > hi)
                                {
                                    return (size_t)((mid - orig) / unitlen);
                                }
                            }
                            else
                            {	// val > mid
                                lo = mid + unitlen;
                                num = half;

                                if (lo > hi)
                                {
                                    return (size_t)((mid - orig) / unitlen + 1);
                                }
                            }
                        }
                        else if (num)
                        {	//只有一个
                            ::pilo::i32_t n = m_cmp(val, *(const value_type*)lo);
                            if (!n)
                            {
                                found = true;
                                return (size_t)((lo - orig) / unitlen);
                            }
                            else if (n < 0)
                            {
                                return (size_t)((lo - orig) / unitlen);
                            }
                            else
                            {
                                return (size_t)((lo - orig) / unitlen + 1);
                            }
                        }
                        else
                        {	//没有
                            return (size_t)((lo - orig) / unitlen);
                        }
                    }
                    //不可达
                    return MC_INVALID_SIZE;
                }
            };

        }
    }
}