#ifndef _pilo_core_algorithm_uint_sequence_generator_hpp_
#define _pilo_core_algorithm_uint_sequence_generator_hpp_

#include "../../pilo.hpp"
#include <atomic>
#include <limits>

namespace pilo 
{
    namespace core 
    {
        namespace algorithm 
        {
            template<typename T>
            class uint_sequence_generator_nrc
            {
            public:
                uint_sequence_generator_nrc(T cur = 0)
                    : _current(cur) 
                {
                }

                T next()
                {                    
                    return ++_current;
                }

            protected:
                std::atomic<T>          _current;
            };

            template<typename T>
            class uint_sequence_generator
            {
            public:
                uint_sequence_generator(T cur = 0)
                    : _current(cur) {}

                T next()
                {
                    T v = ++_current;
                    if (v == (std::numeric_limits<T>::max)()) {
                        _current = 0;
                    }
                        
                    return v;
                }

            protected:
                std::atomic<T>          _current;
            };
        }
    }
}


#endif