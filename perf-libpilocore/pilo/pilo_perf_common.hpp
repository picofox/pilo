#pragma once
#include "core/coredefs.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/threading/basic_thread.hpp"
#include "core/datetime/datetime.hpp"

#if (M_CXX_FEATURE_TIME)
#include <chrono>
#endif

namespace pilo
{
    namespace test
    {
        class perf_desc
        {
            public:
            perf_desc()
            {
                reset();
            }
            ~perf_desc()
            {

            }

            void reset()
            {
                m_name.clear();
                m_result.clear();
                m_error.clear();
            }

            void reset(const char* name)
            {
                m_name = name;
                m_result.clear();
                m_error.clear();
            }

            void set_name(const char* str) { m_name = str; }
            const char* name() const { return m_name.c_str(); }

            void set_error(const char* str) { m_error = str; }
            const char* error() const { return m_error.c_str(); }

            void add_result(const char* str)
            {
                m_result += "\t";
                m_result += str;
                m_result += "\n";

            }
            const char* result() const { return m_result.c_str(); }
            private:
            ::pilo::core::string::fixed_astring<64> m_name;
            ::pilo::core::string::fixed_astring<128> m_error;
            ::pilo::core::string::fixed_astring<4096> m_result;

        };

        template<typename LOCK>
        class perf_test_mtx : public ::pilo::core::threading::basic_thread
        {
        public:
            perf_test_mtx()
            {
                m_failed_times = 0;
            }

            void reset(::pilo::u64_t times, ::pilo::u64_t* v, LOCK* ml)
            {
                m_times = times;
                m_value_ptr = v;  
                m_mutex = ml;
                m_failed_times = 0;
            }

            virtual i32_t on_run()
            {
                for (::pilo::u32_t i = 0; i < m_times; i++)
                {
                    m_mutex->lock();
                    (*m_value_ptr)++;
                    m_mutex->unlock();
                }

                return 0;
            }
                
            
            LOCK*            m_mutex;
            ::pilo::u64_t    m_times;
            ::pilo::u64_t*   m_value_ptr; 
            ::pilo::u64_t   m_failed_times;
        };

        template<typename LOCK>
        class perf_test_mtx_try : public ::pilo::core::threading::basic_thread
        {
            public:
            perf_test_mtx_try()
            {
                m_failed_times = 0;
            }

            void reset(::pilo::u64_t times, ::pilo::u64_t* v, LOCK* ml)
            {
                m_times = times;
                m_value_ptr = v;
                m_mutex = ml;
                m_failed_times = 0;
            }

            virtual i32_t on_run()
            {
                for (::pilo::u32_t i = 0; i < m_times; i++)
                {
                    bool bRet = false;
                    do
                    {
                        bRet = m_mutex->try_lock();
                        if (!bRet)
                        {
//                             if (i % 5 == 0)
//                             {
//                                 ::pilo::core::threading::basic_thread::msleep(1);
//                             }
                        }

                    } while (!bRet);
                    (*m_value_ptr)++;

                    m_mutex->unlock();
                }

                return 0;
            }


            LOCK*            m_mutex;
            ::pilo::u64_t    m_times;
            ::pilo::u64_t*   m_value_ptr;
            ::pilo::u64_t   m_failed_times;
        };

#if (M_CXX_FEATURE_TIME)
        template<typename LOCK>
        class perf_test_time_mtx_for_chrono : public ::pilo::core::threading::basic_thread
        {
            public:
            perf_test_time_mtx_for_chrono()
            {
                m_failed_times = 0;
                m_done_count = 0;
            }

            void reset(::pilo::u32_t times, ::pilo::u64_t* v, LOCK* ml)
            {
                m_times = times;
                m_value_ptr = v;
                m_mutex = ml;
                m_done_count = 0;
                m_failed_times = 0;
            }

            virtual i32_t on_run()
            {
               // printf("Child %u: start 0\n", this->id());

                for (::pilo::u32_t i = 0; i < m_times; i++)
                {  
                    bool bRet = false;
                    do 
                    {
                        //time_t t = time(NULL);
/*                        std::chrono::time_point<std::chrono::system_clock> then_tp = std::chrono::system_clock::from_time_t(t + 2);*/

                        std::chrono::milliseconds m(2000);

                        bRet = (m_mutex->try_lock_for(m));
                        if (!bRet) m_failed_times ++;               
                        
                    } while (! bRet);
                    (*m_value_ptr)++;
                    m_done_count ++;
                    m_mutex->unlock();                    
                }

               // printf("Child %u: return 0\n", this->id());
                return 0;
            }
    
            


            LOCK*            m_mutex;
            ::pilo::u32_t   m_times;
            ::pilo::u64_t*   m_value_ptr;
            ::pilo::u64_t   m_failed_times;
            ::pilo::u64_t   m_done_count;
        };

#endif

        template<typename LOCK>
        class perf_test_time_mtx_for_milli : public ::pilo::core::threading::basic_thread
        {
            public:
            perf_test_time_mtx_for_milli()
            {
                m_failed_times = 0;
                m_done_count = 0;
            }

            void reset(::pilo::u32_t times, ::pilo::u64_t* v, LOCK* ml)
            {
                m_times = times;
                m_value_ptr = v;
                m_mutex = ml;
                m_done_count = 0;
                m_failed_times = 0;
            }

