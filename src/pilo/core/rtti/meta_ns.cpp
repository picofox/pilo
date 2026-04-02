#include	"./meta_ns.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			meta_ns::~meta_ns()
			{
			}
			::pilo::err_t meta_ns::append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags) const
			{
				PMC_UNUSED(ss);
				PMC_UNUSED(indent_cstr);
				PMC_UNUSED(flags);
				return PERR_OK;
			}
		}
	}
}



