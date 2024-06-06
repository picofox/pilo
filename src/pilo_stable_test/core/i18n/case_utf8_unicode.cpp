#include "../../pilo_stable_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/testing/case_utils.hpp"
#include "pilo/core/i18n/encoding_conversion.hpp"

using namespace ::pilo::stable_test;
using namespace ::pilo::core::testing;


namespace pilo
{
	namespace stable_test
	{
		namespace core
		{
			namespace i18n
			{
				static const char* s_test_strs[] = {
					""
					,"a"
					,"命中率最高俱乐部"
					,"：：1979.机动战士高达.TV全系列+OVA全系列+剧场版全系列.[收藏版]、\\机动战士高达 剧场版\\2022 高达Reconguista in GⅣ 激战中呼唤的爱 (R.C. 1014)【空文件夹】"
					,nullptr
				};
				static const wchar_t* s_test_cmp_strs[] = {
					L""
					,L"a"
					,L"命中率最高俱乐部"
					,L"：：1979.机动战士高达.TV全系列+OVA全系列+剧场版全系列.[收藏版]、\\机动战士高达 剧场版\\2022 高达Reconguista in GⅣ 激战中呼唤的爱 (R.C. 1014)【空文件夹】"

					,nullptr
				};


				static ::pilo::err_t _s_one_conversion(::pilo::core::testing::stable_test_case* p_case)
				{
					wchar_t wb[1] = { 0 };
					::pilo::wchar_buffer_t wbuf(wb, 1, 0, false);
					::pilo::err_t err = PILO_OK;
					for (int i = 0; i < PMF_COUNT_OF(s_test_strs); i++)
					{
						if (s_test_strs[i] == nullptr)
							return PILO_OK;
						err = ::pilo::core::i18n::utf8_to_os_unicode(wbuf, s_test_strs[i], (::pilo::i32_t)::pilo::core::string::character_count(s_test_strs[i]), 32);
						if (err != PILO_OK)
						{
							return p_case->error(err, "conv %d-[%s] utf-char to unicode Failed", i, s_test_strs[i]);
						}

						::pilo::i32_t clen = (::pilo::i32_t) ::pilo::core::string::character_count(s_test_cmp_strs[i]);
						if (clen != wbuf.size())
						{
							return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "conv %d-[%s] utf-u16 inv-size %d %d", i, s_test_strs[i], clen , wbuf.size());
						}

						if (0 != ::pilo::core::string::strict_compare(s_test_cmp_strs[i], 0, wbuf.begin(), 0, -1))
						{
							return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "conv %d-[%s] content", i, s_test_strs[i]);
						}
						
						if (wbuf.space_available() < 32+1)
						{
							return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_VDT_FAILED), "conv %d-[%s] extra", i, s_test_strs[i]);
						}

						wbuf.clear();

					}


					return PILO_OK;
				}

				int case_utf8_unicode(::pilo::core::testing::stable_test_case* p_case)
				{
					p_case->set_begin();
					::pilo::err_t err = PILO_OK;
					for (::pilo::i64_t i = 0; i < p_case->target_trans_count(); i++)
					{
						err = _s_one_conversion(p_case);
						if (err != PILO_OK)
						{
							return p_case->error(::pilo::make_core_error(PES_TCASE, PEP_ABORTED), "abort on loop %lld", i);
						}
						p_case->inc_print_progress();
					}

					p_case->set_result(PILO_OK);
					return PILO_OK;
				}

			} //io
		} // core
	} // stable test
} // pilo