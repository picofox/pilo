
#include "./mono_service_group.hpp"


namespace pilo
{
	namespace core
	{
		namespace service
		{
			mono_service_group::~mono_service_group()
			{
				remove_service(0);
			}
			service_interface* mono_service_group::service(::pilo::service_id)
			{
				return _m_service;
			}
			::pilo::service_id mono_service_group::add_service(service_interface* iservice)
			{
				if (_m_service != nullptr)
					return -1;
				_m_service = iservice;
				return 0;
			}
			::pilo::err_t mono_service_group::remove_service(::pilo::service_id)
			{
				if (_m_service == nullptr)
					return ::pilo::mk_perr(PERR_NOOP);
				if (!_m_service->is_stopped())
					return ::pilo::mk_perr(PERR_INV_OFF);
				delete _m_service;
				_m_service = nullptr;
				return PILO_OK;
			}
			::pilo::err_t mono_service_group::remove_all_service()
			{
				return remove_service(0);
			}
			::pilo::i32_t mono_service_group::service_count() const
			{
				if (_m_service != nullptr)
					return 1;
				return 0;
			}
			::pilo::err_t mono_service_group::iterate(service_iterate_func_type cb)
			{
				return cb(0, _m_service, this);
			}
			::pilo::err_t mono_service_group::start(::pilo::i64_t now_ts)
			{
				if (_m_service == nullptr)
					return ::pilo::mk_perr(PERR_NULL_PTR);
				return _m_service->start(now_ts);
			}
			::pilo::err_t mono_service_group::stop(::pilo::i64_t now_ts)
			{
				if (_m_service == nullptr)
					return ::pilo::mk_perr(PERR_NULL_PTR);
				return _m_service->stop(now_ts);
			}
			bool mono_service_group::all_stopped() const
			{
				if (_m_service == nullptr)
					return true;
				return _m_service->is_stopped();
			}
		}
	}
}



