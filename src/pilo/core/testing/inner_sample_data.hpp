#pragma once

#include <string>
#include "../../pilo.hpp"
#include "../string/string_operation.hpp"
#include "../memory/serializable_interface.hpp"
#include "../memory/serializable_header_interface.hpp"
#include "../memory/compactable_object_pool.hpp"
#include "../threading/spin_mutex.hpp"

namespace pilo
{
	namespace core
	{
		namespace testing
		{
			class inner_sample_data : public ::pilo::core::memory::serializable_interface
				, public ::pilo::core::memory::portable_compactable_object_pool<inner_sample_data, 1024, ::pilo::core::threading::spin_mutex>
			{
			public:
				static inner_sample_data* deserialise(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer);

			public:
				inner_sample_data() : _seq(0), _name("abcdefghi"), _sal(3.14f) 				
				{
					_sal=3.14f;
				}

				void set(::pilo::i32_t seq)
				{
					_seq = seq;
					//_name = "abcdefghi";
					//_sal = 3.14f;
				}

				bool validate(::pilo::i32_t seq) const
				{
					if (_seq != seq)
						return false;
					if (::pilo::core::string::strict_compare(_name.c_str(), 0, "abcdefghi", 0, -1) != 0)
						return false;
					if (_sal != 3.14f)
						return false;
					return true;
				}

				

				// Inherited via serializable_interface
				::pilo::err_t serialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) const override;
				::pilo::err_t deserialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) override;

			private:
				::pilo::i32_t		_seq;				
				std::string		_name;
				::pilo::f32_t		_sal;

				
			};
		}
	}
}