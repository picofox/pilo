#include <thread>
#include "../../pilo_performance_test.hpp"
#include "pilo/core/testing/performance_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/testing/sample_object.hpp"
#include "pilo/core/testing/fixsized_sample_object.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/memory/linked_byte_buffer.hpp"
#include "pilo/core/threading/auto_join_thread.hpp"
#include "pilo/core/testing/large_sample_object.hpp"
#include "pilo/core/memory/o1l31c16_header.hpp"
#include "pilo/core/datetime/stop_watch.hpp"
#include "pilo/core/memory/fixed_byte_buffer.hpp"
#include <memory>




namespace pilo
{
	namespace performance_test
	{
		namespace core
		{
			namespace memory
			{
				

				template<typename T>
				static void producer(::pilo::core::memory::byte_buffer_interface* bb, ::pilo::core::testing::performance_test_case* p_case)
				{
					::pilo::i64_t in_nano_cost = 0;
					::pilo::i64_t in_nano_cost_total = 0;
					::pilo::i64_t current_prod_byte_size = 0;
					::pilo::i64_t current_prod_count = 0;
					::pilo::i64_t current_prod_target = p_case->target_trans_count() / p_case->concurrency() * 2;
					T obj;
					::pilo::core::memory::o1l31c16_header header;			
					
					
					while (current_prod_count < current_prod_target && ! p_case->timeout())
					{									
						obj.set();			
						::pilo::err_t ret = PILO_OK;
						{
							std::lock_guard<::pilo::core::testing::performance_test_case::lock_type> guard(p_case->get_lock());			
							in_nano_cost = ::pilo::core::datetime::timestamp_nano_steady();
							ret = obj.serialize(&header, bb);	
							in_nano_cost = ::pilo::core::datetime::timestamp_nano_steady() - in_nano_cost;
						}							
						in_nano_cost_total += in_nano_cost;
						if (ret != PILO_OK)
						{
							p_case->set_result(::pilo::make_core_error(PES_OP, PEP_ABORTED));
							p_case->inc_out_milli_seconds(current_prod_count);
							return;
						}
						current_prod_count++;
						current_prod_byte_size += obj.byte_size();
					}
					

					p_case->inc_in_milli_seconds(in_nano_cost_total / 1000000);
					p_case->inc_in_bytes_count(current_prod_byte_size);
					p_case->inc_in_trans_count(current_prod_count);	
				
				}
	

				template<typename T>
				static void consumer(::pilo::core::memory::byte_buffer_interface* bb, ::pilo::core::testing::performance_test_case* p_case)
				{
					::pilo::err_t ret = PILO_OK;
					::pilo::i32_t err_type = 0;
					::pilo::core::memory::o1l31c16_header header;
					T* obj = nullptr;
					::pilo::i64_t beg = 0;
					for (;;)
					{
						err_type = ::pilo::is_ok_or_err_type(p_case->result(), PEP_RETRY);
						if (err_type <= 0)
						{
							return;
						}

						
						{
							std::lock_guard<::pilo::core::testing::performance_test_case::lock_type> guard(p_case->get_lock());
							beg = ::pilo::core::datetime::timestamp_milli_steady();
							ret = ::pilo::core::memory::o1l31c16_header::deserialise(header, bb);							
							err_type = ::pilo::is_ok_or_err_type(ret, PEP_RETRY);
							if (err_type == 0)
							{
								obj = T::allocate();
								obj->deserialize(&header, bb);
								if (obj == nullptr)
								{
									p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_NULL));
									return;
								}
								beg = ::pilo::core::datetime::timestamp_milli_steady() - beg;
								if (!obj->validate())
								{
									p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_INVALID));
									return;
								}
								p_case->inc_out_trans_count(1);
								p_case->inc_out_bytes_count(obj->byte_size());
								p_case->inc_out_milli_seconds(beg);
								T::deallocate(obj);
								obj = nullptr;	
							}
							else if (err_type < 0)
							{
								p_case->set_result(ret);
								return;
							}
						}


