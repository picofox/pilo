#pragma once

namespace pilo
{
    namespace core
    {
        namespace pattern
        {
            template <class T>
            class _singleton
            {
            protected:
                _singleton() {};

            public:
                static T* instance()
                {
                    static T _instance;
                    return &_instance;
                }

            private:
                _singleton(const _singleton &);
                _singleton& operator=(const _singleton &);
                

            };

        }
    }
}
