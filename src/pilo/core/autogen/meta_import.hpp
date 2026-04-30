#ifndef _pilo_core_autogen_meta_import_hpp_
#define _pilo_core_autogen_meta_import_hpp_

#include    "../../pilo.hpp"
#include    <vector>
#include    "./meta_variable.hpp"
#include    "./meta_src_node.hpp"
#include    "./meta_ns.hpp"

namespace pilo
{
    namespace core
    {
        namespace autogen
        {
            class meta_import : public meta_src_node
            {
            public:
                meta_import(::pilo::i16_t indent, ::pilo::i32_t ostype, bool is_sys, bool in_dec, const std::string im)
                    : meta_src_node(meta_node_type_enum::ns, indent, 0), _m_os_type(ostype), _m_issys(is_sys), _m_in_dec(in_dec), _m_import(im)

                {

                }

                ~meta_import();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss,  ::pilo::u32_t flags, const std::string& strparam = "", ::pilo::i16_t effect_indent = -1) const override;


            protected:                
                ::pilo::i32_t               _m_os_type;
                bool                        _m_issys;
                bool                        _m_in_dec;
                std::string                 _m_import;                   

            };



        }
    }
}


#endif // !_pilo_core_autogen_meta_import_hpp_




