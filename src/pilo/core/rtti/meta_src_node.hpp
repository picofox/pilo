#ifndef _pilo_core_rtti_meta_node_hpp_
#define _pilo_core_rtti_meta_node_hpp_

#include	"../../pilo.hpp"
#include	<string>
#include	<sstream>
#include	"../algorithm/uint_sequence_generator.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			enum class meta_node_type_enum : short
			{
				na = 0,	//0			
				file, //1
				ns, //2
				klass,//3
				func,//4
				var, //5
				enum_c,//6
				enum_cls,//7
				macro_define,//8
				macro_other,//9
				code_line,//10
				imports, //11
				hdr_ins,//12
				comment,//13				
				other,//14
				count,
			};

			const ::pilo::u16_t mod_bit_static = 0;
			const ::pilo::u16_t mod_bit_ptr_const = 1;
			const ::pilo::u16_t mod_bit_val_const = 2;
			const ::pilo::u16_t mod_bit_volatile = 3;
			const ::pilo::u16_t mod_bit_mutable = 4;
			const ::pilo::u16_t mod_bit_isptr = 5;
			const ::pilo::u16_t mod_bit_isstr = 6;
			const ::pilo::u16_t mod_bit_ischar = 7;
			const ::pilo::u16_t mod_bit_isnull = 8;

			const ::pilo::u16_t mod_val_static = 1;
			const ::pilo::u16_t mod_val_pre_const = 2;
			const ::pilo::u16_t mod_val_post_const = 4;
			const ::pilo::u16_t mod_val_volatile = 8;
			const ::pilo::u16_t mod_val_mutable = 0x10;
			const ::pilo::u16_t mod_val_isptr = 0x20;
			const ::pilo::u16_t mod_val_isstr = 0x40;
			const ::pilo::u16_t mod_val_ischar = 0x80;
			const ::pilo::u16_t mod_val_isnull = 0x100;




			const ::pilo::u32_t output_flag_need_priv = 0x1;
			const ::pilo::u32_t output_flag_codeline_sep = 0x2;
			const ::pilo::u32_t output_flag_need_nl = 0x4;
			const ::pilo::u32_t output_flag_win_nl = 0x8;





			const ::pilo::u8_t acc_priv_none = 0;
			const ::pilo::u8_t acc_priv_public = 1;
			const ::pilo::u8_t acc_priv_procted = 2;
			const ::pilo::u8_t acc_priv_private = 3;

			const ::pilo::id_value_mapper<5, std::string, ::pilo::u16_t> s_cpp_meta_id_value_mapper({ "static", "const", "const", "volatile","mutable"});
			const ::pilo::id_value_mapper<5, std::string, ::pilo::u8_t> s_cpp_acces_priv_str_mapper({ "", "public", "protected", "private" });

			inline static void s_gen_indent_to_sstream(std::stringstream& ss, ::pilo::i16_t indent, const char* indent_cstr = nullptr)
			{
				if (indent_cstr == nullptr)
					indent_cstr = "    ";
				for (auto i = 0; i < indent; i++) {
					ss << indent_cstr;
				}
			}

			inline static void s_gen_nl_to_sstream(std::stringstream& ss, ::pilo::u32_t flags)
			{
				if (flags & output_flag_win_nl)
					ss << PMS_LINESEP_WIN_A;
				else
					ss << PMS_LINESEP_UNIX_A;
			}

			inline static void s_gen_value_assignment_cppstr(std::stringstream& ss, const std::string& v, const ::pilo::bit_flag<::pilo::u16_t>& modi)
			{
				if (modi.test_value(mod_val_isnull)) {
					ss << " = nullptr";
				}
				else {
					if (!v.empty()) {
						if (modi.test_value(mod_val_isstr)) {
							ss << " = " << "\"" << v << "\"";
						}
						else if (modi.test_value(mod_val_ischar)) {
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
				if (flags & output_flag_need_nl) {
					if (flags & output_flag_win_nl) {
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
				virtual ~meta_src_node() = 0;

				virtual ::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags) const = 0;
				virtual meta_node_type_enum meta_type() const { return _m_type; }
				unsigned int id() const { return _m_id; }
				void set_indent(::pilo::i16_t indent) { _m_indent = indent; }
				::pilo::i16_t indent() const { return _m_indent; }


			public:
				static ::pilo::core::algorithm::uint_sequence_generator<unsigned int>	_s_seq_generator;


			protected:
				meta_node_type_enum	_m_type;
				::pilo::i16_t		_m_indent;
				unsigned int		_m_id;
				
				

			};
		}
	}
}

#endif
