#ifndef _pilo_core_service_service_interface_hpp_
#define _pilo_core_service_service_interface_hpp_

#include "../../pilo.hpp"
#include "../datetime/timestamp.hpp"

namespace pilo
{
	namespace core
	{
		namespace service
		{
			enum class service_state {
				stopped = 0,
				running = 1,
				shutting_down = 2,
			};

			class service_group_interface;

			class service_interface
			{		
			public:
				service_interface(service_group_interface* grp, ::pilo::service_id id)
					:_m_group_ref(grp)
					, _m_last_pulse(0)
					, _m_state(service_state::stopped)
					, _m_id(id)
				{
				}

				::pilo::service_id id() const { return _m_id; }
				void set_id(::pilo::service_id id) { _m_id = id; }
				::pilo::service_group_id group_id() const;
				service_state state() const { return _m_state; }
				inline bool is_running() const { return _m_state == service_state::running;  }
				inline bool is_shutting_down() const { return _m_state == service_state::shutting_down; }
				inline bool is_stopped() const { return _m_state == service_state::stopped; }
				inline ::pilo::err_t shutdown() 
				{
					if (is_running()) {
						_m_state = service_state::shutting_down;
						return PILO_OK;
					}
					return ::pilo::mk_perr(PERR_NOOP);
				}
				inline ::pilo::err_t set_running()
				{
					if (! is_running()) {
						_m_state = service_state::running;
						return PILO_OK;
					}
					return ::pilo::mk_perr(PERR_NOOP);
				}
				inline ::pilo::err_t set_stoppped()
				{
					_m_state = service_state::stopped;
					return PILO_OK;
				}
				virtual ~service_interface() {}
				virtual void pulse(::pilo::i64_t now_ts) = 0;
				virtual void check_pulse(::pilo::i64_t now_ts) = 0;				
				virtual void set_pulse(::pilo::i64_t i) = 0;


			public:
				virtual ::pilo::err_t on_stopped()
				{
					_m_state = service_state::stopped;
					return PILO_OK;
				}
				virtual ::pilo::err_t on_started()
				{
					_m_state = service_state::running;
					return PILO_OK;
				}

				virtual ::pilo::err_t start(::pilo::i64_t now_ts)
				{
					_m_last_pulse = now_ts;
					return this->set_running();
				}
				virtual ::pilo::err_t stop(::pilo::i64_t now_ts)
				{
					_m_last_pulse = now_ts;
					return this->shutdown();
				}

			protected:
				::pilo::core::service::service_group_interface* const  _m_group_ref;
				::pilo::i64_t					_m_last_pulse;
				service_state					_m_state;
				::pilo::service_id				_m_id;	

			};
		}
	}
}

#endif