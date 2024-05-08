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

            static ::pilo::i64_t _iconv_convert_any_to_any_fixed(
                const char* src_charset,
                const char* dst_charset,
                char* dst_buffer_ptr, ::pilo::i64_t buffer_size,
                const char* src_buffer_ptr, ::pilo::i64_t src_len)
            {
                char* tmp_pptr_src = (char*)src_buffer_ptr;
                size_t tmp_src_len = src_len;
                char* tmp_pptr_dst = dst_buffer_ptr;
                size_t tmp_cc_buffer = buffer_size;

                iconv_t cd = iconv_open(dst_charset, src_charset);
                if (cd == 0)
                {
                    return 0;
                }

                iconv(cd, (char**)& tmp_pptr_src, (size_t *) &tmp_src_len, (char**)& tmp_pptr_dst, (size_t *) &tmp_cc_buffer);


                //printf("ret=%d, err=%d, srclen=%d srcleft=%d  bufsz=%d bufleft=%d\n", 
                //	(int) ret, errno, (int)src_len, (int) tmp_src_len,  (int) buffer_size, (int) tmp_cc_buffer  );
                iconv_close(cd);
                return (buffer_size - (::pilo::i64_t) tmp_cc_buffer);
            }
#endif

            //GBK <-> Unicode LE

            ::pilo::i64_t gbk_to_os_unicode_fixed(wchar_t* dst_ucs_buffer, ::pilo::i64_t cc_dst_ucs_buffer, const char* src_gbk_str, ::pilo::i64_t src_gbk_len)
            {
                if (dst_ucs_buffer == nullptr || cc_dst_ucs_buffer < 1)
                    return -1;

                if (src_gbk_len == 0 || src_gbk_str == nullptr || *src_gbk_str == 0)
                {
                    *dst_ucs_buffer = 0;
                    return 1;
                }

                if (src_gbk_len == -1)
                {
                    src_gbk_len = PMF_STRING_LENGTH_A(src_gbk_str) + 1;
                }

#if defined(WINDOWS)	

                return MultiByteToWideChar(PMI_CODE_PAGE_GBK, 0,
                    src_gbk_str,
                    (int)src_gbk_len,
                    dst_ucs_buffer,
                    (int)cc_dst_ucs_buffer);
#else

                ::pilo::i64_t ret = 0;
                ret = _iconv_convert_any_to_any_fixed(
                    PMS_CODE_PAGE_GBK_NAME, PMS_OS_UNICODE_NAME,
                    (char*)dst_ucs_buffer, cc_dst_ucs_buffer * sizeof(wchar_t),
                    src_gbk_str, src_gbk_len
                );

                return ret / (sizeof(wchar_t));

#endif
            }

            wchar_t* gbk_to_os_unicode_neo(const char* src_gbk_str, ::pilo::i64_t src_gbk_len, ::pilo::i64_t* ret_cc_ucs)
            {
                wchar_t* ptr = nullptr;

                if (src_gbk_len == 0 || src_gbk_str == nullptr || *src_gbk_str == 0 )
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    return nullptr;
                }

#if defined(WINDOWS)	
                int nRetLen = MultiByteToWideChar(PMI_CODE_PAGE_GBK, 0, (char*)src_gbk_str, (int)src_gbk_len, nullptr, 0);
                if (nRetLen <= 0)
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    return nullptr;
                }

                ::pilo::i64_t nTmpLen = nRetLen;
                nTmpLen++;
                ptr = (wchar_t*)PMF_HEAP_MALLOC(nTmpLen * sizeof(wchar_t));
                if (ptr == nullptr)
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    return nullptr;
                }

                nRetLen = MultiByteToWideChar(PMI_CODE_PAGE_GBK, 0,
                    src_gbk_str,
                    (int)src_gbk_len,
                    ptr,
                    (int)nRetLen + 1);

                if (nRetLen <= 0)
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    (void)PMF_HEAP_FREE(ptr);
                    return nullptr;
                }

