#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_io_buffer.hpp"
#include <algorithm>
#include "core/datetime/datetime.hpp"
#include "core/threading/nonrecursive_mutex.hpp"
#include "core/memory/io_buffer.hpp"
#include "core/memory/memory_pool.hpp"

using namespace pilo;


namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_io_bufferc_basic(void* param);


        pilo::test::testing_case g_functional_cases_io_buffer[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "Basic Func of io_buffer                       ", nullptr, functional_io_bufferc_basic, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_io_bufferc_basic(void* param)
        {
            M_UNUSED(param);

            char buffer[1024] = {0};
            for (int i = 0; i < 1024; i++)
            {
                buffer[i] = i %256;
            }

            pilo::core::memory::object_pool<pilo::core::memory::fixed_buffer_node<2048>, 128> pool;
            pilo::core::memory::io_buffer<2048> iob(3);
            iob.set_pool(pool);

            if (iob.size() != 0)
            {
                return -1;
            }

            size_t ret = iob.write(buffer,1024);
            if (ret != 1024)
            {
                return -10;
            }

   
            ret = iob.write(buffer, 1024);
            if (ret != 1024)
            {
                return -20;
            }
            if (iob.size() != 2048)
            {
                return -30;
            }

            ret = iob.write(buffer, 1024);
            if (ret != 1024)
            {
                return -40;
            }

            char* pBuf = (char*) calloc(1, 4096);
            ret = iob.read(pBuf, 4000);
            if (ret != 3072)
            {
                return -50;
            }

            for (int i = 0; i < (int) ret; i++)
            {
                if ((unsigned char) pBuf[i] != i % 256)
                {
                    return -100000-i;
                }
            }

            ret = iob.read(pBuf, 4000);
            if (ret != 0)
            {
                return -80;
            }

            free(pBuf);

            if (iob.size() != 0)
            {
                return -1;
            }

            pBuf = (char*)calloc(1, 6144);
            for (int i = 0; i < 6144; i++)
            {
                pBuf[i] = i % 256;
            }
            ret = iob.write(pBuf, 6144);
            if (ret != 6144)
            {
                return -70;
            }

            ret = iob.write("1", 1);
            if (ret != 0)
            {
                return -80;
            }

            ret = iob.write(buffer, 1024, true);
            if (ret != 1024)
            {
                return -80;
            }

            if (iob.size() != 6144+1024)
            {
                return -1;
            }

            free(pBuf);
            return 0;
        }



    }
}