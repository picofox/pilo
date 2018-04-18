#pragma once
#include "./process.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            class process_util
            {
            public:
                enum EnumProcessPrivilegeName
                {
                    ePPN_Backup,
                    ePPN_Restore,
                    ePPN_ChangeNotify,
                    ePPN_Count,
                };

                static ::pilo::i32_t get_parent_process_id(::pilo::os_process_id_t& ppid);
                static ::pilo::os_process_id_t current_process_id();
                static ::pilo::error_number_t set_current_process_privilege(EnumProcessPrivilegeName eWhich, bool enable);

            private:       
            };
        }
    }
}