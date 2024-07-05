#include <thread>
#include "pilo_stable_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/tlv.hpp"
#include "pilo/core/testing/case_utils.hpp"
#include "pilo/core/memory/linked_byte_buffer.hpp"

#include <mutex>


using namespace ::pilo::stable_test;
using namespace ::pilo::core::testing;

namespace pilo
{
	namespace stable_test
	{
		static pilo::err_t inner_test(::pilo::core::testing::stable_test_case* p_case
			, ::pilo::i64_t inner_cnt
			, ::pilo::core::memory::linked_byte_buffer<4096, 1024, false> & bb
			, const char* name)
		{
			::pilo::err_t err = 0;
			std::string str;

			::pilo::tlv* clone_tlvs[100] = { nullptr };

			for (::pilo::i64_t i = 0; i < inner_cnt; i++)
			{
				auto orig_tlv = ::pilo::core::testing::create_random_array_tlv(4);
				if (orig_tlv == nullptr)
				{
					return p_case->error(::pilo::mk_perr(PERR_NULL_PTR), "%s create orig tlv failed", name);
				}

				clone_tlvs[i] = orig_tlv->clone();
				if (orig_tlv == nullptr)
				{
					return p_case->error(::pilo::mk_perr(PERR_NULL_PTR), "%s clone orig tlv failed", name);
				}

				if (! orig_tlv->equals_to(clone_tlvs[i]))
				{
					return p_case->error(::pilo::mk_perr(PERR_NULL_PTR), "%s validate cloned tlv failed", name);
				}

				err = clone_tlvs[i]->serialize(nullptr, &bb);
				if (err != PILO_OK)
				{
					return p_case->error(::pilo::mk_perr(PERR_NULL_PTR), "%s serialize tlv failed", name);
				}			

				::pilo::tlv::deallocate(orig_tlv);				
			}


			for (::pilo::i64_t i = 0; i < inner_cnt; i++)
			{
				::pilo::tlv* des_tlv = ::pilo::tlv::allocate();
				err = des_tlv->deserialize(nullptr, &bb);
				if (err != PILO_OK)
				{
					return p_case->error(::pilo::mk_perr(PERR_NULL_PTR), "%s serialize tlv failed", name);
				}

				::pilo::tlv::deallocate(des_tlv);
				::pilo::tlv::deallocate(clone_tlvs[i]);
				p_case->inc_print_progress();

			}

			if (bb.read_available() != 0)
			{
				return p_case->error(::pilo::mk_perr(PERR_NULL_PTR), "%s buffer remain %lld", name, bb.read_available());
			}

			return PILO_OK;
		}

