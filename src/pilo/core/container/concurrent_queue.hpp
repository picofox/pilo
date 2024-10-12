#ifndef _pilo_core_container_concurrent_queue_hpp_
#define _pilo_core_container_concurrent_queue_hpp_

#include "../../external/concurrentqueue/concurrentqueue.h"

namespace pilo
{
	namespace core
	{
		namespace container
		{
			template<typename T, typename Traits = ::moodycamel::ConcurrentQueueDefaultTraits> 
			class concurrent_queue : public ::moodycamel::ConcurrentQueue<T, Traits>
			{
			public:
			};
		}
	}
}

#endif // ! _pilo_core_container_concurrent_queue_hpp_