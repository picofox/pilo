#include    "./meta_variable.hpp"

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            meta_variable::~meta_variable()
            {
            }

            ::pilo::err_t meta_variable::append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam , const char* indent_cstr, ::pilo::i16_t effect_indent ) const
            {            
                PMC_UNUSED(strparam);
                if (effect_indent == -1)
                    effect_indent = this->indent();
                
                if (flags & oflag_supress_type) {
                    s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
                }
                else {
                    if (flags & oflag_need_priv) {
                        flags |= oflag_need_nl | oflag_need_colsep;
                        s_gen_priv(ss, this->_m_modifiers.data(), flags, false, effect_indent - 1, indent_cstr);
                    }
                    s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
                    if (_m_modifiers.test_value(mod_mutable))
                        ss << "mutable ";
                    if (_m_modifiers.test_value(mod_static))
                        ss << "static ";
                    if (_m_modifiers.test_value(mod_ptr_const))
                        ss << "const ";
                    if (_m_modifiers.test_value(mod_volatile))
                        ss << "volatile ";
                    ss << _m_type << ' ';
                    if (_m_modifiers.test_value(mod_val_const))
                        ss << "const ";
                }

                
                ss << _m_name;
                if (flags & oflag_need_value) {
                    s_gen_value_assignment_cppstr(ss, _m_value, _m_modifiers);
                }
                
                if (flags & oflag_codeline_sep) {
                    ss << ';';
                }
                s_gen_nl(ss, flags);                

                return PERR_OK;
            }

            void meta_variable::append_as_param_string(std::stringstream& ss, ::pilo::u32_t flags) const
            {
                if (_m_modifiers.test_value(mod_ptr_const))
                    ss << "const ";
                if (_m_modifiers.test_value(mod_volatile))
                    ss << "volatile ";                
                
                if (this->_m_modifiers.test_value(mod_is_ref))
                    ss << _m_type << "& ";
                else
                    ss << _m_type << ' ';

                if (_m_modifiers.test_value(mod_val_const))
                    ss << "const ";
                ss << _m_name;
                if (flags & oflag_need_value) {
                    s_gen_value_assignment_cppstr(ss, _m_value, _m_modifiers);
                }

            }


        }
    }
}