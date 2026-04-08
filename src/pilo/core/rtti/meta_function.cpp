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
			::pilo::err_t meta_function::append_to_stringstream_cpp(std::stringstream& ss, const char* indent_cstr, ::pilo::u32_t flags) const
			{
				PMC_UNUSED(ss);
				PMC_UNUSED(indent_cstr);
				PMC_UNUSED(flags);

				return PILO_OK;
			}

			::pilo::err_t meta_function::add_param(::pilo::u16_t modifiers, const std::string& namestr, const std::string& typestr, const std::string& valuestr)
			{
				meta_variable var(modifiers, 0, 0, namestr, typestr, valuestr);
				_m_params.emplace_back(var);
				return PILO_OK;
			}

		}
	}
}



