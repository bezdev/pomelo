cmake_minimum_required (VERSION 3.8)

project ("pomelo")

if (APPLE)
    set(CMAKE_C_COMPILER "gcc-12")
    set(CMAKE_CXX_COMPILER "g++-12")
endif()

set(BUILD_DESKTOP 1)
set(BUILD_TEST 1)

set(CMAKE_BUILD_TYPE Debug)
add_compile_options(-Wall)
# add_compile_options(-Wextra)

set(TEST_FOLDER ${CMAKE_CURRENT_LIST_DIR}/../test)

include(${CMAKE_CURRENT_LIST_DIR}/../src/cpp/CMakePomeloSrc.txt)

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../src/cpp)
include_directories(${TEST_FOLDER})

#file(GLOB SOURCES CONFIGURE_DEPENDS "\\.cpp")

set(ASSET_FOLDER ../src/assets)
configure_file(${ASSET_FOLDER}/shaders/SolidColor.fs ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColor.fs COPYONLY)
configure_file(${ASSET_FOLDER}/shaders/SolidColor.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColor.vs COPYONLY)
configure_file(${ASSET_FOLDER}/shaders/SolidColorInstanced.ps ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColorInstanced.ps COPYONLY)
configure_file(${ASSET_FOLDER}/shaders/SolidColorInstanced.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColorInstanced.vs COPYONLY)
configure_file(${ASSET_FOLDER}/shaders/PixelColor.ps ${CMAKE_CURRENT_BINARY_DIR}/shaders/PixelColor.ps COPYONLY)
configure_file(${ASSET_FOLDER}/shaders/PixelColor.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/PixelColor.vs COPYONLY)

add_library(GLAD "glad.c")

add_library(pomelo ${SOURCE_FILES})
add_executable(game main.cpp)

if (WIN32)
    find_library(GLFW_LIB
        glfw3
        HINTS "${CMAKE_CURRENT_LIST_DIR}/lib/GLFW"
    )

    target_link_libraries(pomelo ${GLFW_LIB} opengl32 GLAD)
    target_link_libraries(game ${GLFW_LIB} opengl32 GLAD pomelo)
elseif (APPLE)
    find_library(GLFW_LIB
        glfw3
        HINTS "${CMAKE_CURRENT_LIST_DIR}/lib/mac/GLFW"
    )
    find_library(COCOA_LIB Cocoa)
    find_library(IOKIT_LIB IOKit)

    target_link_libraries(pomelo ${GLFW_LIB} ${COCOA_LIB} ${IOKIT_LIB} GLAD)
    target_link_libraries(game pomelo)
endif()

if(DEFINED BUILD_TEST)
    include(${TEST_FOLDER}/CMakeLists.txt)
endif()
