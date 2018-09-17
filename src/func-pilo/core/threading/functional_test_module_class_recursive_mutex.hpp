#pragma once

#include "./functional_test_modules_threading_common.hpp"


namespace pilo
{
    namespace test
    {

        class functional_test_module_class_recursive_mutex : public pilo::test::testing_module
        {
        public:
            functional_test_module_class_recursive_mutex(pilo::i32_t id, const char* name, testing_case_t* cases) : testing_module(id, name, cases) {}
        };
    }
}