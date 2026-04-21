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
			::pilo::err_t meta_function::append_to_stringstream_cpp(std::stringstream& ss,  ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr,  ::pilo::i16_t effect_indent) const
			{
				if (flags & oflag_dec)
					return append_cpp_declare_string(ss, flags, indent_cstr);
				else 
					return append_cpp_definition_string(ss, flags, strparam, indent_cstr, effect_indent);

			}

			::pilo::err_t meta_function::add_param(::pilo::u64_t modifiers, const std::string& namestr, const std::string& typestr, const std::string& valuestr)
			{
				meta_variable var(0, modifiers, 0, namestr, typestr, valuestr);
				if (modifiers & mod_map_to_member) {
					meta_variable* mvp = this->find_var_in_members(namestr, modifiers);
					if (mvp == nullptr)
						return mk_perr(PERR_NON_EXIST);
					_init_member_var_list.push_back(mvp);
				}

				_m_params.emplace_back(var);
				return PILO_OK;
			}

			::pilo::err_t meta_function::add_bodyline(::pilo::i16_t rel_indent, ::pilo::u64_t modifiers, const std::string& line, const std::string cmt)
			{
				rel_indent += this->_m_indent;
				this->_m_bodylines.push_back(std::make_unique<meta_codeline>(rel_indent, modifiers, line, cmt));
				return PILO_OK;
			}



			::pilo::err_t meta_function::append_cpp_declare_string(std::stringstream& ss, ::pilo::u32_t flags, const char* indent_cstr) const
			{	
				if (this->_m_modifiers.test_value(mod_inline)) {
					return append_cpp_definition_string(ss, flags, "", indent_cstr, -1);
				}

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
					if (flags & oflag_need_priv) {
						flags |= oflag_need_nl | oflag_need_colsep;
						s_gen_priv(ss, this->_m_modifiers.data(), flags, false, this->indent() - 1, indent_cstr);
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
						if (this->_m_modifiers.test_value(mod_inline))
							ss << "inline ";
						if (this->_m_modifiers.test_value(mod_virtual))
							ss << "virtual ";
						else if (this->_m_modifiers.test_value(mod_static))
							ss << "static ";
						else if (this->_m_modifiers.test_value(mod_friend))
							ss << "friend ";

						if (this->_m_modifiers.test_value(mod_val_const)) {
							ss << "const ";
						}

						ss << this->_m_ret_type;
						if (this->_m_modifiers.test_value(mod_is_ref)) {
							ss << "&";
						}
						ss << " ";

						ss << this->_m_name << '(';
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

			::pilo::err_t meta_function::append_cpp_definition_string(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr, ::pilo::i16_t effect_indent) const
			{
				if (effect_indent == -1)
					effect_indent = this->indent();

				std::string init_buff;
				bool b_find_map_member = false;

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
					if (flags & oflag_need_priv) {
						flags |= oflag_need_nl | oflag_need_colsep;
						s_gen_priv(ss, this->_m_modifiers.data(), flags, false, this->indent() - 1, indent_cstr);
					}
					if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::cons) {
						s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
						if (strparam.size() > 0)
							ss << strparam << "::" << this->_m_name << '(';
						else
							ss <<  this->_m_name << '(';

						for (size_t i = 0; i < this->_m_params.size(); i++) {
							if (i > 0) {
								ss << ", ";
							}

							meta_variable *mvp = this->find_var_in_init_list(this->_m_params[i].name(), this->_m_params[i].modifier_data());
							if (mvp != nullptr) {
								if (b_find_map_member)
									init_buff += ", ";
								init_buff += mvp->name();
								init_buff += '(';
								init_buff += this->_m_params[i].name();
								init_buff += ')';								
								if (!b_find_map_member)
									b_find_map_member = true;

							} 							

							this->_m_params[i].append_as_param_string(ss, flags);
							
						}
						ss << ')';

						if (this->_m_base_init_param_list.size() > 1) {							
							s_gen_nl_to_sstream(ss, flags);
							s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
							ss << ": " << _m_base_init_param_list[0] << "(";
							for (size_t j = 1; j < this->_m_base_init_param_list.size(); j++) {
								if (j > 1) {									
									ss << ", ";
								}
									
								ss << _m_base_init_param_list[j];
							}	
							ss << ")";
						}

						if (init_buff.size() > 0) {
							s_gen_nl_to_sstream(ss, flags);
							s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);							
							if (this->_m_base_init_param_list.size() > 1)
							{
								ss << ", ";
							}
							ss << init_buff;
						}


					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::des) {
						s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
						if (strparam.size() <= 0) {
							if (this->_m_modifiers.test_value(mod_virtual))
								ss << "virtual ";
						}
						ss << strparam << "::" << '~' << this->_m_name << "()";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::copycons) {
						s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
						ss << strparam << "::" << this->_m_name << "(const " << this->_m_name << "& other)";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::copyops) {
						s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
						ss << strparam << "::" << this->_m_name << "& operator=(const " << this->_m_name << "& other)";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::movecons) {
						s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
						ss << strparam << "::" << this->_m_name << "(const " << this->_m_name << "&& other)";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::moveops) {
						s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
						ss << strparam << "::" << this->_m_name << "& operator=(const " << this->_m_name << "&& other)";

					}
					else if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::member) {
						s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);

						if (strparam.size() <= 0) {
							if (this->_m_modifiers.test_value(mod_virtual))
								ss << "virtual ";
						}


						if (this->_m_modifiers.test_value(mod_inline))
							ss << "inline ";
						
						else if (this->_m_modifiers.test_value(mod_static))
							ss << "static ";
						else if (this->_m_modifiers.test_value(mod_friend))
							ss << "friend ";

						if (this->_m_modifiers.test_value(mod_val_const)) {
							ss << "const ";
						}

						ss << this->_m_ret_type;			
						if (this->_m_modifiers.test_value(mod_is_ref)) {
							ss << "&";
						}
						ss << " ";


						if (strparam.size() > 0)
							ss << strparam << "::" << this->_m_name << '(';
						else
							ss << this->_m_name << '(';

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
				s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
				ss << "{";
				s_gen_nl(ss, flags);

				if (this->_m_func_type == ::pilo::core::rtti::meta_func_type::cons) {
					for (size_t i = 0; i < _m_member_var_list.size(); i++) {
						bool b_exist = exist_in_init_member_var_list(_m_member_var_list.at(i));
						if (!b_exist) {
							_m_member_var_list.at(i)->append_to_stringstream_cpp(ss, oflag_codeline_sep | oflag_need_value | oflag_supress_type | oflag_need_nl, "", indent_cstr, effect_indent + 1);
						}
					}
				}

				s_gen_lines_cpp(ss, this->_m_bodylines, effect_indent + 1, flags, indent_cstr);
				
				s_gen_indent_to_sstream(ss, effect_indent, indent_cstr);
				ss << "}";


				if (flags & oflag_need_nl) {
					s_gen_nl(ss, flags);
				}				

				return PILO_OK;
			}

		}
	}
}



