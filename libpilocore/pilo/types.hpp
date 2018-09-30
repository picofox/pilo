#pragma once
#include "sysdef.hpp"

#include <stdint.h>
#if defined(WIN32) || defined(WINDOWS)
#include <WinSock2.h>
#endif


#define B00000000 (0x00)
#define B00000001 (0x01)
#define B00000010 (0x02)
#define B00000011 (0x03)
#define B00000100 (0x04)
#define B00000101 (0x05)
#define B00000110 (0x06)
#define B00000111 (0x07)
#define B00001000 (0x08)
#define B00001001 (0x09)
#define B00001010 (0x0a)
#define B00001011 (0x0b)
#define B00001100 (0x0c)
#define B00001101 (0x0d)
#define B00001110 (0x0e)
#define B00001111 (0x0f)
#define B00010000 (0x10)
#define B00010001 (0x11)
#define B00010010 (0x12)
#define B00010011 (0x13)
#define B00010100 (0x14)
#define B00010101 (0x15)
#define B00010110 (0x16)
#define B00010111 (0x17)
#define B00011000 (0x18)
#define B00011001 (0x19)
#define B00011010 (0x1a)
#define B00011011 (0x1b)
#define B00011100 (0x1c)
#define B00011101 (0x1d)
#define B00011110 (0x1e)
#define B00011111 (0x1f)
#define B00100000 (0x20)
#define B00100001 (0x21)
#define B00100010 (0x22)
#define B00100011 (0x23)
#define B00100100 (0x24)
#define B00100101 (0x25)
#define B00100110 (0x26)
#define B00100111 (0x27)
#define B00101000 (0x28)
#define B00101001 (0x29)
#define B00101010 (0x2a)
#define B00101011 (0x2b)
#define B00101100 (0x2c)
#define B00101101 (0x2d)
#define B00101110 (0x2e)
#define B00101111 (0x2f)
#define B00110000 (0x30)
#define B00110001 (0x31)
#define B00110010 (0x32)
#define B00110011 (0x33)
#define B00110100 (0x34)
#define B00110101 (0x35)
#define B00110110 (0x36)
#define B00110111 (0x37)
#define B00111000 (0x38)
#define B00111001 (0x39)
#define B00111010 (0x3a)
#define B00111011 (0x3b)
#define B00111100 (0x3c)
#define B00111101 (0x3d)
#define B00111110 (0x3e)
#define B00111111 (0x3f)
#define B01000000 (0x40)
#define B01000001 (0x41)
#define B01000010 (0x42)
#define B01000011 (0x43)
#define B01000100 (0x44)
#define B01000101 (0x45)
#define B01000110 (0x46)
#define B01000111 (0x47)
#define B01001000 (0x48)
#define B01001001 (0x49)
#define B01001010 (0x4a)
#define B01001011 (0x4b)
#define B01001100 (0x4c)
#define B01001101 (0x4d)
#define B01001110 (0x4e)
#define B01001111 (0x4f)
#define B01010000 (0x50)
#define B01010001 (0x51)
#define B01010010 (0x52)
#define B01010011 (0x53)
#define B01010100 (0x54)
#define B01010101 (0x55)
#define B01010110 (0x56)
#define B01010111 (0x57)
#define B01011000 (0x58)
#define B01011001 (0x59)
#define B01011010 (0x5a)
#define B01011011 (0x5b)
#define B01011100 (0x5c)
#define B01011101 (0x5d)
#define B01011110 (0x5e)
#define B01011111 (0x5f)
#define B01100000 (0x60)
#define B01100001 (0x61)
#define B01100010 (0x62)
#define B01100011 (0x63)
#define B01100100 (0x64)
#define B01100101 (0x65)
#define B01100110 (0x66)
#define B01100111 (0x67)
#define B01101000 (0x68)
#define B01101001 (0x69)
#define B01101010 (0x6a)
#define B01101011 (0x6b)
#define B01101100 (0x6c)
#define B01101101 (0x6d)
#define B01101110 (0x6e)
#define B01101111 (0x6f)
#define B01110000 (0x70)
#define B01110001 (0x71)
#define B01110010 (0x72)
#define B01110011 (0x73)
#define B01110100 (0x74)
#define B01110101 (0x75)
#define B01110110 (0x76)
#define B01110111 (0x77)
#define B01111000 (0x78)
#define B01111001 (0x79)
#define B01111010 (0x7a)
#define B01111011 (0x7b)
#define B01111100 (0x7c)
#define B01111101 (0x7d)
#define B01111110 (0x7e)
#define B01111111 (0x7f)
#define B10000000 (0x80)
#define B10000001 (0x81)
#define B10000010 (0x82)
#define B10000011 (0x83)
#define B10000100 (0x84)
#define B10000101 (0x85)
#define B10000110 (0x86)
#define B10000111 (0x87)
#define B10001000 (0x88)
#define B10001001 (0x89)
#define B10001010 (0x8a)
#define B10001011 (0x8b)
#define B10001100 (0x8c)
#define B10001101 (0x8d)
#define B10001110 (0x8e)
#define B10001111 (0x8f)
#define B10010000 (0x90)
#define B10010001 (0x91)
#define B10010010 (0x92)
#define B10010011 (0x93)
#define B10010100 (0x94)
#define B10010101 (0x95)
#define B10010110 (0x96)
#define B10010111 (0x97)
#define B10011000 (0x98)
#define B10011001 (0x99)
#define B10011010 (0x9a)
#define B10011011 (0x9b)
#define B10011100 (0x9c)
#define B10011101 (0x9d)
#define B10011110 (0x9e)
#define B10011111 (0x9f)
#define B10100000 (0xa0)
#define B10100001 (0xa1)
#define B10100010 (0xa2)
#define B10100011 (0xa3)
#define B10100100 (0xa4)
#define B10100101 (0xa5)
#define B10100110 (0xa6)
#define B10100111 (0xa7)
#define B10101000 (0xa8)
#define B10101001 (0xa9)
#define B10101010 (0xaa)
#define B10101011 (0xab)
#define B10101100 (0xac)
#define B10101101 (0xad)
#define B10101110 (0xae)
#define B10101111 (0xaf)
#define B10110000 (0xb0)
#define B10110001 (0xb1)
#define B10110010 (0xb2)
#define B10110011 (0xb3)
#define B10110100 (0xb4)
#define B10110101 (0xb5)
#define B10110110 (0xb6)
#define B10110111 (0xb7)
#define B10111000 (0xb8)
#define B10111001 (0xb9)
#define B10111010 (0xba)
#define B10111011 (0xbb)
#define B10111100 (0xbc)
#define B10111101 (0xbd)
#define B10111110 (0xbe)
#define B10111111 (0xbf)
#define B11000000 (0xc0)
#define B11000001 (0xc1)
#define B11000010 (0xc2)
#define B11000011 (0xc3)
#define B11000100 (0xc4)
#define B11000101 (0xc5)
#define B11000110 (0xc6)
#define B11000111 (0xc7)
#define B11001000 (0xc8)
#define B11001001 (0xc9)
#define B11001010 (0xca)
#define B11001011 (0xcb)
#define B11001100 (0xcc)
#define B11001101 (0xcd)
#define B11001110 (0xce)
#define B11001111 (0xcf)
#define B11010000 (0xd0)
#define B11010001 (0xd1)
#define B11010010 (0xd2)
#define B11010011 (0xd3)
#define B11010100 (0xd4)
#define B11010101 (0xd5)
#define B11010110 (0xd6)
#define B11010111 (0xd7)
#define B11011000 (0xd8)
#define B11011001 (0xd9)
#define B11011010 (0xda)
#define B11011011 (0xdb)
#define B11011100 (0xdc)
#define B11011101 (0xdd)
#define B11011110 (0xde)
#define B11011111 (0xdf)
#define B11100000 (0xe0)
#define B11100001 (0xe1)
#define B11100010 (0xe2)
#define B11100011 (0xe3)
#define B11100100 (0xe4)
#define B11100101 (0xe5)
#define B11100110 (0xe6)
#define B11100111 (0xe7)
#define B11101000 (0xe8)
#define B11101001 (0xe9)
#define B11101010 (0xea)
#define B11101011 (0xeb)
#define B11101100 (0xec)
#define B11101101 (0xed)
#define B11101110 (0xee)
#define B11101111 (0xef)
#define B11110000 (0xf0)
#define B11110001 (0xf1)
#define B11110010 (0xf2)
#define B11110011 (0xf3)
#define B11110100 (0xf4)
#define B11110101 (0xf5)
#define B11110110 (0xf6)
#define B11110111 (0xf7)
#define B11111000 (0xf8)
#define B11111001 (0xf9)
#define B11111010 (0xfa)
#define B11111011 (0xfb)
#define B11111100 (0xfc)
#define B11111101 (0xfd)
#define B11111110 (0xfe)
#define B11111111 (0xff)

