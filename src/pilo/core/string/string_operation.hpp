#pragma once
#include "../pattern/singleton.hpp"
#include "../algorithm/find.hpp"
#include "../memory/bits_operation.hpp"
#include <uchar.h>
#include <functional>
#include "../io/formatted_io.hpp"
#include <algorithm>
#include "constants.hpp"
#include <cstring>
#include "../memory/util.hpp"


#define PMI_STRING_LAST_SECTION_FLAG     (1)
#define PMI_STRING_ITER_CREATION_FLAG     (2)

#       define  PMF_STRING_STR_FIND_A        strstr
#       define  PMF_STRING_STR_FIND_W        wcsstr
#       define  PMF_STRING_CH_FIND_A         strchr
#       define  PMF_STRING_CH_FIND_W         wcschr
#       define  PMF_STRING_CH_RFIND_A        strrchr
#       define  PMF_STRING_CH_RFIND_W        wcsrchr
#       define  PMF_STRING_COMPARE_A         strcmp
#       define  PMF_STRING_COMPARE_W         wcscmp
#       define  PMF_STRING_N_COMPARE_A       strncmp
#       define  PMF_STRING_N_COMPARE_W       wcsncmp
#       define  PMF_STRING_LENGTH_A          strlen
#       define  PMF_STRING_LENGTH_W          wcslen

#ifdef WINDOWS
#       define  PMF_STRING_DUPLICATE_A       _strdup
#       define  PMF_STRING_DUPLICATE_W       _wcsdup
#       define  PMF_STRING_I_COMPARE_A       _stricmp
#       define  PMF_STRING_I_COMPARE_W       _wcsicmp
#       define  PMF_STRING_I_N_COMPARE_A     _strnicmp
#       define  PMF_STRING_I_N_COMPARE_W     _wcsnicmp
#else
#       define  PMF_STRING_DUPLICATE_A       strdup
#       define  PMF_STRING_DUPLICATE_W       wcsdup
#       define  PMF_STRING_I_COMPARE_A       strcasecmp
#       define  PMF_STRING_I_COMPARE_W       wcscasecmp
#       define  PMF_STRING_I_N_COMPARE_A     strncasecmp
#       define  PMF_STRING_I_N_COMPARE_W     wcsncasecmp
#endif


#   if defined(_UNICODE) || defined (UNICODE)
#       define  PMF_STRING_STR_FIND		PMF_STRING_STR_FIND_W
#       define  PMF_STRING_N_COMPARE     PMF_STRING_N_COMPARE_W
#       define  PMF_STRING_COMPARE       PMF_STRING_COMPARE_W
#       define  PMF_STRING_LENGTH        PMF_STRING_LENGTH_W
#       define  PMF_STRING_CH_FIND       PMF_STRING_CH_FIND_W
#       define  PMF_STRING_CH_RFIND      PMF_STRING_CH_RFIND_W
#       define  PMF_STRING_DUPLICATE     PMF_STRING_DUPLICATE_W

#   else
#       define  PMF_STRING_STR_FIND		 PMF_STRING_STR_FIND_A
#       define  PMF_STRING_N_COMPARE     PMF_STRING_N_COMPARE_A
#       define  PMF_STRING_COMPARE       PMF_STRING_COMPARE_A
#       define  PMF_STRING_LENGTH        PMF_STRING_LENGTH_A
#       define  PMF_STRING_CH_FIND       PMF_STRING_CH_FIND_A
#       define  PMF_STRING_CH_RFIND      PMF_STRING_CH_RFIND_A
#       define  PMF_STRING_DUPLICATE     PMF_STRING_DUPLICATE_A

#   endif

namespace pilo
{
    namespace core
    {
        namespace string
        {
            template<typename TA_CHAR>
            ::pilo::i64_t cstring_ch_count(const TA_CHAR* str, ::pilo::i64_t offset, ::pilo::i64_t length,  TA_CHAR ch)
            {
                ::pilo::i64_t count = 0;
                for (::pilo::i64_t i = offset; i < offset + length; i ++) {
                    if (str[i] == ch) {
                        count++;
                    }
                }
                return count;
            }

            /**
            * @brief                    get string length, in character count, not bytes.
            * @param str [in]           the string your want to get it's length.
            * @return                   characters in the string, '\0' not included.
            * @see                      element_count()
            * @note                     can process char and wide char type string. use element_count if string is variable-length encoded string, such as utf8 or MBCS encoded.
            * @attention                [NPC] [MTS]
            *
            */
            ::pilo::i64_t character_count(const char* str);
            ::pilo::i64_t character_count(const wchar_t* str);

            /**
            * @brief                    copy src string to dst buffer and append term-0 at the end of the string. [NPC-Free]
            * @param dst [out]          the dst buffer to perform the copy. will append term-0 at last.
            * @param capa_char [in]     the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [in]           the src string to copy.
            * @return                   count of chars actually copied.(term-0 included)
            *                           return -1 if src or dst is nullptr or capa_char is 0
            * @see                      copy n_copy n_copyz
            * @note                     if capa_char <= len, result will be truncated. if capa_char is 1, result will be a empty string.
            * @attention                [NPC-Free] [MTS]
            */
            ::pilo::i64_t copyz(char* dst, ::pilo::i64_t capa_char, const char* src);
            ::pilo::i64_t copyz(wchar_t* dst, ::pilo::i64_t capa_char, const wchar_t* src);

            /**
            * @brief                    copy src string to dst buffer, No term-0 will be append.[NPC-Free]
            * @param dst [out]          the dst buffer to perform the copy. No term-0 will be append.
            * @param capa_char [in]     the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [in]           the src string to copy.
            * @return                   count of chars actually copied.
            *                           return -1 if src or dst is nullptr
            *                           return 0 if capa_char is 0
            * @see                      copy_z n_copy n_copyz
            * @note                     if capa_char < len, result will be truncated.
            * @attention                [NPC-Free] [MTS]
            */
            ::pilo::i64_t copy(char* dst, ::pilo::i64_t capa_char, const char* src);
            ::pilo::i64_t copy(wchar_t* dst, ::pilo::i64_t capa_char, const wchar_t* src);

            /**
            * @brief                    copy specific count char of src string to dst buffer, No term-0 will be append.[NPC-Free]
            * @param dst [out]           the dst buffer to perform the copy. No term-0 will be append.
            * @param capa_char [in]     the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [in]           the src string to copy.
            * @param src_len [in]       the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @return                   count of chars actually copied.
            * @see                      copy copy_z n_copyz
            * @note                     if capa_char < len, result will be truncated.
            * @attention                [NPC-Free] [MTS]
            */
            ::pilo::i64_t n_copy(char* dst, ::pilo::i64_t capa_char, const char* src, ::pilo::i64_t src_len);
            ::pilo::i64_t n_copy(wchar_t* dst, ::pilo::i64_t capa_char, const wchar_t* src, ::pilo::i64_t src_len);

            /**
            * @brief                    copy specific count char of src string to dst buffer, term-0 will be append.
            * @param dst [out]           the dst buffer to perform the copy. term-0 will be append.
            * @param capa_char [in]     the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [in]           the src string to copy.
            * @param src_len [in]       the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @return                   count of chars actually copied.
            *                           return -1 if src or dst is nullptr or capa_char is 0
            * @see                      copy copy_z n_copy
            * @note                     if capa_char <= len, result will be truncated. if capa_char is 1, result will be a empty string.
            * @attention                [NPC-Free] [MTS]
            */
            ::pilo::i64_t n_copyz(char* dst, ::pilo::i64_t capa_char, const char* src, ::pilo::i64_t src_len);
            ::pilo::i64_t n_copyz(wchar_t* dst, ::pilo::i64_t capa_char, const wchar_t* src, ::pilo::i64_t src_len);

            /**
            * @brief                    copy src string to dst buffer, dst and src can be overlapped term-0 will NOT be append.
            * @param dst [out]          the dst buffer to perform the copy. term-0 will NOT be append.
            * @param cc_dst_capa [in]   the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [in]           the src string to copy.
            * @return                   count of chars actually copied.
            * @see                      movez n_move n_movez
            * @attention                [NPC-Free] [MTS]
            */
            ::pilo::i64_t move(char* dst, ::pilo::i64_t cc_dst_capa, const char* src);
            ::pilo::i64_t move(wchar_t* dst, ::pilo::i64_t cc_dst_capa, const wchar_t* src);

