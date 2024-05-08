#pragma once

#include "../../pilo.hpp"

namespace pilo
{
    namespace core
    {
        namespace io
        {
            /**
            * @brief                        formatted str content output to c file stream.
            * @param fp [in]                c file stream pointer to output to.
            * @param fmt [in]               fmt string like printf.
            * @param ... [in]               the var args.
            * @return                       the chars count outputted.
            * @see                          pea_formatted_output   pea_string_format_output
            * @attention                    [NPC] [RC] [MTS]
            */
            ::pilo::i64_t file_formatted_output(FILE* fp, const char* fmt, ...);
            ::pilo::i64_t file_formatted_output(FILE* fp, const wchar_t* fmt, ...);

            /**
            * @brief                        formatted str content output to str buffer.
            * @param buf [out]              str buf to output to.
            * @param len [in]               buffer capacity by chars.
            * @param fmt [in]               fmt string like printf.
            * @param ... [in]               the var args.
            * @return                       the chars count outputted
            * @see                          pea_formatted_output   pea_string_format_output
            * @attention                    [NPC] [RC] [MTS]
            */
            ::pilo::i64_t string_formated_output(char* buf, ::pilo::i64_t len, const char* fmt, ...);
            ::pilo::i64_t string_formated_output(wchar_t* buf, ::pilo::i64_t len, const wchar_t* fmt, ...);

            template<typename CHAR_T>
            void file_buffer_output(FILE* fp, const CHAR_T* buffer, ::pilo::i64_t len, const CHAR_T* extra = nullptr )
            {
                for (::pilo::i64_t i = 0; i < len; i++)
                {
                    putc(buffer[i], fp);                    
                }
                if (extra)
                    file_formatted_output(fp, "%s", extra);
            }

        }
    }
}
