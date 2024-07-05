#include "./encoding_conversion.hpp"
#include "../string/string_operation.hpp"

#define PMI_CODE_PAGE_GBK		(936)
#define PMS_CODE_PAGE_GBK_NAME	"gb2312"



namespace pilo {
    namespace core {
        namespace i18n {
#if defined(WINDOWS)	
#else
#include <iconv.h>

            static size_t _iconv_convert_any_to_any_fixed(
                const char* src_charset,
                const char* dst_charset,
                char* dst_buffer_ptr, ::pilo::i32_t buffer_size,
                const char* src_buffer_ptr, ::pilo::i32_t src_len)
            {
                char* tmp_pptr_src = (char*)src_buffer_ptr;
                size_t tmp_src_len = (size_t) src_len;
                char* tmp_pptr_dst = dst_buffer_ptr;
                size_t tmp_cc_buffer = (size_t) buffer_size;

                iconv_t cd = iconv_open(dst_charset, src_charset);
                if (cd == 0)
                {
                    return 0;
                }

                iconv(cd, (char**)& tmp_pptr_src, &tmp_src_len, (char**)& tmp_pptr_dst, &tmp_cc_buffer);

                int diff = tmp_pptr_dst - dst_buffer_ptr;
                diff = strnlen(dst_buffer_ptr, diff);

                //printf("ret=%d, err=%d, srclen=%d srcleft=%d  bufsz=%d bufleft=%d\n", 
                //	(int) ret, errno, (int)src_len, (int) tmp_src_len,  (int) buffer_size, (int) tmp_cc_buffer  );
                iconv_close(cd);
                return diff;
            }
#endif

            ::pilo::err_t os_unicode_to_utf8(::pilo::char_buffer_t& buffer, const wchar_t* src, ::pilo::i32_t src_len, ::pilo::i32_t extra)
            {
                if (extra < 0)
                    return ::pilo::mk_perr(PERR_INVALID_PARAM);

                if (src == nullptr || src_len < 0)
                {
                    return ::pilo::mk_perr(PERR_INVALID_PARAM);
                }
                else if (*src == 0 || src_len == 0)
                {
                    return PILO_OK;
                }

                if (src_len < 0)
                {
                    src_len = (::pilo::i32_t) ::pilo::core::string::character_count(src);
                }
                ::pilo::i32_t max_need_capa = (src_len + 1 + extra) * 4;
                if (buffer.invalid())
                {
                    buffer.check_more_space(max_need_capa);
                }
                else if (buffer.space_available() <= (1 + extra))
                {
                    buffer.check_more_space(max_need_capa);
                }
                
#ifdef WINDOWS      
                int tmpcchlen = buffer.space_available() - (1 + extra);
                ::pilo::i32_t ret = PILO_OK;
                if (tmpcchlen > 0)
                {
                    ret = WideCharToMultiByte(CP_UTF8, 0, src, src_len, buffer.ptr(), buffer.space_available() - (1 + extra), nullptr, nullptr);
                    if (ret > 0)
                    {
                        buffer.add_size(ret);
                        buffer.append(0);
                        return PILO_OK;
                    }
                }                
                                             
                ::pilo::i32_t cal_len = WideCharToMultiByte(CP_UTF8, 0, src, src_len, nullptr, 0, nullptr, nullptr);
                if (cal_len <= 0)
                {
                    return ::pilo::mk_perr(PERR_ICONV_FAIL);
                }
                buffer.check_more_space(cal_len + 1 + extra);
                ret = WideCharToMultiByte(CP_UTF8, 0, src, src_len, buffer.ptr(), cal_len+1, nullptr, nullptr);
                if (ret > 0)
                {
                    buffer.add_size(ret);
                    buffer.append(0);
                    return PILO_OK;
                }

#else
                ::pilo::i32_t ret = _iconv_convert_any_to_any_fixed(
                    PMS_OS_UNICODE_NAME, "utf-8",
                    buffer.ptr(), buffer.space_available() - (1+extra),
                    (const char*)src, src_len * sizeof(wchar_t)
                );
                if (ret > 0)
                {
                    buffer.add_size(ret);
                    buffer.append(0);
                    return PILO_OK;
                }
                else
                {
                    if (buffer.capacity() < max_need_capa)
                    {
                        return ::pilo::mk_perr(PERR_ICONV_FAIL);
                    }
                }
                buffer.check_more_space(max_need_capa);
                ret = _iconv_convert_any_to_any_fixed(
                    PMS_OS_UNICODE_NAME, "utf-8",
                    buffer.ptr(), buffer.space_available() - (1 + extra),
                    (const char*)src, src_len * sizeof(wchar_t)
                );
                if (ret > 0)
                {
                    buffer.add_size(ret);
                    buffer.append(0);
                    return PILO_OK;
                }                
#endif         
                return ::pilo::mk_perr(PERR_ICONV_FAIL);

            }

