#pragma once

#include "core/coredefs.hpp"
#include "pilo_test.hpp"
#include <vector>
#include <string>
#include "core/string/fixed_astring.hpp"

namespace pilo
{
    namespace test
    {     
        class testing_module
        {
        public:
            testing_module(pilo::i32_t id, const char* name, testing_case_t* cases);
            virtual ~testing_module(){}

            pilo::i32_t id() const { return _m_id; }
            const std::string& name() const { return _m_name; }
            size_t total_count() const { return _m_data.size(); }
            size_t failed_count() const { return _m_failed_count; }
            
            virtual pilo::i32_t run_cases(bool break_on_error);
            virtual void console_output() const;
            
        protected:
            virtual pilo::i32_t _finalize_module()   { return 0; }
            virtual pilo::i32_t _initialize_module() { return 0; }


        protected:
            pilo::i32_t                 _m_id;
            std::vector<testing_case_t> _m_data;
            std::string                 _m_name;  
            size_t                      _m_failed_count;
        };
        
        class testing_pod_info
        {
        public:

            bool operator<(const pilo::test::testing_pod_info& o) const
            {
                return (m_id < o.m_id);
            }

            friend bool operator==(const testing_pod_info& l, const testing_pod_info& r)
            {
                if (l.m_id == r.m_id)
                {
                    return true;
                }

                return false;
            }

            testing_pod_info()
            {
                m_id = -1;
            }

            testing_pod_info(pilo::i32_t id, const char* name)
            {
                m_id = id;
                m_name = name;
            }

            pilo::i32_t                                 m_id;
            pilo::core::string::fixed_astring<128>      m_name;
        };  

        

    }
}

DECLARE_SIMPLE_TYPE(pilo::test::testing_pod_info);

