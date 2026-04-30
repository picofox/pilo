#include	"./meta_function.hpp"

namespace pilo
{
	namespace core
	{
		namespace autogen
		{
			meta_function::~meta_function()
			{
			}
			::pilo::err_t meta_function::append_to_stringstream_cpp(std::stringstream& ss,  ::pilo::u32_t flags, const std::string& strparam,  ::pilo::i16_t effect_indent) const
			{
				if (flags & oflag_dec)
					return append_cpp_declare_string(ss, flags);
				else 
					return append_cpp_definition_string(ss, flags, strparam, effect_indent);

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



			::pilo::err_t meta_function::append_cpp_declare_string(std::stringstream& ss, ::pilo::u32_t flags) const
			{	
				if (this->_m_modifiers.test_value(mod_inline)) {
					return append_cpp_definition_string(ss, flags, "", -1);
				}

				if (this->_m_func_type == meta_func_type::c) {
					s_gen_indent_to_sstream(ss, this->indent());
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
						s_gen_priv(ss, this->_m_modifiers.data(), flags, false, this->indent() - 1);
					}

					if (this->_m_func_type == meta_func_type::cons) {
						s_gen_indent_to_sstream(ss, this->indent());						
						ss << this->_m_name << '(';
						for (size_t i = 0; i < this->_m_params.size(); i++) {
							if (i > 0) {
								ss << ", ";
							}
							this->_m_params[i].append_as_param_string(ss, flags);
						}
						ss << ')';
					
					} else if (this->_m_func_type == meta_func_type::des) {						
						s_gen_indent_to_sstream(ss, this->indent());
						if (this->_m_modifiers.test_value(mod_virtual))
							ss << "virtual ";
						ss << '~'  << this->_m_name << "()";

					} else if (this->_m_func_type == meta_func_type::copycons) {
						s_gen_indent_to_sstream(ss, this->indent());
						ss << this->_m_name << "(const " << this->_m_name  << "& rhs) noexcept";

					} else if (this->_m_func_type == meta_func_type::copyops) {
						s_gen_indent_to_sstream(ss, this->indent());
						ss << this->_m_name  << "& operator=(const " << this->_m_name << "& rhs) noexcept";

					} else if (this->_m_func_type == meta_func_type::movecons) {
						s_gen_indent_to_sstream(ss, this->indent());
						ss << this->_m_name << "(" << this->_m_name << "&& rhs) noexcept";

					} else if (this->_m_func_type == meta_func_type::moveops) {
						s_gen_indent_to_sstream(ss, this->indent());
						ss << this->_m_name << "& operator=(" << this->_m_name << "&& rhs) noexcept";

					} else if (this->_m_func_type == meta_func_type::getter) {
						s_gen_indent_to_sstream(ss, this->indent());
						if (this->_m_modifiers.test_value(mod_inline))
							ss << "inline ";
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

						if (this->_m_modifiers.test_value(mod_const)) {
							ss << " const";
						}
						if (this->_m_modifiers.test_value(mod_noex)) {
							ss << " noexcep";
						}

					} else if (this->_m_func_type == meta_func_type::setter) {
						s_gen_indent_to_sstream(ss, this->indent());
						if (this->_m_modifiers.test_value(mod_inline))
							ss << "inline ";
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

						if (this->_m_modifiers.test_value(mod_const)) {
							ss << " const";
						}
						if (this->_m_modifiers.test_value(mod_noex)) {
							ss << " noexcep";
						}
					}
					
					else if (this->_m_func_type == meta_func_type::member) {
						s_gen_indent_to_sstream(ss, this->indent());
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

						if (this->_m_modifiers.test_value(mod_const)) {
							ss << " const";
						}

						if (this->_m_modifiers.test_value(mod_noex)) {
							ss << " noexcep";
						}

						if (this->_m_modifiers.test_value(mod_abstract)) {
							ss << " = 0";
						}

					} else {
						 ::pilo::mk_perr(PERR_INVALID_PARAM);;
					}
				}

				if (flags & oflag_codeline_sep) {
					ss << ';';
				}
				s_gen_nl(ss, flags);

				return PILO_OK;
			}

