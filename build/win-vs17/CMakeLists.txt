cmake_minimum_required(VERSION 3.20)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_SUPPORT_WINDOWS_EXPORT_ALL_SYMBOLS 1 )

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
	
	
elseif(CMAKE_HOST_SYSTEM_NAME MATCHES "Windows") 
	message("Current OS Type Name is Windows")
	add_definitions(-DWINDOWS)
	SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} /WX /W4 /Zi /Od")
	SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} /WX /W4 /Zi /O2")
	
else() 
	message(FATAL_ERROR, "Not Support OS : ${CMAKE}")
endif()
 

project(libpilo)

set(CMAKE_DEBUG_POSTFIX "-debug")
set(CMAKE_RELEASE_POSTFIX "-release")
set(CMAKE_CXX_STANDARD 11)

get_filename_component(PILO_PROJECT_DIR "${CMAKE_SOURCE_DIR}/../../" ABSOLUTE NAME)
get_filename_component(PFC_NAME ${CMAKE_SOURCE_DIR} NAME)
get_filename_component(PFC_NAME ${PFC_NAME} NAME)

set(PILO_OUTPUT_DIR ${PILO_PROJECT_DIR}/output/${PFC_NAME}/pilo)
set(PILO_LIBSRC_DIR "${PILO_PROJECT_DIR}/src/pilo")
set(PILO_OUTPUT_LIB_DIR ${PILO_PROJECT_DIR}/output/${PFC_NAME}/pilo/lib)

message("PFC_NAME is -> <${PFC_NAME}>")
message("PILO_PROJECT_DIR is -> <${PILO_PROJECT_DIR}>")
message("PILO_OUTPUT_DIR is -> <${PILO_OUTPUT_DIR}>")

set(LIBRARY_OUTPUT_PATH ${PILO_OUTPUT_DIR}/lib)
message("LIBRARY_OUTPUT_PATH is -> <${LIBRARY_OUTPUT_PATH}>")

function(get_current_directory_depth depth_var)
  set(current_path "${CMAKE_CURRENT_SOURCE_DIR}")
  set(depth 0)
  
  while(NOT "${current_path}" STREQUAL "/")
    get_filename_component(parent_path "${current_path}" PATH)
    if(NOT "${parent_path}" STREQUAL "${current_path}")
      math(EXPR depth "${depth} + 1")
      set(current_path "${parent_path}")
    else()
      # Reached the root directory or a loop occurred.
      break()
    endif()
  endwhile()
  
  set(${depth_var} ${depth} PARENT_SCOPE)
endfunction()

get_current_directory_depth(dir_depth)

macro(source_group_by_dir source_files)
    if(MSVC)
        	set(sgbd_cur_dir ${CMAKE_CURRENT_SOURCE_DIR})
        	foreach(sgbd_file ${${source_files}})  
          string(REGEX REPLACE ${sgbd_cur_dir}/\(.*\) \\1 sgbd_fpath ${sgbd_file})

          string(REGEX REPLACE "\(.*\)/.*" \\1 sgbd_group_name ${sgbd_fpath})
          string(COMPARE EQUAL ${sgbd_fpath} ${sgbd_group_name} sgbd_nogroup)

          if(sgbd_nogroup)
          set(sgbd_group_name "/")
          endif(sgbd_nogroup)            

          string(REPLACE "/" "/;" fplist "${sgbd_group_name}")
            
          list(LENGTH fplist len)

          list(SUBLIST fplist ${dir_depth} -1 sgbd_file_list)
					string(REPLACE ";" "<->"  grppathname ${sgbd_file_list})
          source_group(${grppathname} FILES ${sgbd_file})   
        endforeach(sgbd_file)
    endif(MSVC)
endmacro(source_group_by_dir)


