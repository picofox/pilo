#pragma once
#include "../../pilo.hpp"

namespace pilo {
    namespace core {
        namespace i18n {
            /**
            * @brief                        convert gbk encoded string to unicode string. (os dependent, win: ucs-2LE, linux ucs-4LE) use caller-supplied buffer.
            * @param dst_ucs_buffer [out]   buffer to hold unicode string after conversion.
            * @param cc_dst_ucs_buffer [in] the size of destination buffer in char count.
            * @param src_gbk_str [in]       the source gbk string.
            * @param src_gbk_len [in]       the length of source gbk string in chars.
            * @return                       the destination size after conversion in chars including terminated-0.
            * @see                          os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            ::pilo::i64_t	gbk_to_os_unicode_fixed(wchar_t* dst_ucs_buffer, ::pilo::i64_t cc_dst_ucs_buffer, const char* src_gbk_str, ::pilo::i64_t src_gbk_len);

            /**
            * @brief                        convert gbk encoded string to unicode string. (os dependent, win: ucs-2LE, linux ucs-4LE), use dynamic heap allocation.
            * @param src_gbk_str [in]       the source gbk string.
            * @param src_gbk_len [in]       the length of source gbk string in chars.
            * @param ret_cc_ucs [out]       the destination size after conversion in chars including terminated-0.
            * @return                       the buffer to hold destination unicode string after conversion.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            wchar_t* gbk_to_os_unicode_neo(const char* src_gbk_str, ::pilo::i64_t src_gbk_len, ::pilo::i64_t* ret_cc_ucs);

            /**
            * @brief                        convert unicode encoded string (os dependent, win: ucs-2LE, linux ucs-4LE) to gbk string. use caller-supplied buffer.
            * @param dst_gbk_buffer [out]   buffer to hold gbk string after conversion.
            * @param cc_dst_gbk_buffer [in] the size of destination buffer in chars.
            * @param src_ucs_str [in]       the source unicode string.
            * @param src_ucs_len [in]       the length of source unicode string in chars.
            * @return                       the destination size after conversion in chars including terminated-0.
            * @see                          gbk_to_os_unicode_fixed  os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            ::pilo::i64_t os_unicode_to_gbk_fixed(char* dst_gbk_buffer, ::pilo::i64_t cc_dst_gbk_buffer, const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len);

            /**
            * @brief                        convert unicode encoded string (os dependent, win: ucs-2LE, linux ucs-4LE) to unicode string.  use dynamic heap allocation.
            * @param src_ucs_str [in]       the source unicode string.
            * @param src_ucs_len [in]       the length of source unicode string in chars.
            * @param ret_cc_gbk [out]       the destination size after conversion in chars including terminated-0.
            * @return                       the buffer to hold destination gbk string after conversion.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            char* os_unicode_to_gbk_neo(const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len, ::pilo::i64_t* ret_cc_gbk);

            /**
            * @brief                        convert unicode encoded string (os dependent, win: ucs-2LE, linux ucs-4LE) to utf8 string. use caller-supplied buffer.
            * @param dst_utf_buffer [out]   buffer to hold utf8 string after conversion.
            * @param cc_dst_utf_buffer [in] the size of destination buffer in chars.
            * @param src_ucs_str [in]       the source unicode string.
            * @param src_ucs_len [in]       the length of source unicode string in chars.
            * @return                       the destination size after conversion in chars including terminated-0.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            ::pilo::i64_t os_unicode_to_utf8_fixed(char* dst_utf_buffer, ::pilo::i64_t cc_dst_utf_buffer, const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len);
            
            /**
            * @brief                        convert unicode encoded string (os dependent, win: ucs-2LE, linux ucs-4LE) to unicode string.  use dynamic heap allocation.
            * @param src_ucs_str [in]       the source unicode string.
            * @param src_ucs_len [in]       the length of source unicode string in chars.
            * @param ret_cc_utf [out]       the destination size after conversion in chars including terminated-0.
            * @return                       the buffer to hold destination utf string after conversion.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo utf8_to_os_unicode_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            char* os_unicode_to_utf8_neo(const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len, ::pilo::i64_t* ret_cc_utf);

            /**
            * @brief                        convert utf encoded string to unicode string. (os dependent, win: ucs-2LE, linux ucs-4LE) use caller-supplied buffer.
            * @param dst_ucs_buffer [out]   buffer to hold unicode string after conversion.
            * @param cc_dst_ucs_buffer [in] the size of destination buffer in char count.
            * @param src_utf_str [in]       the source utf8 string.
            * @param src_utf_len [in]       the length of source utf8 string in chars.
            * @return                       the destination size after conversion in chars including terminated-0.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            ::pilo::i64_t utf8_to_os_unicode_fixed(wchar_t* dst_ucs_buffer, ::pilo::i64_t cc_dst_ucs_buffer, const char* src_utf_str, ::pilo::i64_t src_utf_len);

            /**
            * @brief                        convert utf8 encoded string to unicode string. (os dependent, win: ucs-2LE, linux ucs-4LE), use dynamic heap allocation.
            * @param src_utf_str [in]       the source utf8 string.
            * @param src_utf_len [in]       the length of source utf8 string in chars.
            * @param ret_cc_ucs [out]       the destination size after conversion in chars including terminated-0.
            * @return                       the buffer to hold destination unicode string after conversion.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            wchar_t* utf8_to_os_unicode_neo(const char* src_utf_str, ::pilo::i64_t src_utf_len, ::pilo::i64_t* ret_cc_ucs);

            /**
            * @brief                        convert utf encoded string to gbk string. use caller-supplied buffer.
            * @param dst_gbk_buffer [out]   buffer to hold unicode string after conversion.
            * @param cc_dst_gbk_buffer [in] the size of destination buffer in char count.
            * @param src_utf_str [in]       the source utf8 string.
            * @param src_utf_len [in]       the length of source utf8 string in chars.
            * @return                       the destination size after conversion in chars including terminated-0.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            ::pilo::i64_t utf8_to_gbk_fixed(char* dst_gbk_buffer, ::pilo::i64_t cc_dst_gbk_buffer, const char* src_utf_str, ::pilo::i64_t src_utf_len);

            /**
            * @brief                        convert utf8 encoded string to gbk string. use dynamic heap allocation.
            * @param src_utf_str [in]       the source utf8 string.
            * @param src_utf_len [in]       the length of source utf8 string in chars.
            * @param ret_cc_gbk [out]       the destination size after conversion in chars including terminated-0.
            * @return                       the buffer to hold destination gbk string after conversion.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            char* utf8_to_gbk_neo(const char* src_utf_str, ::pilo::i64_t src_utf_len, ::pilo::i64_t* ret_cc_gbk);

            /**
            * @brief                        convert gbk encoded string to utf8 string. use caller-supplied buffer.
            * @param dst_utf8_buffer [out]  buffer to hold unicode string after conversion.
            * @param cc_dst_utf8_buffer [in] the size of destination buffer in char count.
            * @param src_gbk_str [in]       the source gbk string.
            * @param src_gbk_len [in]       the length of source gbk string in chars.
            * @return                       the destination size after conversion in chars including terminated-0.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed 
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo utf8_to_gbk_neo gbk_to_utf8_neo
            * @attention                    [NPC] [RC] [MTS]
            */
            ::pilo::i64_t gbk_to_utf8_fixed(char* dst_utf8_buffer, ::pilo::i64_t cc_dst_utf8_buffer, const char* src_gbk_str, ::pilo::i64_t src_gbk_len);

