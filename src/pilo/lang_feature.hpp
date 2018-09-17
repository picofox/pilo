#pragma once
#include "sysdef.hpp"



#ifdef WINDOWS

#define MC_VS_VERSION	_MSC_VER


//#   if (_MSC_VER > 1800)
#		if (MC_VS_VERSION > 1800)
#       define M_CXX_FEATURE_RVALUE_REF                 1 //Rvalue references
#       define M_CXX_FEATURE_RVALUE_REF_THIS            0 //Rvalue references for *this
#       define M_CXX_FEATURE_NON_STC_DATA_MEMBER_INIT   0 //Non-static data member initializers
#       define M_CXX_FEATURE_VARIADIC_TMPL              0 //Variadic templates
#       define M_CXX_FEATURE_INITIALIZER_LIST           1 //Initializer lists
#       define M_CXX_FEATURE_STC_ASSERT                 1 //static_assert 
#       define M_CXX_FEATURE_AUTO                       1 //auto keyword
#       define M_CXX_FEATURE_TRAILING_RETURN_TYPES      1 //Trailing return types
#       define M_CXX_FEATURE_LAMBDAS                    1 //Lambdas
#       define M_CXX_FEATURE_DECLTYPE                   1 //decltype
#       define M_CXX_FEATURE_RIGHT_ANGLE_BRACKETS       1 //Right angle brackets TR1
#       define M_CXX_FEATURE_DFL_TMPL_ARG_FOR_FUNC_TMPL 0 //Default template arguments for function templates
#       define M_CXX_FEATURE_EXPRESSION_SFINAE          0 //
#       define M_CXX_FEATURE_ALIAS_TMPL                 0 //Alias templates 
#       define M_CXX_FEATURE_EXTERN_TMPL                1 //Extern templates
#       define M_CXX_FEATURE_NULLPTR                    1 //nullptr
#       define M_CXX_FEATURE_STRONG_TYPE_ENUM           1 //Strongly typed enums
#       define M_CXX_FEATURE_FORWARD_DECLARED_ENUM      1 //Forward declared enums
#       define M_CXX_FEATURE_ATTRIBUTES                 0 //Attributes
#       define M_CXX_FEATURE_CONSTEXPR                  0 //constexpr 
#       define M_CXX_FEATURE_ALIGNMENT                  0 //Alignment
#       define M_CXX_FEATURE_DELEGATING_CONSTRUCTOR     0 //Delegating constructors 
#       define M_CXX_FEATURE_INHERITING_CONSTRUCTOR     0 //Inheriting constructors 
#       define M_CXX_FEATURE_EXPLICIT_CONV_OPERATOR     0 //Explicit conversion operators
#       define M_CXX_FEATURE_CHAR_16_32_T               0 //char16_t and char32_t
#       define M_CXX_FEATURE_UNICODE_STRING_LITERAL     0 //Unicode string literals
#       define M_CXX_FEATURE_RAW_STRING_LITERA          0 //Raw string literals
#       define M_CXX_FEATURE_UNI_CHAR_NAME_IN_LITERAL   0 //Universal character names in literals
#       define M_CXX_FEATURE_USER_DEFINED_LITERAL       0 //User - defined literals 
#       define M_CXX_FEATURE_STD_LAYOUT_AND_TRIVAL_TYPE 1 //Standard - layout and trivial types
#       define M_CXX_FEATURE_DFL_AND_DELETED_FUNC       0 //Defaulted and deleted functions 
#       define M_CXX_FEATURE_EXTENDED_FRIEND_DECL       1 //Extended friend declarations
#       define M_CXX_FEATURE_EXTENDED_SIZEOF            0 //Extended sizeof
#       define M_CXX_FEATURE_INLINE_NAMESPACE           0 //Inline namespaces
#       define M_CXX_FEATURE_UNRESTRICTED_UNIONS        0 //Unrestricted unions 
#       define M_CXX_FEATURE_LOCAL_UNNAM_TYPE_TMPL_ARG  1 //Local and unnamed types as template arguments
#       define M_CXX_FEATURE_RANGE_BASED_FOR_LOOP       1 //Range - based for - loop
#       define M_CXX_FEATURE_OVERRIDE_AND_FINAL         1 //override and final 
#       define M_CXX_FEATURE_MINIMAL_GC_SUPPORT         1 //Minimal GC support 
#       define M_CXX_FEATURE_NOEXCEPT                   0 //noexcept

#       define M_CXX_FEATURE_THREAD                     1
#       define M_CXX_FEATURE_MUTEX                      1
#       define M_CXX_FEATURE_REWORDED_SEQUENCE_POINT    0 //Reworded sequence points No No No No
#       define M_CXX_FEATURE_ATOMIC                     1 //Atomics No Yes No Yes Boost.Atomic 
#       define M_CXX_FEATURE_STRONG_COMPARE             1 //Strong compare and exchange No Yes No No   
#       define M_CXX_FEATURE_BIDIRECTIONAL              1 //Bidirectional fences No Yes No No   
#       define M_CXX_FEATURE_MEMORY_MODEL               0 //Memory model No No No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING           1 //Data-dependency ordering No Yes No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING_FUNC_ANNO 0 //Data-dependency ordering: function annotation No No No No   
#       define M_CXX_FEATURE_EXCEPTION_PTR              1 //exception_ptr Yes Yes Yes Yes Boost.Exception 
#       define M_CXX_FEATURE_QUICK_EXIT                 0 //quick_exit and at_quick_exit No No No No   
#       define M_CXX_FEATURE_ATOMIC_IN_SIGNAL_HANDLERS  0 //Atomics in signal handlers No No No No   
#       define M_CXX_FEATURE_THREAD_LOCAL_STORAGE       0 //Thread-local storage No No No No Boost.Thread 
#       define M_CXX_FEATURE_MAGIC_STATICS              0 //Magic statics No No No No  

#       define M_CXX_FEATURE_SHARED_WEAK_PTR            1 // shared_ptr, weak_ptr Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_MAKE_SHARED                1 // make_ / allocate_shared, owner_before Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_UNIQUE_PTR                 1 // unique_ptr Yes Yes Yes Yes Boost.Interprocess
#       define M_CXX_FEATURE_FIXED_SIZE_ARRAY           1 // Fixed-Size Arrays Yes Yes Yes Yes Boost.Arrays 
#       define M_CXX_FEATURE_HASH_TABLE                 1 // Hash Tables Yes Yes Yes Yes Boost.Unordered 
#       define M_CXX_FEATURE_SINGLY_LINKED_LIST         1 // Singly-linked list Yes Yes Yes Yes Boost.Container 
#       define M_CXX_FEATURE_REGEXP                     1 // Regular Expressions Yes Yes Yes Yes Boost.Regex 
#       define M_CXX_FEATURE_TUPLE                      1 // Tuple Yes Yes Yes Yes Boost.Tuple 
#       define M_CXX_FEATURE_RAND_NUMBER_AND_DIST       1 // Random Numbers & Distributions Yes Yes Yes Yes Boost.Random 
#       define M_CXX_FEATURE_TYPE_TRAITS                1 // Type Traits Yes Yes Yes Yes Boost.TypeTraits 
#       define M_CXX_FEATURE_GENERALIZED_FUNC_PTR       1 // Generalized Function Pointers Yes Yes Yes Yes Boost.Function 
#       define M_CXX_FEATURE_GENERALIZED_BINDERS        1 // Generalized Binders Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_GENERALIZED_MEMBER_FUNC    1 // Generalized Member Function Adapters Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_REF_WRAPPER                1 // Reference Wrappers Yes Yes Yes Yes Boost.Ref 
#       define M_CXX_FEATURE_RETURN_TYPE_CALC           1 // Return type calculation Yes Yes Yes Yes Boost.ReturnType 
#       define M_CXX_FEATURE_NEW_ALGORITHM              1 // New algorithms Yes Yes Yes Yes Boost.Algorithm 
#       define M_CXX_FEATURE_TIME                       1 //Time No No Yes Yes Boost.Chrono 


