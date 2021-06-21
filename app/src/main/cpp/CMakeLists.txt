cmake_minimum_required(VERSION 3.10.2)

project("fame")

add_library(native_app_glue
    STATIC
    ${ANDROID_NDK}/sources/android/native_app_glue/android_native_app_glue.c
)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++11 -Wall")

add_library(fame-native
    SHARED
    main.cpp
)

find_library(
    log-lib
    log
)

set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")

target_include_directories(fame-native
    PRIVATE
    ${ANDROID_NDK}/sources/android/native_app_glue
)

target_link_libraries(
    fame-native
    android
    native_app_glue
    ${log-lib}
)
