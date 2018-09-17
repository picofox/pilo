#include "core/string/string_util.hpp"
#include "core/string/fixed_astring.hpp"
#include "core/io/format_output.hpp"
#include "functional_test_module_class_dynamic_buffer.hpp"
#include <algorithm>
#include "core/datetime/datetime.hpp"
#include "core/threading/nonrecursive_mutex.hpp"
#include "core/memory/io_buffer.hpp"
#include "core/memory/dynamic_buffer.hpp"

using namespace pilo;


namespace pilo
{
    namespace test
    {
        static pilo::i32_t functional_dynamic_bufferc_basic(void* param);


        pilo::test::testing_case g_functional_cases_dynamic_buffer[] =
        {
            /*---"----------------------------------------------"*/
            { 1, "Basic Func of dynamic_buffer                  ", nullptr, functional_dynamic_bufferc_basic, 0, -1, (pilo::u32_t) - 1 },


            { -1, "end", nullptr, nullptr, 0, -1, 0 },
        };


        pilo::i32_t functional_dynamic_bufferc_basic(void* param)
        {
            M_UNUSED(param);
            ::pilo::core::memory::dynamic_buffer<> buffer0;

            if (buffer0.capacity() != 0)
            {
                return -1;
            }
            if (buffer0.data() != nullptr)
            {
                return -3;
            }
            if (buffer0.available_bytes_for_reading() != 0)
            {
                return -4;
            }
            if (buffer0.available_bytes_for_writing() != 0)
            {
                return -5;
            }
            if (buffer0.read_buffer() != nullptr)
            {
                return -6;
            }
            if (buffer0.write_buffer() != nullptr)
            {
                return -6;
            }

            ::pilo::core::memory::dynamic_buffer<1024> buffer1;
            if (buffer1.capacity() != 1024)
            {
                return -11;
            }
            if (buffer1.data() == nullptr)
            {
                return -13;
            }
            if (buffer1.available_bytes_for_reading() != 0)
            {
                return -14;
            }
            if (buffer1.available_bytes_for_writing() != 1024)
            {
                return -15;
            }
            if (buffer1.read_buffer() == nullptr)
            {
                return -16;
            }
            if (buffer1.write_buffer() == nullptr)
            {
                return -16;
            }

            for (int i = 0; i < 1024; i++)
            {
                buffer1[i] = (char) (i % 256);
            }
            buffer1.set_rp(300);
            buffer1.set_wp(400);

            buffer0.assign(buffer1);
            if (buffer0.capacity() != 1024)
            {
                return -21;
            }
            if (buffer0.data() == nullptr)
            {
                return -23;
            }
            if (buffer0.available_bytes_for_reading() != 100)
            {
                return -24;
            }
            if (buffer0.available_bytes_for_writing() != 1024-400)
            {
                return -25;
            }
            if (buffer0.read_buffer() == nullptr)
            {
                return -26;
            }
            if (buffer0.write_buffer() == nullptr)
            {
                return -27;
            }



            return 0;
        }



    }
}