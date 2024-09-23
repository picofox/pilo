#ifndef _pilo_core_process_environment_variable_manager_hpp_
#define _pilo_core_process_environment_variable_manager_hpp_

#include "process.hpp"
#include "../memory/util.hpp"
#include <map>
#include <vector>
#include "../../tlv.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {
            class environment_variable_manager
            {
            public:
                ~environment_variable_manager();
                ::pilo::err_t initialize();

                ::pilo::err_t insert(const char* key, ::pilo::i32_t key_len, const char* val, ::pilo::i32_t val_len);

                ::pilo::err_t set(const std::string& key, const std::vector<std::string> & vec);
                ::pilo::err_t append(const std::string& key, const std::vector<std::string>& vec);
                ::pilo::err_t remove(const std::string& key, const std::vector<std::string>& vec);
                ::pilo::err_t unset(const std::string& key);

                
            private:
                std::map<std::string, ::pilo::tlv*> _vars;
            };


        }
    }
}

#endif // !_pilo_core_process_environment_variable_manager_hpp_
