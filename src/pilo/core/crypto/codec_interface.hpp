#pragma once

#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace crypto
        {
            class codec_interface
            {
            public:
                virtual void set_encoding_key(const char* , ::pilo::i32_t ) {}
                virtual const char* encoding_key(::pilo::i32_t&) const { return nullptr;  }
                virtual void set_decoding_key(const char*, ::pilo::i32_t) {}
                virtual const char* decoding_key(::pilo::i32_t&) const { return nullptr; }
                virtual ::pilo::i64_t encoding_size(const char* src, ::pilo::i64_t src_len) const = 0;
                virtual ::pilo::i64_t decoding_size(const char* src, ::pilo::i64_t src_len) const = 0;
                virtual char* encode(char* buffer, ::pilo::i64_t capa, ::pilo::i64_t& rlen, const char* src, ::pilo::i64_t src_len, ::pilo::err_t* err, const char* key, ::pilo::i32_t key_len) = 0;
                virtual char* decode(char* buffer, ::pilo::i64_t capa, ::pilo::i64_t& rlen, const char* src, ::pilo::i64_t src_len, ::pilo::err_t* err, const char* key, ::pilo::i32_t key_len) = 0;
            };

            template <typename T>
            class static_codec
            {
            public:
                typedef  T       codec_type;

                inline static codec_type* codec()
                {
                    static codec_type _codec;
                    return &_codec;
                }

                inline static char* s_encode(char* buffer, ::pilo::i64_t capa, ::pilo::i64_t& rlen, const char* src, ::pilo::i64_t src_len, ::pilo::err_t* err, const char* key, ::pilo::i32_t key_len)
                {
                    codec_type* c = static_codec::codec();                    
                    if (c != nullptr)
                    {
                        return c->encode(buffer, capa, rlen, src, src_len, err, key, key_len);
                    }
                    return nullptr;                                      
                }
                inline static char* s_decode(char* buffer, ::pilo::i64_t capa, ::pilo::i64_t& rlen, const char* src, ::pilo::i64_t src_len, ::pilo::err_t* err, const char* key, ::pilo::i32_t key_len)
                {
                    codec_type* c = static_codec::codec();                    
                    if (c != nullptr)
                    {
                        return c->decode(buffer, capa, rlen, src, src_len, err, key, key_len);
                    }
                    return nullptr;                                       
                }
            };
        }
    }
}