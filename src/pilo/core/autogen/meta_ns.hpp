#ifndef _pilo_core_autogen_meta_ns_hpp_
#define _pilo_core_autogen_meta_ns_hpp_

#include    "../../pilo.hpp"
#include    <vector>
#include    "./meta_variable.hpp"
#include    "./meta_src_node.hpp"
#include    "./meta_klass.hpp"
#include    "./meta_codeline.hpp"
#include    "./meta_enum.hpp"

namespace pilo
{
    namespace core
    {
        namespace autogen
        {
            class meta_ns : public meta_src_node
            {
            public:
                meta_ns(::pilo::i16_t indent, const std::string& namestr)
                    : meta_src_node(meta_node_type_enum::ns, indent,0), _m_name(namestr)

                {

                }

                ~meta_ns();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam = "",  ::pilo::i16_t effect_indent = -1) const override;
                std::string name() const { return _m_name;  };

                meta_ns* add_ns_node(const std::string& ns_namestr);
                meta_klass* add_klass_node(::pilo::u64_t modifiers, const std::string& namestr, const std::string& basenamestr, ::pilo::u64_t base_mod);
                meta_function* add_function_node(::pilo::u64_t modifiers, meta_func_type func_ype, const std::string& name, const std::string ret_type);
                meta_variable* add_variable_node(::pilo::u64_t modifiers, ::pilo::u32_t accessor_flag, const std::string& namestr, const std::string& typestr, const std::string& valuestr);
                meta_codeline* add_codeline_node(::pilo::u64_t modifiers, const std::string& codeline_string, const std::string& comment_string);
                

                template<typename T>
                meta_enum<T>* add_enum_node(::pilo::u64_t modifiers, const std::string& namestr, const std::string& typestr)
                {
                    _m_nodes.push_back(std::move(std::make_unique<meta_enum<T>>((::pilo::i16_t)(this->_m_indent + 1), namestr, typestr)));

                    return static_cast<meta_enum<T>*>(_m_nodes.back().get());
                }


            protected:
                std::string    _m_name;
                std::vector<std::unique_ptr<meta_src_node>>           _m_nodes;

                


                

            };



        }
    }
}


#endif // !_pilo_core_autogen_meta_ns_hpp_




