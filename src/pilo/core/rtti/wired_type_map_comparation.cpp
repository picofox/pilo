#include "../../tlv.hpp"
#include "wired_type_map_comparation.hpp"
#include "./wired_type.hpp"

namespace pilo
{
	namespace core
	{
		namespace rtti
		{
			typedef bool(* handle_map_compare_func_type)(const char* a, const char* b, ::pilo::err_t * err);
			static handle_map_compare_func_type stc_handle_compare_func[::pilo::core::rtti::wired_type::key_type_count][::pilo::core::rtti::wired_type::value_type_intrincs_count] =
			{
				{
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },
				},
				{//i8
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },//0
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::i8_t>((const std::map<::pilo::i8_t, ::pilo::i8_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::i8_t>*) b_data, err); },//1
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::u8_t>((const std::map<::pilo::i8_t, ::pilo::u8_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::u8_t>*) b_data, err); },//2
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::i16_t>((const std::map<::pilo::i8_t, ::pilo::i16_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::i16_t>*) b_data, err); },//3
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::u16_t>((const std::map<::pilo::i8_t, ::pilo::u16_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::u16_t>*) b_data, err); },//4
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::i32_t>((const std::map<::pilo::i8_t, ::pilo::i32_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::i32_t>*) b_data, err); },//5
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::u32_t>((const std::map<::pilo::i8_t, ::pilo::u32_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::u32_t>*) b_data, err); },//6
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::i64_t>((const std::map<::pilo::i8_t, ::pilo::i64_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::i64_t>*) b_data, err); },//7
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::u64_t>((const std::map<::pilo::i8_t, ::pilo::u64_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::u64_t>*) b_data, err); },//8
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, bool>((const std::map<::pilo::i8_t, bool>*) a_data, (const std::map<::pilo::i8_t, bool>*) b_data, err); },//9
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::f32_t>((const std::map<::pilo::i8_t, ::pilo::f32_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::f32_t>*) b_data, err); },//10
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, ::pilo::f64_t>((const std::map<::pilo::i8_t, ::pilo::f64_t>*) a_data, (const std::map<::pilo::i8_t, ::pilo::f64_t>*) b_data, err); },//11
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t>((const std::map<::pilo::i8_t, char*>*) a_data, (const std::map<::pilo::i8_t, char*>*) b_data, err); },//12
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t, std::string>((const std::map<::pilo::i8_t, std::string>*) a_data, (const std::map<::pilo::i8_t, std::string>*) b_data, err); },//13
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i8_t>((const std::map<::pilo::i8_t, ::pilo::tlv*>*) a_data, (const std::map<::pilo::i8_t, ::pilo::tlv*>*) b_data, err); },//14
				},
				{//u8
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },//0
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::i8_t>((const std::map<::pilo::u8_t, ::pilo::i8_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::i8_t>*) b_data, err); },//1
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::u8_t>((const std::map<::pilo::u8_t, ::pilo::u8_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::u8_t>*) b_data, err); },//2
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::i16_t>((const std::map<::pilo::u8_t, ::pilo::i16_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::i16_t>*) b_data, err); },//3
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::u16_t>((const std::map<::pilo::u8_t, ::pilo::u16_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::u16_t>*) b_data, err); },//4
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::i32_t>((const std::map<::pilo::u8_t, ::pilo::i32_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::i32_t>*) b_data, err); },//5
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::u32_t>((const std::map<::pilo::u8_t, ::pilo::u32_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::u32_t>*) b_data, err); },//6
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::i64_t>((const std::map<::pilo::u8_t, ::pilo::i64_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::i64_t>*) b_data, err); },//7
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::u64_t>((const std::map<::pilo::u8_t, ::pilo::u64_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::u64_t>*) b_data, err); },//8
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, bool>((const std::map<::pilo::u8_t, bool>*) a_data, (const std::map<::pilo::u8_t, bool>*) b_data, err); },//9
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::f32_t>((const std::map<::pilo::u8_t, ::pilo::f32_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::f32_t>*) b_data, err); },//10
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, ::pilo::f64_t>((const std::map<::pilo::u8_t, ::pilo::f64_t>*) a_data, (const std::map<::pilo::u8_t, ::pilo::f64_t>*) b_data, err); },//11
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t>((const std::map<::pilo::u8_t, char*>*) a_data, (const std::map<::pilo::u8_t, char*>*) b_data, err); },//12
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t, std::string>((const std::map<::pilo::u8_t, std::string>*) a_data, (const std::map<::pilo::u8_t, std::string>*) b_data, err); },//13
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u8_t>((const std::map<::pilo::u8_t, ::pilo::tlv*>*) a_data, (const std::map<::pilo::u8_t, ::pilo::tlv*>*) b_data, err); },//14
				},
				{//i16
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },//0
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::i8_t>((const std::map<::pilo::i16_t, ::pilo::i8_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::i8_t>*) b_data, err); },//1
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::u8_t>((const std::map<::pilo::i16_t, ::pilo::u8_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::u8_t>*) b_data, err); },//2
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::i16_t>((const std::map<::pilo::i16_t, ::pilo::i16_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::i16_t>*) b_data, err); },//3
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::u16_t>((const std::map<::pilo::i16_t, ::pilo::u16_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::u16_t>*) b_data, err); },//4
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::i32_t>((const std::map<::pilo::i16_t, ::pilo::i32_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::i32_t>*) b_data, err); },//5
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::u32_t>((const std::map<::pilo::i16_t, ::pilo::u32_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::u32_t>*) b_data, err); },//6
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::i64_t>((const std::map<::pilo::i16_t, ::pilo::i64_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::i64_t>*) b_data, err); },//7
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::u64_t>((const std::map<::pilo::i16_t, ::pilo::u64_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::u64_t>*) b_data, err); },//8
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, bool>((const std::map<::pilo::i16_t, bool>*) a_data, (const std::map<::pilo::i16_t, bool>*) b_data, err); },//9
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::f32_t>((const std::map<::pilo::i16_t, ::pilo::f32_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::f32_t>*) b_data, err); },//10
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, ::pilo::f64_t>((const std::map<::pilo::i16_t, ::pilo::f64_t>*) a_data, (const std::map<::pilo::i16_t, ::pilo::f64_t>*) b_data, err); },//11
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t>((const std::map<::pilo::i16_t, char*>*) a_data, (const std::map<::pilo::i16_t, char*>*) b_data, err); },//12
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t, std::string>((const std::map<::pilo::i16_t, std::string>*) a_data, (const std::map<::pilo::i16_t, std::string>*) b_data, err); },//13
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i16_t>((const std::map<::pilo::i16_t, ::pilo::tlv*>*) a_data, (const std::map<::pilo::i16_t, ::pilo::tlv*>*) b_data, err); },//14
				},
				{//u16
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },//0
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::i8_t>((const std::map<::pilo::u16_t, ::pilo::i8_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::i8_t>*) b_data, err); },//1
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::u8_t>((const std::map<::pilo::u16_t, ::pilo::u8_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::u8_t>*) b_data, err); },//2
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::i16_t>((const std::map<::pilo::u16_t, ::pilo::i16_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::i16_t>*) b_data, err); },//3
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::u16_t>((const std::map<::pilo::u16_t, ::pilo::u16_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::u16_t>*) b_data, err); },//4
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::i32_t>((const std::map<::pilo::u16_t, ::pilo::i32_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::i32_t>*) b_data, err); },//5
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::u32_t>((const std::map<::pilo::u16_t, ::pilo::u32_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::u32_t>*) b_data, err); },//6
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::i64_t>((const std::map<::pilo::u16_t, ::pilo::i64_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::i64_t>*) b_data, err); },//7
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::u64_t>((const std::map<::pilo::u16_t, ::pilo::u64_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::u64_t>*) b_data, err); },//8
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, bool>((const std::map<::pilo::u16_t, bool>*) a_data, (const std::map<::pilo::u16_t, bool>*) b_data, err); },//9
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::f32_t>((const std::map<::pilo::u16_t, ::pilo::f32_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::f32_t>*) b_data, err); },//10
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, ::pilo::f64_t>((const std::map<::pilo::u16_t, ::pilo::f64_t>*) a_data, (const std::map<::pilo::u16_t, ::pilo::f64_t>*) b_data, err); },//11
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t>((const std::map<::pilo::u16_t, char*>*) a_data, (const std::map<::pilo::u16_t, char*>*) b_data, err); },//12
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t, std::string>((const std::map<::pilo::u16_t, std::string>*) a_data, (const std::map<::pilo::u16_t, std::string>*) b_data, err); },//13
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u16_t>((const std::map<::pilo::u16_t, ::pilo::tlv*>*) a_data, (const std::map<::pilo::u16_t, ::pilo::tlv*>*) b_data, err); },//14
				},
				{//i32
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },//0
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::i8_t>((const std::map<::pilo::i32_t, ::pilo::i8_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::i8_t>*) b_data, err); },//1
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::u8_t>((const std::map<::pilo::i32_t, ::pilo::u8_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::u8_t>*) b_data, err); },//2
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::i16_t>((const std::map<::pilo::i32_t, ::pilo::i16_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::i16_t>*) b_data, err); },//3
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::u16_t>((const std::map<::pilo::i32_t, ::pilo::u16_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::u16_t>*) b_data, err); },//4
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::i32_t>((const std::map<::pilo::i32_t, ::pilo::i32_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::i32_t>*) b_data, err); },//5
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::u32_t>((const std::map<::pilo::i32_t, ::pilo::u32_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::u32_t>*) b_data, err); },//6
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::i64_t>((const std::map<::pilo::i32_t, ::pilo::i64_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::i64_t>*) b_data, err); },//7
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::u64_t>((const std::map<::pilo::i32_t, ::pilo::u64_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::u64_t>*) b_data, err); },//8
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, bool>((const std::map<::pilo::i32_t, bool>*) a_data, (const std::map<::pilo::i32_t, bool>*) b_data, err); },//9
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::f32_t>((const std::map<::pilo::i32_t, ::pilo::f32_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::f32_t>*) b_data, err); },//10
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, ::pilo::f64_t>((const std::map<::pilo::i32_t, ::pilo::f64_t>*) a_data, (const std::map<::pilo::i32_t, ::pilo::f64_t>*) b_data, err); },//11
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t>((const std::map<::pilo::i32_t, char*>*) a_data, (const std::map<::pilo::i32_t, char*>*) b_data, err); },//12
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t, std::string>((const std::map<::pilo::i32_t, std::string>*) a_data, (const std::map<::pilo::i32_t, std::string>*) b_data, err); },//13
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i32_t>((const std::map<::pilo::i32_t, ::pilo::tlv*>*) a_data, (const std::map<::pilo::i32_t, ::pilo::tlv*>*) b_data, err); },//14
				},
				{//u32
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },//0
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::i8_t>((const std::map<::pilo::u32_t, ::pilo::i8_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::i8_t>*) b_data, err); },//1
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::u8_t>((const std::map<::pilo::u32_t, ::pilo::u8_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::u8_t>*) b_data, err); },//2
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::i16_t>((const std::map<::pilo::u32_t, ::pilo::i16_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::i16_t>*) b_data, err); },//3
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::u16_t>((const std::map<::pilo::u32_t, ::pilo::u16_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::u16_t>*) b_data, err); },//4
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::i32_t>((const std::map<::pilo::u32_t, ::pilo::i32_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::i32_t>*) b_data, err); },//5
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::u32_t>((const std::map<::pilo::u32_t, ::pilo::u32_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::u32_t>*) b_data, err); },//6
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::i64_t>((const std::map<::pilo::u32_t, ::pilo::i64_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::i64_t>*) b_data, err); },//7
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::u64_t>((const std::map<::pilo::u32_t, ::pilo::u64_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::u64_t>*) b_data, err); },//8
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, bool>((const std::map<::pilo::u32_t, bool>*) a_data, (const std::map<::pilo::u32_t, bool>*) b_data, err); },//9
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::f32_t>((const std::map<::pilo::u32_t, ::pilo::f32_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::f32_t>*) b_data, err); },//10
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, ::pilo::f64_t>((const std::map<::pilo::u32_t, ::pilo::f64_t>*) a_data, (const std::map<::pilo::u32_t, ::pilo::f64_t>*) b_data, err); },//11
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t>((const std::map<::pilo::u32_t, char*>*) a_data, (const std::map<::pilo::u32_t, char*>*) b_data, err); },//12
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t, std::string>((const std::map<::pilo::u32_t, std::string>*) a_data, (const std::map<::pilo::u32_t, std::string>*) b_data, err); },//13
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u32_t>((const std::map<::pilo::u32_t, ::pilo::tlv*>*) a_data, (const std::map<::pilo::u32_t, ::pilo::tlv*>*) b_data, err); },//14
				},
				{//i64
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },//0
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::i8_t>((const std::map<::pilo::i64_t, ::pilo::i8_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::i8_t>*) b_data, err); },//1
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::u8_t>((const std::map<::pilo::i64_t, ::pilo::u8_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::u8_t>*) b_data, err); },//2
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::i16_t>((const std::map<::pilo::i64_t, ::pilo::i16_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::i16_t>*) b_data, err); },//3
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::u16_t>((const std::map<::pilo::i64_t, ::pilo::u16_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::u16_t>*) b_data, err); },//4
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::i32_t>((const std::map<::pilo::i64_t, ::pilo::i32_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::i32_t>*) b_data, err); },//5
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::u32_t>((const std::map<::pilo::i64_t, ::pilo::u32_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::u32_t>*) b_data, err); },//6
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::i64_t>((const std::map<::pilo::i64_t, ::pilo::i64_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::i64_t>*) b_data, err); },//7
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::u64_t>((const std::map<::pilo::i64_t, ::pilo::u64_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::u64_t>*) b_data, err); },//8
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, bool>((const std::map<::pilo::i64_t, bool>*) a_data, (const std::map<::pilo::i64_t, bool>*) b_data, err); },//9
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::f32_t>((const std::map<::pilo::i64_t, ::pilo::f32_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::f32_t>*) b_data, err); },//10
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, ::pilo::f64_t>((const std::map<::pilo::i64_t, ::pilo::f64_t>*) a_data, (const std::map<::pilo::i64_t, ::pilo::f64_t>*) b_data, err); },//11
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t>((const std::map<::pilo::i64_t, char*>*) a_data, (const std::map<::pilo::i64_t, char*>*) b_data, err); },//12
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t, std::string>((const std::map<::pilo::i64_t, std::string>*) a_data, (const std::map<::pilo::i64_t, std::string>*) b_data, err); },//13
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::i64_t>((const std::map<::pilo::i64_t, ::pilo::tlv*>*) a_data, (const std::map<::pilo::i64_t, ::pilo::tlv*>*) b_data, err); },//14
				},
				{//u64
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },//0
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::i8_t>((const std::map<::pilo::u64_t, ::pilo::i8_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::i8_t>*) b_data, err); },//1
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::u8_t>((const std::map<::pilo::u64_t, ::pilo::u8_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::u8_t>*) b_data, err); },//2
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::i16_t>((const std::map<::pilo::u64_t, ::pilo::i16_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::i16_t>*) b_data, err); },//3
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::u16_t>((const std::map<::pilo::u64_t, ::pilo::u16_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::u16_t>*) b_data, err); },//4
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::i32_t>((const std::map<::pilo::u64_t, ::pilo::i32_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::i32_t>*) b_data, err); },//5
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::u32_t>((const std::map<::pilo::u64_t, ::pilo::u32_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::u32_t>*) b_data, err); },//6
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::i64_t>((const std::map<::pilo::u64_t, ::pilo::i64_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::i64_t>*) b_data, err); },//7
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::u64_t>((const std::map<::pilo::u64_t, ::pilo::u64_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::u64_t>*) b_data, err); },//8
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, bool>((const std::map<::pilo::u64_t, bool>*) a_data, (const std::map<::pilo::u64_t, bool>*) b_data, err); },//9
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::f32_t>((const std::map<::pilo::u64_t, ::pilo::f32_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::f32_t>*) b_data, err); },//10
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, ::pilo::f64_t>((const std::map<::pilo::u64_t, ::pilo::f64_t>*) a_data, (const std::map<::pilo::u64_t, ::pilo::f64_t>*) b_data, err); },//11
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t>((const std::map<::pilo::u64_t, char*>*) a_data, (const std::map<::pilo::u64_t, char*>*) b_data, err); },//12
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t, std::string>((const std::map<::pilo::u64_t, std::string>*) a_data, (const std::map<::pilo::u64_t, std::string>*) b_data, err); },//13
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<::pilo::u64_t>((const std::map<::pilo::u64_t, ::pilo::tlv*>*) a_data, (const std::map<::pilo::u64_t, ::pilo::tlv*>*) b_data, err); },//14
				},
				{//str
					[](const char*, const char*, ::pilo::err_t* err) -> bool { if (err != nullptr) *err = ::pilo::mk_perr(PERR_OP_UNSUPPORT); return false; },//0
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::i8_t>((const std::map<std::string, ::pilo::i8_t>*) a_data, (const std::map<std::string, ::pilo::i8_t>*) b_data, err); },//1
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::u8_t>((const std::map<std::string, ::pilo::u8_t>*) a_data, (const std::map<std::string, ::pilo::u8_t>*) b_data, err); },//2
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::i16_t>((const std::map<std::string, ::pilo::i16_t>*) a_data, (const std::map<std::string, ::pilo::i16_t>*) b_data, err); },//3
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::u16_t>((const std::map<std::string, ::pilo::u16_t>*) a_data, (const std::map<std::string, ::pilo::u16_t>*) b_data, err); },//4
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::i32_t>((const std::map<std::string, ::pilo::i32_t>*) a_data, (const std::map<std::string, ::pilo::i32_t>*) b_data, err); },//5
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::u32_t>((const std::map<std::string, ::pilo::u32_t>*) a_data, (const std::map<std::string, ::pilo::u32_t>*) b_data, err); },//6
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::i64_t>((const std::map<std::string, ::pilo::i64_t>*) a_data, (const std::map<std::string, ::pilo::i64_t>*) b_data, err); },//7
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::u64_t>((const std::map<std::string, ::pilo::u64_t>*) a_data, (const std::map<std::string, ::pilo::u64_t>*) b_data, err); },//8
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, bool>((const std::map<std::string, bool>*) a_data, (const std::map<std::string, bool>*) b_data, err); },//9
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::f32_t>((const std::map<std::string, ::pilo::f32_t>*) a_data, (const std::map<std::string, ::pilo::f32_t>*) b_data, err); },//10
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, ::pilo::f64_t>((const std::map<std::string, ::pilo::f64_t>*) a_data, (const std::map<std::string, ::pilo::f64_t>*) b_data, err); },//11
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string>((const std::map<std::string, char*>*) a_data, (const std::map<std::string, char*>*) b_data, err); },//12
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string, std::string>((const std::map<std::string, std::string>*) a_data, (const std::map<std::string, std::string>*) b_data, err); },//13
					[](const char* a_data, const char* b_data, ::pilo::err_t* err) -> bool { return map_equals<std::string>((const std::map<std::string, ::pilo::tlv*>*) a_data, (const std::map<std::string, ::pilo::tlv*>*) b_data, err); },//14
				},
			};


			bool map_equals(::pilo::u8_t a_key_type, ::pilo::u16_t a_val_type, const char* a_data, ::pilo::u8_t b_key_type, ::pilo::u16_t b_val_type, const char* b_data, ::pilo::err_t *err)
			{
				if (a_key_type != b_key_type) 
					return false;
				if (a_val_type != b_val_type) 
					return false;
				if (a_data == b_data)
					return true;
				if (a_data == nullptr || b_data == nullptr) 
					return false;

				if (a_key_type >= ::pilo::core::rtti::wired_type::key_type_count)
				{
					if (err != nullptr)
						*err = ::pilo::mk_perr(PERR_MIS_DATA_TYPE);
					return false;
				}

				if (a_val_type >= ::pilo::core::rtti::wired_type::value_type_intrincs_count)
				{
					PMC_ASSERT(false);
					return false;
				}

				return stc_handle_compare_func[a_key_type][a_val_type](a_data, b_data, err);

			}
			bool map_equals(::pilo::u8_t a_key_type, ::pilo::u16_t a_val_type, const char* a_data, const char* b_data, ::pilo::err_t* err)
			{
				return map_equals(a_key_type, a_val_type, a_data, a_key_type, a_val_type, b_data, err);
			}
		}
	}
}