#   elif (MC_VS_VERSION == 1800) //vc12, vs2013

#       define M_CXX_FEATURE_RVALUE_REF                 1 //Rvalue references
#       define M_CXX_FEATURE_RVALUE_REF_THIS            0 //Rvalue references for *this
#       define M_CXX_FEATURE_NON_STC_DATA_MEMBER_INIT   0 //Non-static data member initializers
#       define M_CXX_FEATURE_VARIADIC_TMPL              0 //Variadic templates
#       define M_CXX_FEATURE_INITIALIZER_LIST           1 //Initializer lists
#       define M_CXX_FEATURE_STC_ASSERT                 1 //static_assert 
#       define M_CXX_FEATURE_AUTO                       1 //auto keyword
#       define M_CXX_FEATURE_TRAILING_RETURN_TYPES      1 //Trailing return types
#       define M_CXX_FEATURE_LAMBDAS                    1 //Lambdas
#       define M_CXX_FEATURE_DECLTYPE                   1 //decltype
#       define M_CXX_FEATURE_RIGHT_ANGLE_BRACKETS       1 //Right angle brackets TR1
#       define M_CXX_FEATURE_DFL_TMPL_ARG_FOR_FUNC_TMPL 0 //Default template arguments for function templates
#       define M_CXX_FEATURE_EXPRESSION_SFINAE          0 //
#       define M_CXX_FEATURE_ALIAS_TMPL                 0 //Alias templates 
#       define M_CXX_FEATURE_EXTERN_TMPL                1 //Extern templates
#       define M_CXX_FEATURE_NULLPTR                    1 //nullptr
#       define M_CXX_FEATURE_STRONG_TYPE_ENUM           1 //Strongly typed enums
#       define M_CXX_FEATURE_FORWARD_DECLARED_ENUM      1 //Forward declared enums
#       define M_CXX_FEATURE_ATTRIBUTES                 0 //Attributes
#       define M_CXX_FEATURE_CONSTEXPR                  0 //constexpr 
#       define M_CXX_FEATURE_ALIGNMENT                  0 //Alignment
#       define M_CXX_FEATURE_DELEGATING_CONSTRUCTOR     0 //Delegating constructors 
#       define M_CXX_FEATURE_INHERITING_CONSTRUCTOR     0 //Inheriting constructors 
#       define M_CXX_FEATURE_EXPLICIT_CONV_OPERATOR     0 //Explicit conversion operators
#       define M_CXX_FEATURE_CHAR_16_32_T               0 //char16_t and char32_t
#       define M_CXX_FEATURE_UNICODE_STRING_LITERAL     0 //Unicode string literals
#       define M_CXX_FEATURE_RAW_STRING_LITERA          0 //Raw string literals
#       define M_CXX_FEATURE_UNI_CHAR_NAME_IN_LITERAL   0 //Universal character names in literals
#       define M_CXX_FEATURE_USER_DEFINED_LITERAL       0 //User - defined literals 
#       define M_CXX_FEATURE_STD_LAYOUT_AND_TRIVAL_TYPE 1 //Standard - layout and trivial types
#       define M_CXX_FEATURE_DFL_AND_DELETED_FUNC       0 //Defaulted and deleted functions 
#       define M_CXX_FEATURE_EXTENDED_FRIEND_DECL       1 //Extended friend declarations
#       define M_CXX_FEATURE_EXTENDED_SIZEOF            0 //Extended sizeof
#       define M_CXX_FEATURE_INLINE_NAMESPACE           0 //Inline namespaces
#       define M_CXX_FEATURE_UNRESTRICTED_UNIONS        0 //Unrestricted unions 
#       define M_CXX_FEATURE_LOCAL_UNNAM_TYPE_TMPL_ARG  1 //Local and unnamed types as template arguments
#       define M_CXX_FEATURE_RANGE_BASED_FOR_LOOP       1 //Range - based for - loop
#       define M_CXX_FEATURE_OVERRIDE_AND_FINAL         1 //override and final 
#       define M_CXX_FEATURE_MINIMAL_GC_SUPPORT         1 //Minimal GC support 
#       define M_CXX_FEATURE_NOEXCEPT                   0 //noexcept

#       define M_CXX_FEATURE_THREAD                     1
#       define M_CXX_FEATURE_MUTEX                      1
#       define M_CXX_FEATURE_REWORDED_SEQUENCE_POINT    0 //Reworded sequence points No No No No
#       define M_CXX_FEATURE_ATOMIC                     1 //Atomics No Yes No Yes Boost.Atomic 
#       define M_CXX_FEATURE_STRONG_COMPARE             1 //Strong compare and exchange No Yes No No   
#       define M_CXX_FEATURE_BIDIRECTIONAL              1 //Bidirectional fences No Yes No No   
#       define M_CXX_FEATURE_MEMORY_MODEL               0 //Memory model No No No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING           1 //Data-dependency ordering No Yes No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING_FUNC_ANNO 0 //Data-dependency ordering: function annotation No No No No   
#       define M_CXX_FEATURE_EXCEPTION_PTR              1 //exception_ptr Yes Yes Yes Yes Boost.Exception 
#       define M_CXX_FEATURE_QUICK_EXIT                 0 //quick_exit and at_quick_exit No No No No   
#       define M_CXX_FEATURE_ATOMIC_IN_SIGNAL_HANDLERS  0 //Atomics in signal handlers No No No No   
#       define M_CXX_FEATURE_THREAD_LOCAL_STORAGE       0 //Thread-local storage No No No No Boost.Thread 
#       define M_CXX_FEATURE_MAGIC_STATICS              0 //Magic statics No No No No   

#       define M_CXX_FEATURE_SHARED_WEAK_PTR            1 // shared_ptr, weak_ptr Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_MAKE_SHARED                1 // make_ / allocate_shared, owner_before Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_UNIQUE_PTR                 1 // unique_ptr Yes Yes Yes Yes Boost.Interprocess
#       define M_CXX_FEATURE_FIXED_SIZE_ARRAY           1 // Fixed-Size Arrays Yes Yes Yes Yes Boost.Arrays 
#       define M_CXX_FEATURE_HASH_TABLE                 1 // Hash Tables Yes Yes Yes Yes Boost.Unordered 
#       define M_CXX_FEATURE_SINGLY_LINKED_LIST         1 // Singly-linked list Yes Yes Yes Yes Boost.Container 
#       define M_CXX_FEATURE_REGEXP                     1 // Regular Expressions Yes Yes Yes Yes Boost.Regex 
#       define M_CXX_FEATURE_TUPLE                      1 // Tuple Yes Yes Yes Yes Boost.Tuple 
#       define M_CXX_FEATURE_RAND_NUMBER_AND_DIST       1 // Random Numbers & Distributions Yes Yes Yes Yes Boost.Random 
#       define M_CXX_FEATURE_TYPE_TRAITS                1 // Type Traits Yes Yes Yes Yes Boost.TypeTraits 
#       define M_CXX_FEATURE_GENERALIZED_FUNC_PTR       1 // Generalized Function Pointers Yes Yes Yes Yes Boost.Function 
#       define M_CXX_FEATURE_GENERALIZED_BINDERS        1 // Generalized Binders Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_GENERALIZED_MEMBER_FUNC    1 // Generalized Member Function Adapters Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_REF_WRAPPER                1 // Reference Wrappers Yes Yes Yes Yes Boost.Ref 
#       define M_CXX_FEATURE_RETURN_TYPE_CALC           1 // Return type calculation Yes Yes Yes Yes Boost.ReturnType 
#       define M_CXX_FEATURE_NEW_ALGORITHM              1 // New algorithms Yes Yes Yes Yes Boost.Algorithm 
#       define M_CXX_FEATURE_TIME                       1 //Time No No Yes Yes Boost.Chrono 




