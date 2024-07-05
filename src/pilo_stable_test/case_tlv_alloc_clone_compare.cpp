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
		template<typename T>
		static ::pilo::err_t _case_array(std::initializer_list<T> list, ::pilo::core::testing::stable_test_case* p_case
			,::pilo::core::memory::linked_byte_buffer<4096, 1024, false> &bb
			, ::pilo::tlv* t
			, ::pilo::tlv* der_tlv
			,::pilo::i64_t loops, ::pilo::u16_t vt)
		{
			::pilo::err_t err = PILO_OK;
			for (::pilo::i64_t i = 0; i < loops; i++)
			{
				t->reset();
				der_tlv->reset();

				t->set_array_type(vt);
				err = t->serialize(nullptr, &bb);
				if (p_case->check_error(err, PILO_OK, "serialize %d arr failed", vt) != 0) return PILO_OK;
				err = der_tlv->deserialize(nullptr, &bb);
				if (p_case->check_error(err, PILO_OK, "derserialize %d arr failed", vt) != 0) return PILO_OK;
				if (!der_tlv->equals_to(t))
				{
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "validate empty %d arr failed",vt);
				}
				t->reset();
				der_tlv->reset();

				if (bb.read_available() != 0)
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "deserial %d arr err, remain data in buffer", vt);

				err = t->init_by_array(list);
				if (p_case->check_error(err, PILO_OK, "init %d arr failed", vt) != 0) return PILO_OK;
				err = t->serialize(nullptr, &bb);
				if (p_case->check_error(err, PILO_OK, "serialize %d arr failed", vt) != 0) return PILO_OK;
				err = der_tlv->deserialize(nullptr, &bb);
				if (p_case->check_error(err, PILO_OK, "derserialize %d arr failed", vt) != 0) return PILO_OK;

				if (!der_tlv->equals_to(t))
				{
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "validate %d arr failed", vt);
				}

				if (bb.read_available() != 0)
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "deserial %d arr err, remain data in buffer", vt);

			}
			return PILO_OK;
		}

		static ::pilo::err_t _case_array_tlv(::pilo::i32_t tlv_cnt, ::pilo::core::testing::stable_test_case* p_case
			, ::pilo::core::memory::linked_byte_buffer<4096, 1024, false>& bb
			, ::pilo::tlv* t
			, ::pilo::tlv* der_tlv
			, ::pilo::i64_t loops)
		{
			::pilo::err_t err = PILO_OK;
			for (::pilo::i64_t i = 0; i < loops; i++)
			{
				t->reset();
				der_tlv->reset();

				t->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
				err = t->serialize(nullptr, &bb);
				if (p_case->check_error(err, PILO_OK, "serialize tlv failed")) return PILO_OK;
				err = der_tlv->deserialize(nullptr, &bb);
				if (p_case->check_error(err, PILO_OK, "derserialize  arr failed")) return PILO_OK;
				if (!der_tlv->equals_to(t))
				{
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "validate empty tlv arr failed");
				}
				t->reset();
				der_tlv->reset();

				if (bb.read_available() != 0)
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "deserial tlv arr err, remain data in buffer");
				t->set_array_type(::pilo::core::rtti::wired_type::value_type_tlv);
				for (::pilo::i32_t j = 0; j < tlv_cnt; j++)
				{
					::pilo::tlv * elem = create_random_single_tlv();
					err = t->push_back<::pilo::tlv*>(elem, -1, true);
					if (p_case->check_error(err, PILO_OK, "create random single failed for tlv arr."))
						return PILO_OK;
				}

				//printf("data (%s)\n", t->to_string().c_str());

				err = t->serialize(nullptr, &bb);				
				if (p_case->check_error(err, PILO_OK, "serialize tlv arr failed")) return PILO_OK;
				err = der_tlv->deserialize(nullptr, &bb);
				if (p_case->check_error(err, PILO_OK, "derserialize tlv arr failed") != 0) return PILO_OK;

				if (!der_tlv->equals_to(t))
				{
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "validate tlv arr failed");
				}

				if (bb.read_available() != 0)
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "deserial tlv arr err, remain data in buffer");

			}
			return PILO_OK;
		}

		int case_tlv_acc_array(::pilo::core::testing::stable_test_case* p_case)
		{
			::pilo::core::memory::linked_byte_buffer<4096, 1024, false> bb;
			auto t = ::pilo::tlv::allocate_unique();
			auto der_tlv = ::pilo::tlv::allocate_unique();
			::pilo::err_t err;

			err = _case_array_tlv(3, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count());
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<::pilo::u8_t>({1,2,0,127,255}, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_u8);
			if (err != PILO_OK) return PILO_OK;			

			err = _case_array<::pilo::i8_t>({-128,1,2,3,4,0,127}, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_i8);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<::pilo::i16_t>({PILO_INT16_MIN, 1,2,3,4,6,PILO_INT16_MAX},p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_i16);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<::pilo::u16_t>({ 0, 1,2,3,4,6,PILO_UINT16_MAX }, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_u16);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<::pilo::i32_t>({ PILO_INT32_MIN, 1,2,3,4,6,PILO_INT32_MAX },p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_i32);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<::pilo::u32_t>({ 0, 1,2,3,4,6,PILO_UINT32_MAX }, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_u32);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<::pilo::i64_t>({ PILO_INT64_MIN, 1,2,3,4,6,PILO_INT64_MAX }, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_i64);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<::pilo::u64_t>({ 0, 1,2,3,4,6,PILO_UINT64_MAX }, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_u64);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<::pilo::f32_t>({ 1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f }, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_f32);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<::pilo::f64_t>({ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 }, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_f64);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<const char*>({ "fox1", "fox2", "fox3", "fox4", "fox5", "fox6", "fox7" }, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_bytes);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<std::string>({ "fox1", "fox2", "fox3", "fox4", "fox5", "fox6", "fox7"}, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_str);
			if (err != PILO_OK) return PILO_OK;

			err = _case_array<const char*>({ "fox1", "fox2", "fox3", "fox4", "fox5", "fox6", "fox7" }, p_case, bb, t.get(), der_tlv.get(), p_case->target_trans_count(), ::pilo::core::rtti::wired_type::value_type_bytes);
			if (err != PILO_OK) return PILO_OK;



			p_case->set_result(PILO_OK);
			return PILO_OK;

		}

		int case_tlv_acc_dict(::pilo::core::testing::stable_test_case* p_case)
		{
			srand(0);
			for (int i = 0; i < p_case->target_trans_count(); i++)
			{
				::pilo::tlv* t = create_random_dict_tlv(::pilo::core::rtti::wired_type::key_type_na, ::pilo::core::rtti::wired_type::value_type_na, 3);
				if (t == nullptr)
				{
					return p_case->error(::pilo::mk_perr(PERR_NULL_PTR), "create dict tlv failed");
				}
				::pilo::tlv* cloned_t = t->clone();
				if (cloned_t == nullptr)
				{
					return p_case->error(::pilo::mk_perr(PERR_NULL_PTR), "clone dict tlv failed");
				}

				if (!t->equals_to(cloned_t))
				{
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "orig tlv and cloned tlv not equals");
				}

				p_case->inc_print_progress();

				::pilo::tlv::deallocate(cloned_t);
				::pilo::tlv::deallocate(t);
			}
			p_case->set_result(PILO_OK);
			return PILO_OK;
		}

		int case_tlv_acc_single(::pilo::core::testing::stable_test_case* p_case)
		{
			srand(0);
			::pilo::core::memory::linked_byte_buffer<4096, 1024, false> bb;
			for (int i = 0; i < p_case->target_trans_count(); i++)
			{
				::pilo::tlv* t = create_random_single_tlv();
				if (t == nullptr)
				{
					p_case->set_result(::pilo::mk_perr(PERR_NULL_PTR));
					return PILO_OK;
				}

				::pilo::tlv* copied_t = t->clone();
				if (t == nullptr)
				{
					p_case->set_result(::pilo::mk_perr(PERR_NULL_PTR));
					return PILO_OK;
				}

				if (!t->equals_to(copied_t))
				{
					p_case->set_result(::pilo::mk_perr(PERR_TESTCASE_FAIL));
					return PILO_OK;
				}		

				::pilo::tlv* der_t = ::pilo::tlv::allocate();
				if (t->serialize(nullptr, &bb) != PILO_OK)
				{
					p_case->set_result(::pilo::mk_perr(PERR_TESTCASE_FAIL));
					return PILO_OK;
				}
				if (der_t->deserialize(nullptr, &bb) != PILO_OK)
				{
					p_case->set_result(::pilo::mk_perr(PERR_TESTCASE_FAIL));
					return PILO_OK;
				}
				if (!t->equals_to(der_t))
				{
					p_case->set_result(::pilo::mk_perr(PERR_TESTCASE_FAIL));
					return PILO_OK;
				}
				
				p_case->inc_print_progress();

				::pilo::tlv::deallocate(der_t);
				::pilo::tlv::deallocate(copied_t);
				::pilo::tlv::deallocate(t);
			}

			p_case->set_result(PILO_OK);
			return PILO_OK;
		}



		int case_tlv_acc(::pilo::core::testing::stable_test_case* p_case)
		{
			srand(0);
			::pilo::core::memory::linked_byte_buffer<4096, 1024, false> bb;
			for (int i = 0; i < p_case->target_trans_count(); i++)
			{
				::pilo::tlv* t = create_random_array_tlv(4);
				if (t == nullptr)
				{
					p_case->set_result(::pilo::mk_perr(PERR_NULL_PTR));
					return PILO_OK;
				}

			//	printf("\n%s\n",t->to_string().c_str());

				::pilo::tlv* copied_t = t->clone();
				if (t == nullptr)
				{
					p_case->set_result(::pilo::mk_perr(PERR_NULL_PTR));
					return PILO_OK;
				}

				if (! t->equals_to(copied_t))
				{
					p_case->set_result(::pilo::mk_perr(PERR_TESTCASE_FAIL));
					return PILO_OK;
				}

				//printf("\n%s\n",t->to_string().c_str());
				::pilo::err_t err = t->serialize(nullptr, &bb);
				if (p_case->check_error(err, PILO_OK, "serlize %d failed", i))
					return PILO_OK;

				auto der_tlv = pilo::tlv::allocate_unique();
				err = der_tlv->deserialize(nullptr, &bb);
				if (p_case->check_error(err, PILO_OK, "deserlize %d failed", i))
					return PILO_OK;

				if (bb.read_available() != 0)
					return p_case->error(::pilo::mk_perr(PERR_TESTCASE_FAIL), "deserial tlv arr err, remain data in buffer");

				p_case->inc_print_progress();
				::pilo::tlv::deallocate(copied_t);
				::pilo::tlv::deallocate(t);
			}
			
  			p_case->set_result(PILO_OK);
			return PILO_OK;
		}
	}
}

