 #pragma once

#include <functional>
#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace pattern
        {
            template<typename T, ::pilo::i32_t TA_SIZE>
            class function_dispatcher
            {
            public:
                function_dispatcher() : _handlers{nullptr}
                {
                   
                }
                template<typename ... Args>
                function_dispatcher(std::function<T> first, Args...args) : _handlers{ nullptr }
                {
                    set_param(0, first, args...);
                }

                void set_param(::pilo::i32_t)
                {
                    
                }
                template<typename ... Args>
                void set_param(::pilo::i32_t idx, std::function<T> first, Args...args)
                {
                    _handlers[idx] = (first);
                    set_param(idx+1, args...);
                }

  
                ::pilo::err_t register_handler(::pilo::i32_t index, std::function<T> t)
                {
                    if (index < TA_SIZE)
                        _handlers[index] = t;
                    return -1;
                }

                ::pilo::err_t unregister_handler(::pilo::i32_t index)
                {
                    if (index < TA_SIZE)
                        _handlers[index] = nullptr;

                    return -1;
                }

                std::function<T>& dispatch(::pilo::i32_t index)
                {
                    return _handlers[index];
                }

            private:
                std::function<T>   _handlers[TA_SIZE];
            };
        }
    }
}