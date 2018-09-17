#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_portable_object_pool.hpp"
#include <algorithm>
#include "core/memory/compactable_memory_pool.hpp"
#include "core/datetime/datetime.hpp"
#include "core/threading/nonrecursive_mutex.hpp"
#include "../../sample_data.hpp"

using namespace pilo;
//#define TEST_UNIT_IN_POOL_NUM (1024*1024)

#define TEST_UNIT_IN_POOL_NUM (1024*1024)
#define TEST_UNIT_STEP          (1024)



namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_compactable_portable_objct_pool_basic(void* param);
        //static pilo::i32_t functional_compactable_memory_pool_mt(void* param);


        pilo::test::testing_case g_functional_cases_portable_object_pool[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "Basic Functions of portable object pool       ", nullptr, functional_compactable_portable_objct_pool_basic, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_compactable_portable_objct_pool_basic(void* param)
        {
            M_UNUSED(param);  

            TestNpc0** ppnpc = new TestNpc0* [99];

            for (int i = 0; i < 99; i++)
            {  

                ppnpc[i] = ::pilo::test::TestNpc0::allocate();

                if (ppnpc[i] == nullptr) return -1;

                if (!ppnpc[i]->Valid())
                {
                    return -2;
                }                

               // 
                
            }

            for (int i = 0; i < 99; i++)
            {
                ::pilo::test::TestNpc0::deallocate(ppnpc[i]);
            }

//             std::string str = ::pilo::test::TestNpc0::pool()->report(
//                 MB_DEBUG_POOL_SHOW_FREELIST_DETAIL | 
//                 MB_DEBUG_POOL_SHOW_AVAILIST_DETAIL |
//                 MB_DEBUG_POOL_SHOW_FULLLIST_DETAIL |
//                 MB_DEBUG_POOL_SHOW_UNITARRY_DETAIL, 0);

//             std::string str = ::pilo::test::TestNpc0::pool()->make_report(MB_DEBUG_POOL_SHOW_AVAILIST_DETAIL, 0);
//             ::pilo::core::io::console_format_output("%s", str.c_str());

            return 0;
        }


    }
}

