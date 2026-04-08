#include	"./meta_variable.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			meta_variable::~meta_variable()
			{
			}

			::pilo::err_t meta_variable::append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags) const
			{			
				if ((flags & oflag_need_priv) && this->_m_access_priv != acc_priv_none) {
					s_gen_indent_to_sstream(ss, pilo_min<::pilo::i16_t>(this->indent() - 1, 1), indent_cstr);
					ss << s_cpp_acces_priv_str_mapper.get_value(this->_m_access_priv);
					ss << ':';					
					s_gen_nl_to_sstream(ss, flags);					
				}
				
				s_gen_indent_to_sstream(ss, this->indent(), indent_cstr);
				
				if (_m_modifiers.test_value(_mod_bit_mutable))
					ss << s_cpp_meta_id_value_mapper.get_value(_mod_bit_mutable) << ' ';
				if (_m_modifiers.test_value(mod_static))
					ss << s_cpp_meta_id_value_mapper.get_value(_mod_bit_static) << ' ';
				if (_m_modifiers.test_index(_mod_bit_ptr_const))
					ss << s_cpp_meta_id_value_mapper.get_value(_mod_bit_ptr_const) << ' ';
				if (_m_modifiers.test_index(_mod_bit_volatile))
					ss << s_cpp_meta_id_value_mapper.get_value(_mod_bit_volatile) << ' ';
				ss << _m_type << ' ';
				if (_m_modifiers.test_index(_mod_bit_val_const))
					ss << s_cpp_meta_id_value_mapper.get_value(_mod_bit_val_const) << ' ';
				ss << _m_name;
				s_gen_value_assignment_cppstr(ss, _m_value, _m_modifiers);
				if (flags & oflag_codeline_sep) {
					ss << ';';
				}
				s_gen_nl(ss, flags);				

				return PERR_OK;
			}

		}
	}
}