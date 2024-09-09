#ifndef _pilo_core_config_tlv_driver_interface_h_
#define _pilo_core_config_tlv_driver_interface_h_


#include "../../pilo.hpp"
#include "../rtti/wired_type.hpp"


namespace pilo {

    class tlv;

    namespace core {

        namespace io {
            class path;
        }

        namespace ml {

            enum class driver_type : ::pilo::i32_t
            {
                json = 0,
            };

            class tlv_driver_interface
            {
            public:
                virtual ~tlv_driver_interface() {};
                virtual ::pilo::err_t load(const::pilo::core::io::path* path_ptr) = 0;
                virtual ::pilo::err_t load(const char* data, ::pilo::i64_t len) = 0;
                virtual ::pilo::err_t save(const ::pilo::core::io::path * dest_path) = 0;
                virtual ::pilo::tlv* root() = 0;
                virtual ::pilo::tlv* get_value_node(const char* fqn, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, bool iv, bool is_force) = 0 ;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::i8_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::i16_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::i32_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::i64_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::u8_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::u16_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::u32_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::u64_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::f32_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, ::pilo::f64_t v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, const std::string& v, bool is_force) = 0;
                virtual ::pilo::err_t set_value(const char* fqn, const char* value, ::pilo::i32_t len, bool adopt, bool is_cstr, bool is_force) = 0;


            public:
                virtual ::pilo::tlv* make_value_node(const char* fqn, ::pilo::err_t& err) = 0;
                virtual ::pilo::err_t get_value(const char* fqn, ::pilo::i8_t& v);
                virtual ::pilo::err_t get_value(const char* fqn, ::pilo::u8_t& v);
                virtual ::pilo::err_t get_value(const char* fqn, ::pilo::i16_t& v);
                virtual ::pilo::err_t get_value(const char* fqn, ::pilo::u16_t& v);
                virtual ::pilo::err_t get_value(const char* fqn, ::pilo::i32_t& v);
                virtual ::pilo::err_t get_value(const char* fqn, ::pilo::u32_t& v);
                virtual  ::pilo::err_t get_value(const char* fqn, ::pilo::i64_t& v);
                virtual ::pilo::err_t get_value(const char* fqn, ::pilo::u64_t& v);
                virtual ::pilo::err_t get_value(const char* fqn, ::pilo::f32_t& v);
                virtual ::pilo::err_t get_value(const char* fqn, ::pilo::f64_t& v);
                virtual ::pilo::err_t get_value(const char* fqn, std::string& v);
                virtual ::pilo::err_t get_value(const char* fqn, bool& v);
                virtual ::pilo::err_t get_value(const char* fqn, const char*& cstr, ::pilo::i32_t& len, bool* is_bytes_as_cstr);

            };
        }
    }
}

#endif // !_pilo_core_config_config_interface_h_  
