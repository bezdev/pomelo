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

set(SHADER_FOLDER ../src/shaders)
set(DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/shaders")
get_filename_component(SHADER_FOLDER_ABSOLUTE "${SHADER_FOLDER}" ABSOLUTE)
add_custom_target(CopyShaders ALL)
add_custom_command(
    TARGET CopyShaders
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${SHADER_FOLDER_ABSOLUTE} ${DESTINATION}
)

set(ASSET_FOLDER "../assets/")
set(DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets")
get_filename_component(ASSET_FOLDER_ABSOLUTE "${ASSET_FOLDER}" ABSOLUTE)
add_custom_target(CopyAssets ALL)
add_custom_command(
    TARGET CopyAssets
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSET_FOLDER_ABSOLUTE} ${DESTINATION}
)
set(DESTINATION "${CMAKE_CURRENT_LIST_DIR}/../android/app/src/main/assets/assets")
get_filename_component(ASSET_FOLDER_ABSOLUTE "${ASSET_FOLDER}" ABSOLUTE)
add_custom_target(CopyAssetsAndroid ALL)
add_custom_command(
    TARGET CopyAssetsAndroid
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${ASSET_FOLDER_ABSOLUTE} ${DESTINATION}
)

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
