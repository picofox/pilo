#pragma once

#include <string>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <vector>
#include <deque>
#include <set>
#include <list>
#include <map>
#include "./pilo.hpp"
#include "./core/memory/compactable_autoreset_object_pool.hpp"
#include "./core/memory/compactable_object_pool.hpp"
#include "./core/threading/native_mutex.hpp"
#include "./core/rtti/wired_type.hpp"
#include "./core/string/string_operation.hpp"
#include "./core/rtti/wired_type_conversion.hpp"
#include "./core/memory/serializable_interface.hpp"
#include "./core/pattern/function_dispatcher.hpp"
#include "./core/rtti/wired_type_deleter.hpp"
#include "./core/rtti/wired_type_map_comparation.hpp"
#include "./core/rtti/wired_type_deque.hpp"
#include "./core/stat/pool_object_stat_info.hpp"
#include "./core/rtti/wired_type_map_creation.hpp"

#define PMI_TLV_TO_STRING   ((::pilo::u16_t) 0)

#define PILO_TLV_POOL_ALLOCATOR_STEP_SIZE	(4096)

using namespace ::pilo::core::rtti;

namespace pilo
{

    class tlv
		: public ::pilo::core::memory::portable_compactable_autoreset_object_pool<::pilo::tlv, SP_PMI__TLV_STEP, ::pilo::core::threading::native_mutex>
    {
	public:
		const static ::pilo::u8_t	FlagBytesAsCStr = 0x01;

	public:
		typedef ::pilo::core::memory::portable_compactable_autoreset_object_pool<::pilo::tlv, SP_PMI__TLV_STEP, ::pilo::core::threading::native_mutex> pool_type;

	public:
		friend bool core::rtti::map_equals(::pilo::u8_t a_key_type, ::pilo::u16_t a_val_type, const char* a_data, ::pilo::u8_t b_key_type, ::pilo::u16_t b_val_type, const char* b_data, ::pilo::err_t* err);
		friend class ::pilo::core::pattern::function_dispatcher<::pilo::err_t(::pilo::tlv*, ::pilo::core::memory::byte_buffer_interface*), ::pilo::core::rtti::wired_type::value_type_intrincs_count>;
		friend void stc_wrapper_clear_bool(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_i8(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_u8(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_i16(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_u16(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_i32(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_u32(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_i64(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_u64(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_f32(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_f64(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_bytes(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_tlv(::pilo::tlv* t, bool compact);
		friend void stc_wrapper_clear_str(::pilo::tlv* t, bool compact);
		friend ::pilo::err_t single_tlv_deserialize_na(::pilo::tlv*, ::pilo::core::memory::byte_buffer_interface*);
		friend ::pilo::err_t single_tlv_deserialize_i8(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_u8(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_i16(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_u16(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_i32(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_u32(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_i64(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_u64(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_bool(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_f32(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_f64(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_bytes(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);
		friend ::pilo::err_t single_tlv_deserialize_tlv(::pilo::tlv*, ::pilo::core::memory::byte_buffer_interface*);
		friend ::pilo::err_t single_tlv_deserialize_str(::pilo::tlv* t, ::pilo::core::memory::byte_buffer_interface* bb);


    public:
		~tlv()
		{
			reset();
		}
		tlv() :               _size(0), _type(false, wired_type::wrapper_na, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_na), _dynamic_data(nullptr) {}
		tlv(::pilo::i8_t v) : _size(1), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_i8),_i8(v) {}
		tlv(::pilo::u8_t v) : _size(1), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_u8), _u8(v) {}
		tlv(::pilo::i16_t v) : _size(2), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_i16), _i16(v) {}
		tlv(::pilo::u16_t v) : _size(2), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_u16), _u16(v) {}
		tlv(::pilo::i32_t v) : _size(4), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_i32), _i32(v) {}
		tlv(::pilo::u32_t v) : _size(4), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_u32), _u32(v) {}
		tlv(::pilo::i64_t v) : _size(8), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_i64), _i64(v) {}
		tlv(::pilo::u64_t v) : _size(8), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_u64), _u64(v) {}
		tlv(::pilo::f32_t v) : _size(4), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_f32), _f32(v) {}
		tlv(::pilo::f64_t v) : _size(8), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_f64), _f64(v) {}
		tlv(bool v) : _size(1), _type(false, wired_type::wrapper_single, wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_boolean), _i8((::pilo::i8_t)v) {}
		tlv(const char* str, ::pilo::i32_t len, bool is_str, bool adopt)
		{
			if (is_str)
				_set_cstr(str, len, adopt);
			else
				_set_bytes(str, len, adopt);
		}
		tlv(std::string& str)
		{
			set(str);
		}

		inline void set_flag(::pilo::u8_t f)
		{
			_type.set_flag(f);
		}

		inline void clear_flag(::pilo::u8_t f)
		{
			_type.clear_flag(f);
		}

		inline bool test_flag(::pilo::u8_t f) const
		{
			return _type.test_flag(f);
		}

		inline ::pilo::i32_t as_str_length() const
		{
			if (_dynamic_data == nullptr)
				return -1;

			if (_size < 0) {
				return -1;
			}				
			else if (_size == 0) {
				return 0;
			}				

			if (_dynamic_data[_size-1] == 0)
				return _size-1;
			else
				return _size;

		}

		inline ::pilo::err_t assign_as_str(std::string & str) const
		{
			::pilo::err_t err = PILO_OK;

			if (_type.wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
				return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			}

			if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
				str = this->as_str(&err, nullptr);

			}
			else if (_type.value_type() == ::pilo::core::rtti::wired_type::value_type_bytes)
			{
				if (_dynamic_data == nullptr)
					return ::pilo::mk_perr(PERR_NULL_PTR);

				::pilo::i32_t data_size = _size;
				if (this->test_flag(::pilo::tlv::FlagBytesAsCStr)) {
					data_size--;
				}

				if (data_size == 0) {
					str.clear();
					return PILO_OK;
				}
				else if (data_size < 0)
					return ::pilo::mk_perr(PERR_INV_LEN);

				if (this->test_flag(::pilo::tlv::FlagBytesAsCStr)) {
					str.assign(_dynamic_data, data_size);
				}
				else {
					str.assign(_dynamic_data, data_size);
				}				
			}
			else {
				str = this->as_str(&err, nullptr);
			}			

			return err;
		}

		template<typename INT_T, ::pilo::i32_t capacity>
		::pilo::err_t to_flags(::pilo::bit_flag<INT_T> & flags
			, const char* delim, ::pilo::i32_t delim_len
			, const char* const str_dict[]) const
		{
			INT_T ev = 0;
			if (this->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
				return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			}

			if (_dynamic_data == nullptr)
				return ::pilo::mk_perr(PERR_NULL_PTR);

			if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_na) {
				return ::pilo::mk_perr(PERR_INV_VAL_TYPE);
			}
			else if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
				::pilo::core::string::compose_strlist_to_flags<INT_T, capacity>(ev, this->_dynamic_data, this->as_str_length(), delim, delim_len, str_dict);
				flags.set(ev);
				return PILO_OK;
			}
			else if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
				std::string* p = (std::string*)this->_dynamic_data;
				::pilo::core::string::compose_strlist_to_flags<INT_T, capacity>(ev, p->c_str(), (::pilo::i32_t) p->size(), delim, delim_len, str_dict);
				flags.set(ev);
				return PILO_OK;
			}
			else if (this->value_type() > ::pilo::core::rtti::wired_type::value_type_na
				&& this->value_type() < ::pilo::core::rtti::wired_type::value_type_boolean) {
				ev = (INT_T)this->as_i64(nullptr);
				flags.set(ev);
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}


		template<typename RET_ID_T, typename INT_T>
		::pilo::err_t to_enum_id(RET_ID_T& ret, const char* const dict[], INT_T count, bool ignore_case) const
		{
			if (this->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_single) {
				return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			}

			if (_dynamic_data == nullptr)
				return ::pilo::mk_perr(PERR_NULL_PTR);

			if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_na) {
				return ::pilo::mk_perr(PERR_INV_VAL_TYPE);
			} else if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_bytes) {
				if (this->test_flag(::pilo::tlv::FlagBytesAsCStr)) {
					return ::pilo::core::string::str_to_enum_id(ret, _dynamic_data, _size - 1, dict, count, ignore_case);
				}
				else {
					return ::pilo::core::string::str_to_enum_id(ret, _dynamic_data, _size, dict, count, ignore_case);
				}
			} else if (this->value_type() == ::pilo::core::rtti::wired_type::value_type_str) {
				std::string* p = (std::string*)this->_dynamic_data;
				return ::pilo::core::string::str_to_enum_id(ret, p, dict, count, ignore_case);
			}
			else if (this->value_type() > ::pilo::core::rtti::wired_type::value_type_na
				&& this->value_type() < ::pilo::core::rtti::wired_type::value_type_boolean) {
				if (this->as_i64(nullptr) >= (::pilo::i64_t)count) {
					return ::pilo::mk_perr(PERR_INC_DATA);
				}
				ret = (RET_ID_T)this->as_i64(nullptr);
			}

			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}

		template <typename T, ::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, T & value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			return  t->get_array<T>(value);
		}

		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::i8_t & value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_i8(&err);
			return PILO_OK;
		}
		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::u8_t & value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_u8(&err);
			return PILO_OK;
		}
		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::i16_t& value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_i16(&err);
			return PILO_OK;
		}
		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::u16_t& value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_u16(&err);
			return PILO_OK;
		}
		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::i32_t& value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_i32(&err);
			return PILO_OK;
		}
		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::u32_t& value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_u32(&err);
			return PILO_OK;
		}
		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::i64_t& value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_i64(&err);
			return PILO_OK;
		}
		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::u64_t& value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_u64(&err);
			return PILO_OK;
		}

		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, bool& value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_bool(&err);
			return PILO_OK;
		}

		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::f32_t& value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_f32(&err);
			return PILO_OK;
		}

		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, ::pilo::f64_t& value)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->as_f64(&err);
			return PILO_OK;
		}

		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, char*& value, ::pilo::i32_t * sz)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			value = t->_dynamic_data;
			::pilo::set_if_ptr_is_not_null(sz, t->size());
			return PILO_OK;
		}

		template <::pilo::i32_t TV_CNT = 32 > ::pilo::err_t get(const char* fqn, std::string& str)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* t = get_tlv<TV_CNT>(fqn, err);
			if (t == nullptr)
			{
				return err;
			}
			str = t->as_str(&err, nullptr);
			return PILO_OK;
		}

		template <::pilo::i32_t TV_CNT = 32 >
		::pilo::tlv* get_tlv(const char* fqn, ::pilo::err_t& err)
		{
			if (fqn == nullptr || *fqn == 0)
			{
				return this;
			}
			::pilo::cstr_ref<char> parts[TV_CNT];
			::pilo::i64_t cnt = ::pilo::core::string::split_fixed<char>(fqn, (::pilo::i64_t)-1, ".", 1, parts, TV_CNT, true, true, true, true);
			if (cnt < 1)
			{
				err = ::pilo::mk_perr(PERR_INV_LEN);
				return nullptr;
			}
			::pilo::i64_t ri_len = 0;
			::pilo::i64_t rs_len = 0;
			const char* pinch_cstr = nullptr;
			::pilo::tlv* p = this;
			::pilo::tlv* tmp = nullptr;
			for (::pilo::i64_t i = 0; i < cnt; i++)
			{
				pinch_cstr = ::pilo::core::string::extract_pinch(parts[i].ptr, parts[i].length, '[', ']', &ri_len, &rs_len);
				if (pinch_cstr == nullptr) //pure dict
				{
					std::string key(parts[i].ptr, parts[i].length);
					if (p->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_dict)
					{
						err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
						return nullptr;
					}
					err = p->get<std::string, ::pilo::tlv*>(key, tmp);
					if (err != PILO_OK || tmp == nullptr)
					{
						err = ::pilo::mk_perr( PERR_NON_EXIST);
						return nullptr;
					}
					p = tmp;
				}
				else
				{
					if (rs_len < 1) //[] only
					{
						if (ri_len < 1)
						{
							err = ::pilo::mk_perr(PERR_INVALID_PARAM);
							return nullptr;
						}
						::pilo::i32_t idx = 0;
						::pilo::core::string::string_to_number(idx, pinch_cstr, ri_len);
						if (p->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_array)
						{
							err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
							return nullptr;
						}
						tmp = p->get<::pilo::tlv*>(idx, &err);
						if (tmp == nullptr)
						{
							err = ::pilo::mk_perr( PERR_NON_EXIST);
							return nullptr;
						}
						p = tmp;
					}
					else //mixed key and []
					{
						if (ri_len < 1)
						{
							err = ::pilo::mk_perr(PERR_INVALID_PARAM);
							return nullptr;
						}

						std::string key(parts[i].ptr, rs_len);
						if (p->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_dict)
						{
							err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
							return nullptr;
						}
						err = p->get<std::string, ::pilo::tlv*>(key, tmp);
						if (err != PILO_OK || tmp == nullptr)
						{
							err = ::pilo::mk_perr( PERR_NON_EXIST);
							return nullptr;
						}
						p = tmp;
						::pilo::i32_t idx = 0;
						::pilo::core::string::string_to_number(idx, pinch_cstr, ri_len);
						if (p->wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_array)
						{
							err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
							return nullptr;
						}
						tmp = p->get<::pilo::tlv*>(idx, &err);
						if (tmp == nullptr)
						{
							err = ::pilo::mk_perr( PERR_NON_EXIST);
							return nullptr;
						}
						p = tmp;
					}
				}
			} //end of for
			return p;
		}

		template <::pilo::i32_t TV_CNT = 32 >
		::pilo::tlv* set(const char* fqn, ::pilo::err_t& err, bool is_force = false)
		{
			if (fqn == nullptr || *fqn == 0)
			{
				this->set_na();
				return this;
			}

			if (!is_force)
			{
				::pilo::tlv* tmp = get_tlv<TV_CNT>(fqn, err);
				if (tmp != nullptr)
				{
					err = ::pilo::mk_perr(PERR_EXIST);
					return nullptr;
				}
				err = PILO_OK;
			}

			::pilo::tlv* t = set_tlv<TV_CNT>(fqn, err);
			if (t != nullptr)
			{
				t->set_na();
			}
			return t;
		}

		template <typename T, ::pilo::i32_t TV_CNT = 32 >
		::pilo::tlv* set(const char* fqn, T&& value, ::pilo::err_t& err, bool is_force = false)
		{
			if (fqn == nullptr || *fqn == 0)
			{
				err = this->set(value);
				return this;
			}

			if (!is_force)
			{
				::pilo::tlv* tmp = get_tlv<TV_CNT>(fqn, err);
				if (tmp != nullptr)
				{
					err = ::pilo::mk_perr(PERR_EXIST);
					return nullptr;
				}
				err = PILO_OK;
			}

			::pilo::tlv* t = set_tlv<TV_CNT>(fqn, err);
			if (t != nullptr)
			{
				t->set(value);
			}
			return t;
		}

		template <::pilo::i32_t TV_CNT = 32 >
		::pilo::tlv* set(const char* fqn, const char* value, ::pilo::i32_t len, bool adopt, ::pilo::err_t& err, bool is_force = false)
		{
			if (fqn == nullptr || *fqn == 0)
			{
				err = set_cstr(value, len, adopt);
				return this;
			}

			if (!is_force)
			{
				::pilo::tlv* tmp = get_tlv<TV_CNT>(fqn, err);
				if (tmp != nullptr)
				{
					err = ::pilo::mk_perr(PERR_EXIST);
					return nullptr;
				}
				err = PILO_OK;
			}

			::pilo::tlv* t = set_tlv<TV_CNT>(fqn, err);
			if (t != nullptr)
			{
				t->set_cstr(value, len, adopt);
			}
			return t;
		}



		template <::pilo::i32_t TV_CNT = 32 >
		::pilo::tlv* set_tlv(const char* fqn, ::pilo::err_t& err)
		{
			::pilo::cstr_ref<char> parts[TV_CNT];
			::pilo::i64_t cnt = ::pilo::core::string::split_fixed<char>(fqn, (::pilo::i64_t) - 1, ".", 1, parts, TV_CNT, true, true, true, true);
			if (cnt < 1)
			{
				err = ::pilo::mk_perr(PERR_INV_LEN);
				return nullptr;
			}

			::pilo::i64_t ri_len = 0;
			::pilo::i64_t rs_len = 0;
			const char* pinch_cstr = nullptr;
			::pilo::tlv* p = this;
			for (::pilo::i64_t i = 0; i < cnt; i++)
			{
				if (parts[i].length < 1)
				{
					err = ::pilo::mk_perr(PERR_INVALID_PARAM);
					return nullptr;
				}				
			}
			
			::pilo::tlv* tmp = nullptr;
			for (::pilo::i64_t i = 0; i < cnt; i++)
			{
				pinch_cstr = ::pilo::core::string::extract_pinch(parts[i].ptr, parts[i].length, '[', ']', &ri_len, &rs_len);				
				if (pinch_cstr == nullptr)
				{
					std::string key(parts[i].ptr, parts[i].length);
					if (p->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_na)
					{
						p->set_dict_type(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_tlv);
						tmp = ::pilo::tlv::allocate();
						p->insert(key, tmp, false);
						p = tmp;
					}
					else if (p->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
					{
						if (p->key_type() != ::pilo::core::rtti::wired_type::key_type_str || p->value_type() != ::pilo::core::rtti::wired_type::value_type_tlv)
						{
							err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
							return nullptr;
						}

						tmp = nullptr;
						err = p->get<std::string, ::pilo::tlv*>(key, tmp);

						if (err == PILO_OK)
						{
							p = tmp;
						}
						else if (err == PERR_NON_EXIST)
						{
							tmp = ::pilo::tlv::allocate();
							p->insert<std::string, ::pilo::tlv*>(key, tmp, false);
							p = tmp;
							err = PILO_OK;
						}
						else {
							return nullptr;
						}
					}
					else
					{
						err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
						return nullptr;
					}
				}
				else //has[]
				{
					if (rs_len < 1) //only has[]
					{						
						if (p->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_na)
						{
							p->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
							tmp = ::pilo::tlv::allocate();
							p->push_back(tmp, -1, true);
							p = tmp;
						}
						else if (p->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
						{
							if (p->value_type() != ::pilo::core::rtti::wired_type::value_type_tlv)
							{
								err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
								return nullptr;
							}
							if (ri_len < 1)
							{
								tmp = ::pilo::tlv::allocate();
								p->push_back(tmp, -1, true);
								p = tmp;
							}
							else
							{
								::pilo::i32_t idx = 0;
								::pilo::core::string::string_to_number(idx, pinch_cstr, ri_len);
								if (idx < 0 || idx > p->size())
								{
									err = ::pilo::mk_perr( PERR_IDX_TOO_OOB);
									return nullptr;
								}
								else if (idx == p->size())
								{
									tmp = ::pilo::tlv::allocate();
									p->push_back(tmp, -1, true);
									p = tmp;
								}
								else
								{
									tmp = (::pilo::tlv*) p->get<::pilo::tlv*>(idx, &err);
									if (tmp == nullptr)
									{
										err = ::pilo::mk_perr( PERR_NON_EXIST);
										return nullptr;
									}
									p = tmp;
								}
							}
						}
						else
						{
							err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
							return nullptr;
						}
					}
					else //like a[0]
					{
						std::string key(parts[i].ptr, rs_len);
						if (p->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_na)
						{
							::pilo::i32_t idx = 0;
							if (ri_len > 0)
							{								
								::pilo::core::string::string_to_number(idx, pinch_cstr, ri_len);
							}
							if (idx > 0)
							{
								err = ::pilo::mk_perr( PERR_IDX_TOO_OOB);
								return nullptr;
							}							
							p->set_dict_type(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_tlv);
							tmp = ::pilo::tlv::allocate();
							tmp->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
							p->insert<std::string, ::pilo::tlv*>(key, tmp,false);
							p = tmp;
							tmp = ::pilo::tlv::allocate();
							p->push_back(tmp, -1, true);
							p = tmp;
						}
						else if (p->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
						{
							if (p->key_type() != ::pilo::core::rtti::wired_type::key_type_str || p->value_type() != ::pilo::core::rtti::wired_type::value_type_tlv)
							{
								err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
								return nullptr;
							}
							err =  p->get<std::string, ::pilo::tlv*>(key, tmp);
							if (tmp != nullptr)
							{
								p = tmp;
								if (p->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_na)
								{
									::pilo::i32_t idx = 0;
									if (ri_len > 0)
									{
										::pilo::core::string::string_to_number(idx, pinch_cstr, ri_len);
									}
									if (idx > 0)
									{
										err = ::pilo::mk_perr( PERR_IDX_TOO_OOB);
										return nullptr;
									}
									p->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
									tmp = ::pilo::tlv::allocate();
									p->push_back(tmp, -1, true);
									p = tmp;
								}
								else if (p->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
								{
									if (p->value_type() != ::pilo::core::rtti::wired_type::value_type_tlv)
									{
										err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
										return nullptr;
									}
									if (ri_len < 1)
									{
										tmp = ::pilo::tlv::allocate();
										p->push_back(tmp, -1, true);
										p = tmp;
									}
									else
									{
										::pilo::i32_t idx = 0;
										::pilo::core::string::string_to_number(idx, pinch_cstr, ri_len);
										if (idx < 0 || idx > p->size())
										{
											err = ::pilo::mk_perr( PERR_IDX_TOO_OOB);
											return nullptr;
										}
										else if (idx == p->size())
										{
											tmp = ::pilo::tlv::allocate();
											p->push_back(tmp, -1, true);
											p = tmp;
										}
										else
										{
											tmp = (::pilo::tlv*) p->get<::pilo::tlv*>(idx, &err);
											if (tmp == nullptr)
											{
												err = ::pilo::mk_perr( PERR_NON_EXIST);
												return nullptr;
											}
											p = tmp;
										}
									}
								}
								else
								{
									err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
									return nullptr;
								}
							}
							else
							{
								::pilo::i32_t idx = 0;
								if (ri_len > 0)
								{
									::pilo::core::string::string_to_number(idx, pinch_cstr, ri_len);
								}
								if (idx > 0)
								{
									err = ::pilo::mk_perr( PERR_IDX_TOO_OOB);
									return nullptr;
								}
								p->set_dict_type(::pilo::core::rtti::wired_type::key_type_str, ::pilo::core::rtti::wired_type::value_type_tlv);
								tmp = ::pilo::tlv::allocate();
								tmp->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
								p->insert<std::string, ::pilo::tlv*>(key, tmp, false);
								p = tmp;
								tmp = ::pilo::tlv::allocate();
								p->push_back(tmp, -1, true);
								p = tmp;
							}
						}
						else
						{
							err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
							return nullptr;
						}
					}				
				}
			}

			if (p == nullptr)
			{
				err = ::pilo::mk_perr(PERR_NULL_PTR);
				return nullptr;
			}
			return p;
		}
		


		//common
		void wrapper_clear(bool compact);
		inline bool valid() const
		{
			return _type.valid();
		}
		inline ::pilo::i32_t size() const
		{
			return _size;
		}
		inline ::pilo::i32_t& size_ref()
		{
			return _size;
		}
		void reset()
		{
			_clear_dynamic_data();
			_size = 0;
			_type.reset();
		}
		void set_na()
		{
			_clear_dynamic_data();
			_size = 0;
			_type.reset();
			_type.set_single_type(::pilo::core::rtti::wired_type::value_type_na);
		}
		inline ::pilo::u16_t value_type() const
		{
			return _type.value_type();
		}
		inline ::pilo::u8_t key_type() const
		{
			return _type.key_type();
		}
		inline ::pilo::u8_t wrapper_type() const
		{
			return _type.wrapper_type();
		}
		inline ::pilo::u8_t category_id() const
		{
			return _type.category_id();
		}
		::pilo::i64_t internal_array_size(::pilo::err_t* err) const;

		/*::pilo::tlv* get_sub_tlv(const char* path, bool create_on_nonexist);*/


		//static allocation
		template<typename T> static ::pilo::tlv * allocate_single(T&& v)
		{
			auto t = ::pilo::tlv::allocate();
			bool is_ptr = false;
			::pilo::u16_t vt = ::pilo::core::rtti::to_wired_value_type<T>(&is_ptr);
			t->set_single_type(vt);
			t->set(v);
			return t;
		}

		static ::pilo::tlv * allocate_single(const char* v,::pilo::i32_t len, bool is_str, bool adopt)
		{
			auto t = ::pilo::tlv::allocate();
			t->set_single_type(::pilo::core::rtti::wired_type::value_type_bytes);
			t->set(v, len, is_str, adopt);
			return t;
		}
		template<typename T> static std::shared_ptr<::pilo::tlv> allocate_single_shared(T&& v)
		{
			auto t = ::pilo::tlv::allocate_shared();
			t->set(v);
			return t;
		}
		static std::shared_ptr<::pilo::tlv> allocate_single_shared(const char* v, ::pilo::i32_t len, bool is_str, bool adopt)
		{
			auto t = ::pilo::tlv::allocate_shared();
			t->set(v, len, is_str, adopt);
			return t;
		}
		template<typename T> static std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> allocate_single_unique(T&& v)
		{
			auto t = ::pilo::tlv::allocate_unique();
			t->set(v);
			return t;
		}
		static std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> allocate_single_unique(const char* v, ::pilo::i32_t len, bool is_str, bool adopt)
		{
			auto t = ::pilo::tlv::allocate_unique();
			t->set(v, len, is_str, adopt);
			return t;
		}
		static std::shared_ptr<::pilo::tlv> allocate_shared()
		{
			std::shared_ptr<::pilo::tlv> t(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});
			return t;
		}
		static std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> allocate_unique()
		{
			std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> t(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});
			return t;
		}

		static ::pilo::err_t update_pool_object_stat(::pilo::core::stat::pool_object_stat_manager::stat_item * si);

		//single manipulations
		inline ::pilo::err_t set_single_type(::pilo::u16_t valtype)
		{
			if (this->valid())
			{
				return ::pilo::mk_perr(PERR_NO_OVERWRITING);
			}
			if (valtype == ::pilo::core::rtti::wired_type::value_type_tlv)
			{
				return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			}
			_type.set_wrapper_type(::pilo::core::rtti::wired_type::wrapper_single);
			_type.set_value_type(valtype);
			return PILO_OK;
		}
		inline ::pilo::err_t set_cstr(const char* str, ::pilo::i32_t len, bool adopt)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_bytes) == PILO_OK)
			{
				_clear_dynamic_data();
				return _set_cstr(str, len, adopt);
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set_bytes(const char* str, ::pilo::i32_t len, bool adopt)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_bytes) == PILO_OK)
			{
				_clear_dynamic_data();
				return _set_bytes(str, len, adopt);
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(const char* str, ::pilo::i32_t len, bool is_str, bool adopt)
		{
			if (is_str)
				return set_cstr(str, len, adopt);
			else
				return set_bytes(str, len, adopt);
		}
		inline ::pilo::err_t set_string(const char* p, ::pilo::i64_t len)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_str) == PILO_OK)
			{
				_clear_dynamic_data();
				if (len < 0)
				{
					this->_dynamic_data = nullptr;
				}
				else if (len == 0)
				{
					this->_dynamic_data = (char*) new std::string();
				}
				else
				{
					this->_dynamic_data = (char*) new std::string(p, len);
				}
				return PILO_OK;
			}			
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);			
		}
		inline ::pilo::err_t set(const std::string& str)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_str) == PILO_OK)
			{
				_clear_dynamic_data();
				this->_dynamic_data = (char*) new std::string(str);
				return PILO_OK;
			}			
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);			
		}
		inline ::pilo::err_t set(std::string& str)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_str) == PILO_OK)
			{
				_clear_dynamic_data();
				this->_dynamic_data = (char*) new std::string(str);
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::i8_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_i8) == PILO_OK)
			{
				_clear_dynamic_data();
				_i8 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::u8_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_u8) == PILO_OK)
			{
				_clear_dynamic_data();
				_u8 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::i16_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_i16) == PILO_OK)
			{
				_clear_dynamic_data();
				_i16 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::u16_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_u16) == PILO_OK)
			{
				_clear_dynamic_data();
				_u16 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::i32_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_i32) == PILO_OK)
			{
				_clear_dynamic_data();
				_i32 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::u32_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_u32) == PILO_OK)
			{
				_clear_dynamic_data();
				_u32 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::i64_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_i64) == PILO_OK)
			{
				_clear_dynamic_data();
				_i64 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::u64_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_u64) == PILO_OK)
			{
				_clear_dynamic_data();
				_u64 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::f32_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_f32) == PILO_OK)
			{
				_clear_dynamic_data();
				_f32 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(::pilo::f64_t v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_f64) == PILO_OK)
			{
				_clear_dynamic_data();
				_f64 = v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(bool v)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_boolean) == PILO_OK)
			{
				_clear_dynamic_data();
				_i8 = (::pilo::i8_t)v;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		inline ::pilo::err_t set(std::string&& str)
		{
			if (_type.set_single_type(::pilo::core::rtti::wired_type::value_type_str) == PILO_OK)
			{
				_clear_dynamic_data();
				_dynamic_data = (char*) new std::string(str);
				_size = (::pilo::i32_t)((std::string*)_dynamic_data)->size();
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}

		inline ::pilo::i8_t int8() const { return _i8; }
		inline ::pilo::u8_t uint8() const { return _u8; }
		inline ::pilo::i16_t int16() const { return _i16; }
		inline ::pilo::u16_t uint16() const { return _u16; }
		inline ::pilo::i32_t int32() const { return _i32; }
		inline ::pilo::u32_t uint32() const { return _u32; }
		inline ::pilo::i64_t int64() const { return _i64; }
		inline ::pilo::u64_t uint64() const { return _u64; }
		inline ::pilo::f32_t float32() const { return _f32; }
		inline ::pilo::f64_t float64() const { return _f64; }
		const char* daynamic_data() const { return _dynamic_data; }
		char* &daynamic_data_ref()  { return _dynamic_data; }
		const std::string* string_ptr() const
		{
			if (_dynamic_data != nullptr)
			{
				return (const std::string*)_dynamic_data;
			}
			return nullptr;
		}
		bool as_bool(::pilo::err_t* err) const;
		::pilo::i8_t as_i8(::pilo::err_t* err) const;
		::pilo::u8_t as_u8(::pilo::err_t* err) const;
		::pilo::i16_t as_i16(::pilo::err_t* err) const;
		::pilo::u16_t as_u16(::pilo::err_t* err) const;
		::pilo::i32_t as_i32(::pilo::err_t* err) const;
		::pilo::u32_t as_u32(::pilo::err_t* err) const;
		::pilo::i64_t as_i64(::pilo::err_t* err) const;
		::pilo::u64_t as_u64(::pilo::err_t* err) const;
		::pilo::f32_t as_f32(::pilo::err_t* err) const;
		::pilo::f64_t as_f64(::pilo::err_t* err) const;
		char* as_cstr_dup(::pilo::err_t* err, ::pilo::i32_t* rlen) const;
		const char* as_cstr(char* buffer, ::pilo::i32_t sz, ::pilo::err_t* err, ::pilo::i32_t* rlen, const char* fmt = nullptr) const;
		const std::string* as_str_ptr(::pilo::err_t* err) const;
		std::string as_str(::pilo::err_t* err, const char* fmt = nullptr) const;
		const char* as_bytes(char* buffer, ::pilo::i32_t sz, ::pilo::err_t* err, ::pilo::i32_t* rlen) const;

		template<typename T>
		::pilo::err_t get_array(T& arr)
		{
			if (_dynamic_data == nullptr || wrapper_type() != ::pilo::core::rtti::wired_type::wrapper_array)
				return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			arr = *(T *) _dynamic_data;
			return PILO_OK;
		}

		template <typename VT> inline ::pilo::err_t set(std::deque<VT>& vec)
		{
			::pilo::err_t err = _set_value_type<VT>();
			if (err != PILO_OK)
			{
				return err;
			}
			_set_wrapper<VT>(vec);
			return PILO_OK;
		}
		template <typename KT, typename VT> inline ::pilo::err_t set(std::map<KT, VT>& map)
		{			
			::pilo::err_t err = _set_value_type<VT>();
			if (err != PILO_OK)
			{
				return err;
			}
			err = _set_key_type<KT>();
			if (err != PILO_OK)
			{
				return err;
			}
			_set_wrapper<KT,VT>(map);
			return PILO_OK;
		}

		//template <typename TA_WRAPPER> inline ::pilo::err_t set(TA_WRAPPER& vec)
		//{
		//	::pilo::err_t err = _set_value_type<typename TA_WRAPPER::value_type>();
		//	if (err != PILO_OK)
		//	{
		//		return err;
		//	}
		//	_set_wrapper(vec);
		//	return PILO_OK;
		//}
		template<typename T> inline ::pilo::err_t init_by_array(std::initializer_list<T> list)
		{
			::pilo::u16_t wt = ::pilo::core::rtti::to_wired_value_type<T>(nullptr);
			::pilo::err_t err = set_array_type(wt);
			for (T v : list)
			{
				this->push_back<T>(v);
			}
			return err;
		}
		inline ::pilo::err_t set_array_type(::pilo::u16_t valtype)
		{
			if (this->valid())
			{
				return ::pilo::mk_perr(PERR_NO_OVERWRITING);
			}
			if (valtype == ::pilo::core::rtti::wired_type::value_type_na)
			{
				return ::pilo::mk_perr(PERR_INVALID_PARAM);
			}
			_type.set_wrapper_type(::pilo::core::rtti::wired_type::wrapper_array);
			_type.set_value_type(valtype);
			if (_dynamic_data == nullptr)
			{
				_size = -1;
			}
			else
			{
				_size = 0;
			}
			return PILO_OK;
		}	

		template <typename TA_ARR_VT> inline const TA_ARR_VT& get(::pilo::i32_t index, ::pilo::err_t* err) const
		{
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
			{
				if (_dynamic_data == nullptr)
				{
					if (err != nullptr) *err = ::pilo::mk_perr( PERR_NON_EXIST);
					return ::pilo::_invalid_object_<TA_ARR_VT>::get();
				}
				std::deque<TA_ARR_VT>* ptr = (std::deque<TA_ARR_VT>*) this->_dynamic_data;
				if (index < 0 || index >= (::pilo::i32_t)ptr->size())
				{
					if (err != nullptr) *err = ::pilo::mk_perr( PERR_NON_EXIST);
					return ::pilo::_invalid_object_<TA_ARR_VT>::get();
				}
				return ptr->at(index);
			}

			if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			return ::pilo::_invalid_object_<TA_ARR_VT>::get();
		}
		template<typename T> inline ::pilo::tlv* push_back_array_element(std::initializer_list<T> list, ::pilo::err_t* err)
		{
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
			{
				::pilo::tlv* _t = ::pilo::tlv::allocate();
				_t->init_by_array<T>(list);

				this->push_back((::pilo::tlv*)_t, -1, true);
				return _t;
			}
			if (err != nullptr) *err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			return nullptr;
		}
		template<typename T> inline ::pilo::err_t push_back(T value, ::pilo::i32_t size = -1, bool adopt = false)
		{
			::pilo::err_t err = PILO_OK;
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
			{
				err = _make_dynamic_data_array();
				if (err != PILO_OK)
					return err;

				err = ::pilo::core::rtti::deque_push_back(this->value_type(), this->_dynamic_data,  value, size, adopt);
				if (err != PILO_OK)
					return err;
				_size++;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}		
		template<typename T> ::pilo::tlv* push_back_tlv(T init_param, ::pilo::err_t* err)
		{
			::pilo::err_t e = PILO_OK;
			::pilo::u16_t vt = ::pilo::core::rtti::to_wired_value_type<T>(nullptr);
			::pilo::tlv* p_tlv = ::pilo::tlv::allocate_single(init_param);
			p_tlv->set<T>(init_param);
			e = push_back<::pilo::tlv*>(p_tlv, -1, true);
			if (err != nullptr)
				*err = e;
			if (e != PILO_OK)
			{
				::pilo::tlv::deallocate(p_tlv);
				return nullptr;
			}
			
			return p_tlv;
		}
		::pilo::tlv* push_back_tlv(const char* init_param, ::pilo::i32_t len, bool is_str, bool adopt, ::pilo::err_t* err)
		{
			::pilo::err_t e = PILO_OK;
			::pilo::tlv* p_tlv = ::pilo::tlv::allocate_single(init_param, len, false, adopt);
			p_tlv->set(init_param, len, is_str, adopt);
			e = push_back<::pilo::tlv*>(p_tlv, -1, true);
			if (err != nullptr)
				*err = e;
			if (e != PILO_OK)
			{
				::pilo::tlv::deallocate(p_tlv);
				return nullptr;
			}
			return p_tlv;
		}
		template<typename T> inline ::pilo::err_t push_front(T value, ::pilo::i32_t size = -1, bool adopt = false)
		{
			::pilo::err_t err = PILO_OK;
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
			{
				err = _make_dynamic_data_array();
				if (err != PILO_OK)
					return err;

				err = ::pilo::core::rtti::deque_push_front(this->value_type(), this->_dynamic_data, value, size, adopt);
				if (err != PILO_OK)
					return err;
				_size++;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		template<typename T> inline ::pilo::err_t insert(::pilo::i32_t index, T value, ::pilo::i32_t size = -1)
		{
			::pilo::err_t err = PILO_OK;
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
			{
				err = _make_dynamic_data_array();
				if (err != PILO_OK)
					return err;

				err = ::pilo::core::rtti::deque_insert(this->value_type(), index, this->_dynamic_data, value, size, false);
				_size++;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		template<typename T> inline ::pilo::err_t set(::pilo::i32_t index, T value)
		{
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
			{
				if (_dynamic_data == nullptr)
				{
					return ::pilo::mk_perr(PERR_NULL_PTR);
				}
				std::deque<T>* ptr = (std::deque<T>*) this->_dynamic_data;
				if (index < 0 || (size_t) index >= ptr->size())
				{
					return ::pilo::mk_perr(PERR_IDX_TOO_OOB);
				}
				(*ptr)[index] = value;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		template<typename T> inline ::pilo::err_t erase(::pilo::i32_t index)
		{
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
			{
				if (_dynamic_data == nullptr)
				{
					return ::pilo::mk_perr(PERR_NULL_PTR);
				}
				std::deque<T>* ptr = (std::deque<T>*) this->_dynamic_data;
				if ((size_t)index >= ptr->size())
				{
					return ::pilo::mk_perr( PERR_NON_EXIST);
				}
				ptr->erase(ptr->begin() + index);
				_size--;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}

		//dict manipulations		
		inline ::pilo::err_t set_dict_type(::pilo::u8_t key_type, ::pilo::u16_t valtype)
		{
			if (this->valid())
			{
				return ::pilo::mk_perr(PERR_NO_OVERWRITING);
			}
			if (valtype == ::pilo::core::rtti::wired_type::value_type_na)
			{
				return ::pilo::mk_perr(PERR_INVALID_PARAM);
			}
			_type.set_wrapper_type(::pilo::core::rtti::wired_type::wrapper_dict);
			_type.set_key_type(key_type);
			_type.set_value_type(valtype);
			if (_dynamic_data == nullptr)
			{
				_size = -1;
			}
			else
			{
				_size = 0;
			}
			return PILO_OK;
		}
		template<typename TA_KEY, typename TA_VALUE> ::pilo::err_t init_by_dict(std::initializer_list<std::pair<TA_KEY, TA_VALUE>> list, bool adopt = false)
		{
			PMC_UNUSED(adopt);

			if (this->valid())
			{
				return ::pilo::mk_perr(PERR_NOOP);
			}

			::pilo::u8_t kt = ::pilo::core::rtti::to_wired_key_type<TA_KEY>();
			::pilo::u16_t vt = ::pilo::core::rtti::to_wired_value_type<TA_VALUE>(nullptr);
			::pilo::err_t err = PILO_OK;
			if (kt == ::pilo::core::rtti::wired_type::key_type_na || vt == ::pilo::core::rtti::wired_type::value_type_na)
			{
				return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			}
			err = set_dict_type(kt, vt);
			for (std::pair<TA_KEY, TA_VALUE> kvpair : list)
			{
				insert<TA_KEY, TA_VALUE>(kvpair, false);
			}
			return err;
		}
		template<typename TA_KEY> ::pilo::err_t init_by_dict(std::initializer_list<std::pair<TA_KEY, const char*>> list, bool adopt = false)
		{
			PMC_UNUSED(adopt);
			if (this->valid())
			{
				return ::pilo::mk_perr(PERR_NOOP);
			}

			::pilo::u8_t kt = ::pilo::core::rtti::to_wired_key_type<TA_KEY>();
			::pilo::u16_t vt = ::pilo::core::rtti::to_wired_value_type<char*>(nullptr);
			::pilo::err_t err = PILO_OK;
			if (kt == ::pilo::core::rtti::wired_type::key_type_na || vt == ::pilo::core::rtti::wired_type::value_type_na)
			{
				return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			}
			err = set_dict_type(kt, vt);
			for (std::pair<TA_KEY, const char*> kvpair : list)
			{
				if (kvpair.second == nullptr)
					insert<TA_KEY, const char*>(kvpair.first, nullptr, false);
				else
					insert<TA_KEY, const char*>(kvpair.first, ::pilo::core::string::duplicate(kvpair.second, -1), false);
			}

			return err;
		}
		template<typename TA_KEY> ::pilo::err_t init_by_dict(std::initializer_list<std::pair<TA_KEY, char*>> list, bool adopt = false)
		{
			if (this->valid())
			{
				return ::pilo::mk_perr(PERR_NOOP);
			}

			::pilo::u8_t kt = ::pilo::core::rtti::to_wired_key_type<TA_KEY>();
			::pilo::u16_t vt = ::pilo::core::rtti::to_wired_value_type<char*>(nullptr);
			::pilo::err_t err = PILO_OK;
			if (kt == ::pilo::core::rtti::wired_type::key_type_na || vt == ::pilo::core::rtti::wired_type::value_type_na)
			{
				return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			}
			err = set_dict_type(kt, vt);
			if (adopt)
			{
				for (std::pair<TA_KEY,  char*> kvpair : list)
				{
					insert<TA_KEY,  char*>(kvpair.first, kvpair.second, false);
				}
			}
			else
			{
				for (std::pair<TA_KEY,  char*> kvpair : list)
				{
					if (kvpair.second == nullptr)
						insert<TA_KEY,  char*>(kvpair.first, nullptr, false);
					else
						insert<TA_KEY,  char*>(kvpair.first, ::pilo::core::string::duplicate(kvpair.second,-1), false);
				}
			}
			
			return err;
		}
		template<typename TA_KEY, typename TA_VALUE> ::pilo::err_t insert(std::pair<TA_KEY, TA_VALUE> kvpair, bool is_force)
		{
			if (this->_dynamic_data == nullptr)
			{
				_dynamic_data = (char*) new std::map<TA_KEY, TA_VALUE>();
				if (_dynamic_data == nullptr)
				{
					return ::pilo::mk_perr( PERR_INSUF_HEAP);
				}
				_size = 0;
			}
			else
			{
				if (((std::map<TA_KEY, TA_VALUE> *) _dynamic_data)->size() >= (size_t)PILO_INT32_MAX)
				{
					return ::pilo::mk_perr(PERR_VAL_TOO_LARGE);
				}

				auto it = ((std::map<TA_KEY, TA_VALUE> *) _dynamic_data)->find(kvpair.first);
				if (it != ((std::map<TA_KEY, TA_VALUE> *) _dynamic_data)->end())
				{
					if (! is_force)
					{
						return ::pilo::mk_perr(PERR_EXIST);
					}
					//TODO fix potential leak problem
					((std::map<TA_KEY, TA_VALUE> *) _dynamic_data)->erase(it);
				}
			}
			
			((std::map<TA_KEY, TA_VALUE> *) _dynamic_data)->insert(kvpair);
			this->_size++;
			return PILO_OK;
		}
		template<typename TA_KEY, typename TA_VALUE> ::pilo::err_t insert(const TA_KEY& key, const TA_VALUE& value, bool is_force)
		{
			return insert(std::pair<TA_KEY, TA_VALUE>(key, value), is_force);
		}


		template<typename TA_KEY, typename TA_VALUE> inline ::pilo::err_t get(const TA_KEY& key, TA_VALUE& value) const
		{
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
			{
				typename std::map<TA_KEY, TA_VALUE>::const_iterator cit = ((std::map<TA_KEY, TA_VALUE>*) this->_dynamic_data)->find(key);
				if (cit != ((std::map<TA_KEY, TA_VALUE>*) this->_dynamic_data)->cend())
				{
					value = cit->second;
					return PILO_OK;
				}
				return ::pilo::mk_perr( PERR_NON_EXIST);
			}
			return  ::pilo::mk_perr( PERR_MIS_DATA_TYPE);
		}

		//template<typename TA_KEY, typename TA_VALUE> inline const TA_VALUE& get(const TA_KEY & key, ::pilo::err_t* err) const
		//{
		//	if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
		//	{
		//		typename std::map<TA_KEY, TA_VALUE>::const_iterator cit = ((std::map<TA_KEY, TA_VALUE>*) this->_dynamic_data)->find(key);
		//		if (cit != ((std::map<TA_KEY, TA_VALUE>*) this->_dynamic_data)->cend())
		//		{
		//			return cit->second;
		//		}
		//		if (err != nullptr)
		//			*err = ::pilo::mk_perr( PERR_NON_EXIST);
		//		return ::pilo::_type_default_const_value_<TA_VALUE>::get();
		//	}
		//	if (err != nullptr)
		//		*err = ::pilo::mk_perr( PERR_MIS_DATA_TYPE);
		//	return ::pilo::_type_default_const_value_<TA_VALUE>::get();
		//}


		template<typename TA_KEY, typename TA_VALUE> inline ::pilo::err_t erase(TA_KEY && key)
		{
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_dict)
			{
				if (_dynamic_data == nullptr)
				{
					return ::pilo::mk_perr(PERR_NULL_PTR);
				}
				std::map<TA_KEY, TA_VALUE>* ptr = (std::map<TA_KEY, TA_VALUE>*) this->_dynamic_data;
				if (ptr->empty())
				{
					return ::pilo::mk_perr( PERR_NON_EXIST);
				}
				typename std::map<TA_KEY, TA_VALUE>::iterator it = ((std::map<TA_KEY, TA_VALUE>*) this->_dynamic_data)->find(key);
				if (it == ((std::map<TA_KEY, TA_VALUE>*) this->_dynamic_data)->end())
				{
					return ::pilo::mk_perr( PERR_NON_EXIST);
				}

				::pilo::core::rtti::wired_type_deleter<TA_VALUE>(it->second);

				ptr->erase(it);
				_size--;
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}

		//clone serialize and compare
		::pilo::err_t serialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer) const;
		::pilo::err_t deserialize(::pilo::core::memory::serializable_header_interface* header, ::pilo::core::memory::byte_buffer_interface* byte_buffer);
		tlv* clone() const;
		std::shared_ptr<::pilo::tlv> clone_shared() const
		{
			std::shared_ptr<::pilo::tlv> t(this->clone(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});
			return t;
		}
		std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> clone_unique() const
		{
			std::unique_ptr<::pilo::tlv, void(*)(::pilo::tlv*)> t(this->clone(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});
			return t;
		}
		bool equals_to(const ::pilo::tlv* other) const;

		//debuging
		::pilo::err_t _to_string(std::stringstream& ss) const;
		std::string to_string()
		{
			std::stringstream ss;
			_to_string(ss);
			return ss.str();
		}

	private:
		tlv(const ::pilo::tlv& other) = delete;
		::pilo::tlv operator=(const ::pilo::tlv& other) = delete;
		tlv(::pilo::tlv&& other) = delete;
	private:
		::pilo::err_t _internal_map_travelsal(::pilo::u16_t func_id, bool ignore_error, ::pilo::i64_t* total_cnt, ::pilo::i64_t* travel_cnt, void* ctx) const;

		void _clear_dynamic_data();
		::pilo::err_t _set_cstr(const char* str, ::pilo::i32_t len, bool adopt);
		::pilo::err_t _set_bytes(const char* str, ::pilo::i32_t len, bool adopt);
		template <typename TA_VAL_TYPE>
		inline ::pilo::err_t _set_value_type()
		{
			bool is_ptr = false;
			::pilo::u16_t wtype = ::pilo::core::rtti::to_wired_value_type<TA_VAL_TYPE>(&is_ptr);
			if (wtype != ::pilo::core::rtti::wired_type::value_type_na)
			{
				_type.set_value_type(wtype);
				return PILO_OK;
			}
			return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
		}
		//template <typename TA_WRAPPER>
		//inline void _set_array(TA_WRAPPER& arr)
		//{
		//	_clear_dynamic_data();
		//	_dynamic_data = (char*) new std::deque<typename TA_WRAPPER::value_type>();
		//	typename TA_WRAPPER::const_iterator it = arr.cbegin();
		//	for (; it != arr.cend(); ++it)
		//	{
		//		((std::deque<typename TA_WRAPPER::value_type>*) _dynamic_data)->push_back(*it);
		//	}
		//	_size = (::pilo::i32_t)arr.size();
		//	_type.set_wrapper_type(::pilo::core::rtti::wired_type::wrapper_array);
		//}

		template <typename VT>
		inline void _set_wrapper(std::deque<VT> & cont)
		{
			_clear_dynamic_data();
			_dynamic_data = (char*) new std::deque<VT>;
			typename std::deque<VT>::const_iterator it = cont.cbegin();
			for (; it != cont.cend(); ++it)
			{
				((std::deque<VT>*)_dynamic_data)->push_back(*it);
			}
			_size = (::pilo::i32_t)cont.size();
			_type.set_wrapper_type(::pilo::core::rtti::wired_type::wrapper_array);
		}
		template <typename KT, typename VT>
		inline void _set_wrapper(std::map<KT, VT>& cont)
		{
			_clear_dynamic_data();
			_dynamic_data = (char*) new std::map<KT, VT>;
			typename std::map<KT, VT>::const_iterator it = cont.cbegin();
			for (; it != cont.cend(); ++it)
			{
				((std::map<KT, VT>*)_dynamic_data)->insert(*it);
			}
			_size = (::pilo::i32_t)cont.size();
			_type.set_wrapper_type(::pilo::core::rtti::wired_type::wrapper_dict);
		}


		template <typename T>
		bool _array_equals(const char* other_data) const
		{
			std::deque<T>* parr = (std::deque<T> *) this->_dynamic_data;
			std::deque<T>* other_arr = (std::deque<T>*) other_data;
			if (parr == nullptr && other_arr == nullptr)
				return true;
			else if (parr == nullptr || other_arr == nullptr)
				return false;
			else
			{
				if (parr->size() != other_arr->size())
				{
					return false;
				}

				for (size_t i = 0; i < parr->size(); i++)
				{
					if (parr->at(i) != other_arr->at(i))
						return false;
				}
			}
			return true;
		}		

		template <typename TA_KEY_TYPE>
		inline ::pilo::err_t _set_key_type()
		{
			if (typeid(TA_KEY_TYPE) == typeid(::pilo::i64_t))
			{
				_type.set_key_type(::pilo::core::rtti::wired_type::key_type_i64);
			}
			else if (typeid(TA_KEY_TYPE) == typeid(::pilo::u64_t))
			{
				_type.set_key_type(::pilo::core::rtti::wired_type::key_type_u64);
			}
			else if (typeid(TA_KEY_TYPE) == typeid(::pilo::i32_t))
			{
				_type.set_key_type(::pilo::core::rtti::wired_type::key_type_i32);
			}
			else if (typeid(TA_KEY_TYPE) == typeid(::pilo::u32_t))
			{
				_type.set_key_type(::pilo::core::rtti::wired_type::key_type_u32);
			}
			else if (typeid(TA_KEY_TYPE) == typeid(::pilo::i16_t))
			{
				_type.set_key_type(::pilo::core::rtti::wired_type::key_type_i16);
			}
			else if (typeid(TA_KEY_TYPE) == typeid(::pilo::u16_t))
			{
				_type.set_key_type(::pilo::core::rtti::wired_type::key_type_u16);
			}
            else if (typeid(TA_KEY_TYPE) == typeid(::pilo::i8_t))
            {
                _type.set_key_type(::pilo::core::rtti::wired_type::key_type_i8);
            }
            else if (typeid(TA_KEY_TYPE) == typeid(::pilo::u8_t))
            {
                _type.set_key_type(::pilo::core::rtti::wired_type::key_type_u8);
            }
			else if (typeid(TA_KEY_TYPE) == typeid(std::string))
			{
				_type.set_key_type(::pilo::core::rtti::wired_type::key_type_str);
			}
			
			else
			{
				return ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
			}
			return PILO_OK;
		}

		template<typename T>
		inline void _wrapper_clear(bool compact)
		{
			if (this->wrapper_type() == ::pilo::core::rtti::wired_type::wrapper_array)
			{
				if (_dynamic_data == nullptr)
				{
					return;
				}
				std::deque<T>* ptr = (std::deque<T>*) this->_dynamic_data;
				ptr->clear();
				if (compact)
					ptr->shrink_to_fit();
				_size = 0;
			}
		}

		::pilo::err_t _make_dynamic_data_array();		
		::pilo::err_t _clone_data_array(const ::pilo::tlv * src_tlv);
		::pilo::err_t _clone_data_dict(const ::pilo::tlv* src_tlv);
		
    private:
		::pilo::i32_t _size;
		::pilo::core::rtti::wired_type _type;
		union
		{			
			::pilo::i8_t	_i8;
			::pilo::i16_t	_i16;
			::pilo::i32_t	_i32;
			::pilo::i64_t	_i64;
			::pilo::u8_t	_u8;
			::pilo::u16_t	_u16;
			::pilo::u32_t	_u32;
			::pilo::u64_t	_u64;
			::pilo::f32_t	_f32;
			::pilo::f64_t	_f64;
			char* _dynamic_data;
			char _internal_data[sizeof(::pilo::u64_t)];
		};
    };

	template <>
	inline bool ::pilo::tlv::_array_equals<const ::pilo::tlv*>(const char* other_data) const
	{
		std::deque<const ::pilo::tlv*>* parr = (std::deque<const ::pilo::tlv*>*) this->_dynamic_data;
		std::deque<const ::pilo::tlv*>* other_arr = (std::deque<const ::pilo::tlv*>*) other_data;
		if (parr == nullptr && other_arr == nullptr)
			return true;
		else if (parr == nullptr || other_arr == nullptr)
			return false;
		else
		{
			if (parr->size() != other_arr->size())
			{
				return false;
			}

			for (size_t i = 0; i < parr->size(); i++)
			{
				if (!parr->at(i)->equals_to(other_arr->at(i)))
				{
					return false;
				}
			}
		}
		return true;
	}

	template <>
	inline bool ::pilo::tlv::_array_equals<char*>(const char* other_data) const
	{
		std::deque<char*>* parr = (std::deque<char*> *) this->_dynamic_data;
		std::deque<char*>* other_arr = (std::deque<char*>*) other_data;
		if (parr == nullptr && other_arr == nullptr)
			return true;
		else if (parr == nullptr || other_arr == nullptr)
			return false;
		else
		{
			if (parr->size() != other_arr->size())
			{
				return false;
			}

			for (size_t i = 0; i < parr->size(); i++)
			{
				if (parr->at(i) == nullptr && other_arr->at(i) == nullptr)
				{

				}
				else if (parr->at(i) == nullptr || other_arr->at(i) == nullptr)
				{
					return false;
				}
				else
				{
					if (0 != ::pilo::core::string::strict_compare(parr->at(i), 0, other_arr->at(i), 0, -1))
					{
						return false;
					}
				}
			}
		}
		return true;
	}
	
}