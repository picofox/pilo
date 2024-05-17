#pragma once

#include <memory>

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template<typename T>
            void clear_raw_ptr_linear_container(T& cont, bool purge)
            {
                for (typename T::iterator it = cont.begin(); it != cont.end(); ++it)
                {
                    delete* it;
                }
                cont.clear();
                if (purge)
                {
                    cont.shrink_to_fit();
                }
            }

            template<typename TA_ELEMOBJ, ::pilo::i32_t TV_SIZE, typename T = typename _pilo_type_trait<TA_ELEMOBJ>::is_basetype>
            class object_array
            {
                object_array()
                {
                    PMC_ASSERT(false);
                }
            };


            template<typename TA_ELEMOBJ, ::pilo::i32_t TV_SIZE>
            class object_array<TA_ELEMOBJ, TV_SIZE, _pilo_falsetype>
            {
            public:
                object_array() : _dynamic(nullptr), _dynamic_capacity(-1), _data_size(0), _fixed{}
                {
                    
                }
                ~object_array()
                {
                    destory();
                }

                void destory()
                {
                    if (_dynamic != nullptr)
                    {
                        delete[] _dynamic;                        
                    }
                    _dynamic = nullptr;
                    _dynamic_capacity = -1;
                    _data_size = 0;
                }

                ::pilo::i32_t check_space(::pilo::i32_t neosz)
                {
                    if (_dynamic == nullptr)
                    {
                        if (neosz > TV_SIZE)
                        {
                            _dynamic = new TA_ELEMOBJ[neosz];
                            for (::pilo::i32_t i = 0; i < TV_SIZE; i++)
                            {
                                _dynamic[i] = _fixed[i];
                            }
                            _dynamic_capacity = neosz;
                            return 1;
                        }
                    }
                    else
                    {
                        if (neosz > _dynamic_capacity)
                        {
                            TA_ELEMOBJ * p = new TA_ELEMOBJ[neosz];
                            for (::pilo::i32_t i = 0; i < TV_SIZE; i++)
                            {
                                p[i] = _dynamic[i];
                            }
                            _dynamic_capacity = neosz;
                            delete _dynamic;
                            _dynamic = p;
                            return 1;
                        }
                    }
                    return 0;
                }

                TA_ELEMOBJ* begin()
                {
                    if (_dynamic == nullptr)
                    {
                        return _fixed;
                    }
                    else
                    {
                        return _dynamic;
                    }
                }

                TA_ELEMOBJ* ptr()
                {
                    if (_dynamic == nullptr)
                    {
                        return _fixed + _data_size;
                    }
                    else
                    {
                        return _dynamic + _data_size;
                    }
                }

                TA_ELEMOBJ* ptr(::pilo::i32_t idx)
                {
                    if (_dynamic == nullptr)
                    {
                        return &(_fixed[idx]);
                    }
                    else
                    {
                        return &(_dynamic[idx]);
                    }
                }

                const TA_ELEMOBJ & at(::pilo::i32_t idx) const
                {
                    if (_dynamic == nullptr)
                    {
                        return _fixed[idx];
                    }
                    else
                    {
                        return _dynamic[idx];
                    }
                }

                TA_ELEMOBJ& at(::pilo::i32_t idx)
                {
                    if (_dynamic == nullptr)
                    {
                        return _fixed[idx];
                    }
                    else
                    {
                        return _dynamic[idx];
                    }
                }


                ::pilo::i32_t capacity() const
                {
                    if (_dynamic == nullptr)
                    {
                        return TV_SIZE;
                    }
                    else
                    {
                        return _dynamic_capacity;
                    }
                }

                ::pilo::i32_t size() const
                {
                    return _data_size;
                }

                ::pilo::i32_t space_available() const
                {
                    return capacity() - _data_size;
                }

                ::pilo::err_t set_size(::pilo::i32_t sz)
                {
                    if (sz < 0 || sz > this->capacity())
                    {
                        PMC_ASSERT(false);
                        return ::pilo::make_core_error(PES_ELEM, PEP_ARR_IDX_OOB);
                    }
                    _data_size = sz;
                    return PILO_OK;
                }

                ::pilo::err_t add_size(::pilo::i32_t sz)
                {
                    ::pilo::i32_t ret = _data_size + sz;
                    if (ret < 0 || ret > _capacity)
                    {
                        PMC_ASSERT(false);
                        return ::pilo::make_core_error(PES_ELEM, PEP_ARR_IDX_OOB);
                    }

                    _data_size += sz;
                    return PILO_OK;
                }

                ::pilo::i32_t& ref_size()
                {
                    return _data_size;
                }

            private:
                TA_ELEMOBJ* _dynamic;
                ::pilo::i32_t _dynamic_capacity;
                ::pilo::i32_t _data_size;                
                TA_ELEMOBJ _fixed[TV_SIZE];
            };
            template<typename TA_ELEMOBJ, ::pilo::i32_t TV_SIZE>
            class object_array<TA_ELEMOBJ, TV_SIZE, _pilo_truetype>
            {
            public:
                object_array() : _dynamic(nullptr), _dynamic_capacity(-1), _data_size(0), _fixed{ 0 }
                {
                }
                ~object_array()
                {
                    destory();
                }

                void destory()
                {
                    if (_dynamic != nullptr)
                    {
                        PMF_HEAP_FREE(_dynamic);
                    }
                    _dynamic = nullptr;
                    _dynamic_capacity = -1;
                    _data_size = 0;
                }

                ::pilo::i32_t check_space(::pilo::i32_t neosz)
                {
                    if (_dynamic == nullptr)
                    {
                        if (neosz > TV_SIZE)
                        {
                            _dynamic = (TA_ELEMOBJ*) PMF_HEAP_MALLOC(neosz);
                            for (::pilo::i32_t i = 0; i < TV_SIZE; i++)
                            {
                                _dynamic[i] = _fixed[i];
                            }
                            _dynamic_capacity = neosz;
                            return 1;
                        }
                    }
                    else
                    {
                        if (neosz > _dynamic_capacity)
                        {
                            TA_ELEMOBJ* p = (TA_ELEMOBJ*) PMF_HEAP_MALLOC(neosz*sizeof(TA_ELEMOBJ));
                            for (::pilo::i32_t i = 0; i < TV_SIZE; i++)
                            {
                                p[i] = _dynamic[i];
                            }
                            _dynamic_capacity = neosz;
                            PMF_HEAP_FREE(_dynamic);
                            _dynamic = p;
                            return 1;
                        }
                    }
                    return 0;
                }

                TA_ELEMOBJ* begin()
                {
                    if (_dynamic == nullptr)
                    {
                        return _fixed;
                    }
                    else
                    {
                        return _dynamic;
                    }
                }

                TA_ELEMOBJ* ptr()
                {
                    if (_dynamic == nullptr)
                    {
                        return _fixed + _data_size;
                    }
                    else
                    {
                        return _dynamic + _data_size;
                    }
                }

                TA_ELEMOBJ* ptr(::pilo::i32_t idx)
                {
                    if (_dynamic == nullptr)
                    {
                        return &(_fixed[idx]);
                    }
                    else
                    {
                        return &(_dynamic[idx]);
                    }
                }

                const TA_ELEMOBJ& at(::pilo::i32_t idx) const
                {
                    if (_dynamic == nullptr)
                    {
                        return _fixed[idx];
                    }
                    else
                    {
                        return _dynamic[idx];
                    }
                }

                TA_ELEMOBJ& at(::pilo::i32_t idx)
                {
                    if (_dynamic == nullptr)
                    {
                        return _fixed[idx];
                    }
                    else
                    {
                        return _dynamic[idx];
                    }
                }


                ::pilo::i32_t capacity() const
                {
                    if (_dynamic == nullptr)
                    {
                        return TV_SIZE;
                    }
                    else
                    {
                        return _dynamic_capacity;
                    }
                }

                ::pilo::i32_t size() const
                {
                    return _data_size;
                }

                ::pilo::i32_t space_available() const
                {
                    return capacity() - _data_size;
                }

                ::pilo::err_t set_size(::pilo::i32_t sz)
                {
                    if (sz < 0 || sz > this->capacity())
                    {
                        PMC_ASSERT(false);
                        return ::pilo::make_core_error(PES_ELEM, PEP_ARR_IDX_OOB);
                    }
                    _data_size = sz;
                    return PILO_OK;
                }

                ::pilo::err_t add_size(::pilo::i32_t sz)
                {
                    ::pilo::i32_t ret = _data_size + sz;
                    if (ret < 0 || ret > _capacity)
                    {
                        PMC_ASSERT(false);
                        return ::pilo::make_core_error(PES_ELEM, PEP_ARR_IDX_OOB);
                    }

                    _data_size += sz;
                }

                ::pilo::i32_t& ref_size()
                {
                    return _data_size;
                }

            private:
                TA_ELEMOBJ* _dynamic;
                ::pilo::i32_t _dynamic_capacity;
                ::pilo::i32_t _data_size;
                TA_ELEMOBJ _fixed[TV_SIZE];
            };



            template<typename TA_ELEMOBJ, typename T = typename _pilo_type_trait<TA_ELEMOBJ>::is_basetype>
            class object_array_adapter
            {
                object_array_adapter()
                {
                    PMC_ASSERT(false);
                }
            };

            template<typename TA_ELEMOBJ>
            class object_array_adapter<TA_ELEMOBJ, ::pilo::_pilo_truetype>
            {
            public:
                typedef TA_ELEMOBJ value_type;

            public:
                object_array_adapter(value_type* orig_buffer, ::pilo::i32_t sz, ::pilo::i32_t data_size = 0 ,bool is_dynamic = false) : _ptr(orig_buffer), _capacity(sz),_data_size(data_size), _is_dynamic(is_dynamic)
                {
                }
                object_array_adapter() : _ptr(nullptr), _capacity(0), _data_size(0), _is_dynamic(false)
                {
                }
                ~object_array_adapter()
                {
                    destory();
                }

                object_array_adapter(const object_array_adapter&) = delete;

                object_array_adapter(object_array_adapter&& other) noexcept  
                {
                    _data_size = other._data_size;
                    _is_dynamic = other._is_dynamic;
                    _capacity = other._capacity;
                    _ptr = other._ptr;
                    other._ptr = nullptr;
                    other._capacity = -1;
                    other._is_dynamic = false;
                }

                object_array_adapter& operator=(object_array_adapter&& other) noexcept {
                    if (this != &other) {
                        this->destory();
                        _data_size = other._data_size;
                        _is_dynamic = other._is_dynamic;
                        _capacity = other._capacity;
                        _ptr = other._ptr;
                        other._ptr = nullptr;
                        other._capacity = -1;
                        other._is_dynamic = false;
                    }
                    return *this;
                }

                void destory()
                {
                    if (_ptr != nullptr && _is_dynamic)
                    {
                        PMF_HEAP_FREE(_ptr);
                    }
                    _ptr = nullptr;
                    _capacity = -1;
                    _is_dynamic = false;
                    _data_size = -1;
                }

                object_array_adapter mark_invalid()
                {
                    destory();
                    return std::move(*this);
                }

                void reset(value_type* obj_buffer, ::pilo::i32_t capa, ::pilo::i32_t data_size = 0, bool is_dynamic = false)
                {
                    if (_ptr != nullptr && _is_dynamic)
                    {
                        PMF_HEAP_FREE(_ptr);
                    }
                    _ptr = obj_buffer;
                    _capacity = capa;
                    _is_dynamic = is_dynamic;
                    _data_size = data_size;
                }

                void set_adopt(bool need_adopt)
                {
                    this->_is_dynamic = need_adopt;
                }
                
                ::pilo::err_t check_more_space(::pilo::i32_t neosz)
                {
                    ::pilo::i32_t delta = neosz - this->space_available;
                    if (delta > 0)
                    {
                        ::pilo::i32_t nsz = this->space_available() + delta;
                        return check_space(nsz);
                    }
                    return PILO_OK;
                }

                ::pilo::err_t check_space(::pilo::i32_t neosz)
                {                    
                    if (neosz > _capacity)
                    {
                        value_type* ptr = (value_type*) PMF_HEAP_MALLOC(neosz);
                        if (ptr == nullptr)
                        {
                            return ::pilo::make_core_error(PES_MEM, PEP_INSUFF);
                        }
                        memcpy(ptr, _ptr, _capacity * sizeof(value_type));
                        if (_ptr != nullptr && _is_dynamic)
                        {
                            PMF_HEAP_FREE(_ptr);
                        }
                        _ptr = ptr;
                        _capacity = neosz;
                        _is_dynamic = true;
                        return PILO_OK;
                    }
                    return PILO_OK;
                }

                bool is_dynamic() const
                {
                    return _is_dynamic;
                }

                value_type* begin()
                {
                    return _ptr;
                }

                value_type* ptr()
                {
                    return _ptr + _data_size;
                }

                value_type* ptr(::pilo::i32_t idx)
                {
                    return &(_ptr[idx]);
                }

                const value_type& at(::pilo::i32_t idx) const
                {
                    return _ptr[idx];
                }

                value_type& at(::pilo::i32_t idx)
                {
                    return _ptr[idx];
                }

                value_type& operator [] (::pilo::i32_t idx)
                {
                    return this->at(idx);
                }
                const value_type& operator [] (::pilo::i32_t idx) const
                {
                    return this->at(idx);
                }

                ::pilo::i32_t capacity() const
                {
                    return _capacity;
                }

                ::pilo::i32_t size() const
                {
                    return _data_size;
                }

                ::pilo::i32_t space_available() const
                {
                    return _capacity - _data_size;
                }

                ::pilo::err_t set_size(::pilo::i32_t sz)
                {
                    if (sz < 0 || sz > _capacity)
                    {
                        PMC_ASSERT(false);
                        return ::pilo::make_core_error(PES_ELEM, PEP_ARR_IDX_OOB);
                    }
                    _data_size = sz;
                    return PILO_OK;
                }

                ::pilo::err_t add_size(::pilo::i32_t sz)
                {
                    ::pilo::i32_t ret = _data_size + sz;
                    if (ret < 0 || ret > _capacity)
                    {
                        PMC_ASSERT(false);
                        return ::pilo::make_core_error(PES_ELEM, PEP_ARR_IDX_OOB);
                    }

                    _data_size += sz;
                    return PILO_OK;
                }


                ::pilo::i32_t& ref_size()
                {
                    return _data_size;
                }

                bool invalid() const
                {
                    if (_ptr == nullptr || _capacity < 1)
                        return true;
                    return false;
                }


                ::pilo::err_t set(::pilo::i32_t idx, value_type&& value)
                {
                    if (_ptr == nullptr || _capacity <= idx)
                    {
                        return ::pilo::make_core_error(PES_BUFFER, PEP_ARR_IDX_OOB);
                    }
                    _ptr[idx] = value;
                    return PILO_OK;
                }

                ::pilo::err_t set(::pilo::i32_t idx, const value_type& value)
                {
                    if (_ptr == nullptr || _capacity <= idx)
                    {
                        return ::pilo::make_core_error(PES_BUFFER, PEP_ARR_IDX_OOB);
                    }
                    _ptr[idx] = value;
                    return PILO_OK;
                }


            private:
                value_type* _ptr;
                ::pilo::i32_t _capacity;
                ::pilo::i32_t _data_size;
                bool _is_dynamic;
            };


            template<typename TA_ELEMOBJ>
            class object_array_adapter<TA_ELEMOBJ, ::pilo::_pilo_falsetype>
            {
            public:
                typedef TA_ELEMOBJ value_type;
            public:
                object_array_adapter(value_type* orig_buffer, ::pilo::i32_t sz, ::pilo::i32_t data_size = 0, bool is_dynamic = false) : _ptr(orig_buffer), _capacity(sz), _data_size(data_size), _is_dynamic(is_dynamic)
                {
                }
                object_array_adapter() : _ptr(nullptr), _capacity(0), _data_size(0), _is_dynamic(false)
                {
                }
                ~object_array_adapter()
                {
                    destory();
                }

                object_array_adapter(const object_array_adapter&) = delete;

                object_array_adapter(object_array_adapter&& other) noexcept
                {
                    _data_size = other._data_size;
                    _is_dynamic = other._is_dynamic;
                    _capacity = other._capacity;
                    _ptr = other._ptr;
                    other._ptr = nullptr;
                    other._capacity = -1;
                    other._is_dynamic = false;
                }

                object_array_adapter& operator=(object_array_adapter&& other) noexcept {
                    if (this != &other) {
                        this->destory();
                        _data_size = other._data_size;
                        _is_dynamic = other._is_dynamic;
                        _capacity = other._capacity;
                        _ptr = other._ptr;
                        other._ptr = nullptr;
                        other._capacity = -1;
                        other._is_dynamic = false;
                    }
                    return *this;
                }

                object_array_adapter mark_invalid()
                {
                    destory();
                    return std::move(*this);
                }

                void destory()
                {
                    if (_ptr != nullptr && _is_dynamic)
                    {
                        delete[] _ptr;
                    }
                    _ptr = nullptr;
                    _capacity = -1;
                    _is_dynamic = false;
                    _data_size = -1;
                }

                void reset(value_type* obj_buffer, ::pilo::i32_t capa, ::pilo::i32_t data_size = 0, bool is_dynamic = false)
                {
                    if (_ptr != nullptr && _is_dynamic)
                    {
                        delete[] _ptr;
                    }
                    _ptr = obj_buffer;
                    _capacity = capa;
                    _is_dynamic = is_dynamic;
                    _data_size = data_size;
                }

                const value_type& at(::pilo::i32_t idx) const
                {
                    return _ptr[idx];
                }

                value_type& at(::pilo::i32_t idx)
                {
                    return _ptr[idx];
                }

                value_type& operator [] (::pilo::i32_t idx)
                {
                    return this->at(idx);
                }
                const value_type& operator [] (::pilo::i32_t idx) const
                {
                    return this->at(idx);
                }

                ::pilo::i32_t check_space(::pilo::i32_t new_sz)
                {
                    if (new_sz > _capacity)
                    {
                        value_type* ptr = new value_type[new_sz];
                        for (::pilo::i32_t i = 0; i < _capacity; i++)
                        {
                            ptr[i] = _ptr[i];
                        }
                        if (_ptr != nullptr && _is_dynamic)
                        {
                            delete[] _ptr;
                        }                              
                        _ptr = ptr;
                        _capacity = new_sz;
                        _is_dynamic = true;
                        return 1;
                    }
                    return 0;
                }

                bool is_dynamic() const
                {
                    return _is_dynamic;
                }

                value_type* begin()
                {
                    return _ptr;
                }

                value_type* ptr()
                {
                    return _ptr + _data_size;
                }

                value_type* ptr(::pilo::i32_t idx)
                {
                    return &(_ptr[idx]);
                }

                ::pilo::i32_t capacity() const
                {
                    return _capacity;
                }

                ::pilo::i32_t size() const
                {
                    return _data_size;
                }

                ::pilo::i32_t space_available() const
                {
                    return _capacity - _data_size;
                }

                ::pilo::err_t set_size(::pilo::i32_t sz)
                {
                    if (sz < 0 || sz > _capacity)
                    {
                        PMC_ASSERT(false);
                        return ::pilo::make_core_error(PES_ELEM, PEP_ARR_IDX_OOB);
                    }
                    _data_size = sz;
                }

                ::pilo::err_t add_size(::pilo::i32_t sz)
                {
                    ::pilo::i32_t ret = _data_size + sz;
                    if (ret < 0 || ret > _capacity)
                    {
                        PMC_ASSERT(false);
                        return ::pilo::make_core_error(PES_ELEM, PEP_ARR_IDX_OOB);
                    }
                    _data_size += sz;
                    return PILO_OK;
                }

                ::pilo::i32_t& ref_size()
                {
                    return _data_size;
                }

                bool invalid() const
                {
                    if (_ptr == nullptr || _capacity < 1)
                        return true;
                    return false;
                }

                ::pilo::err_t set(::pilo::i32_t idx, value_type&& value)
                {
                    if (_ptr == nullptr || _capacity <= idx)
                    {
                        return ::pilo::make_core_error(PES_BUFFER, PEP_ARR_IDX_OOB);
                    }
                    _ptr[idx] = value;
                    return PILO_OK;
                }

            private:
                value_type* _ptr;
                ::pilo::i32_t _capacity;
                ::pilo::i32_t _data_size;
                bool _is_dynamic;
            };

        }
    }
    typedef ::pilo::core::memory::object_array_adapter<char> char_buffer_t;
    typedef ::pilo::core::memory::object_array_adapter<wchar_t> wchar_buffer_t;


}