/* 7.18.2.1  Limits of exact-width integer types */

#ifndef INT8_MIN
#   define INT8_MIN (-128) 
#endif

#ifndef INT16_MIN
#   define INT16_MIN (-32768)
#endif

#ifndef INT32_MIN
#define INT32_MIN (-2147483647 - 1)
#endif

#ifndef INT64_MIN
#   ifdef __GNUC__
#        define INT64_MIN  (-9223372036854775807LL - 1)
#   else
#       define INT64_MIN  (-9223372036854775807i64 - 1)
#   endif
#endif

#ifndef INT8_MAX
#   define INT8_MAX 127
#endif

#ifndef INT16_MAX
#   define INT16_MAX 32767
#endif

#ifndef INT32_MAX
#   define INT32_MAX 2147483647
#endif

#ifndef INT64_MAX
#   ifdef __GNUC__
#       define INT64_MAX 9223372036854775807LL
#   else
#       define INT64_MAX 9223372036854775807i64
#   endif
#endif

#ifndef UINT8_MAX
#   define UINT8_MAX 0xff /* 255U */
#endif

#ifndef UINT16_MAX
#   define UINT16_MAX 0xffff /* 65535U */
#endif

#ifndef UINT32_MAX
#   define UINT32_MAX 0xffffffff  /* 4294967295U */
#endif

