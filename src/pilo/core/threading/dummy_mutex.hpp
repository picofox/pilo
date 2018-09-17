#pragma once


namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class dummy_mutex
            {
            public:
                //Tries to lock the mutex. Returns immediately. On successful lock acquisition returns true, otherwise returns false. 
                bool try_lock() { return true; }
                void lock() {};
                void unlock() {}
            };
        }
    }
}
