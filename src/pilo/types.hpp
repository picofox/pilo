#ifndef __pilo_types_hpp_
#define __pilo_types_hpp_

#include "os_headers.hpp"
#include <string>
#include <sstream>

#ifdef WINDOWS
#define PILO_INT8_MIN         (-127i8 - 1)
#define PILO_INT16_MIN        (-32767i16 - 1)
#define PILO_INT32_MIN        (-2147483647i32 - 1)
#define PILO_INT64_MIN        (-9223372036854775807i64 - 1)
#define PILO_INT8_MAX         127i8
#define PILO_INT16_MAX        32767i16
#define PILO_INT32_MAX        2147483647i32
#define PILO_INT64_MAX        9223372036854775807i64
#define PILO_UINT8_MAX        0xffui8
#define PILO_UINT16_MAX       0xffffui16
#define PILO_UINT32_MAX       0xffffffffui32
#define PILO_UINT64_MAX       0xffffffffffffffffui64

#else

#define PILO_INT8_MIN   CHAR_MIN
#define PILO_INT16_MIN  SHRT_MIN
#define PILO_INT32_MIN  INT_MIN
#define PILO_INT64_MIN  LONG_MIN
#define PILO_INT8_MAX   CHAR_MAX
#define PILO_INT16_MAX  SHRT_MAX
#define PILO_INT32_MAX  INT_MAX
#define PILO_INT64_MAX  LONG_MAX

#define PILO_UINT8_MAX  UCHAR_MAX
#define PILO_UINT16_MAX USHRT_MAX
#define PILO_UINT32_MAX UINT_MAX
#define PILO_UINT64_MAX ULONG_MAX

#endif

namespace pilo 
{

	
#if defined(WIN32) || defined(WINDOWS)
    typedef unsigned                thread_routine_return_t;
    typedef SOCKET                  os_socket_t;
    typedef DWORD                   os_error_number_t;
    typedef HANDLE                  os_file_handle_t;
    typedef DWORD                   os_process_id_t;
    typedef HMODULE                 os_dll_module_handle_t;
    typedef unsigned int            os_thread_id_t;
    typedef CONDITION_VARIABLE      os_event_handle_t;
    typedef CRITICAL_SECTION        os_cs_handle_t;
	typedef DWORD					os_pid_t;

#else
    typedef void* thread_routine_return_t;
    typedef int                     os_socket_t;
    typedef int                     os_error_number_t;
    typedef int                     os_file_handle_t;
    typedef int                     os_process_id_t;
    typedef void* os_dll_module_handle_t;
    typedef pthread_t               os_thread_id_t;
    typedef pthread_cond_t          os_event_handle_t;
    typedef pthread_mutex_t         os_cs_handle_t;
	typedef pid_t					os_pid_t;
#endif

    template <size_t n> struct __regtype__ { typedef int int_r; typedef unsigned int uint_r; };
    template <> struct __regtype__<8> { typedef long long int_r; typedef unsigned long long uint_r; };
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
    typedef bool                            bi_t;

    typedef float                           f32_t;
    typedef double                          f64_t;

    typedef unsigned long long				object_id_t;

    typedef void* pointer;
    typedef const void* const_pointer;

#if PMI_PFD_PTR_SIZE == 4
    typedef ::pilo::i32_t                    s_pointer;
    typedef ::pilo::u32_t                    u_pointer;
#else
    typedef ::pilo::i64_t                    s_pointer;
    typedef ::pilo::u64_t                    u_pointer;
#endif

    typedef uint_r							address_t;
    typedef i64_t                           os_tick_t;
    //typedef size_t                          i64_t;
    //typedef int_r                           i64_t;
    typedef time_t                          tsec_t;
    typedef ::pilo::u16_t                   pathlen_t;
	typedef i32_t err_t;


	template <typename CHAR_T>
	class cstr_ref
	{
	public:
		CHAR_T* ptr;
		pilo::i64_t  length;

		cstr_ref() : ptr(nullptr), length(0) {}
		cstr_ref(CHAR_T* b, pilo::i64_t  l) : ptr(b), length(l) {}

		std::string& to_string(std::string& ret) const
		{
			ret.assign(ptr, length);
			return ret;
		}

		std::string to_string() const
		{
			return std::string(ptr, length);
		}

		const wchar_t* to_cstring(wchar_t* buffer, ::pilo::i64_t len) const
		{
			if (ptr == nullptr)
				return nullptr;
			::pilo::i64_t src_len = length;
			if (src_len >= len)
			{
				src_len = len - 1;
			}

			if (src_len > 0)
			{
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
				wcsncpy(buffer, ptr, src_len);
#   pragma warning( pop ) 
#else
				wcsncpy(buffer, ptr, src_len);
#endif
			}
			buffer[src_len] = 0;
			return buffer;
		}

