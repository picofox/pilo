#include <thread>
#include "../../pilo_stable_test.hpp"
#include "pilo/core/testing/func_test_suite.hpp"
#include "pilo/core/string/string_operation.hpp"
#include "pilo/core/io/formatted_io.hpp"
#include "pilo/core/threading/auto_join_thread.hpp"
#include "pilo/core/threading/spin_mutex.hpp"
#include "pilo/core/threading/order_awared_mutex_adaptor.hpp"
#include "pilo/core/container/concurrent_queue.hpp"
#include "pilo/core/datetime/stop_watch.hpp"
#include "pilo/core/threading/spin_mutex.hpp"
#include "pilo/core/testing/fixsized_sample_object.hpp"


using namespace ::pilo::stable_test;
using namespace ::pilo::core::testing;


namespace pilo
{
	namespace stable_test
	{
		namespace core
		{
			namespace container
			{
				static void producer(::pilo::core::container::concurrent_queue<sample_128b_object> * queue_ptr, ::pilo::i64_t count, ::pilo::core::testing::stable_test_case* p_case)
				{
					::pilo::i64_t sent = 0;
					while (true)
					{
						if (p_case->result() == PILO_OK)
						{
							return;
						}
						else if (p_case->result() != ::pilo::mk_perr(PERR_RETRY))
						{
							return;
						}
						if (p_case->target_trans_count() > 0 && p_case->trans_count() >= p_case->target_trans_count())
						{
							return;
						}
						if (p_case->target_persist_millisecs() > 0 && p_case->timeout())
						{
							return;
						}

						sample_128b_object obj;
						obj.set(0);

						bool ok = queue_ptr->enqueue(obj);
						if (ok)
						{
							sent++;
							if (sent >= count)
							{
								return;
							}
						}
						else
						{
							p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
							return;
						}

					}
					
				}

				static void consumer(::pilo::core::container::concurrent_queue<sample_128b_object>* queue_ptr, ::pilo::core::testing::stable_test_case* p_case)
				{
					while (true)
					{
						if (p_case->trans_count() >= p_case->target_trans_count())
						{
							p_case->set_result(PILO_OK);
							return;
						}
							
						if (p_case->result() == PILO_OK)
						{
							return;
						}
						else if (p_case->result() != ::pilo::mk_perr(PERR_RETRY))
						{
							return;
						}


						sample_128b_object obj;
						bool ok = queue_ptr->try_dequeue(obj);
						if (ok)
						{
							if (obj.validate())
							{
								p_case->inc_trans_count();
							}
							else
							{
								p_case->set_result(::pilo::mk_perr(PERR_USER_CANCEL));
								return;
							}
						}
					}
				}

				static int _concurrent_queue_test(::pilo::core::testing::stable_test_case* p_case, ::pilo::i32_t nproducer, ::pilo::i32_t ncomsumer)
				{
					::pilo::core::container::concurrent_queue<sample_128b_object> queue;
					::pilo::i64_t target_count_per_thread = p_case->target_trans_count() / nproducer;
					::pilo::i64_t remain_count = p_case->target_trans_count() % nproducer;
					std::vector<::pilo::core::threading::auto_join_thread> producers;
					std::vector<::pilo::core::threading::auto_join_thread> consumers;

					p_case->set_begin();

					for (int i = 0; i < ncomsumer; i++)
					{
						::pilo::core::threading::auto_join_thread thr(consumer, &queue, p_case);
						consumers.push_back(std::move(thr));
					}

					for (int i = 0; i < nproducer; i++)
					{
						::pilo::i64_t target_count = target_count_per_thread;
						if (i == 0)
							target_count += remain_count;
						::pilo::core::threading::auto_join_thread thr(producer, &queue, target_count, p_case);
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

					if (p_case->result() != PILO_OK) 
					{
						return p_case->result();
					}
					
					return PILO_OK;
				}

				int case_concurrent_queue_for_1g_count(::pilo::core::testing::stable_test_case* p_case)
				{
					return _concurrent_queue_test(p_case, 2, 3);
				}
			}			
		}
	}
}

