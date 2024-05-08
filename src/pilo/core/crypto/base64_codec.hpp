#pragma once

#include "../../pilo.hpp"
#include "./codec_interface.hpp"

namespace pilo 
{
    namespace core
    {
        namespace crypto
        {
            class base64_codec
                : public codec_interface
                , public static_codec<base64_codec>
                
            {
            public:
                virtual ::pilo::i64_t encoding_size(const char* src, ::pilo::i64_t src_len) const;
                virtual ::pilo::i64_t decoding_size(const char* src, ::pilo::i64_t src_len) const;
                virtual char* encode(char* buffer, ::pilo::i64_t capa, ::pilo::i64_t& rlen, const char* src, ::pilo::i64_t src_len, ::pilo::err_t* err, const char* key, ::pilo::i32_t key_len);
                virtual char* decode(char* buffer, ::pilo::i64_t capa, ::pilo::i64_t& rlen, const char* src, ::pilo::i64_t src_len, ::pilo::err_t* err, const char* key, ::pilo::i32_t key_len);
            };
        }
    }
}