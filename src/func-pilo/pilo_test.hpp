#pragma once

#include "core/coredefs.hpp"



namespace pilo
{
    namespace test
    {
        typedef pilo::i32_t(*cb_test_func_t)(void * param);

#define MB_TESTING_NEED_TIME_COUNT           0x1

        typedef struct testing_case
        {
            pilo::i32_t         m_id;
            const char *        m_decription;
            void*               m_arg;
            cb_test_func_t      m_function;
            pilo::u32_t         m_flags;
            pilo::i32_t         m_result;
            pilo::u32_t         m_ms_exhausted;
        } testing_case_t;

        extern pilo::test::testing_case g_functional_cases_path[];
        extern pilo::test::testing_case g_functional_cases_string_util[];
        extern pilo::test::testing_case g_functional_cases_fixed_astring[];
        extern pilo::test::testing_case g_functional_cases_fixed_wstring[];
        extern pilo::test::testing_case g_functional_cases_basic_thread[];
        extern pilo::test::testing_case g_functional_cases_nonrecursive_timed_mutex[];
        extern pilo::test::testing_case g_functional_cases_nonrecursive_mutex[];
        extern pilo::test::testing_case g_functional_cases_recursive_timed_mutex[];
        extern pilo::test::testing_case g_functional_cases_recursive_mutex[];
        extern pilo::test::testing_case g_functional_cases_fixed_array[];
        extern pilo::test::testing_case g_functional_cases_fixed_bin_heap[];
        extern pilo::test::testing_case g_functional_cases_fixed_memory_stream[];
        extern pilo::test::testing_case g_functional_cases_singly_linked_selflist[];
        extern pilo::test::testing_case g_functional_cases_overlapped_memory_pool[];
        extern pilo::test::testing_case g_functional_cases_memory_pool[];
        extern pilo::test::testing_case g_functional_cases_compactable_memory_pool[];
        extern pilo::test::testing_case g_functional_cases_compactable_overlapped_memory_pool[];
        extern pilo::test::testing_case g_functional_cases_io_buffer[];
        extern pilo::test::testing_case g_functional_cases_datetime[];
        extern pilo::test::testing_case g_functional_cases_fs_util[];
        extern pilo::test::testing_case g_functional_cases_dynamic_buffer[];
        extern pilo::test::testing_case g_functional_cases_file[];
        extern pilo::test::testing_case g_functional_cases_portable_object_pool[];
        
    }
}