			::pilo::err_t meta_function::append_cpp_definition_string(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam,::pilo::i16_t effect_indent) const
			{
				if (effect_indent == -1)
					effect_indent = this->indent();

				std::string init_buff;
				bool b_find_map_member = false;

				if (this->_m_func_type == meta_func_type::c) {
					s_gen_indent_to_sstream(ss, this->indent());
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
						s_gen_priv(ss, this->_m_modifiers.data(), flags, false, this->indent() - 1);
					}
					if (this->_m_func_type == meta_func_type::cons) {
						s_gen_indent_to_sstream(ss, effect_indent);
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
							ss << g_autogen_config.newline_sep();
							s_gen_indent_to_sstream(ss, effect_indent);
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
							ss << g_autogen_config.newline_sep();
							s_gen_indent_to_sstream(ss, effect_indent);							
							if (this->_m_base_init_param_list.size() > 1)
							{
								ss << ", ";
							}
							ss << init_buff;
						}


					}
					else if (this->_m_func_type == meta_func_type::des) {
						s_gen_indent_to_sstream(ss, effect_indent);
						if (strparam.size() <= 0) {
							if (this->_m_modifiers.test_value(mod_virtual))
								ss << "virtual ";
						}
						if (strparam.size() > 0)
							ss << strparam << "::" << '~' << this->_m_name << "()";
						else
							ss << '~' << this->_m_name << "()";

					}
					else if (this->_m_func_type == meta_func_type::copycons) {
						s_gen_indent_to_sstream(ss, effect_indent);
						if (strparam.size() > 0)
							ss << strparam << "::" << this->_m_name << "(const " << this->_m_name << "& rhs) noexcept";
						else
							ss << this->_m_name << "(const " << this->_m_name << "& rhs) noexcept";

						if (this->test_modifier(mod_autofill)) {
							if (this->_m_base_clsname.size() > 0) {
								ss << g_autogen_config.newline_sep();
								s_gen_indent_to_sstream(ss, effect_indent);
								ss << ": " << _m_base_clsname << "(rhs)";
							}

							if (this->_m_member_var_list.size() > 0) {
								ss << g_autogen_config.newline_sep();
								s_gen_indent_to_sstream(ss, effect_indent);
								for (size_t i = 0; i < this->_m_member_var_list.size(); i++) {
									if (i > 0 || (this->_m_base_clsname.size() > 0)) {
										ss << ", ";
									}
									ss << _m_member_var_list[i]->name();
									ss << "(rhs.";
									ss << _m_member_var_list[i]->name();
									ss << ')';
								}
							}
						}
						
					}
					else if (this->_m_func_type == meta_func_type::copyops) {
						s_gen_indent_to_sstream(ss, effect_indent);
						if (strparam.size() > 0)
							ss << this->_m_name << "& " << this->_m_name<< "::" << "operator=(const " << this->_m_name << "& rhs) noexcept";
						else
							ss << this->_m_name << "& " << "operator=(const " << this->_m_name << "& rhs) noexcept";

					}
					else if (this->_m_func_type == meta_func_type::movecons) {
						s_gen_indent_to_sstream(ss, effect_indent);
						if (strparam.size() > 0)
							ss << strparam << "::" << this->_m_name << "(" << this->_m_name << "&& rhs) noexcept";
						else
							ss << this->_m_name << "(" << this->_m_name << "&& rhs) noexcept";

						if (this->test_modifier(mod_autofill)) {
							if (this->_m_base_clsname.size() > 0) {
								ss << g_autogen_config.newline_sep();
								s_gen_indent_to_sstream(ss, effect_indent);
								ss << ": " << _m_base_clsname << "(std::move(rhs))";
							}

							if (this->_m_member_var_list.size() > 0) {
								ss << g_autogen_config.newline_sep();
								s_gen_indent_to_sstream(ss, effect_indent);
								for (size_t i = 0; i < this->_m_member_var_list.size(); i++) {
									if (i > 0 || (this->_m_base_clsname.size() > 0)) {
										ss << ", ";
									}
									ss << _m_member_var_list[i]->name();
									if (this->_m_member_var_list.at(i)->test_modifier(mod_non_basetype) || this->_m_member_var_list.at(i)->test_modifier(mod_isstr)) {
										ss << "(" << "std::move(" << "rhs." << _m_member_var_list[i]->name() << ")";
									}
									else {
										ss << "(rhs." << _m_member_var_list[i]->name();
									}

									ss << ')';
								}
							}
						}					

					} else if (this->_m_func_type == meta_func_type::moveops) {
						s_gen_indent_to_sstream(ss, effect_indent);
						if (strparam.size() > 0)
							ss << this->_m_name << "& " << this->_m_name << "::" << "operator=(" << this->_m_name << "&& rhs)  noexcept";
						else
							ss << this->_m_name << "& " << "operator=(" << this->_m_name << "&& rhs) noexcept";

					} else if (this->_m_func_type == meta_func_type::getter) {
						s_gen_indent_to_sstream(ss, effect_indent);
						if (this->_m_modifiers.test_value(mod_inline))
							ss << "inline ";

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

						if (this->_m_modifiers.test_value(mod_const)) {
							ss << " const";
						}

						if (this->_m_modifiers.test_value(mod_noex)) {
							ss << " noexcep";
						}
					} else if (this->_m_func_type == meta_func_type::setter) {
						s_gen_indent_to_sstream(ss, effect_indent);
						if (this->_m_modifiers.test_value(mod_inline))
							ss << "inline ";

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

						if (this->_m_modifiers.test_value(mod_const)) {
							ss << " const";
						}

						if (this->_m_modifiers.test_value(mod_noex)) {
							ss << " noexcep";
						}

					} else if (this->_m_func_type == meta_func_type::member) {
						s_gen_indent_to_sstream(ss, effect_indent);

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

						if (this->_m_modifiers.test_value(mod_const)) {
							ss << " const";
						}

						if (this->_m_modifiers.test_value(mod_noex)) {
							ss << " noexcep";
						}

					}
					else {
						::pilo::mk_perr(PERR_INVALID_PARAM);;
					}					
				}

