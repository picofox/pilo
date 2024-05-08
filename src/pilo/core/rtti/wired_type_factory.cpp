#include "./wired_type_factory.hpp"
#include "../../tlv.hpp"

namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            void wired_type_factory::_load()
            {
                _allocators[0][wired_type::value_type_tlv] = (wired_type_allocator_type) ::pilo::tlv::allocate;
                _deallocators[0][wired_type::value_type_tlv] = (wired_type_deallocator_type) ::pilo::tlv::deallocate;

            }
        }
    }
}