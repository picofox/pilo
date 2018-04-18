#include "testing_module.hpp"
#include "core/io/format_output.hpp"

namespace pilo
{
    namespace test
    {
        testing_module::testing_module(pilo::i32_t id, const char* name, testing_case_t* cases)
        {
            M_ASSERT(cases != nullptr);

            _m_id = id;
            _m_name = name == nullptr? "" : name;

            size_t i = 0;

            while (true)
            {
                testing_case_t * caseParam = &cases[i];
                M_ASSERT(caseParam != nullptr);

                if (caseParam->m_id == -1 || caseParam->m_function == nullptr)
                {
                    break;
                }

                i++;

                _m_data.push_back(*caseParam);
            }
        }

        pilo::i32_t testing_module::run_cases(bool break_on_error)
        {
            pilo::i32_t ret = _initialize_module();
            if (pilo::EC_OK != ret)
            {
                return pilo::EC_INITIALIZE_FAILED;
            }

            _m_failed_count = 0;

            size_t count = _m_data.size();
            for (size_t i = 0; i < count; i++)
            {
                M_ASSERT(_m_data[i].m_function != nullptr);
                _m_data[i].m_result = _m_data[i].m_function(&_m_data[i]);
                if (_m_data[i].m_result != 0)
                {
                    _m_failed_count ++;
                    if (break_on_error)
                    {
                        M_ASSERT(false);
                    }
                }
            }

            if (_m_failed_count > 0)
            {
                ret = pilo::EC_PARTIAL;
            }

            if (pilo::EC_OK != _finalize_module())
            {
                return pilo::EC_FINALIZE_FAILED;
            }

            return 0;
        }

        void testing_module::console_output() const
        {
            pilo::core::io::console_format_output("Testing Component_%u [%s]\n", _m_id, _m_name.c_str());

            size_t count = _m_data.size();
            for (size_t i = 0; i < count; i++)
            {
                const testing_case_t * caseParam = &_m_data[i];
                M_ASSERT(caseParam != nullptr);

                if (caseParam->m_id == -1 || caseParam->m_function == nullptr)
                {
                    break;
                }

                pilo::core::io::console_format_output(" #[%03u.%03d]: Testing %s", \
                    _m_id, caseParam->m_id, caseParam->m_decription);

                if (caseParam->m_result == 0)
                {
                    pilo::core::io::console_format_output(" [PASSED]\n");
                }
                else
                {
                    pilo::core::io::console_format_output(" [FAILED:%d]\n", caseParam->m_result);
                }
            }

            pilo::core::io::console_format_output("Result ==> total %u, failed %u\n\n", _m_data.size(), _m_failed_count);
        }

    }
}