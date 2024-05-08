#include "../../pilo_func_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/memory/linked_byte_buffer.hpp"
#include "pilo/core/threading/spin_mutex.hpp"
#include "pilo/core/threading/auto_join_thread.hpp"
#include "pilo/core/testing/large_sample_object.hpp"
#include "pilo/core/memory/o1l31c16_header.hpp"
#include "pilo/core/datetime/stop_watch.hpp"

using namespace ::pilo::func_test;

#define PILO_LINKED_BYTE_BUFFER_TEST_CNT_0 (100000LL)
#define PILO_LINKED_BYTE_BUFFER_PROD_THR_CNT_0 (2)
#define PILO_LINKED_BYTE_BUFFER_CONS_THR_CNT_0 (2)

namespace pilo
{
	namespace func_test
	{
		namespace core
		{
			namespace memory
			{
				
				static std::atomic<::pilo::i64_t> stc_count;
				const static ::pilo::i64_t stc_target = PILO_LINKED_BYTE_BUFFER_PROD_THR_CNT_0 * PILO_LINKED_BYTE_BUFFER_TEST_CNT_0;



				static void producer(::pilo::core::memory::byte_buffer_interface * bb, ::pilo::core::testing::func_test_case* p_case, ::pilo::u32_t tn)
				{
					::pilo::core::testing::large_sample_object obj(false, tn);
					::pilo::core::memory::o1l31c16_header header;

				
					for (::pilo::i64_t i = 0; i < PILO_LINKED_BYTE_BUFFER_TEST_CNT_0; i++)
					{
						::pilo::err_t ret = PILO_OK;
						{
							obj.set();
							std::lock_guard<::pilo::core::threading::spin_mutex> guard(*(p_case->get_lock()));	
							ret = obj.serialize(&header, bb);							
						}						
						if (ret != PILO_OK)
						{
							p_case->set_result(::pilo::make_core_error(PES_OP, PEP_ABORTED));
							return;
						}						
					}
				}

				static void consumer(::pilo::core::memory::byte_buffer_interface* bb, ::pilo::core::testing::func_test_case* p_case)
				{
					::pilo::err_t ret = PILO_OK;
					::pilo::i32_t err_type = 0;
					::pilo::core::memory::o1l31c16_header header;
					for (;;)
					{
						err_type = ::pilo::is_ok_or_err_type(p_case->result(), PEP_RETRY);
						if (err_type <= 0)
						{
							return;
						}

						if (stc_count.load() >= stc_target)
						{
							char buffer[128] = { 0 };
							::pilo::core::io::string_formated_output(buffer, 128, "%d obj is tested. %s", stc_target, bb->to_string().c_str());
							p_case->set_desc(buffer);
							p_case->set_result(PILO_OK);
							return;
						}

						::pilo::core::testing::large_sample_object* obj = nullptr;
						{
							std::lock_guard<::pilo::core::threading::spin_mutex> guard(*(p_case->get_lock()));
							ret = ::pilo::core::memory::o1l31c16_header::deserialise(header, bb);

							err_type = ::pilo::is_ok_or_err_type(ret, PEP_RETRY);
							if (err_type == 0)
							{
								obj = ::pilo::core::testing::large_sample_object::deserialise(&header, bb);
								if (obj == nullptr)
								{
									p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_NULL));
									return;
								}
								if (!obj->validate())
								{
									p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_INVALID));
									return;
								}

								delete obj;
								obj = nullptr;

								stc_count.fetch_add(1);
							}
							else if (err_type > 0)
							{
								//std::this_thread::yield();
							}
							else
							{								
								p_case->set_result(ret);
								return;
							}
						}

