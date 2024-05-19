#include "./context.hpp"
#include "../stat/system_information.hpp"
#include "../../tlv.hpp"
#include "../memory/linked_byte_buffer.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {

            static void s_on_exit(void)
            {
                printf("pilo on exit, finanlizing.....\n");

                PILO_CONTEXT->finalize();
            }
            context::context()
            {

               // atexit(s_on_exit);
                _page_pool = new ::pilo::core::memory::dynamic_memory_pool<::pilo::core::threading::spin_mutex>(PMSO_SYSTEM_INFORMATION->page_size(), 1024);
            }

            context::~context()
            {
                s_on_exit();
            }

            ::pilo::i32_t context::initialize()
            {
                _pool_object_stat_mgr.register_item(::pilo::core::stat::pool_object_stat_manager::pool_object_key_code::key_tlv
                    , sizeof(::pilo::tlv), [](::pilo::core::stat::pool_object_stat_manager::pool_object_key_code 
                        , ::pilo::core::stat::pool_object_stat_manager::stat_item* si) -> ::pilo::err_t { return ::pilo::tlv::update_pool_object_stat(si);}
                    , "pilo_tlv"
                );
                _pool_object_stat_mgr.register_item(::pilo::core::stat::pool_object_stat_manager::pool_object_key_code::local_buffer_node
                    , sizeof(::pilo::core::memory::linked_byte_buffer<4096, 1024, false>::buffer_node), [](::pilo::core::stat::pool_object_stat_manager::pool_object_key_code
                        , ::pilo::core::stat::pool_object_stat_manager::stat_item* si) -> ::pilo::err_t { return  ::pilo::core::memory::linked_byte_buffer<4096, 1024, false>::buffer_node::update_pool_object_stat(si); }
                    , "local_bn"
                );

                ::pilo::err_t err = PILO_OK;
                err = ::pilo::core::io::path::get_executable_path(& (this->_proc_paths[(int)path_types::exe]));
                if (err != PILO_OK)
                    return err;

                std::string bin_str = this->_proc_paths[(int)path_types::exe].parentpathname();
                err = (this->_proc_paths[(int)path_types::bin].set(true, bin_str.c_str(), bin_str.size(), 0));
                if (err != PILO_OK)
                    return err;

                std::string par_str = this->_proc_paths[(int)path_types::bin].parentpathname();
                err = (this->_proc_paths[(int)path_types::parent].set(true, par_str.c_str(), par_str.size(), 0));
                if (err != PILO_OK)
                    return err;

                return PILO_OK;
            }

            void context::finalize()
            {
                std::string str = this->_pool_object_stat_mgr.to_string();
                std::cout << str << std::endl;
            }

           
        }
    }
}