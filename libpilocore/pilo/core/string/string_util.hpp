#pragma once

#include "../coredefs.hpp"
#include <cstring>

#if defined(WIN32) || defined(WINDOWS)
#   pragma comment(lib, "Ws2_32.lib")
#endif

namespace pilo
{
    namespace core
    {
        namespace string
        {
            enum EnumCharsetEncodeType
            {
                eEncodeMBCS, /**< multi-bytes encode*/
                eEncodeUTF8, /**< utf8 encode */
            };

            class string_util
            {
                public:          

                    static bool is_invalid_filename_char(const char x);
                    static bool is_invalid_path_char(const char x, size_t pos);

                    /***
                    * get string length, in character count, not bytes.
                    * @param str[in]           the string your want to get it's length.
                    * @return characters in the string, '\0' not included.
                    * @see element_count
                    * @note  can process char and wide char type string. use element_count if string is
                    *         variable-length encoded string, such as utf8 or MBCS encoded.
                    */
                    static size_t length(const char* str);
                    static size_t length(const wchar_t* str);

                    /***
                    * get string length of variable-length type, in character count, not bytes.
                    * @param str[in]            the string your want to get it's length.
                    * @param eType[in]          the type of the string.
                    * @return characters in the string, '\0' not included.
                    * @see length
                    * @note  for variable-length string.
                    */
                    static size_t element_count(const char* str, pilo::core::string::EnumCharsetEncodeType eType);
                    static size_t element_count(const wchar_t* str, pilo::core::string::EnumCharsetEncodeType eType);
                    /***
                    * copy string from source string to destination string.
                    * @param dst_str[out]       the start address of the destination buffer.
                    *                           no content will be copied if this param is null.
                    * @param dst_pos[in]        the offset to the start address of the destination buffer.
                    *                           no content will be copied if this param is not less than dst_capacity - 1.
                    * @param dst_capacity[in]   the total size of the destination buffer.
                    *                           pass MC_INVALID_SIZE means dst_str is big enough, no needs to consider it's size,
                    *                           only src_count will be considered.
                    * @param src_str[in]        the start address of the source string.
                    *                           no content will be copied if this param is null.
                    * @param src_pos[in]        the offset to the start address of the source string.
                    * @param src_count[in]      the number of characters to be copied into the destination buffer.
                    *                           pass MC_INVALID_SIZE means no copy length limit, copy until zero is encountered.
                    * @return element count that copied in to he destination buffer.
                    * @see m_copy
                    * @note  It's not a thread-safe function.
                    */
                    template <typename T>
                    static size_t copy(T * dst_str, size_t dst_pos, size_t dst_capacity, const T * src_str, size_t src_pos, size_t src_count);
                    
                    /***
                    * copy string from source string to destination string,
                    * @param dst_str[out]       the start address of the destination buffer.
                    *                           no content will be copied if this param is null.
                    * @param dst_capacity[in]   the total size of the destination buffer.
                    *                           pass MC_INVALID_SIZE means dst_str is big enough, no needs to consider it's size,
                    *                           only src_count will be considered.
                    * @param src_str[in]        the start address of the source string.
                    *                           no content will be copied if this param is null.
                    * @param src_count[in]      the number of characters to be copied into the destination buffer.
                    *                           pass MC_INVALID_SIZE means no copy length limit, copy until zero is encountered.
                    * @return element count that copied in to he destination buffer.
                    * @see m_copy
                    * @note  It's not a thread-safe function.
                    */
                    template <typename T>
                    static size_t copy(T * dst_str, size_t dst_capacity, const T * src_str, size_t src_count)
                    {
                        return copy<T>(dst_str, 0, dst_capacity, src_str, 0, src_count);
                    }

                    template <typename T>
                    static size_t copy(T * dst_str, const T * src_str)
                    {
                        return copy<T>(dst_str, 0, MC_INVALID_SIZE, src_str, 0, MC_INVALID_SIZE);
                    }

