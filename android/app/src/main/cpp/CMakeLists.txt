cmake_minimum_required(VERSION 3.10.2)

project("pomelo")

set(BUILD_ANDROID 1)
include(${CMAKE_CURRENT_LIST_DIR}/../../../../../src/cpp/CMakePomeloSrc.txt)

include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../../../src/cpp)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../../../../../desktop/inc)

add_library(native_app_glue
    STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wall")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

file(GLOB SOURCES CONFIGURE_DEPENDS "\\.cpp")

add_library(pomelo-native
    SHARED
    main.cpp
    ${SOURCE_FILES}
)

target_include_directories(pomelo-native
    PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue
)

find_library(log-lib log)
find_library(gl-v3 GLESv3)

target_link_libraries(
    pomelo-native
    EGL
    ${gl-v3}
    android
    native_app_glue
    ${log-lib}
)
