#pragma once



namespace pilo
{
    namespace core
    {
        namespace memory
        {
            class byte_buffer_interface;

            class serializable_header_interface
            {
            public:
                virtual ~serializable_header_interface() {}

                virtual ::pilo::err_t begin_serializing(byte_buffer_interface * byte_buffer) = 0;
                virtual ::pilo::err_t end_serializing(byte_buffer_interface * byte_buffer, ::pilo::i64_t header_pos, ::pilo::i64_t total_len) = 0;
                virtual ::pilo::err_t begin_deserializing(byte_buffer_interface * byte_buffer) = 0;
                virtual ::pilo::err_t end_deserializing(byte_buffer_interface * byte_buffer, ::pilo::i64_t actual_read_bytes) = 0;
                virtual ::pilo::i64_t header_length() const = 0;
                virtual ::pilo::i64_t body_length() const= 0;
                virtual ::pilo::err_t set_body_length(::pilo::i64_t body_len) = 0;
                virtual std::string to_string() = 0;
                virtual const char* to_cstring(char* buffer, ::pilo::i64_t sz) = 0;
            };
        }
    }
}