                    /***
                    * copy data from source string to destination string,
                    * @param dst_str[out]       the start address of the destination buffer.
                    *                           no content will be copied if this param is null.
                    * @param dst_pos[in]        the offset to the start address of the destination buffer.
                    *                           no content will be copied if this param is not less than dst_capacity.
                    * @param dst_capacity[in]   the total size of the destination buffer.
                    *                           pass MC_INVALID_SIZE means dst_str is big enough, no needs to consider it's size,
                    *                           only src_count will be considered.
                    * @param src_str[in]        the start address of the source string.
                    *                           no content will be copied if this param is null.
                    * @param src_pos[in]        the offset to the start address of the source string.
                    * @param src_count[in]      the number of characters to be copied into the destination buffer.
                    *                           pass MC_INVALID_SIZE means copy until destination buffer is full.
                    *                           no content will be copied  if MC_INVALID_SIZE is passed to both 2 parameters.
                    * @return element count that copied in to he destination buffer.
                    * @see copy
                    * @note  It's not a thread-safe function.
                    */
                    template <typename T>
                    static size_t m_copy(T * dst_str, size_t dst_pos, size_t dst_capacity, const T * src_str, size_t src_pos, size_t src_count);
                    
                    template <typename T>
                    static size_t s_move(T * dst_str, size_t dst_pos, size_t dst_capacity, const T * src_str, size_t src_pos, size_t src_count);
                    /***
                    * copy data from source string to destination string,
                    * @param dst_str[out]       the start address of the destination buffer.
                    *                           no content will be copied if this param is null.
                    * @param dst_capacity[in]   the total size of the destination buffer.
                    *                           pass MC_INVALID_SIZE means dst_str is big enough, no needs to consider it's size,
                    *                           only src_count will be considered.
                    * @param src_str[in]        the start address of the source string.
                    *                           no content will be copied if this param is null.
                    * @param src_count[in]      the number of characters to be copied into the destination buffer.
                    *                           pass MC_INVALID_SIZE means copy until destination buffer is full.
                    *                           no content will be copied  if MC_INVALID_SIZE is passed to both 2 parameters.
                    * @return element count that copied in to he destination buffer.
                    * @see copy
                    * @note  It's not a thread-safe function.
                    */
                    template <typename T>
                    static size_t m_copy(T * dst_str, size_t dst_capacity, const T * src_str, size_t src_count)
                    {
                        return m_copy<T>(dst_str, 0, dst_capacity, src_str, 0, src_count);
                    }

                    /***
                    * copy data from source string to destination string, source and destination can overlapped.
                    * @param dst_str[out]       the start address of the destination buffer.
                    *                           no content will be copied if this param is null.
                    * @param dst_pos[in]        the offset to the start address of the destination buffer.
                    *                           no content will be copied if this param is not less than dst_capacity.
                    * @param dst_capacity[in]   the total size of the destination buffer.
                    *                           pass MC_INVALID_SIZE means dst_str is big enough, no needs to consider it's size,
                    *                           only src_count will be considered.
                    * @param src_str[in]        the start address of the source string.
                    *                           no content will be copied if this param is null.
                    * @param src_pos[in]        the offset to the start address of the source string.
                    * @param src_count[in]      the number of characters to be copied into the destination buffer.
                    *                           no content will be copied is the parameter is MC_INVALID_SIZE.
                    *                           no content will be copied  if MC_INVALID_SIZE is passed to both 2 parameters.
                    * @return element count that copied in to he destination buffer.
                    * @see copy
                    * @note  It's not a thread-safe function.
                    */
                    template <typename T>
                    static size_t m_move(T * dst_str, size_t dst_pos, size_t dst_capacity, const T * src_str, size_t src_pos, size_t src_count);

                   
                    /***
                    * copy data from source string to destination string, source and destination can overlapped.
                    * @param dst_str[out]       the start address of the destination buffer.
                    *                           no content will be copied if this param is null.
                    * @param dst_capacity[in]   the total size of the destination buffer.
                    *                           pass MC_INVALID_SIZE means dst_str is big enough, no needs to consider it's size,
                    *                           only src_count will be considered.
                    * @param src_str[in]        the start address of the source string.
                    *                           no content will be copied if this param is null.
                    * @param src_count[in]      the number of characters to be copied into the destination buffer.
                    *                           no content will be copied is the parameter is MC_INVALID_SIZE.
                    *                           no content will be copied  if MC_INVALID_SIZE is passed to both 2 parameters.
                    * @return element count that copied in to he destination buffer.
                    * @see copy
                    * @note  It's not a thread-safe function.
                    */
                    template <typename T>
                    static size_t m_move(T* dst_str, size_t dst_capacity, const T * src_str, size_t src_count)
                    {
                        return m_move<T>(dst_str, 0, dst_capacity, src_str, 0, src_count);
                    }

