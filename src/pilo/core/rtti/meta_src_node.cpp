#include "meta_src_node.hpp"
#include <vector>

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			::pilo::core::algorithm::uint_sequence_generator<unsigned int> meta_src_node::_s_seq_generator = ::pilo::core::algorithm::uint_sequence_generator<unsigned int>();

			meta_src_node::meta_src_node(meta_node_type_enum mnte) : _m_type(mnte), _m_indent(0)
			{
				_m_id = _s_seq_generator.next();
			}

			meta_src_node::~meta_src_node()
			{
			}


			void s_gen_lines_cpp(std::stringstream& ss, const std::vector<std::unique_ptr<meta_src_node>>& lines, ::pilo::i16_t indent, ::pilo::u32_t flags, const char* indent_cstr)
			{				
				PMC_UNUSED(indent);
				for (auto it = lines.cbegin(); it != lines.cend(); it++) {
					(*it)->append_to_stringstream_cpp(ss, indent_cstr, flags, "");
				}
			}

		}
	}
}