		static int _s_one_path_set(::pilo::core::testing::stable_test_case* p_case)
		{
			::pilo::err_t err = PILO_OK;
			auto tlvp0 = ::pilo::tlv::allocate_shared();
			::pilo::tlv* t = tlvp0->set("character.attr.borned[0]", (::pilo::u8_t)30, err);
			if (t == nullptr || err != PILO_OK)
			{
				return p_case->error(err, "set character.attr.borned[0] failed");
			}

			::pilo::u8_t u8v = 0;
			tlvp0->get("character.attr.borned[0]", u8v);
			if (u8v != 30)
			{
				return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "character.attr.borned[0] should be 30");
			}
			::pilo::i16_t i16v = 0;
			t = tlvp0->set("character.attr.borned[]", (::pilo::i16_t)-32768, err);
			tlvp0->get("character.attr.borned[1]", i16v);
			if (i16v != -32768)
			{
				return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "character.attr.borned[1] should be 30");
			}
			::pilo::i32_t i32v = 0;
			t = tlvp0->set("character.attr.borned[2]", (::pilo::i32_t)PILO_INT32_MAX, err);
			tlvp0->get("character.attr.borned[2]", i32v);
			if (i32v != PILO_INT32_MAX)
			{
				return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "character.attr.borned[2] should be 30");
			}
			::pilo::i64_t i64v = 0;
			t = tlvp0->set("character.attr.borned[3]", (::pilo::i64_t)PILO_INT64_MAX, err);
			tlvp0->get("character.attr.borned[3]", i64v);
			if (i64v != PILO_INT64_MAX)
			{
				return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "character.attr.borned[3] should be PILO_INT64_MAX");
			}
			::pilo::f64_t f64v = 0;
			t = tlvp0->set("character.attr.borned[4]", (::pilo::f64_t)3.14, err);
			tlvp0->get("character.attr.borned[4]", f64v);
			if (f64v != 3.14)
			{
				return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "character.attr.borned[4] should be 3.14");
			}
			::pilo::f32_t f32v = 0;
			t = tlvp0->set("character.attr.borned[5]", (::pilo::f32_t)2.71f, err);
			tlvp0->get("character.attr.borned[5]", f32v);
			if (f32v != 2.71f)
			{
				return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "character.attr.borned[5] should be 3.14");
			}
			char* charv = nullptr;
			::pilo::i32_t rlen = 0;
			t = tlvp0->set("character.attr.borned[6]", "hello this is fox", (::pilo::i32_t)::pilo::core::string::character_count("hello this is fox") + 1, false, err);
			tlvp0->get("character.attr.borned[6]", charv, &rlen);
			if (::pilo::core::string::strict_compare(charv, 0, "hello this is fox", 0, -1) != 0)
			{
				return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "character.attr.borned[6] should be 3.14");
			}

			std::string str;
			t = tlvp0->set("character.attr.borned[7]", std::string("0123456789"), err);
			tlvp0->get("character.attr.borned[7]", str);
			if (::pilo::core::string::strict_compare(str.c_str(), 0, "0123456789", 0, 10) != 0)
			{
				return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "character.attr.borned[7]");
			}

			std::deque<::pilo::i32_t> passive_deq{ 0,1,2,3,4,5,6,7,8,9 };
			t = tlvp0->set("character.attr.borned[7]", passive_deq, err);
			if (t != nullptr || err == PILO_OK)
			{
				return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "character.attr.borned[7] exist should not be set");
			}
			t = tlvp0->set("character.attr.borned[8]", passive_deq, err);
			std::deque<::pilo::i32_t> passive_deq_ret;
			tlvp0->get("character.attr.borned[8]", passive_deq_ret);

			std::map<std::string, ::pilo::i64_t> t_map;
			for (::pilo::i64_t j = 0; j < 1000; j++)
			{
				char tbuf[32] = { 0 };
				::pilo::core::io::string_formated_output(tbuf, 32, "key_%lld", j);
				std::string key(tbuf);
				t_map[key] = j * 10000;
			}
			t = tlvp0->set("character.attr.borned[9]", t_map, err);

			return 1;
		}

		int case_tlv_pathset(::pilo::core::testing::stable_test_case* p_case)
		{
			for (::pilo::i64_t i = 0; i < p_case->target_trans_count(); i++)
			{
				int ret = _s_one_path_set(p_case);
				if (ret != 1)
				{
					return 0;
				}
				p_case->inc_print_progress();
			}

			p_case->set_result(PILO_OK);
			return 0;
		}

		int case_tlv_serialization(::pilo::core::testing::stable_test_case *p_case)
		{
			srand(0);
			::pilo::core::memory::linked_byte_buffer<4096, 1024, false> bb;
			::pilo::i64_t inner_cnt = 100;
			::pilo::i64_t outer_cnt = p_case->target_trans_count() / 100;
			
			for (::pilo::i64_t i = 0; i < outer_cnt; i++)
			{
				::pilo::err_t err = inner_test(p_case, inner_cnt, bb, "100 ser and deser test");
				if (err != PILO_OK)
				{
					return 0;
				}
			}				

			

			p_case->set_result(PILO_OK);
			return 0;
		}
	}
}

