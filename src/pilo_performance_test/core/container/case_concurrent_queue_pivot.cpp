#include <thread>
#include "../../pilo_performance_test.hpp"
#include "pilo/core/testing/performance_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/threading/auto_join_thread.hpp"
#include "pilo/core/threading/spin_mutex.hpp"
#include "pilo/core/threading/order_awared_mutex_adaptor.hpp"
#include "pilo/core/container/concurrent_queue.hpp"
#include "pilo/core/datetime/stop_watch.hpp"
#include "pilo/core/threading/spin_mutex.hpp"
#include "pilo/core/testing/fixsized_sample_object.hpp"
#include "pilo/core/container/trivil_concurrent_queue.hpp"
#include <memory>

namespace pilo
{
	namespace performance_test
	{
		namespace core
		{
			namespace container
			{
				typedef ::pilo::core::container::concurrent_queue<::pilo::core::testing::sample_129b_object> c_queue_t;
				typedef ::pilo::core::container::trivil_concurrent_queue<::pilo::core::testing::sample_129b_object, ::pilo::core::threading::spin_mutex> tc_queue_t;
				typedef ::pilo::core::container::trivil_concurrent_queue<::pilo::core::testing::sample_129b_object, ::pilo::core::threading::dummy_mutex> nl_tc_queue_t;


				template<typename TA_QUEUE>
				void producer(TA_QUEUE * queue_ptr, ::pilo::i64_t count, ::pilo::core::testing::performance_test_case* p_case)
				{
					::pilo::i64_t sent = 0;
					::pilo::i64_t beg = ::pilo::core::datetime::timestamp_milli_steady();
					::pilo::i64_t cost = 0;
					while (true)
					{
						if (p_case->result() == PILO_OK)
						{
							break;
						}
						else if (p_case->result() != ::pilo::mk_perr(PERR_RETRY))
						{
							break;
						}
						if (p_case->target_trans_count() > 0 && p_case->in_trans_count() >= p_case->target_trans_count())
						{
							break;
						}
						if (p_case->target_persist_millisecs() > 0 && p_case->timeout())
						{
							break;
						}
						::pilo::core::testing::sample_129b_object obj;
						obj.set(0);
						bool ok = queue_ptr->enqueue(obj);
						if (ok)
						{
							sent++;
							p_case->inc_in_trans_count(1);
							if (sent >= count)
							{
								break;
							}
						}
						else
						{
							p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
							break;
						}

					}
					p_case->inc_in_bytes_count(sent * 129);
					cost = ::pilo::core::datetime::timestamp_milli_steady() - beg;
					p_case->inc_in_milli_seconds(cost);
					
				}

				template<typename TA_QUEUE>
				void consumer(TA_QUEUE* queue_ptr, ::pilo::core::testing::performance_test_case* p_case)
				{
					::pilo::i64_t beg = ::pilo::core::datetime::timestamp_milli_steady();
					::pilo::i64_t cost = 0;
					while (true)
					{
						if (p_case->out_trans_count() >= p_case->target_trans_count())
						{
							break;
						}
							
						if (p_case->result() == PILO_OK)
						{
							break;
						}
						else if (p_case->result() != ::pilo::mk_perr(PERR_RETRY))
						{
							break;
						}


						::pilo::core::testing::sample_129b_object obj;
						bool ok = queue_ptr->try_dequeue(obj);
						if (ok)
						{
							if (obj.validate())
							{
								p_case->inc_out_trans_count(1);
								p_case->inc_out_bytes_count(129);
							}
							else
							{
								p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
								break;
							}
						}
					}
					
					
					cost = ::pilo::core::datetime::timestamp_milli_steady() - beg;
					p_case->inc_out_milli_seconds(cost);
				}


				template<typename TA_QUEUE>
				int _concurrent_queue_test(::pilo::core::testing::performance_test_case* p_case, ::pilo::i32_t nproducer, ::pilo::i32_t ncomsumer)
				{
					TA_QUEUE queue;
					::pilo::i64_t target_count_per_thread = p_case->target_trans_count() / nproducer;
					::pilo::i64_t remain_count = p_case->target_trans_count() % nproducer;
					std::vector<::pilo::core::threading::auto_join_thread> producers;
					std::vector<::pilo::core::threading::auto_join_thread> consumers;

					p_case->set_begin();

					for (int i = 0; i < ncomsumer; i++)
					{
						::pilo::core::threading::auto_join_thread thr(consumer<TA_QUEUE>, &queue, p_case);
						consumers.push_back(std::move(thr));
					}

					for (int i = 0; i < nproducer; i++)
					{
						::pilo::i64_t target_count = target_count_per_thread;
						if (i == 0)
							target_count += remain_count;
						::pilo::core::threading::auto_join_thread thr(producer<TA_QUEUE>, &queue, target_count, p_case);
						producers.push_back(std::move(thr));
					}	

					for (int i = 0; i < nproducer; i++)
					{
						producers[i].join();
					}

					for (int i = 0; i < ncomsumer; i++)
					{
						consumers[i].join();
					}

					if (p_case->result() == ::pilo::mk_perr(PERR_RETRY))
					{
						if (p_case->out_trans_count() != p_case->target_trans_count())
						{
							p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
							return ::pilo::mk_perr(PERR_USER_CANCEL);
						}
						p_case->set_result(PILO_OK);
					} 
					else if (p_case->result() == PILO_OK)
					{

					}
					else
					{
						return p_case->result();
					}
				

					
					return PILO_OK;
				}

