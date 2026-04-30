#include    "./meta_srcfile.hpp"
#include    <sstream>
#include    "../string/string_operation.hpp"


namespace pilo
{
    namespace core
    {
        namespace autogen
        {
            meta_srcfile::~meta_srcfile()
            {
            }

            ::pilo::err_t meta_srcfile::append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, ::pilo::i16_t effect_indent ) const
            {
                PMC_UNUSED(strparam);

                ::pilo::i16_t real_indent = effect_indent;
                if (real_indent < 0)
                    real_indent = this->indent();

                bool need_endif_for_uniqe = false;
                if (flags & oflag_dec) {                    
                    need_endif_for_uniqe = append_uniqe_header_declaration_for_cpp(ss);
                }
                    


                for (size_t i = 0; i < _m_nodes.size(); i++) {
                    ::pilo::err_t eret = _m_nodes.at(i)->append_to_stringstream_cpp(ss, flags, "", (::pilo::i16_t)(real_indent));
                    if (eret != PILO_OK) {
                        return eret;
                    }
                }

                if (need_endif_for_uniqe)
                    ss << g_autogen_config.newline_sep() << "#endif" << g_autogen_config.newline_sep();

                return PILO_OK;
            }

            std::string meta_srcfile::name() const
            {
                size_t lastPos = _m_fqdn.find_last_of('.');
                if (lastPos == std::string::npos) {
                    return _m_fqdn;
                }
                return _m_fqdn.substr(lastPos + 1);
            }

            bool meta_srcfile::append_uniqe_header_declaration_for_cpp(std::stringstream& ss) const
            {
                if (_m_fqdn.find('.') == std::string::npos) {
                    ss << "#pragma once" << g_autogen_config.newline_sep() << g_autogen_config.newline_sep();
                    return false;
                }

                std::string tmp = _m_fqdn;
                for (char& c : tmp) {
                    if (c == '.') {
                        c = '_';
                    }
                }
                tmp = "_" + tmp + "_hpp_";
                ss << "#ifndef " << tmp << g_autogen_config.newline_sep() << "#define " << tmp << g_autogen_config.newline_sep() << g_autogen_config.newline_sep();
                return true;
            }

            meta_import* meta_srcfile::add_import(::pilo::i32_t ostype, bool is_sys, bool in_dec, const std::string im)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_import>((::pilo::i16_t)(this->_m_indent), ostype, is_sys, in_dec, im)));

                return static_cast<meta_import*>(_m_nodes.back().get());
            }

            meta_ns* meta_srcfile::add_ns_node(const std::string& ns_namestr)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_ns>((::pilo::i16_t)(this->_m_indent), ns_namestr)));

                return static_cast<meta_ns*>(_m_nodes.back().get());
            }

            meta_klass* meta_srcfile::add_klass_node(::pilo::u64_t modifiers, const std::string& namestr, const std::string& basenamestr, ::pilo::u64_t base_mod)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_klass>((::pilo::i16_t)(this->_m_indent), modifiers, namestr, basenamestr, base_mod)));

                return static_cast<meta_klass*>(_m_nodes.back().get());
            }

            meta_function* meta_srcfile::add_function_node(::pilo::u64_t modifiers, meta_func_type func_ype, const std::string& name, const std::string ret_type)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_function>((::pilo::i16_t)(this->_m_indent), modifiers, func_ype, name, ret_type)));

                return static_cast<meta_function*>(_m_nodes.back().get());
            }

            meta_variable* meta_srcfile::add_variable_node(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_variable>((::pilo::i16_t)(this->_m_indent), modifiers, accessor_flag, namestr, typestr, valuestr)));

                return static_cast<meta_variable*>(_m_nodes.back().get());
            }

            meta_codeline* meta_srcfile::add_codeline_node(::pilo::u64_t modifiers, const std::string& codeline_string, const std::string& comment_string)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_codeline>((::pilo::i16_t)(this->_m_indent), modifiers, codeline_string, comment_string)));

                return static_cast<meta_codeline*>(_m_nodes.back().get());
            }

            void meta_srcfile::add_empty_lines(::pilo::i32_t cnt)
            {
                std::string nls;
                for (::pilo::i32_t i = 0; i < cnt; i++)
                    nls += g_autogen_config.newline_sep();
                _m_nodes.push_back(std::move(std::make_unique<meta_codeline>((::pilo::i16_t)(this->_m_indent), 0, nls, "")));
            }
        }
    }
}



