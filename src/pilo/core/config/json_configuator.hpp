#ifndef _pilo_core_config_json_configuator_h_
#define _pilo_core_config_json_configuator_h_

#include "./configuator_interface.hpp"
#include "../io/file.hpp"

namespace pilo {
    namespace core {
        namespace config {
        
        
            class json_configuator : public configuator_interface
            {
            public:
                json_configuator() : _m_file_ptr(nullptr) {}
                json_configuator(const::pilo::core::io::path* path_ptr);
                

            public:
                // Inherited via configuator_interface
                ::pilo::err_t load() override;
                ::pilo::err_t load(const char* data, ::pilo::i64_t len) override;
                ::pilo::err_t save() override;

            public:
                void set_file(const::pilo::core::io::path* path_ptr);



            
            protected:
                ::pilo::core::io::file<>  *_m_file_ptr;


            private:
            PMC_DISABLE_COPY(json_configuator)

                
                
            };
        
        }
    }
}


#endif // !_pilo_core_config_json_reader_h_
