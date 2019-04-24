#pragma once

namespace pilo
{
    namespace core
    {
        namespace container
        {

            /**
            * 可以扩展的数组，有一个初始容量
            * 对可以放到容器中的数据结构是有要求的:
            *   1. 元素必须可以通过memcpy来复制
            *   2. 析构时没有额外的操作
            *
            * 适用于简单的数据类型，可以和栈里面的数组绑定
            */
            template <typename T, size_t initCapacity = 32, size_t inflateStep = 16> 
            class dynamic_array
            {
            public:
                typedef T value_type;

            public:
                bool            _m_is_attached;
                size_t          _m_size;
                size_t          _m_capacity;
                value_type*     _m_vals;

            public:
                dynamic_array()
                    : _m_is_attached(false)
                    , _m_size(0)
                    , _m_capacity(0)
                    , _m_vals(nullptr)
                {
                    M_ASSERT(_is_simple_type_<T>::value);
                }
                dynamic_array(value_type* buffer, size_t capacity, size_t size)
                    : _m_is_attached(true)
                    , _m_vals(buffer)
                    , _m_capacity(capacity)
                    , _m_size(size)
                {
                    M_ASSERT(_is_simple_type_<T>::value);
                }
                dynamic_array(const dynamic_array& o)
                    :_m_is_attached(false)
                    , _m_size(0)
                    , _m_capacity(0)
                    , _m_vals(nullptr)
                {
                    M_ASSERT(_is_simple_type_<T>::value);
                    if (o._m_size == 0) return;
                    this->inflate(o._m_size);
                    memcpy(_m_vals, o._m_vals, o._m_size*sizeof(value_type));
                    _m_size = o._m_size;
                }
                ~dynamic_array()
                {
                    this->destroy();
                }
                dynamic_array& operator= (const dynamic_array& o)
                {
                    if (this == &o) return *this;

                    this->clear();
                    if (o._m_size==0) return *this;

                    if (_m_capacity < o._m_size) this->inflate(o._m_size);
                    memcpy(_m_vals, o._m_vals, o._m_size*sizeof(value_type));
                    _m_size = o._m_size;

                    return *this;
                }
                size_t size() const
                {
                    return _m_size;
                }
                void clear()
                {
                    _m_size = 0;
                }
                bool empty() const
                {
                    return _m_size < 1;
                }
                dynamic_array& attach(value_type* vals, size_t capacity, size_t size)
                {
                    this->destroy();
                    _m_is_attached = true;
                    _m_size = size;
                    _m_capacity = capacity;
                    _m_vals = vals;
                    return *this;
                }
                void detach()
                {
                    if (!_m_is_attached) return;
                    _m_is_attached = false;
                    _m_size = 0;
                    _m_capacity = 0;
                    _m_vals = nullptr;
                }
                void resize(size_t newSize)
                {
                    M_ASSERT(newSize >= 0);
                    if (_m_capacity < newSize) this->inflate(newSize);
                    if (_m_size < newSize)
                    {
                        if (_type_default_value_<value_type>::need_set_default_value)
                        {
                            this->set_defualt_value(_m_size, newSize - _m_size);
                        }
                    }
                    _m_size = newSize;
                }
                value_type& at(size_t idx)
                {
                    M_ASSERT(idx >= 0);
                    size_t size = idx + 1;
                    if (_m_capacity < size) this->inflate(size);
                    if (_m_size < size)
                    {
                        if (_type_default_value_<value_type>::need_set_default_value)
                        {
                            this->set_defualt_value(_m_size, size - _m_size);
                        }
                        _m_size = size;
                    }
                    return _m_vals[idx];
                }
                const value_type& at(size_t idx) const
                {
                    M_ASSERT(idx < _m_size);
                    return _m_vals[idx];
                }
                value_type& get(size_t idx)
                {
                    M_ASSERT(idx < _m_size);
                    return _m_vals[idx];
                }
                value_type& operator[] (size_t idx)
                {
                    return this->at(idx);
                }
                const value_type& operator[] (size_t idx) const
                {
                    return this->at(idx);
                }
                dynamic_array& push_back(const value_type& val)
                {
                    this->at(_m_size) = val;
                    return *this;
                }
                value_type pop_back()
                {
                    M_ASSERT(_m_size > 0);
                    _m_size -= 1;
                    return _m_vals[_m_size];
                }
                value_type& back()
                {
                    M_ASSERT(_m_size > 0);
                    return _m_vals[_m_size - 1];
                }
                const value_type& back() const
                {
                    M_ASSERT(_m_size > 0);
                    return _m_vals[_m_size - 1];
                }
                value_type& front()
                {
                    M_ASSERT(_m_size > 0);
                    return _m_vals[0];
                }
                const value_type& front() const
                {
                    M_ASSERT(_m_size > 0);
                    return _m_vals[0];
                }
                dynamic_array& erase(size_t idx)
                {
                    if (idx >= _m_size) return *this;
                    size_t len = (_m_size - idx - 1) * sizeof(value_type);
                    if (len > 0)
                    {
                        void* pSrc = &_m_vals[idx + 1];
                        void* pDst = &_m_vals[idx];
                        ::memmove(pDst, pSrc, len);
                    }
                    _m_size -= 1;
                    return *this;
                }
                dynamic_array& insert(size_t idx, const value_type& val)
                {
                    if (idx >= _m_size)
                    {
                        this->at(idx) = val;
                        return *this;
                    }

                    size_t size = _m_size + 1;
                    if (_m_capacity < size) this->inflate(size);

                    size_t len = (_m_size - idx)*sizeof(value_type);
                    if (len > 0)
                    {
                        void* pSrc = &_m_vals[idx];
                        void* pDst = &_m_vals[idx + 1];
                        memmove(pDst, pSrc, len);
                    }
                    _m_vals[idx] = val;
                    _m_size += 1;
                    return *this;
                }

            protected:
                // 在当前的capacity小于size时被调用
                bool inflate(size_t size)
                {
                    M_ASSERT(!_m_is_attached);
                    if (_m_is_attached)
                    {
                        return false;
                    }

                    if (nullptr == _m_vals)
                    {
                        size_t capacity = initCapacity;
                        while (capacity < size) capacity += inflateStep;
                        _m_vals = (value_type*) malloc (capacity*sizeof(value_type));
                        _m_capacity = capacity;
                    }
                    else
                    {
                        size_t capacity = _m_capacity;
                        while (capacity < size) capacity += inflateStep;
                        _m_vals = (value_type*)realloc(_m_vals, capacity*sizeof(value_type));
                        _m_capacity = capacity;
                    }
                    return true;
                }
                void destroy()
                {
                    this->clear();
                    if ((! _m_is_attached) && (_m_vals != nullptr))
                    {
                        free(_m_vals);
                        _m_vals = nullptr;
                        _m_capacity = 0;
                    }
                }
                void set_defualt_value(size_t from, size_t count)
                {
                    const value_type defVal = _type_default_value_<value_type>::get();
                    while (count > 0)
                    {
                        _m_vals[from++] = defVal;
                        count -= 1;
                    }
                }
            };
        }
    }
}