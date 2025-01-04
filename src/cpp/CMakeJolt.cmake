set(JOLT_BASE_PATH ${SOURCES_BASE_PATH}/vendor/JoltPhysics)

file(GLOB_RECURSE JOLT_SOURCES
    ${JOLT_BASE_PATH}/Jolt/**.cpp
    ${JOLT_BASE_PATH}/Jolt/**.h
    ${JOLT_BASE_PATH}/Jolt/**.inl
)

# Define the Jolt library
add_library(Jolt STATIC ${JOLT_SOURCES})

# Include directories for Jolt
target_include_directories(Jolt PUBLIC ${JOLT_BASE_PATH}/Jolt)
target_include_directories(Jolt PUBLIC ${JOLT_BASE_PATH})

# Enable C++17 features
target_compile_features(Jolt PRIVATE cxx_std_17)
