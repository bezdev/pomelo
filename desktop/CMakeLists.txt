cmake_minimum_required (VERSION 3.8)

project ("fame")

set(BUILD_DESKTOP 1)
include(${CMAKE_CURRENT_LIST_DIR}/../src/cpp/CMakeFameSrc.txt)

include_directories(${CMAKE_CURRENT_LIST_DIR}/inc)
include_directories(${CMAKE_CURRENT_LIST_DIR}/../src/cpp)

#file(GLOB SOURCES CONFIGURE_DEPENDS "\\.cpp")

#set(ASSET_FOLDER ../android/app/src/main/assets)
set(ASSET_FOLDER ../src/assets)
configure_file(${ASSET_FOLDER}/shaders/SolidColor.fs ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColor.fs COPYONLY)
configure_file(${ASSET_FOLDER}/shaders/SolidColor.vs ${CMAKE_CURRENT_BINARY_DIR}/shaders/SolidColor.vs COPYONLY)

add_library(GLAD "glad.c")

add_executable(fame
    main.cpp
    ${SOURCE_FILES}
)

find_library(GLFW_LIB
    glfw3
    HINTS "${CMAKE_CURRENT_LIST_DIR}/lib/GLFW"
)

link_libraries(glfw)

target_link_libraries(fame
    ${GLFW_LIB}
    opengl32
    GLAD
)
