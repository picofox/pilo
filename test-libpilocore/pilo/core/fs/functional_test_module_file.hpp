#pragma once

#include "../../testing_module.hpp"

namespace pilo
{
    namespace test
    {
        class functional_test_module_file : public pilo::test::testing_module
        {
        public:
            functional_test_module_file(pilo::i32_t id, const char* name, testing_case_t* cases) : testing_module(id, name, cases) {}
        };
    }
}