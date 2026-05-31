#pragma once

#include "../../pilo.hpp"
#include "./wired_type.hpp"
#include "../memory/serializable_interface.hpp"


namespace pilo
{
    namespace core
    {
        namespace rtti
        {
            class wired_type_factory
            {
            public:
                typedef ::pilo::core::memory::serializable_interface* (*wired_type_allocator_type)();
                typedef void (*wired_type_deallocator_type)(::pilo::core::memory::serializable_interface*);
                wired_type_factory() : _allocators{ nullptr }, _deallocators{ nullptr }

                {
                    _load();
                }

            public:
                ::pilo::core::memory::serializable_interface* allocate(::pilo::u8_t is_message, ::pilo::u16_t id)
                {
                    return _allocators[is_message][id]();
                }

                void deallocate(::pilo::core::memory::serializable_interface* obj, ::pilo::u8_t is_message, ::pilo::u16_t id)
                {
                    _deallocators[is_message][id](obj);
                }


            private:
                void _load();



            private:
                wired_type_allocator_type   _allocators[2][PILO_UINT16_MAX];
                wired_type_deallocator_type _deallocators[2][PILO_UINT16_MAX];

            };
        }
    }
}