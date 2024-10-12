cmake_minimum_required(VERSION 3.20)



IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PILO_ARCH x86_64)
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PILO_ARCH i386)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux")
    message("Current OS Type Name is Linux")
    add_definitions(-DLINUX)
else()
    message(FATAL_ERROR, "Not Support OS: ${CMAKE_HOST_SYSTEM_NAME} ")
endif()

project(libpilo)
set(CMAKE_DEBUG_POSTFIX "-debug")
set(CMAKE_RELEASE_POSTFIX "-release")
set(CMAKE_CXX_STANDARD 11)

get_filename_component(PILO_PROJECT_DIR "${CMAKE_SOURCE_DIR}/../../" ABSOLUTE NAME)
get_filename_component(PFC_NAME ${CMAKE_SOURCE_DIR} NAME)
get_filename_component(PFC_NAME ${PFC_NAME} NAME)

message("Project Dir is -> <${PILO_PROJECT_DIR}>")
message("CMake Source Dir is -> <${CMAKE_SOURCE_DIR}>")

set(PILO_OUTPUT_DIR ${PILO_PROJECT_DIR}/output/${PFC_NAME}/pilo)
set(PILO_LIBSRC_DIR "${PILO_PROJECT_DIR}/src/pilo")
set(PILO_OUTPUT_LIB_DIR ${PILO_PROJECT_DIR}/output/${PFC_NAME}/pilo/lib)

message("PFC_NAME is -> <${PFC_NAME}>")
message("PILO_PROJECT_DIR is -> <${PILO_PROJECT_DIR}>")
message("PILO_OUTPUT_DIR is -> <${PILO_OUTPUT_DIR}>")

set(LIBRARY_OUTPUT_PATH ${PILO_OUTPUT_DIR}/lib)
message("LIBRARY_OUTPUT_PATH is -> <${LIBRARY_OUTPUT_PATH}>")