                    template <typename T>
                    static size_t s_move(T* dst_str, size_t dst_capacity, const T * src_str, size_t src_count)
                    {
                        return s_move<T>(dst_str, 0, dst_capacity, src_str, 0, src_count);
                    }

                    /***
                    * fill memory with a constant byte, but all a '\0' after the last character
                    * @param str[out]           the start address of the buffer to set. must greater than count
                    * @param ch[in]             the character to write.
                    * @param count[in]          the number of characters to write.
                    * @return void
                    * @see
                    * @note  It's not a thread-safe function.
                    */
                    static void set(char* str, char ch, size_t count);
                    static void set(wchar_t* str, wchar_t ch, size_t count);

                    /***
                    * fill memory with a constant byte
                    * @param str[out]           the start address of the buffer to set.
                    * @param ch[in]             the character to write.
                    * @param count[in]          the number of characters to write.
                    * @return void
                    * @see
                    * @note  It's not a thread-safe function.
                    */
                    static void m_set(char* str, char ch, size_t count);
                    static void m_set(wchar_t* str, wchar_t ch, size_t count);

                    /***
                    * convert integer to string buffer
                    * @param dst_buffer[out]    the start address of the buffer to save result string.
                    * @param dst_len[in]        the length of the buffer by characters.
                    * @param i/u[in]            the integer to write.
                    * @param width[in]         the width of the format of the string.
                    * @param zero_fill[in]     if fill with 0 at empty place of the string.
                    * @return size_t            the characters written.
                    * @see
                    * @note  It's not a thread-safe function.
                    */
                    static size_t number_to_string(char* dst_buffer,    size_t dst_len, pilo::i32_t i, pilo::i32_t width = 0, bool zero_fill = 0);
                    static size_t number_to_string(wchar_t* dst_buffer, size_t dst_len, pilo::i32_t i, pilo::i32_t width = 0, bool zero_fill = 0);
                    static size_t number_to_string(char* dst_buffer,    size_t dst_len, pilo::i64_t i, pilo::i32_t width = 0, bool zero_fill = 0);
                    static size_t number_to_string(wchar_t* dst_buffer, size_t dst_len, pilo::i64_t i, pilo::i32_t width = 0, bool zero_fill = 0);
                    static size_t number_to_string(char* dst_buffer,    size_t dst_len, pilo::u32_t u, pilo::i32_t width = 0, bool zero_fill = 0);
                    static size_t number_to_string(wchar_t* dst_buffer, size_t dst_len, pilo::u32_t u, pilo::i32_t width = 0, bool zero_fill = 0);
                    static size_t number_to_string(char* dst_buffer,    size_t dst_len, pilo::u64_t u, pilo::i32_t width = 0, bool zero_fill = 0);
                    static size_t number_to_string(wchar_t* dst_buffer, size_t dst_len, pilo::u64_t u, pilo::i32_t width = 0, bool zero_fill = 0);

                    /***
                    * convert integer to string buffer
                    * @param dst_buffer[out]    the start address of the buffer to save result string.
                    * @param dst_len[in]        the length of the buffer by characters.
                    * @param i/u[in]            the integer to write.
                    * @param fmt[in]            the format of the float. such as  '4.3'
                    * @param zero_fill[in]      if fill with 0 at empty place of the string.
                    * @return size_t            the characters written.
                    * @see
                    * @note  It's not a thread-safe function.
                    */
                    static size_t number_to_string(char* dst_buffer,    size_t dst_len, float f,  const char* fmt = nullptr, bool zero_fill = 0);
                    static size_t number_to_string(wchar_t* dst_buffer, size_t dst_len, float f,  const wchar_t* fmt = nullptr, bool zero_fill = 0);
                    static size_t number_to_string(char* dst_buffer,    size_t dst_len, double f, const char* fmt = nullptr, bool zero_fill = 0);
                    static size_t number_to_string(wchar_t* dst_buffer, size_t dst_len, double f, const wchar_t* fmt = nullptr, bool zero_fill = 0);

