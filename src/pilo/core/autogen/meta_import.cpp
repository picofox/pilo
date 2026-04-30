#include    "./meta_import.hpp"
#include    <sstream>
#include    "../string/string_operation.hpp"


namespace pilo
{
    namespace core
    {
        namespace autogen
        {
            meta_import::~meta_import()
            {
            }

            ::pilo::err_t meta_import::append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, ::pilo::i16_t effect_indent ) const
            {
                PMC_UNUSED(strparam);
                
                if (((flags & oflag_dec) != 0) !=  _m_in_dec ){
                    return PILO_OK;
                }


                ::pilo::i16_t real_indent = effect_indent;
                if (real_indent < 0)
                    real_indent = this->indent();

                s_gen_indent_to_sstream(ss, real_indent);
                if (_m_os_type == PMI_OS_WINDOWS) {
                    ss << "#if defined(_WIN32) || defined(_WIN64)" << g_autogen_config.newline_sep();                    
                } else if (_m_os_type == PMI_OS_LINUX) {
                    ss << "#if defined(__linux__)" << g_autogen_config.newline_sep();
                } else if (_m_os_type == PMI_OS_APPLE) {
                    ss << "if defined(__APPLE__)" << g_autogen_config.newline_sep();
                } 

                s_gen_indent_to_sstream(ss, real_indent);
                ss << "#include ";
                if (_m_issys) {
                    ss << "<";
                }
                else {
                    ss << "\"";
                }
                ss << _m_import;
                if (_m_issys) {
                    ss << ">";
                }
                else {
                    ss << "\"";
                }
                ss << g_autogen_config.newline_sep();

                s_gen_indent_to_sstream(ss, real_indent);
                if (_m_os_type == PMI_OS_WINDOWS) {
                    ss << "#endif" << g_autogen_config.newline_sep();
                }
                else if (_m_os_type == PMI_OS_LINUX) {
                    ss << "#endif" << g_autogen_config.newline_sep();
                }
                else if (_m_os_type == PMI_OS_APPLE) {
                    ss << "#endif" << g_autogen_config.newline_sep();
                }
                

                return PERR_OK;
            }

        }
    }
}