            /**
            * @brief                        convert gbk encoded string to utf8 string. use dynamic heap allocation.
            * @param src_gbk_str [in]       the source gbk string.
            * @param src_gbk_len [in]       the length of source gbk string in chars.
            * @param ret_cc_utf [out]       the destination size after conversion in chars including terminated-0.
            * @return                       the buffer to hold destination utf8 string after conversion.
            * @see                          gbk_to_os_unicode_fixed os_unicode_to_gbk_fixed os_unicode_to_utf8_fixed utf8_to_os_unicode_fixed utf8_to_gbk_fixed gbk_to_utf8_fixed
            *                               gbk_to_os_unicode_neo os_unicode_to_gbk_neo os_unicode_to_utf8_neo utf8_to_os_unicode_neo utf8_to_gbk_neo 
            * @attention                    [NPC] [RC] [MTS]
            */
            char* gbk_to_utf8_neo(const char* src_gbk_str, ::pilo::i64_t src_gbk_len, ::pilo::i64_t* ret_cc_utf);



            wchar_t* gbk_to_os_unicode(wchar_t* dst_ucs_buffer, ::pilo::i64_t cc_dst_ucs_buffer, const char* src_gbk_str, ::pilo::i64_t src_gbk_len);
            char* gbk_to_utf8(char* dst_utf8_buffer, ::pilo::i64_t cc_dst_utf8_buffer, const char* src_gbk_str, ::pilo::i64_t src_gbk_len);
            char* utf8_to_gbk(char* dst_gbk_buffer, ::pilo::i64_t cc_dst_gbk_buffer, const char* src_utf_str, ::pilo::i64_t src_utf_len);
            wchar_t* utf8_to_os_unicode(wchar_t* dst_ucs_buffer, ::pilo::i64_t cc_dst_ucs_buffer, const char* src_utf_str, ::pilo::i64_t src_utf_len);
            char* os_unicode_to_utf8(char* dst_utf_buffer, ::pilo::i64_t cc_dst_utf_buffer, const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len);
            char* os_unicode_to_gbk(char* dst_gbk_buffer, ::pilo::i64_t cc_dst_gbk_buffer, const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len);
        }
    }
}

