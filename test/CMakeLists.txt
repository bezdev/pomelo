file(GLOB TEST_FILES "${TEST_FOLDER}/*")
file(GLOB EXCLUDED_FILES "${TEST_FOLDER}/results")
list(REMOVE_ITEM TEST_FILES ${EXCLUDED_FILES})

##
# file(GLOB EXCLUDED_FILES "${TEST_FOLDER}/PerformanceTest.cpp")
# list(REMOVE_ITEM TEST_FILES ${EXCLUDED_FILES})
##


add_executable(pomelotest ${TEST_FILES})

set_target_properties(pomelotest PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED YES CXX_EXTENSIONS NO)

target_include_directories(pomelotest SYSTEM PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../desktop/inc)

target_link_libraries(
    pomelotest
    pomelo
)