file(GLOB_RECURSE PILO_LIBCPPS ${PILO_LIBSRC_DIR}/*.cpp)
file(GLOB_RECURSE PILO_LIBHPPS ${PILO_LIBSRC_DIR}/*.hpp)
file(GLOB_RECURSE PILO_LIBHS ${PILO_LIBSRC_DIR}/*.h)



#message("cpp source -> ${PILO_LIBCPPS}")
#message("hpp source -> ${PILO_LIBHPPS}")
#message("h source -> ${PILO_LIBHS}")

source_group_by_dir(PILO_LIBCPPS)
source_group_by_dir(PILO_LIBHPPS)
source_group_by_dir(PILO_LIBHS)


add_library(pilo_static STATIC ${PILO_LIBCPPS} ${PILO_LIBHPPS} ${PILO_LIBHS})
SET_TARGET_PROPERTIES(pilo_static PROPERTIES OUTPUT_NAME "libpilo-static")

add_library(pilo_shared SHARED ${PILO_LIBCPPS} ${PILO_LIBHPPS} ${PILO_LIBHS})
SET_TARGET_PROPERTIES(pilo_shared PROPERTIES OUTPUT_NAME "libpilo-shared")

target_include_directories(pilo_static PUBLIC "${PILO_LIBSRC_DIR}/external/mysqlclient/8.0.19/include" "${PILO_LIBSRC_DIR}")
target_include_directories(pilo_shared PUBLIC "${PILO_LIBSRC_DIR}/external/mysqlclient/8.0.19/include" "${PILO_LIBSRC_DIR}")
target_link_libraries(pilo_shared PUBLIC "ws2_32.lib" "")

string(REPLACE "/" "\\" PILO_LIBSRC_DIR_DOS ${PILO_LIBSRC_DIR})
string(REPLACE "/" "\\" PILO_OUTPUT_DIR_DOS ${PILO_OUTPUT_DIR})
string(REPLACE "/" "\\" LIBRARY_OUTPUT_PATH_DOS ${LIBRARY_OUTPUT_PATH})




add_custom_command(TARGET pilo_static POST_BUILD		
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pilo_static> ${LIBRARY_OUTPUT_PATH}/
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libpilo-static-debug.pdb copy ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libpilo-static-debug.pdb ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libpilo-static-release.pdb copy ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libpilo-static-release.pdb ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIBRARY_OUTPUT_PATH}/Debug
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIBRARY_OUTPUT_PATH}/Release
	COMMAND xcopy /Y /E /I "${PILO_LIBSRC_DIR_DOS}" "${PILO_OUTPUT_DIR_DOS}\\include\\pilo"
	COMMAND del /f /s "${PILO_OUTPUT_DIR_DOS}\\include\\pilo\\*.cpp"
)

add_custom_command(TARGET pilo_shared POST_BUILD		
	COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_FILE:pilo_shared> ${LIBRARY_OUTPUT_PATH}/
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libpilo-shared-debug.pdb copy ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libpilo-shared-debug.pdb ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libpilo-shared-release.pdb copy ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libpilo-shared-release.pdb ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libpilo-shared-debug.lib copy ${LIBRARY_OUTPUT_PATH_DOS}\\Debug\\libpilo-shared-debug.lib ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND if exist ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libpilo-shared-release.lib copy ${LIBRARY_OUTPUT_PATH_DOS}\\Release\\libpilo-shared-release.lib ${LIBRARY_OUTPUT_PATH_DOS}
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIBRARY_OUTPUT_PATH}/Debug
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${LIBRARY_OUTPUT_PATH}/Release
	COMMAND xcopy /Y /E /I "${PILO_LIBSRC_DIR_DOS}" "${PILO_OUTPUT_DIR_DOS}\\include\\pilo"
	COMMAND del /f /s "${PILO_OUTPUT_DIR_DOS}\\include\\pilo\\*.cpp"
)

set(PILO_LIBPILO_INCLUDE_DIR ${PILO_OUTPUT_DIR}/include)
set(PILO_LIBPILO_LIB_DIR ${PILO_OUTPUT_DIR}/lib)

# for function-test
set(PILO_FUNCTEST_DIR "${PILO_PROJECT_DIR}/src/pilo_func_test")
file(GLOB_RECURSE PILOFUNC_TEST_CPPS ${PILO_FUNCTEST_DIR}/*.cpp)
file(GLOB_RECURSE PILOFUNC_TEST_PPS ${PILO_FUNCTEST_DIR}/*.hpp)
file(GLOB_RECURSE PILOFUNC_TEST_HS ${PILO_FUNCTEST_DIR}/*.h)

#static func-test
add_executable(pilo_func_test-static ${PILOFUNC_TEST_CPPS} ${PILOFUNC_TEST_PPS} ${PILOFUNC_TEST_HS})
set_target_properties(pilo_func_test-static PROPERTIES OUTPUT_NAME "pilo_func_test-static")
set_target_properties(pilo_func_test-static PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_func_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_func_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_func_test-static PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)..\\..\\output\\win-vs17\\pilo")
source_group_by_dir(PILOFUNC_TEST_CPPS)
source_group_by_dir(PILOFUNC_TEST_PPS)
source_group_by_dir(PILOFUNC_TEST_HS)
target_include_directories(pilo_func_test-static PUBLIC "${PILO_PROJECT_DIR}/src")


target_link_libraries(pilo_func_test-static PUBLIC debug "libpilo-static-debug" optimized libpilo-static-release)
target_link_libraries(pilo_func_test-static PUBLIC "ws2_32.lib")
target_link_directories(pilo_func_test-static PUBLIC ${PILO_OUTPUT_LIB_DIR})

#dynamic func-test
add_executable(pilo_func_test-shared ${PILOFUNC_TEST_CPPS} ${PILOFUNC_TEST_PPS} ${PILOFUNC_TEST_HS})
set_target_properties(pilo_func_test-shared PROPERTIES OUTPUT_NAME "pilo_func_test-shared")
set_target_properties(pilo_func_test-shared PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_func_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_func_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_func_test-shared PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)..\\..\\output\\win-vs17\\pilo")
source_group_by_dir(PILOFUNC_TEST_CPPS)
source_group_by_dir(PILOFUNC_TEST_PPS)
source_group_by_dir(PILOFUNC_TEST_HS)
target_include_directories(pilo_func_test-shared PUBLIC "${PILO_PROJECT_DIR}/src")
target_link_libraries(pilo_func_test-shared PUBLIC debug "libpilo-shared-debug" optimized libpilo-shared-release)
target_link_libraries(pilo_func_test-shared PUBLIC "ws2_32.lib")
target_link_directories(pilo_func_test-shared PUBLIC ${PILO_OUTPUT_LIB_DIR})


#stable test
set(PILO_STABLETEST_DIR "${PILO_PROJECT_DIR}/src/pilo_stable_test")
file(GLOB_RECURSE PILOSTABLE_TEST_CPPS ${PILO_STABLETEST_DIR}/*.cpp)
file(GLOB_RECURSE PILOSTABLE_TEST_PPS ${PILO_STABLETEST_DIR}/*.hpp)
file(GLOB_RECURSE PILOSTABLE_TEST_HS ${PILO_STABLETEST_DIR}/*.h)
#static stable-test
add_executable(pilo_stable_test-static ${PILOSTABLE_TEST_CPPS} ${PILOSTABLE_TEST_PPS} ${PILOSTABLE_TEST_HS})
set_target_properties(pilo_stable_test-static PROPERTIES OUTPUT_NAME "pilo_stable_test-static")
set_target_properties(pilo_stable_test-static PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_stable_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_stable_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_stable_test-static PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)..\\..\\output\\win-vs17\\pilo")
source_group_by_dir(PILOSTABLE_TEST_CPPS)
source_group_by_dir(PILOSTABLE_TEST_PPS)
source_group_by_dir(PILOSTABLE_TEST_HS)
target_include_directories(pilo_stable_test-static PUBLIC "${PILO_PROJECT_DIR}/src")
target_link_libraries(pilo_stable_test-static PUBLIC debug "libpilo-static-debug" optimized libpilo-static-release)
target_link_libraries(pilo_stable_test-static PUBLIC "ws2_32.lib")
target_link_directories(pilo_stable_test-static PUBLIC ${PILO_OUTPUT_LIB_DIR})

#dynamic stable-test
add_executable(pilo_stable_test-shared ${PILOSTABLE_TEST_CPPS} ${PILOSTABLE_TEST_PPS} ${PILOSTABLE_TEST_HS})
set_target_properties(pilo_stable_test-shared PROPERTIES OUTPUT_NAME "pilo_stable_test-shared")
set_target_properties(pilo_stable_test-shared PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_stable_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_stable_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_stable_test-shared PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)..\\..\\output\\win-vs17\\pilo")
source_group_by_dir(PILOSTABLE_TEST_CPPS)
source_group_by_dir(PILOSTABLE_TEST_PPS)
source_group_by_dir(PILOSTABLE_TEST_HS)
target_include_directories(pilo_stable_test-shared PUBLIC "${PILO_PROJECT_DIR}/src")
target_link_libraries(pilo_stable_test-shared PUBLIC debug "libpilo-shared-debug" optimized libpilo-shared-release)
target_link_libraries(pilo_stable_test-shared PUBLIC "ws2_32.lib")
target_link_directories(pilo_stable_test-shared PUBLIC ${PILO_OUTPUT_LIB_DIR})


#performance test
set(PILO_PERFORMANCETEST_DIR "${PILO_PROJECT_DIR}/src/pilo_performance_test")
file(GLOB_RECURSE PILOPERFORMANCE_TEST_CPPS ${PILO_PERFORMANCETEST_DIR}/*.cpp)
file(GLOB_RECURSE PILOPERFORMANCE_TEST_PPS ${PILO_PERFORMANCETEST_DIR}/*.hpp)
file(GLOB_RECURSE PILOPERFORMANCE_TEST_HS ${PILO_PERFORMANCETEST_DIR}/*.h)
#static performance-test
add_executable(pilo_performance_test-static ${PILOPERFORMANCE_TEST_CPPS} ${PILOPERFORMANCE_TEST_PPS} ${PILOPERFORMANCE_TEST_HS})
set_target_properties(pilo_performance_test-static PROPERTIES OUTPUT_NAME "pilo_performance_test-static")
set_target_properties(pilo_performance_test-static PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_performance_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_performance_test-static PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_performance_test-static PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)..\\..\\output\\win-vs17\\pilo")
source_group_by_dir(PILOPERFORMANCE_TEST_CPPS)
source_group_by_dir(PILOPERFORMANCE_TEST_PPS)
source_group_by_dir(PILOPERFORMANCE_TEST_HS)
target_include_directories(pilo_performance_test-static PUBLIC "${PILO_PROJECT_DIR}/src")
target_link_libraries(pilo_performance_test-static PUBLIC debug "libpilo-static-debug" optimized libpilo-static-release)
target_link_libraries(pilo_performance_test-static PUBLIC "ws2_32.lib")
target_link_directories(pilo_performance_test-static PUBLIC ${PILO_OUTPUT_LIB_DIR})

#dynamic performance-test
add_executable(pilo_performance_test-shared ${PILOPERFORMANCE_TEST_CPPS} ${PILOPERFORMANCE_TEST_PPS} ${PILOPERFORMANCE_TEST_HS})
set_target_properties(pilo_performance_test-shared PROPERTIES OUTPUT_NAME "pilo_performance_test-shared")
set_target_properties(pilo_performance_test-shared PROPERTIES DEBUG_POSTFIX "-debug" RELEASE_POSTFIX "-release")
set_target_properties(pilo_performance_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_performance_test-shared PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE ${PILO_OUTPUT_DIR}/bin)
set_target_properties(pilo_performance_test-shared PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "$(ProjectDir)..\\..\\output\\win-vs17\\pilo")
source_group_by_dir(PILOPERFORMANCE_TEST_CPPS)
source_group_by_dir(PILOPERFORMANCE_TEST_PPS)
source_group_by_dir(PILOPERFORMANCE_TEST_HS)
target_include_directories(pilo_performance_test-shared PUBLIC "${PILO_PROJECT_DIR}/src")
target_link_libraries(pilo_performance_test-shared PUBLIC debug "libpilo-shared-debug" optimized libpilo-shared-release)
target_link_libraries(pilo_performance_test-shared PUBLIC "ws2_32.lib")
target_link_directories(pilo_performance_test-shared PUBLIC ${PILO_OUTPUT_LIB_DIR})





