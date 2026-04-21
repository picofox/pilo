#ifndef _pilo_core_rtti_meta_function_hpp_
#define _pilo_core_rtti_meta_function_hpp_


#include    "../../pilo.hpp"
#include    <vector>
#include    "./meta_variable.hpp"
#include    "./meta_codeline.hpp"
#include    <memory>

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            class meta_function : public meta_src_node
            {
            public:
                meta_function(::pilo::i16_t indent, ::pilo::u64_t modifiers,  meta_func_type func_ype, const std::string& name, const std::string ret_type)
                    :meta_src_node(meta_node_type_enum::var, indent, modifiers)
                    , _m_func_type(func_ype)
                    , _m_name(name), _m_ret_type(ret_type) {
                }
                    

                ~meta_function();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam = "", const char* indent_cstr = nullptr, ::pilo::i16_t effect_indent = -1) const override;

                ::pilo::err_t add_param(::pilo::u64_t modifiers, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                ::pilo::err_t add_bodyline(::pilo::i16_t rel_indent, ::pilo::u64_t modifiers, const std::string & line, const std::string cmt = "");

            
                ::pilo::err_t append_cpp_declare_string(std::stringstream& ss,  ::pilo::u32_t flags, const char* indent_cstr) const;
                ::pilo::err_t append_cpp_definition_string(std::stringstream& ss, ::pilo::u32_t flags,  const std::string& strparam, const char* indent_cstr, ::pilo::i16_t effect_indent = -1) const;
                void add_param_to_base_init_param_list(const std::string& param) { _m_base_init_param_list.push_back(param); }
                void add_member_var_list(meta_variable* vp) { _m_member_var_list.push_back(vp); }
                meta_variable* find_var_in_members(const std::string& name, ::pilo::u64_t mod)
                {
                    for (size_t i = 0; i < _m_member_var_list.size(); i++) {
                        std::string mname = s_prefix_to_variable_name_by_mod(name, mod);
                        if (mname == _m_member_var_list.at(i)->name()) {
                            return _m_member_var_list.at(i);
                        }
                    }
                    return nullptr;
                }
                meta_variable* find_var_in_init_list(const std::string& name, ::pilo::u64_t mod) const
                {
                    for (size_t i = 0; i < _init_member_var_list.size(); i++) {
                        std::string mname = s_prefix_to_variable_name_by_mod(name, mod);
                        if (mname == _init_member_var_list.at(i)->name()) {
                            return _init_member_var_list.at(i);
                        }
                    }
                    return nullptr;
                }
                bool exist_in_init_member_var_list(const meta_variable* mvp) const
                {
                    for (size_t i = 0; i < _init_member_var_list.size(); i++) {
                        if (mvp->id() == _init_member_var_list.at(i)->id()) {
                            return true;
                        }
                    }
                    return false;
                }

            protected:                
                meta_func_type                    _m_func_type;
                std::string                        _m_name;
                std::string                        _m_ret_type;                
                std::vector<meta_variable>        _m_params;
                std::vector<std::unique_ptr<meta_src_node>>    _m_bodylines;
                std::vector<std::string>          _m_base_init_param_list;
                std::vector<meta_variable*>       _m_member_var_list;

            private:
                std::vector<meta_variable*>       _init_member_var_list;
            };

        }
    }
}


#endif // !_pilo_core_rtti_meta_function_hpp_




