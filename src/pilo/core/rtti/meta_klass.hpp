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
                    , _m_name(namestr), _m_basename(basenamestr)

                {

                }

                ~meta_klass();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags, const std::string& strparam) const override;

                std::string name() const { return _m_name;  };


            protected:
                std::string                     _m_name;
                std::string                     _m_basename;
                std::vector<std::string>        _m_base_param_init_list;
                std::vector<std::unique_ptr<meta_src_node>>        _m_param_init_head_list;
                std::vector<std::unique_ptr<meta_src_node>>        _m_param_init_tail_list;
                std::vector<std::unique_ptr<meta_src_node>>        _m_member_variables;

                


                

            };



        }
    }
}


#endif // !_pilo_core_rtti_meta_klass_hpp_




