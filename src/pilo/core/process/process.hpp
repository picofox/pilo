#ifndef _process_hpp_
#define _process_hpp_

#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            os_pid_t current_process_id();
            os_pid_t parent_process_id(os_pid_t);
            os_pid_t current_parent_process_id();
        }
    }
}


#endif