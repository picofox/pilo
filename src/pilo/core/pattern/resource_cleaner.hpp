#ifndef __resource_cleaner_h_
#define __resource_cleaner_h_

#include <array>
#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace pattern
        {
            template<::pilo::i32_t TA_SIZE>
            class resource_cleaner
            {
            public:
                typedef ::pilo::err_t(*cleaner_func_t) (void* ptr, void* ctx);

            public:
                class rc_item
                {
                public:
                    rc_item() : m_cleaner(nullptr),m_ptr(nullptr), m_ctx(nullptr)  {}
                    rc_item(cleaner_func_t cleaner, void* ptr, void* ctx) :m_cleaner(cleaner), m_ptr(ptr), m_ctx(ctx) {}
                    rc_item(rc_item&& o)
                        :m_ptr(nullptr)
                        , m_ctx(nullptr)
                        , m_cleaner(nullptr)
                    {
                        m_ptr = o.m_ptr;
                        m_ctx = o.m_ctx;
                        m_cleaner = o.m_cleaner;
                        o.m_cleaner = nullptr;
                        o.m_ctx = nullptr;
                        o.m_ptr = nullptr;
                    }

                    rc_item& operator=(rc_item&& o)
                    {
                        if (this != &o)
                        {
                            m_ptr = o.m_ptr;
                            m_ctx = o.m_ctx;
                            m_cleaner = o.m_cleaner;
                            o.m_cleaner = nullptr;
                            o.m_ctx = nullptr;
                            o.m_ptr = nullptr;
                        }
                        return *this;
                    }

                    ~rc_item()
                    {
                        m_cleaner = nullptr;
                        m_ptr = nullptr;
                        m_ctx = nullptr;                        
                    }

                    ::pilo::err_t clean()
                    {
                        return m_cleaner(m_ptr, m_ctx);
                    }

                    bool valid() const
                    {
                        return (m_cleaner != nullptr);
                    }

                public:
                    cleaner_func_t m_cleaner;
                    void* m_ptr;
                    void* m_ctx;
                };

            public:

                resource_cleaner() : _size(0)
                {
                }

                ~resource_cleaner()
                {
                    clean();
                }

                ::pilo::err_t push(cleaner_func_t cbf, void* ptr, void* ctx)
                {
                    if (cbf == nullptr)
                    {
                        return ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                    }

                    if (_size >= _items.max_size())
                    {
                        return ::pilo::make_core_error(PES_ELEM, PEP_TOO_LARGE);
                    }
                    rc_item ri(cbf, ptr, ctx);
                    _items[_size] = std::move(ri);
                    _size++;
                    return PILO_OK;
                }
                
                ::pilo::err_t clean()
                {   
                    ::pilo::err_t saved_err = PILO_OK;
                    if (_size > 0)
                    {
                        _size--;
                        while(_size >= 0)
                        {
                            if (_items[_size].valid())
                            {
                                pilo::err_t err = _items[_size].clean();
                                if (err != PILO_OK)
                                    saved_err = err;
                            }                            
                            _size--;
                        }
                    }
                    return saved_err;
                }

            private:
                ::pilo::i32_t _size;
                std::array<rc_item, TA_SIZE>   _items;
            };
        }
    }
}


#endif