#include "./pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/tlv.hpp"
#include "pilo/core/rtti/wired_type_factory.hpp"
#include "pilo/core/memory/linked_byte_buffer.hpp"

#include <memory>

using namespace ::pilo::func_test;

namespace pilo
{
	namespace func_test
	{
		static bool _validate_tlv_internal(::pilo::core::testing::func_test_case* p_case, std::shared_ptr<::pilo::tlv> tlv, ::pilo::u8_t ism, ::pilo::u8_t wt, ::pilo::u8_t kt, ::pilo::u16_t vt, ::pilo::i32_t sz )
		{
			char errbuf[128] = { 0 };
			if (tlv->value_type() != vt || tlv->wrapper_type() != wt)
			{
				p_case->set_result(::pilo::make_core_error(PES_TCASE, PEP_TYPE_MISMATCH));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "internal vtype(%u) and wtype(%u) mismatch. should be %u %u\n"
					, tlv->value_type(), tlv->wrapper_type()
					, vt, wt);
				p_case->set_desc(errbuf);
				return false;
			}
			if (tlv->size() != sz)
			{
				p_case->set_result(::pilo::make_core_error(PES_TCASE, PEP_LEN_INV));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "size (%d) invalid should be (%d)\n", tlv->size(), sz);
				p_case->set_desc(errbuf);
				return false;
			}
			if (tlv->category_id() != ism)
			{
				p_case->set_result(::pilo::make_core_error(PES_TCASE, PEP_INC_DATA));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "category_id (%d) invalid should be (%d)\n", tlv->category_id(), ism);
				p_case->set_desc(errbuf);
				return false;
			}
			if (tlv->key_type() != kt)
			{
				p_case->set_result(::pilo::make_core_error(PES_TCASE, PEP_INC_DATA));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "key_type (%d) invalid should be (%d)\n", tlv->key_type(), kt);
				p_case->set_desc(errbuf);
				return false;
			}
			return true;
		}

		int case_tlv_basic(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::tlv* t = (::pilo::tlv*) PILO_WIRED_TYPE_FACTORY->allocate(0, ::pilo::core::rtti::wired_type::value_type_tlv);
			if (t == nullptr)
				return 9;

			::pilo::err_t err = PILO_OK;
			::pilo::tlv* tlv_0 = ::pilo::tlv::allocate();
			char errbuf[128] = { 0 };
			
			if (tlv_0->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}
			//---------------- i8
			tlv_0->set((::pilo::i64_t)127);
			::pilo::i8_t i8v = tlv_0->as_i8(&err);
			if (err != PILO_OK || i8v != 127)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 127 i64 as i8 failed %d", i8v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::u64_t)256);			
			if (tlv_0->value_type() != ::pilo::core::rtti::wired_type::key_type_u64)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_TYPE_MISMATCH));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "expect %d type but got %d type %s", ::pilo::core::rtti::wired_type::value_type_u64 ,tlv_0->value_type(), ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			//-------------------------- i16 
			tlv_0->set((::pilo::i64_t)PILO_INT16_MAX);
			::pilo::i16_t i16v = tlv_0->as_i16(&err);
			if (err != PILO_OK || i16v != PILO_INT16_MAX)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get %d i64 as i16 failed %d", PILO_INT16_MAX, i16v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			tlv_0->set((::pilo::i64_t)PILO_INT16_MIN);
			i16v = tlv_0->as_i16(&err);
			if (err != PILO_OK || i16v != PILO_INT16_MIN)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get %d i64 as i16 failed %d", PILO_INT16_MIN, i16v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			

			//-------------------------- u16 
			tlv_0->set((::pilo::i64_t)(PILO_UINT16_MAX));
			::pilo::u16_t u16v = tlv_0->as_u16(&err);
			if (err != PILO_OK || u16v != PILO_UINT16_MAX)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get %d i64 as u16 failed %d", PILO_UINT16_MAX, u16v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			
			if (tlv_0->value_type() != ::pilo::core::rtti::wired_type::value_type_i64)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_TYPE_MISMATCH));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "expect %d type but got %d type %s", ::pilo::core::rtti::wired_type::value_type_u64, tlv_0->value_type(), ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			//---------------- i32
			tlv_0->set((::pilo::i64_t)PILO_INT32_MAX);
			::pilo::i32_t i32v = tlv_0->as_i32(&err);
			if (err != PILO_OK || i32v != PILO_INT32_MAX)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get %d i64 as i32 failed %d", PILO_INT32_MAX, i32v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			tlv_0->set((::pilo::i64_t)PILO_INT32_MIN);
			i32v = tlv_0->as_i32(&err);
			if (err != PILO_OK || i32v != PILO_INT32_MIN)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get %d i64 as i32 failed %d", PILO_INT32_MIN, i32v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			
			//-----------------u32
			tlv_0->set((::pilo::i64_t)(PILO_UINT32_MAX));
			::pilo::u32_t u32v = tlv_0->as_u32(&err);
			if (err != PILO_OK || u32v != PILO_UINT32_MAX)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get %d i64 as u32 failed %d", PILO_UINT32_MAX, u32v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			
			if (tlv_0->value_type() != ::pilo::core::rtti::wired_type::value_type_i64)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_TYPE_MISMATCH));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "expect %d type but got %d type %s", ::pilo::core::rtti::wired_type::value_type_u64, tlv_0->value_type(), ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			//-----------------i64
			tlv_0->set((::pilo::i64_t)PILO_INT64_MAX);
			::pilo::i64_t i64v = tlv_0->as_i64(&err);
			if (err != PILO_OK || i64v != PILO_INT64_MAX)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get %lld i64 as i64 failed %d", PILO_INT64_MAX, i64v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			tlv_0->set((::pilo::i64_t)PILO_INT64_MIN);
			i64v = tlv_0->as_i64(&err);
			if (err != PILO_OK || i64v != PILO_INT64_MIN)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get %lld i64 as i64 failed %d", PILO_INT64_MAX, i64v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			//-----------------i64
			tlv_0->set((::pilo::u64_t)(PILO_UINT64_MAX));
			::pilo::u64_t u64v = tlv_0->as_u64(&err);
			if (err != PILO_OK || u64v != PILO_UINT64_MAX)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get %lld i64 as u64 failed %d", PILO_UINT64_MAX, u64v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			//-----------------f32
			tlv_0->set((::pilo::f32_t)3.14f);
			i8v = tlv_0->as_i8(&err);
			if (err != PILO_OK || i8v != 3)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 3.14 f32 as i8 failed %d",  i8v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			::pilo::u8_t u8v = tlv_0->as_u8(&err);
			if (err != PILO_OK || u8v != 3)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 3.14 f32 as u8 failed %d", u8v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::f32_t)30000.14f);
			i16v = tlv_0->as_i16(&err);
			if (err != PILO_OK || i16v != 30000)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 30000.14 f32 as i16 failed %d",  i16v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			u16v = tlv_0->as_u16(&err);
			if (err != PILO_OK || u16v != 30000)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 30000.14 f32 as i16 failed %d", u16v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::f32_t)200001.14f);
			i32v = tlv_0->as_i32(&err);
			if (err != PILO_OK || i32v != 200001)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 200001.14 f32 as i32 failed %d", i32v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			u32v = tlv_0->as_u32(&err);
			if (err != PILO_OK || u32v != 200001)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 200001.14 f32 as i32 failed %d", u32v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::f32_t)200001.14f);
			i64v = tlv_0->as_i64(&err);
			if (err != PILO_OK || i64v != 200001)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 200001.14 f32 as i64 failed %d", i64v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			u64v = tlv_0->as_u64(&err);
			if (err != PILO_OK || u64v != 200001)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 200001.14 f32 as i64 failed %d", u64v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::f64_t)2.71f);
			i8v = tlv_0->as_i8(&err);
			if (err != PILO_OK || i8v != 2)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 2.71 f64 as i8 failed %d", PILO_UINT64_MAX, i8v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}			

			u8v = tlv_0->as_u8(&err);
			if (err != PILO_OK || u8v != 2)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 3.14 f64 as u8 failed %d", PILO_UINT64_MAX, u8v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::f64_t)30000.14f);
			i16v = tlv_0->as_i16(&err);
			if (err != PILO_OK || i16v != 30000)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 30000.14 f64 as i16 failed %d", i16v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			u16v = tlv_0->as_u16(&err);
			if (err != PILO_OK || u16v != 30000)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 30000.14 f64 as i16 failed %d", u16v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::f64_t)200001.14f);
			i32v = tlv_0->as_i32(&err);
			if (err != PILO_OK || i32v != 200001)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 200001.14 f64 as i32 failed %d", i32v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			u32v = tlv_0->as_u32(&err);
			if (err != PILO_OK || u32v != 200001)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 200001.14 f64 as i32 failed %d", u32v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::f64_t)123456789.1234);
			i64v = tlv_0->as_i64(&err);
			if (err != PILO_OK || i64v != 123456789)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 123456789.1234 f64 as i64 failed %d", i64v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			u64v = tlv_0->as_u64(&err);
			if (err != PILO_OK || u64v != 123456789)
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_INVALID));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get 123456789.1234 f64 as i64 failed %d", u64v);
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			::pilo::tlv::deallocate(tlv_0);
			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_str(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* tlv_0 = ::pilo::tlv::allocate();
			char errbuf[128] = { 0 };

			if (tlv_0->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}

			tlv_0->set(std::string("123"));
			::pilo::i8_t i8v = tlv_0->as_i8(&err);
			if (err != PILO_OK || i8v != 123)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str 123 as i8 failed %d err:%s",i8v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("-123"));
			i8v = tlv_0->as_i8(&err);
			if (err != PILO_OK || i8v != -123)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str -123 as i8 failed %d err:%s", i8v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("255"));
			::pilo::u8_t u8v = tlv_0->as_u8(&err);
			if (err != PILO_OK || u8v != 255)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str 123 as u8 failed %d err:%s", u8v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("-32768"));
			::pilo::i16_t i16v = tlv_0->as_i16(&err);
			if (err != PILO_OK || i16v != -32768)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str -32768 as i16 failed %d err:%s", i16v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("32767"));
			i16v = tlv_0->as_i16(&err);
			if (err != PILO_OK || i16v != 32767)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str 32767 as i16 failed %d err:%s", i16v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("65535"));
			::pilo::u16_t u16v = tlv_0->as_u16(&err);
			if (err != PILO_OK || u16v != 65535)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str 65535 as i16 failed %d err:%s", u16v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("-2147483648"));
			::pilo::i32_t i32v = tlv_0->as_i32(&err);
			if (err != PILO_OK || i32v != PILO_INT32_MIN)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str -2147483648 as i32 failed %d err:%s", i32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("2147483647"));
			i32v = tlv_0->as_i32(&err);
			if (err != PILO_OK || i32v != PILO_INT32_MAX)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str 2147483647 as i32 failed %d err:%s", i32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("4294967295"));
			::pilo::u32_t u32v = tlv_0->as_u32(&err);
			if (err != PILO_OK || u32v != PILO_UINT32_MAX)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str 4294967295 as u32 failed %d err:%s", u32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("-9223372036854775808"));
			::pilo::i64_t i64v = tlv_0->as_i64(&err);
			if (err != PILO_OK || i64v != PILO_INT64_MIN)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str -9223372036854775808 as i64 failed %lld err:%s", i64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set(std::string("9223372036854775807"));
			i64v = tlv_0->as_i64(&err);
			if (err != PILO_OK || i64v != PILO_INT64_MAX)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str 9223372036854775807 as i64 failed %lld err:%s", i64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}


			std::string s = tlv_0->as_str(&err);
			if (err != PILO_OK || s != "9223372036854775807")
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str 9223372036854775807 as str failed %s err:%s", s.c_str(), ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			tlv_0->set((::pilo::i8_t)-123);
			s = tlv_0->as_str(&err);
			if (err != PILO_OK || s != "-123")
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str -123 as str failed %s err:%s", s.c_str(), ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			s = tlv_0->as_str(&err, "%02x");
			if (err != PILO_OK || s != "85")
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str -123 as str failed %s err:%s", s.c_str(), ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::i16_t)-11223);
			s = tlv_0->as_str(&err);
			if (err != PILO_OK || s != "-11223")
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str -11223 as str failed %s err:%s", s.c_str(), ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			s = tlv_0->as_str(&err, "%04hx");
			if (err != PILO_OK || s != "d429")
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get str -11223 as str failed %s err:%s", s.c_str(), ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}


			::pilo::tlv::deallocate(tlv_0);
			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_float(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			::pilo::tlv* tlv_0 = ::pilo::tlv::allocate();
			char errbuf[128] = { 0 };

			if (tlv_0->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}

			tlv_0->set((::pilo::i8_t)-23);
			::pilo::f32_t f32v = tlv_0->as_f32(&err);
			if (err != PILO_OK || f32v != -23)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i8_t 23 as f32 failed %f err:%s", f32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			::pilo::f64_t f64v = tlv_0->as_f64(&err);
			if (err != PILO_OK || f32v != -23)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i8_t 23 as f64 failed %f err:%s", f64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::u8_t) 125);
			f32v = tlv_0->as_f32(&err);
			if (err != PILO_OK || f32v != 125)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i8_t 23 as f32 failed %f err:%s", f32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			f64v = tlv_0->as_f64(&err);
			if (err != PILO_OK || f32v != 125)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i8_t 23 as f64 failed %f err:%s", f64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::i16_t)-22113);
			f32v = tlv_0->as_f32(&err);
			if (err != PILO_OK || f32v != -22113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i16_t -22113 as f32 failed %f err:%s", f32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			f64v = tlv_0->as_f64(&err);
			if (err != PILO_OK || f32v != -22113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i16_t -22113 as f64 failed %f err:%s", f64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::u16_t)62113);
			f32v = tlv_0->as_f32(&err);
			if (err != PILO_OK || f32v != 62113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get u16_t 62113 as f32 failed %f err:%s", f32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			f64v = tlv_0->as_f64(&err);
			if (err != PILO_OK || f32v != 62113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get u16_t 62113 as f64 failed %f err:%s", f64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::i32_t)-8222113);
			f32v = tlv_0->as_f32(&err);
			if (err != PILO_OK || f32v != -8222113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i32_t -8222113 as f32 failed %f err:%s", f32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			f64v = tlv_0->as_f64(&err);
			if (err != PILO_OK || f32v != -8222113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i32_t -8222113 as f64 failed %f err:%s", f64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::u32_t)8222113);
			f32v = tlv_0->as_f32(&err);
			if (err != PILO_OK || f32v != 8222113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get u32_t 8222113 as f32 failed %f err:%s", f32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			f64v = tlv_0->as_f64(&err);
			if (err != PILO_OK || f32v != 8222113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get u32_t 8222113 as f64 failed %f err:%s", f64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::i64_t)-8222113);
			f32v = tlv_0->as_f32(&err);
			if (err != PILO_OK || f32v != -8222113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i64_t -8222113 as f32 failed %f err:%s", f32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			f64v = tlv_0->as_f64(&err);
			if (err != PILO_OK || f32v != -8222113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get i64_t -8222113 as f64 failed %f err:%s", f64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			tlv_0->set((::pilo::u64_t)8222113);
			f32v = tlv_0->as_f32(&err);
			if (err != PILO_OK || f32v != 8222113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get u64_t 8222113 as f32 failed %f err:%s", f32v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			f64v = tlv_0->as_f64(&err);
			if (err != PILO_OK || f32v != 8222113)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "get u64_t 8222113 as f64 failed %f err:%s", f64v, ::pilo::str_error(err).c_str());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			::pilo::tlv::deallocate(tlv_0);
			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			char errbuf[128] = { 0 };
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}


			std::deque<std::string> vec;
			vec.push_back(std::string("fox1"));
			vec.push_back(std::string("fox2"));
			vec.push_back(std::string("fox3"));
			tlv->set(vec);
			if (err != PILO_OK)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "make a array of string type tlv failed");
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			bool br = _validate_tlv_internal(p_case, tlv, 0, ::pilo::core::rtti::wired_type::wrapper_array, ::pilo::core::rtti::wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_str, 3);
			if (!br)
			{
				return PILO_OK;
			}
			tlv->push_back(std::string("fox4"));
			tlv->push_front(std::string("fox0"));
			br = _validate_tlv_internal(p_case, tlv, 0, ::pilo::core::rtti::wired_type::wrapper_array, ::pilo::core::rtti::wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_str, 5);
			if (!br)
			{
				return PILO_OK;
			}
			tlv->set(2, std::string("fox2b"));
			std::string ret_str = tlv->get<std::string>((::pilo::i32_t)2, &err);
			if (ret_str != "fox2b")
			{
				p_case->set_result(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "val str failed");
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			tlv->insert(3, std::string("fox2.5"));
			ret_str = tlv->get<std::string>((::pilo::i32_t)3, &err);
			if (ret_str != "fox2.5")
			{
				p_case->set_result(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "val str failed");
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			err = tlv->erase<std::string>(3);
			br = _validate_tlv_internal(p_case, tlv, 0, ::pilo::core::rtti::wired_type::wrapper_array, ::pilo::core::rtti::wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_str, 5);
			if (!br)
			{
				return PILO_OK;
			}
			
			tlv->push_back(0);


			tlv->wrapper_clear(true);

			std::deque<::pilo::tlv*> vec2;
			err = tlv->set(vec2);			
			if (err != PILO_OK)
			{
				p_case->set_result(err != PILO_OK ? err : ::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED));;
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "make a array of tlv type tlv failed");
				p_case->set_desc(errbuf);
				return PILO_OK;
			}
			br = _validate_tlv_internal(p_case, tlv, 0, ::pilo::core::rtti::wired_type::wrapper_array, ::pilo::core::rtti::wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_tlv, 0);
			if (!br)
			{
				return PILO_OK;
			}			

			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_bool(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			char errbuf[128] = { 0 };
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_boolean);
			if (err != PILO_OK)
			{
				p_case->set_result(err);
				return PILO_OK;
			}

			tlv->push_back(false);
			tlv->push_back(true);
			tlv->push_back(false);
			tlv->push_back(true);
			tlv->push_back(false);
			tlv->push_back(true);
			tlv->push_back(false);
			tlv->push_front(true);

			if (tlv->size() != 8)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));	
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "bool arr size invalid should be %d, actual %d", 8, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			bool cv = true;
			for (int i = 0; i < 8; i++)
			{
				if (tlv->get<bool>(i, &err) != cv)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "bool arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
				cv = cv ? false : true;
			}

			tlv->erase<bool>(3);
			tlv->erase<bool>(3);
	
			if (tlv->size() != 6)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "bool arr size invalid should be %d, actual %d", 6, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			cv = true;
			for (int i = 0; i < 6; i++)
			{
				if (tlv->get<bool>(i, &err) != cv)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "bool arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
				cv = cv ? false : true;
			}

			tlv->wrapper_clear(true);
			if (tlv->size() != 0)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "bool arr size invalid should be %d, actual %d", 0, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_i8(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			char errbuf[128] = { 0 };
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_i8);
			if (err != PILO_OK)
			{
				p_case->set_result(err);
				return PILO_OK;
			}

			for (::pilo::i32_t i = -128; i < 128; i++)
			{
				tlv->push_back((::pilo::i8_t)i);
			}			

			if (tlv->size() != 256)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "i8 arr size invalid should be %d, actual %d", 256, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			for (::pilo::i32_t i = -128; i < 128; i++)
			{
				if (tlv->get<::pilo::i8_t>(i+128, &err) != i)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "i8 arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
			}

			tlv->wrapper_clear(false);
			if (tlv->size() != 0)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "i8 arr size invalid should be %d, actual %d", 0, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			for (::pilo::i32_t i = 0; i < 256; i++)
			{
				tlv->push_front((::pilo::i8_t)i-128);
			}

			for (::pilo::i32_t i = 0; i < 256; i++)
			{
				if (tlv->get<::pilo::i8_t>(i, &err) != 127 - i)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "i8 arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
			}

			tlv->wrapper_clear(true);
			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_u8(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			char errbuf[128] = { 0 };
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_u8);
			if (err != PILO_OK)
			{
				p_case->set_result(err);
				return PILO_OK;
			}

			for (::pilo::i32_t i = 0; i < 256; i++)
			{
				tlv->push_back((::pilo::u8_t)i);
			}

			if (tlv->size() != 256)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "u8 arr size invalid should be %d, actual %d", 256, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			for (::pilo::i32_t i = 0; i < 256; i++)
			{
				if (tlv->get<::pilo::u8_t>(i, &err) != i)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "u8 arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
			}

			tlv->wrapper_clear(false);
			if (tlv->size() != 0)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "u8 arr size invalid should be %d, actual %d", 0, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			for (::pilo::i32_t i = 0; i < 256; i++)
			{
				tlv->push_front((::pilo::u8_t)i);
			}

			for (::pilo::i32_t i = 0; i < 256; i++)
			{
				if (tlv->get<::pilo::u8_t>(i, &err) != 255- i)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "u8 arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
			}

			tlv->wrapper_clear(true);
			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_i16(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			char errbuf[128] = { 0 };
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_i16);
			if (err != PILO_OK)
			{
				p_case->set_result(err);
				return PILO_OK;
			}

			for (::pilo::i32_t i = -32768; i < 32768; i++)
			{
				tlv->push_back((::pilo::i16_t)i);
			}

			if (tlv->size() != 65536)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "i16 arr size invalid should be %d, actual %d", 65535, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			for (::pilo::i32_t i = -32768; i < 32768; i++)
			{
				if (tlv->get<::pilo::i16_t>(i + 32768, &err) != i)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "i16 arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
			}

			tlv->wrapper_clear(false);
			if (tlv->size() != 0)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "i16 arr size invalid should be %d, actual %d", 0, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			for (::pilo::i32_t i = 0; i < 65536; i++)
			{
				tlv->push_front((::pilo::i16_t)i - 32768);
			}

			for (::pilo::i32_t i = 0; i < 65536; i++)
			{
				if (tlv->get<::pilo::i16_t>(i, &err) != 32767 - i)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "i16 arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
			}

			tlv->wrapper_clear(true);
			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_u16(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			char errbuf[128] = { 0 };
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_u16);
			if (err != PILO_OK)
			{
				p_case->set_result(err);
				return PILO_OK;
			}

			for (::pilo::i32_t i = 0; i < 65536; i++)
			{
				tlv->push_back((::pilo::u16_t)i);
			}

			if (tlv->size() != 65536)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "u16 arr size invalid should be %d, actual %d", 256, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			for (::pilo::i32_t i = 0; i < 65536; i++)
			{
				if (tlv->get<::pilo::u16_t>(i, &err) != i)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "u16 arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
			}

			tlv->wrapper_clear(false);
			if (tlv->size() != 0)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "u16 arr size invalid should be %d, actual %d", 0, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			for (::pilo::i32_t i = 0; i < 65536; i++)
			{
				tlv->push_front((::pilo::u16_t)i);
			}

			for (::pilo::i32_t i = 0; i < 65536; i++)
			{
				if (tlv->get<::pilo::u16_t>(i, &err) != 65535 - i)
				{
					p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 2));
					::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "u16 arr element %d validate failed", i);
					p_case->set_desc(errbuf);
					return PILO_OK;
				}
			}

			tlv->wrapper_clear(true);
			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_i32(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			char errbuf[128] = { 0 };
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_i32);
			if (err != PILO_OK)
			{
				p_case->set_result(err);
				return PILO_OK;
			}
			tlv->push_back(PILO_INT32_MAX);
			tlv->push_front(PILO_INT32_MIN);
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				tlv->push_back(i * 100000);
			}
			if (tlv->size() != 10002)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "i32 arr size invalid should be %d, actual %d", 256, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			if (tlv->get<::pilo::i32_t>(0, &err) != PILO_INT32_MIN)
			if (p_case->check_error(err, "i32 arr check 1st element failed shoudl be %d", PILO_INT32_MIN)) return PILO_OK;
			
			if (tlv->get<::pilo::i32_t>(1, &err) != PILO_INT32_MAX)
				if (p_case->check_error(err, "i32 arr check end element failed shoudl be %d", PILO_INT32_MAX)) return PILO_OK;
			for (::pilo::i32_t i = 2; i < 10002; i++)
			{
				if (tlv->get<::pilo::i32_t>(i, &err) != (i -2) * 100000)
				{
					p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "i32 arr check %d element failed should be %d", i, i - 2 * 100000);
				}				
			}

			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_u32(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			char errbuf[128] = { 0 };
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				p_case->set_result(::pilo::make_core_error(PES_TLV, PEP_IS_VALID));
				p_case->set_desc("initial state should be invalid.");
				return PILO_OK;
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_u32);
			if (err != PILO_OK)
			{
				p_case->set_result(err);
				return PILO_OK;
			}
			tlv->push_back(PILO_UINT32_MAX);
			tlv->push_front(0);
			for (::pilo::u32_t i = 0; i < 10000; i++)
			{
				tlv->push_back(i * 200000);
			}
			if (tlv->size() != 10002)
			{
				p_case->set_result(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1));
				::pilo::core::io::string_formated_output(errbuf, sizeof(errbuf), "u32 arr size invalid should be %d, actual %d", 10002, tlv->size());
				p_case->set_desc(errbuf);
				return PILO_OK;
			}

			if (tlv->get<::pilo::u32_t>(0, &err) != 0)
				if (p_case->check_error(err, "u32 arr check 1st element failed shoudl be %d", PILO_INT32_MIN)) return PILO_OK;

			if (tlv->get<::pilo::u32_t>(1, &err) != PILO_UINT32_MAX)
				if (p_case->check_error(err, "u32 arr check end element failed shoudl be %d", PILO_INT32_MAX)) return PILO_OK;
			for (::pilo::u32_t i = 2; i < 10002; i++)
			{
				if (tlv->get<::pilo::u32_t>(i, &err) != (i - 2) * 200000)
				{
					p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "u32 arr check %d element failed should be %d", i, i - 2 * 100000);
				}
			}

			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_i64(::pilo::core::testing::func_test_case* p_case)
		{
			const ::pilo::i32_t c_cnt = 1000000;
			::pilo::err_t err = PILO_OK;
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_VALID), "initial state should be invalid.");
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_i64);
			if (p_case->check_error(err, PILO_OK, "set arr type to i64 failed")) return PILO_OK;

			for (::pilo::i32_t i = 0; i < c_cnt; i++)
			{
				tlv->push_back((::pilo::i64_t) i);
			}

			if (tlv->size() != (size_t) c_cnt)
			{
				return p_case->error(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1), "i64 arr size invalid should be %d, actual %d", 256, tlv->size());
			}

			for (::pilo::i32_t i = 0; i < c_cnt; i++)
			{
				if (tlv->get<::pilo::i64_t>(i, &err) != i)
				{
					return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "i64 arr check %d element failed should be %d", i, i - 2 * 100000);
				}
			}

			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_u64(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_VALID), "initial state should be invalid.");
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_u64);
			if (p_case->check_error(err, PILO_OK, "set arr type to u64 failed")) return PILO_OK;

			tlv->push_back((::pilo::u64_t)PILO_UINT64_MAX);
			tlv->push_front(0);
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				::pilo::u64_t u64v = ((::pilo::u64_t) i) * 100000000;
				tlv->push_back(u64v);
			}
			if (tlv->size() != 10002)
			{
				return p_case->error(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1), "u64 arr size invalid should be %d, actual %d", 256, tlv->size());
			}

			if (tlv->get<::pilo::u64_t>(0, &err) != 0)
			{
				return p_case->error(err, "u64 arr check 1st element failed shoudl be %d", 0);
			}
				

			if (tlv->get<::pilo::u64_t>(1, &err) != PILO_UINT64_MAX)
			{
				return p_case->error(err, "u64 arr check end element failed shoudl be %d", PILO_INT64_MAX);
			}
			for (::pilo::i32_t i = 2; i < 10002; i++)
			{
				if (tlv->get<::pilo::u64_t>(i, &err) != (::pilo::u64_t)( ((::pilo::u64_t)i - 2) * 100000000))
				{
					p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "u64 arr check %d element failed should be %d", i, i - 2 * 100000);
				}
			}

			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_f32(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_VALID), "initial state should be invalid.");
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_f32);
			if (p_case->check_error(err, PILO_OK, "set arr type to f32 failed")) return PILO_OK;

			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				tlv->push_back(i * 10000.f);
			}
			if (tlv->size() != 10000)
			{
				return p_case->error(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1), "f32 arr size invalid should be %d, actual %d", 10000, tlv->size());
			}

			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				if (tlv->get<::pilo::f32_t>(i, &err) != i * 10000.f)
				{
					return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "f32 arr check %d element failed should be %f", i, i * 10000);
				}
			}

			tlv->wrapper_clear(false);
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				tlv->push_front(i*10000.f);
			}
			if (tlv->size() != 10000)
			{
				return p_case->error(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1), "f32 arr size invalid should be %d, actual %d", 10000, tlv->size());
			}

			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				if (tlv->get<::pilo::f32_t>(i, &err) != 99990000.f - i * 10000.f)
				{
					return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "f32 arr check %d element failed should be %f", i, 99990000.f - i * 10000.f);
				}
			}


			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_f64(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_VALID), "initial state should be invalid.");
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_f64);
			if (p_case->check_error(err, PILO_OK, "set arr type to f64 failed")) return PILO_OK;

			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				tlv->push_back(i * 10000.f);
			}
			if (tlv->size() != 10000)
			{
				return p_case->error(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1), "f64 arr size invalid should be %d, actual %d", 10000, tlv->size());
			}

			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				if (tlv->get<::pilo::f64_t>(i, &err) != i * 10000.f)
				{
					return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "f64 arr check %d element failed should be %f", i, i * 10000);
				}
			}

			tlv->wrapper_clear(false);
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				tlv->push_front(i * 10000.f);
			}
			if (tlv->size() != 10000)
			{
				return p_case->error(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1), "f64 arr size invalid should be %d, actual %d", 10000, tlv->size());
			}

			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				if (tlv->get<::pilo::f64_t>(i, &err) != 99990000.f - i * 10000.f)
				{
					return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "f64 arr check %d element failed should be %f", i, 99990000.f - i * 10000.f);
				}
			}


			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_str(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_VALID), "initial state should be invalid.");
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_str);
			if (p_case->check_error(err, PILO_OK, "set arr type to str failed")) return PILO_OK;

			char strbuff[128] = { 0 };
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				::pilo::core::io::string_formated_output(strbuff, sizeof(strbuff), "test-str-%d", i);
				tlv->push_back(strbuff);
			}
			if (tlv->size() != 10000)
			{
				return p_case->error(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1), "f64 arr size invalid should be %d, actual %d", 10000, tlv->size());
			}
			
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				::pilo::core::io::string_formated_output(strbuff, sizeof(strbuff), "test-str-%d", i);
				std::string ele = tlv->get<std::string>(i, &err);
				if (memcmp(ele.c_str(), strbuff, ::pilo::core::string::character_count(strbuff) + 1) != 0)
				{
					return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "str arr check %d element failed get %s should be %s", i, ele.c_str(), strbuff);
				}

				tlv->push_back(strbuff);
			}

			tlv->wrapper_clear(false);
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				memset(strbuff, i % 255, 128);
				tlv->push_front(strbuff, 128);
			}
			if (tlv->size() != 10000)
			{
				return p_case->error(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1), "f64 arr size invalid should be %d, actual %d", 10000, tlv->size());
			}
			int v = 0;
			for (::pilo::i32_t i = 9999; i >= 0; i--)
			{
				std::string ele = tlv->get<std::string>(v++, &err);
				memset(strbuff, i % 255, 128);
				
				if (memcmp(ele.c_str(), strbuff, 128) != 0)
				{
					return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "str arr check %d element failed get %s should be %s", i, ele.c_str(), strbuff);
				}
			}

			tlv->wrapper_clear(false);

			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_array_bytes(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_VALID), "initial state should be invalid.");
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_bytes);
			if (p_case->check_error(err, PILO_OK, "set arr type to str failed")) return PILO_OK;

			char strbuff[128] = { 0 };
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				::pilo::core::io::string_formated_output(strbuff, sizeof(strbuff), "test-str-%d", i);
				tlv->push_front(strbuff);
			}
			if (tlv->size() != 10000)
			{
				return p_case->error(::pilo::make_core_error(PES_OP, PEP_VDT_FAILED, 1), "bytes arr size invalid should be %d, actual %d", 10000, tlv->size());
			}

			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				::pilo::core::io::string_formated_output(strbuff, sizeof(strbuff), "test-str-%d", 9999-i);
				char* ele = tlv->get<char*>(i, &err);
				if (memcmp(ele, strbuff, ::pilo::core::string::character_count(strbuff) + 1) != 0)
				{
					return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "bytes arr check %d element failed get %s should be %s", i, ele, strbuff);
				}
			}

			tlv->wrapper_clear(false);
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				memset(strbuff, i % 255, 128);
				tlv->push_back(strbuff, 128);
			}
			for (::pilo::i32_t i = 0; i < 10000; i++)
			{
				char* ele = tlv->get<char*>(i, &err);
				memset(strbuff, i % 255, 128);
				if (memcmp(ele, strbuff, 128) != 0)
				{
					return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "bytes arr check %d element failed", i);
				}
			}

			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		template<typename TA_KEY, typename TV_VALUE>
		static ::pilo::err_t s_add_dict_to_array(::pilo::core::testing::func_test_case* p_case
			,std::shared_ptr<::pilo::tlv> arr_tlv, const char* desc, std::initializer_list<std::pair<TA_KEY, TV_VALUE>> list)
		{
			::pilo::err_t err = PILO_OK;
			auto dict_str_tlv = ::pilo::tlv::allocate();
			err = dict_str_tlv->init_by_dict(list);
			if (p_case->check_error(err, PILO_OK, desc))
			{
				return err;
			}
			err = arr_tlv->push_back(dict_str_tlv);
			if (err != PILO_OK)
			{
				p_case->set_desc(desc);
			}
			return err;
		}

		int case_tlv_dict(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			std::shared_ptr<::pilo::tlv> t(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			err = t->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
			if (p_case->check_error(err, PILO_OK, "set_arr_tyype failed"))
			{
				return 0;
			}

			err = s_add_dict_to_array<::pilo::i16_t, bool>(p_case, t, "init i16,bool dict",
				{ std::pair<::pilo::i16_t, bool>((::pilo::i16_t)-32768, false)
				, std::pair<::pilo::i16_t, bool>((::pilo::i16_t)0, true)
				, std::pair<::pilo::i16_t, bool>((::pilo::i16_t)99, false)
				, std::pair<::pilo::i16_t, bool>((::pilo::i16_t)-1000, true)
				, std::pair<::pilo::i16_t, bool>((::pilo::i16_t)32767, false) });
			if (err != PILO_OK)
			{
				return PILO_OK;
			}

			err = s_add_dict_to_array<::pilo::i32_t, std::string>(p_case, t, "init i32,str dict",
				{ std::pair<::pilo::i32_t, std::string>((::pilo::i32_t)PILO_INT32_MIN, std::string("fox0"))
				, std::pair<::pilo::i32_t, std::string>((::pilo::i32_t)0, std::string("fox1"))
				, std::pair<::pilo::i32_t, std::string>((::pilo::i32_t)99, std::string("fox2"))
				, std::pair<::pilo::i32_t, std::string>((::pilo::i32_t)-1000, std::string("fox3"))
				, std::pair<::pilo::i32_t, std::string>((::pilo::i32_t)PILO_INT32_MAX, std::string("")) });
			if (err != PILO_OK)
			{
				return PILO_OK;
			}

			err = s_add_dict_to_array<::pilo::i32_t, ::pilo::f64_t>(p_case, t, "init i32,f64 dict",
				{ std::pair<::pilo::i32_t, ::pilo::f64_t>((::pilo::i32_t)PILO_INT32_MIN, 1.23456)
				, std::pair<::pilo::i32_t, ::pilo::f64_t>((::pilo::i32_t)0, 0.0)
				, std::pair<::pilo::i32_t, ::pilo::f64_t>((::pilo::i32_t)99, 3.14)
				, std::pair<::pilo::i32_t, ::pilo::f64_t>((::pilo::i32_t)-1000,2.71)
				, std::pair<::pilo::i32_t, ::pilo::f64_t>((::pilo::i32_t)PILO_INT32_MAX, 0.0) });
			if (err != PILO_OK)
			{
				return PILO_OK;
			}

			//3
			err = s_add_dict_to_array<std::string, const char*>(p_case, t, "init str,char* dict", 
				{ std::pair<std::string, const char*>(std::string("name"), "kaoru")
				, std::pair<std::string, const char*>(std::string("career"), "worker")
				, std::pair<std::string, const char*>(std::string("loc"), "los angles")
				, std::pair<std::string, const char*>(std::string("nation"), "m78hikari")
				, std::pair<std::string, const char*>(std::string("tel"), "1111114534")});
			if (err != PILO_OK)
			{
				return PILO_OK;
			}

			//4
			err = s_add_dict_to_array<std::string, ::pilo::tlv*>(p_case, t, "init str,tlv dict",
				{ std::pair<std::string, ::pilo::tlv*>(std::string("cs_tlv"), ::pilo::tlv::allocate_single("tlv0", 4, true, false))
				, std::pair<std::string, ::pilo::tlv*>(std::string("f_tlv"), ::pilo::tlv::allocate_single(3.14f))
				, std::pair<std::string, ::pilo::tlv*>(std::string("d_tlv"), ::pilo::tlv::allocate_single(3.14))
				, std::pair<std::string, ::pilo::tlv*>(std::string("s_tlv"), ::pilo::tlv::allocate_single(std::string("hi, there")))
				, std::pair<std::string, ::pilo::tlv*>(std::string("b_tlv"), nullptr) });
			if (err != PILO_OK)
			{
				return PILO_OK;
			}
			::pilo::tlv* t12 = t->get<::pilo::tlv*>(4, &err);
			err = t12->insert<std::string, ::pilo::tlv*>(std::string("b_tlv2"), ::pilo::tlv::allocate_single(true), false);
			if (p_case->check_error(err, PILO_OK, "insert to dict failed"))
			{
				return PILO_OK;
			}

			err = t->get<::pilo::tlv*>(4, &err)->erase<std::string, ::pilo::tlv*>(std::string("cs_tlv"));
			if (p_case->check_error(err, PILO_OK, "erase to dict failed"))
			{
				return PILO_OK;
			}

			err = t->get<::pilo::tlv*>(2, &err)->erase<::pilo::i32_t, ::pilo::f64_t>(99);
			if (p_case->check_error(err, PILO_OK, "erase to dict failed"))
			{
				return PILO_OK;
			}

			::pilo::core::memory::linked_byte_buffer<4096, 1024, false> lbb;

			err = t->serialize(nullptr, &lbb);
			if (err != PILO_OK)
			{
				return p_case->error(err, "serialize to lbb failed.");
			}
			auto des_tlv = ::pilo::tlv::allocate_unique();
			err = des_tlv->deserialize(nullptr, &lbb);
			if (err != PILO_OK)
			{
				return p_case->error(err, "serialize to lbb failed.");
			}
			if (!t->equals_to(des_tlv.get()))
			{
				return p_case->error(err, "validate des and orig tlv failed.");
			}
			auto cloned_tlv = des_tlv->clone_unique();
			if (!t->equals_to(cloned_tlv.get()))
			{
				return p_case->error(err, "validate des and orig tlv failed.");
			}

			//::pilo::tlv* t12 = t->get<::pilo::tlv*>(4, &err)->get<std::string, ::pilo::tlv*>(std::string("d_tlv"), &err);
			//printf(t12->to_string().c_str());

			p_case->set_result(PILO_OK);
			return 0;
		}

		int case_tlv_path_set(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			auto tlvp0 = ::pilo::tlv::allocate_shared();
			::pilo::tlv * t = tlvp0->set("character.attr.borned[0]", (::pilo::u8_t) 30,  err);
			if (t == nullptr || err != PILO_OK)
			{
				return p_case->error(err, "set character.attr.borned[0] failed");
			}

			::pilo::u8_t u8v = 0;
			tlvp0->get("character.attr.borned[0]", u8v);
			if (u8v != 30)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "character.attr.borned[0] should be 30");
			}
			::pilo::i16_t i16v = 0;
			t = tlvp0->set("character.attr.borned[]", (::pilo::i16_t)-32768, err);
			tlvp0->get("character.attr.borned[1]", i16v);
			if (i16v != -32768)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "character.attr.borned[1] should be 30");
			}
			::pilo::i32_t i32v = 0;
			t = tlvp0->set("character.attr.borned[2]", (::pilo::i32_t)PILO_INT32_MAX, err);
			tlvp0->get("character.attr.borned[2]", i32v);
			if (i32v != PILO_INT32_MAX)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "character.attr.borned[2] should be 30");
			}
			::pilo::i64_t i64v = 0;
			t = tlvp0->set("character.attr.borned[3]", (::pilo::i64_t)PILO_INT64_MAX, err);
			tlvp0->get("character.attr.borned[3]", i64v);
			if (i64v != PILO_INT64_MAX)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "character.attr.borned[3] should be PILO_INT64_MAX");
			}
			::pilo::f64_t f64v = 0;
			t = tlvp0->set("character.attr.borned[4]", (::pilo::f64_t)3.14, err);
			tlvp0->get("character.attr.borned[4]", f64v);
			if (f64v != 3.14)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "character.attr.borned[4] should be 3.14");
			}
			::pilo::f32_t f32v = 0;
			t = tlvp0->set("character.attr.borned[5]", (::pilo::f32_t)2.71f, err);
			tlvp0->get("character.attr.borned[5]", f32v);
			if (f32v != 2.71f)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "character.attr.borned[5] should be 3.14");
			}
			char* charv = nullptr;
			::pilo::i32_t rlen = 0;
			t = tlvp0->set("character.attr.borned[6]","hello this is fox", (::pilo::i32_t)::pilo::core::string::character_count("hello this is fox") + 1, false, err);
			tlvp0->get("character.attr.borned[6]", charv, &rlen);
			if (::pilo::core::string::strict_compare(charv, 0, "hello this is fox", 0, -1) != 0 )
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "character.attr.borned[6] should be 3.14");
			}

			std::string str;
			t = tlvp0->set("character.attr.borned[7]", std::string("0123456789"), err);
			tlvp0->get("character.attr.borned[7]", str);
			if (::pilo::core::string::strict_compare(str.c_str(), 0, "0123456789", 0, 10) != 0)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "character.attr.borned[7]");
			}

			std::deque<::pilo::i32_t> passive_deq{0,1,2,3,4,5,6,7,8,9};
			t = tlvp0->set("character.attr.borned[7]", passive_deq, err);
			if (t != nullptr || err == PILO_OK)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "character.attr.borned[7] exist should not be set");
			}
			t = tlvp0->set("character.attr.borned[8]", passive_deq, err);
			std::deque<::pilo::i32_t> passive_deq_ret;
			tlvp0->get("character.attr.borned[8]", passive_deq_ret);

			std::map<std::string, ::pilo::i64_t> t_map;
			for (::pilo::i64_t j = 0; j < 1000; j++)
			{
				char tbuf[32] = { 0 };
				::pilo::core::io::string_formated_output(tbuf, 32, "key_%lld",j);
				std::string key(tbuf);
				t_map[key] = j * 10000;
			}
			t = tlvp0->set("character.attr.borned[9]", t_map, err);


			p_case->set_result(PILO_OK);
			return 0;
		}

		int case_tlv_array_tlv(::pilo::core::testing::func_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_VALID), "initial state should be invalid.");
			}

			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
			if (p_case->check_error(err, PILO_OK, "set arr type to str failed")) return PILO_OK;

			err = tlv->push_back("0123456789", 5);
			p_case->check_error(err, PILO_OK, "push_back value %s failed.", "0123456789");

			err = tlv->push_front((::pilo::i32_t) -9999);
			p_case->check_error(err, PILO_OK, "push_back value -9999.");

			err = tlv->push_back(std::string("nihao1"));
			p_case->check_error(err, PILO_OK, "push_back value %s failed.", "nihao1");

			err = tlv->insert(2, std::string("nihao2"));
			p_case->check_error(err, PILO_OK, "push_back value %s failed.", "nihao1");


			::pilo::tlv* rtlv = tlv->get<::pilo::tlv*>(0, &err);
			::pilo::i32_t iv_9999 = rtlv->as_i32(&err);
			if (iv_9999 != -9999)
			{
				return p_case->error(err, "tlv arr iv_9999 should be -9999, but got %d", iv_9999);
			}
			rtlv = tlv->get<::pilo::tlv*>(1, &err);
			if (rtlv->value_type() != ::pilo::core::rtti::wired_type::value_type_bytes)
			{
				return p_case->error(err, "elem 1 type invalid got %d expect %d", rtlv->value_type(),::pilo::core::rtti::wired_type::value_type_bytes);
			}
			::pilo::i32_t rlen = 0;
			const char* bytes_v_1 = rtlv->as_bytes(nullptr, 0, &err, &rlen);
			if (p_case->check_error(err, PILO_OK, "as_bytes failed")) return PILO_OK;
			if (bytes_v_1 == nullptr || rlen != 5 || ::pilo::core::string::strict_compare(bytes_v_1,0, "01234",0, 5) != 0)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "elem 1 value failed");
			}
			rtlv = tlv->get<::pilo::tlv*>(2, &err);
			if (rtlv->value_type() != ::pilo::core::rtti::wired_type::value_type_str)
			{
				return p_case->error(err, "elem 2 type invalid got %d expect %d", rtlv->value_type(), ::pilo::core::rtti::wired_type::value_type_str);
			}
			std::string str_v_2 = rtlv->as_str(&err, nullptr);
			if (::pilo::core::string::strict_compare(str_v_2.c_str(), 0, "nihao2",0, 7) != 0)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "elem 1 value failed");
			}

			::pilo::tlv* bool_arr_tlv = tlv->push_back_array_element<bool>({ false, true, false, true }, &err);		
			if (p_case->check_error(err, PILO_OK, "push_back_array_element")) return PILO_OK;
			if (bool_arr_tlv == nullptr)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "elem bool_arr_tlv value failed");
			}
			bool_arr_tlv->push_back(false);

			tlv->push_back_array_element<std::string>({"test0","test1", "test2", "test3"}, &err);
			if (p_case->check_error(err, PILO_OK, "push_back_array_element of str")) return PILO_OK;

			tlv->push_back_array_element<const char *>({ "test0","test1", "test2", "test3" }, &err);
			if (p_case->check_error(err, PILO_OK, "push_back_array_element of bytes")) return PILO_OK;			
			tlv->push_back_tlv("0123456789", 10, false, false, &err);
			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_clone(::pilo::core::testing::func_test_case* p_case)
		{

			::pilo::err_t err = PILO_OK;
			std::shared_ptr<::pilo::tlv> tlv(::pilo::tlv::allocate(),
				[](::pilo::tlv* x) {
					::pilo::tlv::deallocate(x);
				});

			if (tlv->valid())
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_VALID), "initial state should be invalid.");
			}
			
			err = tlv->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
			if (p_case->check_error(err, PILO_OK, "set arr type to str failed")) return PILO_OK;
			//0
			err = tlv->push_back("0123456789", 5);
			p_case->check_error(err, PILO_OK, "push_back value %s failed.", "0123456789");
			//1
			err = tlv->push_back((::pilo::i32_t)-9999);
			p_case->check_error(err, PILO_OK, "push_back value -9999.");
			//2
			err = tlv->push_back(std::string("nihao1"));
			p_case->check_error(err, PILO_OK, "push_back value %s failed.", "nihao1");
			//3
			err = tlv->push_back(std::string("nihao2"));
			p_case->check_error(err, PILO_OK, "push_back value %s failed.", "nihao1");
			//4			
			::pilo::tlv* bool_arr_tlv = tlv->push_back_array_element<bool>({ false, true, false, true }, &err);
			if (p_case->check_error(err, PILO_OK, "push_back_array_element")) return PILO_OK;
			if (bool_arr_tlv == nullptr)
			{
				return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "elem bool_arr_tlv value failed");
			}
			bool_arr_tlv->push_back(false);
			//5
			tlv->push_back_array_element<std::string>({ "test0","test1", "test2", "test3" }, &err);
			if (p_case->check_error(err, PILO_OK, "push_back_array_element of str")) return PILO_OK;
			//6
			tlv->push_back_array_element<const char*>({ "test0","test1", "test2", "test3" }, &err);
			if (p_case->check_error(err, PILO_OK, "push_back_array_element of bytes")) return PILO_OK;
			//7
			tlv->push_back_tlv("0123456789", 10, false, false, &err);
			//8
			tlv->push_back(3.14);

			auto saved_tlv = tlv->clone_shared();
			if (saved_tlv == nullptr)
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_CLONE_FAILED), "clone failed");
			}

			std::string jsonstr0 = saved_tlv->to_string();
			std::string jsonstr1 = saved_tlv->to_string();
			if (jsonstr0 != jsonstr1)
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_VDT_FAILED), "j clone validation failed.");
			}

			if (! saved_tlv->equals_to(tlv.get()))
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_VDT_FAILED), "clone validation failed.");
			}


			p_case->set_result(PILO_OK);
			return PILO_OK;
		}
	
	}

	
}