#ifndef _pilo_hpp_
#define _pilo_hpp_

#include <memory>
#include <thread>
#include <atomic>

#include "error.hpp"
#include "preprocessor.hpp"
#include "atomic_capi.hpp"

#ifdef WINDOWS
#   ifdef pilo_shared_EXPORTS
#       define DLL_API _declspec(dllexport)
#   else
#       define DLL_API _declspec(dllimport)
#   endif

#else
#define DLL_API

#endif

#ifdef WINDOWS


#   define PMI_PATH_MAX     MAX_PATH

#else
#   define PMI_PATH_MAX     PATH_MAX

#endif // WINDOWS

#define TRISTATE_TRUE   (1)
#define TRISTATE_FALSE  (0)
#define TRISTATE_UNKNOW (-1)

#define PMF_MAKE_U8(l, h)      ((unsigned char)(((unsigned char)(((unsigned char)(l)) & 0xf)) | ((unsigned char)((unsigned char)(((unsigned char)(h)) & 0xf))) << 4))
#define PMF_LO_NIMBLE_OF_8(w)            ((unsigned char)(((unsigned char)(w)) & 0xf))
#define PMF_HI_NIMBLE_OF_8(w)           ((unsigned char)((((unsigned char)(w)) >> 4) & 0xf))

#define PMF_MAKE_U16(a, b)      ((unsigned short)(((unsigned byte)(((unsigned long)(a)) & 0xff)) | ((unsigned short)((unsigned byte)(((unsigned long)(b)) & 0xff))) << 8))
#define PMF_MAKE_I32(a, b)      ((long)(((unsigned short)(((unsigned long)(a)) & 0xffff)) | ((unsigned int)((unsigned short)(((unsigned long)(b)) & 0xffff))) << 16))
#define PMF_MAKE_U32(a, b)      ((unsigned int)(((unsigned short)(((unsigned int)(a)) & 0xffff)) | ((unsigned int)((unsigned short)(((unsigned int)(b)) & 0xffff))) << 16))
#define PMF_LO_U16_OF_32(l)           ((unsigned short)(((unsigned long)(l)) & 0xffff))
#define PMF_HI_U16_OF_32(l)           ((unsigned short)((((unsigned long)(l)) >> 16) & 0xffff))
#define PMF_LO_I16_OF_32(l)           ((short)(((unsigned long)(l)) & 0xffff))
#define PMF_HI_I16_OF_32(l)           ((short)((((unsigned long)(l)) >> 16) & 0xffff))
#define PMF_LO_U8_OF_16(w)            ((unsigned byte)(((unsigned long)(w)) & 0xff))
#define PMF_HI_U8_OF_16(w)           ((unsigned byte)((((unsigned long)(w)) >> 8) & 0xff))

#define PMF_MAKE_U32_BY_U16S_BE(b0,b1) (((unsigned int)(b0) << 16) | (b1))
#define PMF_MAKE_U32_BY_U16S_NE(b0,b1) (((unsigned int)(b1) << 16) | (b0))
#define PMF_MAKE_U32_BY_BYTES_BE(b0,b1,b2,b3) (((unsigned int)(b0) << 24) | ((unsigned int)(b1) << 16) | ((unsigned int)(b2) << 8) | (b3))
#define PMF_MAKE_U32_BY_BYTES_LE(b0,b1,b2,b3) (((unsigned int)(b3) << 24) | ((unsigned int)(b2) << 16) | ((unsigned int)(b1) << 8) | (b0))
#define PMF_EXTRACT_U8(var, index) ((unsigned char)((var >> (index * 8)) & 0xFF))
#define PMF_EXTRACT_U16(var, index) ((unsigned short)((var >> (index * 16)) & 0xFFFF))

#define PMI_INVALID_PTR			(void*)(~0)

#define PMI_INVALID_THREAD_ID   (::pilo::os_thread_id_t (-1))
#define PMI_INVALID_TIMESTAMP   (::pilo::i64_t  (~1))

#define PMF_HEAP_CALLOC calloc
#define PMF_HEAP_MALLOC malloc
#define PMF_HEAP_FREE free
#define PMF_HEAP_REALLOC realloc

#define PMF_COMPARE_HEAP_FREE(ptr, buffer_ptr)      do { if (ptr != nullptr && ptr != buffer_ptr) { (void) free(ptr); ptr = nullptr; }} while(0)

#define PMF_INV_PATHLEN_TO_SZ(plen)	(plen==PMI_INVALID_PATH_LEN ? -1 : plen)