file(GLOB_RECURSE PILO_LIBCPPS ${PILO_LIBSRC_DIR}/*.cpp)
file(GLOB_RECURSE PILO_LIBHPPS ${PILO_LIBSRC_DIR}/*.hpp)
file(GLOB_RECURSE PILO_LIBHS ${PILO_LIBSRC_DIR}/*.h)

add_library(pilo_static STATIC ${PILO_LIBCPPS} ${PILO_LIBHPPS} ${PILO_LIBHS})
set_target_properties(pilo_static PROPERTIES OUTPUT_NAME "pilo-static")

add_library(pilo_shared SHARED ${PILO_LIBCPPS} ${PILO_LIBHPPS} ${PILO_LIBHS})
set_target_properties(pilo_shared PROPERTIES OUTPUT_NAME "pilo-shared")

target_include_directories(pilo_static PUBLIC "${PILO_LIBSRC_DIR}/external/mysqlclient/8.0.19/include" "${PILO_LIBSRC_DIR}")
target_include_directories(pilo_shared PUBLIC "${PILO_LIBSRC_DIR}/external/mysqlclient/8.0.19/include" "${PILO_LIBSRC_DIR}")

set_target_properties(pilo_static PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11"
    COMPILE_FLAGS_DEBUG "-g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11"
)

set_target_properties(pilo_shared PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -fPIC -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11"
    COMPILE_FLAGS_DEBUG "-g -fPIC -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11"
)

#target_compile_options(pilo_static PUBLIC -g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11)
#target_compile_options(pilo_shared PUBLIC -O2 -fPIC -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11)

set(PILO_LIBPILO_INCLUDE_DIR ${PILO_OUTPUT_DIR}/include)
set(PILO_LIBPILO_LIB_DIR ${PILO_OUTPUT_DIR}/lib)
add_custom_command(TARGET pilo_static PRE_BUILD
        COMMAND rm -f ${PILO_OUTPUT_DIR}/lib/libpilo-static-debug.a
        COMMAND rm -f ${PILO_OUTPUT_DIR}/lib/libpilo-static-release.a
)
add_custom_command(TARGET pilo_static POST_BUILD
        COMMAND mkdir -p ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND cp -R ${PILO_LIBSRC_DIR}/* ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND find ${PILO_OUTPUT_DIR}/include/pilo -type f -name "*.cpp" -delete
)
add_custom_command(TARGET pilo_static PRE_BUILD
        COMMAND rm -f ${PILO_OUTPUT_DIR}/lib/libpilo-shared-debug.a
        COMMAND rm -f ${PILO_OUTPUT_DIR}/lib/libpilo-shared-release.a
)
add_custom_command(TARGET pilo_shared POST_BUILD
        COMMAND mkdir -p ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND cp -R ${PILO_LIBSRC_DIR}/* ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND find ${PILO_OUTPUT_DIR}/include/pilo -type f -name "*.cpp" -delete
)

# for function-test
set(PILO_FUNCTEST_DIR "${PILO_PROJECT_DIR}/src/pilo_func_test")
file(GLOB_RECURSE PILOFUNC_TEST_CPPS ${PILO_FUNCTEST_DIR}/*.cpp)
file(GLOB_RECURSE PILOFUNC_TEST_PPS ${PILO_FUNCTEST_DIR}/*.hpp)
file(GLOB_RECURSE PILOFUNC_TEST_HS ${PILO_FUNCTEST_DIR}/*.h)


add_executable(pilo_func_test-static ${PILOFUNC_TEST_CPPS} ${PILOFUNC_TEST_PPS} ${PILOFUNC_TEST_HS})
set_target_properties(pilo_func_test-static PROPERTIES OUTPUT_NAME "pilo_func_test-static")
set_target_properties(pilo_func_test-static PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_func_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_func_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
#target_include_directories(pilo_func_test-static PUBLIC "${PILO_OUTPUT_DIR}/include")
target_include_directories(pilo_func_test-static PUBLIC ${PILO_PROJECT_DIR}/src)
target_link_libraries(pilo_func_test-static PUBLIC "pthread" "dl")
target_link_libraries(pilo_func_test-static PUBLIC debug "pilo-static-debug" optimized "pilo-static-release")
target_link_directories(pilo_func_test-static PUBLIC ${PILO_OUTPUT_LIB_DIR})
add_dependencies(pilo_func_test-static pilo_static)
set_target_properties(pilo_func_test-static PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11"
    COMPILE_FLAGS_DEBUG "-g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11"
)
add_custom_command(TARGET pilo_func_test-static PRE_BUILD
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_func_test-static-debug
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_func_test-static-release
)

add_executable(pilo_func_test-shared ${PILOFUNC_TEST_CPPS} ${PILOFUNC_TEST_PPS} ${PILOFUNC_TEST_HS})
set_target_properties(pilo_func_test-shared PROPERTIES OUTPUT_NAME "pilo_func_test-shared")
set_target_properties(pilo_func_test-shared PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_func_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_func_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
target_include_directories(pilo_func_test-shared PUBLIC ${PILO_PROJECT_DIR}/src)
target_link_libraries(pilo_func_test-shared PUBLIC "pthread" "dl")
target_link_libraries(pilo_func_test-shared PUBLIC debug "pilo-shared-debug" optimized "pilo-shared-release")
target_link_directories(pilo_func_test-shared PUBLIC ${PILO_OUTPUT_LIB_DIR})
add_dependencies(pilo_func_test-shared pilo_shared)
set_target_properties(pilo_func_test-shared PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11"
    COMPILE_FLAGS_DEBUG "-g -fPIC -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11"
)
add_custom_command(TARGET pilo_func_test-shared PRE_BUILD
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_func_test-shared-debug
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_func_test-shared-release
)
# for stable-test
set(PILO_STABLE_DIR "${PILO_PROJECT_DIR}/src/pilo_stable_test")
file(GLOB_RECURSE PILOSTABLE_TEST_CPPS ${PILO_STABLE_DIR}/*.cpp)
file(GLOB_RECURSE PILOSTABLE_TEST_PPS ${PILO_STABLE_DIR}/*.hpp)
file(GLOB_RECURSE PILOSTABLE_TEST_HS ${PILO_STABLE_DIR}/*.h)

add_executable(pilo_stable_test-static ${PILOSTABLE_TEST_CPPS} ${PILOSTABLE_TEST_PPS} ${PILOSTABLE_TEST_HS})
set_target_properties(pilo_stable_test-static PROPERTIES OUTPUT_NAME "pilo_stable_test-static")
set_target_properties(pilo_stable_test-static PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_stable_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_stable_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
#target_include_directories(pilo_stable_test-static PUBLIC "${PILO_OUTPUT_DIR}/include")
target_include_directories(pilo_stable_test-static PUBLIC ${PILO_PROJECT_DIR}/src)
target_link_libraries(pilo_stable_test-static PUBLIC "pthread" "dl")
target_link_libraries(pilo_stable_test-static PUBLIC debug "pilo-static-debug" optimized "pilo-static-release")
target_link_directories(pilo_stable_test-static PUBLIC ${PILO_OUTPUT_LIB_DIR})
add_dependencies(pilo_stable_test-static pilo_static)
set_target_properties(pilo_stable_test-static PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11"
    COMPILE_FLAGS_DEBUG "-g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11"
)
add_custom_command(TARGET pilo_stable_test-static PRE_BUILD
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_stable_test-static-debug
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_stable_test-static-release
)

add_executable(pilo_stable_test-shared ${PILOSTABLE_TEST_CPPS} ${PILOSTABLE_TEST_PPS} ${PILOSTABLE_TEST_HS})
set_target_properties(pilo_stable_test-shared PROPERTIES OUTPUT_NAME "pilo_stable_test-shared")
set_target_properties(pilo_stable_test-shared PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_stable_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_stable_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
target_include_directories(pilo_stable_test-shared PUBLIC ${PILO_PROJECT_DIR}/src)
target_link_libraries(pilo_stable_test-shared PUBLIC "pthread" "dl")
target_link_libraries(pilo_stable_test-shared PUBLIC debug "pilo-shared-debug" optimized "pilo-shared-release")
target_link_directories(pilo_stable_test-shared PUBLIC ${PILO_OUTPUT_LIB_DIR})
add_dependencies(pilo_stable_test-shared pilo_shared)
set_target_properties(pilo_stable_test-shared PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11"
    COMPILE_FLAGS_DEBUG "-g -fPIC -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11"
)
add_custom_command(TARGET pilo_stable_test-shared PRE_BUILD
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_stable_test-shared-debug
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_stable_test-shared-release
)

# for performance-test
set(PILO_PERFORMANCE_DIR "${PILO_PROJECT_DIR}/src/pilo_performance_test")
file(GLOB_RECURSE PILOPERFORMANCE_TEST_CPPS ${PILO_PERFORMANCE_DIR}/*.cpp)
file(GLOB_RECURSE PILOPERFORMANCE_TEST_PPS ${PILO_PERFORMANCE_DIR}/*.hpp)
file(GLOB_RECURSE PILOPERFORMANCE_TEST_HS ${PILO_PERFORMANCE_DIR}/*.h)

add_executable(pilo_performance_test-static ${PILOPERFORMANCE_TEST_CPPS} ${PILOPERFORMANCE_TEST_PPS} ${PILOPERFORMANCE_TEST_HS})
set_target_properties(pilo_performance_test-static PROPERTIES OUTPUT_NAME "pilo_performance_test-static")
set_target_properties(pilo_performance_test-static PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_performance_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_performance_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
#target_include_directories(pilo_performance_test-static PUBLIC "${PILO_OUTPUT_DIR}/include")
target_include_directories(pilo_performance_test-static PUBLIC ${PILO_PROJECT_DIR}/src)
target_link_libraries(pilo_performance_test-static PUBLIC "pthread" "dl")
target_link_libraries(pilo_performance_test-static PUBLIC debug "pilo-static-debug" optimized "pilo-static-release")
target_link_directories(pilo_performance_test-static PUBLIC ${PILO_OUTPUT_LIB_DIR})
add_dependencies(pilo_performance_test-static pilo_static)
set_target_properties(pilo_performance_test-static PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11"
    COMPILE_FLAGS_DEBUG "-g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11"
)
add_custom_command(TARGET pilo_performance_test-static PRE_BUILD
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_performance_test-static-debug
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_performance_test-static-release
)



add_executable(pilo_performance_test-shared ${PILOPERFORMANCE_TEST_CPPS} ${PILOPERFORMANCE_TEST_PPS} ${PILOPERFORMANCE_TEST_HS})
set_target_properties(pilo_performance_test-shared PROPERTIES OUTPUT_NAME "pilo_performance_test-shared")
set_target_properties(pilo_performance_test-shared PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_performance_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_performance_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
target_include_directories(pilo_performance_test-shared PUBLIC ${PILO_PROJECT_DIR}/src)
target_link_libraries(pilo_performance_test-shared PUBLIC "pthread" "dl")
target_link_libraries(pilo_performance_test-shared PUBLIC debug "pilo-shared-debug" optimized "pilo-shared-release")
target_link_directories(pilo_performance_test-shared PUBLIC ${PILO_OUTPUT_LIB_DIR})
add_dependencies(pilo_performance_test-shared pilo_shared)
set_target_properties(pilo_performance_test-shared PROPERTIES
    COMPILE_FLAGS_RELEASE "-O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11"
    COMPILE_FLAGS_DEBUG "-g -fPIC -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11"
)
add_custom_command(TARGET pilo_performance_test-shared PRE_BUILD
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_performance_test-shared-debug
        COMMAND rm -f ${PILO_OUTPUT_DIR}/bin/pilo_performance_test-shared-release
)