#else
                if (src_gbk_len == -1)
                {
                    src_gbk_len = PMF_STRING_LENGTH_A(src_gbk_str) + 1;
                }

                ::pilo::i64_t nTmpLen  = src_gbk_len * sizeof(wchar_t) + 1;
                ptr = (wchar_t*)PMF_HEAP_MALLOC(nTmpLen);
                if (ptr == nullptr)
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    return nullptr;
                }

                nTmpLen = (int)_iconv_convert_any_to_any_fixed(
                    PMS_CODE_PAGE_GBK_NAME, PMS_OS_UNICODE_NAME,
                    (char*)ptr, nTmpLen,
                    src_gbk_str, src_gbk_len);

#endif

                if (ret_cc_ucs != nullptr)* ret_cc_ucs = nTmpLen;
                return ptr;
            }

            ::pilo::i64_t os_unicode_to_gbk_fixed(char* dst_gbk_buffer, ::pilo::i64_t cc_dst_gbk_buffer, const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len)
            {
                if (dst_gbk_buffer == nullptr || cc_dst_gbk_buffer < 1)
                    return -1;

                if (src_ucs_len == 0 || src_ucs_str == nullptr || *src_ucs_str == 0)
                {
                    *dst_gbk_buffer = 0;
                    return 1;
                }

#if defined(WINDOWS)	
                return WideCharToMultiByte(PMI_CODE_PAGE_GBK, 0,
                    src_ucs_str,
                    (int)src_ucs_len,
                    dst_gbk_buffer,
                    (int)cc_dst_gbk_buffer, nullptr, nullptr);


#else
                if (src_ucs_len == -1)
                {
                    src_ucs_len = PMF_STRING_LENGTH_W(src_ucs_str);
                }

                return _iconv_convert_any_to_any_fixed(
                    PMS_OS_UNICODE_NAME, PMS_CODE_PAGE_GBK_NAME,
                    dst_gbk_buffer, cc_dst_gbk_buffer,
                    (const char*)src_ucs_str, src_ucs_len * sizeof(wchar_t)
                );

#endif
            }

            char* os_unicode_to_gbk_neo(const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len, ::pilo::i64_t* ret_cc_gbk)
            {
                char* ptr = nullptr;

                if (src_ucs_len == 0 || src_ucs_str == nullptr || *src_ucs_str == 0 )
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    return nullptr;
                }


#if defined(WINDOWS)	
                int nRetLen = WideCharToMultiByte(PMI_CODE_PAGE_GBK, 0, src_ucs_str, (int)src_ucs_len, nullptr, 0, nullptr, nullptr);
                if (nRetLen <= 0)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    return nullptr;
                }
                ::pilo::i64_t nTmpRetLen = nRetLen;
                ptr = (char*)PMF_HEAP_MALLOC(nTmpRetLen+1);
                if (ptr == nullptr)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    return nullptr;
                }

                nRetLen = WideCharToMultiByte(PMI_CODE_PAGE_GBK, 0,
                    src_ucs_str,
                    (int)src_ucs_len,
                    ptr,
                    (int)nRetLen, nullptr, nullptr);

                if (nRetLen <= 0)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    (void)PMF_HEAP_FREE(ptr);
                    return nullptr;
                }

#else
                if (src_ucs_len == -1)
                {
                    src_ucs_len = PMF_STRING_LENGTH_W(src_ucs_str) + 1;
                }

                ::pilo::i64_t nRetLen = src_ucs_len * 2;
                ptr = (char*)PMF_HEAP_MALLOC(nRetLen);
                if (ptr == nullptr)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    return nullptr;
                }

                nRetLen = _iconv_convert_any_to_any_fixed(
                    PMS_OS_UNICODE_NAME, PMS_CODE_PAGE_GBK_NAME,
                    ptr, nRetLen,
                    (const char*)src_ucs_str, src_ucs_len * sizeof(wchar_t));