                    /***
                    * compare too string, to test if they are equal.
                    * @param str0[in]           left string to compare.
                    * @param begin_pos_0[in]    the start position of the left string to compare.
                    * @param str1[in]           right string to compare.
                    * @param begin_pos_1[in]    the start position of the right string to compare.
                    * @param length[in]         the characters to compare, compare until null if MC_INVALID_SIZE is given.
                    * @return pilo::i32_t       return 0 if they are equal, 
                    *                           <0 if left < right
                    *                           >0 if left > right
                    * @see
                    * @note  It's not a thread-safe function.
                    */
                    static pilo::i32_t binary_compare(const char* str0, size_t begin_pos_0, const char* str1, size_t begin_pos_1, size_t length);
                    static pilo::i32_t binary_compare(const wchar_t* str0, size_t begin_pos_0, const wchar_t* str1, size_t begin_pos_1, size_t length);

                    /***
                    * compare too string, to test if they are equal. simple version
                    * @param str0[in]           left string to compare.
                    * @param str1[in]           right string to compare.
                    * @param length[in]         the characters to compare, compare until null if MC_INVALID_SIZE is given.
                    *                           default is MC_INVALID_SIZE.
                    * @return pilo::i32_t       return 0 if they are equal,
                    *                           <0 if left < right
                    *                           >0 if left > right
                    * @see
                    * @note  It's not a thread-safe function.
                    */
                    template<typename charT>
                    static pilo::i32_t binary_compare(const charT* str0, const charT* str1, size_t length = MC_INVALID_SIZE)
                    {
                        return binary_compare(str0, 0, str1, 0, length);
                    }

                    template<typename charT>
                    static charT* alloc_copy(const charT* str, size_t length = MC_INVALID_SIZE)
                    {
                        if (str == nullptr)
                        {
                            return nullptr;
                        }
                        
                        if (length == MC_INVALID_SIZE)
                        {
                            length = ::pilo::core::string::string_util::length(str);
                            if (MC_INVALID_SIZE == length)
                            {
                                return nullptr;
                            }
                        }
    
                        charT* dst = (charT*) ::malloc((length+1) * sizeof(charT));
                        if (dst == nullptr)
                        {
                            return nullptr;
                        }
                        
                        ::pilo::core::string::string_util::copy(dst, length+1, str, length);
                        
                        return dst;
                    }

                    template <typename T>
                    static T* concatenate_string(T* dstStringBuffer, size_t destBufferCount, const T* srcString, size_t sourceStringCount)
                    {
                        if (dstStringBuffer == 0) 
                        {
                            return 0;
                        }

                        size_t cDst = ::pilo::core::string::string_util::length(dstStringBuffer);

                        if (srcString == 0) 
                        {
                            return dstStringBuffer;
                        }

                        if (sourceStringCount == MC_INVALID_SIZE) 
                        {
                            sourceStringCount = ::pilo::core::string::string_util::length(srcString);
                        }

                        if (sourceStringCount == 0) 
                        {
                            return dstStringBuffer;
                        }

                        if (destBufferCount != MC_INVALID_SIZE) 
                        {
                            if (destBufferCount <= sourceStringCount + cDst) 
                            {
                                return 0;
                            }
                        }

                        ::pilo::core::string::string_util::copy(dstStringBuffer + cDst, destBufferCount - cDst, srcString, sourceStringCount);

                        return dstStringBuffer;
                    }

                    static pilo::i32_t cstr_to_int32(const char* pc);

                    static pilo::i64_t cstr_to_int64(const char* pc);

                    static const char* binary_to_cstr(char* dst_buffer, size_t dst_len, const void* src, size_t src_len, size_t num_perline);

                    static size_t split(char* s, const char delimiter, char* ss[], size_t maxs);
                    static size_t split(char* s, const char* delimiter, char* ss[], size_t maxs);
                    
