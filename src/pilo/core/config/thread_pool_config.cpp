#include "thread_pool_config.hpp"
#include "../process/context.hpp"

::pilo::i32_t pilo::core::config::thread_pool_config::workers_count() const
{
	if (_workers_count < 0) {
		return PILO_CONTEXT->system_information()->num_logical_cores();
	}
	else if (_workers_count > 65535) {
		return 65535;
	}
	return _workers_count;
}

::pilo::i32_t pilo::core::config::thread_pool_config::task_executor_count() const
{
	if (_task_executor_count < 0) {
		return 1;
	}
	else if (_task_executor_count > workers_count()) {
		return workers_count() - 1;
	}
	return _task_executor_count;
}

::pilo::err_t pilo::core::config::thread_pool_config::load()
{
	return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
}

::pilo::err_t pilo::core::config::thread_pool_config::save()
{
	return ::pilo::mk_perr(PERR_OP_UNSUPPORT);
}

::pilo::err_t pilo::core::config::thread_pool_config::set_default()
{
	_performance_mode = false;
	_pulse_delay_usec = 10000;
	_workers_count = -1;
	_task_executor_count = -1;
	_name = "global";
	return PILO_OK;
}

void pilo::core::config::thread_pool_config::clear(bool )
{
	_performance_mode = false;
	_pulse_delay_usec = 0;
	_workers_count = 0;
	_task_executor_count = 0;
	_name.clear();
}

::pilo::err_t pilo::core::config::thread_pool_config::save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver)
{
	::pilo::err_t err = PILO_OK;
	::pilo::core::memory::object_array<char, 128>   cb;

	::pilo::i32_t fqdn_len = (::pilo::i32_t) ::pilo::core::string::character_count(fqdn_path);
	cb.check_space(fqdn_len + 48);
	::pilo::core::string::n_copyz(cb.begin(), cb.capacity(), fqdn_path, fqdn_len);
	cb.set_size(fqdn_len);

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".performance_mode");
	PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), this->_performance_mode, true));

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".global_task_queue");
	PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), this->_global_task_queue, true));

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".pulse_delay_usec");
	PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), this->_pulse_delay_usec, true));

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".workers_count");
	PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), this->_workers_count, true));

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".task_executor_count");
	PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), this->_task_executor_count, true));

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".task_dequeue_block_usec");
	PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), this->_task_dequeue_block_usec, true));

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".name");
	PILO_CHKERR_RET(err, configuator_driver->set_value(cb.begin(), this->_name.c_str(), (int)this->_name.size(), false, true, true));

	return PILO_OK;
}

::pilo::err_t pilo::core::config::thread_pool_config::load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver)
{
	::pilo::err_t err = PILO_OK;
	::pilo::core::memory::object_array<char, 128>   cb;

	::pilo::i32_t fqdn_len = (::pilo::i32_t) ::pilo::core::string::character_count(fqdn_path);
	cb.check_space(fqdn_len + 48);
	::pilo::core::string::n_copyz(cb.begin(), cb.capacity(), fqdn_path, fqdn_len);
	cb.set_size(fqdn_len);

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".performance_mode");
	err = configuator_driver->get_value(cb.begin(), this->_performance_mode);
	if (err != PILO_OK)
		return err;

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".global_task_queue");
	err = configuator_driver->get_value(cb.begin(), this->_global_task_queue);
	if (err != PILO_OK)
		return err;

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".pulse_delay_usec");
	err = configuator_driver->get_value(cb.begin(), this->_pulse_delay_usec);
	if (err != PILO_OK)
		return err;

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".workers_count");
	err = configuator_driver->get_value(cb.begin(), this->_workers_count);
	if (err != PILO_OK)
		return err;

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".task_executor_count");
	err = configuator_driver->get_value(cb.begin(), this->_task_executor_count);
	if (err != PILO_OK)
		return err;

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".task_dequeue_block_usec");
	err = configuator_driver->get_value(cb.begin(), this->_task_dequeue_block_usec);
	if (err != PILO_OK)
		return err;

	::pilo::core::string::copyz(cb.ptr(), cb.space_available(), ".name");
	err = configuator_driver->get_value(cb.begin(), this->_name);
	if (err != PILO_OK)
		return err;

	return PILO_OK;
}


const::pilo::core::io::path* pilo::core::config::thread_pool_config::file_path() const
{
	return nullptr;
}

bool pilo::core::config::thread_pool_config::invalid() const
{
	if (_name.empty()) {
		return false;
	}
	return true;
}
