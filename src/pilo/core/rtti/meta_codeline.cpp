#include    "./meta_codeline.hpp"

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            meta_codeline::~meta_codeline()
            {                
            }
            ::pilo::err_t meta_codeline::append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr, ::pilo::i16_t effect_indent ) const
            {
                if (effect_indent == -1)
                    effect_indent = this->_m_indent;

                PMC_UNUSED(strparam);
                flags |= oflag_need_nl;
                if (this->is_blank()) {
                    s_gen_nl_to_sstream(ss, flags);
                    return PERR_OK;
                }
                else {
                    s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
                    if (this->is_comment()) {
                        if (flags & oflag_sl_cmt)
                            ss << "//" << this->_m_comment;
                        else
                            ss << "/* " << this->_m_comment << " */";
                        s_gen_nl(ss, flags);
                    
                    } else if (this->is_code()) {                    
                        ss << this->_m_codeline;
                        if (this->_m_modifiers.test_value(mod_sentence_end))
                            ss << ";";
                        s_gen_nl(ss, flags);

                    } else {
                        ss << this->_m_codeline;
                        if (flags & ::pilo::core::rtti::oflag_cmt_diff_line) {
                            s_gen_nl_to_sstream(ss, flags);
                        }
                        if (flags & oflag_sl_cmt)
                            ss << "//" << this->_m_comment;
                        else
                            ss << "/* " << this->_m_comment << " */";
                        s_gen_nl(ss, flags);
                    }
                }                

                return PERR_OK;
            }
        }
    }
}



