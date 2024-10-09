#ifndef _pilo_core_config_service_config_h_
#define _pilo_core_config_service_config_h_

#include "../logging/logger_def.hpp"
#include "../io/path.hpp"
#include "../ml/json_tlv_driver.hpp"
#include "./configuation_interface.hpp"


namespace pilo 
{
    namespace core 
    {
        namespace config 
        {
            class service_config : public configuation_interface
            {
            public:
                service_config()
                    : _name("")
                    , _pulse_msec(SP_PMI_SERVICE_PULSE_DEFAULT)
                    , _type_id(PMI_INVALID_SERVICE_GROUP_ID)
                    , _param(nullptr)
                {
                    
                }

                service_config(const char* name, ::pilo::i64_t pulse_msec, ::pilo::i16_t svc_type_id, ::pilo::tlv* param)
                    : _name(name)
                    , _pulse_msec(pulse_msec)
                    , _type_id(svc_type_id)
                    , _param(param)
                {

                }

                service_config(const service_config& o)
                {
                    _name = o._name;
                    _pulse_msec = o._pulse_msec;
                    _type_id = o._type_id;
                    if (o._param != nullptr)
                        _param = o._param->clone();
                    else
                        _param = nullptr;
                }

                service_config& operator=(const service_config& o)
                {
                    if (this != &o) {
                        _name = o._name;
                        _pulse_msec = o._pulse_msec;
                        _type_id = o._type_id;
                        if (o._param != nullptr)
                            _param = o._param->clone();
                        else
                            _param = nullptr;
                    }
                    return *this;
                }

                service_config(service_config&& o) noexcept
                {
                    _name = std::move(o._name);
                    _pulse_msec = o._pulse_msec;
                    _type_id = o._type_id;
                    if (o._param != nullptr) {
                        _param = o._param;
                        o._param = nullptr;
                    }
                    else {
                        _param = nullptr;
                    }
                }

                service_config& operator=(service_config&& o) noexcept
                {
                    if (this != &o) {
                        _name = std::move(o._name);
                        _pulse_msec = o._pulse_msec;
                        _type_id = o._type_id;
                        if (o._param != nullptr) {
                            _param = o._param;
                            o._param = nullptr;
                        }
                        else {
                            _param = nullptr;
                        }
                    }
                    return *this;
                }


                ~service_config()
                {
                    if (_param != nullptr) {
                        ::pilo::tlv::deallocate(_param);
                        _param = nullptr;
                    }
                }

                // Inherited via configuation_interface
                ::pilo::err_t load() override;
                ::pilo::err_t save() override;
                ::pilo::err_t set_default() override;
                void clear(bool purge) override;
                ::pilo::err_t save_to_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver) override;
                ::pilo::err_t load_from_configurator(const char* fqdn_path, ::pilo::core::ml::tlv_driver_interface* configuator_driver) override;
                const::pilo::core::io::path* file_path() const override;
                bool invalid() const override;

                inline ::pilo::i64_t pulse_msec() const {
                    return _pulse_msec;
                }

                inline ::pilo::i16_t type_id() const { return _type_id;  }
                inline void set_type_id(::pilo::i16_t id) { _type_id = id;  }
                inline const std::string& name() const { return _name; }
                inline const ::pilo::tlv* param() const { return _param;  }

            private:
                std::string     _name;
                ::pilo::i64_t	_pulse_msec;
                ::pilo::i16_t   _type_id;
                ::pilo::tlv*    _param;
            };
        }
    }
}


#endif