				int case_bench_pivot_floor(::pilo::core::testing::performance_test_case* p_case)
				{
					int concur = p_case->concurrency() / 2;
					return _concurrent_queue_test<tc_queue_t>(p_case, concur, concur);
				}


				static void queue_test_pivot_ceil(nl_tc_queue_t* queue, ::pilo::i64_t cnt, ::pilo::core::testing::performance_test_case* p_case)
				{
					::pilo::i64_t beg = ::pilo::core::datetime::timestamp_milli_steady();
					::pilo::i64_t in_nano_cost = 0;
					for (::pilo::i64_t i = 0; i < cnt; i++)
					{
						::pilo::i64_t tmp_beg = ::pilo::core::datetime::timestamp_nano_steady();
						::pilo::core::testing::sample_129b_object obj;
						obj.set(0);
						bool ok = queue->enqueue(obj);
						if (ok)
						{
							
						}
						else
						{
							p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
							return;
						}
						::pilo::i64_t tmp_diff = ::pilo::core::datetime::timestamp_nano_steady() - tmp_beg;
						in_nano_cost += tmp_diff;
						::pilo::core::testing::sample_129b_object obj_received;
						ok = queue->try_dequeue(obj);
						if (ok)
						{
							if (!obj.validate())
							{
								p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
								return;
							}
							else 
							{
								p_case->inc_out_trans_count(1);
							}
						}

					}

					::pilo::i64_t diff = ::pilo::core::datetime::timestamp_milli_steady() - beg;
					::pilo::f64_t ff = ((::pilo::f64_t) in_nano_cost) / 1000000.0f;
					p_case->inc_in_milli_seconds((::pilo::i64_t)ff);
					p_case->inc_out_milli_seconds(diff - (::pilo::i64_t)ff);
					p_case->inc_in_trans_count(cnt);					
					p_case->inc_in_bytes_count(cnt*129);
					p_case->inc_out_bytes_count(cnt * 129);


					return;
				}

				int case_bench_pivot_ceil(::pilo::core::testing::performance_test_case* p_case)
				{
					std::unique_ptr<nl_tc_queue_t[]> queues(new nl_tc_queue_t[p_case->concurrency()]);
					std::unique_ptr<::pilo::i64_t[]> tcnts(new ::pilo::i64_t[p_case->concurrency()]);
					std::vector<::pilo::core::threading::auto_join_thread> threads;
					::pilo::i64_t target_count_per_thread = p_case->target_trans_count() / p_case->concurrency();
					::pilo::i64_t remain_count = p_case->target_trans_count() % p_case->concurrency();

					for (int i = 0; i < p_case->concurrency(); i++)
					{
						tcnts[i] = target_count_per_thread;
					}
					tcnts[0] += remain_count;

					p_case->set_begin();

					for (int i = 0; i < p_case->concurrency(); i++)
					{
						::pilo::core::threading::auto_join_thread thr(queue_test_pivot_ceil, &queues[i], tcnts[i], p_case);
						threads.push_back(std::move(thr));
					}

					for (int i = 0; i < p_case->concurrency(); i++)
					{
						threads[i].join();
					}

					if (p_case->out_trans_count() != p_case->target_trans_count())
					{
						p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
						return ::pilo::mk_perr(PERR_USER_CANCEL);
					}

					if (p_case->result() == ::pilo::mk_perr(PERR_RETRY))
					{
						p_case->set_result(PILO_OK);
					}
					return PILO_OK;
				}




				int case_bench_pivot_cq(::pilo::core::testing::performance_test_case* p_case)
				{
					int concur = p_case->concurrency() / 2;								
					return _concurrent_queue_test<c_queue_t>(p_case, concur, concur);
		
				}

				std::string make_cq_score(const ::pilo::core::testing::performance_test_case* lowest_pivot_case
					, const ::pilo::core::testing::performance_test_case* highest_pivot_case
					, const ::pilo::core::testing::performance_test_case* case_ptr)
				{
					::pilo::f64_t f_score_hl = (::pilo::f64_t) highest_pivot_case->bps_overall() / (::pilo::f64_t)lowest_pivot_case->bps_overall();
					::pilo::f64_t f_score_cl= (::pilo::f64_t)case_ptr->bps_overall() / (::pilo::f64_t)lowest_pivot_case->bps_overall();
					::pilo::f64_t f_score = f_score_cl / f_score_hl * 1000;
					::pilo::i32_t score = (::pilo::i32_t)f_score;
					std::string str = std::to_string(score);
					str += "/1000";
					return str;
				}
			}			
		}
	}
}

