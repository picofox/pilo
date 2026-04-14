#include    "./meta_ns.hpp"

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            meta_ns::~meta_ns()
            {
            }
            ::pilo::err_t meta_ns::append_to_stringstream_cpp(std::stringstream& ss, ::pilo::u32_t flags, const std::string& strparam, const char* indent_cstr) const
            {
                PMC_UNUSED(ss);
                PMC_UNUSED(indent_cstr);
                PMC_UNUSED(flags);
                PMC_UNUSED(strparam);
                return PERR_OK;
            }
        }
    }
}



