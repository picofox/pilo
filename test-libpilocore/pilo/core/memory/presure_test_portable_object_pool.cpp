#include "presure_test_portable_object_pool.hpp"
#include "core/threading/basic_thread.hpp"

namespace pilo
{
    namespace test
    {
        

        ::pilo::error_number_t presure_test_portable_object_pool::test(char* cstr_arg, ::pilo::i32_t * i_arg, void* vp_arg)
        {
            M_UNUSED(cstr_arg); M_UNUSED(i_arg); M_UNUSED(vp_arg);

            ::pilo::i32_t ret = ::pilo::EC_OK;
            ret = m_consumer[0].execute();            
            if (ret != ::pilo::EC_OK)
            {
                return -1;
            }
            ret = m_consumer[1].execute();
            if (ret != ::pilo::EC_OK)
            {
                return -2;
            }

            ret = m_producer[0].execute();
            if (ret != ::pilo::EC_OK)
            {
                return -3;
            }
            ret = m_producer[1].execute();
            if (ret != ::pilo::EC_OK)
            {
                return -4;
            }

            bool has_error = false;          


            while (true)
            {
                ::pilo::core::io::console_format_output("Enter debug cmd: (r or s)\n");

                char buffer[1024] = {0};
                fgets(buffer, 1024, stdin);
                if (buffer[0] == 's')
                {
                    m_producer[0].request_advisory_termination(true);
                    m_producer[1].request_advisory_termination(true);
                    break;
                }
                else if (buffer[0] == 'r')
                {
                    reprot(nullptr, 0, nullptr);
                }
                
            }


            ret = m_producer[0].wait();
            if (ret != ::pilo::EC_OK)
            {
                has_error = true;
            }
            ret = m_producer[1].wait();
            if (ret != ::pilo::EC_OK)
            {
                has_error = true;
            }

            ::pilo::core::threading::basic_thread::msleep(1000);

            m_consumer[0].m_stop = true;
            m_consumer[1].m_stop = true;


            ret = m_consumer[0].wait();
            if (ret != ::pilo::EC_OK)
            {
                has_error = true;
            }
            ret = m_consumer[1].wait();
            if (ret != ::pilo::EC_OK)
            {
                has_error = true;
            }


            if (has_error)
            {
                return -6;
            }
            
            return 0;
        }

        ::pilo::error_number_t presure_test_portable_object_pool::terminate(char* cstr_arg, ::pilo::i32_t * i_arg, void* vp_arg)
        {
            M_UNUSED(cstr_arg); M_UNUSED(i_arg); M_UNUSED(vp_arg);

            return ::pilo::EC_OK;
        }

        ::pilo::error_number_t presure_test_portable_object_pool::reprot(char* cstr_arg, ::pilo::i32_t * i_arg, void* vp_arg)
        {
            M_UNUSED(cstr_arg); M_UNUSED(i_arg); M_UNUSED(vp_arg);

//             ::pilo::core::threading::mutex_locker<::pilo::core::threading::recursive_mutex> locker(m_mutex);
// 
//             ::pilo::core::io::console_format_output("P0 count: %llu\n", (::pilo::u64_t) m_producer[0].m_count);
//             ::pilo::core::io::console_format_output("P1 count: %llu\n", (::pilo::u64_t) m_producer[1].m_count);
//             ::pilo::core::io::console_format_output("C0 count: %llu\n", (::pilo::u64_t) m_consumer[0].m_count);
//             ::pilo::core::io::console_format_output("C1 count: %llu\n", (::pilo::u64_t) m_consumer[1].m_count);
// 
//             size_t total_p_count = m_producer[0].m_count + m_producer[1].m_count;
//             size_t total_c_count = m_consumer[0].m_count + m_consumer[1].m_count;
//             ::pilo::core::io::console_format_output("tp: %llu tc: %llu\n", (::pilo::u64_t) total_p_count, (::pilo::u64_t)total_c_count);
// 
//             const char* faster = nullptr;
//             if (total_p_count > total_c_count) faster = "P";
//             else if (total_p_count < total_c_count) faster = "C";
//             else
//                 faster = "None of P or C";



          //  ::pilo::core::io::console_format_output("%s is faster, delta count = %lld\n", faster,(::pilo::i64_t) (total_c_count - total_p_count));

           // std::string str = ::pilo::test::TestNpc0::pool()->make_report(MB_DEBUG_POOL_SHOW_AVAILIST_DETAIL);

           // ::pilo::core::io::console_format_output("%s", str.c_str());

            return ::pilo::EC_OK;
        }

        pilo::i32_t _object_pool_pruducer::on_run()
        {
            while (! termination_is_requested())
            {
                ::pilo::core::threading::basic_thread::msleep(1000);
                ::pilo::test::TestNpc0* obj = ::pilo::test::TestNpc0::allocate();
                if (obj == nullptr)
                {
                    M_ASSERT(false);
                    exit(-1);
                }

                if (_controller->enqueue(obj))
                {
                    m_count++;
                }
                else
                {
                    M_ASSERT(false);
                    exit(-1);
                }
            }

            return ::pilo::EC_OK;
        }

        pilo::i32_t _object_pool_consumer::on_run()
        {
            ::pilo::test::TestNpc0* obj = nullptr;
            
            while (true)
            {
                obj = _controller->dequeue();
                if (obj != nullptr)
                {  
                    m_count++;
                    ::pilo::test::TestNpc0::deallocate(obj);
                }
                else
                {
                    if (m_stop) return pilo::EC_OK;
                }
            }

        }

    }
}