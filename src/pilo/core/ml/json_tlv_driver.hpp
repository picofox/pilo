#ifndef _pilo_core_config_json_tlv_driver_h_
#define _pilo_core_config_json_tlv_driver_h_

#include "./tlv_driver_interface.hpp"

#include "../../external/rapidjson/document.h"
#include "../../tlv.hpp"

namespace pilo {

    class tlv;

    namespace core {
        namespace ml {
        
        
            class json_tlv_driver : public tlv_driver_interface
            {
            public:
                json_tlv_driver() : _m_root_value(nullptr) {}
                virtual ~json_tlv_driver();


            public:
                // Inherited via tlv_driver_interface
                ::pilo::err_t load(const::pilo::core::io::path* path_ptr) override;
                ::pilo::err_t load(const char* data, ::pilo::i64_t len) override;
                ::pilo::err_t save(const ::pilo::core::io::path* dest_path) override;
                ::pilo::tlv* root() override;
                ::pilo::tlv* get_value_node(const char* fqn, ::pilo::err_t& err) override;

                ::pilo::err_t set_value(const char* fqn, bool is_force);
                ::pilo::err_t set_value(const char* fqn, bool iv, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::i8_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::i16_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::i32_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::i64_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::u8_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::u16_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::u32_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::u64_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::f32_t v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, ::pilo::f64_t v, bool is_force);            
                ::pilo::err_t set_value(const char* fqn, const std::string & v, bool is_force);
                ::pilo::err_t set_value(const char* fqn, const char* value, ::pilo::i32_t len, bool adopt, bool is_cstr, bool is_force);

            public:
                




            protected:
                template<typename T>
                ::pilo::err_t _set_trivil_type(const char* fqn, T&& value, bool is_force)
                {
                    ::pilo::err_t err = PILO_OK;
                    if (_m_root_value == nullptr) {
                        _m_root_value = ::pilo::tlv::allocate();
                    }

                    if (fqn == nullptr || *fqn == 0)
                    {
                        return _m_root_value->set(value);
                    }

                    if (!is_force)
                    {
                        ::pilo::tlv* tmp = _m_root_value->get_tlv<32>(fqn, err);
                        if (tmp != nullptr)
                        {
                            return ::pilo::mk_perr(PERR_EXIST);
                        }
                        err = PILO_OK;
                    }

                    ::pilo::tlv* t = _m_root_value->set_tlv<32>(fqn, err);
                    if (t != nullptr)
                    {
                        err = t->set(value);
                    }

                    return err;
                }




                ::pilo::err_t _write_json_object(::rapidjson::Value& obj, const ::pilo::tlv* tlvp, ::rapidjson::Document::AllocatorType & allocator);
                ::pilo::err_t _parse_json_object(::rapidjson::Value & obj, ::pilo::tlv* parent_tlv);
                ::pilo::err_t _parse_json_array(::rapidjson::Value& obj, ::pilo::tlv* parent_tlv);


            protected:

                ::pilo::tlv* _m_root_value;

            private:
            PMC_DISABLE_COPY(json_tlv_driver)

   

                // Inherited via tlv_driver_interface
                ::pilo::tlv* make_value_node(const char* fqn, ::pilo::err_t& err) override;

            };
        
        }
    }
}


#endif // !_pilo_core_config_json_reader_h_
