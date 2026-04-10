#ifndef _pilo_core_rtti_meta_ns_hpp_
#define _pilo_core_rtti_meta_ns_hpp_

#include	"../../pilo.hpp"
#include	<vector>
#include	"./meta_src_node.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			class meta_codeline : public meta_src_node
			{
			public:			
				meta_codeline(::pilo::u16_t modifiers, const std::string& codeline_string, const std::string& comment_string)
					: meta_src_node(meta_node_type_enum::codeline)
					, _m_modifiers(modifiers)
					, _m_codeline(codeline_string), _m_comment(comment_string)


				{

				}

				~meta_codeline();

				// Inherited via meta_src_node
				::pilo::err_t append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags, const std::string& strparam) const override;

				std::string codeline() const { return _m_codeline;  }
				std::string comment() const { return _m_comment; }

				bool is_comment() const { return (_m_codeline.empty() && (!_m_comment.empty())); }
				bool is_blank() const { return (_m_codeline.empty() && _m_comment.empty()); }
				bool is_code() const { return ((!_m_codeline.empty()) && (_m_comment.empty())); }
				//bool has_code() const { return (! _m_codeline.empty()); }				

			protected:
				::pilo::bit_flag<::pilo::u16_t> _m_modifiers;
				std::string	_m_codeline;
				std::string _m_comment;
				
				


				

			};



		}
	}
}


#endif // !_pilo_core_rtti_meta_ns_hpp_




