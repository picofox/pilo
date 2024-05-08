#pragma once

#include "../threading/spin_mutex.hpp"
#include <queue>
#include <mutex>

namespace pilo
{
    namespace core
    {
        namespace container
        {
            template<typename TA_ITEM, typename TA_LOCK = ::pilo::core::threading::spin_mutex>
            class trivil_concurrent_queue
            {
            public:
                typedef TA_ITEM value_type;
                typedef TA_LOCK lock_type;

            public:
                bool enqueue(value_type const& item)
                {
                    std::lock_guard<lock_type> locker(_mutex);
                    _q.push(item);
                    return true;
                }

                bool try_enqueue(value_type const& item)
                {
                    if (_mutex.try_lock())
                    {
                        try
                        {
                            _q.push(item);

                        }
                        catch (...)
                        {
                            _mutex.unlock();
                            return false;
                        }
                        _q.push(item);
                        return true;
                    } 
                    return false;
                }

                bool dequeue(value_type& item)
                {
                    std::lock_guard<lock_type> locker(_mutex);
                    if (_q.empty())
                        return false;
                    item = _q.front();
                    _q.pop();
                    return true;
                }

                bool try_dequeue(value_type& item)
                {
                    if (_mutex.try_lock())
                    {
                        try
                        {
                            if (_q.empty())
                            {
                                _mutex.unlock();
                                return false;
                            }
                            item = _q.front();
                            _q.pop();                            
                        }
                        catch (...)
                        {               
                            _mutex.unlock();
                            return false;
                        }
                        _mutex.unlock();
                        return true;
                    }
                    return false;
                }



            protected:
                std::queue<value_type> _q;
                lock_type _mutex;
            };
        }
    }
}