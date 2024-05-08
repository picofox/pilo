#pragma once

#include <thread>
#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class auto_join_thread
            {
            public:
                auto_join_thread() noexcept = delete;

                template<typename TA_START_ROUTINE, typename ... TA_ARG> 
                explicit auto_join_thread(TA_START_ROUTINE&& func, TA_ARG&& ... args)
                    : _internal_handler(std::forward<TA_START_ROUTINE>(func), std::forward<TA_ARG>(args)...) {}

                explicit auto_join_thread(std::thread t) noexcept : _internal_handler(std::move(t)) {}

                auto_join_thread(auto_join_thread && thr) noexcept : _internal_handler(std::move(thr._internal_handler)) {}

                auto_join_thread& operator=(auto_join_thread&& thr) noexcept
                {
                    join();
                    _internal_handler = std::move(thr._internal_handler);
                    return *this; 
                }

                auto_join_thread& operator=(std::thread thr_ih) noexcept
                {
                    join();
                    _internal_handler = std::move(thr_ih);
                    return *this;
                }

                ~auto_join_thread()
                {
                    join();
                }

                void swap(auto_join_thread& thr) noexcept 
                {
                    _internal_handler.swap(thr._internal_handler);
                }

                std::thread::id id() const noexcept
                {
                    return _internal_handler.get_id();
                }

                void join() 
                {
                    if (_internal_handler.joinable())
                        _internal_handler.join();
                }

                bool joinable() const noexcept
                {
                    return _internal_handler.joinable();
                }

                void detach()
                {
                    _internal_handler.detach();
                }

                std::thread& as_std_thread() noexcept
                {
                    return _internal_handler;
                }

                const std::thread& as_std_thread() const noexcept
                {
                    return _internal_handler;
                }

            private:
                std::thread _internal_handler;

            };
        }
    }
}