                    static bool ipv4_address_cstr_to_int(pilo::u32_t& ip, pilo::u16_t& port, const char* cstr, size_t length = MC_INVALID_SIZE);
                    
                    template <typename T>
                    static ::pilo::error_number_t rescanable_replace(T* strSource, size_t cTotalSizeOfSource, const T* strNeedle, const T* strReplacement, size_t* refCount)
                    {
                        if (strSource == nullptr) 
                        {
                            return ::pilo::EC_NULL_PARAM;
                        }
                        if (refCount != nullptr) 
                        { //set result count properly at first;
                            *refCount = 0;
                        }

                        if (::pilo::core::string::string_util::find(strReplacement, strNeedle)) 
                        { // replace abc with abcde is illegal
                            return ::pilo::EC_INFINITE_LOOP;
                        }

                        if (cTotalSizeOfSource == MC_INVALID_SIZE) 
                        {
                            cTotalSizeOfSource = ::pilo::core::string::string_util::length(strSource)+1;
                        }

                        size_t cCharsToReplace = ::pilo::core::string::string_util::length(strSource);
                        if (cCharsToReplace == MC_INVALID_SIZE) 
                        {
                            return ::pilo::EC_INVALID_PARAM;
                        }

                        size_t cNeedleLength = ::pilo::core::string::string_util::length(strNeedle);
                        if (MC_INVALID_SIZE == cNeedleLength) 
                        { // noop either
                             return ::pilo::EC_NULL_PARAM;
                        }

                        size_t cReplacementLength = ::pilo::core::string::string_util::length(strReplacement);
                        if (cReplacementLength != MC_INVALID_SIZE) 
                        {
                            if ((cReplacementLength > cNeedleLength) && (cCharsToReplace >= cTotalSizeOfSource)) 
                            {
                                return ::pilo::EC_BUFFER_TOO_SMALL; //insufficient buffer size;
                            }
                        }
                        else {
                            cReplacementLength = 0; //null replacement string, means removal
                        }


                        ::pilo::tristate_t tsNeedEnlarged;
                        size_t diff;
                        if (cReplacementLength > cNeedleLength) 
                        {
                            tsNeedEnlarged = ::pilo::eTS_True;
                            diff = cReplacementLength - cNeedleLength;

                        }
                        else if (cReplacementLength < cNeedleLength) 
                        {
                            tsNeedEnlarged = ::pilo::eTS_False;
                            diff = cNeedleLength - cReplacementLength;
                        }
                        else 
                        {
                            tsNeedEnlarged = ::pilo::eTS_NA; //means same size
                            diff = 0;
                        }

                        size_t diffCount = 0;
                        T* strFound = 0;

                        while ((strFound = ::pilo::core::string::string_util::find(strSource, strNeedle, cCharsToReplace)) != nullptr) 
                        {
                            diffCount++; //increase the found counter 
                            if (::pilo::eTS_True == tsNeedEnlarged) 
                            { //check if the buffer is sufficient 
                                if (cCharsToReplace + diff >= cTotalSizeOfSource) 
                                {
                                    if (refCount != 0) {
                                        *refCount = diffCount - 1;
                                    }
                                    return ::pilo::EC_BUFFER_TOO_SMALL;
                                }
                                size_t tmpRemainLength = cCharsToReplace - (strFound - strSource) - cNeedleLength;
                                ::pilo::core::string::string_util::s_move(strFound + cReplacementLength,MC_INVALID_SIZE, strFound + cNeedleLength, tmpRemainLength);
                                cCharsToReplace += diff;
                            }
                            else if (::pilo::eTS_False == tsNeedEnlarged)
                            {
                                size_t tmpRemainLength = cCharsToReplace - (strFound - strSource) - cNeedleLength;
                                ::pilo::core::string::string_util::s_move(strFound + cReplacementLength, MC_INVALID_SIZE, strFound + cNeedleLength, tmpRemainLength);
                                cCharsToReplace -= diff;
                            }

                            strSource[cCharsToReplace] = '\0';

                            // do replacement or removal 
                            if (0 != cReplacementLength) 
                            { //replace, remove otherwise
                                ::pilo::core::string::string_util::m_move(strFound, MC_INVALID_SIZE, (T*)strReplacement, cReplacementLength);
                            }
                        }

                        if (refCount != 0) 
                        {
                            *refCount = diffCount;
                        }

                        return ::pilo::EC_OK;
                    }

