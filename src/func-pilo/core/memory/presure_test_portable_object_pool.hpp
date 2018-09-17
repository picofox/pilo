#pragma once
#include "presure_test.hpp"
#include "core/threading/basic_thread.hpp"
#include "core/threading/recursive_mutex.hpp"
#include "core/threading/mutex_locker.hpp"
#include "core/threading/sync_condition_variable_any.hpp"
#include <queue>

namespace pilo
{
    namespace test
    {     
        class presure_test_portable_object_pool;

        class _object_pool_pruducer : public ::pilo::core::threading::basic_thread
        {
        public:
            _object_pool_pruducer()
            {
                m_count = 0;
                _controller = nullptr;
            }
            virtual i32_t on_run();
           

            presure_test_portable_object_pool* _controller;
            size_t          m_count;
        };

        class _object_pool_consumer : public ::pilo::core::threading::basic_thread
        {
        public:
            _object_pool_consumer()
            {
                m_count = 0;
                _controller = nullptr;
                m_stop = false;
            }
            virtual i32_t on_run();

            presure_test_portable_object_pool* _controller;
            size_t          m_count;
            bool            m_stop;
        };


        class presure_test_portable_object_pool : public presure_test
        {
        public:
            presure_test_portable_object_pool()
            {
                m_consumer[0]._controller = this;
                m_consumer[1]._controller = this;
                m_producer[0]._controller = this;
                m_producer[1]._controller = this;
            }


            virtual ::pilo::error_number_t test(char* cstr_arg, ::pilo::i32_t * i_arg, void* vp_arg);
            virtual ::pilo::error_number_t terminate(char* cstr_arg, ::pilo::i32_t * i_arg, void* vp_arg);
            virtual ::pilo::error_number_t reprot(char* cstr_arg, ::pilo::i32_t * i_arg, void* vp_arg);

        public:
            
            ::pilo::test::TestNpc0*  dequeue()
            {
                ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> locker(m_mutex);

                if (m_queue.empty()) return nullptr;

                ::pilo::test::TestNpc0* obj = m_queue.front();
                if (obj == nullptr) return nullptr;
                m_queue.pop();

                if (!obj->Valid())
                {
                    M_ASSERT(false);
                    exit(-1);
                }

                return obj;
            }

            bool enqueue(::pilo::test::TestNpc0* obj)
            {
                ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> locker(m_mutex);

                try
                {
                    m_queue.push(obj);
                }
                catch (...)
                {
                    return false;
                }

                return true;                
            }

            
        private:
            
            _object_pool_pruducer           m_producer[2];
            _object_pool_consumer           m_consumer[2];

            ::pilo::core::threading::recursive_mutex m_mutex;
            std::queue<::pilo::test::TestNpc0*> m_queue;

        };
    }
}