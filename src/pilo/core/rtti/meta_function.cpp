#include	"./meta_function.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			meta_function::~meta_function()
			{
			}
			::pilo::err_t meta_function::append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags, const std::string& strparam) const
			{
				if (flags & oflag_dec)
					return append_cpp_declare_string(ss, indent_cstr, flags);
				else 
					return append_cpp_definition_string(ss, indent_cstr, flags, strparam);

			}

			::pilo::err_t meta_function::add_param(::pilo::u16_t modifiers, const std::string& namestr, const std::string& typestr, const std::string& valuestr)
			{
				meta_variable var(modifiers, 0, 0, namestr, typestr, valuestr);
				_m_params.emplace_back(var);
				return PILO_OK;
			}

			::pilo::err_t meta_function::add_bodyline(::pilo::u16_t modifiers,::pilo::i16_t indent, const std::string& line, const std::string cmt)
			{
				std::unique_ptr<meta_src_node> node_ptr = std::make_unique<meta_codeline>(modifiers, line, cmt);
				node_ptr->set_indent(indent);
				this->_m_bodylines.push_back(std::move(node_ptr));
				return PILO_OK;
			}



			::pilo::err_t meta_function::append_cpp_declare_string(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags) const
			{	
				if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::c) {
					s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
					ss << this->_m_ret_type << ' ' << this->_m_name << '(' ;
					for (size_t i = 0; i < this->_m_params.size(); i++) {
						if (i > 0) {
							ss << ", ";
						}
						this->_m_params[i].append_as_param_string(ss, flags);
					}
					ss << ')';							

				}	
				else {
					if ((flags & oflag_need_priv) && this->_m_access_priv != acc_priv_none) {
						s_gen_indent_to_sstream(ss, pilo_min<::pilo::i16_t>(this->indent() - 1, 1), indent_cstr);
						ss << s_cpp_acces_priv_str_mapper.get_value(this->_m_access_priv);
						ss << ':';
						s_gen_nl_to_sstream(ss, flags);
					}

					if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::cons) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);						
						ss << this->_m_name << '(';
						for (size_t i = 0; i < this->_m_params.size(); i++) {
							if (i > 0) {
								ss << ", ";
							}
							this->_m_params[i].append_as_param_string(ss, flags);
						}
						ss << ')';
					
					} else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::des) {						
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						if (this->_m_modifiers.test_value(mod_virtual))
							ss << "virtual ";
						ss << '~'  << this->_m_name << "()";

					} else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::copycons) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						ss << this->_m_name << "(const " << this->_m_name  << "& other)";

					} else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::copyops) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						ss << this->_m_name  << "& operator=(const " << this->_m_name << "& other)";

					} else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::movecons) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						ss << this->_m_name << "(const " << this->_m_name << "&& other)";

					} else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::moveops) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						ss << this->_m_name << "& operator=(const " << this->_m_name << "&& other)";

					} else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::member) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						if (this->_m_modifiers.test_value(mod_virtual))
							ss << "virtual ";
						else if (this->_m_modifiers.test_value(mod_static))
							ss << "static ";
						else if (this->_m_modifiers.test_value(mod_friend))
							ss << "friend ";

						ss << this->_m_ret_type << ' ' << this->_m_name << '(';
						for (size_t i = 0; i < this->_m_params.size(); i++) {
							if (i > 0) {
								ss << ", ";
							}
							this->_m_params[i].append_as_param_string(ss, flags);
						}
						ss << ')';

					} else {
						 ::pilo::mk_perr(PERR_INVALID_PARAM);;
					}


					if (this->_m_modifiers.test_value(mod_const)) {
						ss << " const";
					}

					if (this->_m_modifiers.test_value(mod_noex)) {
						ss << " noexcep";
					}

					if (this->_m_modifiers.test_value(mod_abstract)) {
						ss << " = 0";
					}

				}

				if (flags & oflag_codeline_sep) {
					ss << ';';
				}
				s_gen_nl(ss, flags);

				return PILO_OK;
			}

			::pilo::err_t meta_function::append_cpp_definition_string(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags, const std::string& strparam) const
			{
				if (this->_m_modifiers.test_value(mod_inline))
					ss << "inline ";

				if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::c) {
					s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
					ss << this->_m_ret_type << ' ' << this->_m_name << '(';
					for (size_t i = 0; i < this->_m_params.size(); i++) {
						if (i > 0) {
							ss << ", ";
						}
						this->_m_params[i].append_as_param_string(ss, flags);
					}					
					ss << ')';

				}
				else {
					if ((flags & oflag_need_priv) && this->_m_access_priv != acc_priv_none) {
						s_gen_indent_to_sstream(ss, pilo_min<::pilo::i16_t>(this->indent() - 1, 1), indent_cstr);
						ss << s_cpp_acces_priv_str_mapper.get_value(this->_m_access_priv);
						ss << ':';
						s_gen_nl_to_sstream(ss, flags);
					}

					if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::cons) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						ss << strparam << "::" << this->_m_name << '(';
						for (size_t i = 0; i < this->_m_params.size(); i++) {
							if (i > 0) {
								ss << ", ";
							}
							this->_m_params[i].append_as_param_string(ss, flags);
						}
						ss << ')';

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::des) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						if (this->_m_modifiers.test_value(mod_virtual))
							ss << "virtual ";
						ss << strparam << "::" << '~' << this->_m_name << "()";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::copycons) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						ss << strparam << "::" << this->_m_name << "(const " << this->_m_name << "& other)";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::copyops) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						ss << strparam << "::" << this->_m_name << "& operator=(const " << this->_m_name << "& other)";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::movecons) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						ss << strparam << "::" << this->_m_name << "(const " << this->_m_name << "&& other)";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::moveops) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						ss << strparam << "::" << this->_m_name << "& operator=(const " << this->_m_name << "&& other)";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::member) {
						s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
						if (this->_m_modifiers.test_value(mod_virtual))
							ss << "virtual ";
						else if (this->_m_modifiers.test_value(mod_static))
							ss << "static ";
						else if (this->_m_modifiers.test_value(mod_friend))
							ss << "friend ";

						ss << strparam << "::" << this->_m_ret_type << ' ' << this->_m_name << '(';
						for (size_t i = 0; i < this->_m_params.size(); i++) {
							if (i > 0) {
								ss << ", ";
							}
							this->_m_params[i].append_as_param_string(ss, flags);
						}
						ss << ')';

					}
					else {
						::pilo::mk_perr(PERR_INVALID_PARAM);;
					}


					if (this->_m_modifiers.test_value(mod_const)) {
						ss << " const";
					}

					if (this->_m_modifiers.test_value(mod_noex)) {
						ss << " noexcep";
					}
				}

				s_gen_nl(ss, flags);
				s_gen_indent_to_sstream(ss, this->_m_indent, indent_cstr);
				ss << "{";
				s_gen_nl(ss, flags);
				s_gen_lines_cpp(ss, this->_m_bodylines, this->_m_indent+1, flags, indent_cstr);
				s_gen_nl(ss, flags);
				s_gen_indent_to_sstream(ss, this->_m_indent, indent_cstr);
				ss << "}";


				if (flags & oflag_need_nl) {
					s_gen_nl(ss, flags);
				}				

				return PILO_OK;
			}

		}
	}
}



