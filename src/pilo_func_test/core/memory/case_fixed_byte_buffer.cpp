#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/memory/fixed_byte_buffer.hpp"
#include "pilo/core/threading/spin_mutex.hpp"
#include "pilo/core/threading/auto_join_thread.hpp"
#include "pilo/core/testing/large_sample_object.hpp"
#include "pilo/core/memory/o1l31c16_header.hpp"
#include "pilo/core/datetime/stop_watch.hpp"

using namespace ::pilo::func_test;

#define PILO_FBB_TEST_CNT_0 (100000)

namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace memory
			{
				
				int case_fixed_byte_buffer_serialize_net(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::core::memory::fixed_byte_buffer<1700* PILO_FBB_TEST_CNT_0, true> buffer;
					::pilo::core::testing::large_sample_object sobj(false, 0);
					::pilo::core::memory::o1l31c16_header header;
					::pilo::i64_t ser_time = 0;
					for (::pilo::i64_t i = 0; i < PILO_FBB_TEST_CNT_0; i++)
					{
						::pilo::err_t ret = PILO_OK;
						sobj.set();
						::pilo::i64_t b = ::pilo::core::datetime::timestamp_nano_steady();
						ret = sobj.serialize(&header, &buffer);
 						ser_time += (::pilo::core::datetime::timestamp_nano_steady() - b);
						if (ret != PILO_OK)
						{
							p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
							return PILO_OK;
						}
						
					}

					for (::pilo::i64_t i = 0; i < PILO_FBB_TEST_CNT_0; i++)
					{
						::pilo::err_t ret = ::pilo::core::memory::o1l31c16_header::deserialise(header, &buffer);
						::pilo::i32_t ok = ::pilo::is_ok_perr(ret, PERR_RETRY);

						::pilo::core::testing::large_sample_object* obj = nullptr;
						if (ok == 0)
						{
							obj = ::pilo::core::testing::large_sample_object::deserialise(&header, &buffer);
							if (obj == nullptr)
							{
								p_case->set_result(::pilo::mk_perr(PERR_INV_OBJECT));
								return PILO_OK;
							}
							if (!obj->validate())
							{
								p_case->set_result(::pilo::mk_perr(PERR_INV_OBJECT));
								return PILO_OK;
							}
							delete(obj);
							obj = nullptr;
						}
						else if (ok < 0)
						{
							p_case->set_result(ret);
							return PILO_OK;
						}
					}

					p_case->set_result(PILO_OK);
					char desc_buffer[128] = { 0 };
					::pilo::core::io::string_formated_output(desc_buffer, 128, "cnt %lld time %f (%s)", PILO_FBB_TEST_CNT_0
						, ((double)ser_time) / 1000000000.0f
						, buffer.to_string().c_str()
						);
					p_case->set_desc(desc_buffer);

					return PILO_OK;
				}

				int case_fixed_byte_buffer_serialize_host(::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::core::memory::fixed_byte_buffer< 4096* PILO_FBB_TEST_CNT_0, false> buffer;
					::pilo::core::testing::large_sample_object sobj(false, 0);
					::pilo::core::memory::o1l31c16_header header;
					::pilo::i64_t ser_time = 0;
					for (::pilo::i64_t i = 0; i < PILO_FBB_TEST_CNT_0; i++)
					{
						::pilo::err_t ret = PILO_OK;
						sobj.set();
						::pilo::i64_t b = ::pilo::core::datetime::timestamp_nano_steady();
						ret = sobj.serialize(&header, &buffer);
						ser_time += (::pilo::core::datetime::timestamp_nano_steady() - b);
						if (ret != PILO_OK)
						{
							p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
							return PILO_OK;
						}

					}

					for (::pilo::i64_t i = 0; i < PILO_FBB_TEST_CNT_0; i++)
					{
						::pilo::err_t ret = ::pilo::core::memory::o1l31c16_header::deserialise(header, &buffer);
						::pilo::i32_t ok = ::pilo::is_ok_perr(ret, PERR_RETRY);
						::pilo::core::testing::large_sample_object* obj = nullptr;
						if (ok == 0)
						{
							obj = ::pilo::core::testing::large_sample_object::deserialise(&header, &buffer);
							if (obj == nullptr)
							{
								p_case->set_result(::pilo::mk_perr(PERR_INV_OBJECT));
								return PILO_OK;
							}
							if (!obj->validate())
							{
								p_case->set_result(::pilo::mk_perr(PERR_INV_OBJECT));
								return PILO_OK;
							}
							delete(obj);
							obj = nullptr;
						}
						else if (ok < 0)
						{
							p_case->set_result(ret);
							return PILO_OK;
						}
					}

					p_case->set_result(PILO_OK);
					char desc_buffer[128] = { 0 };
					::pilo::core::io::string_formated_output(desc_buffer, 128, "cnt %lld time %f ", PILO_FBB_TEST_CNT_0
						, ((double)ser_time) / 1000000000.0f
					);
					p_case->set_desc(desc_buffer);

					return PILO_OK;
				}				
			}
		}
	}
}