#endif

                if (ret_cc_gbk != nullptr)* ret_cc_gbk = nRetLen;
                return ptr;
            }

            //unicode <-> utf8
            ::pilo::i64_t os_unicode_to_utf8_fixed(char* dst_utf_buffer, ::pilo::i64_t cc_dst_utf_buffer, const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len)
            {
                if (dst_utf_buffer == nullptr || cc_dst_utf_buffer < 1)
                    return -1;

                if (src_ucs_len == 0 || src_ucs_str == nullptr || *src_ucs_str == 0)
                {
                    *dst_utf_buffer = 0;
                    return 1;
                }

#if defined(WINDOWS)	
                return WideCharToMultiByte(CP_UTF8, 0,
                    src_ucs_str,
                    (int)src_ucs_len,
                    dst_utf_buffer,
                    (int)cc_dst_utf_buffer, nullptr, nullptr);


#else
                if (src_ucs_len == -1)
                {
                    src_ucs_len = PMF_STRING_LENGTH_W(src_ucs_str) + 1;
                }

                return _iconv_convert_any_to_any_fixed(
                    PMS_OS_UNICODE_NAME, "utf-8",
                    dst_utf_buffer, cc_dst_utf_buffer,
                    (const char*)src_ucs_str, src_ucs_len * sizeof(wchar_t)
                );

#endif
            }

            char* os_unicode_to_utf8_neo(const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len, ::pilo::i64_t* ret_cc_utf)
            {
                char* ptr = nullptr;

                if (src_ucs_len == 0 || src_ucs_str == nullptr || *src_ucs_str == 0 )
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    return nullptr;
                }


#if defined(WINDOWS)	
                int nRetLen = WideCharToMultiByte(CP_UTF8, 0, src_ucs_str, (int)src_ucs_len, nullptr, 0, nullptr, nullptr);
                if (nRetLen <= 0)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    return nullptr;
                }
                ::pilo::i64_t nTmpRetLen = nRetLen;
                ptr = (char*)PMF_HEAP_MALLOC(nTmpRetLen+1);
                if (ptr == nullptr)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    return nullptr;
                }

                nRetLen = WideCharToMultiByte(CP_UTF8, 0,
                    src_ucs_str,
                    (int)src_ucs_len,
                    ptr,
                    (int)nRetLen, nullptr, nullptr);

                if (nRetLen <= 0)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    (void)PMF_HEAP_FREE(ptr);
                    return nullptr;
                }

#else
                if (src_ucs_len == -1)
                {
                    src_ucs_len = PMF_STRING_LENGTH_W(src_ucs_str) + 1;
                }

                ::pilo::i64_t nRetLen = src_ucs_len * sizeof(wchar_t);
                ptr = (char*)PMF_HEAP_MALLOC(nRetLen);
                if (ptr == nullptr)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    return nullptr;
                }

                nRetLen = _iconv_convert_any_to_any_fixed(
                    PMS_OS_UNICODE_NAME, "utf-8",
                    ptr, nRetLen,
                    (const char*)src_ucs_str, src_ucs_len * sizeof(wchar_t));

#endif

                if (ret_cc_utf != nullptr)* ret_cc_utf = nRetLen;
                return ptr;
            }

            ::pilo::i64_t utf8_to_os_unicode_fixed(wchar_t* dst_ucs_buffer, ::pilo::i64_t cc_dst_ucs_buffer, const char* src_utf_str, ::pilo::i64_t src_utf_len)
            {
                if (dst_ucs_buffer == nullptr || cc_dst_ucs_buffer < 1)
                    return -1;

                if (src_utf_len == 0 || src_utf_str == nullptr || *src_utf_str == 0)
                {
                    *dst_ucs_buffer = 0;
                    return 0;
                }

#if defined(WINDOWS)	

                return MultiByteToWideChar(CP_UTF8, 0,
                    src_utf_str,
                    (int)src_utf_len,
                    dst_ucs_buffer,
                    (int)cc_dst_ucs_buffer);
#else
                if (src_utf_len == -1)
                {
                    src_utf_len = PMF_STRING_LENGTH_A(src_utf_str) + 1;
                }

                return _iconv_convert_any_to_any_fixed(
                    "utf-8", PMS_OS_UNICODE_NAME,
                    (char*)dst_ucs_buffer, cc_dst_ucs_buffer * sizeof(wchar_t),
                    src_utf_str, src_utf_len
                );

#endif
            }

            wchar_t* utf8_to_os_unicode_neo(const char* src_utf_str, ::pilo::i64_t src_utf_len, ::pilo::i64_t* ret_cc_ucs)
            {
                wchar_t* ptr = nullptr;

                if (src_utf_len == 0 || src_utf_str == nullptr || *src_utf_str == 0)
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    return nullptr;
                }

