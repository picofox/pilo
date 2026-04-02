#ifndef _pilo_core_rtti_meta_variable_hpp_
#define _pilo_core_rtti_meta_variable_hpp_

#include	"../../pilo.hpp"
#include	"../string/string_operation.hpp"
#include	"./meta_src_node.hpp"
#include	<sstream>


namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			class meta_variable : public meta_src_node
			{
			public:
				meta_variable(const std::string& namestr, const std::string& typestr, const std::string& valuestr, ::pilo::u8_t modifiers, ::pilo::u8_t accpriv)
					: meta_src_node(meta_node_type_enum::var), _m_name(namestr), _m_type(typestr), _m_value(valuestr), _m_modifiers(modifiers), _m_access_priv(accpriv)

				{

				}
				~meta_variable();

				// Inherited via meta_src_node
				::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags) const override;





				const ::pilo::bit_flag<::pilo::u16_t>& modifiers() { return _m_modifiers; }
				const std::string& name() const { return _m_name; }
				const std::string& type() const { return _m_type; }
				const std::string& value() const { return _m_value; }
				::pilo::u8_t access_priv() const { return _m_access_priv; }

				void mark_modifier(::pilo::u8_t m) { _m_modifiers.mark_index(m); }
				void set_name(const std::string& s) { _m_name = s; }
				void set_type(const std::string& s) { _m_type = s; }
				void set_value(const std::string& s) { _m_value = s; }
				void set_access_priv(::pilo::u8_t acp) { _m_access_priv = acp; }

				void reset()
				{
					_m_modifiers.reset();
					_m_name.clear();
					_m_type.clear();
					_m_value.clear();
					_m_access_priv = acc_priv_none;
				}

			protected:
				std::string _m_name;
				std::string _m_type;
				std::string _m_value;
				::pilo::bit_flag<::pilo::u16_t> _m_modifiers;
				::pilo::u8_t	_m_access_priv;

				

			};
		}
	}
}


#endif // !_pilo_core_rtti_meta_variable_hpp_




