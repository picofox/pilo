#include    "./meta_ns.hpp"

namespace pilo
{
    namespace core
    {
        namespace autogen
        {
            meta_ns::~meta_ns()
            {
            }
            ::pilo::err_t meta_ns::append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, ::pilo::i16_t effect_indent ) const
            {   
                PMC_UNUSED(strparam);
                ::pilo::i16_t real_indent = effect_indent;
                if (real_indent < 0)
                    real_indent = this->indent();

                s_gen_indent_to_sstream(ss, real_indent);
                ss << "namespace " << this->_m_name << g_autogen_config.newline_sep();
                s_gen_indent_to_sstream(ss, real_indent);
                ss << "{" << g_autogen_config.newline_sep();

                for (size_t i = 0; i < _m_nodes.size(); i++) {
                    ::pilo::err_t eret = _m_nodes.at(i)->append_to_stringstream_cpp(ss, flags, "", (::pilo::i16_t) (real_indent + 1));
                    if (eret != PILO_OK) {
                        return eret;
                    }
                }

                ss<< g_autogen_config.newline_sep();
                s_gen_indent_to_sstream(ss, real_indent);
                ss << "}" << g_autogen_config.newline_sep();
                
                return PILO_OK;
            }

            meta_ns* meta_ns::add_ns_node(const std::string& ns_namestr)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_ns>((::pilo::i16_t ) (this->_m_indent + 1), ns_namestr)));

                return static_cast<meta_ns*>(_m_nodes.back().get());
            }

            meta_klass* meta_ns::add_klass_node(::pilo::u64_t modifiers, const std::string& namestr, const std::string& basenamestr, ::pilo::u64_t base_mod)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_klass>((::pilo::i16_t)(this->_m_indent + 1), modifiers, namestr, basenamestr, base_mod)));

                return static_cast<meta_klass*>(_m_nodes.back().get());
            }

            meta_function* meta_ns::add_function_node(::pilo::u64_t modifiers, meta_func_type func_ype, const std::string& name, const std::string ret_type)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_function>((::pilo::i16_t)(this->_m_indent + 1), modifiers, func_ype, name, ret_type)));

                return static_cast<meta_function*>(_m_nodes.back().get());
            }

            meta_variable* meta_ns::add_variable_node(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_variable>((::pilo::i16_t)(this->_m_indent + 1), modifiers, accessor_flag, namestr, typestr, valuestr)));

                return static_cast<meta_variable*>(_m_nodes.back().get());
            }

            meta_codeline* meta_ns::add_codeline_node(::pilo::u64_t modifiers, const std::string& codeline_string, const std::string& comment_string)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_codeline>((::pilo::i16_t)(this->_m_indent + 1), modifiers, codeline_string, comment_string)));

                return static_cast<meta_codeline*>(_m_nodes.back().get());
            }
        }
    }
}



