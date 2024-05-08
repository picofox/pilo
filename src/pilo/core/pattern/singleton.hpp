#pragma once

namespace pilo
{
    namespace core
    {
        namespace pattern
        {
            template <class T>
            class singleton
            {
            protected:
                singleton() {};                

            public:
                static T* instance()
                {
                    static T _instance;
                    return &_instance;
                }

            private:
                singleton(const singleton &);
                singleton& operator=(const singleton &);
                

            };

        }
    }
}