#   elif (MC_VS_VERSION == 1700) //vc11, vs2012
#       define M_CXX_FEATURE_RVALUE_REF                 1 //Rvalue references
#       define M_CXX_FEATURE_RVALUE_REF_THIS            0 //Rvalue references for *this
#       define M_CXX_FEATURE_NON_STC_DATA_MEMBER_INIT   0 //Non-static data member initializers
#       define M_CXX_FEATURE_VARIADIC_TMPL              0 //Variadic templates
#       define M_CXX_FEATURE_INITIALIZER_LIST           0 //Initializer lists
#       define M_CXX_FEATURE_STC_ASSERT                 1 //static_assert 
#       define M_CXX_FEATURE_AUTO                       1 //auto keyword
#       define M_CXX_FEATURE_TRAILING_RETURN_TYPES      1 //Trailing return types
#       define M_CXX_FEATURE_LAMBDAS                    1 //Lambdas
#       define M_CXX_FEATURE_DECLTYPE                   1 //decltype
#       define M_CXX_FEATURE_RIGHT_ANGLE_BRACKETS       1 //Right angle brackets TR1
#       define M_CXX_FEATURE_DFL_TMPL_ARG_FOR_FUNC_TMPL 0 //Default template arguments for function templates
#       define M_CXX_FEATURE_EXPRESSION_SFINAE          0 //
#       define M_CXX_FEATURE_ALIAS_TMPL                 0 //Alias templates 
#       define M_CXX_FEATURE_EXTERN_TMPL                1 //Extern templates
#       define M_CXX_FEATURE_NULLPTR                    1 //nullptr
#       define M_CXX_FEATURE_STRONG_TYPE_ENUM           1 //Strongly typed enums
#       define M_CXX_FEATURE_FORWARD_DECLARED_ENUM      1 //Forward declared enums
#       define M_CXX_FEATURE_ATTRIBUTES                 0 //Attributes
#       define M_CXX_FEATURE_CONSTEXPR                  0 //constexpr 
#       define M_CXX_FEATURE_ALIGNMENT                  0 //Alignment
#       define M_CXX_FEATURE_DELEGATING_CONSTRUCTOR     0 //Delegating constructors 
#       define M_CXX_FEATURE_INHERITING_CONSTRUCTOR     0 //Inheriting constructors 
#       define M_CXX_FEATURE_EXPLICIT_CONV_OPERATOR     0 //Explicit conversion operators
#       define M_CXX_FEATURE_CHAR_16_32_T               0 //char16_t and char32_t
#       define M_CXX_FEATURE_UNICODE_STRING_LITERAL     0 //Unicode string literals
#       define M_CXX_FEATURE_RAW_STRING_LITERA          0 //Raw string literals
#       define M_CXX_FEATURE_UNI_CHAR_NAME_IN_LITERAL   0 //Universal character names in literals
#       define M_CXX_FEATURE_USER_DEFINED_LITERAL       0 //User - defined literals 
#       define M_CXX_FEATURE_STD_LAYOUT_AND_TRIVAL_TYPE 1 //Standard - layout and trivial types
#       define M_CXX_FEATURE_DFL_AND_DELETED_FUNC       0 //Defaulted and deleted functions 
#       define M_CXX_FEATURE_EXTENDED_FRIEND_DECL       1 //Extended friend declarations
#       define M_CXX_FEATURE_EXTENDED_SIZEOF            0 //Extended sizeof
#       define M_CXX_FEATURE_INLINE_NAMESPACE           0 //Inline namespaces
#       define M_CXX_FEATURE_UNRESTRICTED_UNIONS        0 //Unrestricted unions 
#       define M_CXX_FEATURE_LOCAL_UNNAM_TYPE_TMPL_ARG  1 //Local and unnamed types as template arguments
#       define M_CXX_FEATURE_RANGE_BASED_FOR_LOOP       1 //Range - based for - loop
#       define M_CXX_FEATURE_OVERRIDE_AND_FINAL         1 //override and final 
#       define M_CXX_FEATURE_MINIMAL_GC_SUPPORT         1 //Minimal GC support 
#       define M_CXX_FEATURE_NOEXCEPT                   0 //noexcept    1

#       define M_CXX_FEATURE_THREAD                     1
#       define M_CXX_FEATURE_MUTEX                      1
#       define M_CXX_FEATURE_REWORDED_SEQUENCE_POINT    0 //Reworded sequence points No No No No
#       define M_CXX_FEATURE_ATOMIC                     1 //Atomics No Yes No Yes Boost.Atomic 
#       define M_CXX_FEATURE_STRONG_COMPARE             1 //Strong compare and exchange No Yes No No   
#       define M_CXX_FEATURE_BIDIRECTIONAL              1 //Bidirectional fences No Yes No No   
#       define M_CXX_FEATURE_MEMORY_MODEL               0  //Memory model No No No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING           1  //Data-dependency ordering No Yes No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING_FUNC_ANNO 0  //Data-dependency ordering: function annotation No No No No   
#       define M_CXX_FEATURE_EXCEPTION_PTR              1  //exception_ptr Yes Yes Yes Yes Boost.Exception 
#       define M_CXX_FEATURE_QUICK_EXIT                 0  //quick_exit and at_quick_exit No No No No   
#       define M_CXX_FEATURE_ATOMIC_IN_SIGNAL_HANDLERS  0  //Atomics in signal handlers No No No No   
#       define M_CXX_FEATURE_THREAD_LOCAL_STORAGE       0  //Thread-local storage No No No No Boost.Thread 
#       define M_CXX_FEATURE_MAGIC_STATICS              0  //Magic statics No No No No  

#       define M_CXX_FEATURE_SHARED_WEAK_PTR            1 // shared_ptr, weak_ptr Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_MAKE_SHARED                1 // make_ / allocate_shared, owner_before Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_UNIQUE_PTR                 1 // unique_ptr Yes Yes Yes Yes Boost.Interprocess
#       define M_CXX_FEATURE_FIXED_SIZE_ARRAY           1 // Fixed-Size Arrays Yes Yes Yes Yes Boost.Arrays 
#       define M_CXX_FEATURE_HASH_TABLE                 1 // Hash Tables Yes Yes Yes Yes Boost.Unordered 
#       define M_CXX_FEATURE_SINGLY_LINKED_LIST         1 // Singly-linked list Yes Yes Yes Yes Boost.Container 
#       define M_CXX_FEATURE_REGEXP                     1 // Regular Expressions Yes Yes Yes Yes Boost.Regex 
#       define M_CXX_FEATURE_TUPLE                      1 // Tuple Yes Yes Yes Yes Boost.Tuple 
#       define M_CXX_FEATURE_RAND_NUMBER_AND_DIST       1 // Random Numbers & Distributions Yes Yes Yes Yes Boost.Random 
#       define M_CXX_FEATURE_TYPE_TRAITS                1 // Type Traits Yes Yes Yes Yes Boost.TypeTraits 
#       define M_CXX_FEATURE_GENERALIZED_FUNC_PTR       1 // Generalized Function Pointers Yes Yes Yes Yes Boost.Function 
#       define M_CXX_FEATURE_GENERALIZED_BINDERS        1 // Generalized Binders Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_GENERALIZED_MEMBER_FUNC    1 // Generalized Member Function Adapters Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_REF_WRAPPER                1 // Reference Wrappers Yes Yes Yes Yes Boost.Ref 
#       define M_CXX_FEATURE_RETURN_TYPE_CALC           1 // Return type calculation Yes Yes Yes Yes Boost.ReturnType 
#       define M_CXX_FEATURE_NEW_ALGORITHM              1 // New algorithms Yes Yes Yes Yes Boost.Algorithm 
#       define M_CXX_FEATURE_TIME                       0 // Time No No Yes Yes Boost.Chrono 


