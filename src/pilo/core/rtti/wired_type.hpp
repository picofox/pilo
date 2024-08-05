#pragma once

#include "../../types.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			class wired_type
			{


			public:
				const static ::pilo::u16_t value_type_na = 0;
				const static ::pilo::u16_t value_type_i8 = 1;
				const static ::pilo::u16_t value_type_u8 = 2;
				const static ::pilo::u16_t value_type_i16 = 3;
				const static ::pilo::u16_t value_type_u16 = 4;
				const static ::pilo::u16_t value_type_i32 = 5;
				const static ::pilo::u16_t value_type_u32 = 6;
				const static ::pilo::u16_t value_type_i64 = 7;
				const static ::pilo::u16_t value_type_u64 = 8;
				const static ::pilo::u16_t value_type_boolean = 9;
				const static ::pilo::u16_t value_type_f32 = 10;
				const static ::pilo::u16_t value_type_f64 = 11;
				const static ::pilo::u16_t value_type_bytes = 12;				
				const static ::pilo::u16_t value_type_str = 13;
				const static ::pilo::u16_t value_type_tlv = 14;
				const static ::pilo::u16_t value_type_intrincs_count = 15;


				const static ::pilo::u16_t wired_type_count = 1024;

			public:
				const static ::pilo::u8_t obj_group_id = 0;
				const static ::pilo::u8_t msg_group_id = 1;

				const static ::pilo::u8_t wrapper_na = 0;
				const static ::pilo::u8_t wrapper_single = 1;
				const static ::pilo::u8_t wrapper_array = 2;
				const static ::pilo::u8_t wrapper_dict = 3;

				const static ::pilo::u8_t key_type_na = 0;
				const static ::pilo::u8_t key_type_i8 = 1;
				const static ::pilo::u8_t key_type_u8 = 2;
				const static ::pilo::u8_t key_type_i16 = 3;
				const static ::pilo::u8_t key_type_u16 = 4;
				const static ::pilo::u8_t key_type_i32 = 5;
				const static ::pilo::u8_t key_type_u32 = 6;
				const static ::pilo::u8_t key_type_i64 = 7;
				const static ::pilo::u8_t key_type_u64 = 8;
				const static ::pilo::u8_t key_type_str = 9;
				const static ::pilo::u8_t key_type_count = 10;

				wired_type() : _attribute(0), _flags(0), _value_type(0) {}
				wired_type(::pilo::u8_t attrs, ::pilo::u8_t flags, ::pilo::u16_t vt) :_attribute(attrs), _flags(flags), _value_type(vt) {}
				wired_type(bool is_msg, ::pilo::u8_t wrapper_type, ::pilo::u8_t key_type, ::pilo::u16_t value_type) : _flags(0)
				{
					set(is_msg, wrapper_type, key_type, value_type);
				}

				::pilo::u8_t attribute() const { return _attribute; }
				::pilo::u8_t flags() const { return _flags; }

				inline void set_flag(::pilo::u8_t f)
				{
					_flags |= f;
				}

				inline void clear_flag(::pilo::u8_t f)
				{
					_flags &= ~f;
				}

				inline bool test_flag(::pilo::u8_t f) const
				{
					return (_flags & f);
				}

				bool valid() const
				{
					if (value_type() != ::pilo::core::rtti::wired_type::value_type_na && wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_na)
					{
						return true;
					}
					return false;
				}

				::pilo::err_t set_single_type(::pilo::u16_t value_type)
				{
					if (this->wrapper_type() > ::pilo::core::rtti::wired_type::wrapper_single )
						return ::pilo::mk_perr(PERR_NO_OVERWRITING);
					else if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_single)
					{
						if (_value_type != value_type)
						{
							set_value_type(value_type);
						}						
					}
					else if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_na)
					{
						set(false, wrapper_single, key_type_na, value_type);
					}
					else
					{
						return ::pilo::mk_perr( PERR_MIS_DATA_TYPE);
					}
					return PILO_OK;
				}

				void set(bool is_msg, ::pilo::u8_t wrapper_type, ::pilo::u8_t key_type, ::pilo::u16_t value_type)
				{
					::pilo::u8_t val_is_msg = is_msg ? 1 : 0;
					_attribute = (key_type & 0xF) | ((wrapper_type & 0x3) << 4) | ((val_is_msg & 0x1) << 6);
					_value_type = value_type;
				}
				void set(::pilo::u8_t attr, ::pilo::u8_t flags, ::pilo::u16_t vtype)
				{
					_attribute = attr;
					_flags = flags;
					_value_type = vtype;
				}

				void set_wrapper_type(::pilo::u16_t wrapper_type)
				{
					_attribute &= ~((::pilo::u16_t)0x3 << 4);
					_attribute |= ((wrapper_type & 0x3) << 4);
				}

				void set_key_type(::pilo::u16_t key_type)
				{
					_attribute &= ~((::pilo::u16_t)0xF);
					_attribute |= ((key_type & 0xF));
				}

				void set_value_type(::pilo::u16_t value_type, bool is_message = false)
				{
					_value_type = value_type;
					if (is_message)
						set_is_msg(true);
					else
						set_is_msg(false);
				}

				void set_is_msg(bool is_msg)
				{
					if (is_msg)
					{
						_attribute |= (((::pilo::u16_t)0x1) << 6);
					}
					else
					{
						_attribute &= ~((::pilo::u16_t)0x1 << 6);
					}
				}

				::pilo::u8_t key_type() const
				{
					return  (::pilo::u8_t)((_attribute) & 0xF);
				}

				::pilo::u8_t wrapper_type() const
				{
					return  (::pilo::u8_t)((_attribute >> 4) & 0x3);
				}

				bool is_single() const
				{
					return ((::pilo::u8_t)((_attribute >> 4) & 0xF) == wrapper_single);
				}

				bool is_array() const
				{
					return ((::pilo::u8_t)((_attribute >> 4) & 0xF) == wrapper_array);
				}

				bool value_type_in_range(::pilo::u16_t low, ::pilo::u16_t high) const
				{
					if (_value_type >= low && _value_type < high)
						return true;
					else
						return false;
				}


				::pilo::u16_t value_type() const
				{
					return _value_type;
				}

				::pilo::u8_t category_id() const
				{
					return ((_attribute >> 6) & 0x1);
				}

				void to_string(std::stringstream & ss) const
				{
					ss << "\"a\":" << (int) _attribute <<",\"f\":" << (int) _flags << ",\"t\":" << (int) _value_type;
				}

				std::string to_string() const
				{
					std::stringstream ss;
					ss << "m:" << category_id() << " wt:" << (int)wrapper_type() << " kt:" << (int)key_type() << " f:" << (int)_flags << " vt:" << value_type();
					return ss.str();
				}

				void reset()
				{
					_attribute = 0;
					_flags = 0;
					_value_type = 0;
				}

			private:
				::pilo::u8_t	_attribute;
				::pilo::u8_t	_flags;
				::pilo::u16_t	_value_type;
			};
		}
	}
	
}