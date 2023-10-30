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

set(SHADER_FOLDER ../src/shaders)
file(GLOB SHADER_FILES "${SHADER_FOLDER}/*")
foreach(shader_file ${SHADER_FILES})
    get_filename_component(shader_filename ${shader_file} NAME)
    configure_file(${shader_file} ${CMAKE_CURRENT_BINARY_DIR}/shaders/${shader_filename} COPYONLY)
endforeach()

# TODO: change this to build action
set(ASSET_FOLDER "../assets/")
set(DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/assets")
get_filename_component(ASSET_FOLDER_ABSOLUTE "${ASSET_FOLDER}" ABSOLUTE)
# message(STATUS "ASSET_FOLDER_ABSOLUTE = ${ASSET_FOLDER_ABSOLUTE}")
file(GLOB_RECURSE ASSET_FILES "${ASSET_FOLDER_ABSOLUTE}/*")
file(GLOB_RECURSE EXCLUDED_FILES "${ASSET_FOLDER_ABSOLUTE}/blender/*")
list(REMOVE_ITEM ASSET_FILES ${EXCLUDED_FILES})
# message(STATUS "ASSET_FILES= ${ASSET_FILES}")

foreach(asset_file ${ASSET_FILES})
    get_filename_component(RELATIVE_PATH "${asset_file}" DIRECTORY)
    file(RELATIVE_PATH RELATIVE_DIR "${ASSET_FOLDER_ABSOLUTE}" "${RELATIVE_PATH}")
    get_filename_component(FILE_NAME "${asset_file}" NAME)
    set(DESTINATION_FILE "${DESTINATION}/${RELATIVE_DIR}/${FILE_NAME}")
    get_filename_component(DESTINATION_DIR "${DESTINATION_FILE}" DIRECTORY)
    message(STATUS "DESTINATION_DIR for ${asset_file} = ${DESTINATION_DIR}")
    file(MAKE_DIRECTORY "${DESTINATION_DIR}")
    configure_file(${asset_file} ${DESTINATION_FILE} COPYONLY)
endforeach()

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
