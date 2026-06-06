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

                std::string sig = load_file_header_signature(_m_header_signature_file);
                ss << sig << g_autogen_config.newline_sep();


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


            bool meta_srcfile::append_uniqe_header_declaration_for_cpp(std::stringstream& ss) const
            {
                ss << "#pragma once" << g_autogen_config.newline_sep() << g_autogen_config.newline_sep();
                return false;               
            }

            meta_import* meta_srcfile::add_import(::pilo::i32_t ostype, bool is_sys, bool in_dec, const std::string im)
            {
                _m_nodes.push_back(std::move(std::make_unique<meta_import>((::pilo::i16_t)(this->_m_indent), ostype, is_sys, in_dec, im)));

                return static_cast<meta_import*>(_m_nodes.back().get());
            }




        }
    }
}