#if defined(WINDOWS)	
                int nRetLen = MultiByteToWideChar(CP_UTF8, 0, src_utf_str, (int)src_utf_len, nullptr, 0);
                if (nRetLen <= 0)
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    return nullptr;
                }

                ::pilo::i64_t nTmpRetLen = nRetLen;
                ptr = (wchar_t*)PMF_HEAP_MALLOC((nTmpRetLen + 1) * sizeof(wchar_t));
                if (ptr == nullptr)
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    return nullptr;
                }

                nRetLen = MultiByteToWideChar(CP_UTF8, 0,
                    src_utf_str,
                    (int)src_utf_len,
                    ptr,
                    (int)nRetLen + 1);

                if (nRetLen <= 0)
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    (void)PMF_HEAP_FREE(ptr);
                    return nullptr;
                }

#else
                if (src_utf_len == -1)
                {
                    src_utf_len = PMF_STRING_LENGTH_A(src_utf_str) + 1;
                }

                ::pilo::i64_t nRetLen = src_utf_len * sizeof(wchar_t);
                ptr = (wchar_t*)PMF_HEAP_MALLOC(nRetLen);
                if (ptr == nullptr)
                {
                    if (ret_cc_ucs != nullptr)* ret_cc_ucs = 0;
                    return nullptr;
                }

                nRetLen = _iconv_convert_any_to_any_fixed(
                    "utf-8", PMS_OS_UNICODE_NAME,
                    (char*)ptr, nRetLen,
                    src_utf_str, src_utf_len);

#endif

                if (ret_cc_ucs != nullptr)* ret_cc_ucs = nRetLen;
                return ptr;
            }

            ::pilo::i64_t utf8_to_gbk_fixed(char* dst_gbk_buffer, ::pilo::i64_t cc_dst_gbk_buffer, const char* src_utf_str, ::pilo::i64_t src_utf_len)
            {
                if (dst_gbk_buffer == nullptr || cc_dst_gbk_buffer < 1)
                    return -1;

                if (src_utf_len == 0 || src_utf_str == nullptr || *src_utf_str == 0)
                {
                    *dst_gbk_buffer = 0;
                    return 0;
                }

                if (src_utf_len == -1)
                {
                    src_utf_len = PMF_STRING_LENGTH_A(src_utf_str) + 1;
                }

#if defined(WINDOWS)	
                wchar_t* wbuffer_ptr = nullptr;
                wbuffer_ptr = (wchar_t*)PMF_HEAP_MALLOC(cc_dst_gbk_buffer * sizeof(wchar_t));
                if (wbuffer_ptr == nullptr)
                {
                    return 0;
                }

                int nUC = MultiByteToWideChar(CP_UTF8, 0,
                    src_utf_str,
                    (int)src_utf_len,
                    wbuffer_ptr,
                    (int)cc_dst_gbk_buffer);

                if (nUC <= 0)
                {
                    (void)PMF_HEAP_FREE(wbuffer_ptr);
                    return 0;
                }

                nUC = WideCharToMultiByte(PMI_CODE_PAGE_GBK, 0,
                    wbuffer_ptr,
                    -1,
                    dst_gbk_buffer,
                    (int)cc_dst_gbk_buffer, nullptr, nullptr);

                (void)PMF_HEAP_FREE(wbuffer_ptr);

                if (nUC <= 0)
                {
                    return 0;
                }

                return nUC;

#else	

                return _iconv_convert_any_to_any_fixed(
                    "utf-8", PMS_CODE_PAGE_GBK_NAME,
                    dst_gbk_buffer, cc_dst_gbk_buffer,
                    src_utf_str, src_utf_len
                );

#endif


            }

            char* utf8_to_gbk_neo(const char* src_utf_str, ::pilo::i64_t src_utf_len, ::pilo::i64_t* ret_cc_gbk)
            {
                char* res_ptr = nullptr;

                if (src_utf_len == 0 || src_utf_str == nullptr || *src_utf_str == 0 )
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    return nullptr;
                }