#   elif (MC_VS_VERSION == 1600) //vc10, vc2010
#       define M_CXX_FEATURE_RVALUE_REF                 1 //Rvalue references
#       define M_CXX_FEATURE_RVALUE_REF_THIS            0 //Rvalue references for *this
#       define M_CXX_FEATURE_NON_STC_DATA_MEMBER_INIT   0 //Non-static data member initializers
#       define M_CXX_FEATURE_VARIADIC_TMPL              0 //Variadic templates
#       define M_CXX_FEATURE_INITIALIZER_LIST           0 //Initializer lists
#       define M_CXX_FEATURE_STC_ASSERT                 1 //static_assert 
#       define M_CXX_FEATURE_AUTO                       1 //auto keyword
#       define M_CXX_FEATURE_TRAILING_RETURN_TYPES      1 //Trailing return types
#       define M_CXX_FEATURE_LAMBDAS                    1 //Lambdas
#       define M_CXX_FEATURE_DECLTYPE                   1 //decltype
#       define M_CXX_FEATURE_RIGHT_ANGLE_BRACKETS       1 //Right angle brackets TR1
#       define M_CXX_FEATURE_DFL_TMPL_ARG_FOR_FUNC_TMPL 0 //Default template arguments for function templates
#       define M_CXX_FEATURE_EXPRESSION_SFINAE          0 //
#       define M_CXX_FEATURE_ALIAS_TMPL                 0 //Alias templates 
#       define M_CXX_FEATURE_EXTERN_TMPL                1 //Extern templates
#       define M_CXX_FEATURE_NULLPTR                    1 //nullptr
#       define M_CXX_FEATURE_STRONG_TYPE_ENUM           1 //Strongly typed enums
#       define M_CXX_FEATURE_FORWARD_DECLARED_ENUM      1 //Forward declared enums
#       define M_CXX_FEATURE_ATTRIBUTES                 0 //Attributes
#       define M_CXX_FEATURE_CONSTEXPR                  0 //constexpr 
#       define M_CXX_FEATURE_ALIGNMENT                  0 //Alignment
#       define M_CXX_FEATURE_DELEGATING_CONSTRUCTOR     0 //Delegating constructors 
#       define M_CXX_FEATURE_INHERITING_CONSTRUCTOR     0 //Inheriting constructors 
#       define M_CXX_FEATURE_EXPLICIT_CONV_OPERATOR     0 //Explicit conversion operators
#       define M_CXX_FEATURE_CHAR_16_32_T               0 //char16_t and char32_t
#       define M_CXX_FEATURE_UNICODE_STRING_LITERAL     0 //Unicode string literals
#       define M_CXX_FEATURE_RAW_STRING_LITERA          0 //Raw string literals
#       define M_CXX_FEATURE_UNI_CHAR_NAME_IN_LITERAL   0 //Universal character names in literals
#       define M_CXX_FEATURE_USER_DEFINED_LITERAL       0 //User - defined literals 
#       define M_CXX_FEATURE_STD_LAYOUT_AND_TRIVAL_TYPE 0 //Standard - layout and trivial types
#       define M_CXX_FEATURE_DFL_AND_DELETED_FUNC       0 //Defaulted and deleted functions 
#       define M_CXX_FEATURE_EXTENDED_FRIEND_DECL       1 //Extended friend declarations
#       define M_CXX_FEATURE_EXTENDED_SIZEOF            0 //Extended sizeof
#       define M_CXX_FEATURE_INLINE_NAMESPACE           0 //Inline namespaces
#       define M_CXX_FEATURE_UNRESTRICTED_UNIONS        0 //Unrestricted unions 
#       define M_CXX_FEATURE_LOCAL_UNNAM_TYPE_TMPL_ARG  1 //Local and unnamed types as template arguments
#       define M_CXX_FEATURE_RANGE_BASED_FOR_LOOP       1 //Range - based for - loop
#       define M_CXX_FEATURE_OVERRIDE_AND_FINAL         0 //override and final 
#       define M_CXX_FEATURE_MINIMAL_GC_SUPPORT         1 //Minimal GC support 
#       define M_CXX_FEATURE_NOEXCEPT                   0 //noexcept    1

#       define M_CXX_FEATURE_THREAD                     0
#       define M_CXX_FEATURE_MUTEX                      0
#       define M_CXX_FEATURE_REWORDED_SEQUENCE_POINT    0 //Reworded sequence points No No No No
#       define M_CXX_FEATURE_ATOMIC                     0 //Atomics No Yes No Yes Boost.Atomic 
#       define M_CXX_FEATURE_STRONG_COMPARE             0 //Strong compare and exchange No Yes No No   
#       define M_CXX_FEATURE_BIDIRECTIONAL              0 //Bidirectional fences No Yes No No   
#       define M_CXX_FEATURE_MEMORY_MODEL               0 //Memory model No No No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING           0 //Data-dependency ordering No Yes No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING_FUNC_ANNO 0 //Data-dependency ordering: function annotation No No No No   
#       define M_CXX_FEATURE_EXCEPTION_PTR              1 //exception_ptr Yes Yes Yes Yes Boost.Exception 
#       define M_CXX_FEATURE_QUICK_EXIT                 0 //quick_exit and at_quick_exit No No No No   
#       define M_CXX_FEATURE_ATOMIC_IN_SIGNAL_HANDLERS  0 //Atomics in signal handlers No No No No   
#       define M_CXX_FEATURE_THREAD_LOCAL_STORAGE       0 //Thread-local storage No No No No Boost.Thread 
#       define M_CXX_FEATURE_MAGIC_STATICS              0 //Magic statics No No No No  

#       define M_CXX_FEATURE_SHARED_WEAK_PTR            1 // shared_ptr, weak_ptr Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_MAKE_SHARED                1 // make_ / allocate_shared, owner_before Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_UNIQUE_PTR                 1 // unique_ptr Yes Yes Yes Yes Boost.Interprocess
#       define M_CXX_FEATURE_FIXED_SIZE_ARRAY           1 // Fixed-Size Arrays Yes Yes Yes Yes Boost.Arrays 
#       define M_CXX_FEATURE_HASH_TABLE                 1 // Hash Tables Yes Yes Yes Yes Boost.Unordered 
#       define M_CXX_FEATURE_SINGLY_LINKED_LIST         1 // Singly-linked list Yes Yes Yes Yes Boost.Container 
#       define M_CXX_FEATURE_REGEXP                     1 // Regular Expressions Yes Yes Yes Yes Boost.Regex 
#       define M_CXX_FEATURE_TUPLE                      1 // Tuple Yes Yes Yes Yes Boost.Tuple 
#       define M_CXX_FEATURE_RAND_NUMBER_AND_DIST       1 // Random Numbers & Distributions Yes Yes Yes Yes Boost.Random 
#       define M_CXX_FEATURE_TYPE_TRAITS                1 // Type Traits Yes Yes Yes Yes Boost.TypeTraits 
#       define M_CXX_FEATURE_GENERALIZED_FUNC_PTR       1 // Generalized Function Pointers Yes Yes Yes Yes Boost.Function 
#       define M_CXX_FEATURE_GENERALIZED_BINDERS        1 // Generalized Binders Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_GENERALIZED_MEMBER_FUNC    1 // Generalized Member Function Adapters Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_REF_WRAPPER                1 // Reference Wrappers Yes Yes Yes Yes Boost.Ref 
#       define M_CXX_FEATURE_RETURN_TYPE_CALC           1 // Return type calculation Yes Yes Yes Yes Boost.ReturnType 
#       define M_CXX_FEATURE_NEW_ALGORITHM              1 // New algorithms Yes Yes Yes Yes Boost.Algorithm 
#       define M_CXX_FEATURE_TIME                       0 // Time No No Yes Yes Boost.Chrono 