/*
UCS-4*
UCS-4BE
UCS-4LE*
UCS-2
UCS-2BE
UCS-2LE
UTF-32*
UTF-32BE*
UTF-32LE*
UTF-16*
UTF-16BE*
UTF-16LE*
UTF-7
UTF7-IMAP
UTF-8*
ASCII*
EUC-JP*
SJIS*
eucJP-win*
SJIS-win*
ISO-2022-JP
ISO-2022-JP-MS
CP932
CP51932
SJIS-mac** (Alias: MacJapanese)
SJIS-Mobile#DOCOMO** (Alias:SJIS-DOCOMO)
SJIS-Mobile#KDDI** (Alias:SJIS-KDDI)
SJIS-Mobile#SOFTBANK** (Alias:SJIS-SOFTBANK)
UTF-8-Mobile#DOCOMO** (Alias:UTF-8-DOCOMO)
UTF-8-Mobile#KDDI-A**
UTF-8-Mobile#KDDI-B** (Alias:UTF-8-KDDI)
UTF-8-Mobile#SOFTBANK** (Alias:UTF-8-SOFTBANK)
ISO-2022-JP-MOBILE#KDDI** (Alias:ISO-2022-JP-KDDI)
JIS
JIS-ms
CP50220
CP50220raw
CP50221
CP50222
ISO-8859-1*
ISO-8859-2*
ISO-8859-3*
ISO-8859-4*
ISO-8859-5*
ISO-8859-6*
ISO-8859-7*
ISO-8859-8*
ISO-8859-9*
ISO-8859-10*
ISO-8859-13*
ISO-8859-14*
ISO-8859-15*
ISO-8859-16*
byte2be
byte2le
byte4be
byte4le
BASE64
HTML-ENTITIES
7bit
8bit
EUC-CN*
CP936
GB18030**
HZ
EUC-TW*
CP950
BIG-5*
EUC-KR*
UHC (CP949)
ISO-2022-KR
Windows-1251 (CP1251)
Windows-1252 (CP1252)
CP866 (IBM866)
KOI8-R*
KOI8-U*
ArmSCII-8 (ArmSCII8)

*/