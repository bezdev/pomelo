if(DEFINED BUILD_ANDROID)
    add_definitions(-DBUILD_ANDROID)
endif()

if(DEFINED BUILD_DESKTOP)
    add_definitions(-DBUILD_DESKTOP)
endif()

if(DEFINED BUILD_TEST)
    add_definitions(-DBUILD_TEST)
endif()

set(SOURCES_BASE_PATH ${CMAKE_CURRENT_LIST_DIR})

set(SOURCE_FILES
    ${SOURCES_BASE_PATH}/app/App.cpp
    ${SOURCES_BASE_PATH}/engine/InputManager.cpp
    ${SOURCES_BASE_PATH}/engine/PhysicsEngine.cpp
    ${SOURCES_BASE_PATH}/engine/CollisionEngine.cpp
    ${SOURCES_BASE_PATH}/render/Renderer.cpp
    ${SOURCES_BASE_PATH}/render/GUI.cpp
    ${SOURCES_BASE_PATH}/render/Shader.cpp
    ${SOURCES_BASE_PATH}/render/RenderBuffer.cpp
    ${SOURCES_BASE_PATH}/render/Texture.cpp
    ${SOURCES_BASE_PATH}/render/Text.cpp
    ${SOURCES_BASE_PATH}/util/Timer.cpp
    ${SOURCES_BASE_PATH}/engine/Camera.cpp
    ${SOURCES_BASE_PATH}/engine/Mesh.cpp
    ${SOURCES_BASE_PATH}/engine/Scene.cpp
    ${SOURCES_BASE_PATH}/engine/ECS.cpp
    ${SOURCES_BASE_PATH}/engine/Font.cpp
    ${SOURCES_BASE_PATH}/util/Logger.cpp
    ${SOURCES_BASE_PATH}/util/Util.cpp
    ${SOURCES_BASE_PATH}/util/Math.cpp
    ${SOURCES_BASE_PATH}/util/Color.cpp
)

if(DEFINED BUILD_ANDROID)
    set(SOURCE_FILES
        ${SOURCE_FILES}
        ${SOURCES_BASE_PATH}/app/AndroidApp.cpp
        ${SOURCES_BASE_PATH}/util/jniutil.cpp
    )
endif()

if(DEFINED BUILD_DESKTOP)
    set(SOURCE_FILES
        ${SOURCE_FILES}
        ${SOURCES_BASE_PATH}/app/GLFWApp.cpp
    )
endif()

include(${SOURCES_BASE_PATH}/CMakeJolt.cmake)
