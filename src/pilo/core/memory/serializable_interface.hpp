#pragma once

#include "./serializable_header_interface.hpp"

namespace pilo
{
    namespace core
    {
        namespace memory
        {
     
            class serializable_interface
            {
            public:
                virtual ~serializable_interface() {}
                    
                virtual ::pilo::err_t serialize(serializable_header_interface * header, byte_buffer_interface * byte_buffer) const = 0;
                virtual ::pilo::err_t deserialize(serializable_header_interface * header, byte_buffer_interface * byte_buffer) = 0;
            };
        }
    }
}