#include "../../pilo.hpp"
#include "../../tlv.hpp"
namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			
			void delete_tlv(::pilo::tlv* t)
			{
				::pilo::tlv::deallocate(t);
			}
		}
	}
}