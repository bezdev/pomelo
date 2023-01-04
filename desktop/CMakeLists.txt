cmake_minimum_required (VERSION 3.8)

project ("pomelo")

set(BUILD_DESKTOP 1)
set(BUILD_TEST 1)

set(TEST_FOLDER ${CMAKE_CURRENT_LIST_DIR}/../test)

include(${CMAKE_CURRENT_LIST_DIR}/../src/cpp/CMakepomeloSrc.txt)

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../src/cpp)
include_directories(${TEST_FOLDER})

#file(GLOB SOURCES CONFIGURE_DEPENDS "\\.cpp")

set(ASSET_FOLDER ../src/assets)
configure_file(${ASSET_FOLDER}/shaders/SolidColor.fs ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColor.fs COPYONLY)
configure_file(${ASSET_FOLDER}/shaders/SolidColor.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColor.vs COPYONLY)

add_library(GLAD
    "glad.c"
)

add_library(pomelo
    ${SOURCE_FILES}
)

add_executable(game
    main.cpp
)

find_library(GLFW_LIB
    glfw3
    HINTS "${CMAKE_CURRENT_LIST_DIR}/lib/GLFW"
)

target_link_libraries(pomelo
    ${GLFW_LIB}
    opengl32
    GLAD
)

target_link_libraries(game
    ${GLFW_LIB}
    opengl32
    GLAD
    pomelo
)

if(DEFINED BUILD_TEST)
    include(${TEST_FOLDER}/CMakeLists.txt)
endif()
