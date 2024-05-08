cmake_minimum_required(VERSION 3.20)

IF(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PILO_ARCH x86_64)    
ELSE(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(PILO_ARCH i386)
ENDIF(CMAKE_SIZEOF_VOID_P EQUAL 8)

message("${CMAKE_SIZEOF_VOID_P}")
message("Current Arch is ${PILO_ARCH}")


if(CMAKE_HOST_SYSTEM_NAME MATCHES "Linux") 
	message("Current OS Type Name is Linux")
	add_definitions(-DLINUX)	
else()
	message(FATAL_ERROR, "Not Support OS: ${CMAKE_HOST_SYSTEM_NAME} ")
endif()

project(libpilo)
SET(CMAKE_CXX_COMPILER /usr/bin/g++)
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

#message("cpp source -> ${PILO_LIBCPPS}")
#message("hpp source -> ${PILO_LIBHPPS}")
#message("h source -> ${PILO_LIBHS}")

add_library(pilo_static_debug STATIC ${PILO_LIBCPPS} ${PILO_LIBHPPS} ${PILO_LIBHS})
SET_TARGET_PROPERTIES(pilo_static_debug PROPERTIES OUTPUT_NAME "pilo-static-debug")
add_library(pilo_static_release STATIC ${PILO_LIBCPPS} ${PILO_LIBHPPS} ${PILO_LIBHS})
SET_TARGET_PROPERTIES(pilo_static_release PROPERTIES OUTPUT_NAME "pilo-static-release")
add_library(pilo_shared_debug SHARED ${PILO_LIBCPPS} ${PILO_LIBHPPS} ${PILO_LIBHS})
SET_TARGET_PROPERTIES(pilo_shared_debug PROPERTIES OUTPUT_NAME "pilo-shared-debug")
add_library(pilo_shared_release SHARED ${PILO_LIBCPPS} ${PILO_LIBHPPS} ${PILO_LIBHS})
SET_TARGET_PROPERTIES(pilo_shared_release PROPERTIES OUTPUT_NAME "pilo-shared-release")

target_include_directories(pilo_static_debug PUBLIC "${PILO_LIBSRC_DIR}/external/mysqlclient/8.0.19/include" "${PILO_LIBSRC_DIR}")
target_include_directories(pilo_static_release PUBLIC "${PILO_LIBSRC_DIR}/external/mysqlclient/8.0.19/include" "${PILO_LIBSRC_DIR}")
target_include_directories(pilo_shared_debug PUBLIC "${PILO_LIBSRC_DIR}/external/mysqlclient/8.0.19/include" "${PILO_LIBSRC_DIR}")
target_include_directories(pilo_shared_release PUBLIC "${PILO_LIBSRC_DIR}/external/mysqlclient/8.0.19/include" "${PILO_LIBSRC_DIR}")

target_compile_options(pilo_static_debug PUBLIC -g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11)
target_compile_options(pilo_static_release PUBLIC -O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11)
target_compile_options(pilo_shared_debug PUBLIC -g -fPIC -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11)
target_compile_options(pilo_shared_release PUBLIC -O2 -fno-aggressive-loop-optimizations -fPIC -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11)

set(PILO_LIBPILO_INCLUDE_DIR ${PILO_OUTPUT_DIR}/include)
set(PILO_LIBPILO_LIB_DIR ${PILO_OUTPUT_DIR}/lib)


add_custom_command(TARGET pilo_static_debug POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pilo_static_debug> ${LIBRARY_OUTPUT_PATH}
        COMMAND mkdir -p ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND cp -R ${PILO_LIBSRC_DIR}/* ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND find ${PILO_OUTPUT_DIR}/include/pilo -type f -name "*.cpp" -delete
)
add_custom_command(TARGET pilo_static_release POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pilo_static_release> ${LIBRARY_OUTPUT_PATH}
        COMMAND mkdir -p ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND cp -R ${PILO_LIBSRC_DIR}/* ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND find ${PILO_OUTPUT_DIR}/include/pilo -type f -name "*.cpp" -delete
)
add_custom_command(TARGET pilo_shared_debug POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pilo_shared_debug> ${LIBRARY_OUTPUT_PATH}
        COMMAND mkdir -p ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND cp -R ${PILO_LIBSRC_DIR}/* ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND find ${PILO_OUTPUT_DIR}/include/pilo -type f -name "*.cpp" -delete
)
add_custom_command(TARGET pilo_shared_release POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pilo_shared_release> ${LIBRARY_OUTPUT_PATH}
        COMMAND mkdir -p ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND cp -R ${PILO_LIBSRC_DIR}/* ${PILO_OUTPUT_DIR}/include/pilo
        COMMAND find ${PILO_OUTPUT_DIR}/include/pilo -type f -name "*.cpp" -delete
)

################################################################### Functional Test ###########################################################################
# for function-test
set(PILO_FUNCTEST_DIR "${PILO_PROJECT_DIR}/src/pilo_func_test")
file(GLOB_RECURSE PILOFUNC_TEST_CPPS ${PILO_FUNCTEST_DIR}/*.cpp)
file(GLOB_RECURSE PILOFUNC_TEST_PPS ${PILO_FUNCTEST_DIR}/*.hpp)
file(GLOB_RECURSE PILOFUNC_TEST_HS ${PILO_FUNCTEST_DIR}/*.h)

##pilo_func_test-static-debug 
add_executable(pilo_func_test-static-debug ${PILOFUNC_TEST_CPPS} ${PILOFUNC_TEST_PPS} ${PILOFUNC_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_func_test-static-debug PUBLIC ${PILO_PROJECT_DIR}/src)
set_target_properties(pilo_func_test-static-debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_func_test-static-debug PUBLIC -g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11)
target_link_directories(pilo_func_test-static-debug PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_func_test-static-debug PUBLIC "pthread" "dl" "libpilo-static-debug.a")

##pilo_func_test-static-release
add_executable(pilo_func_test-static-release ${PILOFUNC_TEST_CPPS} ${PILOFUNC_TEST_PPS} ${PILOFUNC_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_func_test-static-release PUBLIC ${PILO_PROJECT_DIR}/src)
set_target_properties(pilo_func_test-static-release PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_func_test-static-release PUBLIC -O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11)
target_link_directories(pilo_func_test-static-release PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_func_test-static-release PUBLIC "pthread" "dl" "libpilo-static-release.a")

##pilo_func_test-shared-debug
add_executable(pilo_func_test-shared-debug ${PILOFUNC_TEST_CPPS} ${PILOFUNC_TEST_PPS} ${PILOFUNC_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_func_test-shared-debug PUBLIC ${PILO_PROJECT_DIR}/src)
set_target_properties(pilo_func_test-shared-debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_func_test-shared-debug PUBLIC -g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11)
target_link_directories(pilo_func_test-shared-debug PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_func_test-shared-debug PUBLIC "pthread" "dl" "libpilo-shared-debug.so")

##pilo_func_test-shared-release
add_executable(pilo_func_test-shared-release ${PILOFUNC_TEST_CPPS} ${PILOFUNC_TEST_PPS} ${PILOFUNC_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_func_test-shared-release PUBLIC ${PILO_PROJECT_DIR}/src)
set_target_properties(pilo_func_test-shared-release PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_func_test-shared-release PUBLIC -O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11)
target_link_directories(pilo_func_test-shared-release PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_func_test-shared-release PUBLIC "pthread" "dl" "libpilo-shared-release.so")



##################################################################### Stable Test #############################################################################
# for stable-test
set(PILO_STABLETEST_DIR "${PILO_PROJECT_DIR}/src/pilo_stable_test")
file(GLOB_RECURSE PILOSTABLE_TEST_CPPS ${PILO_STABLETEST_DIR}/*.cpp)
file(GLOB_RECURSE PILOSTABLE_TEST_PPS ${PILO_STABLETEST_DIR}/*.hpp)
file(GLOB_RECURSE PILOSTABLE_TEST_HS ${PILO_STABLETEST_DIR}/*.h)

##pilo_stable_test-static-debug
add_executable(pilo_stable_test-static-debug ${PILOSTABLE_TEST_CPPS} ${PILOSTABLE_TEST_PPS} ${PILOSTABLE_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_stable_test-static-debug PUBLIC "${PILO_PROJECT_DIR}/src")
set_target_properties(pilo_stable_test-static-debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_stable_test-static-debug  PUBLIC -g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11)
target_link_directories(pilo_stable_test-static-debug PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_stable_test-static-debug PUBLIC "pthread" "dl" "libpilo-static-debug.a")

##pilo_stable_test-static-release
add_executable(pilo_stable_test-static-release ${PILOSTABLE_TEST_CPPS} ${PILOSTABLE_TEST_PPS} ${PILOSTABLE_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
set_target_properties(pilo_stable_test-static-release PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_include_directories(pilo_stable_test-static-release PUBLIC "${PILO_PROJECT_DIR}/src")
target_compile_options(pilo_stable_test-static-release PUBLIC -O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11)
target_link_directories(pilo_stable_test-static-release PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_stable_test-static-release PUBLIC "pthread" "dl" "libpilo-static-release.a")

##pilo_stable_test-shared-debug
add_executable(pilo_stable_test-shared-debug ${PILOSTABLE_TEST_CPPS} ${PILOSTABLE_TEST_PPS} ${PILOSTABLE_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
set_target_properties(pilo_stable_test-shared-debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_include_directories(pilo_stable_test-shared-debug PUBLIC "${PILO_PROJECT_DIR}/src")
target_compile_options(pilo_stable_test-shared-debug PUBLIC -g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11)
target_link_directories(pilo_stable_test-shared-debug PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_stable_test-shared-debug PUBLIC "pthread" "dl" "libpilo-shared-debug.so")

##pilo_stable_test-shared-release
add_executable(pilo_stable_test-shared-release ${PILOSTABLE_TEST_CPPS} ${PILOSTABLE_TEST_PPS} ${PILOSTABLE_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_stable_test-shared-release PUBLIC "${PILO_PROJECT_DIR}/src")
set_target_properties(pilo_stable_test-shared-release PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_stable_test-shared-release PUBLIC -O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11)
target_link_directories(pilo_stable_test-shared-release PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_stable_test-shared-release PUBLIC "pthread" "dl" "libpilo-shared-release.so")


##################################################################### Performance Test #############################################################################
# for performance-test
set(PILO_PERFORMANCETEST_DIR "${PILO_PROJECT_DIR}/src/pilo_performance_test")
file(GLOB_RECURSE PILOPERFORMANCE_TEST_CPPS ${PILO_PERFORMANCETEST_DIR}/*.cpp)
file(GLOB_RECURSE PILOPERFORMANCE_TEST_PPS ${PILO_PERFORMANCETEST_DIR}/*.hpp)
file(GLOB_RECURSE PILOPERFORMANCE_TEST_HS ${PILO_PERFORMANCETEST_DIR}/*.h)
## pilo_performance_test-static-debug
add_executable(pilo_performance_test-static-debug ${PILOPERFORMANCE_TEST_CPPS} ${PILOPERFORMANCE_TEST_PPS} ${PILOPERFORMANCE_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_performance_test-static-debug PUBLIC "${PILO_PROJECT_DIR}/src")
set_target_properties(pilo_performance_test-static-debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_performance_test-static-debug  PUBLIC -g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11)
target_link_directories(pilo_performance_test-static-debug PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_performance_test-static-debug PUBLIC "pthread" "dl" "libpilo-static-debug.a")

##pilo_performance_test-static-release
add_executable(pilo_performance_test-static-release ${PILOPERFORMANCE_TEST_CPPS} ${PILOPERFORMANCE_TEST_PPS} ${PILOPERFORMANCE_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_performance_test-static-release PUBLIC "${PILO_PROJECT_DIR}/src")
set_target_properties(pilo_performance_test-static-release PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_performance_test-static-release PUBLIC -O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11)
target_link_directories(pilo_performance_test-static-release PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_performance_test-static-release PUBLIC "pthread" "dl" "libpilo-static-release.a")

##pilo_performance_test-shared-debug
add_executable(pilo_performance_test-shared-debug ${PILOPERFORMANCE_TEST_CPPS} ${PILOPERFORMANCE_TEST_PPS} ${PILOPERFORMANCE_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_performance_test-shared-debug PUBLIC "${PILO_PROJECT_DIR}/src")
set_target_properties(pilo_performance_test-shared-debug PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_performance_test-shared-debug PUBLIC -g -pthread -Wall -Werror -Wno-stringop-truncation -std=c++11)
target_link_directories(pilo_performance_test-shared-debug PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_performance_test-shared-debug PUBLIC "pthread" "dl" "libpilo-shared-debug.so")

##pilo_performance_test-shared-release
add_executable(pilo_performance_test-shared-release ${PILOPERFORMANCE_TEST_CPPS} ${PILOPERFORMANCE_TEST_PPS} ${PILOPERFORMANCE_TEST_HS} ${PILO_LIBHPPS} ${PILO_LIBHS})
target_include_directories(pilo_performance_test-shared-release PUBLIC "${PILO_PROJECT_DIR}/src")
set_target_properties(pilo_performance_test-shared-release PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${PILO_OUTPUT_DIR}/bin)
target_compile_options(pilo_performance_test-shared-release PUBLIC -O2 -fno-aggressive-loop-optimizations -pthread -Wall -Werror -Wno-stringop-truncation  -std=c++11)
target_link_directories(pilo_performance_test-shared-release PUBLIC ${PILO_OUTPUT_LIB_DIR})
target_link_libraries(pilo_performance_test-shared-release PUBLIC "pthread" "dl" "libpilo-shared-release.so")


#use -DCMAKE_INSTALL_PREFIX=? to assgin install path
install(FILES "${PILO_OUTPUT_LIB_DIR}/libpilo-static-debug.a"  DESTINATION "pilo/lib")
install(FILES "${PILO_OUTPUT_LIB_DIR}/libpilo-static-release.a"  DESTINATION "pilo/lib")
install(FILES "${PILO_OUTPUT_LIB_DIR}/libpilo-shared-debug.so"  DESTINATION "pilo/lib")
install(FILES "${PILO_OUTPUT_LIB_DIR}/libpilo-shared-debug.so"  DESTINATION "pilo/lib")
install(DIRECTORY "${PILO_LIBPILO_INCLUDE_DIR}/pilo"  DESTINATION "pilo/include")

