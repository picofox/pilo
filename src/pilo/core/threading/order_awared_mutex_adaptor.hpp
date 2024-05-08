#pragma once
#include "../../pilo.hpp"
#include <mutex>

namespace pilo
{
	namespace core
	{
		namespace threading
		{
			template<typename TA_LOCK>
			class order_awared_mutex_adaptor
			{
			public:
				typedef TA_LOCK	lock_type;

			public:
				explicit order_awared_mutex_adaptor(::pilo::u32_t seq) : _order(seq), _previous_order(0) {}
				order_awared_mutex_adaptor(const order_awared_mutex_adaptor&) = delete;
				order_awared_mutex_adaptor& operator=(const order_awared_mutex_adaptor&) = delete;

				inline void lock()
				{
					_lock_check();
					this->_internal_handler.lock();
					_locked_update();
				}

				inline void unlock()
				{
					if (tsd_current_lock_order != _order)
						throw std::logic_error("multi-mutex lock/unlock order sequence logic illegal!");
					tsd_current_lock_order = _previous_order;
					this->_internal_handler.unlock();
				}

				inline bool trylock()
				{
					_lock_check();
					if (! this->_internal_handler.trylock())
					{
						return false;
					}
					_locked_update();
				}


			private:
				inline void _lock_check() const
				{
					if (tsd_current_lock_order <= _order)
						throw std::logic_error("multi-mutex lock/unlock order sequence logic illegal!");
				}

				inline void _locked_update()
				{
					_previous_order = tsd_current_lock_order;
					tsd_current_lock_order = _order;
				}


			private:
				::pilo::u32_t  const _order;
				::pilo::u32_t _previous_order;
				static thread_local ::pilo::u32_t tsd_current_lock_order;

				lock_type _internal_handler;

			
			};

			template < typename T > thread_local ::pilo::u32_t order_awared_mutex_adaptor<T>::tsd_current_lock_order(0xFFFFFFFF);
			
		}
	}
}