#ifndef _pilo_core_rtti_meta_function_hpp_
#define _pilo_core_rtti_meta_function_hpp_

#include	"../../pilo.hpp"
#include	<vector>
#include	"./meta_variable.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			class meta_function : public meta_src_node
			{
			public:
				meta_function(::pilo::u16_t modifiers, ::pilo::u8_t accpriv, meta_func_type func_ype, const std::string& name, const std::string ret_type)
					:meta_src_node(meta_node_type_enum::var)
					, _m_modifiers(modifiers), _m_access_priv(accpriv), _m_func_type(func_ype)
					, _m_name(name), _m_ret_type(ret_type) {
				}
					

				~meta_function();

				// Inherited via meta_src_node
				::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags) const override;

				::pilo::err_t add_param(::pilo::u16_t modifiers, const std::string& namestr, const std::string& typestr, const std::string& valuestr);

			protected:
				::pilo::bit_flag<::pilo::u16_t> _m_modifiers;
				::pilo::u8_t					_m_access_priv;
				meta_func_type					_m_func_type;
				std::string						_m_name;
				std::string						_m_ret_type;
				std::string						_m_dec;
				std::string						_m_def;
				std::vector<meta_variable>		_m_params;
			};

		}
	}
}


#endif // !_pilo_core_rtti_meta_function_hpp_