#   elif (MC_VS_VERSION < 1600)
#       define M_CXX_FEATURE_THREAD                     0
#       define M_CXX_FEATURE_MUTEX                      0
#       define M_CXX_FEATURE_RVALUE_REF                 0 //Rvalue references
#       define M_CXX_FEATURE_RVALUE_REF_THIS            0 //Rvalue references for *this
#       define M_CXX_FEATURE_NON_STC_DATA_MEMBER_INIT   0 //Non-static data member initializers
#       define M_CXX_FEATURE_VARIADIC_TMPL              0 //Variadic templates
#       define M_CXX_FEATURE_INITIALIZER_LIST           0 //Initializer lists
#       define M_CXX_FEATURE_STC_ASSERT                 0 //static_assert 
#       define M_CXX_FEATURE_AUTO                       0 //auto keyword
#       define M_CXX_FEATURE_TRAILING_RETURN_TYPES      0 //Trailing return types
#       define M_CXX_FEATURE_LAMBDAS                    0 //Lambdas
#       define M_CXX_FEATURE_DECLTYPE                   0 //decltype
#       define M_CXX_FEATURE_RIGHT_ANGLE_BRACKETS       0 //Right angle brackets TR1
#       define M_CXX_FEATURE_DFL_TMPL_ARG_FOR_FUNC_TMPL 0 //Default template arguments for function templates
#       define M_CXX_FEATURE_EXPRESSION_SFINAE          0 //
#       define M_CXX_FEATURE_ALIAS_TMPL                 0 //Alias templates 
#       define M_CXX_FEATURE_EXTERN_TMPL                0 //Extern templates
#       define M_CXX_FEATURE_NULLPTR                    0 //nullptr
#       define M_CXX_FEATURE_STRONG_TYPE_ENUM           0 //Strongly typed enums
#       define M_CXX_FEATURE_FORWARD_DECLARED_ENUM      0 //Forward declared enums
#       define M_CXX_FEATURE_ATTRIBUTES                 0 //Attributes
#       define M_CXX_FEATURE_CONSTEXPR                  0 //constexpr 
#       define M_CXX_FEATURE_ALIGNMENT                  0 //Alignment
#       define M_CXX_FEATURE_DELEGATING_CONSTRUCTOR     0 //Delegating constructors 
#       define M_CXX_FEATURE_INHERITING_CONSTRUCTOR     0 //Inheriting constructors 
#       define M_CXX_FEATURE_EXPLICIT_CONV_OPERATOR     0 //Explicit conversion operators
#       define M_CXX_FEATURE_CHAR_16_32_T               0 //char16_t and char32_t
#       define M_CXX_FEATURE_UNICODE_STRING_LITERAL     0 //Unicode string literals
#       define M_CXX_FEATURE_RAW_STRING_LITERA          0 //Raw string literals
#       define M_CXX_FEATURE_UNI_CHAR_NAME_IN_LITERAL   0 //Universal character names in literals
#       define M_CXX_FEATURE_USER_DEFINED_LITERAL       0 //User - defined literals 
#       define M_CXX_FEATURE_STD_LAYOUT_AND_TRIVAL_TYPE 0 //Standard - layout and trivial types
#       define M_CXX_FEATURE_DFL_AND_DELETED_FUNC       0 //Defaulted and deleted functions 
#       define M_CXX_FEATURE_EXTENDED_FRIEND_DECL       0 //Extended friend declarations
#       define M_CXX_FEATURE_EXTENDED_SIZEOF            0 //Extended sizeof
#       define M_CXX_FEATURE_INLINE_NAMESPACE           0 //Inline namespaces
#       define M_CXX_FEATURE_UNRESTRICTED_UNIONS        0 //Unrestricted unions 
#       define M_CXX_FEATURE_LOCAL_UNNAM_TYPE_TMPL_ARG  0 //Local and unnamed types as template arguments
#       define M_CXX_FEATURE_RANGE_BASED_FOR_LOOP       0 //Range - based for - loop
#       define M_CXX_FEATURE_OVERRIDE_AND_FINAL         0 //override and final 
#       define M_CXX_FEATURE_MINIMAL_GC_SUPPORT         0 //Minimal GC support 
#       define M_CXX_FEATURE_NOEXCEPT                   0 //noexcept    1

#       define M_CXX_FEATURE_REWORDED_SEQUENCE_POINT    0 //Reworded sequence points No No No No
#       define M_CXX_FEATURE_ATOMIC                     0 //Atomics No Yes No Yes Boost.Atomic 
#       define M_CXX_FEATURE_STRONG_COMPARE             0 //Strong compare and exchange No Yes No No   
#       define M_CXX_FEATURE_BIDIRECTIONAL              0 //Bidirectional fences No Yes No No   
#       define M_CXX_FEATURE_MEMORY_MODEL               0  //Memory model No No No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING           0  //Data-dependency ordering No Yes No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING_FUNC_ANNO 0  //Data-dependency ordering: function annotation No No No No   
#       define M_CXX_FEATURE_EXCEPTION_PTR              0  //exception_ptr Yes Yes Yes Yes Boost.Exception 
#       define M_CXX_FEATURE_QUICK_EXIT                 0  //quick_exit and at_quick_exit No No No No   
#       define M_CXX_FEATURE_ATOMIC_IN_SIGNAL_HANDLERS  0  //Atomics in signal handlers No No No No   
#       define M_CXX_FEATURE_THREAD_LOCAL_STORAGE       0  //Thread-local storage No No No No Boost.Thread 
#       define M_CXX_FEATURE_MAGIC_STATICS              0  //Magic statics No No No No  

#       define M_CXX_FEATURE_SHARED_WEAK_PTR            0 // shared_ptr, weak_ptr Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_MAKE_SHARED                0 // make_ / allocate_shared, owner_before Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_UNIQUE_PTR                 0 // unique_ptr Yes Yes Yes Yes Boost.Interprocess
#       define M_CXX_FEATURE_FIXED_SIZE_ARRAY           0 // Fixed-Size Arrays Yes Yes Yes Yes Boost.Arrays 
#       define M_CXX_FEATURE_HASH_TABLE                 0 // Hash Tables Yes Yes Yes Yes Boost.Unordered 
#       define M_CXX_FEATURE_SINGLY_LINKED_LIST         0 // Singly-linked list Yes Yes Yes Yes Boost.Container 
#       define M_CXX_FEATURE_REGEXP                     0 // Regular Expressions Yes Yes Yes Yes Boost.Regex 
#       define M_CXX_FEATURE_TUPLE                      0 // Tuple Yes Yes Yes Yes Boost.Tuple 
#       define M_CXX_FEATURE_RAND_NUMBER_AND_DIST       0 // Random Numbers & Distributions Yes Yes Yes Yes Boost.Random 
#       define M_CXX_FEATURE_TYPE_TRAITS                0 // Type Traits Yes Yes Yes Yes Boost.TypeTraits 
#       define M_CXX_FEATURE_GENERALIZED_FUNC_PTR       0 // Generalized Function Pointers Yes Yes Yes Yes Boost.Function 
#       define M_CXX_FEATURE_GENERALIZED_BINDERS        0 // Generalized Binders Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_GENERALIZED_MEMBER_FUNC    0 // Generalized Member Function Adapters Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_REF_WRAPPER                0 // Reference Wrappers Yes Yes Yes Yes Boost.Ref 
#       define M_CXX_FEATURE_RETURN_TYPE_CALC           0 // Return type calculation Yes Yes Yes Yes Boost.ReturnType 
#       define M_CXX_FEATURE_NEW_ALGORITHM              0 // New algorithms Yes Yes Yes Yes Boost.Algorithm 
#       define M_CXX_FEATURE_TIME                       0 // Time No No Yes Yes Boost.Chrono 

#   endif

#else
#	if ( __GNUC__  >= 4 )