#if defined(WINDOWS)	
                wchar_t* ptr_wbuffer_tmp = nullptr;
                int nRetLen = MultiByteToWideChar(CP_UTF8, 0, src_utf_str, (int)src_utf_len, nullptr, 0);
                if (nRetLen <= 0)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    return nullptr;
                }

                ::pilo::i64_t nTmpRetLen = nRetLen;
                ptr_wbuffer_tmp = (wchar_t*)PMF_HEAP_MALLOC((nTmpRetLen + 1) * sizeof(wchar_t));
                if (ptr_wbuffer_tmp == nullptr)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    return nullptr;
                }

                nRetLen = MultiByteToWideChar(CP_UTF8, 0,
                    src_utf_str,
                    (int)src_utf_len,
                    ptr_wbuffer_tmp,
                    (int)nRetLen + 1);

                if (nRetLen <= 0)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    (void)PMF_HEAP_FREE(ptr_wbuffer_tmp);
                    return nullptr;
                }

                nRetLen = WideCharToMultiByte(PMI_CODE_PAGE_GBK, 0, ptr_wbuffer_tmp, (int)nRetLen, nullptr, 0, nullptr, nullptr);
                if (nRetLen <= 0)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    (void)PMF_HEAP_FREE(ptr_wbuffer_tmp);
                    return nullptr;
                }

                nTmpRetLen = nRetLen;
                res_ptr = (char*)PMF_HEAP_MALLOC((nTmpRetLen + 1));
                if (res_ptr == nullptr)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    (void)PMF_HEAP_FREE(ptr_wbuffer_tmp);
                    return nullptr;
                }

                nRetLen = WideCharToMultiByte(PMI_CODE_PAGE_GBK, 0, ptr_wbuffer_tmp, (int)nRetLen, res_ptr, nRetLen, nullptr, nullptr);
                (void)PMF_HEAP_FREE(ptr_wbuffer_tmp);


#else
                if (src_utf_len == -1)
                {
                    src_utf_len = PMF_STRING_LENGTH_A(src_utf_str) + 1;
                }

                ::pilo::i64_t nRetLen = src_utf_len * 2;
                res_ptr = (char*)PMF_HEAP_MALLOC(nRetLen);
                if (res_ptr == nullptr)
                {
                    if (ret_cc_gbk != nullptr)* ret_cc_gbk = 0;
                    return nullptr;
                }

                nRetLen = _iconv_convert_any_to_any_fixed(
                    "utf-8", PMS_CODE_PAGE_GBK_NAME,
                    res_ptr, nRetLen,
                    src_utf_str, src_utf_len);