#if (PMI_BITS == 64)
#   define PMB_MEM_POOL_UNIT_FREED_FLAG (0xFFFFEEEEFFFFEEEE)
#else
#   define PMB_MEM_POOL_UNIT_FREED_FLAG (0xFFFFEEEE)
#endif

#define PMF_MAX_SIZE(a,b) ((a)>(b)?(a):(b))
#define PMF_MPOOL_ALIGN_SIZE(usize,cnt) PMF_MAX_SIZE((usize+sizeof(void*)-1)&(~(sizeof(void*)-1)),sizeof(void*)*(cnt))
#define PMF_ALIGN_SIZE(usize,cnt) PMF_MAX_SIZE((usize+cnt-1)&(~(cnt-1)),(cnt))
#define PMF_FREE_IF_DIFF_ADDR(cptr, ptr) do { if (cptr!=ptr&&ptr!=nullptr) PMF_HEAP_FREE(ptr); } while (0)

#define PMB_DEBUG_MPOOL_SHOW_FREELIST_DETAIL  (0x1)
#define PMB_DEBUG_MPOOL_SHOW_AVAILIST_DETAIL  (0x2)
#define PMB_DEBUG_MPOOL_SHOW_FULLLIST_DETAIL  (0x3)
#define PMB_DEBUG_MPOOL_SHOW_UNIT_IMAGE		  (0x4)



#if defined(WINDOWS)
#   define PMC_UNUSED(x) do { (void)(x); } while (0)
#   define PMF_COUNT_OF _countof
#   define PMI_MAP_FAILED_INDICATOR  (NULL)
#   define PMF_OFFSETOF   offsetof
#   define PMI_PATH_SEP '\\'
#   define PMI_PATH_OTHER_SEP '/'
#	define PMI_FILENAME_EXT_SEP  '.'
#   define PMI_OS_PARAM_PATH_MAX	(1024)
#   define PMS_PATH_SEP_S_A               "\\"
#   define PMS_PATH_SEP_S_W               L"\\"
#   define PMS_INVALID_PATH_CHAR_A      "|<>\"\'`:"
#   define PMS_INVALID_PATH_CHAR_W     L"|<>\"\'`:"
#   define PMK_THREAD_ROUTINE_CALL     __stdcall
#   define PMI_INVALID_THREAD_HANDLE    INVALID_HANDLE_VALUE
#   define PMI_INVALID_FILE_HANDLE		INVALID_HANDLE_VALUE
#   define PMS_NEW_LINE_SEQ             "\r\n"
#   define PMI_ENV_VAR_SEP              ';'
#   define PMI_ENV_VAR_SEP_STR          ";"

#else
#   define PMC_UNUSED(x) (void)x
#   define PMF_COUNT_OF(_Array) (sizeof(_Array) / sizeof(_Array[0]))
#   define PMI_MAP_FAILED_INDICATOR  (MAP_FAILED)
#   define PMF_OFFSETOF   offsetof
#   define PMI_PATH_SEP		'/'
#   define PMI_PATH_OTHER_SEP '\\'
#	define PMI_FILENAME_EXT_SEP  '.'
#   define PMI_OS_PARAM_PATH_MAX	PATH_MAX
#   define PMS_PATH_SEP_S_A               "/"
#   define PMS_PATH_SEP_S_W               L"/"
#   define PMS_INVALID_PATH_CHAR_A      "|<>\"\'`"
#   define PMS_INVALID_PATH_CHAR_W     L"|<>\"\'`"
#   define PMK_THREAD_ROUTINE_CALL
#   define PMI_INVALID_THREAD_HANDLE    0
#   define PMI_INVALID_FILE_HANDLE    (-1)
#   define PMS_NEW_LINE_SEQ             "\n"
#   define PMI_ENV_VAR_SEP              ':'
#   define PMI_ENV_VAR_SEP_STR          ":"

#   if defined(_UNICODE) || defined (UNICODE)
#       define  TCHAR       wchar_t
#       define __T(x)       L ## x
#   else
#       define  TCHAR       char
#       define __T(x)       x
#   endif

#   define _T(x)       __T(x)
#   define _TEXT(x)    __T(x)

#endif

#   define PMI_PILO_SPARAM_PATH_MAX	((::pilo::pathlen_t) 0xFFFE)

#define PMC_DISABLE_COPY(Class) \
private:\
    Class(const Class &); \
    Class &operator=(const Class &);

#define PMC_HEAP_THREAD(Class) \
protected:\
    ~Class();


template <class T> const T& pilo_min(const T& t1, const T& t2) { return t1 < t2 ? t1 : t2; }
template <class T> const T& pilo_max(const T& t1, const T& t2) { return t1 < t2 ? t2 : t1; }