#ifndef UINT64_MAX
#   ifdef __GNUC__
#       define UINT64_MAX 0xffffffffffffffffULL /* 18446744073709551615ULL */
#   else
#       define UINT64_MAX 0xffffffffffffffffui64 /* 18446744073709551615ULL */
#   endif
#endif

#if defined(WIN32) || defined(WINDOWS)

#else
#   if defined(_UNICODE) || defined(UNICODE)
#       define __TEXT(quote) L##quote     
#   else
#       define __TEXT(quote) quote
#	endif
#   define TEXT(quote) __TEXT(quote)

#endif

namespace pilo
{ 
	template <size_t n> struct __regtype__{ typedef int int_r; typedef unsigned int uint_r; };
	template <> struct __regtype__<8>     { typedef long long int_r; typedef unsigned long long uint_r; };
	typedef __regtype__<sizeof(void*)>::int_r  int_r;
	typedef __regtype__<sizeof(void*)>::uint_r uint_r;


	typedef char							i8_t;
	typedef short							i16_t;
	typedef int								i32_t;
	typedef long long						i64_t;
	typedef unsigned char					u8_t;
	typedef unsigned short					u16_t;
	typedef unsigned int					u32_t;
	typedef unsigned long long				u64_t;

	typedef uint_r							address_t;
    typedef i32_t                           error_number_t;    
    typedef u32_t                           tick32_t;
    typedef u64_t                           tick64_t;
    typedef long long                       os_tick_t;
    

    template<typename T>
    class vec2_t
    {
    public:
        vec2_t() {}
        vec2_t(T x0, T y0) : x(x0), y(y0) {}
        T x;
        T y;
    };

    template<typename T>
    class size2_t
    {
    public:
        size2_t() {}
        size2_t(T w, T h) : width(w), height(h) {}
        T width;
        T height;
    };

    template<typename T>
    class rect_t
    {
    public:
        rect_t() {}
        rect_t(const vec2_t<T>& p, const size2_t<T>& sz) : pos(p), size(sz) {}
        vec2_t<T>	pos;
        size2_t<T>	size;
    };

