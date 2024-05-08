#include "../../pilo.hpp"
#include "base64_codec.hpp"
#include "../string/string_operation.hpp"

namespace pilo
{
    namespace core
    {
        namespace crypto
        {
            char stc_base64_map[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
                     'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
                     'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
                     'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '+', '/' };

            int stc_base64_map_inverse[] = { 62, -1, -1, -1, 63, 52, 53, 54, 55, 56, 57, 58,
                    59, 60, 61, -1, -1, -1, -1, -1, -1, -1, 0, 1, 2, 3, 4, 5,
                    6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20,
                    21, 22, 23, 24, 25, -1, -1, -1, -1, -1, -1, 26, 27, 28,
                    29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42,
                    43, 44, 45, 46, 47, 48, 49, 50, 51 };


            ::pilo::i64_t base64_codec::encoding_size(const char* , ::pilo::i64_t src_len) const
            {
                if (src_len == 0)
                    return 0;

                ::pilo::i64_t ret = src_len;
                if (src_len % 3 != 0)
                    ret += 3 - (src_len % 3);
                ret /= 3;
                ret *= 4;
                return ret;
            }

            ::pilo::i64_t base64_codec::decoding_size(const char* in, ::pilo::i64_t len) const
            {
                size_t ret;
                size_t i;
                if (in == nullptr || len == 0)
                    return 0;
                ret = len / 4 * 3;
                for (i = len; i-- > 0; ) {
                    if (in[i] == '=') {
                        ret--;
                    }
                    else {
                        break;
                    }
                }
                return ret;
            }

            char* base64_codec::encode(char* buffer, ::pilo::i64_t capa, ::pilo::i64_t& rlen, const char* src, ::pilo::i64_t src_len, ::pilo::err_t* err, const char* , ::pilo::i32_t )
            {
                if (src == nullptr)
                {
                    if (err != nullptr) *err = ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                    return nullptr;
                }
                else if (src_len < 0)
                {
                    src_len = ::pilo::core::string::character_count(src);
                }
                
                char* ret = buffer;
                rlen = encoding_size(src, src_len);
                if (capa < rlen +1)
                {
                    ret = (char*)PMF_HEAP_MALLOC(rlen +1);
                    if (ret == nullptr)
                    {
                        if (err != nullptr) *err = ::pilo::make_core_error(PES_MEM, PEP_INSUFF);
                        return nullptr;
                    }
                    capa = rlen + 1;
                }

                if (src_len <= 0)
                {
                    rlen = 0;
                    return buffer;
                }
                ret[rlen] = 0;
                ::pilo::i64_t i = 0;
                ::pilo::i64_t j = 0;
                ::pilo::i64_t v = 0;
                for (i = 0, j = 0; i < src_len; i += 3, j += 4) {
                    v = src[i];
                    v = i + 1 < src_len ? v << 8 | src[i + 1] : v << 8;
                    v = i + 2 < src_len ? v << 8 | src[i + 2] : v << 8;

                    ret[j] = stc_base64_map[(v >> 18) & 0x3F];
                    ret[j + 1] = stc_base64_map[(v >> 12) & 0x3F];
                    if (i + 1 < src_len) {
                        ret[j + 2] = stc_base64_map[(v >> 6) & 0x3F];
                    }
                    else {
                        ret[j + 2] = '=';
                    }
                    if (i + 2 < src_len) {
                        ret[j + 3] = stc_base64_map[v & 0x3F];
                    }
                    else {
                        ret[j + 3] = '=';
                    }
                }

                return ret;
            }

            char* base64_codec::decode(char* buffer, ::pilo::i64_t capa, ::pilo::i64_t& rlen, const char* src, ::pilo::i64_t src_len,  ::pilo::err_t* err, const char* , ::pilo::i32_t )
            {
                if (src == nullptr)
                {
                    if (err != nullptr) *err = ::pilo::make_core_error(PES_PARAM, PEP_IS_NULL);
                    return nullptr;
                }
                else if (src_len < 0)
                {
                    src_len = ::pilo::core::string::character_count(src);
                }

                if (src_len <= 0)
                {
                    rlen = 0;
                    return buffer;
                }

                char* ret = buffer;
                rlen = decoding_size(src, src_len);
                if (capa < rlen + 1)
                {
                    ret = (char*)PMF_HEAP_MALLOC(rlen + 1);
                    if (ret == nullptr)
                    {
                        if (err != nullptr) *err = ::pilo::make_core_error(PES_MEM, PEP_INSUFF);
                        return nullptr;
                    }
                    capa = rlen + 1;
                }

                ::pilo::i64_t i = 0;
                ::pilo::i64_t j = 0;
                ::pilo::i64_t v = 0;

                for (i = 0, j = 0; i < src_len; i += 4, j += 3) {
                    v = stc_base64_map_inverse[src[i] - 43];
                    v = (v << 6) | stc_base64_map_inverse[src[i + 1] - 43];
                    v = src[i + 2] == '=' ? v << 6 : (v << 6) | stc_base64_map_inverse[src[i + 2] - 43];
                    v = src[i + 3] == '=' ? v << 6 : (v << 6) | stc_base64_map_inverse[src[i + 3] - 43];

                    ret[j] = (v >> 16) & 0xFF;
                    if (src[i + 2] != '=')
                        ret[j + 1] = (v >> 8) & 0xFF;
                    if (src[i + 3] != '=')
                        ret[j + 2] = v & 0xFF;
                }
                ret[rlen] = 0;
                return ret;
            }
        }
    }
}