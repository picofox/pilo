#pragma once
#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace pattern
        {
            template <class T>
            class hungry_singleton_mts
            {
            protected:
                hungry_singleton_mts(){};
            private:
                hungry_singleton_mts(const hungry_singleton_mts&){};//½ûÖ¹¿½±´
                hungry_singleton_mts& operator=(const hungry_singleton_mts&){};//½ûÖ¹¸³Öµ
                static T* sp_instance;
            public:
                static T* instance();
            };


            template <class T>
            T* hungry_singleton_mts<T>::instance()
            {
                return sp_instance;
            }

            template <class T>
            T* hungry_singleton_mts<T>::sp_instance = new T();
        }
    }
}

