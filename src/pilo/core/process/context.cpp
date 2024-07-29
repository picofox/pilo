#include "./context.hpp"
#include "../stat/system_information.hpp"
#include "../../tlv.hpp"
#include "../memory/linked_byte_buffer.hpp"
#include "../string/fixed_width_line_formater.hpp"
#include "process.hpp"

namespace pilo
{
    namespace core
    {
        namespace process
        {

#ifdef WINDOWS
            char* xpf_get_proc_name(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t * rlen)
            {
                char tmp_buffer[PMI_PATH_MAX] = { 0 };
                ::pilo::u32_t len = GetModuleFileName(NULL, tmp_buffer, sizeof(tmp_buffer));
                ::pilo::char_buffer_t   cb(buffer, bufsz, 0, false);

                if (len > 0) {
                    cb.check_space(len + 1);
                    const char* lastSlash = ::pilo::core::string::rfind_char(tmp_buffer,  len, '\\');
                    if (lastSlash != nullptr) {
                        ::pilo::core::string::copyz(cb.begin(), cb.capacity(), lastSlash + 1);
                        ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t) len - (::pilo::i32_t)(lastSlash - tmp_buffer + 1));
                        return cb.begin();
                    }
                    else {
                        ::pilo::core::string::copyz(cb.begin(), cb.capacity(), tmp_buffer);
                        ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t) len);
                        return cb.begin();
                    }
                }
                else {
                    return nullptr;
                }

            }
#else
            extern char* __progname;

            char* xpf_get_proc_name(char* buffer, ::pilo::i32_t bufsz, ::pilo::i32_t* rlen)
            {
                char filename[PMI_PATH_MAX] = {0};
                ::pilo::char_buffer_t   cb(buffer, bufsz, 0, false);
                int fd = -1;
                fd = open("/proc/self/comm", O_RDONLY);
                ssize_t n = ::read(fd, filename, sizeof(filename));
                if (n < 0) {
                    ::close(fd);
                    return nullptr;
                }
                int len = (int) ::pilo::core::string::character_count(filename);
                cb.check_space(len + 1);
                ::pilo::core::string::copyz(cb.begin(), cb.capacity(), filename);
                ::pilo::set_if_ptr_is_not_null(rlen, (::pilo::i32_t) len);
                return cb.begin();
            }
#endif

            static void s_on_exit(void)
            {
                printf("pilo on exit, finanlizing.....\n");

                PILO_CONTEXT->finalize();
            }
            context::context()
            {
                char buffer[PMI_PATH_MAX] = { 0 };
                ::pilo::i32_t rlen = 0;

                _pid = ::pilo::core::process::current_process_id();
                _ppid = ::pilo::core::process::current_parent_process_id();
               
                char* cret = xpf_get_proc_name(buffer, sizeof(buffer), &rlen);
                if (cret != nullptr) {
                    _proc_name.assign(cret, rlen);
                    size_t pos = _proc_name.rfind('.');
                    if (pos != std::string::npos) {
                        _proc_basename = _proc_name.substr(0, pos);
                    }
                    if (cret != nullptr && cret != buffer) {
                        PMF_HEAP_FREE(cret);
                    }

                } else {
                    _proc_name.assign("unknow_proc", 11);
                    _proc_basename = _proc_name;
                }

                    

                _page_pool = new ::pilo::core::memory::dynamic_memory_pool<::pilo::core::threading::spin_mutex>(PMSO_SYSTEM_INFORMATION->page_size(), 1024);


            }

            context::~context()
            {
                s_on_exit();
            }

            std::string context::startup_info() const
            {
                char buffer[64] = {0};
                std::stringstream ss;
                ::pilo::core::string::fixed_width_line_formater formater;


                formater.add_meta_field(16, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Item ");
                formater.add_meta_field(56, PMI_FIXED_WIDTH_LINE_FMT_LEFT_ALIGH, "Info");
                formater.format_header(ss);

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Process:");
                formater.format_field(ss, 1, this->process_basename());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t) 0, (const char*) "PILO Version:");
                ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%u.%u.%u.%s"
                                                         , major_version(), minor_version(), revision(), stage_cstr());
                formater.format_field(ss, 1, (const char *)buffer);
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t) 0, (const char*) "Executable");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_dir_enum::exe).fullpath());
                ss << std::endl;

                ::pilo::core::io::string_formated_output(buffer, sizeof(buffer), "%d (%d)", _pid, _ppid);
                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"IDS");
                formater.format_field(ss, 1, (const char*) buffer);
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"CWD");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_dir_enum::cwd).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Home");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_dir_enum::home).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Bin");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_dir_enum::bin).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Cnf");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_dir_enum::cnf).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Log");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_dir_enum::log).fullpath());
                ss << std::endl;

                formater.format_field(ss, (::pilo::i64_t)0, (const char*)"Tmp");
                formater.format_field(ss, 1, proc_path(::pilo::predefined_pilo_dir_enum::tmp).fullpath());
                ss << std::endl;


                return ss.str();
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
                err = this->_proc_paths[(int) ::pilo::predefined_pilo_dir_enum::cwd].fill_with_cwd(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_dir_enum::exe].fill_with_exe(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_dir_enum::bin].fill_with_bin(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_dir_enum::home].fill_with_home(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_dir_enum::cnf].fill_with_cnf(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_dir_enum::log].fill_with_log(0);
                if (err != PILO_OK)
                    return err;

                err = this->_proc_paths[(int) ::pilo::predefined_pilo_dir_enum::tmp].fill_with_tmp(0);
                if (err != PILO_OK)
                    return err;


                std::string si = startup_info();                
                printf("%s\n",si.c_str());

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