                    template<typename charT>
                    static charT* trim(charT* s)
                    {
                        charT* pb = s;
                        charT* pe = s + string_util::length(s) - 1;
                        while ((*pb == (charT) ' ' || *pb == (charT) '\t' || *pb == (charT) '\n' || *pb == (charT) '\r') && *pb != (charT) '\0') pb++;
                        while (pe != pb && (*pe == (charT) ' ' || *pe == (charT) '\t' || *pe == (charT) '\n' || *pe == (charT) '\r')) pe--;
                        *(pe + 1) = (charT)  '\0';
                        return pb;
                    }

                    template<typename T>
                    static T* find(T* strSource, const T* needle, size_t count = MC_INVALID_SIZE)
                    {
                        if (strSource == nullptr)
                        {
                            return nullptr;
                        }

                        //return source string is needle is zero length (or null)
                        if (needle == nullptr)
                        {
                            return strSource;
                        }
                        size_t needleLength = ::pilo::core::string::string_util::length(needle);
                        if (needleLength <= 0)
                        {
                            return strSource;
                        }

                        size_t strSourceLength;
                        if (count == MC_INVALID_SIZE)
                        {
                            strSourceLength = ::pilo::core::string::string_util::length(strSource);
                            if (strSourceLength == 0)
                            {
                                return nullptr;
                            }
                        }
                        else
                        {
                            strSourceLength = count;
                        }

                        if (strSourceLength == 0)
                        {
                            return strSource;
                        }

                        for (int i = 0; i < (int)strSourceLength; i++)  {
                            size_t charsRemains = strSourceLength - i;
                            if (needleLength > charsRemains)
                            {
                                return nullptr;
                            }
                            if (0 == ::pilo::core::string::string_util::binary_compare(strSource + i, needle, needleLength)) {
                                return (strSource + i);
                            }
                        }

                        return nullptr;
                    }


                    template<typename T>
                    static const T* find(const T* strSource, const T* needle, size_t count = MC_INVALID_SIZE)
                    {
                        if (strSource == nullptr)
                        {
                            return nullptr;
                        }

                        //return source string is needle is zero length (or null)
                        if (needle == nullptr)
                        {
                            return strSource;
                        }
                        size_t needleLength = ::pilo::core::string::string_util::length(needle);
                        if (needleLength <= 0)
                        {
                            return strSource;
                        }

                        size_t strSourceLength;
                        if (count == MC_INVALID_SIZE)
                        {
                            strSourceLength = ::pilo::core::string::string_util::length(strSource);
                            if (strSourceLength == 0)
                            {
                                return nullptr;
                            }
                        }
                        else
                        {
                            strSourceLength = count;
                        }

                        if (strSourceLength == 0)
                        {
                            return strSource;
                        }

                        for (int i = 0; i < (int)strSourceLength; i++)  {
                            size_t charsRemains = strSourceLength - i;
                            if (needleLength > charsRemains) 
                            {
                                return nullptr;
                            }
                            if (0 == ::pilo::core::string::string_util::binary_compare(strSource + i, needle, needleLength)) {
                                return (strSource + i);
                            }
                        }

                        return nullptr;
                    }

                    template<typename T>
                    static const T* find_reversely(const T* strSource, const T* needle, size_t count = MC_INVALID_SIZE)
                    {
                        if (strSource == nullptr)
                        {
                            return nullptr;
                        }

                        //return source string is needle is zero length (or null)
                        if (needle == nullptr)
                        {
                            return strSource;
                        }
                        size_t needleLength = ::pilo::core::string::string_util::length(needle);
                        if (needleLength <= 0) 
                        {
                            return strSource;
                        }

                        size_t strSourceLength;
                        if (count == MC_INVALID_SIZE) 
                        {
                            strSourceLength = ::pilo::core::string::string_util::length(strSource);
                            if (strSourceLength == 0) 
                            {
                                return nullptr;
                            }
                        }
                        else 
                        {
                            strSourceLength = count;
                        }

                        if (strSourceLength == 0) 
                        {
                            return strSource;
                        }

                        for (int i = (int)strSourceLength - 1; i >= 0; i--)  {
                            size_t charsRemains = strSourceLength - i;
                            if (needleLength > charsRemains) {
                                continue;
                            }
                            if (0 == ::pilo::core::string::string_util::binary_compare(strSource + i, needle, needleLength)) {
                                return (strSource + i);
                            }
                        }

                        return nullptr;
                    }

