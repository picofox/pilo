#ifndef _pilo_core_rtti_meta_klass_hpp_
#define _pilo_core_rtti_meta_klass_hpp_

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
                meta_klass(::pilo::i16_t indent, ::pilo::u64_t modifiers,const std::string& namestr, const std::string& basenamestr)
                    : meta_src_node(meta_node_type_enum::ns, indent, modifiers)
                    , _m_basepriv(access_priv_type_enum::none)
                    , _m_name(namestr), _m_basename(basenamestr)

                {

                }

                ~meta_klass();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss,  ::pilo::u32_t flags, const std::string& strparam = "", const char* indent_cstr = nullptr) const override;
                ::pilo::err_t append_cpp_declare_string(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr) const;
                ::pilo::err_t append_cpp_definition_string(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr) const;


                std::string name() const { return _m_name;  };
                ::pilo::err_t add_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                ::pilo::err_t add_basetype_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                ::pilo::err_t add_string_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                ::pilo::err_t add_nonbasetype_member_variable(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);

                

            protected:
                access_priv_type_enum           _m_basepriv;
                std::string                     _m_name;
                std::string                     _m_basename;
                std::vector<std::string>        _m_base_param_init_list;
                std::vector<std::unique_ptr<meta_src_node>>        _m_param_init_head_list;
                std::vector<std::unique_ptr<meta_src_node>>        _m_param_init_tail_list;
                std::vector<std::unique_ptr<meta_src_node>>        _m_member_variables;
                std::vector<std::unique_ptr<meta_src_node>>        _m_accessor_methods;
                


                

            };



        }
    }
}


#endif // !_pilo_core_rtti_meta_klass_hpp_




