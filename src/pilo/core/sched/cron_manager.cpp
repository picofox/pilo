#include "cron_manager.hpp"
#include <thread>
#include "../process/context.hpp"
#include "../logging/logger_interface.hpp"

void pilo::core::sched::cron_manager::s_cron_timer_func(::pilo::core::sched::task* t)
{
	cron_manager* m = (cron_manager*)(t->object());
	m->pulse();
}


void pilo::core::sched::cron_manager::start()
{
	PILO_CONTEXT->add_rel_sec_timer(1,::pilo::core::sched::timer::infinit_repeat,1, cron_manager::s_cron_timer_func,this, nullptr, nullptr);
}

void pilo::core::sched::cron_manager::finanlize()
{
	if (_timer_id > 0)
		PILO_CONTEXT->delete_timer(_timer_id);
	std::map<::pilo::i8_t, cron*>::const_iterator it = _crons.cbegin();
	for (; it != _crons.cend(); it++) {
		it->second->finalize();
		delete it->second;
	}
	_crons.clear();
}

void pilo::core::sched::cron_manager::pulse()
{
	std::lock_guard<lock_type>	guard(this->_mutex);
	std::map<::pilo::i8_t, cron*>::const_iterator it = _crons.cbegin();
	for (; it != _crons.cend(); it++) {
		it->second->pulse();
	}

}

::pilo::i64_t pilo::core::sched::cron_manager::add_job(::pilo::i8_t tz, const std::string& spec, task_func_type f_func, void* obj, void* param, task_destructor_func_type dtor)
{
	std::lock_guard<lock_type>	guard(this->_mutex);

	std::map<::pilo::i8_t, cron*>::const_iterator it = _crons.find(tz);
	if (it != _crons.end()) {
		return it->second->add_job(spec, f_func, obj, param, dtor);
	}

	cron* c = new cron(tz, this);
	::pilo::i64_t id = c->add_job(spec, f_func, obj, param, dtor);
	if (id < 0) {
		delete c;
		return id;
	}
	_crons.insert(std::pair<::pilo::i8_t, cron*>(tz,c));
	return id;
}

::pilo::err_t pilo::core::sched::cron_manager::delete_job(::pilo::i64_t cron_id)
{
	std::lock_guard<lock_type>	guard(this->_mutex);
	::pilo::i8_t tzkey = (::pilo::i8_t)((cron_id >> 48) & 0xFFFF);
	std::map<::pilo::i8_t, cron*>::const_iterator it = _crons.find(tzkey);
	if (it == _crons.end()) {
		return ::pilo::mk_perr(PERR_NON_EXIST);
	}
	return it->second->delete_job_async(cron_id);

}