#if defined(WIN32) || defined(WINDOWS)
#define _USER_BREAK_	__debugbreak() //{ do { __asm { int 3 }; } while (0); }
#else
#define _USER_BREAK_ { do { asm ( "int $3;" ); } while(0); }
#endif

#   define DEBUG_BREAK		_USER_BREAK_
#   define RELEASE_BREAK	_USER_BREAK_

#define PMC_ASSERT(expression) \
			{ do { if ( ! (expression) ) { RELEASE_BREAK; } } while(0); }


#if (__cplusplus < 201103)
#   define PML_NOEXCEPT
#else
#   define PML_NOEXCEPT     noexcept
#endif

#define PMC_REGULATE_BETWEEN(d, lc, lv, hc, hv)	\
do {  \
	if (d < (lc)) d = (lv); \
	else if (d > (hc)) d = (hv);\
} while(0)

#define PMC_NOT_IN_RANGE_RET(d, lc, hc, r)\
 do {  \
	if (d < (lc)) return r; \
	else if (d > (hc)) return r;\
} while(0)

#define PMC_NO_MORE_THAN(d, lc)	\
do {  \
	if (d > (lc)) {d = (lc);} \
} while(0)

#define M_DISABLE_COPY(Class) \
    Class(const Class &); \
    Class &operator=(const Class &);




#ifndef WINDOWS

#   if BYTE_ORDER == LITTLE_ENDIAN
inline uint64_t htonll(uint64_t val)
{
    return (((uint64_t)htonl(val)) << 32) + htonl(val >> 32);
}

inline uint64_t ntohll(uint64_t val)
{
    return (((uint64_t)ntohl(val)) << 32) + ntohl(val >> 32);
}

#   elif BYTE_ORDER == BIG_ENDIAN
#define htonll(val) val   
#define ntohll(val) val

#else
error "Unknown byte order"

#endif // LITTLE_ENDIAN






#endif

#define PMO_CALL_MEMBER(obj, pmf) ((obj).*(pmf))

#if PMI_PFD_HAS_CONSTEXPR == 0
#	define PMC_CONSTEXPR

#else
#	define PMC_CONSTEXPR constexpr
#endif


// Constexpr if
#ifndef PMC_CONSTEXPR_IF
#   if (defined(_MSC_VER) && defined(_HAS_CXX17) && _HAS_CXX17) || __cplusplus > 201402L
#       define PMC_CONSTEXPR_IF if constexpr
#       define MOODYCAMEL_MAYBE_UNUSED [[maybe_unused]]
#else
#       define PMC_CONSTEXPR_IF if
#       define MOODYCAMEL_MAYBE_UNUSED
#   endif
#endif

#ifdef PMD_PFD_EXCEPTIONS_ENABLED
#   define  PMC_TRY try
#   define  PMC_CATCH(...) catch(__VA_ARGS__)
#   define  PMC_RETHROW throw
#   define  PMC_THROW(expr) throw (expr)

#else
#   define  PMC_TRY PMC_CONSTEXPR_IF (true)
#   define  PMC_CATCH(...) else PMC_CONSTEXPR_IF (false)
#   define  PMC_RETHROW
#   define  PMC_THROW(expr)
#endif

#ifndef PMC_NOEXCEPT
#   if !defined(PMD_PFD_EXCEPTIONS_ENABLED)
#       define PMC_NOEXCEPT
#       define PMC_NOEXCEPT_CTOR(type, valueType, expr) true
#       define PMC_NOEXCEPT_ASSIGN(type, valueType, expr) true
#   elif defined(_MSC_VER) && defined(_NOEXCEPT) && _MSC_VER < 1800
        // VS2012's std::is_nothrow_[move_]constructible is broken and returns true when it shouldn't :-(
        // We have to assume *all* non-trivial constructors may throw on VS2012!
