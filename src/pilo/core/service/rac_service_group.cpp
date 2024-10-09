
#include "./rac_service_group.hpp"


namespace pilo
{
	namespace core
	{
		namespace service
		{
			rac_service_group::~rac_service_group()
			{

			}

			service_interface* rac_service_group::service(::pilo::service_id id)
			{
				return _services[id];
			}

			::pilo::service_id rac_service_group::add_service(service_interface* iservice)
			{
				::pilo::i32_t sz = (::pilo::i32_t) _services.size();
				if (sz >= PMI_MAX_SERVICE_ID) {
					return PMI_INVALID_SERVICE_ID;
				}
				_services.push_back(iservice);
				iservice->set_id((::pilo::service_id) sz);
				return (::pilo::service_id) sz;
			}

			::pilo::err_t rac_service_group::remove_service(::pilo::service_id id)
			{
				service_interface* iservice = nullptr;
				if (_services.size() <= id) {
					return ::pilo::mk_err(PERR_NON_EXIST);
				}
				iservice = _services[id];
				if (iservice == nullptr)
					return ::pilo::mk_perr(PERR_NOOP);
				if (!iservice->is_stopped())
					return ::pilo::mk_perr(PERR_INV_OFF);
				delete iservice;
				_services[id] = nullptr;
				return PILO_OK;
			}
			::pilo::err_t rac_service_group::remove_all_service()
			{
				::pilo::err_t err = PILO_OK;
				for (int i = 0; i < _services.size(); i++) {
					err = remove_service((::pilo::service_id)i);
					if (err != PILO_OK)
						return err;
				}
				return PILO_OK;
			}
			::pilo::i32_t rac_service_group::service_count() const
			{
				return (::pilo::i32_t) _services.size();
			}
			::pilo::err_t rac_service_group::iterate(service_iterate_func_type cb)
			{
				::pilo::err_t err = PILO_OK;
				for (::pilo::service_id i = 0; i < _services.size(); i++) {
					err = cb(i, _services[i], this);
					if (err != PILO_OK)
						return err;
				}
				return PILO_OK;
			}
			::pilo::err_t rac_service_group::start(::pilo::i64_t now_ts)
			{
				::pilo::err_t err = PILO_OK;
				for (int i = 0; i < _services.size(); i++) {
					err = _services.at(i)->start(now_ts);
					if (err != PILO_OK)
						return err;
				}
				return PILO_OK;
			}
			::pilo::err_t rac_service_group::stop(::pilo::i64_t now_ts)
			{
				::pilo::err_t err = PILO_OK;
				for (int i = 0; i < _services.size(); i++) {
					err = _services.at(i)->stop(now_ts);
					if (err != PILO_OK)
						return err;
				}
				return PILO_OK;
			}
			bool rac_service_group::all_stopped() const
			{
				for (int i = 0; i < _services.size(); i++) {
					if (! _services.at(i)->is_stopped()) {
						return false;
					}					
				}
				return true;
			}
		}
	}
}