                    template<typename T>
                    static T* find_reversely(T* strSource, const T* needle, size_t count = MC_INVALID_SIZE)
                    {
                        if (strSource == nullptr)
                        {
                            return nullptr;
                        }

                        //return source string is needle is zero length (or null)
                        if (needle == nullptr)
                        {
                            return strSource;
                        }
                        size_t needleLength = ::pilo::core::string::string_util::length(needle);
                        if (needleLength <= 0)
                        {
                            return strSource;
                        }

                        size_t strSourceLength;
                        if (count == MC_INVALID_SIZE)
                        {
                            strSourceLength = ::pilo::core::string::string_util::length(strSource);
                            if (strSourceLength == 0)
                            {
                                return nullptr;
                            }
                        }
                        else
                        {
                            strSourceLength = count;
                        }

                        if (strSourceLength == 0)
                        {
                            return strSource;
                        }

                        for (int i = (int)strSourceLength - 1; i >= 0; i--)  {
                            size_t charsRemains = strSourceLength - i;
                            if (needleLength > charsRemains) {
                                continue;
                            }
                            if (0 == ::pilo::core::string::string_util::binary_compare(strSource + i, needle, needleLength)) {
                                return (strSource + i);
                            }
                        }

                        return nullptr;
                    }


			private:
				static size_t _element_count_mbcs(const unsigned char* str);
				static size_t _element_count_utf8(const char* str);

            };


            //implementation

            template <typename T>
            size_t pilo::core::string::string_util::copy(T * dst_str, size_t dst_pos, size_t dst_capacity, const T * src_str, size_t src_pos, size_t src_count)
            {
                if (nullptr == dst_str || nullptr == src_str)
                {
                    return MC_INVALID_SIZE;
                }                

                if (dst_pos >= (dst_capacity - 1))
                {
                    return MC_INVALID_SIZE;
                }

                size_t dest_left = dst_capacity - dst_pos - 1;

                size_t numCopy = 0;
                size_t maxCopyCount;
                if ((dst_capacity == MC_INVALID_SIZE) && (src_count == MC_INVALID_SIZE))
                {
                    maxCopyCount = MC_INVALID_SIZE;
                }
                else if (dst_capacity == MC_INVALID_SIZE)
                {
                    maxCopyCount = src_count;
                }
                else if (src_count == MC_INVALID_SIZE)
                {
                    maxCopyCount = dest_left;
                }
                else
                {
                    maxCopyCount = (dest_left < src_count) ? dest_left : src_count;
                }

                if (dst_pos + maxCopyCount >= dst_capacity)
                {
                    maxCopyCount = dest_left;
                }

//                 if (maxCopyCount == 0)
//                 {
//                     return 0;
//                 }                

                T* dst_write_ptr = dst_str + dst_pos;
                const T* src_read_ptr = src_str + src_pos;

                while ((numCopy < maxCopyCount) && (0 != (*src_read_ptr)))
                {
                    *dst_write_ptr = *src_read_ptr;
                    numCopy++;
                    dst_write_ptr++;
                    src_read_ptr++;
                }
                *dst_write_ptr = 0;

                return numCopy;
            }
            
