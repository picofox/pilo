#ifndef _pilo_core_autogen_meta_srcfile_hpp_
#define _pilo_core_autogen_meta_srcfile_hpp_

#include    "../../pilo.hpp"
#include    <vector>
#include    "./meta_ns.hpp"
#include    "./meta_import.hpp"


namespace pilo
{
    namespace core
    {
        namespace autogen
        {
            class meta_srcfile : public meta_src_node
            {
            public:
                meta_srcfile(::pilo::i16_t indent, const std::string& namestr)
                    : meta_src_node(meta_node_type_enum::ns, indent, 0), _m_fqdn(namestr)

                {

                }

                ~meta_srcfile();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss,  ::pilo::u32_t flags, const std::string& strparam = "", ::pilo::i16_t effect_indent = -1) const override;

                std::string fqdn() const { return _m_fqdn; };
                std::string name() const;
                bool append_uniqe_header_declaration_for_cpp(std::stringstream& ss) const;

                meta_import* add_import(::pilo::i32_t ostype, bool is_sys, bool in_dec, const std::string im);
                meta_ns* add_ns_node(const std::string& ns_namestr);
                meta_klass* add_klass_node(::pilo::u64_t modifiers, const std::string& namestr, const std::string& basenamestr, ::pilo::u64_t base_mod);
                meta_function* add_function_node(::pilo::u64_t modifiers, meta_func_type func_ype, const std::string& name, const std::string ret_type);
                meta_variable* add_variable_node(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                meta_codeline* add_codeline_node(::pilo::u64_t modifiers, const std::string& codeline_string, const std::string& comment_string);
                void           add_empty_lines(::pilo::i32_t cnt);

            protected:
                const std::string                                       _m_fqdn;
                std::vector<std::unique_ptr<meta_src_node>>             _m_nodes;
            };



        }
    }
}


#endif // !_pilo_core_autogen_meta_srcfile_hpp_