						if (obj != nullptr)
						{
							
						}						
					}
					return;
				}

				int case_linked_byte_buffer_mt_4k_net(::pilo::core::testing::func_test_case* p_case)
				{
					stc_count.store(0);
					::pilo::core::memory::linked_byte_buffer<4096, 1024, true> buffer;
					std::vector<::pilo::core::threading::auto_join_thread> producers;
					std::vector<::pilo::core::threading::auto_join_thread> consumers;

					for (int i = 0; i < PILO_LINKED_BYTE_BUFFER_CONS_THR_CNT_0; i++)
					{
						::pilo::core::threading::auto_join_thread thr(consumer, &buffer, p_case);
						consumers.push_back(std::move(thr));
					}

					for (int i = 0; i < PILO_LINKED_BYTE_BUFFER_PROD_THR_CNT_0; i++)
					{
						::pilo::core::threading::auto_join_thread thr(producer, &buffer, p_case, (::pilo::u32_t)i);
						producers.push_back(std::move(thr));
					}

					for (int i = 0; i < PILO_LINKED_BYTE_BUFFER_PROD_THR_CNT_0; i++)
					{
						producers[i].join();
					}

					for (int i = 0; i < PILO_LINKED_BYTE_BUFFER_CONS_THR_CNT_0; i++)
					{
						consumers[i].join();
					}


					return PILO_OK;
				}


				int case_linked_byte_buffer_mt_4k_local(::pilo::core::testing::func_test_case* p_case)
				{
					stc_count.store(0);
					::pilo::core::memory::linked_byte_buffer<4096, 1024, false> buffer;
					std::vector<::pilo::core::threading::auto_join_thread> producers;
					std::vector<::pilo::core::threading::auto_join_thread> consumers;

					for (int i = 0; i < PILO_LINKED_BYTE_BUFFER_CONS_THR_CNT_0; i++)
					{
						::pilo::core::threading::auto_join_thread thr(consumer, &buffer, p_case);
						consumers.push_back(std::move(thr));
					}

					for (int i = 0; i < PILO_LINKED_BYTE_BUFFER_PROD_THR_CNT_0; i++)
					{
						::pilo::core::threading::auto_join_thread thr(producer, &buffer, p_case, (::pilo::u32_t)i);
						producers.push_back(std::move(thr));
					}

					for (int i = 0; i < PILO_LINKED_BYTE_BUFFER_PROD_THR_CNT_0; i++)
					{
						producers[i].join();
					}

					for (int i = 0; i < PILO_LINKED_BYTE_BUFFER_CONS_THR_CNT_0; i++)
					{
						consumers[i].join();
					}
									

					return PILO_OK;
				}				

				int case_linked_byte_buffer_serialize_net(::pilo::core::testing::func_test_case* p_case)
				{
					const int test_count = 100000;
					::pilo::core::memory::linked_byte_buffer< 4096, 1024, true> buffer;
					::pilo::core::testing::large_sample_object sobj(false, 0);
					::pilo::core::memory::o1l31c16_header header;
					::pilo::i64_t ser_time = 0;
					for (::pilo::i64_t i = 0; i < test_count; i++)
					{
						::pilo::err_t ret = PILO_OK;
						sobj. set();
						::pilo::i64_t b = ::pilo::core::datetime::timestamp_nano_steady();
						ret = sobj.serialize(&header, &buffer);
 						ser_time += (::pilo::core::datetime::timestamp_nano_steady() - b);
						if (ret != PILO_OK)
						{
							p_case->set_result(::pilo::make_core_error(PES_OP, PEP_ABORTED));
							return PILO_OK;
						}
						
					}

					for (::pilo::i64_t i = 0; i < test_count; i++)
					{
						::pilo::err_t ret = ::pilo::core::memory::o1l31c16_header::deserialise(header, &buffer);
						::pilo::i32_t err_type = ::pilo::is_ok_or_err_type(ret, PEP_RETRY);
						::pilo::core::testing::large_sample_object* obj = nullptr;
						if (err_type == 0)
						{
							obj = ::pilo::core::testing::large_sample_object::deserialise(&header, &buffer);
							if (obj == nullptr)
							{
								p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_NULL));
								return PILO_OK;
							}
							if (!obj->validate())
							{
								p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_INVALID));
								return PILO_OK;
							}
							delete(obj);
							obj = nullptr;
						}
						else
						{
							p_case->set_result(ret);
							return PILO_OK;
						}
					}

					p_case->set_result(PILO_OK);
					char desc_buffer[128] = { 0 };
					::pilo::core::io::string_formated_output(desc_buffer, 128, "cnt %lld time %f (%s)", test_count
						, ((double)ser_time) / 1000000000.0f
						, buffer.to_string().c_str()
						);
					p_case->set_desc(desc_buffer);

					return PILO_OK;
				}

				int case_linked_byte_buffer_serialize_host(::pilo::core::testing::func_test_case* p_case)
				{
					const int test_count = 100000;
					::pilo::core::memory::linked_byte_buffer< 4096, 1024, false> buffer;
					::pilo::core::testing::large_sample_object sobj(false, 0);
					::pilo::core::memory::o1l31c16_header header;
					::pilo::i64_t ser_time = 0;
					for (::pilo::i64_t i = 0; i < test_count; i++)
					{
						::pilo::err_t ret = PILO_OK;
						sobj.set();
						::pilo::i64_t b = ::pilo::core::datetime::timestamp_nano_steady();
						ret = sobj.serialize(&header, &buffer);
						ser_time += (::pilo::core::datetime::timestamp_nano_steady() - b);
						if (ret != PILO_OK)
						{
							p_case->set_result(::pilo::make_core_error(PES_OP, PEP_ABORTED));
							return PILO_OK;
						}

					}

					for (::pilo::i64_t i = 0; i < test_count; i++)
					{
						::pilo::err_t ret = ::pilo::core::memory::o1l31c16_header::deserialise(header, &buffer);
						::pilo::i32_t err_type = ::pilo::is_ok_or_err_type(ret, PEP_RETRY);
						::pilo::core::testing::large_sample_object* obj = nullptr;
						if (err_type == 0)
						{
							obj = ::pilo::core::testing::large_sample_object::deserialise(&header, &buffer);
							if (obj == nullptr)
							{
								p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_NULL));
								return PILO_OK;
							}
							if (!obj->validate())
							{
								p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_INVALID));
								return PILO_OK;
							}
							delete(obj);
							obj = nullptr;
						}
						else
						{
							p_case->set_result(ret);
							return PILO_OK;
						}
					}

					p_case->set_result(PILO_OK);
					char desc_buffer[128] = { 0 };
					::pilo::core::io::string_formated_output(desc_buffer, 128, "cnt %lld time %f ", test_count
						, ((double)ser_time) / 1000000000.0f
					);
					p_case->set_desc(desc_buffer);

					return PILO_OK;
				}

				int case_linked_byte_buffer_serialize_1(::pilo::core::testing::func_test_case* p_case)
				{
					const ::pilo::i64_t test_count = 1000000;
					::pilo::core::memory::linked_byte_buffer< 4096, 1024, true> buffer;
					::pilo::core::testing::large_sample_object sobj(false, 0);
					::pilo::core::memory::o1l31c16_header header;
					::pilo::i64_t ser_time = 0;
					for (::pilo::i64_t i = 0; i < test_count; i++)
					{
						::pilo::err_t ret = PILO_OK;
						sobj.set();						
						::pilo::i64_t b = ::pilo::core::datetime::timestamp_nano_steady();
						ret = sobj.serialize(&header, &buffer);
						
						ser_time += (::pilo::core::datetime::timestamp_nano_steady() - b);
						if (ret != PILO_OK)
						{
							p_case->set_result(::pilo::make_core_error(PES_OP, PEP_ABORTED));
							return PILO_OK;
						}
						

						ret = ::pilo::core::memory::o1l31c16_header::deserialise(header, &buffer);
						::pilo::i32_t err_type = ::pilo::is_ok_or_err_type(ret, PEP_RETRY);
						::pilo::core::testing::large_sample_object* obj = nullptr;
						if (err_type == 0)
						{
							obj = ::pilo::core::testing::large_sample_object::deserialise(&header, &buffer);
							if (obj == nullptr)
							{
								p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_NULL));
								return PILO_OK;
							}
							if (!obj->validate())
							{
								p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_INVALID));
								return PILO_OK;
							}
							delete(obj);
							obj = nullptr;
						}
						else
						{
							p_case->set_result(ret);
							return PILO_OK;
						}
					}


					p_case->set_result(PILO_OK);
					char desc_buffer[128] = { 0 };
					::pilo::core::io::string_formated_output(desc_buffer, 128, "cnt %d time %f ", test_count
						, ((double)ser_time) / 1000000000.0f
					);
					p_case->set_desc(desc_buffer);

					return PILO_OK;
				}


				int case_linked_byte_buffer_basic(::pilo::core::testing::func_test_case* p_case)
				{
					std::string err_info;
					::pilo::core::memory::linked_byte_buffer<4096, 4096, false> buffer;
					if (!buffer.validate(err_info, 0, 0, 0, 0))
						return ::pilo::make_core_error(PES_BUFFER, PEP_IS_INVALID);

					::pilo::err_t err;
					::pilo::i64_t orig_pos = 0;
					err = buffer.writable_seek(::pilo::seek_whence_enum::current, 408000, false,  &orig_pos);
					if (err != PILO_OK)
						return err;
					if (orig_pos != 0)
						return  ::pilo::make_core_error(PES_BUFFER, PEP_OFF_INV);
					if (!buffer.validate(err_info, 408000, 0, 408000, 100))
						return ::pilo::make_core_error(PES_BUFFER, PEP_IS_INVALID);
					err = buffer.writable_seek(::pilo::seek_whence_enum::begin, 0, false, &orig_pos);
					if (err != PILO_OK)
						return err;
					if (orig_pos != 408000)
						return  ::pilo::make_core_error(PES_BUFFER, PEP_OFF_INV);
					if (!buffer.validate(err_info, 408000, 0, 0, 100))
						return ::pilo::make_core_error(PES_BUFFER, PEP_IS_INVALID);
					err = buffer.compact(::pilo::ioop_type_enum::write);
					if (err != PILO_OK)
						return err;
					if (!buffer.validate(err_info, 0, 0, 0, 0))
						return ::pilo::make_core_error(PES_BUFFER, PEP_IS_INVALID);

					err = buffer.writable_seek(::pilo::seek_whence_enum::current, 408000, false, &orig_pos);
					if (err != PILO_OK)
						return err;
					err = buffer.readable_seek(::pilo::seek_whence_enum::current, 204000, false, &orig_pos);
					if (err != PILO_OK)
						return err;
					if (orig_pos != 0)
						return  ::pilo::make_core_error(PES_BUFFER, PEP_OFF_INV);
					if (!buffer.validate(err_info, 408000, 204000, 204000, 100))
						return ::pilo::make_core_error(PES_BUFFER, PEP_IS_INVALID);
					err = buffer.compact(::pilo::ioop_type_enum::read);
					if (err != PILO_OK)
						return err;
					if (!buffer.validate(err_info, 204000, 0, 204000, 50))
						return ::pilo::make_core_error(PES_BUFFER, PEP_IS_INVALID);
					err = buffer.readable_seek(::pilo::seek_whence_enum::current, 204000, false, &orig_pos);
					if (err != PILO_OK)
						return err;
					if (orig_pos != 0)
						return  ::pilo::make_core_error(PES_BUFFER, PEP_OFF_INV);
					err = buffer.compact(::pilo::ioop_type_enum::read);
					if (err != PILO_OK)
						return err;
					if (!buffer.validate(err_info, 0, 0, 0, 0))
						return ::pilo::make_core_error(PES_BUFFER, PEP_IS_INVALID);


					err = buffer.writable_seek(::pilo::seek_whence_enum::current, 408000, false, &orig_pos);
					if (err != PILO_OK)
						return err;
					err = buffer.readable_seek(::pilo::seek_whence_enum::current, 204000, false, &orig_pos);
					if (err != PILO_OK)
						return err;
					err = buffer.writable_seek(::pilo::seek_whence_enum::current, -204000, false, &orig_pos);
					if (err != PILO_OK)
						return err;
					if (orig_pos != 408000)
						return  ::pilo::make_core_error(PES_BUFFER, PEP_OFF_INV);
					err = buffer.compact(::pilo::ioop_type_enum::read_write);
					if (err != PILO_OK)
						return err;
					if (!buffer.validate(err_info, 0, 0, 0, 0))
						return ::pilo::make_core_error(PES_BUFFER, PEP_IS_INVALID);


					p_case->set_result(PILO_OK);
					return PILO_OK;
				}
			}
		}
	}
}