            /**
            * @brief                    copy specific count char of src string to dst buffer, dst and src can be overlapped. term-0 will NOT be append.
            * @param dst [out]          the dst buffer to perform the copy. term-0 will NOT be append.
            * @param cc_dst_capa [in]   the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [in]           the src string to copy.
            * @param src_length [in]    the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @return                   count of chars actually copied.
            * @see                      move movez n_movez
            * @attention                [NPC-Free] [MTS]
            */
            ::pilo::i64_t n_move(char* dst, ::pilo::i64_t cc_dst_capa, const char* src, ::pilo::i64_t src_length);
            ::pilo::i64_t n_move(wchar_t* dst, ::pilo::i64_t cc_dst_capa, const wchar_t* src, ::pilo::i64_t src_length);

            /**
            * @brief                    copy src string to dst buffer, dst and src can be overlapped term-0 will be append.
            * @param dst [out]          the dst buffer to perform the copy. term-0 will be append.
            * @param cc_dst_capa [in]   the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [in]           the src string to copy.
            * @return                   count of chars actually copied.
            * @see                      move n_move n_movez
            * @attention                [NPC-Free] [MTS]
            */
            ::pilo::i64_t movez(char* dst, ::pilo::i64_t cc_dst_capa, const char* src);
            ::pilo::i64_t movez(wchar_t* dst, ::pilo::i64_t cc_dst_capa, const wchar_t* src);

            /**
            * @brief                    copy specific count char of src string to dst buffer, dst and src can be overlapped term-0 will be append.
            * @param dst [out]          the dst buffer to perform the copy. term-0 will be append.
            * @param cc_dst_capa [in]   the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [int]          the src string to copy.
            * @param src_length [in]    the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @return                   count of chars actually copied (terminated zero include).
            *                           return -1 if src or dst is nullptr
            * @see                      move movez n_move
            * @attention                [NPC-Free] [MTS]
            */
            ::pilo::i64_t n_movez(char* dst, ::pilo::i64_t cc_dst_capa, const char* src, ::pilo::i64_t src_length);
            ::pilo::i64_t n_movez(wchar_t* dst, ::pilo::i64_t cc_dst_capa, const wchar_t* src, ::pilo::i64_t src_length);


            /**
            * compare too string, to test if they are equal.
            * @param str0 [in]          left string to compare.
            * @param begin_pos_0 [in]   the start position of the left string to compare.
            * @param str1 [in]          right string to compare.
            * @param begin_pos_1 [in]   the start position of the right string to compare.
            * @param length [in]        the characters to compare, compare until null if MC_INVALID_SIZE is given.
            * @return pilo::i32_t       return 0 if they are equal,
            *                           <0 if left < right
            *                           >0 if left > right
            */
            ::pilo::i32_t strict_compare(const char* str0, ::pilo::i64_t begin_pos_0, const char* str1, ::pilo::i64_t begin_pos_1, ::pilo::i64_t length);
            ::pilo::i32_t strict_compare(const wchar_t* str0, ::pilo::i64_t begin_pos_0, const wchar_t* str1, ::pilo::i64_t begin_pos_1, ::pilo::i64_t length);
			::pilo::i32_t i_compare(const char* str0, ::pilo::i64_t begin_pos_0, const char* str1, ::pilo::i64_t begin_pos_1, ::pilo::i64_t length);
			::pilo::i32_t i_compare(const wchar_t* str0, ::pilo::i64_t begin_pos_0, const wchar_t* str1, ::pilo::i64_t begin_pos_1, ::pilo::i64_t length);

            

            /**
            * @brief                    Sets the first num bytes of the string buffer pointed by str to the specified value.
            * @param str [out]          the dst buffer to perform the filling. No term-0 will be append.
            * @param ch [in]            the char which used to fill the buffer
            * @param len [in]           the count of char to fill.
            * @see                      pea_string_setz_a
            * @see                      pea_string_setz_w
            * @note                     the buffer size should no less than len
            * @attention                [NPC-Free] [RC-Free] [MTS]
            */
            void set_character(char* str, char ch, ::pilo::i64_t len);
            void set_character(wchar_t* str, wchar_t ch, ::pilo::i64_t len);

            /**
            * @brief                    Sets the first num bytes of the string buffer pointed by str to the specified value.
            * @param str [out]          the dst buffer to perform the filling. will append term-0 at last.
            * @param ch [in]            the char which used to fill the buffer
            * @param len [in]           the count of char to fill.
            * @see                      pea_string_set_a
            * @see                      pea_string_set_w
            * @note                     the buffer size should greater than len
            * @attention                [NPC-Free] [RC-Free] [MTS]
            */
            void set_character_z(char* str, char ch, ::pilo::i64_t len);
            void set_character_z(wchar_t* str, wchar_t ch, ::pilo::i64_t len);


            template<typename CHAR_T>
            ::pilo::i64_t cstr_ptr_offset_chars_count(const CHAR_T* s, const CHAR_T* e)
            {
                return (e - s) / sizeof(CHAR_T);
            }


            template<typename CHAR_T>
            char* byte_to_bin_string(CHAR_T* dst_buffer, unsigned char v)
            {
                int idx = 0;
                for (int i = 7; i >= 0; i--)
                {
                    if (((v >> i) & 1) == 0)
                        dst_buffer[idx] = '0';
                    else
                        dst_buffer[idx] = '1';
                    idx++;
                }

                dst_buffer[idx] = 0;
                return dst_buffer;
            }

            template<typename CHAR_T, typename INT_T>
            CHAR_T* integer_to_binString(CHAR_T* dst_buffer, INT_T v, unsigned char hasb, bool need_sep)
            {
                INT_T vv = v;
                ::pilo::core::memory::bits_operation::switch_endian(vv, true);
                unsigned char* ptr = (unsigned char*) (&vv);
                CHAR_T* pdst = dst_buffer;
                
                for (int i = 0; i < sizeof(INT_T); i++)
                {
                    if (i > 0 && need_sep)
                    {
                        *pdst = ' ';
                        pdst++;
                    }
                    byte_to_bin_string(pdst, *(ptr + i));
                    pdst += 8;
                }             

				if (hasb == 'b' || hasb == 'B')
				{
					*pdst = hasb;
                    pdst++;
				}

                *pdst = 0;
                return dst_buffer;
            }

