#include "service_manager.hpp"

#include "abstract_compensable_fixed_frequency_service.hpp"
#include "abstract_fixed_frequency_service.hpp"
#include "abstract_fixed_interval_service.hpp"
#include "../process/context.hpp"
#include "../threading/threading.hpp"

namespace pilo
{
	namespace core
	{
		namespace service
		{
			service_manager::~service_manager()
			{
				stop();
				::pilo::err_t err = PILO_OK;
				std::map<::pilo::service_group_id, service_group_interface*>::iterator it = _group.begin();
				for (; it != _group.end(); it++) {
					err = it->second->remove_all_service();
					if (err != PILO_OK) {
					}
					delete it->second;
					it->second = nullptr;
				}				
				_group.clear();
			}
			::pilo::err_t service_manager::add_service(service_interface* iservice)
			{
				service_group_interface* grp = group(iservice->group_id());
				if (grp == nullptr) {
					return ::pilo::mk_perr(PERR_NON_EXIST);
				}
				grp->add_service(iservice);
				return ::pilo::err_t();
			}

			::pilo::err_t service_manager::remove_service(::pilo::service_group_id sgid, ::pilo::service_id sid)
			{
				service_group_interface* grp = group(sgid);
				if (grp == nullptr) {
					return ::pilo::mk_perr(PERR_NON_EXIST);
				}				
				grp->remove_service(sid);

				if (grp->service_count() < 1) {
					_group.erase(sgid);
					delete grp;
				}
				return PILO_OK;
			}

			::pilo::err_t service_manager::add_group(service_group_interface* grp)
			{
				if (_group.find(grp->id()) != _group.cend()) {
					return ::pilo::mk_perr(PERR_EXIST);
				}
				_group[grp->id()] = grp;
				return PILO_OK;
			}

			::pilo::err_t service_manager::start()
			{
				::pilo::err_t err = PILO_OK;
				std::map<::pilo::service_group_id, service_group_interface*>::const_iterator cit = _group.cbegin();
				for (; cit != _group.cend(); cit++) {
					err = cit->second->start(PILO_TIMESTAMP);
					if (err != PILO_OK)
						return err;
				}
				return err;
			}

			::pilo::err_t service_manager::stop()
			{
				::pilo::err_t err = PILO_OK;
				std::map<::pilo::service_group_id, service_group_interface*>::reverse_iterator it = _group.rbegin();

				if (all_stopped()) {
					return ::pilo::mk_perr(PERR_NOOP);
				}
				
				for (; it != _group.rend(); it++) {
					if (!it->second->all_stopped()) {
						err = it->second->stop(PILO_TIMESTAMP);
						if (err != PILO_OK)
							return err;
					}									
				}

				bool all_stopped = false;
				while (! all_stopped) {
					if (_sched_queue.size_approx() <= 0) {
						break;
					}
					::pilo::core::threading::xpf_msleep(100);
				}

				return PILO_OK;
			}

			bool service_manager::all_stopped() const
			{
				std::map<::pilo::service_group_id, service_group_interface*>::const_reverse_iterator it = _group.crbegin();
				for (; it != _group.crend(); it++) {
					if (!it->second->all_stopped()) {
						return false;
					}
				}
				if (_sched_queue.size_approx() > 0)
					return false;

				return true;
			}

			void service_manager::pulse()
			{
				service_interface* iservice = nullptr;
				if (_sched_queue.try_dequeue(iservice)) {
					if (iservice->is_running()) {
						iservice->check_pulse(PILO_TIMESTAMP);
						this->_sched_queue.enqueue(iservice);
						return;
					}
					else {
						iservice->set_stoppped();
						iservice->on_stopped();
					}
				}
			}
			service_group_interface* service_manager::group(::pilo::service_group_id gid)
			{
				std::map<::pilo::service_group_id, service_group_interface*>::const_iterator cit = _group.find(gid);
				if (cit != _group.cend())
					return cit->second;
				return nullptr;
			}

			service_interface* service_manager::service(::pilo::service_group_id gid, ::pilo::service_id key_or_index)
			{
				service_group_interface* grp = group(gid);
				if (grp != nullptr) {
					return grp->service(key_or_index);
				}
				return nullptr;
			}
		}
	}
}