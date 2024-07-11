#include "../../pilo.hpp"
#include "../../tlv.hpp"
#include "./device_interface.hpp"
#include "path.hpp"


namespace pilo
{
    namespace core
    {
        namespace io
        {
            class file : public device_interface
            {
            public:
                file();
                virtual ~file();
                file(file&& other) noexcept;
                file& operator=(file&& other) noexcept;

            public:
                // Inherited via device_interface
                virtual ::pilo::err_t initialize(::pilo::tlv* param) override;
                virtual ::pilo::err_t open(const char* path_str, creation_mode cm, access_permission perm, predefined_pilo_dir_enum prefix, dev_open_flags f) override;
                virtual ::pilo::err_t open(const ::pilo::core::io::path* p, creation_mode cm, access_permission perm, dev_open_flags f) override;
                virtual ::pilo::err_t open(const char* path_str, creation_mode cm, access_permission perm, dev_open_flags f) override;
                virtual ::pilo::err_t read(char* buffer, ::pilo::i64_t capacity, ::pilo::i64_t rbs, ::pilo::i64_t* n_read) override;
                virtual ::pilo::err_t read(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t rbs, ::pilo::i64_t* n_read) override;
                virtual ::pilo::err_t write(const char* buffer, ::pilo::i64_t wbs, ::pilo::i64_t* n_written) override;
                virtual ::pilo::err_t write(::pilo::core::memory::byte_buffer_interface* buf, ::pilo::i64_t wbs, ::pilo::i64_t* n_written) override;
                virtual ::pilo::err_t seek(seek_whence whence, ::pilo::i64_t off) override;
                virtual ::pilo::err_t control(::pilo::i32_t cmd, ::pilo::tlv* param) override;
                virtual ::pilo::err_t sync(::pilo::i32_t mode) override;
                virtual ::pilo::err_t close() override;
                virtual ::pilo::err_t finalize() override;
                virtual ::pilo::err_t exist() const override;
                ::pilo::err_t remove() override;

            protected:
                ::pilo::err_t _open(creation_mode cm, access_permission perm, dev_open_flags f);
                ::pilo::err_t _pre_open() const;


            private:
                ::pilo::core::io::path      _m_path;
                ::pilo::os_file_handle_t    _m_fd;

                PMC_DISABLE_COPY(file)    
                    
            };
        }
    }
}