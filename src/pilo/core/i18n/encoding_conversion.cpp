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

            //GBK <-> Unicode LE

            

            ::pilo::err_t os_unicode_to_utf8(::pilo::char_buffer_t& buffer, const wchar_t* src, ::pilo::i32_t src_len, ::pilo::i32_t extra)
            {
                if (src == nullptr || src_len < 0)
                {
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);
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
                    buffer.check_space(max_need_capa);
                }
                else if (buffer.capacity() <= (1 + extra))
                {
                    buffer.check_space(max_need_capa);
                }
                
#ifdef WINDOWS                
                
                ::pilo::i32_t ret = WideCharToMultiByte(CP_UTF8, 0, src, src_len, buffer.ptr(), buffer.space_available() - (1 + extra), nullptr, nullptr);
                if (ret > 0)
                {
                    buffer.set(ret, 0);
                    buffer.add_size(ret);
                    return PILO_OK;
                }                                
                ::pilo::i32_t cal_len = WideCharToMultiByte(CP_UTF8, 0, src, src_len, nullptr, 0, nullptr, nullptr);
                if (cal_len <= 0)
                {
                    return ::pilo::make_core_error(PES_TEXT, PEP_ICONV_FAILED);
                }
                buffer.check_space(cal_len + 1 + extra);
                ret = WideCharToMultiByte(CP_UTF8, 0, src, src_len, buffer.ptr(), cal_len+1, nullptr, nullptr);
                if (ret > 0)
                {
                    buffer.set(ret, 0);
                    buffer.add_size(ret);
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
                    buffer.set(ret, 0);
                    buffer.add_size(ret);
                    return PILO_OK;
                }
                else
                {
                    if (buffer.capacity() < max_need_capa)
                    {
                        return ::pilo::make_core_error(PES_TEXT, PEP_ICONV_FAILED);
                    }
                }
                buffer.check_space(max_need_capa);
                ret = _iconv_convert_any_to_any_fixed(
                    PMS_OS_UNICODE_NAME, "utf-8",
                    buffer.ptr(), buffer.space_available() - (1 + extra),
                    (const char*)src, src_len * sizeof(wchar_t)
                );
                if (ret > 0)
                {
                    buffer.set(ret, 0);
                    buffer.add_size(ret);
                    return PILO_OK;
                }                
#endif         
                return ::pilo::make_core_error(PES_TEXT, PEP_ICONV_FAILED, 1);

            }

            ::pilo::err_t utf8_to_os_unicode(::pilo::wchar_buffer_t& buffer, const char* src, ::pilo::i32_t src_len, ::pilo::i32_t extra)
            {
                if (src == nullptr || src_len < 0)
                {
                    return ::pilo::make_core_error(PES_PARAM, PEP_IS_INVALID);
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
                    buffer.check_space(max_need_capa);
                }
                else if (buffer.capacity() < (1 + extra))
                {
                    buffer.check_space(max_need_capa);
                }

#ifdef WINDOWS

                ::pilo::i32_t ret = MultiByteToWideChar(CP_UTF8, 0, src, src_len, buffer.ptr(), buffer.space_available() - (1 + extra));
                if (ret > 0)
                {
                    buffer.set(ret, 0);
                    buffer.add_size(ret);
                    return PILO_OK;
                }
                ::pilo::i32_t cal_len = MultiByteToWideChar(CP_UTF8, 0, src, src_len, nullptr, 0);
                if (cal_len <= 0)
                {
                    return ::pilo::make_core_error(PES_TEXT, PEP_ICONV_FAILED);
                }
                buffer.check_space((cal_len + 1 + extra) * sizeof(wchar_t));
                ret = MultiByteToWideChar(CP_UTF8, 0, src, src_len, buffer.ptr(), cal_len + 1);
                if (ret > 0)
                {
                    buffer.set(ret, 0);
                    buffer.add_size(ret);
                    return PILO_OK;
                }

#else
                ::pilo::i32_t ret = _iconv_convert_any_to_any_fixed(
                    PMS_OS_UNICODE_NAME, "utf-8",
                    (char*)buffer.ptr(), (buffer.space_available() - (1 + extra)) * sizeof(wchar_t),
                    (const char*)src, src_len * sizeof(wchar_t)
                );
                if (ret > 0)
                {
                    buffer.set(ret, 0);
                    buffer.add_size(ret);
                    return PILO_OK;
                }
                else
                {
                    if (buffer.capacity() < max_need_capa)
                    {
                        return ::pilo::make_core_error(PES_TEXT, PEP_ICONV_FAILED);
                    }
                }
                buffer.check_space(max_need_capa);
                ret = _iconv_convert_any_to_any_fixed(
                    PMS_OS_UNICODE_NAME, "utf-8",
                    (char*)buffer.ptr(), (buffer.space_available() - (1 + extra)) * sizeof(wchar_t),
                    (const char*)src, src_len * sizeof(wchar_t)
                );
                if (ret > 0)
                {
                    buffer.set(ret, 0);
                    buffer.set_size(ret);
                    return PILO_OK;
                }
#endif         
                return ::pilo::make_core_error(PES_TEXT, PEP_ICONV_FAILED, 1);



            }

		}
    }
}
