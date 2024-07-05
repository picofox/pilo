#ifndef __auto_array_h_
#define __auto_array_h_

#include "../../pilo.hpp"
#include <algorithm>

namespace pilo
{
    namespace core
    {
        namespace container
        {
            template<typename T, ::pilo::i32_t DFL_SZ>
            class auto_array
            {
            public:
                typedef T value_type;
                typedef T& reference;
                typedef const T& const_reference;
                typedef T* pointer;
                typedef const T* const_pointer;
                typedef ptrdiff_t difference_type;
                typedef ::pilo::i32_t size_type;


            public:
                auto_array() : _capacity(DFL_SZ), _size(0), _dyn_data_ptr(_fixed_data) {}
                ~auto_array() 
                {
                    if (_dyn_data_ptr != nullptr && _dyn_data_ptr != _fixed_data)
                    {
                        delete [] _dyn_data_ptr;
                        _dyn_data_ptr = _fixed_data;
                    }
                }

                //about capacity
                size_type size() const { return _size; }
                size_type capacity() const { return _capacity; }
                bool empty() const { return _size == 0;  }
                bool full() const { return _size >= _capacity;  }
                ::pilo::err_t reserve(::pilo::i32_t neo_size)
                {
                    if (neo_size > _size)
                    {
                        return _inflate(neo_size);
                    }
                    return PILO_OK;
                }
                ::pilo::err_t shrink_to_fit()
                {
                    if (_size < DFL_SZ) //may be in heap, if so, move data into stack kbuffer
                    {
                        if (!_is_fixed())
                        {
                            try
                            {
                                for (size_type i = 0; i < _size; i++)
                                {
                                    _fixed_data[i] = _dyn_data_ptr[i];
                                }
                                T* tmp_ptr = _dyn_data_ptr;
                                _dyn_data_ptr = _fixed_data;
                                delete [] tmp_ptr;
                            }
                            catch (...)
                            {
                                _dyn_data_ptr = _fixed_data;
                                return ::pilo::mk_perr(PERR_FILE_CREAET_FAIL);
                            }                          
                        }
                        //nothing to do with stack situation
                    }
                    else //can not hold all data in fixed data buffer
                    {
                        if (_size < _capacity) 
                        {
                            try
                            {
                                T* tmp_data_ptr = new (std::nothrow) T[_size];
                                if (tmp_data_ptr == nullptr)
                                {
                                    return ::pilo::mk_perr( PERR_INSUF_HEAP);
                                }
                                for (size_type i = 0; i < _size; i++)
                                {
                                    tmp_data_ptr[i] = _dyn_data_ptr[i];
                                }
                                T* tmp_ptr = _dyn_data_ptr;
                                _dyn_data_ptr = tmp_data_ptr;
                                _capacity = _size;
                                delete [] tmp_ptr;
                            }
                            catch (...)
                            {
                                return ::pilo::mk_perr(PERR_FILE_CREAET_FAIL);
                            }                            
                        }  
                        //else no need to adjust
                    }

                    return PILO_OK;
                }


                value_type& at(size_type idx)
                {
                    PMC_ASSERT(idx < _size);
                    return _dyn_data_ptr[idx];
                }
                const value_type& at(size_type idx) const
                {
                    PMC_ASSERT(idx < _size);
                    return _dyn_data_ptr[idx];
                }
                //element access
                value_type& operator[] (size_type idx)
                {
                    return this->at(idx);
                }
                const value_type& operator[] (size_type idx) const
                {
                    return this->at(idx);
                }

                reference front() { return _dyn_data_ptr[0]; }
                const_reference front() const { return _dyn_data_ptr[0]; }
                reference back() { return _dyn_data_ptr[_size]; }
                const_reference back() const { return _dyn_data_ptr[_size]; }
                value_type* data() { return _dyn_data_ptr; }
                const value_type* data() const { return _dyn_data_ptr; }



                ::pilo::i32_t push_back(const T& item)
                {
                    if (full())
                    {
                        ::pilo::err_t rc = _inflate(_size + 1);
                        if (rc != PILO_OK)
                        {
                            return rc;
                        }
                    }
                    _dyn_data_ptr[_size++] = item;
                    return PILO_OK;
                }

                void pop_back()
                {
                    if (! empty())
                    {
                        _size--;
                       // (&(_dyn_data_ptr[_size]))->~T();
                    }
                    
                }



            private:

                ::pilo::err_t _inflate(::pilo::i32_t neo_size)
                {
                    bool orig_in_stack = false;
                    if(_is_fixed())
                    {
                        orig_in_stack = true;
                    }

                    if (neo_size > _capacity)
                    {
                        T* tmp_data_ptr = new (std::nothrow) T[neo_size];
                        if (tmp_data_ptr == nullptr)
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }
                        _capacity = neo_size;

                        try
                        {
                            for (::pilo::i32_t i = 0; i < _size; i++)
                            {
                                tmp_data_ptr[i] = _dyn_data_ptr[i];
                            }
                        }
                        catch (...)
                        {
                            return ::pilo::EC_UNDEFINED;
                        }

                        if (orig_in_stack)
                        {
                            _dyn_data_ptr = tmp_data_ptr;
                        }
                        else
                        {
                            std::swap(tmp_data_ptr, _dyn_data_ptr);
                            delete [] tmp_data_ptr;
                        }

                    }

                    return PILO_OK;
                }

                bool _is_fixed() const
                {
                    return _dyn_data_ptr == _fixed_data;
                }

            private:
                ::pilo::i32_t    _capacity;
                ::pilo::i32_t    _size;
                T               _fixed_data[DFL_SZ];
                T*              _dyn_data_ptr;

            };

        }
    }
}

#endif