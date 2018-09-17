#pragma once

#include "../../testing_module.hpp"
#include "core/threading/basic_thread.hpp"

namespace pilo
{
    namespace test
    {
        class c_test_100threads_accumulation_accuracy : public pilo::core::threading::basic_thread
        {
        public:
            virtual pilo::i32_t on_run(); 

        };

        class functional_test_module_class_basic_thread : public pilo::test::testing_module
        {
        public:
            functional_test_module_class_basic_thread(pilo::i32_t id, const char* name, testing_case_t* cases) : testing_module(id, name, cases) {}
        };
    }
}