            template <typename T>
            size_t pilo::core::string::string_util::m_copy(T * dst_str, size_t dst_pos, size_t dst_capacity, const T * src_str, size_t src_pos, size_t src_count)
            {
                if (nullptr == dst_str || nullptr == src_str)
                {
                    return MC_INVALID_SIZE;
                }

                if (dst_pos >= dst_capacity)
                {
                    return MC_INVALID_SIZE;
                }
                size_t dest_left = dst_capacity - dst_pos;

                size_t maxCopyCount;
                if ((dst_capacity == MC_INVALID_SIZE) && (src_count == MC_INVALID_SIZE))
                {
                    maxCopyCount = 0;
                }
                else if (dst_capacity == MC_INVALID_SIZE)
                {
                    maxCopyCount = src_count;
                }
                else if (src_count == MC_INVALID_SIZE)
                {
                    maxCopyCount = dest_left;
                }
                else
                {
                    maxCopyCount = (dest_left< src_count) ? dest_left : src_count;
                }

                if (maxCopyCount == 0)
                {
                    return 0;
                }

                if (dst_pos + maxCopyCount >= dst_capacity)
                {
                    maxCopyCount = dst_capacity - dst_pos;
                }

                T* dst_write_ptr = dst_str + dst_pos;
                const T* src_read_ptr = src_str + src_pos;

                ::memcpy(dst_write_ptr, src_read_ptr, maxCopyCount * sizeof(T));

                return maxCopyCount;
            }

            template <typename T>
            size_t pilo::core::string::string_util::m_move(T * dst_str, size_t dst_pos, size_t dst_capacity, const T * src_str, size_t src_pos, size_t src_count)
            {
                if (nullptr == dst_str || nullptr == src_str)
                {
                    return MC_INVALID_SIZE;
                }

                if (dst_pos >= dst_capacity)
                {
                    return MC_INVALID_SIZE;
                }
                size_t dest_left = dst_capacity - dst_pos;

                size_t maxCopyCount;
                if ((dst_capacity == MC_INVALID_SIZE) && (src_count == MC_INVALID_SIZE))
                {
                    maxCopyCount = 0;
                }
                else if (dst_capacity == MC_INVALID_SIZE)
                {
                    maxCopyCount = src_count;
                }
                else if (src_count == MC_INVALID_SIZE)
                {
                    maxCopyCount = 0;
                }
                else
                {
                    maxCopyCount = (dest_left < src_count) ? dest_left : src_count;
                }

                if (maxCopyCount == 0)
                {
                    return 0;
                }

                if (dst_pos + maxCopyCount >= dst_capacity)
                {
                    maxCopyCount = dst_capacity - dst_pos;
                }

                T* dst_write_ptr = dst_str + dst_pos;
                const T* src_read_ptr = src_str + src_pos;

                ::memmove(dst_write_ptr, src_read_ptr, maxCopyCount * sizeof(T));

                return maxCopyCount;
            }

            template <typename T>
            size_t pilo::core::string::string_util::s_move(T * dst_str, size_t dst_pos, size_t dst_capacity, const T * src_str, size_t src_pos, size_t src_count)
            {
                if (nullptr == dst_str || nullptr == src_str)
                {
                    return MC_INVALID_SIZE;
                }

                if (dst_pos >= (dst_capacity - 1) )
                {
                    return MC_INVALID_SIZE;
                }
                size_t dest_left = dst_capacity - dst_pos - 1;

                size_t maxCopyCount;
                if ((dst_capacity == MC_INVALID_SIZE) && (src_count == MC_INVALID_SIZE))
                {
                    maxCopyCount = 0;
                }
                else if (dst_capacity == MC_INVALID_SIZE)
                {
                    maxCopyCount = src_count;
                }
                else if (src_count == MC_INVALID_SIZE)
                {
                    maxCopyCount = 0;
                }
                else
                {
                    maxCopyCount = (dest_left < src_count) ? dest_left : src_count;
                }

                if (maxCopyCount == 0)
                {
                    return 0;
                }

                if (dst_pos + maxCopyCount >= dst_capacity)
                {
                    maxCopyCount = dst_capacity - dst_pos;
                }

                T* dst_write_ptr = dst_str + dst_pos;
                const T* src_read_ptr = src_str + src_pos;

                ::memmove(dst_write_ptr, src_read_ptr, maxCopyCount * sizeof(T));

                dst_write_ptr[ maxCopyCount * sizeof(T) ] = 0;

                return maxCopyCount;
            }            
        }
    }
}