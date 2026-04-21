#ifndef _pilo_core_rtti_meta_ns_hpp_
#define _pilo_core_rtti_meta_ns_hpp_

#include    "../../pilo.hpp"
#include    <vector>
#include    "./meta_src_node.hpp"

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            class meta_codeline : public meta_src_node
            {
            public:            
                meta_codeline(::pilo::i16_t indent, ::pilo::u64_t modifiers, const std::string& codeline_string, const std::string& comment_string)
                    : meta_src_node(meta_node_type_enum::codeline, indent, modifiers)
                    , _m_codeline(codeline_string), _m_comment(comment_string)


                {

                }

                ~meta_codeline();

                // Inherited via meta_src_node
                ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam = "", const char* indent_cstr = nullptr, ::pilo::i16_t effect_indent = -1) const override;

                std::string codeline() const { return _m_codeline;  }
                std::string comment() const { return _m_comment; }

                bool is_comment() const { return (_m_codeline.empty() && (!_m_comment.empty())); }
                bool is_blank() const { return (_m_codeline.empty() && _m_comment.empty()); }
                bool is_code() const { return ((!_m_codeline.empty()) && (_m_comment.empty())); }

            protected:
                std::string    _m_codeline;
                std::string _m_comment;
                
                


                

            };



        }
    }
}


#endif // !_pilo_core_rtti_meta_ns_hpp_