						if (p_case->out_trans_count() >= p_case->target_trans_count())
						{
							p_case->set_result(PILO_OK);
							return;
						}
					}
					return;
				}


				template<typename T, typename TA_OBJ>
				int case_buffer_fcnt_local_st(::pilo::core::testing::performance_test_case* p_case)
				{
					T buffer;
					::pilo::i32_t ret;
					::pilo::i64_t in_nano_cost = 0;
					::pilo::i64_t in_nano_cost_total = 0;
					::pilo::i64_t current_prod_byte_size = 0;
					::pilo::i64_t current_prod_count = 0;
					::pilo::i64_t current_prod_target = p_case->target_trans_count();
					TA_OBJ obj;
					::pilo::core::memory::o1l31c16_header header;
					TA_OBJ* pobj = nullptr;
					p_case->set_begin();

					while (current_prod_count < current_prod_target)
					{
						ret = PILO_OK;
						obj.set();
						in_nano_cost = ::pilo::core::datetime::timestamp_nano_steady();
						ret = obj.serialize(&header, &buffer);
						if (ret != PILO_OK)
						{
							p_case->set_result(ret);
							return PILO_OK;
						}						
						in_nano_cost = ::pilo::core::datetime::timestamp_nano_steady() - in_nano_cost;
						in_nano_cost_total += in_nano_cost;						
						current_prod_count++;
						current_prod_byte_size += obj.byte_size();
					}
					p_case->inc_in_milli_seconds(in_nano_cost_total / 1000000);
					p_case->inc_in_bytes_count(current_prod_byte_size);
					p_case->inc_in_trans_count(current_prod_count);

					::pilo::i64_t out_cnt = 0;
					::pilo::i64_t out_bs = 0;
					::pilo::i64_t out_millis = 0;
					::pilo::i64_t out_millis_total = 0;
					while(out_cnt < current_prod_target)
					{
						out_millis = ::pilo::core::datetime::timestamp_nano_steady();
						ret = ::pilo::core::memory::o1l31c16_header::deserialise(header, &buffer);
						int err_type = ::pilo::is_ok_or_err_type(ret, PEP_RETRY);
						if (err_type == 0)
						{
							pobj = TA_OBJ::allocate() ;
							pobj->deserialize(&header, &buffer);
							if (pobj == nullptr)
							{
								p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_NULL));
								return PILO_OK;
							}
							out_millis = ::pilo::core::datetime::timestamp_nano_steady() - out_millis;
							if (!pobj->validate())
							{
								p_case->set_result(::pilo::make_core_error(PES_OBJ, PEP_IS_INVALID));
								return PILO_OK;
							}
							out_cnt++;
							out_bs += pobj->byte_size();
							out_millis_total += out_millis;
							TA_OBJ::deallocate(pobj);
							pobj = nullptr;

						}
						else if (err_type < 0)
						{
							p_case->set_result(ret);
							return PILO_OK;
						}							
					}
					p_case->set_result(PILO_OK);
					p_case->inc_out_bytes_count(out_bs);
					p_case->inc_out_trans_count(out_cnt);
					p_case->inc_out_milli_seconds(out_millis_total / 1000000);


					return PILO_OK;
				}


				int case_linked_byte_buffer_sobj_host_st(::pilo::core::testing::performance_test_case* p_case)
				{
					return case_buffer_fcnt_local_st< ::pilo::core::memory::linked_byte_buffer<4096, 1024, false>, ::pilo::core::testing::sample_object<1600>>(p_case);
				}

				int case_fixed_byte_buffer_sobj_host_st(::pilo::core::testing::performance_test_case* p_case)
				{
					return case_buffer_fcnt_local_st< ::pilo::core::memory::fixed_byte_buffer<1700 * PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, false>, ::pilo::core::testing::sample_object<1600>>(p_case);
				}

				int case_linked_byte_buffer_sobj_net_st(::pilo::core::testing::performance_test_case* p_case)
				{
					return case_buffer_fcnt_local_st< ::pilo::core::memory::linked_byte_buffer<4096, 1024, true>, ::pilo::core::testing::sample_object<1600>>(p_case);
				}

				int case_fixed_byte_buffer_sobj_net_st(::pilo::core::testing::performance_test_case* p_case)
				{
					return case_buffer_fcnt_local_st< ::pilo::core::memory::fixed_byte_buffer<1700 * PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, true>, ::pilo::core::testing::sample_object<1600>>(p_case);
				}





				int case_linked_byte_buffer_lobj_host_st(::pilo::core::testing::performance_test_case* p_case)
				{
					return case_buffer_fcnt_local_st< ::pilo::core::memory::linked_byte_buffer<4096, 1024, false>, ::pilo::core::testing::large_sample_object>(p_case);
				}
			
				int case_fixed_byte_buffer_lobj_host_st(::pilo::core::testing::performance_test_case* p_case)
				{
					return case_buffer_fcnt_local_st< ::pilo::core::memory::fixed_byte_buffer<1700* PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, false>, ::pilo::core::testing::large_sample_object>(p_case);
				}

				int case_linked_byte_buffer_lobj_net_st(::pilo::core::testing::performance_test_case* p_case)
				{
					return case_buffer_fcnt_local_st< ::pilo::core::memory::linked_byte_buffer<4096, 1024, true>, ::pilo::core::testing::large_sample_object>(p_case);
				}

				int case_fixed_byte_buffer_lobj_net_st(::pilo::core::testing::performance_test_case* p_case)
				{
					return case_buffer_fcnt_local_st< ::pilo::core::memory::fixed_byte_buffer<1700 * PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, true>, ::pilo::core::testing::large_sample_object>(p_case);
				}


				template<typename T, typename TA_OBJ>
				static int _case_byte_buffer_perf_spsc(::pilo::core::testing::performance_test_case* p_case)
				{
					T buffer;
					std::vector<::pilo::core::threading::auto_join_thread> producers;
					std::vector<::pilo::core::threading::auto_join_thread> consumers;

					p_case->set_begin();

					for (int i = 0; i < (p_case->concurrency() / 2); i++)
					{
						::pilo::core::threading::auto_join_thread thr(consumer<::pilo::core::testing::sample_object<1600>>, &buffer, p_case);
						consumers.push_back(std::move(thr));
					}

					for (int i = 0; i < (p_case->concurrency() / 2); i++)
					{
						::pilo::core::threading::auto_join_thread thr(producer<::pilo::core::testing::sample_object<1600>>, &buffer, p_case);
						producers.push_back(std::move(thr));
					}

					for (int i = 0; i < (p_case->concurrency() / 2); i++)
					{
						producers[i].join();
					}

					for (int i = 0; i < (p_case->concurrency() / 2); i++)
					{
						consumers[i].join();
					}


					return PILO_OK;
				}


				int case_fixed_byte_buffer_spsc_host(::pilo::core::testing::performance_test_case* p_case)
				{
					return _case_byte_buffer_perf_spsc<::pilo::core::memory::fixed_byte_buffer<1700 * PILO_BYTE_BUFFER_PERF_TEST_PIVOT_COUNT, false>, ::pilo::core::testing::sample_object<1600>>(p_case);
				}

				int case_linked_byte_buffer_spsc_host(::pilo::core::testing::performance_test_case* p_case)
				{
					return _case_byte_buffer_perf_spsc<::pilo::core::memory::linked_byte_buffer<4096, 1024, false>, ::pilo::core::testing::sample_object<32768>>(p_case);
				}

				

			}
		}
	}
}