		const char* to_cstring(char* buffer, ::pilo::i64_t len) const
		{
			if (ptr == nullptr)
				return nullptr;
			::pilo::i64_t src_len = length;
			if (src_len >= len)
			{
				src_len = len - 1;
			}

			if (src_len > 0)
			{
#if defined(WINDOWS)
#   pragma warning( push )
#   pragma warning(disable:4996)
				strncpy(buffer, ptr, src_len);
#   pragma warning( pop ) 
#else
				strncpy(buffer, ptr, src_len);
#endif
			}
			buffer[src_len] = 0;
			return buffer;
		}


		bool init(const CHAR_T* beg, const CHAR_T* end)
		{
			if (end < beg)
				return false;

			this->ptr = (CHAR_T*)beg;
			this->length = end - beg;
			return true;
		}

		bool init(const CHAR_T* begin, ::pilo::i64_t sz)
		{
			this->ptr = (CHAR_T*)begin;
			this->length = sz;
			return true;
		}

		CHAR_T* end_ptr() const
		{
			return (ptr + length);
		}

		CHAR_T* last_char() const
		{
			return (ptr + length - 1);
		}

		bool valid() const
		{
			return (ptr != nullptr && *ptr != 0 && length > 0);
		}

		bool is_empty() const
		{
			return ((ptr != nullptr && *ptr == 0) || length == 0);
		}
	};

	enum class leading_trim_mode_enum {
		eNone,
		eByte,
		eAll
	};


	template<typename INT_T>
	class bit_flag
	{
	public:
		static bool test_index(INT_T v, u32_t b)
		{
			if ((v & (1 << b)) == 0)
			{
				return false;
			}
			return true;
		}
		static bool test_value(INT_T v, INT_T b)
		{
			if ((v & b) == 0)
			{
				return false;
			}
			return true;
		}
		static void mark_index(INT_T& v, u32_t b)
		{
			v |= (INT_T)(1 << b);
		}
		static void mark_value(INT_T& v, INT_T b)
		{
			v |= b;
		}
		static void clear_index(INT_T& v, u32_t b)
		{
			v &= (INT_T)(~(1 << b));
		}
		static void clear_value(INT_T& v, INT_T b)
		{
			v &= (~b);
		}
		static ::pilo::i32_t count_bit_1(INT_T n)
		{
			unsigned int c = 0;
			while (n != 0)
			{
				if ((n & 1) == 1)
				{
					++c;
				}
				n = n >> 1;
			}
			return c;
		}
		static ::pilo::i32_t count_bit_1(::pilo::i8_t sn)
		{
			unsigned int c = 0;
			::pilo::u8_t n = (::pilo::u8_t)sn;
			while (n != 0)
			{
				if ((n & 1) == 1)
				{
					++c;
				}
				n = n >> 1;
			}
			return c;
		}
		static ::pilo::i32_t count_bit_1(::pilo::i16_t sn)
		{
			unsigned int c = 0;
			::pilo::u16_t n = (::pilo::u16_t)sn;
			while (n != 0)
			{
				if ((n & 1) == 1)
				{
					++c;
				}
				n = n >> 1;
			}
			return c;
		}
		static ::pilo::i32_t count_bit_1(::pilo::i32_t sn)
		{
			unsigned int c = 0;
			::pilo::u32_t n = (::pilo::u32_t)sn;
			while (n != 0)
			{
				if ((n & 1) == 1)
				{
					++c;
				}
				n = n >> 1;
			}
			return c;
		}
		static ::pilo::i32_t count_bit_1(::pilo::i64_t sn)
		{
			unsigned int c = 0;
			::pilo::u64_t n = (::pilo::u64_t)sn;
			while (n != 0)
			{
				if ((n & 1) == 1)
				{
					++c;
				}
				n = n >> 1;
			}
			return c;
		}
	public:
		bit_flag(INT_T dfl) { _flags = dfl; }
		bit_flag() { _flags = 0; }
		bit_flag& operator=(INT_T v) { _flags = v; return *this; }
		inline INT_T data() const { return _flags; }
		inline void reset() { _flags = 0; }
		inline bool test_index(::pilo::u32_t pos) const { return bit_flag::test_index(_flags, pos); }
		inline bool test_value(INT_T v) const { return bit_flag::test_value(_flags, v); }
		inline void mark_index(::pilo::u32_t pos) { bit_flag::mark_index(_flags, pos); }
		inline void mark_value(INT_T v) { bit_flag::mark_value(_flags, v); }
		inline void clear_index(::pilo::u32_t pos) { bit_flag::clear_index(_flags, pos); }
		inline void clear_value(INT_T v) { bit_flag::clear_value(_flags, v); }
		inline void toggle_index(::pilo::u32_t pos)
		{
			if (this->test_index(pos))
				this->clear_index(pos);
			else
				this->mark_index(pos);
		}
		inline void toggle_value(INT_T v)
		{
			if (this->test_value(v))
				this->clear_value(v);
			else
				this->mark_value(v);
		}
		inline void set_index(::pilo::u32_t pos, bool enable)
		{
			if (enable)
				this->mark_index(pos);
			else
				this->clear_index(pos);
		}
		inline void set_value(INT_T v, bool enable)
		{
			if (enable)
				this->mark_value(v);
			else
				this->clear_value(v);
		}
		inline ::pilo::i32_t count_1_bits() const
		{
			return bit_flag::count_bit_1(_flags);
		}