            virtual i32_t on_run()
            {
                // printf("Child %u: start 0\n", this->id());

                for (::pilo::u32_t i = 0; i < m_times; i++)
                {
                    bool bRet = false;
                    do
                    {
                        bRet = (m_mutex->try_lock_for(2000));
                        if (!bRet) m_failed_times++;

                    } while (!bRet);
                    (*m_value_ptr)++;
                    m_done_count++;
                    m_mutex->unlock();
                }

                // printf("Child %u: return 0\n", this->id());
                return 0;
            }

            LOCK*            m_mutex;
            ::pilo::u32_t   m_times;
            ::pilo::u64_t*   m_value_ptr;
            ::pilo::u64_t   m_failed_times;
            ::pilo::u64_t   m_done_count;
        };

#if (M_CXX_FEATURE_TIME)
        template<typename LOCK>
        class perf_test_time_mtx_until_chrono : public ::pilo::core::threading::basic_thread
        {
            public:
            perf_test_time_mtx_until_chrono()
            {
                m_failed_times = 0;
                m_done_count = 0;
            }

            void reset(::pilo::u32_t times, ::pilo::u64_t* v, LOCK* ml)
            {
                m_times = times;
                m_value_ptr = v;
                m_mutex = ml;
                m_done_count = 0;
                m_failed_times = 0;
            }

            virtual i32_t on_run()
            {

                for (::pilo::u32_t i = 0; i < m_times; i++)
                {
                    bool bRet = false;
                    do
                    {                        
                        bRet = (m_mutex->try_lock_until(std::chrono::system_clock::now() + std::chrono::milliseconds(2000)));
                        if (!bRet) m_failed_times++;

                    } while (!bRet);
                    (*m_value_ptr)++;
                    m_done_count++;
                    m_mutex->unlock();
                }

                // printf("Child %u: return 0\n", this->id());
                return 0;
            }

            LOCK*            m_mutex;
            ::pilo::u32_t   m_times;
            ::pilo::u64_t*   m_value_ptr;
            ::pilo::u64_t   m_failed_times;
            ::pilo::u64_t   m_done_count;
        };

#endif
        template<typename LOCK>
        class perf_test_time_mtx_until_secs : public ::pilo::core::threading::basic_thread
        {
            public:
            perf_test_time_mtx_until_secs()
            {
                m_failed_times = 0;
                m_done_count = 0;
            }

            void reset(::pilo::u32_t times, ::pilo::u64_t* v, LOCK* ml)
            {
                m_times = times;
                m_value_ptr = v;
                m_mutex = ml;
                m_done_count = 0;
                m_failed_times = 0;
            }

            virtual i32_t on_run()
            {

                for (::pilo::u32_t i = 0; i < m_times; i++)
                {
                    bool bRet = false;
                    do
                    {
                        bRet = (m_mutex->try_lock_until(time(NULL) + 2 ));
                        if (!bRet) m_failed_times++;

                    } while (!bRet);
                    (*m_value_ptr)++;
                    m_done_count++;
                    m_mutex->unlock();
                }

                // printf("Child %u: return 0\n", this->id());
                return 0;
            }

            LOCK*            m_mutex;
            ::pilo::u32_t   m_times;
            ::pilo::u64_t*   m_value_ptr;
            ::pilo::u64_t   m_failed_times;
            ::pilo::u64_t   m_done_count;
        };

        class perf_TickCount : public ::pilo::core::threading::basic_thread
        {
        public:
            perf_TickCount()
            {
                m_savedLastTick = 0;
            }

            virtual i32_t on_run()
            {

                for (::pilo::u32_t i = 0; i < UINT64_MAX; i++)
                {
                    ::pilo::u64_t tc_neo = PiloGetTickCount64();
                    if (tc_neo < m_savedLastTick)
                    {
                        M_ASSERT(false && "tickcount logic error");
                    }      
            
                    m_savedLastTick = tc_neo;              
                }

                // printf("Child %u: return 0\n", this->id());
                return 0;
            }

            ::pilo::u64_t   m_savedLastTick;
            
        };

 //       ::pilo::error_number_t perf_tickcount();
        ::pilo::error_number_t perf_now_ticks();
        ::pilo::error_number_t perf_steady_now_ticks();

        ::pilo::error_number_t perf_nonrecursive_mutex();
        ::pilo::error_number_t perf_nonrecursive_mutex_try();

        ::pilo::error_number_t perf_nonrecursively_timed_mutex();
        ::pilo::error_number_t perf_nonrecursively_timed_mutex_try_lock_until();
        ::pilo::error_number_t perf_nonrecursively_timed_mutex_try_lock_for();
        ::pilo::error_number_t perf_nonrecursively_timed_mutex_try();

        ::pilo::error_number_t perf_recursively_mutex();
        ::pilo::error_number_t perf_recursively_mutex_try();

        ::pilo::error_number_t perf_recursively_timed_mutex_try_lock_for();
        ::pilo::error_number_t perf_recursively_timed_mutex_try_lock_until();
        ::pilo::error_number_t perf_recursively_timed_mutex();
        ::pilo::error_number_t perf_recursively_timed_mutex_try();

        
        

        
    }  
}
