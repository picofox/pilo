#ifndef __pilo_io_path_hpp_
#define __pilo_io_path_hpp_

#include <string>
#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include "./formatted_io.hpp"
#include "../threading/native_mutex.hpp"
#include "../memory/compactable_autoreset_object_pool.hpp"

#define PMI_STC_PARAM_PATH_STEP_SIZE    (510)
#define PMI_PATH_MAX_SIZE (65535)

namespace pilo
{
    namespace core
    {
        namespace io
        {
            class path
                : public ::pilo::core::memory::portable_compactable_autoreset_object_pool<path, PMI_STC_PARAM_PATH_STEP_SIZE, ::pilo::core::threading::native_mutex>
            {
            public: //static methods
                char* get_current_working_directory(char* buffer, ::pilo::i64_t cc_buffer, ::pilo::pathlen_t* out_length, ::pilo::i32_t endsep_mode, ::pilo::pathlen_t extra_space = 0); 

            };
        }
    }
}



#endif //__pilo_io_path_hpp_