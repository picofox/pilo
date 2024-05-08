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
					return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_NULL), "%s create orig tlv failed", name);
				}

				clone_tlvs[i] = orig_tlv->clone();
				if (orig_tlv == nullptr)
				{
					return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_NULL), "%s clone orig tlv failed", name);
				}

				if (! orig_tlv->equals_to(clone_tlvs[i]))
				{
					return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_NULL), "%s validate cloned tlv failed", name);
				}

				err = clone_tlvs[i]->serialize(nullptr, &bb);
				if (err != PILO_OK)
				{
					return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_NULL), "%s serialize tlv failed", name);
				}			

				::pilo::tlv::deallocate(orig_tlv);				
			}


			for (::pilo::i64_t i = 0; i < inner_cnt; i++)
			{
				::pilo::tlv* des_tlv = ::pilo::tlv::allocate();
				err = des_tlv->deserialize(nullptr, &bb);
				if (err != PILO_OK)
				{
					return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_NULL), "%s serialize tlv failed", name);
				}

				::pilo::tlv::deallocate(des_tlv);
				::pilo::tlv::deallocate(clone_tlvs[i]);
				p_case->inc_print_progress();

			}

			if (bb.read_available() != 0)
			{
				return p_case->error(::pilo::make_core_error(PES_TLV, PEP_IS_NULL), "%s buffer remain %lld", name, bb.read_available());
			}

			return PILO_OK;
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

