# TODO:
# file(GLOB TEST_FILES "${TEST_FOLDER}/*")
# file(GLOB EXCLUDED_FILES "${ASSET_FOLDER}/results")
# list(REMOVE_ITEM ASSET_FILES ${EXCLUDED_FILES})
# foreach(asset_file ${ASSET_FILES})
#     get_filename_component(asset_filename ${asset_file} NAME)
#     configure_file(${asset_file} ${CMAKE_CURRENT_BINARY_DIR}/assets/${asset_filename} COPYONLY)
# endforeach()

add_executable(pomelotest
    ${TEST_FOLDER}/Test.cpp
    ${TEST_FOLDER}/PerformanceTest.cpp
    ${TEST_FOLDER}/UtilTest.cpp
    ${TEST_FOLDER}/TimerTest.cpp
    ${TEST_FOLDER}/LoggerTest.cpp
    ${TEST_FOLDER}/ObjectPoolTest.cpp
    ${TEST_FOLDER}/ECSTest.cpp
    ${TEST_FOLDER}/SandboxTest.cpp
    ${TEST_FOLDER}/MathTest.cpp
    ${TEST_FOLDER}/ShaderTest.cpp
    ${TEST_FOLDER}/main.cpp
)

target_link_libraries(
    pomelotest
    pomelo
)
