#pragma once
#include "core/coredefs.hpp"
#include "../../sample_data.hpp"
#include "core/datetime/datetime.hpp"

namespace pilo
{
    namespace test
    {
        class presure_test
        {
        public:
            virtual ::pilo::error_number_t test(char* cstr_arg, ::pilo::i32_t * i_arg, void* vp_arg) = 0;
            virtual ::pilo::error_number_t terminate(char* cstr_arg, ::pilo::i32_t * i_arg, void* vp_arg)  = 0;
            virtual ::pilo::error_number_t reprot(char* cstr_arg, ::pilo::i32_t * i_arg, void* vp_arg) = 0;

        public:
            void set_started_timestamp() { _started_timestamp = PiloGetTickCount64();  }
            void set_stopped_timestamp() { _stopped_timestamp = PiloGetTickCount64(); }

        private:
            ::pilo::tick64_t    _started_timestamp;
            ::pilo::tick64_t    _stopped_timestamp;
        };
    }
}