    template<typename T>
    class vec3_t
    {
    public:
		vec3_t() {}
		vec3_t(T x0, T y0, T z0) : x(x0), y(y0), z(z0) {}
        T x;
        T y;
        T z;
    };

#if defined(WIN32) || defined(WINDOWS)
    typedef unsigned                thread_routine_return_t;
    typedef SOCKET                  os_socket_fd_t;
    typedef DWORD                   os_error_number_t;
    typedef HANDLE                  os_file_descriptor_t;
    typedef DWORD                   os_process_id_t;
    typedef HMODULE                 os_dll_module_handle_t;
    typedef DWORD                   os_thread_id_t;
    typedef CONDITION_VARIABLE      os_event_handle_t;
    typedef CRITICAL_SECTION        os_cs_handle_t;    
    

#else
    typedef int                     os_socket_fd_t;
    typedef void*                   thread_routine_return_t;
    typedef int                     os_error_number_t;
    typedef int                     os_file_descriptor_t;
    typedef int                     os_process_id_t;
    typedef void*                   os_dll_module_handle_t;
    typedef pthread_t               os_thread_id_t;
    typedef pthread_cond_t          os_event_handle_t;
    typedef pthread_mutex_t         os_cs_handle_t;

#	if defined(_UNICODE) || defined(UNICODE)

#	else

#	endif


#endif


	typedef enum
	{
		eTS_NA = -1,
		eTS_False = 0,
		eTS_True = 1
	} tristate_t;

    typedef enum {
        eLT   = -1,
        eEQ   =   0,
        eGT   =  1,
        eLN   =  0xF0,
        eRN   =  0x0F,
        eNull =  0xFF,

    } compare_result_t;

    template <typename T> struct _invalid_object_
    {
        static T& get() { static T v; return v; }
    };

    typedef vec2_t<pilo::u8_t>  u8_vec2_t;
    typedef vec2_t<pilo::u16_t> u16_vec2_t;
    typedef vec2_t<pilo::u32_t> u32_vec2_t;
    typedef vec2_t<pilo::u64_t> u64_vec2_t;
    typedef vec2_t<pilo::i8_t>  i8_vec2_t;
    typedef vec2_t<pilo::i16_t> i16_vec2_t;
    typedef vec2_t<pilo::i32_t> i32_vec2_t;
    typedef vec2_t<pilo::i64_t> i64_vec2_t;

	typedef vec3_t<pilo::i16_t> i16_vec3_t;
	typedef vec3_t<float> f_vec3_t;
    typedef rect_t<pilo::u8_t>	u8_rect_t;
    typedef vec2_t<float> f_vec2_t;



    /**
    * get default value of type T
    * the default value for number type is 0
    * the default value for pointer type is NULL
    * the default value for class object will use the default constructor
    * usage:
    *      int x = _type_default_value_<int>::get();
    *
    * use this technique to initialize a non-existing node within KMapByVector
    *
    */
    template <typename T> struct _type_default_value_
    {
        static const bool need_set_default_value = true;
        static const T& get() { static T v; return v; }
    };
    template <typename T> struct _type_default_value_<T*>
    {
        static const bool need_set_default_value = true;
        static T* get() { return nullptr; }
    };
    template <> struct _type_default_value_<void*>
    {
        static const bool need_set_default_value = true;
        static void* get() { return nullptr; }
    };

    template <> struct _type_default_value_<i8_t>
    {
        static const bool need_set_default_value = true;
        static i8_t get() { return 0; }
    };

    template <> struct _type_default_value_<i16_t>
    {
        static const bool need_set_default_value = true;
        static i16_t get() { return 0; }
    };

    template <> struct _type_default_value_<i32_t>
    {
        static const bool need_set_default_value = true;
        static i32_t get() { return 0; }
    };

    template <> struct _type_default_value_<i64_t>
    {
        static const bool need_set_default_value = true;
        static i64_t get() { return 0; }
    };

    template <> struct _type_default_value_<u8_t>
    {
        static const bool need_set_default_value = true;
        static u8_t get() { return 0; }
    };

    template <> struct _type_default_value_<u16_t>
    {
        static const bool need_set_default_value = true;
        static u16_t get() { return 0; }
    };

    template <> struct _type_default_value_<u32_t>
    {
        static const bool need_set_default_value = true;
        static u32_t get() { return 0; }
    };

