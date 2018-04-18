#pragma once
#include "core/coredefs.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {

            class nonrecursive_mutex
            {
            public:
                nonrecursive_mutex();
                ~nonrecursive_mutex();

                bool try_lock();
                void lock();
                void unlock();


            protected:
#           ifdef WINDOWS
                PILO_STATIC_CONSTANT(unsigned char, lock_flag_bit = 31);
                PILO_STATIC_CONSTANT(unsigned char, event_set_flag_bit = 30);
                PILO_STATIC_CONSTANT(long, lock_flag_value = 1 << lock_flag_bit);
                PILO_STATIC_CONSTANT(long, event_set_flag_value = 1 << event_set_flag_bit);
                void* _m_event;
                long  _m_active_count;

                void* get_event();                
#           else
                pthread_mutex_t _m_mutex;
#           endif

            private:
                M_DISABLE_COPY(nonrecursive_mutex)
            };
            

	    }
    }
}