#		if (__GNUC_MINOR__ == 7)
#       define M_CXX_FEATURE_RVALUE_REF                 1 //Rvalue references
#       define M_CXX_FEATURE_RVALUE_REF_THIS            0 //Rvalue references for *this
#       define M_CXX_FEATURE_NON_STC_DATA_MEMBER_INIT   0 //Non-static data member initializers
#       define M_CXX_FEATURE_VARIADIC_TMPL              0 //Variadic templates
#       define M_CXX_FEATURE_INITIALIZER_LIST           1 //Initializer lists
#       define M_CXX_FEATURE_STC_ASSERT                 1 //static_assert 
#       define M_CXX_FEATURE_AUTO                       1 //auto keyword
#       define M_CXX_FEATURE_TRAILING_RETURN_TYPES      1 //Trailing return types
#       define M_CXX_FEATURE_LAMBDAS                    1 //Lambdas
#       define M_CXX_FEATURE_DECLTYPE                   1 //decltype
#       define M_CXX_FEATURE_RIGHT_ANGLE_BRACKETS       1 //Right angle brackets TR1
#       define M_CXX_FEATURE_DFL_TMPL_ARG_FOR_FUNC_TMPL 0 //Default template arguments for function templates
#       define M_CXX_FEATURE_EXPRESSION_SFINAE          0 //
#       define M_CXX_FEATURE_ALIAS_TMPL                 0 //Alias templates 
#       define M_CXX_FEATURE_EXTERN_TMPL                1 //Extern templates
#       define M_CXX_FEATURE_NULLPTR                    1 //nullptr
#       define M_CXX_FEATURE_STRONG_TYPE_ENUM           1 //Strongly typed enums
#       define M_CXX_FEATURE_FORWARD_DECLARED_ENUM      1 //Forward declared enums
#       define M_CXX_FEATURE_ATTRIBUTES                 0 //Attributes
#       define M_CXX_FEATURE_CONSTEXPR                  0 //constexpr 
#       define M_CXX_FEATURE_ALIGNMENT                  0 //Alignment
#       define M_CXX_FEATURE_DELEGATING_CONSTRUCTOR     0 //Delegating constructors 
#       define M_CXX_FEATURE_INHERITING_CONSTRUCTOR     0 //Inheriting constructors 
#       define M_CXX_FEATURE_EXPLICIT_CONV_OPERATOR     0 //Explicit conversion operators
#       define M_CXX_FEATURE_CHAR_16_32_T               0 //char16_t and char32_t
#       define M_CXX_FEATURE_UNICODE_STRING_LITERAL     0 //Unicode string literals
#       define M_CXX_FEATURE_RAW_STRING_LITERA          0 //Raw string literals
#       define M_CXX_FEATURE_UNI_CHAR_NAME_IN_LITERAL   0 //Universal character names in literals
#       define M_CXX_FEATURE_USER_DEFINED_LITERAL       0 //User - defined literals 
#       define M_CXX_FEATURE_STD_LAYOUT_AND_TRIVAL_TYPE 1 //Standard - layout and trivial types
#       define M_CXX_FEATURE_DFL_AND_DELETED_FUNC       0 //Defaulted and deleted functions 
#       define M_CXX_FEATURE_EXTENDED_FRIEND_DECL       1 //Extended friend declarations
#       define M_CXX_FEATURE_EXTENDED_SIZEOF            0 //Extended sizeof
#       define M_CXX_FEATURE_INLINE_NAMESPACE           0 //Inline namespaces
#       define M_CXX_FEATURE_UNRESTRICTED_UNIONS        0 //Unrestricted unions 
#       define M_CXX_FEATURE_LOCAL_UNNAM_TYPE_TMPL_ARG  1 //Local and unnamed types as template arguments
#       define M_CXX_FEATURE_RANGE_BASED_FOR_LOOP       1 //Range - based for - loop
#       define M_CXX_FEATURE_OVERRIDE_AND_FINAL         1 //override and final 
#       define M_CXX_FEATURE_MINIMAL_GC_SUPPORT         1 //Minimal GC support 
#       define M_CXX_FEATURE_NOEXCEPT                   0 //noexcept

#       define M_CXX_FEATURE_THREAD                     1
#       define M_CXX_FEATURE_MUTEX                      1
#       define M_CXX_FEATURE_REWORDED_SEQUENCE_POINT    0 //Reworded sequence points No No No No
#       define M_CXX_FEATURE_ATOMIC                     1 //Atomics No Yes No Yes Boost.Atomic 
#       define M_CXX_FEATURE_STRONG_COMPARE             1 //Strong compare and exchange No Yes No No   
#       define M_CXX_FEATURE_BIDIRECTIONAL              1 //Bidirectional fences No Yes No No   
#       define M_CXX_FEATURE_MEMORY_MODEL               0 //Memory model No No No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING           1 //Data-dependency ordering No Yes No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING_FUNC_ANNO 0 //Data-dependency ordering: function annotation No No No No   
#       define M_CXX_FEATURE_EXCEPTION_PTR              1 //exception_ptr Yes Yes Yes Yes Boost.Exception 
#       define M_CXX_FEATURE_QUICK_EXIT                 0 //quick_exit and at_quick_exit No No No No   
#       define M_CXX_FEATURE_ATOMIC_IN_SIGNAL_HANDLERS  0 //Atomics in signal handlers No No No No   
#       define M_CXX_FEATURE_THREAD_LOCAL_STORAGE       0 //Thread-local storage No No No No Boost.Thread 
#       define M_CXX_FEATURE_MAGIC_STATICS              0 //Magic statics No No No No   

#       define M_CXX_FEATURE_SHARED_WEAK_PTR            1 // shared_ptr, weak_ptr Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_MAKE_SHARED                1 // make_ / allocate_shared, owner_before Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_UNIQUE_PTR                 1 // unique_ptr Yes Yes Yes Yes Boost.Interprocess
#       define M_CXX_FEATURE_FIXED_SIZE_ARRAY           1 // Fixed-Size Arrays Yes Yes Yes Yes Boost.Arrays 
#       define M_CXX_FEATURE_HASH_TABLE                 1 // Hash Tables Yes Yes Yes Yes Boost.Unordered 
#       define M_CXX_FEATURE_SINGLY_LINKED_LIST         1 // Singly-linked list Yes Yes Yes Yes Boost.Container 
#       define M_CXX_FEATURE_REGEXP                     0 // Regular Expressions Yes Yes Yes Yes Boost.Regex 
#       define M_CXX_FEATURE_TUPLE                      1 // Tuple Yes Yes Yes Yes Boost.Tuple 
#       define M_CXX_FEATURE_RAND_NUMBER_AND_DIST       1 // Random Numbers & Distributions Yes Yes Yes Yes Boost.Random 
#       define M_CXX_FEATURE_TYPE_TRAITS                0 // Type Traits Yes Yes Yes Yes Boost.TypeTraits 
#       define M_CXX_FEATURE_GENERALIZED_FUNC_PTR       1 // Generalized Function Pointers Yes Yes Yes Yes Boost.Function 
#       define M_CXX_FEATURE_GENERALIZED_BINDERS        1 // Generalized Binders Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_GENERALIZED_MEMBER_FUNC    1 // Generalized Member Function Adapters Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_REF_WRAPPER                1 // Reference Wrappers Yes Yes Yes Yes Boost.Ref 
#       define M_CXX_FEATURE_RETURN_TYPE_CALC           1 // Return type calculation Yes Yes Yes Yes Boost.ReturnType 
#       define M_CXX_FEATURE_NEW_ALGORITHM              1 // New algorithms Yes Yes Yes Yes Boost.Algorithm 
#       define M_CXX_FEATURE_TIME                       1 //Time No No Yes Yes Boost.Chrono 



#		elif (__GNUC_MINOR__ == 8)