#endif

                if (ret_cc_gbk != nullptr)* ret_cc_gbk = nRetLen;
                return res_ptr;
            }

            ::pilo::i64_t gbk_to_utf8_fixed(char* dst_utf8_buffer, ::pilo::i64_t cc_dst_utf8_buffer, const char* src_gbk_str, ::pilo::i64_t src_gbk_len)
            {
                if (dst_utf8_buffer == nullptr || cc_dst_utf8_buffer < 1)
                    return -1;

                if (src_gbk_len == 0 || src_gbk_str == nullptr || *src_gbk_str == 0)
                {
                    *dst_utf8_buffer = 0;
                    return 0;
                }

#if defined(WINDOWS)	
                wchar_t* wbuffer_ptr = nullptr;
                int nUC = MultiByteToWideChar(PMI_CODE_PAGE_GBK, 0,
                    src_gbk_str,
                    (int)src_gbk_len,
                    nullptr,
                    0);

                if (nUC <= 0)
                {
                    return 0;
                }


                wbuffer_ptr = (wchar_t*)PMF_HEAP_MALLOC(nUC * sizeof(wchar_t));
                if (wbuffer_ptr == nullptr)
                {
                    return 0;
                }

                nUC = MultiByteToWideChar(PMI_CODE_PAGE_GBK, 0,
                    src_gbk_str,
                    (int)src_gbk_len,
                    wbuffer_ptr,
                    (int)nUC);
                if (nUC <= 0)
                {
                    (void)PMF_HEAP_FREE(wbuffer_ptr);
                    return 0;
                }

                nUC = WideCharToMultiByte(CP_UTF8, 0,
                    wbuffer_ptr,
                    nUC,
                    dst_utf8_buffer,
                    (int)cc_dst_utf8_buffer, nullptr, nullptr);

                (void)PMF_HEAP_FREE(wbuffer_ptr);

                if (nUC <= 0)
                {
                    return 0;
                }

                return nUC;

#else	
                if (src_gbk_len == -1)
                {
                    src_gbk_len = PMF_STRING_LENGTH_A(src_gbk_str) + 1;
                }

                return _iconv_convert_any_to_any_fixed(
                    PMS_CODE_PAGE_GBK_NAME, "utf-8",
                    dst_utf8_buffer, cc_dst_utf8_buffer,
                    src_gbk_str, src_gbk_len
                );

#endif
            }

            char* gbk_to_utf8_neo(const char* src_gbk_str, ::pilo::i64_t src_gbk_len, ::pilo::i64_t* ret_cc_utf)
            {
                char* res_ptr = nullptr;

                if (src_gbk_len == 0 || src_gbk_str == nullptr || *src_gbk_str == 0)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    return nullptr;
                }

#if defined(WINDOWS)	
                wchar_t* ptr_wbuffer_tmp = nullptr;
                int nRetLen = MultiByteToWideChar(PMI_CODE_PAGE_GBK, 0, src_gbk_str, (int)src_gbk_len, nullptr, 0);
                if (nRetLen <= 0)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    return nullptr;
                }

                ::pilo::i64_t nTmpRetLen = nRetLen;
                ptr_wbuffer_tmp = (wchar_t*)PMF_HEAP_MALLOC((nTmpRetLen + 1) * sizeof(wchar_t));
                if (ptr_wbuffer_tmp == nullptr)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    return nullptr;
                }

                nRetLen = MultiByteToWideChar(PMI_CODE_PAGE_GBK, 0,
                    src_gbk_str,
                    (int)src_gbk_len,
                    ptr_wbuffer_tmp,
                    (int)nRetLen + 1);
                if (nRetLen <= 0)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    (void)PMF_HEAP_FREE(ptr_wbuffer_tmp);
                    return nullptr;
                }

                nRetLen = WideCharToMultiByte(CP_UTF8, 0, ptr_wbuffer_tmp, (int)nRetLen, nullptr, 0, nullptr, nullptr);
                if (nRetLen <= 0)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    (void)PMF_HEAP_FREE(ptr_wbuffer_tmp);
                    return nullptr;
                }

                nTmpRetLen = nRetLen;
                res_ptr = (char*)PMF_HEAP_MALLOC(nTmpRetLen + 1);
                if (res_ptr == nullptr)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    (void)PMF_HEAP_FREE(ptr_wbuffer_tmp);
                    return nullptr;
                }

                nRetLen = WideCharToMultiByte(CP_UTF8, 0, ptr_wbuffer_tmp, (int)nRetLen, res_ptr, nRetLen, nullptr, nullptr);
                (void)PMF_HEAP_FREE(ptr_wbuffer_tmp);


#else
                if (src_gbk_len == -1)
                {
                    src_gbk_len = PMF_STRING_LENGTH_A(src_gbk_str) + 1;
                }

                ::pilo::i64_t nRetLen = src_gbk_len * 4 + 1;
                res_ptr = (char*)PMF_HEAP_MALLOC(nRetLen);
                if (res_ptr == nullptr)
                {
                    if (ret_cc_utf != nullptr)* ret_cc_utf = 0;
                    return nullptr;
                }

                nRetLen = _iconv_convert_any_to_any_fixed(
                    PMS_CODE_PAGE_GBK_NAME, "utf-8",
                    res_ptr, nRetLen,
                    src_gbk_str, src_gbk_len
                );

