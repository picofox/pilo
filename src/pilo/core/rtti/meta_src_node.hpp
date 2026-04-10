#ifndef _pilo_core_rtti_meta_node_hpp_
#define _pilo_core_rtti_meta_node_hpp_

#include    "../../pilo.hpp"
#include    <string>
#include    <sstream>
#include    "../algorithm/uint_sequence_generator.hpp"
#include    <vector>

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            enum class meta_node_type_enum : short
            {
                na = 0,    //0            
                file, //1
                ns, //2
                klass,//3
                func,//4
                var, //5
                enum_c,//6
                enum_cls,//7
                codeline,//8
                imports, //9
                hdr_ins,//10      
                count,
            };

            enum class meta_func_type : ::pilo::u8_t
            {
                na = 0,
                c, //1, ctype func
                member, //2, c++ member
                cons,
                des,
                copycons,
                copyops,
                movecons,
                moveops,

            };

            const ::pilo::u16_t mod_const = 0x2;
            const ::pilo::u16_t mod_static = 0x1;
            const ::pilo::u16_t mod_ptr_const = 0x2;
            const ::pilo::u16_t mod_val_const = 0x4;
            const ::pilo::u16_t mod_volatile = 0x8;
            const ::pilo::u16_t mod_mutable = 0x10;
            const ::pilo::u16_t mod_friend = 0x20;
            const ::pilo::u16_t mod_virtual = 0x40;
            const ::pilo::u16_t mod_inline = 0x80;
            const ::pilo::u16_t mod_abstract = 0x100;
            const ::pilo::u16_t mod_isptr = 0x200;
            const ::pilo::u16_t mod_isstr = 0x400;
            const ::pilo::u16_t mod_ischar = 0x800;
            const ::pilo::u16_t mod_isnull = 0x1000;    
            const ::pilo::u16_t mod_noex = 0x2000;

            const ::pilo::u8_t getter_rtype = 0x01;
            const ::pilo::u8_t getter_ptype_ptr = 0x02;
            const ::pilo::u8_t getter_ptype_ref = 0x04;
            const ::pilo::u8_t getter_ptype_cptr = 0x08;
            const ::pilo::u8_t getter_ptype_cref = 0x10;
            const ::pilo::u8_t setter_vtype = 0x20;
            const ::pilo::u8_t setter_ptype_ptr = 0x20;
            const ::pilo::u8_t setter_ptype_ref = 0x80;
        

            const ::pilo::u32_t oflag_need_priv               = 0x00000001;
            const ::pilo::u32_t oflag_codeline_sep            = 0x00000002;
            const ::pilo::u32_t oflag_need_nl                 = 0x00000004;
            const ::pilo::u32_t oflag_win_nl                  = 0x00000008;
            const ::pilo::u32_t oflag_dec                     = 0x00000010;
            const ::pilo::u32_t oflag_need_value              = 0x00000020;
            const ::pilo::u32_t oflag_sl_cmt                  = 0x00000040;
            const ::pilo::u32_t oflag_cmt_diff_line           = 0x00000080;



            const ::pilo::u8_t acc_priv_none = 0;
            const ::pilo::u8_t acc_priv_public = 1;
            const ::pilo::u8_t acc_priv_procted = 2;
            const ::pilo::u8_t acc_priv_private = 3;

            const ::pilo::id_value_mapper<4, std::string, ::pilo::u8_t> s_cpp_acces_priv_str_mapper({ "", "public", "protected", "private" });

            inline static void s_gen_indent_to_sstream(std::stringstream& ss, ::pilo::i16_t indent, const char* indent_cstr = nullptr)
            {
                if (indent <= 0)
                    return;

                if (indent_cstr == nullptr)
                    indent_cstr = "    ";
                
                for (auto i = 0; i < indent; i++) {
                    ss << indent_cstr;
                }
            }

            inline static void s_gen_nl_to_sstream(std::stringstream& ss, ::pilo::u32_t flags)
            {
                if (flags & oflag_win_nl)
                    ss << PMS_LINESEP_WIN_A;
                else
                    ss << PMS_LINESEP_UNIX_A;
            }

            inline static void s_gen_value_assignment_cppstr(std::stringstream& ss, const std::string& v, const ::pilo::bit_flag<::pilo::u16_t>& modi)
            {
                if (modi.test_value(mod_isnull)) {
                    ss << " = nullptr";
                }
                else {
                    if (!v.empty()) {
                        if (modi.test_value(mod_isstr)) {
                            ss << " = " << "\"" << v << "\"";
                        }
                        else if (modi.test_value(mod_ischar)) {
                            ss << " = " << '\'' << v << '\'';
                        }
                        else {
                            ss << " = " << v ;
                        }
                    }
                }                
            }

            inline static void s_gen_nl(std::stringstream& ss, ::pilo::u32_t flags)
            {
                if (flags & oflag_need_nl) {
                    if (flags & oflag_win_nl) {
                        ss << PMS_LINESEP_WIN_A;
                    }
                    else {
                        ss << PMS_LINESEP_UNIX_A;
                    }                    
                }
            }

            
            class meta_src_node
            {
            public:
                meta_src_node(meta_node_type_enum mnte);
                virtual ~meta_src_node();

                virtual ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags, const std::string & strparam) const = 0;
                virtual meta_node_type_enum meta_type() const { return _m_type; }
                unsigned int id() const { return _m_id; }
                void set_indent(::pilo::i16_t indent) { _m_indent = indent; }
                ::pilo::i16_t indent() const { return _m_indent; }


            public:
                static ::pilo::core::algorithm::uint_sequence_generator<unsigned int>    _s_seq_generator;


            protected:
                meta_node_type_enum    _m_type;
                ::pilo::i16_t        _m_indent;
                unsigned int        _m_id;
                
                

            };

            void s_gen_lines_cpp(std::stringstream& ss, const std::vector<std::unique_ptr<meta_src_node>>& lines, ::pilo::i16_t indent, ::pilo::u32_t flags, const char* indent_cstr = nullptr);

        }
    }
}

#endif
