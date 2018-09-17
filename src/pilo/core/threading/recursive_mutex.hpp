#pragma once
#include "../../core/coredefs.hpp"
#include "../datetime/datetime.hpp"

namespace pilo
{
    namespace core
    {
        namespace threading
        {
            class recursive_mutex
            {
            public:
                recursive_mutex();
                ~recursive_mutex();
                bool try_lock();
                void lock();
                void unlock();
            
            protected:

#           ifdef WINDOWS                
                long _m_recursion_count;
                long _m_locking_thread_id;
                PILO_STATIC_CONSTANT(unsigned char, lock_flag_bit = 31);
                PILO_STATIC_CONSTANT(unsigned char, event_set_flag_bit = 30);
                PILO_STATIC_CONSTANT(long, lock_flag_value = 1 << lock_flag_bit);
                PILO_STATIC_CONSTANT(long, event_set_flag_value = 1 << event_set_flag_bit);
                long _m_active_count;
                void* _m_event;

                void* ___get_event();
                void ___mark_waiting_and_try_lock(long& old_count);
                void ___clear_waiting_and_try_lock(long& old_count);
                bool __try_lock();
                void __lock();
                void __unlock();
                bool _try_recursive_lock(long current_thread_id);
                bool _try_basic_lock(long current_thread_id);
                

#           else
                pthread_mutex_t _m_mutex;
#           endif

            private:
                M_DISABLE_COPY(recursive_mutex)
            };

        }
    }
}



