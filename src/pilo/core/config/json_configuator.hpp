#ifndef _pilo_core_config_json_configuator_h_
#define _pilo_core_config_json_configuator_h_

#include "./configuator_interface.hpp"
#include "../io/file.hpp"
#include "../../tlv.hpp"
#include "../../external/rapidjson/document.h"

namespace pilo {
    namespace core {
        namespace config {
        
        
            class json_configuator : public configuator_interface
            {
            public:
                json_configuator() : _m_file_ptr(nullptr), _m_root_value(nullptr) {}
                json_configuator(const::pilo::core::io::path* path_ptr);
                ~json_configuator();


            public:
                // Inherited via configuator_interface
                ::pilo::err_t load() override;
                ::pilo::err_t load(const char* data, ::pilo::i64_t len) override;
                ::pilo::err_t save() override;
                ::pilo::tlv* root() override;
                ::pilo::tlv* get_value(const char* fqn, ::pilo::err_t& err) override;

            public:
                void set_file(const::pilo::core::io::path* path_ptr);


            protected:
                ::pilo::err_t _parse_json_object(::rapidjson::Value & obj, ::pilo::tlv* parent_tlv);
                ::pilo::err_t json_configuator::_parse_json_array(::rapidjson::Value& obj, ::pilo::tlv* parent_tlv);
            protected:
                ::pilo::core::io::file<>  *_m_file_ptr;
                ::pilo::tlv* _m_root_value;

            private:
            PMC_DISABLE_COPY(json_configuator)

                

            };
        
        }
    }
}


#endif // !_pilo_core_config_json_reader_h_