#       define M_CXX_FEATURE_RVALUE_REF                 1 //Rvalue references
#       define M_CXX_FEATURE_RVALUE_REF_THIS            0 //Rvalue references for *this
#       define M_CXX_FEATURE_NON_STC_DATA_MEMBER_INIT   0 //Non-static data member initializers
#       define M_CXX_FEATURE_VARIADIC_TMPL              0 //Variadic templates
#       define M_CXX_FEATURE_INITIALIZER_LIST           1 //Initializer lists
#       define M_CXX_FEATURE_STC_ASSERT                 1 //static_assert 
#       define M_CXX_FEATURE_AUTO                       1 //auto keyword
#       define M_CXX_FEATURE_TRAILING_RETURN_TYPES      1 //Trailing return types
#       define M_CXX_FEATURE_LAMBDAS                    1 //Lambdas
#       define M_CXX_FEATURE_DECLTYPE                   1 //decltype
#       define M_CXX_FEATURE_RIGHT_ANGLE_BRACKETS       1 //Right angle brackets TR1
#       define M_CXX_FEATURE_DFL_TMPL_ARG_FOR_FUNC_TMPL 0 //Default template arguments for function templates
#       define M_CXX_FEATURE_EXPRESSION_SFINAE          0 //
#       define M_CXX_FEATURE_ALIAS_TMPL                 0 //Alias templates 
#       define M_CXX_FEATURE_EXTERN_TMPL                1 //Extern templates
#       define M_CXX_FEATURE_NULLPTR                    1 //nullptr
#       define M_CXX_FEATURE_STRONG_TYPE_ENUM           1 //Strongly typed enums
#       define M_CXX_FEATURE_FORWARD_DECLARED_ENUM      1 //Forward declared enums
#       define M_CXX_FEATURE_ATTRIBUTES                 0 //Attributes
#       define M_CXX_FEATURE_CONSTEXPR                  0 //constexpr 
#       define M_CXX_FEATURE_ALIGNMENT                  0 //Alignment
#       define M_CXX_FEATURE_DELEGATING_CONSTRUCTOR     0 //Delegating constructors 
#       define M_CXX_FEATURE_INHERITING_CONSTRUCTOR     0 //Inheriting constructors 
#       define M_CXX_FEATURE_EXPLICIT_CONV_OPERATOR     0 //Explicit conversion operators
#       define M_CXX_FEATURE_CHAR_16_32_T               0 //char16_t and char32_t
#       define M_CXX_FEATURE_UNICODE_STRING_LITERAL     0 //Unicode string literals
#       define M_CXX_FEATURE_RAW_STRING_LITERA          0 //Raw string literals
#       define M_CXX_FEATURE_UNI_CHAR_NAME_IN_LITERAL   0 //Universal character names in literals
#       define M_CXX_FEATURE_USER_DEFINED_LITERAL       0 //User - defined literals 
#       define M_CXX_FEATURE_STD_LAYOUT_AND_TRIVAL_TYPE 1 //Standard - layout and trivial types
#       define M_CXX_FEATURE_DFL_AND_DELETED_FUNC       0 //Defaulted and deleted functions 
#       define M_CXX_FEATURE_EXTENDED_FRIEND_DECL       1 //Extended friend declarations
#       define M_CXX_FEATURE_EXTENDED_SIZEOF            0 //Extended sizeof
#       define M_CXX_FEATURE_INLINE_NAMESPACE           0 //Inline namespaces
#       define M_CXX_FEATURE_UNRESTRICTED_UNIONS        0 //Unrestricted unions 
#       define M_CXX_FEATURE_LOCAL_UNNAM_TYPE_TMPL_ARG  1 //Local and unnamed types as template arguments
#       define M_CXX_FEATURE_RANGE_BASED_FOR_LOOP       1 //Range - based for - loop
#       define M_CXX_FEATURE_OVERRIDE_AND_FINAL         1 //override and final 
#       define M_CXX_FEATURE_MINIMAL_GC_SUPPORT         1 //Minimal GC support 
#       define M_CXX_FEATURE_NOEXCEPT                   0 //noexcept

#       define M_CXX_FEATURE_THREAD                     1
#       define M_CXX_FEATURE_MUTEX                      1
#       define M_CXX_FEATURE_REWORDED_SEQUENCE_POINT    0 //Reworded sequence points No No No No
#       define M_CXX_FEATURE_ATOMIC                     1 //Atomics No Yes No Yes Boost.Atomic 
#       define M_CXX_FEATURE_STRONG_COMPARE             1 //Strong compare and exchange No Yes No No   
#       define M_CXX_FEATURE_BIDIRECTIONAL              1 //Bidirectional fences No Yes No No   
#       define M_CXX_FEATURE_MEMORY_MODEL               0 //Memory model No No No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING           1 //Data-dependency ordering No Yes No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING_FUNC_ANNO 0 //Data-dependency ordering: function annotation No No No No   
#       define M_CXX_FEATURE_EXCEPTION_PTR              1 //exception_ptr Yes Yes Yes Yes Boost.Exception 
#       define M_CXX_FEATURE_QUICK_EXIT                 0 //quick_exit and at_quick_exit No No No No   
#       define M_CXX_FEATURE_ATOMIC_IN_SIGNAL_HANDLERS  0 //Atomics in signal handlers No No No No   
#       define M_CXX_FEATURE_THREAD_LOCAL_STORAGE       0 //Thread-local storage No No No No Boost.Thread 
#       define M_CXX_FEATURE_MAGIC_STATICS              0 //Magic statics No No No No   

#       define M_CXX_FEATURE_SHARED_WEAK_PTR            1 // shared_ptr, weak_ptr Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_MAKE_SHARED                1 // make_ / allocate_shared, owner_before Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_UNIQUE_PTR                 1 // unique_ptr Yes Yes Yes Yes Boost.Interprocess
#       define M_CXX_FEATURE_FIXED_SIZE_ARRAY           1 // Fixed-Size Arrays Yes Yes Yes Yes Boost.Arrays 
#       define M_CXX_FEATURE_HASH_TABLE                 1 // Hash Tables Yes Yes Yes Yes Boost.Unordered 
#       define M_CXX_FEATURE_SINGLY_LINKED_LIST         1 // Singly-linked list Yes Yes Yes Yes Boost.Container 
#       define M_CXX_FEATURE_REGEXP                     0 // Regular Expressions Yes Yes Yes Yes Boost.Regex 
#       define M_CXX_FEATURE_TUPLE                      1 // Tuple Yes Yes Yes Yes Boost.Tuple 
#       define M_CXX_FEATURE_RAND_NUMBER_AND_DIST       1 // Random Numbers & Distributions Yes Yes Yes Yes Boost.Random 
#       define M_CXX_FEATURE_TYPE_TRAITS                0 // Type Traits Yes Yes Yes Yes Boost.TypeTraits 
#       define M_CXX_FEATURE_GENERALIZED_FUNC_PTR       1 // Generalized Function Pointers Yes Yes Yes Yes Boost.Function 
#       define M_CXX_FEATURE_GENERALIZED_BINDERS        1 // Generalized Binders Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_GENERALIZED_MEMBER_FUNC    1 // Generalized Member Function Adapters Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_REF_WRAPPER                1 // Reference Wrappers Yes Yes Yes Yes Boost.Ref 
#       define M_CXX_FEATURE_RETURN_TYPE_CALC           1 // Return type calculation Yes Yes Yes Yes Boost.ReturnType 
#       define M_CXX_FEATURE_NEW_ALGORITHM              1 // New algorithms Yes Yes Yes Yes Boost.Algorithm 
#       define M_CXX_FEATURE_TIME                       1 //Time No No Yes Yes Boost.Chrono 

