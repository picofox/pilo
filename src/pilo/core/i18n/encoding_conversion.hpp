#pragma once
#include "../../pilo.hpp"
#include "../memory/util.hpp"

namespace pilo {
    namespace core {
        namespace i18n {
            ::pilo::err_t os_unicode_to_utf8(::pilo::char_buffer_t& buffer, const wchar_t* src, ::pilo::i32_t src_len, ::pilo::i32_t extra = 0);
            ::pilo::err_t utf8_to_os_unicode(::pilo::wchar_buffer_t& buffer,const char* src, ::pilo::i32_t src_len, ::pilo::i32_t extra = 0);
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