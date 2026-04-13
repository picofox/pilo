#include    "./meta_klass.hpp"

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            meta_klass::~meta_klass()
            {
            }
            ::pilo::err_t meta_klass::append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags, const std::string& strparam) const
            {     
                PMC_UNUSED(strparam);

                s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                ss << "class ";
                if (_m_modifiers.test_value(mod_adv_enum)) {
                    ss << "class ";
                }
                ss << _m_name << " ";


                if (flags & oflag_need_nl) {
                    s_gen_nl_to_sstream(ss, flags);
                    s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                    ss << "{";
                    s_gen_nl_to_sstream(ss, flags);
                }
                else {
                    ss << " {";
                }

                
                if (flags & oflag_need_nl) {
                    s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                    ss << "};";
                }
                else {
                    ss << " };";
                }


                s_gen_nl_to_sstream(ss, flags);
                return PILO_OK;

            }
        }
    }
}