#		endif
#	else
#       define M_CXX_FEATURE_THREAD                     0
#       define M_CXX_FEATURE_MUTEX                      0
#       define M_CXX_FEATURE_RVALUE_REF                 0 //Rvalue references
#       define M_CXX_FEATURE_RVALUE_REF_THIS            0 //Rvalue references for *this
#       define M_CXX_FEATURE_NON_STC_DATA_MEMBER_INIT   0 //Non-static data member initializers
#       define M_CXX_FEATURE_VARIADIC_TMPL              0 //Variadic templates
#       define M_CXX_FEATURE_INITIALIZER_LIST           0 //Initializer lists
#       define M_CXX_FEATURE_STC_ASSERT                 0 //static_assert 
#       define M_CXX_FEATURE_AUTO                       0 //auto keyword
#       define M_CXX_FEATURE_TRAILING_RETURN_TYPES      0 //Trailing return types
#       define M_CXX_FEATURE_LAMBDAS                    0 //Lambdas
#       define M_CXX_FEATURE_DECLTYPE                   0 //decltype
#       define M_CXX_FEATURE_RIGHT_ANGLE_BRACKETS       0 //Right angle brackets TR1
#       define M_CXX_FEATURE_DFL_TMPL_ARG_FOR_FUNC_TMPL 0 //Default template arguments for function templates
#       define M_CXX_FEATURE_EXPRESSION_SFINAE          0 //
#       define M_CXX_FEATURE_ALIAS_TMPL                 0 //Alias templates 
#       define M_CXX_FEATURE_EXTERN_TMPL                0 //Extern templates
#       define M_CXX_FEATURE_NULLPTR                    0 //nullptr
#       define M_CXX_FEATURE_STRONG_TYPE_ENUM           0 //Strongly typed enums
#       define M_CXX_FEATURE_FORWARD_DECLARED_ENUM      0 //Forward declared enums
#       define M_CXX_FEATURE_ATTRIBUTES                 0 //Attributes
#       define M_CXX_FEATURE_CONSTEXPR                  0 //constexpr 
#       define M_CXX_FEATURE_ALIGNMENT                  0 //Alignment
#       define M_CXX_FEATURE_DELEGATING_CONSTRUCTOR     0 //Delegating constructors 
#       define M_CXX_FEATURE_INHERITING_CONSTRUCTOR     0 //Inheriting constructors 
#       define M_CXX_FEATURE_EXPLICIT_CONV_OPERATOR     0 //Explicit conversion operators
#       define M_CXX_FEATURE_CHAR_16_32_T               0 //char16_t and char32_t
#       define M_CXX_FEATURE_UNICODE_STRING_LITERAL     0 //Unicode string literals
#       define M_CXX_FEATURE_RAW_STRING_LITERA          0 //Raw string literals
#       define M_CXX_FEATURE_UNI_CHAR_NAME_IN_LITERAL   0 //Universal character names in literals
#       define M_CXX_FEATURE_USER_DEFINED_LITERAL       0 //User - defined literals 
#       define M_CXX_FEATURE_STD_LAYOUT_AND_TRIVAL_TYPE 0 //Standard - layout and trivial types
#       define M_CXX_FEATURE_DFL_AND_DELETED_FUNC       0 //Defaulted and deleted functions 
#       define M_CXX_FEATURE_EXTENDED_FRIEND_DECL       0 //Extended friend declarations
#       define M_CXX_FEATURE_EXTENDED_SIZEOF            0 //Extended sizeof
#       define M_CXX_FEATURE_INLINE_NAMESPACE           0 //Inline namespaces
#       define M_CXX_FEATURE_UNRESTRICTED_UNIONS        0 //Unrestricted unions 
#       define M_CXX_FEATURE_LOCAL_UNNAM_TYPE_TMPL_ARG  0 //Local and unnamed types as template arguments
#       define M_CXX_FEATURE_RANGE_BASED_FOR_LOOP       0 //Range - based for - loop
#       define M_CXX_FEATURE_OVERRIDE_AND_FINAL         0 //override and final 
#       define M_CXX_FEATURE_MINIMAL_GC_SUPPORT         0 //Minimal GC support 
#       define M_CXX_FEATURE_NOEXCEPT                   0 //noexcept    1

#       define M_CXX_FEATURE_REWORDED_SEQUENCE_POINT    0 //Reworded sequence points No No No No
#       define M_CXX_FEATURE_ATOMIC                     0 //Atomics No Yes No Yes Boost.Atomic 
#       define M_CXX_FEATURE_STRONG_COMPARE             0 //Strong compare and exchange No Yes No No   
#       define M_CXX_FEATURE_BIDIRECTIONAL              0 //Bidirectional fences No Yes No No   
#       define M_CXX_FEATURE_MEMORY_MODEL               0  //Memory model No No No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING           0  //Data-dependency ordering No Yes No No   
#       define M_CXX_FEATURE_DAT_DEP_ORDERING_FUNC_ANNO 0  //Data-dependency ordering: function annotation No No No No   
#       define M_CXX_FEATURE_EXCEPTION_PTR              0  //exception_ptr Yes Yes Yes Yes Boost.Exception 
#       define M_CXX_FEATURE_QUICK_EXIT                 0  //quick_exit and at_quick_exit No No No No   
#       define M_CXX_FEATURE_ATOMIC_IN_SIGNAL_HANDLERS  0  //Atomics in signal handlers No No No No   
#       define M_CXX_FEATURE_THREAD_LOCAL_STORAGE       0  //Thread-local storage No No No No Boost.Thread 
#       define M_CXX_FEATURE_MAGIC_STATICS              0  //Magic statics No No No No  

#       define M_CXX_FEATURE_SHARED_WEAK_PTR            0 // shared_ptr, weak_ptr Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_MAKE_SHARED                0 // make_ / allocate_shared, owner_before Yes Yes Yes Yes Boost.SmartPtr
#       define M_CXX_FEATURE_UNIQUE_PTR                 0 // unique_ptr Yes Yes Yes Yes Boost.Interprocess
#       define M_CXX_FEATURE_FIXED_SIZE_ARRAY           0 // Fixed-Size Arrays Yes Yes Yes Yes Boost.Arrays 
#       define M_CXX_FEATURE_HASH_TABLE                 0 // Hash Tables Yes Yes Yes Yes Boost.Unordered 
#       define M_CXX_FEATURE_SINGLY_LINKED_LIST         0 // Singly-linked list Yes Yes Yes Yes Boost.Container 
#       define M_CXX_FEATURE_REGEXP                     0 // Regular Expressions Yes Yes Yes Yes Boost.Regex 
#       define M_CXX_FEATURE_TUPLE                      0 // Tuple Yes Yes Yes Yes Boost.Tuple 
#       define M_CXX_FEATURE_RAND_NUMBER_AND_DIST       0 // Random Numbers & Distributions Yes Yes Yes Yes Boost.Random 
#       define M_CXX_FEATURE_TYPE_TRAITS                0 // Type Traits Yes Yes Yes Yes Boost.TypeTraits 
#       define M_CXX_FEATURE_GENERALIZED_FUNC_PTR       0 // Generalized Function Pointers Yes Yes Yes Yes Boost.Function 
#       define M_CXX_FEATURE_GENERALIZED_BINDERS        0 // Generalized Binders Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_GENERALIZED_MEMBER_FUNC    0 // Generalized Member Function Adapters Yes Yes Yes Yes Boost.Bind 
#       define M_CXX_FEATURE_REF_WRAPPER                0 // Reference Wrappers Yes Yes Yes Yes Boost.Ref 
#       define M_CXX_FEATURE_RETURN_TYPE_CALC           0 // Return type calculation Yes Yes Yes Yes Boost.ReturnType 
#       define M_CXX_FEATURE_NEW_ALGORITHM              0 // New algorithms Yes Yes Yes Yes Boost.Algorithm 
#       define M_CXX_FEATURE_TIME                       0 // Time No No Yes Yes Boost.Chrono 


#	endif

#endif

#if (M_CXX_FEATURE_NOEXCEPT == 0)
#   define M_NOEXCEPT
#else
#   define M_NOEXCEPT     noexcept
#endif


//overrive
#       undef M_CXX_FEATURE_MUTEX
#       define M_CXX_FEATURE_MUTEX                      0