            ::pilo::err_t utf8_to_os_unicode(::pilo::wchar_buffer_t& buffer, const char* src, ::pilo::i32_t src_len, ::pilo::i32_t extra)
            {
                if (extra < 0)
                    return ::pilo::mk_perr(PERR_INVALID_PARAM);
                if (src == nullptr || src_len < 0)
                {
                    return ::pilo::mk_perr(PERR_INVALID_PARAM);
                }
                else if (*src == 0 || src_len == 0)
                {
                    buffer.check_more_space((1 + extra) * 4);
                    return PILO_OK;
                }

                if (src_len < 0)
                {
                    src_len = (::pilo::i32_t) ::pilo::core::string::character_count(src);
                }
                ::pilo::i32_t max_need_capa = (src_len + 1 + extra) * 4;
                if (buffer.invalid())
                {
                    buffer.check_more_space(max_need_capa);
                }
                else if (buffer.space_available() <= (1 + extra))
                {
                    buffer.check_more_space(max_need_capa);
                }

#ifdef WINDOWS
                int tmpcchlen  = buffer.space_available() - (1 + extra);
                ::pilo::i32_t ret = PILO_OK;
                if (tmpcchlen > 0)
                {
                    ret = MultiByteToWideChar(CP_UTF8, 0, src, src_len, buffer.ptr(), tmpcchlen);
                    if (ret > 0)
                    {
                        buffer.add_size(ret);
                        buffer.append(0);
                        return PILO_OK;
                    }
                }
                
                ::pilo::i32_t cal_len = MultiByteToWideChar(CP_UTF8, 0, src, src_len, nullptr, 0);
                if (cal_len <= 0)
                {
                    return ::pilo::mk_perr(PERR_ICONV_FAIL);
                }
                buffer.check_more_space((cal_len + 1 + extra) * sizeof(wchar_t));
                ret = MultiByteToWideChar(CP_UTF8, 0, src, src_len, buffer.ptr(), cal_len + 1);
                if (ret > 0)
                {
                    buffer.add_size(ret);
                    buffer.append(0);
                    return PILO_OK;
                }

#else
                ::pilo::i32_t ret = _iconv_convert_any_to_any_fixed(
                    "utf-8",PMS_OS_UNICODE_NAME,
                    (char*)buffer.ptr(), (buffer.space_available() - (1 + extra)) * sizeof(wchar_t),
                    src, src_len
                );
                if (ret > 0)
                {
                    buffer.add_size(ret);
                    buffer.append(0);
                    return PILO_OK;
                }
                else
                {
                    if (buffer.capacity() < max_need_capa)
                    {
                        return ::pilo::mk_perr(PERR_ICONV_FAIL);
                    }
                }
                buffer.check_more_space(max_need_capa);
                ret = _iconv_convert_any_to_any_fixed(
                    "utf-8", PMS_OS_UNICODE_NAME,
                    (char*)buffer.ptr(), (buffer.space_available() - (1 + extra)) * sizeof(wchar_t),
                    src, src_len
                );
                if (ret > 0)
                {
                    buffer.add_size((::pilo::i32_t)ret);
                    buffer.append(0);
                    return PILO_OK;
                }
#endif         
                return ::pilo::mk_perr(PERR_ICONV_FAIL);



            }

