#ifndef _pilo_core_rtti_meta_klass_hpp_
#define _pilo_core_rtti_meta_klass_hpp_

#include    <initializer_list>
#include    "../../pilo.hpp"
#include    <vector>
#include    <memory>
#include    "./meta_function.hpp"


namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            class meta_klass : public meta_src_node
            {
            public:
                meta_klass(::pilo::i16_t indent, ::pilo::u64_t modifiers,const std::string& namestr, const std::string& basenamestr, ::pilo::u64_t base_mod)
                    : meta_src_node(meta_node_type_enum::ns, indent, modifiers)
                    , _m_basemod(base_mod)
                    , _m_name(namestr), _m_basename(basenamestr)

                {

                }

                ~meta_klass();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss,  ::pilo::u32_t flags, const std::string& strparam = "", const char* indent_cstr = nullptr, ::pilo::i16_t effect_indent = -1) const override;
                ::pilo::err_t append_cpp_declare_string(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr) const;
                ::pilo::err_t append_cpp_definition_string(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr, ::pilo::i16_t effect_indent = -1) const;


                std::string name() const { return _m_name;  };
                ::pilo::err_t add_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                ::pilo::err_t add_basetype_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                ::pilo::err_t add_string_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                ::pilo::err_t add_nonbasetype_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                ::pilo::err_t prepend_cons_param(::pilo::u64_t modifiers, const std::string& valuestr);
                ::pilo::err_t append_cons_param(::pilo::u64_t modifiers, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                meta_function* add_constructor(::pilo::u64_t modifiers, std::initializer_list<std::string> base_init_param_list);
                meta_function* add_desstructor(::pilo::u64_t modifiers);

                meta_variable* find_member_var_by_name(const std::string name)
                {
                    meta_variable* ptr = nullptr;
                    for (size_t i = 0; i < _m_member_variables.size(); i++) {
                        ptr = static_cast<meta_variable*>(_m_member_variables[i].get());
                        if (ptr == nullptr)
                            return nullptr;
                        std::string mname = s_prefix_to_variable_name_by_mod(ptr->name(), ptr->modifiers().data());
                        if (mname == ptr->name()) {
                            return ptr;
                        }
                    }
                    return nullptr;
                }

            protected:
                ::pilo::u64_t                   _m_basemod;
                std::string                     _m_name;
                std::string                     _m_basename;
                std::vector<std::string>        _m_base_param_init_list;
                std::vector<std::unique_ptr<meta_src_node>>        _m_cons_param_head_list;
                std::vector<std::unique_ptr<meta_src_node>>        _m_cons_param_tail_list;
                std::vector<std::unique_ptr<meta_src_node>>        _m_member_variables;
                std::vector<std::unique_ptr<meta_src_node>>        _m_accessor_methods;
                std::vector<std::unique_ptr<meta_src_node>>        _m_cons_des;
                


                

            };



        }
    }
}


#endif // !_pilo_core_rtti_meta_klass_hpp_




