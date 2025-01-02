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
    ${SOURCES_BASE_PATH}/vendor/JoltPhysics
)

set(USE_SSE4_1 OFF CACHE BOOL "Disable SSE4.1")
set(USE_SSE4_2 OFF CACHE BOOL "Disable SSE4.2")
set(USE_AVX OFF CACHE BOOL "Disable AVX")
set(USE_AVX2 OFF CACHE BOOL "Disable AVX2")
set(USE_AVX512 OFF CACHE BOOL "Disable AVX512")
set(USE_LZCNT OFF CACHE BOOL "Disable LZCNT")
set(USE_TZCNT OFF CACHE BOOL "Disable TZCNT")
set(USE_F16C OFF CACHE BOOL "Disable F16C")
set(USE_FMADD OFF CACHE BOOL "Disable FMADD")

find_library(log-lib log)
find_library(gl-v3 GLESv3)

target_link_libraries(
    pomelo-native
    EGL
    ${gl-v3}
    android
    native_app_glue
    Jolt
    ${log-lib}
)
