cmake_minimum_required(VERSION 3.10.2)

project("fame")

set(BUILD_ANDROID 1)

include(${CMAKE_CURRENT_LIST_DIR}/../../../../../src/cpp/CMakeFameSrc.txt)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../../../src/cpp)

add_library(native_app_glue
    STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

file(GLOB SOURCES CONFIGURE_DEPENDS "\\.cpp")
#message("bez : " ${SOURCES})

add_library(fame-native
    SHARED
    main.cpp
    ${SOURCE_FILES}
)

target_include_directories(fame-native
    PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue
)

find_library(
    log-lib
    log
)

target_link_libraries(
    fame-native
    GLESv2
    EGL
    android
    native_app_glue
    ${log-lib}
)
