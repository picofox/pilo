#ifndef _pilo_core_rtti_meta_ns_hpp_
#define _pilo_core_rtti_meta_ns_hpp_

#include    "../../pilo.hpp"
#include    <vector>
#include    "./meta_variable.hpp"
#include    "./meta_src_node.hpp"

namespace pilo
{
    namespace core
    {
        namespace rtti
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
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags, const std::string& strparam) const override;

                std::string name() const { return _m_name;  };


            protected:
                std::string    _m_name;
                std::vector<meta_src_node*>    _nodes;

                


                

            };



        }
    }
}


#endif // !_pilo_core_rtti_meta_ns_hpp_