#       define PMC_NOEXCEPT _NOEXCEPT
#       define PMC_NOEXCEPT_CTOR(type, valueType, expr) (std::is_rvalue_reference<valueType>::value && std::is_move_constructible<type>::value ? std::is_trivially_move_constructible<type>::value : std::is_trivially_copy_constructible<type>::value)
#       define PMC_NOEXCEPT_ASSIGN(type, valueType, expr) ((std::is_rvalue_reference<valueType>::value && std::is_move_assignable<type>::value ? std::is_trivially_move_assignable<type>::value || std::is_nothrow_move_assignable<type>::value : std::is_trivially_copy_assignable<type>::value || std::is_nothrow_copy_assignable<type>::value) && MOODYCAMEL_NOEXCEPT_CTOR(type, valueType, expr))
#   elif defined(_MSC_VER) && defined(_NOEXCEPT) && _MSC_VER < 1900
#       define PMC_NOEXCEPT _NOEXCEPT
#       define PMC_NOEXCEPT_CTOR(type, valueType, expr) (std::is_rvalue_reference<valueType>::value && std::is_move_constructible<type>::value ? std::is_trivially_move_constructible<type>::value || std::is_nothrow_move_constructible<type>::value : std::is_trivially_copy_constructible<type>::value || std::is_nothrow_copy_constructible<type>::value)
#       define PMC_NOEXCEPT_ASSIGN(type, valueType, expr) ((std::is_rvalue_reference<valueType>::value && std::is_move_assignable<type>::value ? std::is_trivially_move_assignable<type>::value || std::is_nothrow_move_assignable<type>::value : std::is_trivially_copy_assignable<type>::value || std::is_nothrow_copy_assignable<type>::value) && MOODYCAMEL_NOEXCEPT_CTOR(type, valueType, expr))
#   else
#       define PMC_NOEXCEPT noexcept
#       define PMC_NOEXCEPT_CTOR(type, valueType, expr) noexcept(expr)
#       define PMC_NOEXCEPT_ASSIGN(type, valueType, expr) noexcept(expr)
#   endif
#endif

// VS2012 doesn't support deleted functions. 
// In this case, we declare the function normally but don't define it. A link error will be generated if the function is called.
#ifndef PMC_METHOD_DELETED
#if defined(_MSC_VER) && _MSC_VER < 1800
#define PMC_METHOD_DELETED
#else
#define PMC_METHOD_DELETED = delete
#endif
#endif

namespace pilo
{
#ifndef PMC_ALIGNAS
    // VS2013 doesn't support alignas or alignof, and align() requires a constant literal
#   if defined(_MSC_VER) && _MSC_VER <= 1800
#       define PMC_ALIGNAS(alignment) __declspec(align(alignment))
#       define PMC_ALIGNOF(obj) __alignof(obj)
#       define PMC_ALIGNED_TYPE_LIKE(T, obj) typename pilo::Vs2013Aligned<std::alignment_of<obj>::value, T>::type
        template<int Align, typename T> struct Vs2013Aligned { };  // default, unsupported alignment
        template<typename T> struct Vs2013Aligned<1, T> { typedef __declspec(align(1)) T type; };
        template<typename T> struct Vs2013Aligned<2, T> { typedef __declspec(align(2)) T type; };
        template<typename T> struct Vs2013Aligned<4, T> { typedef __declspec(align(4)) T type; };
        template<typename T> struct Vs2013Aligned<8, T> { typedef __declspec(align(8)) T type; };
        template<typename T> struct Vs2013Aligned<16, T> { typedef __declspec(align(16)) T type; };
        template<typename T> struct Vs2013Aligned<32, T> { typedef __declspec(align(32)) T type; };
        template<typename T> struct Vs2013Aligned<64, T> { typedef __declspec(align(64)) T type; };
        template<typename T> struct Vs2013Aligned<128, T> { typedef __declspec(align(128)) T type; };
        template<typename T> struct Vs2013Aligned<256, T> { typedef __declspec(align(256)) T type; };
#   else
        template<typename T> struct identity { typedef T type; };
#       define PMC_ALIGNAS(alignment) alignas(alignment)
#       define PMC_ALIGNOF(obj) alignof(obj)
#       define PMC_ALIGNED_TYPE_LIKE(T, obj) alignas(alignof(obj)) typename pilo::identity<T>::type
#   endif
#endif
}

// Compiler-specific likely/unlikely hints
namespace pilo {

    template<typename T>
    static inline T ceil_to_pow_2(T x)
    {
        static_assert(std::is_integral<T>::value && !std::numeric_limits<T>::is_signed, "ceil_to_pow_2 is intended to be used only with unsigned integer types");

        // Adapted from http://graphics.stanford.edu/~seander/bithacks.html#RoundUpPowerOf2
        --x;
        x |= x >> 1;
        x |= x >> 2;
        x |= x >> 4;
        for (std::size_t i = 1; i < sizeof(T); i <<= 1) {
            x |= x >> (i << 3);
        }
        ++x;
        return x;
    }

    


#if defined(__GNUC__)
    static inline bool (likely)(bool x) { return __builtin_expect((x), true); }
    static inline bool (unlikely)(bool x) { return __builtin_expect((x), false); }
#else
    static inline bool (likely)(bool x) { return x; }
    static inline bool (unlikely)(bool x) { return x; }
#endif


