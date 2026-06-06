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
            class meta_srcfile : public meta_ns
            {
            public:
                meta_srcfile(::pilo::i16_t indent) : meta_ns(indent, ""), _m_header_signature_file("")
                {
                    this->_m_type = meta_node_type_enum::file;
                }

                ~meta_srcfile();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss,  ::pilo::u32_t flags, const std::string& strparam = "", ::pilo::i16_t effect_indent = -1) const override;

                bool append_uniqe_header_declaration_for_cpp(std::stringstream& ss) const;
                meta_import* add_import(::pilo::i32_t ostype, bool is_sys, bool in_dec, const std::string im);


            protected:
                const std::string                                       _m_header_signature_file;

            };



        }
    }
}


#endif // !_pilo_core_autogen_meta_srcfile_hpp_




