#pragma once
#include "core/coredefs.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/threading//dummy_mutex.hpp"
#include "core/memory/object_pool.hpp"
#include "core/threading/nonrecursive_mutex.hpp"
#include "core/memory/heap_memory_allocator.hpp"
#include "core/threading/mutex_locker.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
            template<
                size_t   _Capacity = 0,
                typename _Lock = pilo::core::threading::dummy_mutex,
                typename _ALLOCATOR = ::pilo::core::memory::heap_memory_allocator
                >
            class dynamic_buffer
            {
            public: //types
                typedef _ALLOCATOR allocator_type;
                typedef _Lock   lock_type;

            protected: //internal member vars
                char*           _m_buffer;
                size_t          _m_capacity;
                ::pilo::u32_t   _m_flags;
                allocator_type  _m_allocator;
                mutable lock_type   _m_lock;
                size_t      _m_rp;
                size_t      _m_wp;

            protected: // internal member methods
                inline void _reset()
                {
                    _m_rp = 0;
                    _m_wp = 0;
                    _m_capacity = 0;
                    _m_flags = 0;
                    _m_buffer = nullptr;
                }

                inline bool _is_owner() const
                {
                    return ((_m_flags & 0x1) == 0);
                }

                ::pilo::i32_t _assign(const char* buffer, size_t capacity, size_t len)
                {
                    if (_m_capacity < capacity)
                    {
                        if (_expand(capacity) != ::pilo::EC_OK)
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }

                        _m_capacity = capacity;
                    }

                    _m_rp = 0;
                    _m_wp = 0;
                    _m_capacity = 0;
                    _m_flags = 0;

                    ::memcpy(_m_buffer, buffer, len);
                    _m_capacity = capacity;

                    return ::pilo::EC_OK;
                }

                ::pilo::i32_t _assign(const char* buffer, size_t capacity)
                {
                    if (_m_capacity < capacity)
                    {
                        if (_expand(capacity) != ::pilo::EC_OK)
                        {
                            return ::pilo::EC_INSUFFICIENT_MEMORY;
                        }

                        _m_capacity = capacity;
                    }

                    _m_rp = 0;
                    _m_wp = 0;
                    _m_capacity = 0;
                    _m_flags = 0;

                    ::memcpy(_m_buffer, buffer, capacity);
                    _m_capacity = capacity;

                    return ::pilo::EC_OK;
                }

                template<typename T>
                ::pilo::i32_t _assign(const T& o)
                {
                    if ( &o != (T*) this)
                    {
                        ::pilo::i32_t ret = this->_assign(o.data(), o.capacity());
                        if (ret != ::pilo::EC_OK)
                        {
                            return ret;
                        }
                        _m_rp = o.rp();
                        _m_wp = o.wp();
                    }
                    return ::pilo::EC_OK;
                }
                

                inline ::pilo::i32_t _expand(size_t sz)
                {
                    if (!_is_owner())
                    {
                        return ::pilo::EC_NOT_OWNER;
                    }

                    size_t new_capa = 0;
                    if (sz == _m_capacity)
                    {
                        return ::pilo::EC_OK;
                    }
                    else if (sz < _m_capacity)
                    {
                        return ::pilo::EC_OP_NOT_ALLOWED;
                    }
                    else
                    {
                        new_capa = sz;
                    }

                    char * new_buffer = (char*)_m_allocator.allocate(new_capa);
                    if (new_buffer == nullptr)
                    {
                        return ::pilo::EC_INSUFFICIENT_MEMORY;
                    }

                    MP_SAFE_FREE(_m_buffer);
                    _m_buffer = new_buffer;
                    _m_capacity = new_capa;

                    return ::pilo::EC_OK;
                }

                inline void _compact()
                {
                    if (_m_rp != 0)
                    {
                        if (_m_wp > _m_rp)
                        {
                            memmove(&_m_buffer[0], &_m_buffer[_m_rp], _m_wp - _m_rp);
                            _m_wp = _m_wp - _m_rp;
                            _m_rp = 0;
                        }
                        else
                        {
                            _m_rp = _m_wp = 0;
                        }
                    }
                }

            public: //c-tors and d-tors
                dynamic_buffer()
                {
                    _reset();
                    if (_Capacity > 0)
                    {
                        if (_expand(_Capacity) != ::pilo::EC_OK)
                        {
                            throw ::pilo::EC_INSUFFICIENT_MEMORY;
                            return;
                        }
                    }
                    
                }
                dynamic_buffer(size_t capacity)
                {
                    _reset();
                    if (_expand(capacity) != ::pilo::EC_OK)
                    {
                        throw ::pilo::EC_INSUFFICIENT_MEMORY;
                        return;
                    }
                }

                dynamic_buffer(const char* pData, size_t capacity, size_t data_size, bool is_owner)
                {
                    M_ASSERT(pData != nullptr);
                    M_ASSERT(data_size <= capacity);

                    _reset();
                    _m_buffer = nullptr;

                    if (!is_owner)
                    {
                        this->attach(pData, capacity, data_size);
                    }
                    else
                    {
                        if (_expand(capacity) != ::pilo::EC_OK)
                        {
                            throw ::pilo::EC_INSUFFICIENT_MEMORY;
                            return;
                        }

                        if (pData != nullptr && data_size <= capacity)
                        {
                            ::memcpy(_m_buffer, pData, data_size);
                        }
                        
                    }
                }

                ~dynamic_buffer()
                {
                    reset();
                }
                
            public:
                char& operator[] (size_t pos)
                {
                    M_ASSERT(pos < capacity());
                    return _m_buffer[pos];
                }
                const char& operator[] (size_t pos) const
                {
                    M_ASSERT(pos < capacity());
                    return _m_buffer[pos];
                }

                

            public: 
                size_t capacity() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    size_t c = _m_capacity;
                    return c;
                }

                inline bool is_owner() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    bool ret = _is_owner();
                    return ret;
                }

                inline bool read_available() const
                {          
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    return _m_rp < _m_wp;
                }

                inline size_t available_bytes_for_reading() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    return _m_wp - _m_rp;
                }

                inline bool write_available() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    return _m_wp < _m_capacity;
                }

                inline size_t available_bytes_for_writing() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    return _m_capacity - _m_wp;
                }

                inline const char* data() const
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    return &_m_buffer[0];
                }

                inline const char* read_buffer() const
                {
                    char* ret = nullptr;
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);

                    if (capacity() > 0)
                    {
                        ret = &_m_buffer[_m_rp];                        
                    }
                    return ret;
                }

                inline char* write_buffer()
                {
                    char* ret = nullptr;
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);

                    if (capacity() > 0)
                    {
                        ret = &_m_buffer[_m_wp];    
                    }
                    return ret;                    
                }

                inline void set_rp(size_t rp) 
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    _m_rp = rp; 
                }

                inline void inc_rp(size_t delta) 
                { 
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    _m_rp += delta; 
                }

                inline void dec_rp(size_t delta) 
                { 
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    _m_rp -= delta; 
                }

                inline void set_wp(size_t wp) 
                { 
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    _m_wp = wp; 
                }

                inline void inc_wp(size_t delta) 
                { 
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    _m_wp += delta; 
                }
                inline void dec_wp(size_t delta) 
                { 
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    _m_wp -= delta; 
                }
                inline size_t rp() const
                {
                    return _m_rp;
                }
                inline size_t wp() const
                {
                    return _m_wp;
                }


                inline void reset()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    
                    if (_is_owner())
                    {
                        if (_m_buffer != nullptr)
                        {
                            _m_allocator.deallocate(_m_buffer, _m_capacity);

                        }                        
                    }
                    _m_buffer = nullptr;
                    _reset();                    
                }

                void clear()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    _m_rp = 0;
                    _m_wp = 0;
                }                               

                ::pilo::i32_t attach(char* external_buffer, size_t capacity)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    _reset();
                    _m_flags |= 0x1;
                    _m_buffer = external_buffer;
                    _m_rp = 0;
                    _m_wp = 0;
                    _m_capacity = capacity; 
                    return ::pilo::EC_OK;
                }

                void detach()
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    if (!_is_owner())
                    {
                        _reset();
                    }
                }

                ::pilo::i32_t resize(size_t capacity)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    return _expand(capacity);
                }                

                inline size_t peek(void* pData, size_t want)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    size_t ravail = this->available_bytes_for_reading();
                    if (ravail < 1) return 0;
                    if (ravail < want) want = ravail;
                    ::memcpy(pData, &_m_buffer[_m_rp], want);
                    return want;
                }
                
                inline size_t read(void* pData, size_t want)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    size_t ravail = this->available_bytes_for_reading();
                    if (ravail < 1) return 0;
                    if (ravail < want) want = ravail;
                    memcpy(pData, &_m_buffer[_m_rp], want);
                    _m_rp += want;
                    return want;
                }

                inline size_t write(const void* pData, size_t len)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    size_t wavail = this->available_bytes_for_writing();
                    if (wavail < 1) return 0;
                    if (wavail < len) len = wavail;
                    memcpy(&_m_buffer[_m_wp], pData, len);
                    _m_wp += len;
                    return len;
                }

                inline ::pilo::i32_t assign(const char* buffer, size_t capacity, size_t len)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    return _assign(buffer,capacity,len);
                }

                inline ::pilo::i32_t assign(const char* buffer, size_t capacity)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    return _assign(buffer, capacity);
                }

                template<typename T>
                inline ::pilo::i32_t assign(const T& o)
                {
                    pilo::core::threading::mutex_locker<lock_type>   locker(_m_lock);
                    return _assign(o);
                }
            };



            template<
                size_t   _Capacity = 0,
                typename _Lock = pilo::core::threading::dummy_mutex,
                typename _ALLOCATOR = ::pilo::core::memory::heap_memory_allocator
            >
            struct dynamic_buffer_node : public dynamic_buffer<_Capacity, _Lock, _ALLOCATOR>
            {
                dynamic_buffer_node* m_next;
            };
        }
    }
}