				s_gen_nl(ss, flags);
				s_gen_indent_to_sstream(ss, effect_indent);
				ss << "{";
				s_gen_nl(ss, flags);

				if (this->_m_func_type == meta_func_type::cons && this->test_modifier(mod_autofill)) {
					for (size_t i = 0; i < _m_member_var_list.size(); i++) {
						bool b_exist = exist_in_init_member_var_list(_m_member_var_list.at(i));
						if (!b_exist) {
							_m_member_var_list.at(i)->append_to_stringstream_cpp(ss, oflag_codeline_sep | oflag_need_value | oflag_supress_type | oflag_need_nl, "", effect_indent + 1);
						}
					}

				} else if (this->_m_func_type == meta_func_type::movecons && this->test_modifier(mod_autofill)) {
					for (size_t i = 0; i < _m_member_var_list.size(); i++) {
						if (_m_member_var_list.at(i)->test_modifier(mod_isptr)) {
							s_gen_indent_to_sstream(ss, effect_indent + 2);
							ss << "rhs." << _m_member_var_list.at(i)->name() << " = nullptr" << ";";
							ss << g_autogen_config.newline_sep();
						}
					}					
				}
				else if (this->_m_func_type == meta_func_type::moveops && this->test_modifier(mod_autofill)) {
					s_gen_indent_to_sstream(ss, effect_indent + 1);
					ss << "if (this != &rhs) {";
					ss << g_autogen_config.newline_sep();
					if (this->_m_base_clsname.size() > 0) {
						s_gen_indent_to_sstream(ss, effect_indent + 2);
						ss << this->_m_base_clsname << "::operator=(std::move(rhs));";
						ss << g_autogen_config.newline_sep();
					}
					
					for (size_t i = 0; i < _m_member_var_list.size(); i++) {
						s_gen_indent_to_sstream(ss, effect_indent + 2);
						ss << _m_member_var_list.at(i)->name() << " = " << "rhs." << _m_member_var_list.at(i)->name() << ";";
						ss << g_autogen_config.newline_sep();

						if (_m_member_var_list.at(i)->test_modifier(mod_isptr)) {
							s_gen_indent_to_sstream(ss, effect_indent + 2);
							ss << "rhs." << _m_member_var_list.at(i)->name() << " = nullptr" << ";";
							ss << g_autogen_config.newline_sep();
						}
					}
					s_gen_indent_to_sstream(ss, effect_indent + 1);
					ss << "}";
					ss << g_autogen_config.newline_sep();
					s_gen_indent_to_sstream(ss, effect_indent + 1);
					ss << "return *this;";
					ss << g_autogen_config.newline_sep();

				} else if (this->_m_func_type == meta_func_type::copyops && this->test_modifier(mod_autofill)) {
					s_gen_indent_to_sstream(ss, effect_indent + 1);
					ss << "if (this != &rhs) {";
					ss << g_autogen_config.newline_sep();
					if (this->_m_base_clsname.size() > 0) {
						s_gen_indent_to_sstream(ss, effect_indent + 2);
						ss << this->_m_base_clsname << "::operator=(rhs);";
						ss << g_autogen_config.newline_sep();
					}
					for (size_t i = 0; i < _m_member_var_list.size(); i++) {
						s_gen_indent_to_sstream(ss, effect_indent + 2);
						if (this->_m_member_var_list.at(i)->test_modifier(mod_non_basetype) || this->_m_member_var_list.at(i)->test_modifier(mod_isstr)) {
							ss << _m_member_var_list.at(i)->name() << " = " << "std::move(" << "rhs." << _m_member_var_list.at(i)->name() << ");";
						} else {
							ss << _m_member_var_list.at(i)->name() << " = " << "rhs." << _m_member_var_list.at(i)->name() << ";";
						}
						
						ss << g_autogen_config.newline_sep();
					}
					s_gen_indent_to_sstream(ss, effect_indent + 1);
					ss << "}" << g_autogen_config.newline_sep();
					s_gen_indent_to_sstream(ss, effect_indent + 1);
					ss << "return *this;";
					ss << g_autogen_config.newline_sep();
									
				}

				s_gen_lines_cpp(ss, this->_m_bodylines, effect_indent + 1, flags);

				if (this->_m_func_type == meta_func_type::member) {
					if (this->_m_ret_type.size() > 0 || ! this->_m_ret_type.compare("void")) {
						s_gen_indent_to_sstream(ss, effect_indent + 1);
						ss << "return " << this->_m_ret_type << "();" << g_autogen_config.newline_sep();
					} 
				}
				
				s_gen_indent_to_sstream(ss, effect_indent);
				ss << "}";


				if (flags & oflag_need_nl) {
					s_gen_nl(ss, flags);
				}				

				return PILO_OK;
			}

		}
	}
}



