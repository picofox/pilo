#ifndef _pilo_core_rtti_meta_enum_hpp_
#define _pilo_core_rtti_meta_enum_hpp_

#include    "../../pilo.hpp"
#include    <vector>
#include    "./meta_src_node.hpp"


namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            template<typename T>
            class meta_enum : public meta_src_node
            {
            public:
                meta_enum(::pilo::i16_t indent, ::pilo::u64_t modifiers, const std::string& namestr, const std::string& typestr)
                    : meta_src_node(meta_node_type_enum::enumer, indent, modifiers)
                    , _m_name(namestr), _m_expl_type(typestr)

                {
                }

                ~meta_enum() {}

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam = "", const char* indent_cstr = nullptr) const override
                {
                    PMC_UNUSED(strparam);
                    s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                    ss << "enum ";
                    if (_m_modifiers.test_value(mod_adv_enum)) {
                        ss << "class ";
                    }
                    ss << _m_name << " ";

                    if (_m_expl_type.size() > 0) {
                        ss << ": " << _m_expl_type;
                    }
                                        
                    if (flags & oflag_need_nl) {
                        s_gen_nl_to_sstream(ss, flags);
                        s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
                        ss << "{";
                        s_gen_nl_to_sstream(ss, flags);
                    } else {
                        ss << " {";
                    }
                                        
                    
                    for (size_t i = 0; i < _m_item_names.size(); i++) {         
                        if (flags & oflag_need_nl) {
                            s_gen_indent_to_sstream(ss, this->indent() + 1, indent_cstr);
                        }
                        
                        ss << _m_item_names[i];
                        if (_m_item_vals_expl[i]) {
                            ss << " = " << _m_item_value[i] ;
                        }
                        ss << ",";
                        if (!_m_item_cmt[i].empty()) {
                            ss << " /* " << _m_item_cmt[i] << " */ ";
                        }
                        if (flags & oflag_need_nl)
                            s_gen_nl_to_sstream(ss, flags | oflag_need_nl);
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

                std::string name() const { return _m_name;  };

                void add(const std::string& name, bool has_exp_val, T value, const std::string& cmt = "")
                {
                    _m_item_names.push_back(name);
                    _m_item_vals_expl.push_back(has_exp_val);
                    _m_item_value.push_back(value);
                    _m_item_cmt.push_back(cmt);
                }

            protected:
                std::string                     _m_name;
                std::string                     _m_expl_type;
                std::vector<std::string>        _m_item_names;
                std::vector<bool>               _m_item_vals_expl;
                std::vector<T>                  _m_item_value;
                std::vector<std::string>        _m_item_cmt;
                

            };

        }
    }
}


#endif // !_pilo_core_rtti_meta_enum_hpp_