            ::pilo::err_t utf8_to_ansi(::pilo::char_buffer_t& buffer, const char* src, ::pilo::i32_t src_len, ::pilo::i32_t extra)
            {
                if (extra < 0)
                    return ::pilo::mk_perr(PERR_INVALID_PARAM);

                if (src == nullptr || src_len < 0)
                {
                    return ::pilo::mk_perr(PERR_INVALID_PARAM);
                }
                else if (*src == 0 || src_len == 0)
                {
                    return PILO_OK;
                }

                if (src_len < 0)
                {
                    src_len = (::pilo::i32_t) ::pilo::core::string::character_count(src);
                }

                ::pilo::i32_t max_need_capa = (src_len + 1 + extra);
                if (buffer.invalid())
                {
                    buffer.check_more_space(max_need_capa);
                }
                else if (buffer.space_available() <= (1 + extra))
                {
                    buffer.check_more_space(max_need_capa);
                }

#if defined(WINDOWS)
                ::pilo::core::memory::object_array<wchar_t, PMI_STCPARAM_PATH_DEFAULT_LENGTH> wbuf;
                int dwUnicodeLen = MultiByteToWideChar(CP_UTF8, 0, src, src_len, NULL, 0);
                dwUnicodeLen++;
                wbuf.check_more_space(dwUnicodeLen + 1);                
                int ret = MultiByteToWideChar(CP_UTF8, 0, src, src_len, wbuf.begin(), dwUnicodeLen);
                if (ret <= 0)
                {
                    ::pilo::mk_perr(PERR_ICONV_FAIL);
                }
                wbuf.begin()[ret] = 0;
                wbuf.set_size(ret);
                
                int len = WideCharToMultiByte(CP_ACP, 0, (const wchar_t*)wbuf.begin(), wbuf.size(), NULL, 0, NULL, NULL);
                buffer.check_more_space(len + 1 + extra);                
                ret = WideCharToMultiByte(CP_ACP, 0, (const wchar_t*)wbuf.begin(), wbuf.size(), buffer.ptr(), len + 1, NULL, NULL);
                if (ret <= 0)
                {
                    ::pilo::mk_perr(PERR_ICONV_FAIL);
                }
                buffer.add_size(ret);
                buffer.append(0);

#else
                size_t ret = _iconv_convert_any_to_any_fixed(
                    "utf-8", PMS_CODE_PAGE_GBK_NAME,
                    (char*)buffer.ptr(), (buffer.space_available() - (1 + extra)),
                    src, src_len
                );
                if (ret > 0)
                {
                    buffer.add_size((::pilo::i32_t)ret);
                    buffer.append(0);
                    return PILO_OK;
                }
                ::pilo::i32_t max_len = (src_len + 1 + extra) * 2;
                buffer.check_more_space(max_len);
                ret = _iconv_convert_any_to_any_fixed(
                    "utf-8", PMS_CODE_PAGE_GBK_NAME,
                    (char*)buffer.ptr(), (buffer.space_available() - (1 + extra)),
                    src, src_len
                );
                if (ret <= 0)
                {
                    ::pilo::mk_perr(PERR_ICONV_FAIL);
                }
                buffer.add_size((::pilo::i32_t)ret);
                buffer.append(0);

#endif
                return PILO_OK;

            }

            ::pilo::err_t ansi_to_utf8(::pilo::char_buffer_t& buffer, const char* src, ::pilo::i32_t src_len, ::pilo::i32_t extra)
            {
                if (extra < 0)
                    return ::pilo::mk_perr(PERR_INVALID_PARAM);

                if (src == nullptr || src_len < 0)
                {
                    return ::pilo::mk_perr(PERR_INVALID_PARAM);
                }
                else if (*src == 0 || src_len == 0)
                {
                    return PILO_OK;
                }

                if (src_len < 0)
                {
                    src_len = (::pilo::i32_t) ::pilo::core::string::character_count(src);
                }
#if defined(WINDOWS)
                ::pilo::core::memory::object_array<wchar_t, PMI_STCPARAM_PATH_DEFAULT_LENGTH> wbuf;
                int dwUnicodeLen = MultiByteToWideChar(CP_ACP, 0, src, src_len, NULL, 0);
                dwUnicodeLen++;
                wbuf.check_space(dwUnicodeLen);
                int ret = MultiByteToWideChar(CP_ACP, 0, src, src_len, wbuf.begin(), dwUnicodeLen);
                if (ret <= 0)
                {
                    ::pilo::mk_perr(PERR_ICONV_FAIL);
                }
                wbuf.begin()[ret] = 0;
                wbuf.set_size(ret);

                int len = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)wbuf.begin(), wbuf.size(), NULL, 0, NULL, NULL);
                buffer.check_more_space(len + 1 + extra);
                ret = WideCharToMultiByte(CP_UTF8, 0, (const wchar_t*)wbuf.begin(), wbuf.size(), buffer.ptr(), len + 1, NULL, NULL);
                if (ret <= 0)
                {
                    ::pilo::mk_perr(PERR_ICONV_FAIL);
                }
                buffer.add_size(len);
                buffer.append(0);
#else
                size_t ret = _iconv_convert_any_to_any_fixed(
                    PMS_CODE_PAGE_GBK_NAME, "utf-8",
                    (char*)buffer.ptr(), (buffer.space_available() - (1 + extra)),
                    src, src_len
                );
                if (ret > 0)
                {
                    buffer.add_size((::pilo::i32_t)ret);
                    buffer.append(0);
                    return PILO_OK;
                }
                ::pilo::i32_t max_len = (src_len + 1 + extra);
                buffer.check_more_space(max_len);
                ret = _iconv_convert_any_to_any_fixed(
                    PMS_CODE_PAGE_GBK_NAME, "utf-8",
                    (char*)buffer.ptr(), (buffer.space_available() - (1 + extra)),
                    src, src_len
                );
                if (ret <= 0)
                {
                    ::pilo::mk_perr(PERR_ICONV_FAIL);
                }
                buffer.add_size((::pilo::i32_t)ret);
                buffer.append(0);

#endif
                return PILO_OK;

            }

		}
    }
}
