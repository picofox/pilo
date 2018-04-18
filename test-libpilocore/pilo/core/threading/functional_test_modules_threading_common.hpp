#pragma once

#include "../../testing_module.hpp"
#include "core/threading/basic_thread.hpp"
#include "core/datetime/datetime.hpp"

namespace pilo
{
    namespace test
    {
        template<typename T>
        class c_test_mutex_lock_and_unlock : public pilo::core::threading::basic_thread
        {
        public:
            c_test_mutex_lock_and_unlock()
            {
                m_loops = 0;
                m_mutex = nullptr;
                m_value = nullptr;
            }

            virtual pilo::i32_t on_run()
            {
                for (size_t i = 0; i < m_loops; i++)
                {
                    m_mutex->lock();
                    (*m_value)++;
                    m_mutex->unlock();
                }
                return  0;
            }

            pilo::uint_r* m_value;
            pilo::u32_t m_loops;
            T* m_mutex;

        };

        template<typename T>
        class c_test_mutex_lock_only : public pilo::core::threading::basic_thread
        {
        public:
            c_test_mutex_lock_only()
            {
                m_mutex = nullptr;
                m_stop = 0;
            }

            ~c_test_mutex_lock_only()
            {
                
            }

            virtual pilo::i32_t on_run()
            {

                m_mutex->lock();

                while (! m_stop)
                {
                    basic_thread::sleep(1);
                }
                
                m_mutex->unlock();
                
                return  0;
            }

            volatile int m_stop;
            T* m_mutex;

        };

        template<typename T>
        class c_test_mutex_lock_and_unlock_recur : public pilo::core::threading::basic_thread
        {
        public:
            c_test_mutex_lock_and_unlock_recur()
            {
                m_loops = 0;
                m_mutex = nullptr;
                m_value = nullptr;
            }

            virtual pilo::i32_t on_run()
            {
                for (size_t i = 0; i < m_loops; i++)
                {
                    for (size_t j = 0; j < 100; j++)
                    {
                        m_mutex->lock();
                    }
                    
                    (*m_value)++;

                    for (size_t j = 0; j < 100; j++)
                    {
                        m_mutex->unlock();
                    }
                }
                return  0;
            }

            pilo::uint_r* m_value;
            pilo::u32_t m_loops;
            T* m_mutex;

        };

        template<typename T>
        class c_test_mutex_timedlock : public pilo::core::threading::basic_thread
        {
        public:
            c_test_mutex_timedlock()
            {
               
            }

            virtual pilo::i32_t on_run()
            {
                bool ret;
                for (size_t i = 0; i < m_loops; i++)
                {
                    pilo::uint_r start = PiloGetTickCount64();
                    ret = m_mutex->try_lock_for(100);
                    if (ret == false)
                    {
                        pilo::uint_r end = PiloGetTickCount64();
                        if ((end - start > 150) || (end - start < 50))
                        {
                            return -1;
                        }
                    }
                    else
                    {
                        return -2;
                    }
                    
                }
                return  0;
            }


            pilo::u32_t m_loops;
            T* m_mutex;
        };

        template<typename T>
        class c_test_mutex_trylock_false : public pilo::core::threading::basic_thread
        {
        public:
            c_test_mutex_trylock_false()
            {

            }

            virtual pilo::i32_t on_run()
            {
                bool ret;
                for (size_t i = 0; i < m_loops; i++)
                {
                    ret = m_mutex->try_lock();
                    if (ret == false)
                    {
                        ;
                    }
                    else
                    {
                        return -2;
                    }

                }
                return  0;
            }


            pilo::u32_t m_loops;
            T* m_mutex;

        };

        template<typename T>
        class c_test_mutex_trylock_for_false : public pilo::core::threading::basic_thread
        {
        public:
            c_test_mutex_trylock_for_false()
            {

            }

            virtual pilo::i32_t on_run()
            {
                bool ret;
                for (size_t i = 0; i < m_loops; i++)
                {
                    ret = m_mutex->try_lock_for(100);
                    if (ret == false)
                    {
                        ;
                    }
                    else
                    {
                        return -2;
                    }

                }
                return  0;
            }


            pilo::u32_t m_loops;
            T* m_mutex;

        };

    }
}