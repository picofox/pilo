#include "meta_src_node.hpp"


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


		}
	}
}