#endif

                if (ret_cc_utf != nullptr)* ret_cc_utf = nRetLen;
                return res_ptr;

            }


			wchar_t* gbk_to_os_unicode(wchar_t* dst_ucs_buffer, ::pilo::i64_t cc_dst_ucs_buffer, const char* src_gbk_str, ::pilo::i64_t src_gbk_len)
			{
                ::pilo::i64_t rsz = gbk_to_os_unicode_fixed(dst_ucs_buffer, cc_dst_ucs_buffer, src_gbk_str, src_gbk_len);
                if (rsz != -1 || rsz - 1 < src_gbk_len)
                {
                    return dst_ucs_buffer;
                }
                return gbk_to_os_unicode_neo(src_gbk_str, src_gbk_len, nullptr);
			}


			char* gbk_to_utf8(char* dst_utf8_buffer, ::pilo::i64_t cc_dst_utf8_buffer, const char* src_gbk_str, ::pilo::i64_t src_gbk_len)
			{
				::pilo::i64_t rsz = gbk_to_utf8_fixed(dst_utf8_buffer, cc_dst_utf8_buffer, src_gbk_str, src_gbk_len);
				if (rsz != -1 || rsz - 1 < src_gbk_len)
				{
                    return dst_utf8_buffer;
				}
				return gbk_to_utf8_neo(src_gbk_str, src_gbk_len, nullptr);
			}


			char* utf8_to_gbk(char* dst_gbk_buffer, ::pilo::i64_t cc_dst_gbk_buffer, const char* src_utf_str, ::pilo::i64_t src_utf_len)
			{
				::pilo::i64_t rsz = utf8_to_gbk_fixed(dst_gbk_buffer, cc_dst_gbk_buffer, src_utf_str, src_utf_len);
				if (rsz != -1)
				{
					return dst_gbk_buffer;
				}
				return utf8_to_gbk_neo(src_utf_str, src_utf_len, nullptr);
			}


			wchar_t* utf8_to_os_unicode(wchar_t* dst_ucs_buffer, ::pilo::i64_t cc_dst_ucs_buffer, const char* src_utf_str, ::pilo::i64_t src_utf_len)
			{
				::pilo::i64_t rsz = utf8_to_os_unicode_fixed(dst_ucs_buffer, cc_dst_ucs_buffer, src_utf_str, src_utf_len);
				if (rsz != -1)
				{
					return dst_ucs_buffer;
				}
				return utf8_to_os_unicode_neo(src_utf_str, src_utf_len, nullptr);
			}


			char* os_unicode_to_utf8(char* dst_utf_buffer, ::pilo::i64_t cc_dst_utf_buffer, const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len)
			{
				::pilo::i64_t rsz = os_unicode_to_utf8_fixed(dst_utf_buffer, cc_dst_utf_buffer, src_ucs_str, src_ucs_len);
				if (rsz != -1 || rsz - 1 < src_ucs_len)
				{
					return dst_utf_buffer;
				}
				return os_unicode_to_utf8_neo(src_ucs_str, src_ucs_len, nullptr);
			}


			char* os_unicode_to_gbk(char* dst_gbk_buffer, ::pilo::i64_t cc_dst_gbk_buffer, const wchar_t* src_ucs_str, ::pilo::i64_t src_ucs_len)
			{
				::pilo::i64_t rsz = os_unicode_to_gbk_fixed(dst_gbk_buffer, cc_dst_gbk_buffer, src_ucs_str, src_ucs_len);
				if (rsz != -1 || rsz - 1 < src_ucs_len)
				{
					return dst_gbk_buffer;
				}
				return os_unicode_to_gbk_neo(src_ucs_str, src_ucs_len, nullptr);
			}

		}
    }
}
