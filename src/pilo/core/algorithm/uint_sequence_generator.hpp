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
                uint_sequence_generator(T cur = 0, T max_value = std::numeric_limits<T>::max)
                    : _current(cur), _max_value(max_value) {}

                T next()
                {
                    T v = ++_current;
                    if (v == _max_value) {
                        _current = 0;
                    }
                        
                    return v;
                }

            protected:
                std::atomic<T>          _current;
                const T                 _max_value;
            };

            template<typename T, T init, T step>
            class uint_step_sequence_generator
            {
            public:
                uint_step_sequence_generator()
                    : _current(init) {}

                T next()
                {
                    T v = _current.fetch_add(step);
                    if (v >= ((((std::numeric_limits<T>::max)() - init) / step * step)) + 1 ) {
                        _current = init;
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