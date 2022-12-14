cmake_minimum_required(VERSION 3.5)
project(gdu_sdk_demo_linux C)

set(CMAKE_C_FLAGS "-pthread -std=gnu99 -g ")
#set(CMAKE_C_FLAGS "-pthread -std=gnu99 ")
set(CMAKE_EXE_LINKER_FLAGS "-pthread")
set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
add_definitions(-D_GNU_SOURCE)

if (NOT USE_SYSTEM_ARCH)
    add_definitions(-DSYSTEM_ARCH_LINUX)
endif ()

set(COMMON_CXX_FLAGS "-std=c++11 -pthread")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage -lgcov")

set(PACKAGE_NAME payloadsdk)

## "uname -m" to auto distinguish Manifold2-G or Manifold2-C
execute_process(COMMAND uname -m
        OUTPUT_VARIABLE DEVICE_SYSTEM_ID)

if (DEVICE_SYSTEM_ID MATCHES x86_64)
    set(TOOLCHAIN_NAME x86_64-linux-gnu-gcc)
    add_definitions(-DPLATFORM_ARCH_x86_64=1)
elseif (DEVICE_SYSTEM_ID MATCHES aarch64)
    set(TOOLCHAIN_NAME aarch64-linux-gnu-gcc)
    add_definitions(-DPLATFORM_ARCH_aarch64=1)
else ()
    message(FATAL_ERROR "FATAL: Please confirm your platform.")
endif ()

file(GLOB_RECURSE MODULE_COMMON_SRC ../common/*.c)
file(GLOB_RECURSE MODULE_HAL_SRC hal/*.c)
file(GLOB_RECURSE MODULE_APP_SRC application/*.c)
#file(GLOB_RECURSE MODULE_SAMPLE_SRC ../../../module_sample/*.c)
file(GLOB_RECURSE MODULE_SAMPLE_SRC 
	#../../../module_sample/*.c
	../../../module_sample/camera_emu/*.c
	../../../module_sample/gimbal_emu/*.c
	../../../module_sample/positioning/*c
	../../../module_sample/time_sync/*c
	../../../module_sample/utils/*c
	../../../module_sample/fc_subscription/*c
	)

#file(GLOB_RECURSE MODULE_SAMPLE_MEDIA ../../../../../psdk_lib/media/*.c)
file(GLOB_RECURSE LIB_SRC ../../../../../psdk_lib/lib/*.c)

include_directories(../../../module_sample)
include_directories(../common)

#include_directories(../../../../../psdk_lib/media)
include_directories(../../../../../psdk_lib/media/lib_include)
include_directories(../../../../../samples/sample_c/module_sample)
include_directories(../../../../../psdk_lib/include)
include_directories(../../../../../glink)
include_directories(../../../../../glink/glink_m)
include_directories(../../../../../glink/glink_np)
#link_directories(../../../../../psdk_lib/lib/${TOOLCHAIN_NAME})

#link_libraries(${CMAKE_CURRENT_LIST_DIR}/../../../../../psdk_lib/lib/${TOOLCHAIN_NAME}/lib${PACKAGE_NAME}.a)

if (NOT EXECUTABLE_OUTPUT_PATH)
    set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
endif ()

#???????????????
#link_directories(../../../../../psdk_lib/media/lib)

add_executable(${PROJECT_NAME}
        ${MODULE_APP_SRC}
        ${MODULE_SAMPLE_SRC}
        ${MODULE_COMMON_SRC}
 #       ${MODULE_SAMPLE_MEDIA}
        ${MODULE_HAL_SRC}
        ${LIB_SRC})

#target_link_libraries(${PROJECT_NAME} m usb-1.0 mk_api)
target_link_libraries(${PROJECT_NAME} m usb-1.0)

add_custom_command(TARGET ${PROJECT_NAME}
        PRE_LINK COMMAND cmake ..
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