    template <> struct _type_default_value_<u64_t>
    {
        static const bool need_set_default_value = true;
        static u64_t get() { return 0; }
    };

    

    //select type according to the condition given
    template <bool C, class A, class B> struct _type_switch_
    {
    };
    template <class A, class B> struct _type_switch_<true, A, B>
    {
        typedef A type;
    };
    template <class A, class B> struct _type_switch_<false, A, B>
    {
        typedef B type;
    };
}

// to judge is a object simple type
template <typename T> struct _is_simple_type_
{
    static const bool value = sizeof(T) <= sizeof(void*) ? true : false;
};

template <> struct _is_simple_type_<pilo::i64_t>
{
    static const bool value = true;
};

template <> struct _is_simple_type_<pilo::u64_t>
{
    static const bool value = true;
};

template <> struct _is_simple_type_<double>
{
    static const bool value = true;
};

template <> struct _is_simple_type_<long double>
{
    static const bool value = true;
};

#define DECLARE_SIMPLE_TYPE(T) \
    template <> struct _is_simple_type_<T> \
    { \
    static const bool value = true; \
};

template <typename T> class ref_compare_less
{
private:
    typedef T value_type;

public:
    bool operator () (const value_type& a, const value_type& b) const
    {
        return (a) < (b);
    }
};

template <typename T> class ref_compare_greater
{
private:
    typedef T value_type;

public:
    bool operator () (const value_type& a, const value_type& b) const
    {
        return (b) < (a);
    }
};


template <typename T> class ref_gel_compare_less
{
private:
    typedef T value_type;

public:
    pilo::i32_t operator () (const value_type& a, const value_type& b) const
    {
        if ((b) < (a))
        {
            return 1;
        }
        else if ((a) < (b))
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
};

template <typename T> class ref_gel_compare_greater
{
private:
    typedef T value_type;

public:
    pilo::i32_t operator () (const value_type& a, const value_type& b) const
    {
        if ((a) < (b))
        {
            return 1;
        }
        else if ((b) < (a))
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
};

template <typename T> class ptr_compare_less
{
private:
    typedef T value_type;

public:
    bool operator () (const value_type* a, const value_type* b) const
    {
        return (*a) < (*b);
    }
};

template <typename T> class ptr_compare_greater
{
private:
    typedef T value_type;

public:
    bool operator () (const value_type* a, const value_type* b) const
    {
        return (*b) < (*a);
    }
};

template <typename T> class ptr_gel_compare_less
{
private:
    typedef T value_type;

public:
    pilo::i32_t operator () (const value_type* a, const value_type* b) const
    {
        if ((*b) < (*a))
        {
            return 1;
        }
        else if ((*a) < (*b))
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
};

template <typename T> class ptr_gel_compare_greater
{
private:
    typedef T value_type;

public:
    pilo::i32_t operator () (const value_type* a, const value_type* b) const
    {
        if ((*a) < (*b))
        {
            return 1;
        }
        else if ((*b) < (*a))
        {
            return -1;
        }
        else
        {
            return 0;
        }
    }
};

typedef enum
{
    eGPE_None = 0,
    eGPE_Read = 4,
    eGPE_Write = 2,
    eGPE_Exec = 1,

    eGPE_ReadWrite = 6,
    eGPE_ReadExec = 5,
    eGPE_WriteExec = 3,
    eGPE_All = 7,

} PiloGenericPrivillegeEnumeration;


template <typename T>
void pilo_set_flag_bit_by_value(T v, T b)
{
    v |= b;
}

template <typename T>
void pilo_clear_flag_bit_by_value(T v, T b)
{
    v &= (~b);
}

template <typename T>
void pilo_set_flag_bit_by_index(T v, T b, bool enable)
{
    if (enable)
    {
        v |= (1 << b);
    }
    else
    {
        v &= (~(1 << b));
    }

}

template <typename T>
bool pilo_test_flag_bit_by_index(T v, T b)
{
    if (v & (1 << b) == 0)
    {
        return false;
    }

    return true;
}

template <typename T>
bool pilo_test_flag_bit_by_value(T v, T b)
{
    if (v & b == 0)
    {
        return false;
    }

    return true;
}