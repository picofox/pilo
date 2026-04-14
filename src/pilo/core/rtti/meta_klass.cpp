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
            ::pilo::err_t meta_klass::append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr) const
            {     
                if (flags & oflag_dec)
                    return append_cpp_declare_string(ss, flags, strparam, indent_cstr);
                else
                    return append_cpp_definition_string(ss, flags, strparam, indent_cstr);


            }

            ::pilo::err_t meta_klass::append_cpp_declare_string(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr) const
            {
                PMC_UNUSED(strparam);
                s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                access_priv_type_enum cur_priv = access_priv_type_enum::none;
                bool need_priv = true;
                if (_m_modifiers.test_value(mod_c_struct)) {
                    ss << "struct ";
                }
                else {
                    ss << "class ";
                }
                ss << _m_name << " ";


                s_gen_nl_to_sstream(ss, flags);
                s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                ss << "{";
                s_gen_nl_to_sstream(ss, flags);


                s_gen_nl_to_sstream(ss, flags);
                s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                ss << "//Accessor methods:";
                s_gen_nl_to_sstream(ss, flags);
                ::pilo::u32_t var_oflag = 0;
                
                for (size_t i = 0; i < _m_accessor_methods.size(); i++) {
                    if (cur_priv != this->_m_accessor_methods[i]->access_priv_type()) {
                        need_priv = true;
                        cur_priv = this->_m_accessor_methods[i]->access_priv_type();
                    }
                    else {
                        need_priv = false;
                    }

                    if (need_priv)
                        var_oflag = oflag_need_priv ;
                    else
                        var_oflag = 0;
                    
                    this->_m_accessor_methods[i]->append_to_stringstream_cpp(ss, oflag_codeline_sep|oflag_need_nl|oflag_dec| var_oflag, "", nullptr);
                    s_gen_nl_to_sstream(ss, flags | oflag_need_nl);

                }

                


               
                s_gen_nl_to_sstream(ss, flags);
                s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                ss << "//Member variables:";
                s_gen_nl_to_sstream(ss, flags);
                

                for (size_t i = 0; i < _m_member_variables.size(); i++) {
                    if (cur_priv != this->_m_member_variables[i]->access_priv_type()) {
                        need_priv = true;
                        cur_priv = this->_m_member_variables[i]->access_priv_type();
                    }
                    else {
                        need_priv = false;
                    }

                    if (need_priv)
                        var_oflag = oflag_need_priv;
                    else
                        var_oflag = 0;
                    this->_m_member_variables[i]->append_to_stringstream_cpp(ss, oflag_codeline_sep| oflag_need_nl|var_oflag, "", nullptr);

                    
                }


                s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                ss << "};";
                s_gen_nl_to_sstream(ss, flags);
                return PILO_OK;

            }

            ::pilo::err_t meta_klass::append_cpp_definition_string(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr) const
            {
                PMC_UNUSED(ss);
                PMC_UNUSED(indent_cstr);
                PMC_UNUSED(flags);
                PMC_UNUSED(strparam);
                return ::pilo::err_t();
            }

            ::pilo::err_t meta_klass::add_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag,const std::string& namestr, const std::string& typestr, const std::string& valuestr)
            {
                ::pilo::i16_t rel_indent = this->_m_indent + 1;   
                ::pilo::u64_t mods = mod_val_const | mod_const | mod_public | mod_inline;
                ::pilo::u64_t setter_mods = mod_public | mod_inline;
                ::pilo::u64_t arg_mods = 0;
                this->_m_member_variables.push_back(std::make_unique<meta_variable>(rel_indent, modifiers,  accessor_flag, namestr, typestr, valuestr));
                if (accessor_flag & getter_rtype) {
                    if (modifiers & mod_non_basetype) {
                        mods |= mod_is_ref;
                    }

                    std::unique_ptr mp = std::make_unique<meta_function>(rel_indent, mods, meta_func_type::member, s_name_to_getter(namestr), typestr);                    
                    mp->add_bodyline(1, mod_sentence_end,  "return " + s_prefix_to_variable_name_by_mod(namestr, modifiers), "");
                    this->_m_accessor_methods.push_back(std::move(mp));                    
                }
                if (accessor_flag & setter_vtype) {
                    std::unique_ptr mp = std::make_unique<meta_function>(rel_indent, setter_mods, meta_func_type::member, s_name_to_setter(namestr), "void");
                    if (modifiers & mod_isstr)
                        arg_mods |= (mod_isstr | mod_is_ref);
                    if (modifiers & mod_non_basetype)
                        arg_mods |= (mod_is_ref | mod_non_basetype);

                    std::string farg = "fa_" + namestr;
                    mp->add_param(mod_const | arg_mods, farg, typestr, "");
                    mp->add_bodyline(1, mod_sentence_end, s_prefix_to_variable_name_by_mod(namestr, modifiers) + " = " + farg, "");
                    this->_m_accessor_methods.push_back(std::move(mp));
                }
                return PILO_OK;
  
            }

            ::pilo::err_t meta_klass::add_basetype_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr)
            {                
                return add_member_variable(mod_prefix | modifiers, accessor_flag, namestr, typestr, valuestr);
            }

            ::pilo::err_t  meta_klass::add_string_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr)
            {
                return add_member_variable(modifiers | mod_prefix | mod_isstr | mod_non_basetype, accessor_flag, namestr, typestr, valuestr);
            }
            ::pilo::err_t meta_klass::add_nonbasetype_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr)
            {
                return add_member_variable(modifiers | mod_prefix | mod_non_basetype, accessor_flag, namestr, typestr, valuestr);
            }
        }
    }
}



