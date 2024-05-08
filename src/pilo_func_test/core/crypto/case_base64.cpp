#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/memory/fixed_byte_buffer.hpp"
#include "pilo/core/crypto/base64_codec.hpp"
#include <memory>
#include <iostream>
#include "pilo/core/pattern/event_manager.hpp"
#include "pilo/core/testing/case_utils.hpp"


using namespace ::pilo::func_test;

namespace pilo
{
	namespace func_test
	{

		int case_base64_static(::pilo::core::testing::func_test_case* p_case)
		{
			

			const char* src = "hi, i am king of rock";
			::pilo::i64_t src_len = ::pilo::core::string::character_count(src);
			char buffer_enough[128] = { 0 };
			::pilo::i64_t rlen = 0;
			::pilo::i64_t slen = 0;
			::pilo::err_t err = PILO_OK;
			char * ret = ::pilo::core::crypto::base64_codec::s_encode(buffer_enough, 128, rlen, src, src_len, &err, nullptr, 0);
			if (ret == nullptr)
			{
				return p_case->error(err, "encode str (%s) failed", src);
			}
			ret = ::pilo::core::crypto::base64_codec::s_decode(buffer_enough, 128, slen, ret, rlen, &err, nullptr, 0);
			if (ret == nullptr)
			{
				return p_case->error(err, "encode str (%s) failed", src);
			}

			std::string srcstr;
			char* encstr = nullptr;
			char* decstr = nullptr;
			for (int i = 0; i < 1000; i++)
			{
				::pilo::core::testing::create_random_memory_sample(srcstr,0, 32777);
				encstr = ::pilo::core::crypto::base64_codec::s_encode(nullptr, 0, rlen, srcstr.c_str(), (::pilo::i64_t) srcstr.size(), &err, nullptr, 0);
				if (ret == nullptr)
				{
					return p_case->error(err, "encode str (%d) failed", srcstr.size());
				}
				decstr = ::pilo::core::crypto::base64_codec::s_decode(nullptr, 0, slen, encstr, rlen, &err, nullptr, 0);
				PMF_HEAP_FREE(encstr);
				if (decstr == nullptr)
				{
					return p_case->error(err, "decode str (%d) failed", rlen);
				}
				if (p_case->check_error(err, PILO_OK, "decode str failed")) 
				{
					PMF_HEAP_FREE(decstr);
					return PILO_OK;
				}
				
				if ((::pilo::i64_t)srcstr.size() != slen)
				{
					PMF_HEAP_FREE(decstr);
					return p_case->error(err, "size validate failed at %d get %d expect %d", i, slen ,srcstr.size() );
				}
				if (memcmp(srcstr.c_str(), decstr, slen) != 0)
				{
					return p_case->error(err, "content validate failed at %d", i);
				}

				PMF_HEAP_FREE(decstr);
				
			}



			p_case->set_result(PILO_OK);
			return PILO_OK;
		}
	}
}