            /**
            * convert integer to string buffer
            * @param dst_buffer [out]    the start address of the buffer to save result string.
            * @param dst_len [in]        the length of the buffer by characters.
            * @param i [in]            the integer to write.
            * @param fmt [in]         the format of the string.
            * @return char*            the ptr to dest_buffer.
            */
            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::i8_t i, const char* fmt = nullptr);
            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::u8_t i, const char* fmt = nullptr);
            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::i16_t i, const char* fmt = nullptr);
            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::u16_t i, const char* fmt = nullptr);

            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::i32_t i, const char* fmt = nullptr);
            wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::i32_t i, const wchar_t* fmt = nullptr);
            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::i64_t i, const char* fmt = nullptr);
            wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::i64_t i, const wchar_t* fmt = nullptr);

            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::u32_t u, const char* fmt = nullptr);
            wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::u32_t u, const wchar_t* fmt = nullptr);
            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::u64_t u, const char* fmt = nullptr);
            wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::u64_t u, const wchar_t* fmt = nullptr);

            /**
            * convert float to string buffer
            * @param dst_buffer [out]   the start address of the buffer to save result string.
            * @param dst_len [in]       the length of the buffer by characters.
            * @param f [in]             the float to convert.
            * @param fmt [in]           the format of the float. such as  '4.3'
            * @return char*             the ptr to dest_buffer.
            */
            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::f32_t d, const char* fmt = nullptr);
            wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::f32_t d, const wchar_t* fmt = nullptr);
            char* number_to_string(char* dst_buffer, ::pilo::i64_t dst_len, pilo::f64_t d, const char* fmt = nullptr);
            wchar_t* number_to_string(wchar_t* dst_buffer, ::pilo::i64_t dst_len, pilo::f64_t d, const wchar_t* fmt = nullptr);

            /**
            * convert a string to number
            * @param v [out]            variable to receive the result, see overloads below.
            * @param pc [in]            the string to proceed
            * @return pilo::error       return EC_INVALID_PARAM if given string is null.
            * @see
            * @not
            */
            ::pilo::err_t string_to_number(::pilo::i32_t& v, const char* pc, ::pilo::i64_t len = -1);
            ::pilo::err_t string_to_number(::pilo::i64_t& v, const char* pc, ::pilo::i64_t len = -1);
            ::pilo::err_t string_to_number(::pilo::f32_t& v, const char* pc, ::pilo::i64_t len = -1);
            ::pilo::err_t string_to_number(::pilo::f64_t& v, const char* pc, ::pilo::i64_t len = -1);

            ::pilo::err_t string_to_number(::pilo::i32_t& v, const wchar_t* pc, ::pilo::i64_t len = -1);
            ::pilo::err_t string_to_number(::pilo::i64_t& v, const wchar_t* pc, ::pilo::i64_t len = -1);
            ::pilo::err_t string_to_number(::pilo::f32_t& v, const wchar_t* pc, ::pilo::i64_t len = -1);
            ::pilo::err_t string_to_number(::pilo::f64_t& v, const wchar_t* pc, ::pilo::i64_t len = -1);

            template<typename T>
            const char* number_to_string_adv(char* dst_buffer, ::pilo::i64_t dst_len, T value, const char* fmt, const char* readable_fmt)
            {
                if (readable_fmt == nullptr)
                {
                    return (const char*) number_to_string(dst_buffer, dst_len, value, fmt);
                }
                else
                {
                    const char* unit_str = "";
                    ::pilo::f64_t fv = (::pilo::f64_t)value;
                    if (fv >= 1000000000000000.0f)
                    {
                        fv /= 1000000000000000.0f;
                        unit_str = "P";
                    }
                    else if (fv >= 1000000000000.0f)
                    {
                        fv /= 1000000000000.0f;
                        unit_str = "T";
                    }
                    else if (fv >= 1000000000.0f)
                    {
                        fv /= 1000000000.0f;
                        unit_str = "G";
                    }
                    else if (fv >= 1000000.0f)
                    {
                        fv /= 1000000.0f;
                        unit_str = "M";
                    }
                    else if (fv >= 1000.0f)
                    {
                        fv /= 1000.0f;
                        unit_str = "K";
                    }
                    else
                    {
                        unit_str = "";
                    }                    
                    ::pilo::core::io::string_formated_output(dst_buffer, dst_len, readable_fmt, fv,unit_str);
                    return dst_buffer;
                }
            }
            
            





            /**
            * @brief                    trim white char from the left of a string, InPlace Mode
            * @param str [in/out]       the string your want to trim
            * @return                   the 1st not-white char in the original string.
            * @see                      right_trim_fixed trim_fixed movable_trim
            * @see                      right_trim_alloc left_trim_alloc trim_alloc
            * @note                     count of trimmed chars can be detected by returned ptr - str
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            CHAR_T* left_trim_inplace(CHAR_T* str)
            {
                CHAR_T* pb = str;
				while ((*pb == (CHAR_T)' ' || *pb == (CHAR_T)'\t' || *pb == (CHAR_T)'\n' || *pb == (CHAR_T)'\r') && *pb != (CHAR_T)'\0') pb++;
				return pb;
            }

            /**
            * @brief                    Appends the first src_length characters of src to dst, plus a term-0.If the length of the C string in src is less than num, only the content up to the terminating null-character is copied.
            * @param dst [in/out]       the dst buffer to perform the concatenate. term-0 will be append.
            * @param cc_dst_capa [in]   the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [int]          the src string to copy.
            * @param src_length [in]    the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @return                   string after concatenate.
            * @see                      concatenate
            * @note                     destination and source shall not overlap.
            * @attention                [NPC-Free] [MTS]
            */
            char* n_concatenate_inplace(char* ddst_bufferst, ::pilo::i64_t dst_capa, const char* src, ::pilo::i64_t src_length);
            wchar_t* n_concatenate_inplace(wchar_t* dst_buffer, ::pilo::i64_t dst_capa, const wchar_t* src, ::pilo::i64_t src_length);

            /**
            * @brief                    trim white char from the left of a string, Neo Mode
            * @param str [in/]          the string your want to trim
            * @param allocator [in]     the memory allocator used to create neo object.
            * @return                   the 1st not-white char in the original string.
            * @see                      right_trim_fixed left_trim_fixed trim_fixed movable_trim
            * @see                      right_trim_alloc trim_alloc
            * @note                     count of trimmed chars can be detected by returned ptr - str
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            CHAR_T* left_trim_neo(const CHAR_T* str)
            {
				const CHAR_T* pb = str;
				while ((*pb == (CHAR_T)' ' || *pb == (CHAR_T)'\t' || *pb == (CHAR_T)'\n' || *pb == (CHAR_T)'\r') && *pb != (CHAR_T)'\0')
				{
					pb++;
				}

				::pilo::i64_t sz = ::pilo::core::string::character_count(pb);
                CHAR_T* p = (CHAR_T*)PMF_HEAP_MALLOC((sz + 1)*sizeof(CHAR_T));
				if (p == nullptr)
				{
					return nullptr;
				}

				::pilo::core::string::n_copyz(p, sz + 1, pb, sz);

				return p;
            }


            /**
            * @brief                    trim white char from the right of a string [NPC-Free] InPlace Mode
            * @param str [in/out]       the string your want to trim
            * @param length [in]        the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @return                   how many chars was trimmed.
            * @see                      left_trim_fixed trim_fixed movable_trim
            * @see                      right_trim_alloc left_trim_alloc trim_alloc
            * @note                     count of trimmed chars can be detected by returned ptr - str
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            ::pilo::i64_t right_trim_inplace(CHAR_T* str, ::pilo::i64_t length)
            {
				if (str == nullptr)
				{
					return -1;
				}
				::pilo::i64_t n = 0;
                CHAR_T* pe = nullptr;
                
				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}
				pe = str + length- 1;
                CHAR_T* pe_orig = pe;
				while (pe != str && (*pe == (CHAR_T)' ' || *pe == (CHAR_T)'\t' || *pe == (CHAR_T)'\n' || *pe == (CHAR_T)'\r'))
				{
					n++;
					pe--;
				}
				if (pe < pe_orig)
				{
                    *(pe + 1) = (CHAR_T)'\0';
				}
				

				return n;
            }

            /**
            * @brief                    trim white char from the right of a string [NPC-Free] Neo Mode
            * @param str [in/out]       the string your want to trim
            * @param length [in]        the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @return                   how many chars was trimmed.
            * @see                      right_trim_fixed left_trim_fixed trim_fixed movable_trim
            * @see                      left_trim_alloc trim_alloc
            * @note                     count of trimmed chars can be detected by returned ptr - str
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            CHAR_T* right_trim_neo(const CHAR_T* str, ::pilo::i64_t length)
            {
				if (str == nullptr)
				{
					return nullptr;
				}
				::pilo::i64_t n = 0;

				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}
				const CHAR_T* pe = str + length - 1;
				while (pe != str && (*pe == (CHAR_T)' ' || *pe == (CHAR_T)'\t' || *pe == (CHAR_T)'\n' || *pe == (CHAR_T)'\r'))
				{
					n++;
					pe--;
				}
				::pilo::i64_t content_size = length - n;
                CHAR_T* p = (CHAR_T*)PMF_HEAP_MALLOC((content_size + 1) * sizeof(CHAR_T));
				if (p == nullptr)
				{
					return nullptr;
				}

				::pilo::core::string::n_copyz(p, content_size + 1, str, content_size);

				return p;
            }

            /**
            * @brief                    trim white char from the both side of a string [NPC-Free] InPlace Mode
            * @param str [in/out]       the string your want to trim
            * @param length [in]        the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @param ntrim [out]        ptr to a var to hold how many chars was removed. pass NULL if caller is not interested.
            * @return                   the 1st not-white char in the original string.
            * @see                      right_trim_fixed left_trim_fixed movable_trim
            * @see                      right_trim_alloc left_trim_alloc trim_alloc
            * @note                     count of trimmed chars can be detected by returned ptr - str
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            CHAR_T* trim_inplace(CHAR_T* str, ::pilo::i64_t length, ::pilo::i64_t* ntrim)
            {
				if (str == nullptr)
				{
					return nullptr;
				}

				::pilo::i64_t n = 0;
				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}

                CHAR_T* pb = str;
                CHAR_T* pe = str + length - 1;
                CHAR_T* pe_orig = pe;
				while ((*pb == (CHAR_T)' ' || *pb == (CHAR_T)'\t' || *pb == (CHAR_T)'\n' || *pb == (CHAR_T)'\r') && *pb != (CHAR_T)'\0')
				{
					n++;
					pb++;
				}
				while (pe != pb && (*pe == (CHAR_T)' ' || *pe == (CHAR_T)'\t' || *pe == (CHAR_T)'\n' || *pe == (CHAR_T)'\r'))
				{
					n++;
					pe--;
				}
				if (pe < pe_orig)
				{
					*(pe + 1) = (CHAR_T)'\0';
				}

				if (ntrim != nullptr)
				{
					*ntrim = n;
				}
				return pb;
            }


            /**
            * @brief                    trim white char from the both side of a string [NPC-Free] Neo Mode
            * @param str [in/out]       the string your want to trim
            * @param length [in]        the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @param ntrim [out]        ptr to a var to hold how many chars was removed. pass NULL if caller is not interested.
            * @return                   the 1st not-white char in the original string.
            * @see                      right_trim_fixed left_trim_fixed trim_fixed movable_trim
            * @see                      right_trim_alloc left_trim_alloc trim_alloc
            * @note                     count of trimmed chars can be detected by returned ptr - str
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            CHAR_T* trim_neo(const CHAR_T* str, ::pilo::i64_t length, ::pilo::i64_t* ntrim)
            {
				if (str == nullptr)
				{
					return nullptr;
				}

				::pilo::i64_t n = 0;
				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}

				const CHAR_T* pb = str;
				const CHAR_T* pe = str + length - 1;
				while ((*pb == (CHAR_T)' ' || *pb == (CHAR_T)'\t' || *pb == (CHAR_T)'\n' || *pb == (CHAR_T)'\r') && *pb != (CHAR_T)'\0')
				{
					n++;
					pb++;
				}
				while (pe != pb && (*pe == (CHAR_T)' ' || *pe == (CHAR_T)'\t' || *pe == (CHAR_T)'\n' || *pe == (CHAR_T)'\r'))
				{
					n++;
					pe--;
				}

				if (ntrim != nullptr)
				{
					*ntrim = n;
				}

				::pilo::i64_t content_size = length - n;
                CHAR_T* p = (CHAR_T*)PMF_HEAP_MALLOC((content_size + 1) * sizeof(CHAR_T));
				if (p == nullptr)
				{
					return nullptr;
				}

				::pilo::core::string::n_copyz(p, content_size + 1, pb, content_size);

				return p;
            }
           

            /**
            * @brief                    trim white char from the left of a string [NPC-Free]
            * @param str [in/out]       the string your want to trim
            * @return                   how many chars was trimmed.
            * @see                      right_trim_fixed left_trim_fixed movable_trim
            * @see                      right_trim_alloc left_trim_alloc trim_alloc trim_inplace
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            ::pilo::i64_t movable_ltrim(CHAR_T* str)
            {
                CHAR_T* pb = str;
				while ((*pb == (CHAR_T)' ' || *pb == (CHAR_T)'\t' || *pb == (CHAR_T)'\n' || *pb == (CHAR_T)'\r') && *pb != (CHAR_T)'\0') pb++;
                ::pilo::i64_t diff = (pb - str);
                if (diff > 0)
                {
                    ::pilo::core::string::movez(str, -1, pb);
                    return (::pilo::i64_t) (diff / sizeof(CHAR_T));
                }
                else if (diff == 0)
                    return 0;
                return -1;
            }

			template<typename CHAR_T>
            ::pilo::i64_t trim_calculate(::pilo::i64_t& beg, ::pilo::i64_t & end, CHAR_T* str, ::pilo::i64_t length)
            {
                ::pilo::i64_t beg_n = 0;
                ::pilo::i64_t end_n = 0;
				CHAR_T* pb = nullptr;
				CHAR_T* pe = nullptr;

                if (str == nullptr)
                {
					beg = -1;
					end = -1;
                    return -1;
                }

				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}
				if (*str == 0 || length == 0)
				{
					beg = 0;
					end = 0;
					return 0;
				}

				pb = str;
				pe = str + length - 1;

				while ((*pb == (CHAR_T)' ' || *pb == (CHAR_T)'\t' || *pb == (CHAR_T)'\n' || *pb == (CHAR_T)'\r') && *pb != (CHAR_T)'\0')
				{
                    beg_n++;
					pb++;
				}
                beg = beg_n;

				while (pe != pb && (*pe == (CHAR_T)' ' || *pe == (CHAR_T)'\t' || *pe == (CHAR_T)'\n' || *pe == (CHAR_T)'\r'))
				{
                    end_n++;
					pe--;
				}
                end = length - end_n;

				return end - beg;
            }

			template<typename CHAR_T>
			::pilo::err_t string_to_bool(bool& b, const CHAR_T* pc, ::pilo::i64_t len)
			{
				::pilo::i64_t beg = 0;
				::pilo::i64_t end = 0;
				::pilo::i64_t ret = ::pilo::core::string::trim_calculate(beg, end, pc, len);

				if (ret == -1)
				{
                    return ::pilo::mk_perr(PERR_NOOP);
				}

				if (ret == 1)
				{
					if (pc[beg] == 'T' || pc[beg] == 't')
					{
						b = true;
						return PILO_OK;
					}
					else if (pc[beg] == 'F' || pc[beg] == 'f')
					{
						b = false;
						return PILO_OK;
					}
					else
					{
                        return ::pilo::mk_perr(PERR_NOOP);
					}
				}
				else if (ret == 4)
				{
					if (::pilo::core::string::i_compare(pc, beg, "true", 0, 4) == 0)
					{
						b = true;
						return PILO_OK;
					}
				}
				else if (ret == 5)
				{
					if (::pilo::core::string::i_compare(pc, beg, "false", 0, 5) == 0)
					{
						b = false;
						return PILO_OK;
					}
				}

				return ::pilo::mk_perr(PERR_NOOP);
			}


            /**
            * @brief                    trim white char from the both side of a string [NPC-Free]
            * @param str [in/out]       the string your want to trim
            * @param length [in]        the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @return                   how many chars was trimmed.
            * @see                      right_trim_fixed left_trim_fixed trim_fixed
            * @see                      right_trim_alloc left_trim_alloc trim_alloc
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            ::pilo::i64_t movable_trim(CHAR_T* str, ::pilo::i64_t length)
            {
				::pilo::i64_t n = 0;
                CHAR_T* pb = nullptr;
                CHAR_T* pe = nullptr;

				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}
                if (length == 0)
                {
                    return 0;
                }                			

				pb = str;
				pe = str + length - 1;
                CHAR_T* pe_orig = pe;
				while ((*pb == (CHAR_T)' ' || *pb == (CHAR_T)'\t' || *pb == (CHAR_T)'\n' || *pb == (CHAR_T)'\r') && *pb != (CHAR_T)'\0')
				{
					n++;
					pb++;
				}
				while (pe != pb && (*pe == (CHAR_T)' ' || *pe == (CHAR_T)'\t' || *pe == (CHAR_T)'\n' || *pe == (CHAR_T)'\r'))
				{
					n++;
					pe--;
				}

                if (pe < pe_orig)
                {
                    *(pe + 1) = (CHAR_T)'\0';
                }

				if (pb != str)
				{
					::pilo::core::string::movez(str, -1, pb);
				}

				return n;
            }

            /**
            * @brief                    Appends a copy of the src string to the dst string.The term-0 in destination is overwritten by the first character of source, and a term-0 is included at the end of the new string formed by the concatenation of both in destination. InPlace Mode
            * @param dst [in/out]       the dst buffer to perform the concatenate. term-0 will be append.
            * @param cc_dst_capa [in]   the buffer size in char count. pass PEA_MI_INVALID_SIZE as infinite.
            * @param src [in]           the src string to copy.
            * @return                   string after concatenate.
            * @see                      n_concatenate
            * @note                     destination and source shall not overlap.
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            CHAR_T* concatenate_inplace(CHAR_T* dst, ::pilo::i64_t cc_dst_capa, const CHAR_T* src)
            {
                return ::pilo::core::string::n_concatenate_inplace(dst, cc_dst_capa, src, -1);
            }


            template<typename CHAR_T>
            CHAR_T* n_concatenate_neo(const CHAR_T* str1, ::pilo::i64_t str1_len, const CHAR_T* str2, ::pilo::i64_t str2_len)
			{
				if (str1 == nullptr && str2 == nullptr)
				{
					return nullptr;
				}

				if (str1 == nullptr)
				{
					return PMF_STRING_DUPLICATE_W(str2);
				}
				if (str2 == nullptr)
				{
					return PMF_STRING_DUPLICATE_W(str1);
				}

				if (str1_len == -1)
				{
					str1_len = PMF_STRING_LENGTH_W(str1);
				}

				if (str2_len)
				{
					str2_len = ::pilo::core::string::character_count(str2);
				}

				::pilo::i64_t content_size = str1_len + str1_len;
                CHAR_T* p = (CHAR_T*)PMF_HEAP_MALLOC((content_size + 1) * sizeof(CHAR_T));
				if (p == nullptr)
				{
					return nullptr;
				}

				::pilo::core::string::n_copy(p, content_size + 1, str1, str1_len);
				::pilo::core::string::n_copyz(p + str1_len, str2_len + 1, str2, str2_len);

				return p;
			}


            /**
            * @brief                    Appends a copy of the src string to the dst string.The term-0 in destination is overwritten by the first character of source, and a term-0 is included at the end of the new string formed by the concatenation of both in destination. Neo Mode
            * @param str1 [in]          the left string to concat.
            * @param str2 [in]          the right string to concat.
            * @return                   a neo string after concatenate.
            * @see                      n_concatenate                  
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            CHAR_T* concatenate_neo(const CHAR_T* str1, const CHAR_T* str2)
            {
                return n_concatenate_neo(str1, -1, str2, -1);
            }

            /**
            * @brief                    Find needle in the str.
            * @param str [in]           the string to search in.
            * @param needle [in]        the sub string to search in the str.
            * @param cc_search_max [in] the max count of char to search. pass PEA_MI_INVALID_SIZE as continue search utile term-0 is met.
            * @return                   a pointer to the first occurrence of needle in str, or a null pointer if needle is not part of str.
            * @see                      pea_string_rfindstr_a
            * @see                      pea_string_rfindstr_w
            * @attention                [NPC-Free] [MTS]
            */
            template<typename CHAR_T>
            CHAR_T* find_substring(const CHAR_T* str, const CHAR_T* needle, ::pilo::i64_t cc_search_max)
            {
				if (str == nullptr)
				{
					return nullptr;
				}

				::pilo::i64_t i = 0;
                ::pilo::i64_t remain = 0;
                ::pilo::i64_t needle_len = 0;

				if (needle == nullptr)
				{
					return (CHAR_T*)str;
				}
				else if (*needle == 0)
				{
					return (CHAR_T*)str;
				}

				needle_len = ::pilo::core::string::character_count(needle);

				if (str == nullptr || cc_search_max == 0)
				{
					return nullptr;
				}

				if (cc_search_max == -1)
				{
                    cc_search_max = ::pilo::core::string::character_count(str);
				}

				for (i = 0; i < cc_search_max; i++)
				{
					if (0 == str[i])
					{
						return nullptr;
					}
					remain = cc_search_max - i;
					if (needle_len > remain)
					{
						return nullptr;
					}
                    
					if (::pilo::core::string::strict_compare(str, i, needle, 0, needle_len) == 0)
					{
						return (CHAR_T*)(str + i);
					}
				}

				return nullptr;
            }

            template<typename CHAR_T>
            CHAR_T* find_substring(const CHAR_T* str, ::pilo::i64_t srclen,  const CHAR_T* needle,  ::pilo::i64_t needle_len, ::pilo::i64_t &needle_match_len)
            {
                needle_match_len = 0;

                if (str == nullptr)
                {
                    return nullptr;
                }
                ::pilo::i64_t i = 0;             
                if (needle == nullptr)
                {
                    return (CHAR_T*)str;
                }
                else if (*needle == 0)
                {
                    return (CHAR_T*)str;
                }
                if (needle_len < 1)
                    needle_len = ::pilo::core::string::character_count(needle);

                if (str == nullptr || srclen == 0)
                {
                    return nullptr;
                }

                if (srclen == -1)
                {
                    srclen = ::pilo::core::string::character_count(str);
                }

                ::pilo::i64_t matching_idx = 0;
                for (i = 0; i < srclen; i++)
                {
                    if (0 == str[i])
                    {
                        return nullptr;
                    }
                    if (str[i] == needle[matching_idx]) {                        
                        matching_idx++;
                        needle_match_len = matching_idx;
                        if (needle_len == matching_idx) {
                            return (CHAR_T*) (str + i + 1 - needle_len);
                        }
                    } else if (str[i] == needle[0]) {
                        matching_idx = 1;
                        needle_match_len = 1;
                        if (needle_len == matching_idx) {
                            return (CHAR_T*)(str + i + 1 - needle_len);
                        }
                    }
                    else {
                        matching_idx = 0;
                        needle_match_len = 0;
                    }

                }

                return nullptr;
            }

            template<typename CHAR_T>
            CHAR_T* find_substring(const CHAR_T* str, ::pilo::i64_t srclen, const CHAR_T* needle, ::pilo::i64_t needle_len)
            {
                if (str == nullptr)
                {
                    return nullptr;
                }

                ::pilo::i64_t i = 0;
                ::pilo::i64_t remain = 0;

                if (needle == nullptr)
                {
                    return (CHAR_T*)str;
                }
                else if (*needle == 0)
                {
                    return (CHAR_T*)str;
                }

                if (needle_len < 1)
                    needle_len = ::pilo::core::string::character_count(needle);

                if (str == nullptr || srclen == 0)
                {
                    return nullptr;
                }

                if (srclen == -1)
                {
                    srclen = ::pilo::core::string::character_count(str);
                }

                for (i = 0; i < srclen; i++)
                {
                    if (0 == str[i])
                    {
                        return nullptr;
                    }
                    remain = srclen - i;
                    if (needle_len > remain)
                    {
                        return nullptr;
                    }

                    if (::pilo::core::string::strict_compare(str, i, needle, 0, needle_len) == 0)
                    {
                        return (CHAR_T*)(str + i);
                    }
                }

                return nullptr;
            }

            /**
            * @brief                    Find needle in the str reversely.
            * @param str [in]           the string to search in.
            * @param needle [in]        the sub string to search in the str.
            * @param cc_search_max [in] the max count of char to search. (still from left) pass PEA_MI_INVALID_SIZE as continue search utile term-0 is met.
            * @return                   a pointer to the first occurrence of needle in str, or a null pointer if needle is not part of str.
            * @see                      pea_string_findstr_a
            * @see                      pea_string_findstr_w
            * @attention                [NPC-Free] [MTS]
            */
			template<typename CHAR_T>
            CHAR_T* rfind_substring(const CHAR_T* str, const CHAR_T* needle, ::pilo::i64_t cc_search_max)
            {
				if (str == nullptr)
				{
					return nullptr;
				}

				::pilo::i64_t i = 0;
                ::pilo::i64_t remain = 0;
                ::pilo::i64_t needle_len = 0;

				if (needle == nullptr)
				{
					return (CHAR_T*)str;
				}
				else if (*needle == 0)
				{
					return (CHAR_T*)str;
				}

				needle_len = ::pilo::core::string::character_count(needle);

				if (str == nullptr || cc_search_max == 0)
				{
					return nullptr;
				}

				if (cc_search_max == -1)
				{
					cc_search_max = ::pilo::core::string::character_count(str);
				}

				for (i = (::pilo::i64_t)cc_search_max - 1; i >= 0; i--)
				{
					remain = cc_search_max - i;
					if (needle_len > remain)
					{
						continue;;
					}

					if (strict_compare(str, i, needle, 0, needle_len) == 0)
					{
						return (CHAR_T*)(str + i);
					}
				}

				return nullptr;
            }

            

            /**
            * @brief                    replace a src sub string to dst sub string after each time it was found, and then search again until it is not found.
            * @param src [in/out]       the string to proceed with
            * @param cc_src_length [in] the length of the string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @param cc_src_buffer [in] the capacity of the string, pass PEA_MI_INVALID_SIZE as large enough to hold the result.
            * @param needle [in]        the src sub string you would like to find and replace
            * @param replacement [in]   the dst sub string you would like to change to.
            * @param rcount [out]       the count of place that replacement is occurred.
            * @return                   error code.
            *   @retval                   EC_OK on replace is occurred at least once and success.
            *   @retval                   EC_BUFFER_TOO_SMALL on the dst buffer is not large enough to hold the result.
            *   @retval                   EC_NULL_PARAM on src or dst buffer param is NULL
            *   @retval                   EC_INFINITE_LOOP on the replacement contain the needle substring.
            * @note                     caller must ensure the replacement does NOT contain the needle.
            * @attention                [NPC] [RC] [MTS]
            */
            template<typename CHAR_T>
            ::pilo::err_t rescanable_replace_inplace(CHAR_T* src, ::pilo::i64_t cc_src_length, ::pilo::i64_t cc_src_buffer, const CHAR_T* needle, const CHAR_T* replacement, ::pilo::i64_t* rcount)
            {
                ::pilo::i64_t cc_rep = 0;
                ::pilo::i64_t cc_needle = 0;
				::pilo::i32_t need_enlarge = 0;
                ::pilo::i64_t diff = 0;
                ::pilo::i64_t diff_count = 0;
                CHAR_T* found_str = 0;
                ::pilo::i64_t tmp_remains = 0;

				if (rcount != nullptr)
				{ //set result count properly at first;
					*rcount = 0;
				}

				if (src == nullptr)
				{
					return ::pilo::mk_perr(PERR_NULL_PARAM);
				}

				if (cc_src_length == -1)
				{
					cc_src_length = ::pilo::core::string::character_count(src);
				}

				if (cc_src_buffer <= cc_src_length)
				{
					return ::pilo::mk_perr( PERR_VAL_TOO_SAMLL);
				}

				if (replacement == nullptr)
				{
					return ::pilo::mk_perr(PERR_NULL_PARAM);
				}

				if (needle == nullptr || *needle == 0)
				{
					return ::pilo::mk_perr(PERR_NULL_PARAM);
				}

				if (::pilo::core::string::find_substring((CHAR_T*)replacement, needle, -1))
				{
					if (rcount != nullptr)
					{
						*rcount = -1;
					}
					return ::pilo::mk_perr(PERR_INF_LOOP);
				}

				cc_rep = ::pilo::core::string::character_count(replacement);
				cc_needle = ::pilo::core::string::character_count(needle);

				if (cc_rep > cc_needle)
				{
					need_enlarge = 1; //true
					diff = cc_rep - cc_needle;

				}
				else if (cc_rep < cc_needle)
				{
					need_enlarge = 0; //false
					diff = cc_needle - cc_rep;
				}
				else
				{
					need_enlarge = -1;//means same size
					diff = 0;
				}

				found_str = src;
				while ((found_str = ::pilo::core::string::find_substring(found_str, needle, -1)) != nullptr)
				{
					diff_count++; //increase the found counter 
					if (1 == need_enlarge)
					{ //check if the buffer is sufficient 
						if (cc_src_length + diff >= cc_src_buffer)
						{
							if (rcount != nullptr)
							{
								*rcount = diff_count - 1;
							}
							return ::pilo::mk_perr( PERR_VAL_TOO_SAMLL);
						}
						tmp_remains = cc_src_length - (found_str - src) - cc_needle;
						cc_src_length += diff;
						::pilo::core::string::n_movez(found_str + cc_rep, cc_src_buffer, found_str + cc_needle, tmp_remains);

					}
					else if (0 == need_enlarge)
					{
						tmp_remains = cc_src_length - (found_str - src) - cc_needle;
						::pilo::core::string::n_movez(found_str + cc_rep, cc_src_buffer, found_str + cc_needle, tmp_remains);
						cc_src_length -= diff;
					}

					src[cc_src_length] = '\0';

					// do replacement or removal 
					if (0 != cc_rep)
					{ //replace, remove otherwise
						::pilo::core::string::n_move(found_str, cc_src_buffer, replacement, cc_rep);
					}

					if (src + cc_needle < found_str)
					{
						found_str -= cc_needle;
					}
					else
					{
						found_str = src;
					}
				}

				if (rcount != 0)
				{
					*rcount = diff_count;
				}

				return PILO_OK;
            }

            /**
            * @brief                    insert a sub string in the original string.
            * @param src [in/out]       the original string which to be inserted into
            * @param cc_src_length [in] the length of the original string, pass PEA_MI_INVALID_SIZE to auto calculate.
            * @param cc_src_buffer [in] the capacity of the original string, pass PEA_MI_INVALID_SIZE as large enough to hold the result.
            * @param pos                the index where insertion began.
            * @param str [in]           the sub string you would like to insert
            * @param len [in]           the char count of the sub string
            * @return                   error code.
            *                           EC_OK on replace is occurred or needle is not found.
            *                           EC_BUFFER_TOO_SMALL on the dst buffer is not large enough to hold the result.
            *                           EC_NULL_PARAM on src or dst buffer param is NULL
            * @see                      pea_string_n_concatenate_a pea_string_n_concatenate_w
            * @note                     if the insertion pos is at the end of the string, eg. pass PEA_MI_INVALID_SIZE as the original string length.
            *                           the func behave like pea_string_n_concatenate
            * @attention                [NPC] [RC] [MTS]
            */
            template<typename CHAR_T>
            ::pilo::err_t insert_substring_inplace(CHAR_T* src, ::pilo::i64_t cc_src_length, ::pilo::i64_t cc_src_buffer, ::pilo::i64_t pos, const CHAR_T* str, ::pilo::i64_t len)
            {
				::pilo::i64_t part_len = 0;

				if (src == nullptr)
				{
					return ::pilo::mk_perr(PERR_NULL_PARAM);
				}

				if (cc_src_length == -1)
				{
					cc_src_length = ::pilo::core::string::character_count(src);
				}

				if (str == nullptr || *str == 0)
				{
					return PILO_OK;
				}

				if (len == -1)
				{
					len = ::pilo::core::string::character_count(str);
				}

				if (pos == -1)
				{
					pos = cc_src_length;
				}
				else if (pos > cc_src_length)
				{
					pos = cc_src_length;
				}

				if (cc_src_length + len >= cc_src_buffer)
				{
					return ::pilo::mk_perr( PERR_VAL_TOO_SAMLL);
				}

				if (pos == len)
				{
					if (nullptr == ::pilo::core::string::n_concatenate_inplace(src, cc_src_buffer, str, len))
					{
						return ::pilo::mk_perr( PERR_VAL_TOO_SAMLL);
					}
				}
				else
				{
					part_len = ::pilo::core::string::character_count(src + pos);
					::pilo::core::string::n_movez(src + pos + len, cc_src_buffer, src + pos, part_len);
					::pilo::core::string::n_move(src + pos, pos + len + 1, str, len);
				}

				return PILO_OK;
            }            

            /**
            * duplicate a string, Neo Mode
            * @param str [in]          the source string to duplicate
            * @param length [in]        the characters to duplicate, auto calculate if -1 is given.
            * @param allocator [in]     the memory allocator used to create neo object.
            * @return pilo::i32_t       return dest string, need caller to destroy it if it is not NULL.
            */
            template<typename CHAR_T>
            CHAR_T* duplicate(const CHAR_T* str, ::pilo::i64_t length)
            {
				if (str == nullptr)
				{
					return nullptr;
				}

				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}

                CHAR_T* dst = (CHAR_T*)PMF_HEAP_MALLOC((length + 1) * sizeof(CHAR_T));
				if (dst == nullptr)
				{
					return nullptr;
				}

				::pilo::core::string::n_copyz(dst, length + 1, str, length);

				return dst;
            }

			template<typename CHAR_T>
			const CHAR_T* rfind_char(const CHAR_T* str, ::pilo::i64_t length, CHAR_T v)
			{
				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}
				return ::pilo::core::algorithm::find_reversely_in_array_by_value<CHAR_T>(str, length, v);
			}

			template<typename CHAR_T>
			const CHAR_T* find_char(const CHAR_T* str, ::pilo::i64_t length, CHAR_T v)
			{
				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}
				return ::pilo::core::algorithm::find_in_array_by_value<CHAR_T>(str, length, v);
			}

            template<typename CHAR_T>
            ::pilo::i64_t replace_char(CHAR_T* str, ::pilo::i64_t length, CHAR_T c, CHAR_T rep)
            {
				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}
                return ::pilo::core::algorithm::replace_in_array_by_value(str, length, c, rep, -1);
            }



            template<typename CHAR_T>
            void* forward_section_iterate(const CHAR_T*                      s,
				::pilo::i64_t                                                s_len,
				const CHAR_T*                                               delimiters, 
				::pilo::i64_t                                                delimiter_len, 
				std::function<void*(const char*,
											::pilo::i64_t idx, 
											const char* start_ptr, 
											::pilo::i64_t sec_len,  
                                            ::pilo::u32_t   flag,
                                            ::pilo::err_t& error_ind,
											void * context,
                                            void * arg
                )>                                                          cb,
                ::pilo::err_t&                                              error_ind,
                void*                                                       context,
                void*                                                       arg
            )
            {
                ::mbstate_t mb = ::mbstate_t();
                ::pilo::i64_t length = 0;
                void* pctx = context;
                if (s == nullptr || s[0] == 0)
                {
                    error_ind = -1;
                    return nullptr;
                }
                    
                if (s_len == -1)
                    s_len = ::pilo::core::string::character_count(s);

                if (delimiters == nullptr || *delimiters == 0)
                {
                    return  cb(s, 0, s, s_len, PMI_STRING_LAST_SECTION_FLAG, error_ind, pctx, arg);
                }

                if (delimiter_len == -1)
                {
                    delimiter_len = ::pilo::core::string::character_count(delimiters);
                }

                const char* p = s;
                const char* p_start = p;
                ::pilo::i64_t idx = 0;
                
                while (p < s + s_len)
                {
                    length = mbrlen(p, 6, &mb);
                    ::pilo::i64_t n = pilo_max((::pilo::i64_t)1, length);
                    if (n == 1 && ::pilo::core::string::find_char(delimiters, delimiter_len, p[0]) != nullptr)
                    {
                        ::pilo::u32_t flag = 0;
                        if (p >= s + s_len - 1)
                            flag |= PMI_STRING_LAST_SECTION_FLAG;
                        ::pilo::i64_t tmp_cnt = cstr_ptr_offset_chars_count(p_start, p);
                        pctx = cb(s, idx++, p_start, tmp_cnt, flag, error_ind, pctx, arg);
                        if (error_ind != 0)
                        {
                            return pctx;
                        }                

                        p += n;
                        p_start = p;
                    }
                    else
                    {
                        p += n;
                    }
                }

                if (p > p_start)
                {
                    ::pilo::i64_t tmp_cnt = cstr_ptr_offset_chars_count(p_start, p);
                    return cb(s, idx++, p_start, tmp_cnt, PMI_STRING_LAST_SECTION_FLAG, error_ind, pctx, arg);
                }

                return pctx;
            }

            template<typename CHAR_T>
            const CHAR_T* trim_readonly(const CHAR_T* str, ::pilo::i64_t len, ::pilo::i64_t& rlen)
            {
                const CHAR_T* pb = str;
                if (len == -1)
                    len = ::pilo::core::string::character_count(str);
                while ((*pb == (CHAR_T)' ' || *pb == (CHAR_T)'\t' || *pb == (CHAR_T)'\n' || *pb == (CHAR_T)'\r') && *pb != (CHAR_T)'\0')
                {
                    pb++;
                    rlen++;
                }
                
                const CHAR_T* pe = str + len - 1;
                while (pe != str && (*pe == (CHAR_T)' ' || *pe == (CHAR_T)'\t' || *pe == (CHAR_T)'\n' || *pe == (CHAR_T)'\r'))
                {
                    pe--;
                    rlen++;
                }

                return pb;
            }

            template<typename CHAR_T>
            const ::pilo::i64_t split_fixed(const CHAR_T* src, ::pilo::i64_t srclen
                , const CHAR_T* delim, ::pilo::i64_t delimlen
                , ::pilo::cstr_ref<CHAR_T>* ret_parts, ::pilo::i64_t max_parts
                , bool filter_empty
                , bool whole_when_no_delim_found
                , bool trim, bool trim_part 

            )
            {
                if (src == nullptr || *src == 0 || ret_parts == nullptr)
                    return -1;
                if (srclen == -1)
                    srclen = ::pilo::core::string::character_count(src);
                if (delimlen == -1)
                    delimlen = ::pilo::core::string::character_count(delim);
                bool has_found = false;

                ::pilo::i64_t src_rlen = srclen; //len after trimed, real length
                ::pilo::i64_t trimmed_len = 0;
                const char* pstr = nullptr;
                if (trim)
                {
                    pstr = trim_readonly(src, srclen, trimmed_len);
                    src_rlen -= trimmed_len;
                }                    
                else
                    pstr = src;
                const char* pend = pstr + src_rlen;

                ::pilo::i64_t ret_count = 0;
                bool ret_tmp = 0;
                while (ret_count < max_parts)
                {
                    CHAR_T* ret_ptr = find_substring(pstr, src_rlen, delim, delimlen);
                    if (ret_ptr == nullptr)
                    {
                        if ((! has_found) && (! whole_when_no_delim_found))
                        {
                            return ret_count;
                        }
                        else
                        {
                            if (trim_part)
                            {
                                ::pilo::i64_t tmppartlen = 0;
                                pstr = trim_readonly(pstr, src_rlen, tmppartlen);
                                src_rlen -= tmppartlen;
                            }                                

                            ret_tmp = ret_parts[ret_count].init(pstr, src_rlen);
                            if (! ret_tmp)
                                return ret_count;
                            if (ret_parts[ret_count].is_empty() && filter_empty)
                                return ret_count;
                            ret_count++;
                        }

                        return ret_count;
                    }
                    else if (ret_ptr == pstr)
                    {
                        if (!filter_empty)
                        {
                            ret_parts[ret_count].init(pstr, ret_ptr);
                            ret_count++;
                        }
                        ret_ptr++;
                        if (ret_ptr >= pend)
                        {
                            return ret_count;
                        }

                        pstr = ret_ptr;
                        if (ret_ptr >= pend)
                        {
                            return ret_count;
                        }

                        src_rlen--;
                        if (src_rlen == 0)
                            return ret_count;
                    }
                    else
                    {
                        has_found = true;
                        if (trim_part)
                        {
                            ::pilo::i64_t tmppartlen = 0;
                            pstr = trim_readonly(pstr, (ret_ptr - pstr) / sizeof(CHAR_T), tmppartlen);
                            src_rlen -= tmppartlen;
                        }
                        ret_tmp = ret_parts[ret_count].init(pstr, ret_ptr);
                        if (!ret_tmp || ret_parts[ret_count].ptr == nullptr)
                            return ret_count;
                        src_rlen -= ret_parts[ret_count].length;
                        if (!ret_parts[ret_count].is_empty() || !filter_empty)
                        {
                            ret_count++;
                        }

                        pstr = ret_ptr + 1;
                        if (ret_ptr >= pend)
                        {
                            return ret_count;
                        }

                        src_rlen--;
                        if (src_rlen == 0)
                            return ret_count;
                    }                    
                }               

                return ret_count;
            }

            template<typename CHAR_T>
            const ::pilo::i64_t split_fixed(::pilo::cstr_ref<CHAR_T>* src
                , const CHAR_T* delim, ::pilo::i64_t delimlen
                , ::pilo::cstr_ref<CHAR_T>* ret_parts, ::pilo::i64_t max_parts
                , bool filter_empty
                , bool whole_when_no_delim_found
                , bool trim, bool trim_part

            )
            {
                if (src == nullptr || src->is_empty()) return -1;

                return split_fixed(src->ptr, src->length, delim, delimlen, ret_parts, max_parts, 
                    filter_empty, whole_when_no_delim_found, trim, trim_part);
            }

            template<typename CHAR_T>
            ::pilo::i64_t split_inplace(CHAR_T* s, const CHAR_T* delimiters, ::pilo::i64_t delimiter_len, CHAR_T* ss[], ::pilo::i64_t maxs)
            {
				::mbstate_t mb = ::mbstate_t();
				::pilo::i64_t i = 0;
				::pilo::i64_t length = 0;

                if (s == nullptr) return 0;

                if (delimiters == nullptr || *delimiters == 0)
                {
                    ss[0] = s;
                    return 1;
                }
                
                if (delimiter_len == -1)
                {
                    delimiter_len = ::pilo::core::string::character_count(delimiters);
                }

				ss[i++] = s;
				while (s[0] && i < maxs)
				{
					length = mbrlen(s, 6, &mb);
					::pilo::i64_t n = pilo_max((::pilo::i64_t)1, length);
					if (n == 1 && ::pilo::core::string::find_char(delimiters, delimiter_len, s[0]) != nullptr)
					{
						s[0] = '\0'; s += n;
						while ((n = mblen(s, 6)) == 1 && ::pilo::core::string::find_char(delimiters, delimiter_len, s[0]) != nullptr)
                            s++; //TODO mblen 
						ss[i++] = s;
					}
					else
					{
						s += n;
					}
				}
				return i;
            }

			

            template<typename CHAR_T>
            const CHAR_T* get_trim_info(const CHAR_T* str, ::pilo::i64_t length, ::pilo::i64_t* ret_len)
            {
				if (str == nullptr)
				{
					return nullptr;
				}

				::pilo::i64_t n = 0;
				if (length == -1)
				{
					length = ::pilo::core::string::character_count(str);
				}

				const CHAR_T* pb = str;
				const CHAR_T* pe = str + length - 1;
                
				while ((*pb == (wchar_t)' ' || *pb == (wchar_t)'\t' || *pb == (wchar_t)'\n' || *pb == (wchar_t)'\r') && *pb != (wchar_t)'\0')
				{
					n++;
					pb++;
				}
				while (pe != pb && (*pe == (wchar_t)' ' || *pe == (wchar_t)'\t' || *pe == (wchar_t)'\n' || *pe == (wchar_t)'\r'))
				{
					n++;
					pe--;
				}

				if (ret_len != nullptr)
				{
					*ret_len = length - n;
				}

				return pb;
            }


			template<typename INT_T, typename CHAR_T>
			inline static const CHAR_T* bincstr_from_int(INT_T v, CHAR_T* buffer, CHAR_T sep_char, CHAR_T leading, CHAR_T tailing, leading_trim_mode_enum ltm, int endian)
			{

				::pilo::i32_t widx = 0;

				if (leading != 0)
				{
					buffer[0] = leading;
					widx = 1;
				}

				if (endian == eLittleEndian)
				{
					unsigned char* p = (unsigned char*)&v + sizeof(v) - 1;
					for (int i = 0; i < sizeof(v); i++)
					{
						if (*p != 0 || ltm == leading_trim_mode_enum::eNone)
						{
							memcpy(buffer + widx, ::pilo::core::string::constants<CHAR_T>::bincstr(*p), 8);
							widx += 8;
							if (sep_char != 0 && i < sizeof(v) - 1)
							{
								buffer[widx] = sep_char;
								widx++;
							}
						}
						p--;
					}

				}
				else
				{
					unsigned char* p = (unsigned char*)&v;
					for (int i = 0; i < sizeof(v); i++)
					{
						if (*p != 0 || ltm == leading_trim_mode_enum::eNone)
						{
							memcpy(buffer + widx, ::pilo::core::string::constants<CHAR_T>::bincstr(*p), 8);
							widx += 8;
							if (sep_char != 0 && i < sizeof(v) - 1)
							{
								buffer[widx] = sep_char;
								widx++;
							}
						}
						p++;
					}
				}

				if (tailing != 0)
				{
					buffer[widx] = tailing;
					widx++;
				}

				buffer[widx] = 0;

				int len = ::pilo::core::string::character_count(buffer);
				int start_idx = leading == 0 ? 0 : 1;
				int final_idx = -1;
				if (ltm == leading_trim_mode_enum::eAll)
				{
					for (int i = start_idx; i < len; i++)
					{
						if (buffer[i] != '0')
						{
							final_idx = i;
							break;
						}
					}
					memmove(buffer, buffer + final_idx, len - final_idx);
					buffer[len - final_idx] = 0;
				}


				return buffer;
			}

            template<typename CHAR_T>
            CHAR_T* cstr_fill_charnum_rand(CHAR_T* str, ::pilo::i64_t len)
            {                
				for (::pilo::i64_t i = 0; i < len - 1; i++)
				{
                    int idx = rand() % ::pilo::core::string::constants<CHAR_T>::alpha_num_length();
                    str[i] = ::pilo::core::string::constants<CHAR_T>::alpha_num()[idx];
				}
					
				str[len - 1] = 0;
                return str;
            }            

            template<typename CHAR_T>
            const CHAR_T* extract_pinch_nocheck(
                const CHAR_T* str,
                ::pilo::i64_t slen,
                CHAR_T cb,
                CHAR_T ce,
                ::pilo::i64_t* ri_len,
                ::pilo::i64_t* rs_len
            )
            {
                if (str == nullptr || slen < 2)
                {
                    if (ri_len != nullptr) *ri_len = 0;
                    if (rs_len != nullptr) *rs_len = slen;
                    return nullptr;
                }
                if (slen == -1)
                {
                    slen = ::pilo::core::string::character_count(str);
                }

                const CHAR_T* p = ::pilo::core::string::find_char(str, slen, cb);
                if (p == nullptr)
                {
                    if (ri_len != nullptr) *ri_len = 0;
                    if (rs_len != nullptr) *rs_len = slen;
                    return nullptr;
                }

                p++;

                ::pilo::i64_t take_len = cstr_ptr_offset_chars_count(str, p);
                
                if (take_len >= slen)
                {
                    if (ri_len != nullptr) *ri_len = 0;
                    if (rs_len != nullptr) *rs_len = slen;
                    return nullptr;
                }
                ::pilo::i64_t left_len = slen - take_len;

                const CHAR_T* p2 = ::pilo::core::string::find_char(p, left_len, ce);
                if (p2 == nullptr)
                {
                    if (ri_len != nullptr) *ri_len = 0;
                    if (rs_len != nullptr) *rs_len = slen;
                    return nullptr;
                }

                left_len = p2 - p;
                if (left_len == 0)
                {
                    if (ri_len != nullptr) *ri_len = 0;
                    if (rs_len != nullptr) *rs_len = cstr_ptr_offset_chars_count(str, p) - 1;
                    return "";
                }

                if (ri_len != nullptr) *ri_len = left_len;
                if (rs_len != nullptr) *rs_len = cstr_ptr_offset_chars_count(str, p) - 1;
                return p;
            }

            template<typename CHAR_T>
            const CHAR_T* extract_pinch(
                const CHAR_T* str,
                ::pilo::i64_t slen,
                CHAR_T cb,
                CHAR_T ce,
                ::pilo::i64_t* ri_len,
                ::pilo::i64_t* rs_len
            )
            {
                return extract_pinch_nocheck<CHAR_T>(str, slen, cb, ce, ri_len, rs_len);
            }

            template<typename TA_LEN>
            std::string cstr_to_str(const char* v, TA_LEN len, ::pilo::err_t* err)
            {
                if (v == nullptr)
                {
                    if (err != nullptr)
                        *err = ::pilo::mk_perr(PERR_NULL_PARAM);
                    return std::string("");
                }
                else if (len < 0)
                {
                    len = (TA_LEN) ::pilo::core::string::character_count(v);
                    if (len <= 0)
                    {
                        return std::string("");
                    }
                }
                return std::string(v, len);
            }

            //template<typename CHAR_T>
            //const CHAR_T* extract_array_name_index(const CHAR_T str,
            //    ::pilo::i64_t slen,
            //    ::pilo::i64_t& name_rlen,
            //    ::pilo::i64_t& idx_rlen)
            //{
            //    if (str == nullptr)
            //        return nullptr;

            //    ::pilo::core::string::find_char(str, slen, '[')

            //    return str;
            //}

            //1024 KB
            //1024*1024 MB
            //1024*1024*1024 GB
            //1024 * 1024 * 1024 * 1024 TB
            //1024 * 1024 * 1024 * 1024 * 1024 PB
            // 1024 * 1024 * 1024 * 1024 * 1024 * 1024 EB 
            
            template<typename T>
            const char* size_to_cstr(char* buffer, ::pilo::i32_t bufsz, T szn, ::pilo::i32_t prec, const char* suffix)
            {
                if (suffix == nullptr)
                    suffix = "";
                char fmt[16] = {0};
                ::pilo::f64_t v = (::pilo::f64_t) szn;
                int cnt = 0;
                char unit = ' ';
                if ((::pilo::i64_t)szn >= ((::pilo::i64_t) 1024 * 1024 * 1024 * 1024 * 1024))
                {
                    cnt = 5;
                    unit = 'P';
                }
                else if ((::pilo::i64_t)szn >= ((::pilo::i64_t)1024 * 1024 * 1024 * 1024))
                {
                    cnt = 4;
                    unit = 'T';
                }
                else if ((::pilo::i64_t)szn >= ((::pilo::i64_t)1024 * 1024 * 1024))
                {
                    cnt = 3;
                    unit = 'G';
                }
                else if ((::pilo::i64_t)szn >= ((::pilo::i64_t)1024 * 1024))
                {
                    cnt = 2;
                    unit = 'M';
                }
                else if ((::pilo::i64_t)szn >= ((::pilo::i64_t)1024))
                {
                    cnt = 1;
                    unit = 'K';
                }

                for (::pilo::i32_t i = 0; i < cnt; i++)
                {
                    v /= 1024.f;
                }

                ::pilo::core::io::string_formated_output(fmt, sizeof(fmt), "%%.%df%%c%%s", prec, unit,suffix);

                ::pilo::core::io::string_formated_output(buffer, bufsz, fmt, v);

                return buffer;
            }

            
            template<typename TA_RAC>
            ::pilo::err_t concatenate_rac(::pilo::char_buffer_t* buffer, const TA_RAC& rac, const char* sep, ::pilo::i32_t sep_len)
            {
                if (sep == nullptr)
                {
                    sep = "";
                }
                if (sep_len < 0)
                {
                    sep_len = (::pilo::i32_t) ::pilo::core::string::character_count(sep);
                }
                ::pilo::i32_t csize = 0;
                typename TA_RAC::const_iterator cit = rac.cbegin();
                for (; cit != rac.cend(); cit++)
                {
                    csize += (::pilo::i32_t) (*cit)->length;
                    csize += sep_len;
                }
                csize++;
                buffer->check_more_space(csize);
                cit = rac.cbegin();
                for (; cit != rac.cend(); cit++)
                {
                    if (cit != rac.cbegin())
                    {
                        *buffer->ptr() = PMI_PATH_SEP;
                        buffer->add_size((::pilo::i32_t)1);
                    }
                    ::pilo::core::string::n_copyz(buffer->ptr(), buffer->space_available(), (*cit)->ptr, (*cit)->length);
                    buffer->add_size((::pilo::i32_t)(*cit)->length);

                }
                return PILO_OK;
            }
            
        }
    }
}