    template<typename T>
    struct const_numeric_max {
        static_assert(std::is_integral<T>::value, "const_numeric_max can only be used with integers");
        static const T value = std::numeric_limits<T>::is_signed
            ? (static_cast<T>(1) << (sizeof(T) * CHAR_BIT - 1)) - static_cast<T>(1)
            : static_cast<T>(-1);
    };

#if defined(__GLIBCXX__)
    typedef ::max_align_t std_max_align_t;      // libstdc++ forgot to add it to std:: for a while
#else
    typedef std::max_align_t std_max_align_t;   // Others (e.g. MSVC) insist it can *only* be accessed via std::
#endif

    // Some platforms have incorrectly set max_align_t to a type with <8 value_type_bytes alignment even while supporting
    // 8-byte aligned scalar values (*cough* 32-bit iOS). Work around this with our own union. See issue #64.
    typedef union {
        std_max_align_t x;
        long long y;
        void* z;
    } max_align_t;    

    template<typename T>
    static inline bool circular_less_than(T a, T b)
    {
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable: 4554)
#endif
        static_assert(std::is_integral<T>::value && !std::numeric_limits<T>::is_signed, "circular_less_than is intended to be used only with unsigned integer types");
        return static_cast<T>(a - b) > static_cast<T>(static_cast<T>(1) << static_cast<T>(sizeof(T) * CHAR_BIT - 1));
#ifdef _MSC_VER
#pragma warning(pop)
#endif
    }

    template<typename U>
    static inline char* align_for(char* ptr)
    {
        const std::size_t alignment = std::alignment_of<U>::value;
        return ptr + (alignment - (reinterpret_cast<std::uintptr_t>(ptr) % alignment)) % alignment;
    }

    template<typename T>
    static inline void swap_relaxed(std::atomic<T>& left, std::atomic<T>& right)
    {
        T temp = std::move(left.load(std::memory_order_relaxed));
        left.store(std::move(right.load(std::memory_order_relaxed)), std::memory_order_relaxed);
        right.store(std::move(temp), std::memory_order_relaxed);
    }

    template<typename T>
    static inline T const& nomove(T const& x)
    {
        return x;
    }

    template<bool Enable>
    struct nomove_if
    {
        template<typename T>
        static inline T const& eval(T const& x)
        {
            return x;
        }
    };

    template<>
    struct nomove_if<false>
    {
        template<typename U>
        static inline auto eval(U&& x)
            -> decltype(std::forward<U>(x))
        {
            return std::forward<U>(x);
        }
    };

    template<typename It>
    static inline auto deref_noexcept(It& it) PMC_NOEXCEPT -> decltype(*it)
    {
        return *it;
    }

#if defined(__clang__) || !defined(__GNUC__) || __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 8)
    template<typename T> struct is_trivially_destructible : std::is_trivially_destructible<T> { };
#else
    template<typename T> struct is_trivially_destructible : std::has_trivial_destructor<T> { };
#endif



}


namespace pilo
{
    

    template<bool use32> struct _hash_32_or_64 {
        static inline std::uint32_t hash(std::uint32_t h)
        {
            // MurmurHash3 finalizer -- see https://code.google.com/p/smhasher/source/browse/trunk/MurmurHash3.cpp
            // Since the thread ID is already unique, all we really want to do is propagate that
            // uniqueness evenly across all the bits, so that we can use a subset of the bits while
            // reducing collisions significantly
            h ^= h >> 16;
            h *= 0x85ebca6b;
            h ^= h >> 13;
            h *= 0xc2b2ae35;
            return h ^ (h >> 16);
        }
    };
    template<> struct _hash_32_or_64<1> {
        static inline std::uint64_t hash(std::uint64_t h)
        {
            h ^= h >> 33;
            h *= 0xff51afd7ed558ccd;
            h ^= h >> 33;
            h *= 0xc4ceb9fe1a85ec53;
            return h ^ (h >> 33);
        }
    };

    
    template<typename T>
    void set_if_ptr_is_not_null(T* ptr, const T & value)
    {
        if (ptr != nullptr)
            *ptr = value;
    }

    


    

}



#define PILO_STARTUP_INIT(c,v)   do { if (PILO_OK != ::pilo::core::process::pilo_startup(c,v)) { exit(-1); } } while(0)
#define PILO_CONTEXT    (::pilo::core::process::pilo_context())
#define PILO_TIMESTAMP  (::pilo::core::datetime::timestamp_milli_system())

#endif