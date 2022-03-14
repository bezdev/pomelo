cmake_minimum_required (VERSION 3.8)

project ("fame")

set(BUILD_DESKTOP 1)
set(BUILD_TEST 1)

include(${CMAKE_CURRENT_LIST_DIR}/../src/cpp/CMakeFameSrc.txt)

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../src/cpp)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../test)

#file(GLOB SOURCES CONFIGURE_DEPENDS "\\.cpp")

set(ASSET_FOLDER ../src/assets)
configure_file(${ASSET_FOLDER}/shaders/SolidColor.fs ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColor.fs COPYONLY)
configure_file(${ASSET_FOLDER}/shaders/SolidColor.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColor.vs COPYONLY)

add_library(GLAD "glad.c")
add_library(fame
    ${SOURCE_FILES}
)

add_executable(game
    main.cpp
)

find_library(GLFW_LIB
    glfw3
    HINTS "${CMAKE_CURRENT_LIST_DIR}/lib/GLFW"
)

target_link_libraries(fame
    ${GLFW_LIB}
    opengl32
    GLAD
)

target_link_libraries(game
    ${GLFW_LIB}
    opengl32
    GLAD
    fame
)

if(DEFINED BUILD_TEST)
    set(TEST_FOLDER ${CMAKE_CURRENT_LIST_DIR}/../test)
    include(${CMAKE_CURRENT_LIST_DIR}/../test/CMakeLists.txt)
endif()