	private:
		INT_T _flags;
	};

	enum class seek_whence_enum : pilo::u8_t {
		begin = (pilo::u8_t) 0,
		current = (pilo::u8_t) 1,
		end = (pilo::u8_t) 2,
	};

	enum class ioop_type_enum : pilo::u8_t {
		none = (pilo::u8_t) 0,
		read = (pilo::u8_t) 1,
		write = (pilo::u8_t) 2,
		read_write = (pilo::u8_t) 3,
	};




	template <typename T> struct _type_default_const_value_
	{
		static const T get() { static const T v; return v; }
	};
	template <typename T> struct _type_default_const_value_<const T*>
	{
		static const T* get() { static const T* v = nullptr; return v; }
	};	
	template <> struct _type_default_const_value_<::pilo::i8_t>
	{
		static const ::pilo::i8_t get() { static const ::pilo::i8_t v = 0; return v; }
	};
	template <> struct _type_default_const_value_<::pilo::u8_t>
	{
		static const ::pilo::u8_t get() { static const ::pilo::u8_t v = 0; return v; }
	};
	template <> struct _type_default_const_value_<::pilo::i16_t>
	{
		static const ::pilo::i16_t get() { static const ::pilo::i16_t v = 0; return v; }
	};
	template <> struct _type_default_const_value_<::pilo::u16_t>
	{
		static const ::pilo::u16_t get() { static const ::pilo::u16_t v = 0; return v; }
	};
	template <> struct _type_default_const_value_<::pilo::i32_t>
	{
		static const ::pilo::i32_t get() { static const ::pilo::i32_t v = 0; return v; }
	};
	template <> struct _type_default_const_value_<::pilo::u32_t>
	{
		static const ::pilo::u32_t get() { static const ::pilo::u32_t v = 0; return v; }
	};
	template <> struct _type_default_const_value_<::pilo::i64_t>
	{
		static const ::pilo::i64_t get() { static const ::pilo::i64_t v = 0; return v; }
	};
	template <> struct _type_default_const_value_<::pilo::u64_t>
	{
		static const ::pilo::u64_t get() { static const ::pilo::u64_t v = 0; return v; }
	};
	template <> struct _type_default_const_value_<bool>
	{
		static const bool get() { static const bool v = false; return v; }
	};
	template <> struct _type_default_const_value_<::pilo::f32_t>
	{
		static const ::pilo::f32_t get() { static const ::pilo::f32_t v = 0.0f; return v; }
	};
	template <> struct _type_default_const_value_<::pilo::f64_t>
	{
		static const ::pilo::f64_t get() { static const ::pilo::f64_t v = 0.0; return v; }
	};
	template <> struct _type_default_const_value_<char*>
	{
		static const char* get() { static const char* v = nullptr; return v; }
	};
	template <> struct _type_default_const_value_<std::string>
	{
		static const std::string get() { static const std::string v; return v; }
	};


	

	template <typename T> struct _invalid_object_
	{
		static T& get() { static T v; return v; }
	};

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

	template <> struct _type_default_value_<f32_t>
	{
		static const bool need_set_default_value = true;
		static f32_t get() { return 0.0f; }
	};

	template <> struct _type_default_value_<f64_t>
	{
		static const bool need_set_default_value = true;
		static f64_t get() { return 0.0; }
	};

	template <> struct _type_default_value_<std::string>
	{
		static const bool need_set_default_value = true;
		static const std::string get() { return std::string(); }
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

	template<typename T>
	T get_default() {
		return T(); // 这将尝试调用T的默认构造函数
	}

	struct _pilo_truetype
	{
		static bool get()
		{
			return true;
		}
	};

	struct _pilo_falsetype//非基本类型
	{
		static bool get()
		{
			return false;
		}
	};

	template <class _Tp>//非基本类型不特化
	struct _pilo_type_trait
	{
		typedef _pilo_falsetype   is_basetype;
	};

	template <>//基本类型的全特化
	struct _pilo_type_trait< bool>
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< char>
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< unsigned char >
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< short>
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< unsigned short >
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< wchar_t >
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< int>
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< unsigned int >
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< long>
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< unsigned long >
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< long long >
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< unsigned long long>
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< float>
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< double>
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <>
	struct _pilo_type_trait< long double >
	{
		typedef _pilo_truetype     is_basetype;
	};

	template <class _Tp>
	struct _pilo_type_trait< _Tp*>//指针类型的偏特化
	{
		typedef _pilo_truetype     is_basetype;
	};

	enum class predefined_pilo_dir_enum
	{
		cwd = 0,
		exe,
		home,
		bin,
		cnf,
		log,
		tmp,
		count,
	};

} //pilo

#endif //__pilo_types_hpp_